#include "eve/bios/runtime.hpp"
#include "eve/brainstem/brainstem.hpp"
#include "eve/neural/network.hpp"
#include "eve/types.hpp"

#include <algorithm>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

class TestFailure : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

void require(const bool condition, const std::string& message) {
    if (!condition) {
        throw TestFailure(message);
    }
}

template <typename Left, typename Right>
void require_equal(const Left& left, const Right& right, const std::string& message) {
    if (!(left == right)) {
        throw TestFailure(message);
    }
}

eve::ExperimentConfig small_config() {
    eve::ExperimentConfig config;
    config.neuron_count = 8;
    config.width = 2;
    config.height = 2;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.growth_enabled = false;
    return config;
}

eve::ExperimentConfig motor_test_config() {
    eve::ExperimentConfig config;
    config.initial_outgoing_synapses = 0;
    config.learning_enabled = false;
    config.growth_enabled = false;
    return config;
}

eve::brainstem::MotorOutputConfig motor_output_config(
    const std::uint32_t contacts = 1,
    const std::uint64_t seed = 20261118,
    const eve::NeuronId first_motor_neuron = 9) {
    eve::brainstem::MotorOutputConfig config;
    config.contacts_per_channel = contacts;
    config.assignment_seed = seed;
    config.response_stimulus_neuron = 8;
    config.first_motor_neuron = first_motor_neuron;
    return config;
}

void test_network_initialization() {
    eve::bios::Runtime runtime({}, 42);
    require_equal(runtime.network().neuron_count(), 256U, "expected 256 neurons");
    require_equal(runtime.network().synapse_count(), 1024U, "expected 1024 synapses");
    for (eve::NeuronId id = 0; id < runtime.network().neuron_count(); ++id) {
        require_equal(
            runtime.network().neuron(id).outgoing_count,
            4U,
            "every neuron must start with four outgoing synapses");
    }
}

void test_duplicate_and_self_connections() {
    eve::neural::Network network;
    auto config = small_config();
    network.reset(config);
    static_cast<void>(network.add_synapse(0, 1, 0));
    bool duplicate_rejected = false;
    try {
        static_cast<void>(network.add_synapse(0, 1, 0));
    } catch (const std::invalid_argument&) {
        duplicate_rejected = true;
    }
    require(duplicate_rejected, "duplicate connection was accepted");

    bool self_rejected = false;
    try {
        static_cast<void>(network.add_synapse(2, 2, 0));
    } catch (const std::invalid_argument&) {
        self_rejected = true;
    }
    require(self_rejected, "self-connection was accepted");
    require(network.validate(), "network invariants failed");
}

void test_3d_index_calculation() {
    eve::neural::Network network;
    eve::ExperimentConfig config;
    config.initial_outgoing_synapses = 0;
    network.reset(config);
    require_equal(network.coordinate(0), eve::Coordinate{0, 0, 0}, "coordinate 0");
    require_equal(network.coordinate(7), eve::Coordinate{7, 0, 0}, "coordinate 7");
    require_equal(network.coordinate(8), eve::Coordinate{0, 1, 0}, "coordinate 8");
    require_equal(network.coordinate(63), eve::Coordinate{7, 7, 0}, "coordinate 63");
    require_equal(network.coordinate(64), eve::Coordinate{0, 0, 1}, "coordinate 64");
    require_equal(network.coordinate(255), eve::Coordinate{7, 7, 3}, "coordinate 255");
}

void test_binary_fire_forwarding() {
    auto config = small_config();
    config.initial_outgoing_synapses = 2;
    config.learning_enabled = false;
    eve::bios::Runtime runtime(config, 7);
    runtime.enqueue_fire(0);
    const std::uint64_t processed = runtime.run();
    require_equal(processed, 3ULL, "one fire plus two impulses expected");
    require_equal(runtime.metrics().fire_events, 1ULL, "one fire event expected");
    require_equal(
        runtime.metrics().transmitted_impulses, 2ULL, "same binary event must reach all outputs");
}

void test_weighted_input_processing() {
    auto config = small_config();
    config.firing_threshold = 10;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId synapse = network.add_synapse(0, 1, 2);
    const eve::brainstem::ImpulseResult result =
        eve::brainstem::Brainstem{}.receive_impulse(network, synapse, config);
    require_equal(result.applied_weight, 2, "weight two should be applied");
    require_equal(network.neuron(1).membrane_potential, 2LL, "membrane should increase by weight");
    require(!result.fired, "threshold ten must not fire at membrane two");
}

void test_strengthening_after_three_impulses() {
    auto config = small_config();
    config.firing_threshold = 100;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId synapse = network.add_synapse(0, 1, 0);
    eve::brainstem::Brainstem brainstem;
    require(!brainstem.receive_impulse(network, synapse, config).strengthened, "impulse one");
    require(!brainstem.receive_impulse(network, synapse, config).strengthened, "impulse two");
    require(brainstem.receive_impulse(network, synapse, config).strengthened, "impulse three");
    require_equal(network.synapse(synapse).weight, 1, "weight should increase to one");
    require_equal(network.synapse(synapse).activity_count, 0U, "learning counter should reset");
}

void test_phase_reset_after_eight_impulses() {
    auto config = small_config();
    config.learning_enabled = false;
    config.firing_threshold = 100;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId first = network.add_synapse(0, 2, 0);
    const eve::SynapseId second = network.add_synapse(1, 2, 0);
    eve::brainstem::Brainstem brainstem;
    for (int index = 0; index < 5; ++index) {
        static_cast<void>(brainstem.receive_impulse(network, first, config));
    }
    eve::brainstem::ImpulseResult last;
    for (int index = 0; index < 3; ++index) {
        last = brainstem.receive_impulse(network, second, config);
    }
    require(last.phase_completed, "eighth input must complete the phase");
    require_equal(network.neuron(2).input_event_count, 0U, "neuron counter must reset");
    require_equal(network.synapse(first).activity_count, 0U, "first synapse counter must reset");
    require_equal(network.synapse(second).activity_count, 0U, "second synapse counter must reset");
}

void test_learning_precedes_phase_completion() {
    auto config = small_config();
    config.firing_threshold = 100;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId selected = network.add_synapse(0, 2, 0);
    const eve::SynapseId other = network.add_synapse(1, 2, 0);
    eve::brainstem::Brainstem brainstem;
    for (int index = 0; index < 2; ++index) {
        static_cast<void>(brainstem.receive_impulse(network, selected, config));
    }
    for (int index = 0; index < 5; ++index) {
        static_cast<void>(brainstem.receive_impulse(network, other, config));
    }
    const auto result = brainstem.receive_impulse(network, selected, config);
    require(result.strengthened, "eighth impulse must strengthen first");
    require(result.phase_completed, "eighth impulse must then close the phase");
    require_equal(network.synapse(selected).weight, 1, "strengthening must survive phase reset");
    require_equal(result.applied_weight, 1, "new weight must affect the same impulse");
}

void test_learning_from_zero_weight() {
    auto config = small_config();
    config.firing_threshold = 100;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId synapse = network.add_synapse(0, 1, 0);
    eve::brainstem::Brainstem brainstem;
    for (int index = 0; index < 3; ++index) {
        static_cast<void>(brainstem.receive_impulse(network, synapse, config));
    }
    require_equal(network.synapse(synapse).weight, 1, "zero-weight synapse failed to learn");
    require_equal(network.neuron(1).membrane_potential, 1LL, "third impulse uses learned weight");
}

void test_maximum_synapse_weight() {
    auto config = small_config();
    config.firing_threshold = 100;
    config.synaptic_learning_threshold = 1;
    config.maximum_synapse_weight = 1;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId synapse = network.add_synapse(0, 1, 1);
    const auto result = eve::brainstem::Brainstem{}.receive_impulse(network, synapse, config);
    require(!result.strengthened, "capped synapse must not be strengthened");
    require(result.strengthening_rejected_by_weight_limit,
            "rejected strengthening was not reported");
    require_equal(network.synapse(synapse).weight, 1, "maximum weight was exceeded");
    require_equal(network.synapse(synapse).activity_count, 0U, "learning counter did not reset");
}

void test_combined_refractory_and_weight_limit() {
    auto config = small_config();
    config.firing_threshold = 1;
    config.synaptic_learning_threshold = 1;
    config.activity_phase_limit = 3;
    config.maximum_synapse_weight = 1;
    config.refractory_input_events = 2;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId synapse = network.add_synapse(0, 1, 0);
    eve::brainstem::Brainstem brainstem;

    const auto first = brainstem.receive_impulse(network, synapse, config);
    require(first.strengthened && first.fired, "combined first impulse should learn and fire");
    const auto second = brainstem.receive_impulse(network, synapse, config);
    const auto third = brainstem.receive_impulse(network, synapse, config);
    require(second.firing_suppressed && third.firing_suppressed,
            "combined refractory phase did not suppress firing");
    require(second.strengthening_rejected_by_weight_limit &&
                third.strengthening_rejected_by_weight_limit,
            "combined weight limit did not reject strengthening");
    require(third.phase_completed, "combined phase did not complete");
    require_equal(network.synapse(synapse).weight, 1,
                  "phase completion bypassed combined weight limit");
    const auto fourth = brainstem.receive_impulse(network, synapse, config);
    require(fourth.fired, "combined neuron did not fire after refractory phase");
}

void test_input_weight_budget_free_capacity() {
    auto config = small_config();
    config.firing_threshold = 100;
    config.synaptic_learning_threshold = 1;
    config.input_weight_budget = 2;
    eve::neural::Network network;
    network.reset(config);
    static_cast<void>(network.add_synapse(0, 3, 1));
    const eve::SynapseId learning = network.add_synapse(1, 3, 0);
    eve::bios::DeterministicRng rng(42);
    const auto result =
        eve::brainstem::Brainstem{}.receive_impulse(network, learning, config, &rng);
    require(result.strengthened, "free input budget should allow strengthening");
    require(!result.weakened, "free input budget must not weaken another synapse");
    require_equal(network.synapse(learning).weight, 1, "learning weight did not increase");
}

void test_input_weight_budget_redistribution() {
    const auto execute = []() {
        auto config = small_config();
        config.firing_threshold = 100;
        config.synaptic_learning_threshold = 1;
        config.input_weight_budget = 2;
        eve::neural::Network network;
        network.reset(config);
        const eve::SynapseId first = network.add_synapse(0, 3, 1);
        const eve::SynapseId second = network.add_synapse(1, 3, 1);
        const eve::SynapseId learning = network.add_synapse(2, 3, 0);
        eve::bios::DeterministicRng rng(99);
        const auto result =
            eve::brainstem::Brainstem{}.receive_impulse(network, learning, config, &rng);
        require(result.strengthened && result.weakened, "budget must redistribute weight");
        require_equal(network.synapse(learning).weight, 1, "learning synapse did not gain weight");
        require(
            network.synapse(first).weight + network.synapse(second).weight == 1,
            "exactly one competing synapse must lose weight");
        require(
            network.synapse(first).weight >= 0 && network.synapse(second).weight >= 0,
            "competition created a negative weight");
        return network.state_fingerprint();
    };
    require_equal(execute(), execute(), "same competition seed selected a different synapse");
}

void test_input_weight_budget_without_competitor() {
    auto config = small_config();
    config.firing_threshold = 100;
    config.synaptic_learning_threshold = 1;
    config.input_weight_budget = 1;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId learning = network.add_synapse(0, 1, 1);
    eve::bios::DeterministicRng rng(7);
    const auto result =
        eve::brainstem::Brainstem{}.receive_impulse(network, learning, config, &rng);
    require(!result.strengthened && !result.weakened, "source synapse was its own competitor");
    require_equal(network.synapse(learning).weight, 1, "weight changed without a competitor");
}

void test_refractory_events_keep_processing_and_learning() {
    auto config = small_config();
    config.firing_threshold = 3;
    config.synaptic_learning_threshold = 1;
    config.maximum_synapse_weight = 10;
    config.refractory_input_events = 2;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId synapse = network.add_synapse(0, 1, 2);
    eve::brainstem::Brainstem brainstem;

    const auto first = brainstem.receive_impulse(network, synapse, config);
    require(first.fired, "first learned impulse should fire");
    const auto second = brainstem.receive_impulse(network, synapse, config);
    const auto third = brainstem.receive_impulse(network, synapse, config);
    require(!second.fired && !third.fired, "neuron fired during refractory events");
    require(second.firing_suppressed && third.firing_suppressed, "suppression was not reported");
    require(second.strengthened && third.strengthened, "learning stopped during refractory phase");
    require_equal(network.neuron(1).input_event_count, 3U, "input events were not counted");
    const auto fourth = brainstem.receive_impulse(network, synapse, config);
    require(fourth.fired, "neuron did not fire after refractory phase");
}

void test_regulation_state_persistence() {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve-experiment-002-test-state.bin";
    auto config = small_config();
    config.initial_outgoing_synapses = 1;
    config.initial_synapse_weight = 1;
    config.maximum_synapse_weight = 1;
    config.input_weight_budget = 8;
    config.refractory_input_events = 2;
    config.synaptic_learning_threshold = 1;
    config.firing_threshold = 100;
    eve::bios::Runtime original(config, 123);
    original.enqueue_fire(0);
    static_cast<void>(original.run());
    require_equal(original.metrics().rejected_strengthenings_at_weight_limit, 1ULL,
                  "runtime did not count a rejected strengthening");
    original.save(path.string());
    eve::bios::Runtime restored = eve::bios::Runtime::load(path.string());
    require_equal(original.config().input_weight_budget, restored.config().input_weight_budget,
                  "input budget was not restored");
    require_equal(original.config().refractory_input_events,
                  restored.config().refractory_input_events,
                  "refractory configuration was not restored");
    require_equal(original.state_fingerprint(), restored.state_fingerprint(),
                  "regulated state differs after restore");
    require_equal(original.metrics(), restored.metrics(),
                  "regulated metrics differ after restore");
    require_equal(original.network().neurons(), restored.network().neurons(),
                  "refractory neuron state differs after restore");
    static_cast<void>(std::filesystem::remove(path));
}

void test_disabled_regulations_match_reference() {
    auto reference_config = small_config();
    reference_config.initial_outgoing_synapses = 2;
    eve::ExperimentConfig explicit_config = reference_config;
    explicit_config.maximum_synapse_weight = std::numeric_limits<std::int32_t>::max();
    explicit_config.input_weight_budget = std::numeric_limits<std::uint64_t>::max();
    explicit_config.refractory_input_events = 0;
    eve::bios::Runtime reference(reference_config, 20261117);
    eve::bios::Runtime explicit_disabled(explicit_config, 20261117);
    static_cast<void>(reference.process_utf8_text("ABAB"));
    static_cast<void>(explicit_disabled.process_utf8_text("ABAB"));
    require_equal(reference.state_fingerprint(), explicit_disabled.state_fingerprint(),
                  "disabled regulations changed reference execution");
    require_equal(reference.metrics(), explicit_disabled.metrics(),
                  "disabled regulations changed reference metrics");
}

void test_growth_probability_and_partner_selection() {
    eve::brainstem::Brainstem brainstem;
    eve::bios::DeterministicRng statistics_rng(1234);
    std::uint32_t successes = 0;
    for (std::uint32_t index = 0; index < 20'000; ++index) {
        if (brainstem.should_attempt_growth(4, statistics_rng)) {
            ++successes;
        }
    }
    require(successes > 3'700 && successes < 4'300, "growth probability differs from 1/(1+k)");

    auto config = small_config();
    eve::neural::Network network;
    network.reset(config);
    eve::bios::DeterministicRng selection_rng(99);
    const auto grown = brainstem.attempt_growth(network, 0, config, selection_rng);
    require(grown.has_value(), "k=0 must always attempt growth");
    const eve::neural::Synapse& synapse = network.synapse(*grown);
    require(synapse.receiver != 0, "growth selected a self-connection");
    require_equal(synapse.weight, 0, "new synapse must start at weight zero");
    require_equal(network.synapse_count(), 1U, "one synapse should have grown");
}

void test_reproducible_execution() {
    eve::ExperimentConfig config;
    config.growth_enabled = true;
    eve::bios::Runtime first(config, 2026);
    eve::bios::Runtime second(config, 2026);
    eve::bios::Runtime different(config, 2027);
    std::vector<eve::bios::FireObservation> observed_fires;
    first.set_fire_observer([&observed_fires](const eve::bios::FireObservation& event) {
        observed_fires.push_back(event);
    });
    static_cast<void>(first.process_utf8_text("ABAB"));
    static_cast<void>(second.process_utf8_text("ABAB"));
    static_cast<void>(different.process_utf8_text("ABAB"));
    require_equal(first.state_fingerprint(), second.state_fingerprint(), "same seed diverged");
    require_equal(
        observed_fires.size(),
        static_cast<std::size_t>(first.metrics().fire_events),
        "observer missed fire events");
    require(
        first.state_fingerprint() != different.state_fingerprint(),
        "different seeds unexpectedly produced the same complete state");
}

void test_persistence_round_trip() {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve-experiment-001-test-state.bin";
    eve::ExperimentConfig config;
    config.growth_enabled = true;
    eve::bios::Runtime original(config, 77);
    static_cast<void>(original.process_utf8_text("AB"));
    original.enqueue_fire(3);
    original.save(path.string());

    eve::bios::Runtime restored = eve::bios::Runtime::load(path.string());
    require_equal(
        original.state_fingerprint(), restored.state_fingerprint(), "restored state differs");
    require_equal(original.metrics(), restored.metrics(), "restored metrics differ");
    require_equal(original.network().neurons(), restored.network().neurons(), "neurons differ");
    require_equal(original.network().synapses(), restored.network().synapses(), "synapses differ");

    static_cast<void>(original.run());
    static_cast<void>(restored.run());
    require_equal(
        original.state_fingerprint(), restored.state_fingerprint(), "continued execution diverged");
    static_cast<void>(std::filesystem::remove(path));
}

void test_utf8_byte_sensory_processing() {
    eve::brainstem::Brainstem brainstem;
    const std::vector<std::uint8_t> bytes{0x41, 0xc3, 0xa4};
    const auto encoded = brainstem.encode_utf8_bytes(bytes, 256);
    require_equal(encoded.size(), std::size_t{3}, "three raw UTF-8 bytes expected");
    require_equal(encoded[0].byte_index, 0ULL, "first byte index");
    require_equal(encoded[0].value, std::uint8_t{0x41}, "first byte value");
    require_equal(
        encoded[0].fired_channels, std::vector<eve::NeuronId>({0, 6}), "A bit channels");
    require_equal(
        encoded[1].fired_channels,
        std::vector<eve::NeuronId>({0, 1, 6, 7}),
        "UTF-8 lead byte channels");
    require_equal(
        encoded[2].fired_channels,
        std::vector<eve::NeuronId>({2, 5, 7}),
        "UTF-8 continuation byte channels");

    auto config = small_config();
    eve::bios::Runtime runtime(config, 5);
    const auto first_call = runtime.process_utf8_bytes({0x41});
    const auto second_call = runtime.process_utf8_bytes({0xc3, 0xa4});
    require_equal(first_call[0].byte_index, 0ULL, "first global byte index");
    require_equal(second_call[0].byte_index, 1ULL, "byte index must continue across calls");
    require_equal(second_call[1].byte_index, 2ULL, "third global byte index");
}

void test_motor_assignment_is_reproducible_and_disjoint() {
    for (const std::uint32_t contacts : {1U, 4U, 8U}) {
        eve::bios::Runtime first(motor_test_config(), 1);
        eve::bios::Runtime second(motor_test_config(), 999);
        first.configure_motor_output(motor_output_config(contacts));
        second.configure_motor_output(motor_output_config(contacts));
        require_equal(first.motor_assignment(), second.motor_assignment(),
                      "motor assignment depends on network initialization seed");

        std::set<eve::NeuronId> selected;
        for (const auto& channel : first.motor_assignment().channels) {
            require_equal(channel.size(), static_cast<std::size_t>(contacts),
                          "unexpected contact group size");
            for (const eve::NeuronId neuron : channel) {
                require(neuron > 8, "motor contact overlaps sensory or response channel");
                require(selected.insert(neuron).second, "motor contact groups overlap");
            }
        }
        require(first.motor_assignment().completion_contact > 8,
                "completion overlaps sensory or response channel");
        require(selected.insert(first.motor_assignment().completion_contact).second,
                "completion contact overlaps a data contact");
    }
}

void test_motor_output_events_form_byte() {
    eve::bios::Runtime runtime(motor_test_config(), 1);
    runtime.configure_motor_output(motor_output_config());
    const auto& assignment = runtime.motor_assignment();
    runtime.enqueue_fire(assignment.channels[0][0]);
    runtime.enqueue_fire(assignment.channels[3][0]);
    runtime.enqueue_fire(assignment.completion_contact);
    const auto response = runtime.request_response(false);
    require(response.valid(), "completion did not produce a valid response");
    require_equal(response.value, std::uint8_t{0x09}, "motor channels formed wrong byte");
    require_equal(response.activations.size(), std::size_t{3},
                  "motor activation sequence was not recorded");
}

void test_all_motor_channels_form_full_byte() {
    eve::bios::Runtime runtime(motor_test_config(), 2);
    runtime.configure_motor_output(motor_output_config(4));
    const auto& assignment = runtime.motor_assignment();
    for (const auto& channel : assignment.channels) {
        runtime.enqueue_fire(channel.front());
    }
    runtime.enqueue_fire(assignment.completion_contact);
    const auto response = runtime.request_response(false);
    require(response.valid(), "full byte response did not complete");
    require_equal(response.value, std::uint8_t{0xff}, "eight channels did not form 0xff");
}

void test_repeated_motor_fire_keeps_binary_value() {
    eve::bios::Runtime runtime(motor_test_config(), 3);
    runtime.configure_motor_output(motor_output_config());
    const auto& assignment = runtime.motor_assignment();
    runtime.enqueue_fire(assignment.channels[2][0]);
    runtime.enqueue_fire(assignment.channels[2][0]);
    runtime.enqueue_fire(assignment.completion_contact);
    const auto response = runtime.request_response(false);
    require_equal(response.value, std::uint8_t{0x04},
                  "repeated firing changed the binary channel value");
    require_equal(response.activations.size(), std::size_t{3},
                  "repeated activation was not retained for diagnostics");
}

void test_motor_events_outside_response_are_ignored() {
    eve::bios::Runtime runtime(motor_test_config(), 4);
    runtime.configure_motor_output(motor_output_config());
    const auto assignment = runtime.motor_assignment();
    runtime.enqueue_fire(assignment.channels[0][0]);
    runtime.enqueue_fire(assignment.completion_contact);
    static_cast<void>(runtime.run());
    const auto response = runtime.request_response(false);
    require(!response.valid(), "input-phase motor activity leaked into a response");
    require_equal(response.value, std::uint8_t{0}, "ignored activity set an output bit");
}

void test_response_stimulus_is_processed_without_answer_content() {
    eve::bios::Runtime runtime(motor_test_config(), 5);
    runtime.configure_motor_output(motor_output_config());
    const auto response = runtime.request_response(true);
    require_equal(response.status, eve::brainstem::MotorResponseStatus::no_response,
                  "isolated response stimulus fabricated an answer");
    require_equal(response.processed_events, 1ULL,
                  "response stimulus was not processed as one neural fire event");
    require_equal(response.value, std::uint8_t{0}, "response stimulus encoded answer bits");
}

void test_null_byte_differs_from_missing_response() {
    eve::bios::Runtime runtime(motor_test_config(), 6);
    runtime.configure_motor_output(motor_output_config());
    runtime.enqueue_fire(runtime.motor_assignment().completion_contact);
    const auto null_byte = runtime.request_response(false);
    const auto missing = runtime.request_response(false);
    require(null_byte.valid(), "completed null byte is not valid");
    require_equal(null_byte.value, std::uint8_t{0}, "completed null byte changed value");
    require_equal(missing.status, eve::brainstem::MotorResponseStatus::no_response,
                  "missing response was treated as a null byte");
}

void test_motor_response_aborts_at_safety_limit() {
    eve::bios::Runtime runtime(motor_test_config(), 7);
    runtime.configure_motor_output(motor_output_config());
    runtime.enqueue_fire(runtime.motor_assignment().channels[1][0]);
    runtime.enqueue_fire(runtime.motor_assignment().completion_contact);
    const auto response = runtime.request_response(false, 1);
    require_equal(response.status, eve::brainstem::MotorResponseStatus::aborted,
                  "response did not stop at safety limit");
    require_equal(response.processed_events, 1ULL, "safety limit processed too many events");
}

void test_motor_responses_are_reproducible() {
    const auto execute = []() {
        eve::bios::Runtime runtime(motor_test_config(), 8);
        runtime.configure_motor_output(motor_output_config(8, 77));
        runtime.enqueue_fire(runtime.motor_assignment().channels[1][3]);
        runtime.enqueue_fire(runtime.motor_assignment().channels[6][5]);
        runtime.enqueue_fire(runtime.motor_assignment().completion_contact);
        return runtime.request_response(false);
    };
    const auto first = execute();
    const auto second = execute();
    require_equal(first.status, second.status, "response status is not reproducible");
    require_equal(first.value, second.value, "response value is not reproducible");
    require_equal(first.activations, second.activations,
                  "motor activation sequence is not reproducible");
}

void test_motor_assignment_after_state_restore() {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve-stage-a-motor-state.bin";
    eve::bios::Runtime original(motor_test_config(), 9);
    static_cast<void>(original.process_utf8_text("AB"));
    original.save(path.string());
    eve::bios::Runtime restored = eve::bios::Runtime::load(path.string());
    original.configure_motor_output(motor_output_config(4, 123));
    restored.configure_motor_output(motor_output_config(4, 123));
    require_equal(original.motor_assignment(), restored.motor_assignment(),
                  "restored state received a different motor assignment");
    require_equal(original.state_fingerprint(), restored.state_fingerprint(),
                  "motor configuration changed restored neural state");
    static_cast<void>(std::filesystem::remove(path));
}

void test_disabled_motor_interface_preserves_existing_behavior() {
    eve::bios::Runtime first(motor_test_config(), 10);
    eve::bios::Runtime second(motor_test_config(), 10);
    require(!first.motor_output_configured(), "motor interface is enabled by default");
    static_cast<void>(first.process_utf8_text("AB"));
    static_cast<void>(second.process_utf8_text("AB"));
    require_equal(first.state_fingerprint(), second.state_fingerprint(),
                  "disabled motor interface changed existing processing");
}

void test_motor_assignment_cannot_directly_copy_sensor_bits() {
    eve::bios::Runtime runtime(motor_test_config(), 11);
    runtime.configure_motor_output(motor_output_config(8));
    for (const auto& channel : runtime.motor_assignment().channels) {
        for (const eve::NeuronId neuron : channel) {
            require(neuron >= 9, "sensory neuron is wired directly to motor output");
        }
    }
    static_cast<void>(runtime.process_utf8_bytes({0xff}));
    const auto response = runtime.request_response(false);
    require_equal(response.status, eve::brainstem::MotorResponseStatus::no_response,
                  "sensory byte was copied into a motor response");
}

eve::brainstem::InnateWiringConfig innate_config(
    const eve::brainstem::InnateTopology topology,
    const std::int32_t weight,
    const std::uint64_t seed = 9917) {
    eve::brainstem::InnateWiringConfig config;
    config.topology = topology;
    config.initial_weight = weight;
    config.topology_seed = seed;
    return config;
}

void test_innate_wiring_is_deterministic() {
    const auto execute = []() {
        eve::bios::Runtime runtime(motor_test_config(), 15);
        runtime.configure_motor_output(motor_output_config());
        const auto result = runtime.configure_innate_wiring(innate_config(
            eve::brainstem::InnateTopology::functional_paths, 1));
        return std::make_pair(result.paths, runtime.state_fingerprint());
    };
    const auto first = execute();
    const auto second = execute();
    require_equal(first.first, second.first, "innate path selection is not deterministic");
    require_equal(first.second, second.second, "innate wiring state is not deterministic");
}

void test_innate_wiring_counts_and_reachability() {
    eve::bios::Runtime runtime(motor_test_config(), 16);
    runtime.configure_motor_output(motor_output_config());
    const auto result = runtime.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::structural_paths, 0));
    require_equal(result.paths.size(), std::size_t{9}, "M1 needs nine innate paths");
    require_equal(result.added_synapses, 27U, "each innate path needs three synapses");
    require_equal(result.adjusted_existing_synapses, 0U,
                  "empty network unexpectedly adjusted a synapse");
    require_equal(result.structurally_reachable_motor_contacts, 9U,
                  "not all M1 motor contacts are structurally reachable");
    require_equal(result.maximum_shortest_path, 3U, "innate path length differs from three");
    require(runtime.network().validate(), "innate wiring violates network invariants");
    const auto analyzed = runtime.motor_reachability();
    require_equal(analyzed.reachable_contacts, 9U,
                  "independent reachability analysis disagrees");
}

void test_structural_zero_weight_path_does_not_transmit() {
    eve::bios::Runtime runtime(motor_test_config(), 17);
    runtime.configure_motor_output(motor_output_config());
    const auto result = runtime.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::structural_paths, 0));
    std::vector<eve::NeuronId> fired;
    runtime.set_fire_observer([&fired](const eve::bios::FireObservation& observation) {
        fired.push_back(observation.neuron);
    });
    runtime.enqueue_fire(result.paths.front().source);
    static_cast<void>(runtime.run());
    require(std::find(fired.begin(), fired.end(), result.paths.front().motor_target) ==
                fired.end(),
            "zero-weight structural path transmitted activity");
}

void test_functional_innate_path_transmits_neurally() {
    auto config = motor_test_config();
    config.firing_threshold = 3;
    eve::bios::Runtime runtime(config, 18);
    runtime.configure_motor_output(motor_output_config());
    const auto result = runtime.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::functional_paths, 3));
    std::vector<eve::NeuronId> fired;
    runtime.set_fire_observer([&fired](const eve::bios::FireObservation& observation) {
        fired.push_back(observation.neuron);
    });
    runtime.enqueue_fire(result.paths.front().source);
    static_cast<void>(runtime.run());
    require(std::find(fired.begin(), fired.end(), result.paths.front().first_relay) !=
                fired.end(),
            "first innate relay did not fire");
    require(std::find(fired.begin(), fired.end(), result.paths.front().second_relay) !=
                fired.end(),
            "second innate relay did not fire");
    require(std::find(fired.begin(), fired.end(), result.paths.front().motor_target) !=
                fired.end(),
            "functional innate path did not reach motor target");
}

void test_varied_innate_topology_changes_paths() {
    const auto execute = [](const std::uint64_t seed) {
        eve::bios::Runtime runtime(motor_test_config(), 19);
        runtime.configure_motor_output(motor_output_config());
        return runtime.configure_innate_wiring(innate_config(
            eve::brainstem::InnateTopology::varied_functional_paths, 1, seed)).paths;
    };
    require(execute(1) != execute(2), "different topology seeds produced identical paths");
}

void test_disabled_innate_wiring_preserves_reference() {
    eve::bios::Runtime reference(motor_test_config(), 20);
    eve::bios::Runtime disabled(motor_test_config(), 20);
    reference.configure_motor_output(motor_output_config());
    disabled.configure_motor_output(motor_output_config());
    static_cast<void>(disabled.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::none, 0)));
    require_equal(reference.state_fingerprint(), disabled.state_fingerprint(),
                  "disabled innate wiring changed reference state");
}

eve::brainstem::ExplorationConfig exploration_config(
    const eve::brainstem::ExplorationMode mode,
    const std::uint32_t count,
    const std::uint64_t seed = 12345) {
    eve::brainstem::ExplorationConfig config;
    config.mode = mode;
    config.activation_count = count;
    config.activation_budget = count == 0 ? 1 : count;
    config.seed = seed;
    config.center = eve::Coordinate{3, 3, 1};
    config.radius = 1;
    return config;
}

void test_exploration_is_reproducible_and_rng_independent() {
    eve::bios::Runtime first(motor_test_config(), 21);
    eve::bios::Runtime second(motor_test_config(), 21);
    const auto network_before = first.network().state_fingerprint();
    const auto a = first.enqueue_exploration(exploration_config(
        eve::brainstem::ExplorationMode::multiple, 6, 777));
    const auto b = second.enqueue_exploration(exploration_config(
        eve::brainstem::ExplorationMode::multiple, 6, 777));
    require_equal(a.activated_neurons, b.activated_neurons,
                  "exploration selection is not reproducible");
    require_equal(first.network().state_fingerprint(), network_before,
                  "exploration selection changed neural state before processing");
    require_equal(first.network().state_fingerprint(), second.network().state_fingerprint(),
                  "exploration RNG changed network initialization");
}

void test_single_and_multiple_exploration_counts() {
    eve::bios::Runtime runtime(motor_test_config(), 22);
    auto single = exploration_config(eve::brainstem::ExplorationMode::single, 99);
    single.activation_budget = 1;
    const auto one = runtime.enqueue_exploration(single);
    const auto many = runtime.enqueue_exploration(exploration_config(
        eve::brainstem::ExplorationMode::multiple, 5));
    require_equal(one.activated_neurons.size(), std::size_t{1},
                  "single exploration did not select one neuron");
    require_equal(many.activated_neurons.size(), std::size_t{5},
                  "multiple exploration selected wrong count");
    require_equal(runtime.diagnostics().queued_events, std::size_t{6},
                  "exploration did not enqueue exactly its selected fires");
}

void test_exploration_budget_is_enforced() {
    eve::bios::Runtime runtime(motor_test_config(), 23);
    auto config = exploration_config(eve::brainstem::ExplorationMode::multiple, 5);
    config.activation_budget = 4;
    bool rejected = false;
    try {
        static_cast<void>(runtime.enqueue_exploration(config));
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    require(rejected, "exploration exceeded activation budget");
    require_equal(runtime.diagnostics().queued_events, std::size_t{0},
                  "rejected exploration enqueued events");
}

void test_localized_exploration_stays_in_region() {
    eve::bios::Runtime runtime(motor_test_config(), 24);
    const auto config = exploration_config(
        eve::brainstem::ExplorationMode::localized, 12);
    const auto result = runtime.enqueue_exploration(config);
    for (const eve::NeuronId neuron : result.activated_neurons) {
        const eve::Coordinate coordinate = runtime.network().coordinate(neuron);
        const auto difference = [](const std::uint32_t left, const std::uint32_t right) {
            return left > right ? left - right : right - left;
        };
        require(difference(coordinate.x, config.center.x) <= config.radius &&
                    difference(coordinate.y, config.center.y) <= config.radius &&
                    difference(coordinate.z, config.center.z) <= config.radius,
                "localized exploration escaped configured region");
    }
}

void test_distributed_exploration_covers_regions() {
    eve::bios::Runtime runtime(motor_test_config(), 25);
    const auto result = runtime.enqueue_exploration(exploration_config(
        eve::brainstem::ExplorationMode::distributed, 8));
    std::set<std::size_t> regions;
    for (const eve::NeuronId neuron : result.activated_neurons) {
        const eve::Coordinate coordinate = runtime.network().coordinate(neuron);
        regions.insert((coordinate.x >= 4 ? 1U : 0U) |
                       (coordinate.y >= 4 ? 2U : 0U) |
                       (coordinate.z >= 2 ? 4U : 0U));
    }
    require_equal(regions.size(), std::size_t{8},
                  "distributed exploration did not cover all spatial regions");
}

void test_exploration_respects_response_event_limit() {
    eve::bios::Runtime runtime(motor_test_config(), 26);
    runtime.configure_motor_output(motor_output_config());
    static_cast<void>(runtime.enqueue_exploration(exploration_config(
        eve::brainstem::ExplorationMode::multiple, 4)));
    const auto response = runtime.request_response(false, 2);
    require_equal(response.status, eve::brainstem::MotorResponseStatus::aborted,
                  "explorative response ignored event safety limit");
    require_equal(response.processed_events, 2ULL,
                  "explorative response processed beyond event limit");
}

void test_disabled_exploration_preserves_reference() {
    eve::bios::Runtime reference(motor_test_config(), 27);
    eve::bios::Runtime disabled(motor_test_config(), 27);
    static_cast<void>(disabled.enqueue_exploration(exploration_config(
        eve::brainstem::ExplorationMode::none, 0)));
    require_equal(reference.state_fingerprint(), disabled.state_fingerprint(),
                  "disabled exploration changed reference state");
}

eve::brainstem::MotorCircuitConfig motor_circuit_config(
    const eve::brainstem::MotorCircuitMode mode,
    const std::int32_t motor_weight,
    const std::int32_t completion_weight,
    const std::uint64_t seed = 4567) {
    eve::brainstem::MotorCircuitConfig config;
    config.mode = mode;
    config.motor_weight = motor_weight;
    config.completion_weight = completion_weight;
    config.relay_count = 4;
    config.seed = seed;
    return config;
}

void test_direct_motor_circuit_completes_neuronally() {
    eve::bios::Runtime runtime(motor_test_config(), 28);
    runtime.configure_motor_output(motor_output_config());
    static_cast<void>(runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::direct_completion_path, 3, 3)));
    runtime.enqueue_fire(runtime.motor_assignment().channels[4][0]);
    const auto response = runtime.request_response(false);
    require(response.valid(), "direct motor circuit did not complete response");
    require_equal(response.value, std::uint8_t{0x10},
                  "direct motor circuit changed data content");
    require_equal(response.activations.back().channel, std::int8_t{-1},
                  "completion did not originate from completion neuron");
}

void test_completion_group_uses_neural_relays() {
    eve::bios::Runtime runtime(motor_test_config(), 29);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::completion_group, 3, 3));
    require_equal(circuit.relay_neurons.size(), std::size_t{4},
                  "completion group has wrong relay count");
    std::vector<eve::NeuronId> fired;
    runtime.set_fire_observer([&fired](const eve::bios::FireObservation& observation) {
        fired.push_back(observation.neuron);
    });
    runtime.enqueue_fire(runtime.motor_assignment().channels[1][0]);
    const auto response = runtime.request_response(false);
    require(response.valid(), "completion group did not complete response");
    require(std::any_of(circuit.relay_neurons.begin(), circuit.relay_neurons.end(),
                        [&fired](const eve::NeuronId relay) {
                            return std::find(fired.begin(), fired.end(), relay) != fired.end();
                        }),
            "completion group bypassed its neural relays");
}

void test_activity_dependent_circuit_requires_multiple_events() {
    const auto execute = [](const std::uint32_t events) {
        eve::bios::Runtime runtime(motor_test_config(), 30);
        runtime.configure_motor_output(motor_output_config());
        static_cast<void>(runtime.configure_motor_circuit(motor_circuit_config(
            eve::brainstem::MotorCircuitMode::activity_dependent, 1, 3)));
        for (std::uint32_t index = 0; index < events; ++index) {
            runtime.enqueue_fire(runtime.motor_assignment().channels[index % 8][0]);
        }
        return runtime.request_response(false);
    };
    require(!execute(2).valid(), "activity-dependent circuit completed too early");
    require(execute(3).valid(), "activity-dependent circuit did not complete after threshold");
}

void test_motor_circuit_can_complete_null_byte_neuronally() {
    eve::bios::Runtime runtime(motor_test_config(), 31);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::completion_group, 3, 3));
    runtime.enqueue_fire(circuit.relay_neurons.front());
    const auto response = runtime.request_response(false);
    require(response.valid(), "neural relay did not complete null byte");
    require_equal(response.value, std::uint8_t{0},
                  "neural null-byte completion set a data channel");
}

void test_motor_circuit_is_reproducible() {
    const auto execute = []() {
        eve::bios::Runtime runtime(motor_test_config(), 32);
        runtime.configure_motor_output(motor_output_config(4));
        const auto result = runtime.configure_motor_circuit(motor_circuit_config(
            eve::brainstem::MotorCircuitMode::completion_group, 3, 3, 888));
        return std::make_pair(result.relay_neurons, runtime.state_fingerprint());
    };
    require_equal(execute(), execute(), "motor circuit is not reproducible");
}

void test_motor_circuit_does_not_copy_sensory_input() {
    eve::bios::Runtime runtime(motor_test_config(), 33);
    runtime.configure_motor_output(motor_output_config());
    static_cast<void>(runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::direct_completion_path, 3, 3)));
    for (eve::NeuronId sensory = 0; sensory <= 8; ++sensory) {
        require(!runtime.network().find_synapse(
                    sensory, runtime.motor_assignment().completion_contact).has_value(),
                "motor circuit connected sensory input directly to completion");
    }
}

void test_disabled_motor_circuit_preserves_reference() {
    eve::bios::Runtime reference(motor_test_config(), 34);
    eve::bios::Runtime disabled(motor_test_config(), 34);
    reference.configure_motor_output(motor_output_config());
    disabled.configure_motor_output(motor_output_config());
    static_cast<void>(disabled.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::none, 1, 1)));
    require_equal(reference.state_fingerprint(), disabled.state_fingerprint(),
                  "disabled motor circuit changed reference state");
}

eve::brainstem::FeedbackConfig feedback_config(
    const eve::brainstem::FeedbackMode mode,
    const std::uint64_t budget = 1000,
    const std::uint8_t environment = 0) {
    eve::brainstem::FeedbackConfig config;
    config.mode = mode;
    config.first_feedback_neuron = 9;
    config.event_budget = budget;
    config.environment_state = environment;
    return config;
}

eve::brainstem::MotorResponse completed_test_response(
    eve::bios::Runtime& runtime,
    const std::vector<std::size_t>& channels) {
    static_cast<void>(runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::direct_completion_path, 3, 3)));
    for (const std::size_t channel : channels) {
        runtime.enqueue_fire(runtime.motor_assignment().channels[channel][0]);
    }
    return runtime.request_response(false);
}

void test_feedback_after_completed_motor_response() {
    eve::bios::Runtime runtime(motor_test_config(), 35);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto response = completed_test_response(runtime, {0, 2});
    require(response.valid(), "feedback test response did not complete");
    static_cast<void>(runtime.run());
    const auto feedback = runtime.process_motor_feedback(
        response, feedback_config(eve::brainstem::FeedbackMode::after_completion));
    require_equal(feedback.sensory_events.size(), std::size_t{3},
                  "feedback did not encode two bits plus marker");
    require_equal(feedback.sensory_events[0].sensory_neuron, 9U,
                  "motor bit zero mapped to wrong feedback neuron");
    require_equal(feedback.sensory_events[1].sensory_neuron, 11U,
                  "motor bit two mapped to wrong feedback neuron");
    require_equal(feedback.sensory_events[2].sensory_neuron, 17U,
                  "feedback marker mapped to wrong neuron");
    require_equal(feedback.processed_events, 3ULL,
                  "feedback processing did not use regular fire events");
}

void test_feedback_requires_observed_activity() {
    eve::bios::Runtime runtime(motor_test_config(), 36);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto missing = runtime.request_response(false);
    const auto feedback = runtime.process_motor_feedback(
        missing, feedback_config(eve::brainstem::FeedbackMode::after_completion));
    require(feedback.sensory_events.empty(),
            "missing motor response produced sensory feedback");
    require_equal(feedback.processed_events, 0ULL,
                  "missing response processed feedback events");
}

void test_per_motor_event_feedback_preserves_sequence() {
    eve::bios::Runtime runtime(motor_test_config(), 37);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    runtime.enqueue_fire(runtime.motor_assignment().channels[3][0]);
    runtime.enqueue_fire(runtime.motor_assignment().channels[3][0]);
    const auto incomplete = runtime.request_response(false);
    const auto feedback = runtime.process_motor_feedback(
        incomplete, feedback_config(eve::brainstem::FeedbackMode::per_motor_event));
    require_equal(feedback.sensory_events.size(), std::size_t{2},
                  "per-event feedback lost repeated motor activation");
    require_equal(feedback.sensory_events[0].sensory_neuron, 12U,
                  "per-event feedback mapped wrong channel");
    require_equal(feedback.sensory_events[1].sensory_neuron, 12U,
                  "per-event feedback changed repeated channel");
}

void test_virtual_environment_feedback_is_distinct() {
    eve::bios::Runtime runtime(motor_test_config(), 38);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto response = completed_test_response(runtime, {0, 2});
    const auto feedback = runtime.process_motor_feedback(
        response,
        feedback_config(eve::brainstem::FeedbackMode::virtual_environment, 1000, 0x80));
    require_equal(feedback.environment_before, std::uint8_t{0x80},
                  "virtual environment lost prior state");
    require_equal(feedback.environment_after, std::uint8_t{0x04},
                  "virtual environment transition is wrong");
    require_equal(feedback.mode, eve::brainstem::FeedbackMode::virtual_environment,
                  "environment feedback is not distinguishable from self-feedback");
}

void test_feedback_event_budget_aborts_cascade() {
    eve::bios::Runtime runtime(motor_test_config(), 39);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto response = completed_test_response(runtime, {0, 1});
    const auto feedback = runtime.process_motor_feedback(
        response, feedback_config(eve::brainstem::FeedbackMode::after_completion, 1));
    require(feedback.aborted, "feedback ignored event budget");
    require_equal(feedback.processed_events, 1ULL,
                  "feedback processed beyond event budget");
}

void test_feedback_rejects_motor_channel_overlap() {
    eve::bios::Runtime runtime(motor_test_config(), 40);
    runtime.configure_motor_output(motor_output_config());
    eve::brainstem::MotorResponse response;
    response.status = eve::brainstem::MotorResponseStatus::completed;
    const eve::NeuronId contact = runtime.motor_assignment().channels[0][0];
    auto overlapping = feedback_config(eve::brainstem::FeedbackMode::after_completion);
    overlapping.first_feedback_neuron = contact <= 247 ? contact : contact - 8;
    bool rejected = false;
    try {
        static_cast<void>(runtime.process_motor_feedback(
            response, overlapping));
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    require(rejected, "overlapping feedback and motor channels were accepted");
}

void test_disabled_feedback_preserves_reference() {
    eve::bios::Runtime runtime(motor_test_config(), 41);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto fingerprint = runtime.state_fingerprint();
    eve::brainstem::MotorResponse response;
    static_cast<void>(runtime.process_motor_feedback(
        response, feedback_config(eve::brainstem::FeedbackMode::none)));
    require_equal(runtime.state_fingerprint(), fingerprint,
                  "disabled feedback changed runtime state");
}

void test_integrated_sensorimotor_cycle() {
    eve::bios::Runtime runtime(motor_test_config(), 42);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    static_cast<void>(runtime.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::functional_paths, 3)));
    static_cast<void>(runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::direct_completion_path, 3, 3)));

    eve::brainstem::ExplorationConfig exploration = exploration_config(
        eve::brainstem::ExplorationMode::localized, 1, 999);
    const eve::NeuronId motor_neuron = runtime.motor_assignment().channels[5][0];
    exploration.center = runtime.network().coordinate(motor_neuron);
    exploration.radius = 0;
    static_cast<void>(runtime.enqueue_exploration(exploration));
    const auto response = runtime.request_response(false);
    require(response.valid(), "integrated exploration did not complete motor response");
    require_equal(response.value, std::uint8_t{0x20},
                  "integrated motor response has wrong value");
    const auto feedback = runtime.process_motor_feedback(
        response, feedback_config(eve::brainstem::FeedbackMode::after_completion));
    require_equal(feedback.sensory_events.size(), std::size_t{2},
                  "integrated response did not return bit plus marker");
    require(!feedback.aborted, "integrated sensorimotor feedback aborted");
}

void test_integrated_sensorimotor_state_restore() {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve-stage-a1-test-state.bin";
    eve::bios::Runtime original(motor_test_config(), 84);
    const auto output_config = motor_output_config(1, 200, 18);
    original.configure_motor_output(output_config);
    static_cast<void>(original.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::functional_paths, 3)));
    static_cast<void>(original.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::direct_completion_path, 3, 3)));
    original.save(path.string());

    eve::bios::Runtime restored = eve::bios::Runtime::load(path.string());
    restored.configure_motor_output(output_config);
    require_equal(original.network().synapses(), restored.network().synapses(),
                  "A.1 innate and motor synapses were not restored");

    const auto execute_cycle = [](eve::bios::Runtime& runtime) {
        eve::brainstem::ExplorationConfig exploration = exploration_config(
            eve::brainstem::ExplorationMode::localized, 1, 1234);
        exploration.center = runtime.network().coordinate(
            runtime.motor_assignment().channels[3][0]);
        exploration.radius = 0;
        static_cast<void>(runtime.enqueue_exploration(exploration));
        const auto response = runtime.request_response(false);
        const auto feedback = runtime.process_motor_feedback(
            response, feedback_config(eve::brainstem::FeedbackMode::after_completion));
        std::vector<std::pair<std::int8_t, eve::NeuronId>> sensory_events;
        for (const auto& event : feedback.sensory_events) {
            sensory_events.emplace_back(event.motor_channel, event.sensory_neuron);
        }
        return std::make_pair(response, sensory_events);
    };
    const auto original_result = execute_cycle(original);
    const auto restored_result = execute_cycle(restored);
    require_equal(original_result.first.status, restored_result.first.status,
                  "response status diverged after state restore");
    require_equal(original_result.first.value, restored_result.first.value,
                  "response value diverged after state restore");
    require_equal(original_result.first.activated_channels,
                  restored_result.first.activated_channels,
                  "motor channels diverged after state restore");
    require_equal(original_result.first.activations, restored_result.first.activations,
                  "motor sequence diverged after state restore");
    require_equal(original_result.second, restored_result.second,
                  "feedback sequence diverged after state restore");
    require_equal(original.state_fingerprint(), restored.state_fingerprint(),
                  "sensorimotor state diverged after continued execution");
    static_cast<void>(std::filesystem::remove(path));
}

}  // namespace

int main() {
    const std::vector<std::pair<std::string, std::function<void()>>> tests{
        {"network initialization", test_network_initialization},
        {"duplicate synapse exclusion", test_duplicate_and_self_connections},
        {"3D index calculation", test_3d_index_calculation},
        {"binary fire forwarding", test_binary_fire_forwarding},
        {"weighted input processing", test_weighted_input_processing},
        {"strengthening after three impulses", test_strengthening_after_three_impulses},
        {"activity phase reset after eight impulses", test_phase_reset_after_eight_impulses},
        {"learning before simultaneous phase completion", test_learning_precedes_phase_completion},
        {"learning through a zero-weight synapse", test_learning_from_zero_weight},
        {"maximum synapse weight", test_maximum_synapse_weight},
        {"combined refractory and weight limit", test_combined_refractory_and_weight_limit},
        {"input budget with free capacity", test_input_weight_budget_free_capacity},
        {"input budget redistribution", test_input_weight_budget_redistribution},
        {"input budget without competitor", test_input_weight_budget_without_competitor},
        {"refractory processing and learning", test_refractory_events_keep_processing_and_learning},
        {"regulation state persistence", test_regulation_state_persistence},
        {"disabled regulations reproduce reference", test_disabled_regulations_match_reference},
        {"growth probability and partner selection", test_growth_probability_and_partner_selection},
        {"reproducible execution", test_reproducible_execution},
        {"state persistence", test_persistence_round_trip},
        {"raw UTF-8 sensory processing", test_utf8_byte_sensory_processing},
        {"motor assignment reproducibility", test_motor_assignment_is_reproducible_and_disjoint},
        {"motor output byte formation", test_motor_output_events_form_byte},
        {"all motor channels", test_all_motor_channels_form_full_byte},
        {"repeated motor fire", test_repeated_motor_fire_keeps_binary_value},
        {"motor phase separation", test_motor_events_outside_response_are_ignored},
        {"response stimulus", test_response_stimulus_is_processed_without_answer_content},
        {"null byte and missing response", test_null_byte_differs_from_missing_response},
        {"motor safety limit", test_motor_response_aborts_at_safety_limit},
        {"motor response reproducibility", test_motor_responses_are_reproducible},
        {"motor assignment after restore", test_motor_assignment_after_state_restore},
        {"disabled motor interface", test_disabled_motor_interface_preserves_existing_behavior},
        {"no direct sensor copy", test_motor_assignment_cannot_directly_copy_sensor_bits},
        {"innate wiring determinism", test_innate_wiring_is_deterministic},
        {"innate wiring reachability", test_innate_wiring_counts_and_reachability},
        {"structural zero-weight path", test_structural_zero_weight_path_does_not_transmit},
        {"functional innate path", test_functional_innate_path_transmits_neurally},
        {"varied innate topology", test_varied_innate_topology_changes_paths},
        {"disabled innate wiring", test_disabled_innate_wiring_preserves_reference},
        {"exploration determinism", test_exploration_is_reproducible_and_rng_independent},
        {"exploration activation counts", test_single_and_multiple_exploration_counts},
        {"exploration budget", test_exploration_budget_is_enforced},
        {"localized exploration", test_localized_exploration_stays_in_region},
        {"distributed exploration", test_distributed_exploration_covers_regions},
        {"exploration response limit", test_exploration_respects_response_event_limit},
        {"disabled exploration", test_disabled_exploration_preserves_reference},
        {"direct motor completion", test_direct_motor_circuit_completes_neuronally},
        {"motor completion group", test_completion_group_uses_neural_relays},
        {"activity-dependent completion", test_activity_dependent_circuit_requires_multiple_events},
        {"neural null-byte completion", test_motor_circuit_can_complete_null_byte_neuronally},
        {"motor circuit reproducibility", test_motor_circuit_is_reproducible},
        {"motor circuit no sensor copy", test_motor_circuit_does_not_copy_sensory_input},
        {"disabled motor circuit", test_disabled_motor_circuit_preserves_reference},
        {"feedback after completion", test_feedback_after_completed_motor_response},
        {"feedback requires activity", test_feedback_requires_observed_activity},
        {"per-event feedback", test_per_motor_event_feedback_preserves_sequence},
        {"virtual environment feedback", test_virtual_environment_feedback_is_distinct},
        {"feedback event budget", test_feedback_event_budget_aborts_cascade},
        {"feedback channel separation", test_feedback_rejects_motor_channel_overlap},
        {"disabled feedback", test_disabled_feedback_preserves_reference},
        {"integrated sensorimotor cycle", test_integrated_sensorimotor_cycle},
        {"integrated sensorimotor restore", test_integrated_sensorimotor_state_restore},
    };

    std::size_t failures = 0;
    for (const auto& [name, test] : tests) {
        try {
            test();
            std::cout << "PASS  " << name << '\n';
        } catch (const std::exception& error) {
            ++failures;
            std::cerr << "FAIL  " << name << ": " << error.what() << '\n';
        }
    }
    std::cout << (tests.size() - failures) << '/' << tests.size() << " tests passed\n";
    return failures == 0 ? 0 : 1;
}
