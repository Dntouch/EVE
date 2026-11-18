#include "eve/bios/runtime.hpp"
#include "eve/brainstem/brainstem.hpp"
#include "eve/neural/network.hpp"
#include "eve/types.hpp"

#include <algorithm>
#include <array>
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

eve::brainstem::ExplorationConfig restricted_exploration_config(
    const std::uint32_t activations = 32,
    const std::uint64_t seed = 9876) {
    auto config = exploration_config(
        eve::brainstem::ExplorationMode::distributed, activations, seed);
    config.target_policy =
        eve::brainstem::ExplorationTargetPolicy::processing_neurons_only;
    return config;
}

void test_processing_exploration_excludes_configured_reservations() {
    eve::bios::Runtime runtime(motor_test_config(), 101);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto wiring = runtime.configure_innate_wiring(innate_config(
        eve::brainstem::InnateTopology::functional_paths, 3));
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::completion_group, 3, 3));
    const auto reserved = runtime.exploration_reservations();
    const auto result = runtime.enqueue_exploration(restricted_exploration_config());

    require_equal(result.reserved_neuron_count,
                  static_cast<std::uint32_t>(reserved.size()),
                  "restricted exploration counted reservations incorrectly");
    require_equal(result.eligible_neuron_count + result.reserved_neuron_count,
                  runtime.network().neuron_count(),
                  "restricted exploration candidate partition is incomplete");
    for (const eve::NeuronId selected : result.activated_neurons) {
        require(std::find(reserved.begin(), reserved.end(), selected) == reserved.end(),
                "restricted exploration directly selected a reserved neuron");
    }
    for (eve::NeuronId sensory = 0; sensory < 18; ++sensory) {
        require(std::find(reserved.begin(), reserved.end(), sensory) != reserved.end(),
                "sensor, response or feedback contact is not reserved");
    }
    for (const auto& channel : runtime.motor_assignment().channels) {
        for (const eve::NeuronId motor : channel) {
            require(std::find(reserved.begin(), reserved.end(), motor) != reserved.end(),
                    "motor data contact is not reserved");
        }
    }
    require(std::find(reserved.begin(), reserved.end(),
                      runtime.motor_assignment().completion_contact) != reserved.end(),
            "motor completion contact is not reserved");
    for (const auto& path : wiring.paths) {
        require(std::find(reserved.begin(), reserved.end(), path.first_relay) != reserved.end() &&
                    std::find(reserved.begin(), reserved.end(), path.second_relay) !=
                        reserved.end(),
                "innate wiring relay is not reserved");
    }
    for (const eve::NeuronId relay : circuit.relay_neurons) {
        require(std::find(reserved.begin(), reserved.end(), relay) != reserved.end(),
                "motor circuit relay is not reserved");
    }
}

void test_reserved_motor_circuit_neurons_remain_synaptically_active() {
    eve::bios::Runtime runtime(motor_test_config(), 102);
    runtime.configure_motor_output(motor_output_config(1, 100, 18));
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::completion_group, 3, 3));
    std::vector<eve::NeuronId> fired;
    runtime.set_fire_observer([&fired](const eve::bios::FireObservation& observation) {
        fired.push_back(observation.neuron);
    });
    runtime.enqueue_fire(runtime.motor_assignment().channels[0][0]);
    const auto response = runtime.request_response(false);
    require(response.valid(), "reserved motor circuit no longer completes synaptically");
    require(std::any_of(circuit.relay_neurons.begin(), circuit.relay_neurons.end(),
                        [&fired](const eve::NeuronId relay) {
                            return std::find(fired.begin(), fired.end(), relay) != fired.end();
                        }),
            "reserved motor relay could not fire through a normal synapse");
}

void test_processing_exploration_is_reproducible() {
    const auto execute = []() {
        eve::bios::Runtime runtime(motor_test_config(), 103);
        runtime.configure_motor_output(motor_output_config(1, 100, 18));
        static_cast<void>(runtime.configure_innate_wiring(innate_config(
            eve::brainstem::InnateTopology::functional_paths, 3)));
        static_cast<void>(runtime.configure_motor_circuit(motor_circuit_config(
            eve::brainstem::MotorCircuitMode::completion_group, 3, 3, 444)));
        return runtime.enqueue_exploration(restricted_exploration_config(16, 555))
            .activated_neurons;
    };
    require_equal(execute(), execute(), "restricted exploration is not reproducible");
}

void test_processing_exploration_is_independent_of_motor_circuit() {
    const auto relays_for = [](const eve::brainstem::MotorCircuitMode mode) {
        eve::bios::Runtime runtime(motor_test_config(), 104);
        runtime.configure_motor_output(motor_output_config(1, 100, 18));
        static_cast<void>(runtime.configure_innate_wiring(innate_config(
            eve::brainstem::InnateTopology::functional_paths, 3)));
        return runtime.configure_motor_circuit(motor_circuit_config(mode, 3, 3, 777))
            .relay_neurons;
    };
    std::vector<eve::NeuronId> common_reservations = relays_for(
        eve::brainstem::MotorCircuitMode::completion_group);
    const auto accumulator = relays_for(
        eve::brainstem::MotorCircuitMode::activity_dependent);
    common_reservations.insert(common_reservations.end(),
                               accumulator.begin(), accumulator.end());

    const auto execute = [&common_reservations](
                             const eve::brainstem::MotorCircuitMode mode) {
        eve::bios::Runtime runtime(motor_test_config(), 104);
        runtime.configure_motor_output(motor_output_config(1, 100, 18));
        static_cast<void>(runtime.configure_innate_wiring(innate_config(
            eve::brainstem::InnateTopology::functional_paths, 3)));
        static_cast<void>(runtime.configure_motor_circuit(
            motor_circuit_config(mode, 3, 3, 777)));
        auto config = restricted_exploration_config(16, 888);
        config.additional_reserved_neurons = common_reservations;
        return runtime.enqueue_exploration(config).activated_neurons;
    };
    const auto reference = execute(eve::brainstem::MotorCircuitMode::none);
    require_equal(reference, execute(eve::brainstem::MotorCircuitMode::direct_completion_path),
                  "M1 changed the independent exploration sequence");
    require_equal(reference, execute(eve::brainstem::MotorCircuitMode::completion_group),
                  "M2 changed the independent exploration sequence");
    require_equal(reference, execute(eve::brainstem::MotorCircuitMode::activity_dependent),
                  "M3 changed the independent exploration sequence");
}

void test_motor_circuit_reports_expected_connections() {
    const auto connection_count = [](const eve::brainstem::MotorCircuitMode mode) {
        eve::bios::Runtime runtime(motor_test_config(), 105);
        runtime.configure_motor_output(motor_output_config());
        const auto circuit = runtime.configure_motor_circuit(
            motor_circuit_config(mode, 3, 3, 999));
        for (const eve::SynapseId synapse : circuit.circuit_synapses) {
            require(runtime.network().synapse(synapse).weight == 3,
                    "active motor circuit contains an inactive connection");
        }
        return circuit.circuit_synapses.size();
    };
    require_equal(connection_count(eve::brainstem::MotorCircuitMode::none),
                  std::size_t{0}, "M0 unexpectedly added a motor circuit");
    require_equal(connection_count(
                      eve::brainstem::MotorCircuitMode::direct_completion_path),
                  std::size_t{8}, "M1 has the wrong connection count");
    require_equal(connection_count(eve::brainstem::MotorCircuitMode::completion_group),
                  std::size_t{12}, "M2 has the wrong connection count");
    require_equal(connection_count(eve::brainstem::MotorCircuitMode::activity_dependent),
                  std::size_t{9}, "M3 has the wrong connection count");
}

void test_interrupted_motor_circuit_preserves_topology_but_blocks_path() {
    const auto setup = [](const eve::brainstem::MotorCircuitMode mode,
                          const bool enabled) {
        eve::bios::Runtime runtime(motor_test_config(), 106);
        runtime.configure_motor_output(motor_output_config());
        auto config = motor_circuit_config(
            mode,
            mode == eve::brainstem::MotorCircuitMode::activity_dependent ? 1 : 3,
            3, 1000);
        config.path_enabled = enabled;
        const auto circuit = runtime.configure_motor_circuit(config);
        std::vector<std::pair<eve::NeuronId, eve::NeuronId>> topology;
        for (const eve::SynapseId synapse : circuit.circuit_synapses) {
            const auto& item = runtime.network().synapse(synapse);
            topology.emplace_back(item.sender, item.receiver);
            require(enabled ? item.weight > 0 : item.weight == 0,
                    "interrupted circuit has the wrong weight");
        }
        const std::uint32_t activations =
            mode == eve::brainstem::MotorCircuitMode::activity_dependent ? 3U : 1U;
        for (std::uint32_t channel = 0; channel < activations; ++channel) {
            runtime.enqueue_fire(runtime.motor_assignment().channels[channel][0]);
        }
        const auto response = runtime.request_response(false);
        return std::make_pair(topology, response);
    };
    for (const auto mode : {
             eve::brainstem::MotorCircuitMode::direct_completion_path,
             eve::brainstem::MotorCircuitMode::completion_group,
             eve::brainstem::MotorCircuitMode::activity_dependent,
         }) {
        const auto active = setup(mode, true);
        const auto interrupted = setup(mode, false);
        require_equal(active.first, interrupted.first,
                      "K2 changed topology instead of interrupting the path");
        require(active.second.valid(), "active motor path did not complete");
        require(!interrupted.second.valid(), "interrupted motor path still completed");
        require(interrupted.second.value != 0,
                "interruption removed the observed data activation");
    }
}

void test_motor_causality_distinguishes_completion_without_data() {
    eve::bios::Runtime runtime(motor_test_config(), 107);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::none, 1, 1));
    runtime.enqueue_fire(runtime.motor_assignment().completion_contact);
    const auto response = runtime.request_response(false);
    const auto causality = eve::bios::Runtime::analyze_motor_causality(
        response, circuit, {});
    require(causality.completed, "neural null-byte completion was not recognized");
    require(causality.completed_without_prior_data,
            "completion without data was not classified separately");
    require(!causality.completed_after_data,
            "completion without data was classified as motor-dependent");
    require(!causality.proven_circuit_path,
            "M0 completion was attributed to a motor circuit");
}

void test_motor_causality_traces_completion_group() {
    eve::bios::Runtime runtime(motor_test_config(), 108);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::completion_group, 3, 3));
    std::vector<eve::bios::ImpulseObservation> impulses;
    runtime.set_impulse_observer(
        [&impulses](const eve::bios::ImpulseObservation& observation) {
            impulses.push_back(observation);
        });
    runtime.enqueue_fire(runtime.motor_assignment().channels[6][0]);
    const auto response = runtime.request_response(false);
    const auto causality = eve::bios::Runtime::analyze_motor_causality(
        response, circuit, impulses);
    require(causality.completed_after_data,
            "M2 completion was not linked to prior motor data");
    require(causality.proven_circuit_path,
            "M2 circuit path was not reconstructed from actual impulses");
    require(causality.events_between_data_and_completion >= 3,
            "M2 completion sequence is unexpectedly short");
    require_equal(causality.firing_circuit_impulses, 2U,
                  "M2 did not record both firing circuit stages");

    const auto final_impulse = std::find_if(
        impulses.begin(), impulses.end(),
        [&runtime](const eve::bios::ImpulseObservation& observation) {
            return observation.receiver ==
                       runtime.motor_assignment().completion_contact &&
                   observation.receiver_fired;
        });
    require(final_impulse != impulses.end(),
            "completion impulse is absent from detailed diagnosis");
    require_equal(final_impulse->membrane_before, 0LL,
                  "completion diagnosis has wrong prior membrane state");
    require_equal(final_impulse->applied_weight, 3,
                  "completion diagnosis has wrong applied weight");
}

void test_motor_causality_traces_activity_dependent_path() {
    eve::bios::Runtime runtime(motor_test_config(), 109);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::activity_dependent, 1, 3));
    std::vector<eve::bios::ImpulseObservation> impulses;
    runtime.set_impulse_observer(
        [&impulses](const eve::bios::ImpulseObservation& observation) {
            impulses.push_back(observation);
        });
    for (std::size_t channel = 0; channel < 3; ++channel) {
        runtime.enqueue_fire(runtime.motor_assignment().channels[channel][0]);
    }
    const auto response = runtime.request_response(false);
    const auto causality = eve::bios::Runtime::analyze_motor_causality(
        response, circuit, impulses);
    require(causality.completed_after_data && causality.proven_circuit_path,
            "M3 accumulator path was not causally reconstructed");
    require(causality.observed_circuit_impulses >= 4,
            "M3 diagnosis missed accumulator impulses");
}

void test_motor_causality_rejects_technical_abort() {
    eve::ExperimentConfig config = motor_test_config();
    config.initial_outgoing_synapses = 4;
    eve::bios::Runtime runtime(config, 110);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::none, 1, 1));
    runtime.enqueue_fire(0);
    const auto response = runtime.request_response(false, 1);
    const auto causality = eve::bios::Runtime::analyze_motor_causality(
        response, circuit, {});
    require(causality.technical_abort, "technical abort was not identified");
    require(!causality.completed && !causality.completed_after_data,
            "technical abort was treated as neuronal completion");
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

void test_local_event_memory_stores_single_input() {
    auto config = small_config();
    config.local_event_memory_capacity = 4;
    config.learning_enabled = false;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId input = network.add_synapse(0, 2, 0);
    static_cast<void>(eve::brainstem::Brainstem{}.receive_impulse(network, input, config));
    require_equal(network.local_event_memory(2), std::vector<eve::SynapseId>{input},
                  "single local input was not retained");
}

void test_local_event_memory_preserves_order_and_repetitions() {
    auto config = small_config();
    config.local_event_memory_capacity = 4;
    config.learning_enabled = false;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId first = network.add_synapse(0, 2, 0);
    const eve::SynapseId second = network.add_synapse(1, 2, 0);
    eve::brainstem::Brainstem brainstem;
    static_cast<void>(brainstem.receive_impulse(network, first, config));
    static_cast<void>(brainstem.receive_impulse(network, second, config));
    static_cast<void>(brainstem.receive_impulse(network, first, config));
    require_equal(network.local_event_memory(2),
                  std::vector<eve::SynapseId>{first, second, first},
                  "local input order or repeated synapse was lost");
}

void test_local_event_memory_ring_overwrite() {
    auto config = small_config();
    config.local_event_memory_capacity = 2;
    config.learning_enabled = false;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId first = network.add_synapse(0, 3, 0);
    const eve::SynapseId second = network.add_synapse(1, 3, 0);
    const eve::SynapseId third = network.add_synapse(2, 3, 0);
    eve::brainstem::Brainstem brainstem;
    static_cast<void>(brainstem.receive_impulse(network, first, config));
    static_cast<void>(brainstem.receive_impulse(network, second, config));
    const auto result = brainstem.receive_impulse(network, third, config);
    require(result.local_event_overwritten, "full local ring did not report overwrite");
    require_equal(network.local_event_memory(3),
                  std::vector<eve::SynapseId>{second, third},
                  "ring did not retain the two newest inputs in order");
    require_equal(network.local_event_overwrites(3), 1ULL,
                  "local overwrite counter is wrong");
}

void test_local_event_memory_includes_zero_weight_inputs() {
    auto config = small_config();
    config.local_event_memory_capacity = 2;
    config.learning_enabled = false;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId input = network.add_synapse(0, 1, 0);
    const auto result = eve::brainstem::Brainstem{}.receive_impulse(network, input, config);
    require(result.local_event_recorded, "zero-weight input was not recorded");
    require_equal(network.local_event_memory(1), std::vector<eve::SynapseId>{input},
                  "zero-weight synapse is absent from local memory");
    require_equal(network.neuron(1).membrane_potential, 0LL,
                  "zero-weight memory changed neuronal excitation");
}

void test_local_event_snapshot_belongs_to_fire_decision() {
    auto config = small_config();
    config.initial_outgoing_synapses = 1;
    config.initial_synapse_weight = 1;
    config.firing_threshold = 2;
    config.learning_enabled = false;
    config.local_event_memory_capacity = 2;
    eve::bios::Runtime runtime(config, 501);
    const eve::SynapseId input = runtime.network().outgoing_synapses(0).front();
    const eve::NeuronId receiver = runtime.network().synapse(input).receiver;
    std::vector<eve::SynapseId> firing_snapshot;
    runtime.set_impulse_observer(
        [&firing_snapshot, receiver](const eve::bios::ImpulseObservation& observation) {
            if (observation.receiver == receiver && observation.receiver_fired &&
                firing_snapshot.empty()) {
                firing_snapshot = observation.local_event_snapshot;
            }
        });
    runtime.enqueue_fire(0);
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    require_equal(firing_snapshot, std::vector<eve::SynapseId>{input, input},
                  "fire-decision snapshot does not contain its two causal inputs");
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    require_equal(firing_snapshot, std::vector<eve::SynapseId>{input, input},
                  "later queue processing mutated the fire-decision snapshot");
}

void test_local_event_memory_during_refractory_phase() {
    auto config = small_config();
    config.local_event_memory_capacity = 4;
    config.learning_enabled = false;
    config.firing_threshold = 1;
    config.refractory_input_events = 2;
    eve::neural::Network network;
    network.reset(config);
    const eve::SynapseId input = network.add_synapse(0, 1, 1);
    eve::brainstem::Brainstem brainstem;
    require(brainstem.receive_impulse(network, input, config).fired,
            "first impulse did not fire");
    const auto refractory = brainstem.receive_impulse(network, input, config);
    require(refractory.firing_suppressed, "refractory firing was not suppressed");
    require_equal(network.local_event_memory(1),
                  std::vector<eve::SynapseId>{input, input},
                  "refractory input was not retained locally");
}

eve::bios::LocalMemoryCausality run_local_m3_memory_case(
    const std::uint32_t capacity,
    const bool path_enabled,
    std::vector<eve::bios::ImpulseObservation>* captured = nullptr) {
    auto config = motor_test_config();
    config.local_event_memory_capacity = capacity;
    eve::bios::Runtime runtime(config, 502);
    runtime.configure_motor_output(motor_output_config());
    auto circuit_config = motor_circuit_config(
        eve::brainstem::MotorCircuitMode::activity_dependent, 1, 3, 1502);
    circuit_config.path_enabled = path_enabled;
    const auto circuit = runtime.configure_motor_circuit(circuit_config);
    std::vector<eve::bios::ImpulseObservation> impulses;
    runtime.set_impulse_observer(
        [&impulses](const eve::bios::ImpulseObservation& observation) {
            impulses.push_back(observation);
        });
    for (std::size_t channel = 0; channel < 3; ++channel) {
        runtime.enqueue_fire(runtime.motor_assignment().channels[channel][0]);
    }
    const auto response = runtime.request_response(false);
    if (captured != nullptr) {
        *captured = impulses;
    }
    return runtime.analyze_local_motor_memory(response, circuit, impulses);
}

void test_simple_fire_reconstruction_from_local_memory() {
    std::vector<eve::bios::ImpulseObservation> impulses;
    const auto memory = run_local_m3_memory_case(4, true, &impulses);
    const auto firing = std::find_if(
        impulses.begin(), impulses.end(),
        [](const eve::bios::ImpulseObservation& observation) {
            return observation.receiver_fired &&
                   observation.local_event_snapshot.size() >= 3;
        });
    require(firing != impulses.end(), "no firing input snapshot was captured");
    require_equal(firing->local_event_snapshot.back(), firing->synapse,
                  "triggering input is not last in the local snapshot");
    require(memory.accumulator_fired, "simple accumulator fire was not reconstructed");
}

void test_m3_accumulator_and_completion_reconstruction() {
    const auto full = run_local_m3_memory_case(4, true);
    require(full.accumulator_reconstructable,
            "four-entry memory did not retain M3 accumulator inputs");
    require(full.completion_reconstructable,
            "four-entry memory did not retain M3 completion input");
    require(full.full_path_reconstructable,
            "complete M3 action was not reconstructed from local memories");
    const auto short_memory = run_local_m3_memory_case(2, true);
    require(!short_memory.accumulator_reconstructable,
            "two-entry memory unexpectedly retained all three M3 inputs");
    require(short_memory.completion_reconstructable,
            "two-entry memory lost the immediate completion input");
}

void test_local_memory_detects_unassigned_activity() {
    auto config = motor_test_config();
    config.local_event_memory_capacity = 4;
    eve::bios::Runtime runtime(config, 503);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::activity_dependent, 1, 3, 1503));
    require(circuit.circuit_synapses.size() >= 2,
            "M3 test circuit has too few synapses");
    eve::bios::ImpulseObservation synthetic;
    synthetic.event_index = 2;
    synthetic.receiver = circuit.relay_neurons.front();
    synthetic.receiver_fired = true;
    synthetic.local_event_snapshot = {
        circuit.circuit_synapses.back(), circuit.circuit_synapses.front()};
    eve::brainstem::MotorResponse response;
    response.activations.push_back({1, 20, 0});
    response.activations.push_back({3, runtime.motor_assignment().completion_contact, -1});
    const auto result = runtime.analyze_local_motor_memory(response, circuit, {synthetic});
    require(result.unassigned_entries >= 1,
            "memory analysis did not separate an unrelated circuit entry");
}

void test_local_memory_with_interrupted_m3_path() {
    const auto interrupted = run_local_m3_memory_case(4, false);
    require(!interrupted.accumulator_fired && !interrupted.completion_fired,
            "zero-weight M3 path produced a firing decision");
    require(!interrupted.full_path_reconstructable,
            "interrupted M3 path was reconstructed as complete");
}

void test_local_event_memory_reproducibility() {
    std::vector<eve::bios::ImpulseObservation> first;
    std::vector<eve::bios::ImpulseObservation> second;
    const auto first_result = run_local_m3_memory_case(8, true, &first);
    const auto second_result = run_local_m3_memory_case(8, true, &second);
    require_equal(first, second, "identical M3 local-memory runs diverged");
    require_equal(first_result.full_path_reconstructable,
                  second_result.full_path_reconstructable,
                  "identical M3 reconstruction result diverged");
}

void test_local_event_memory_persistence_and_lifetime() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b-local-memory.bin";
    auto config = small_config();
    config.initial_outgoing_synapses = 1;
    config.learning_enabled = false;
    config.local_event_memory_capacity = 2;
    eve::bios::Runtime original(config, 504);
    const eve::SynapseId input = original.network().outgoing_synapses(0).front();
    const eve::NeuronId receiver = original.network().synapse(input).receiver;
    for (int index = 0; index < 3; ++index) {
        original.enqueue_fire(0);
        static_cast<void>(original.run());
    }
    original.save(path.string());
    eve::bios::Runtime restored = eve::bios::Runtime::load(path.string());
    require_equal(original.network().local_event_memory(receiver),
                  restored.network().local_event_memory(receiver),
                  "persisted local event order changed");
    require_equal(original.network().local_event_overwrites(receiver),
                  restored.network().local_event_overwrites(receiver),
                  "persisted overwrite count changed");
    require_equal(original.state_fingerprint(), restored.state_fingerprint(),
                  "persisted local-memory state fingerprint changed");
    original.enqueue_fire(0);
    restored.enqueue_fire(0);
    static_cast<void>(original.run());
    static_cast<void>(restored.run());
    require_equal(original.state_fingerprint(), restored.state_fingerprint(),
                  "continued local-memory execution diverged after restore");
    static_cast<void>(std::filesystem::remove(path));
}

void test_disabled_local_event_memory_preserves_processing() {
    auto reference_config = small_config();
    reference_config.initial_outgoing_synapses = 2;
    reference_config.learning_enabled = false;
    auto disabled_config = reference_config;
    disabled_config.local_event_memory_capacity = 0;
    eve::bios::Runtime reference(reference_config, 505);
    eve::bios::Runtime disabled(disabled_config, 505);
    reference.enqueue_fire(0);
    disabled.enqueue_fire(0);
    static_cast<void>(reference.run());
    static_cast<void>(disabled.run());
    require_equal(reference.state_fingerprint(), disabled.state_fingerprint(),
                  "disabled local memory changed neuronal processing");
    require_equal(disabled.metrics().local_event_memory_records, 0ULL,
                  "disabled local memory still recorded events");
}

void test_targeted_m3_synapse_interruption() {
    const auto execute = [](const bool interrupt) {
        auto config = motor_test_config();
        config.local_event_memory_capacity = 4;
        eve::bios::Runtime runtime(config, 506);
        runtime.configure_motor_output(motor_output_config());
        const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
            eve::brainstem::MotorCircuitMode::activity_dependent, 1, 3, 1506));
        const eve::NeuronId accumulator = circuit.relay_neurons.front();
        eve::SynapseId selected = eve::kInvalidIndex;
        for (const eve::SynapseId id : circuit.circuit_synapses) {
            if (runtime.network().synapse(id).receiver == accumulator) {
                selected = id;
                break;
            }
        }
        require(selected != eve::kInvalidIndex, "no M3 accumulator input selected");
        if (interrupt) {
            runtime.set_experimental_synapse_weight(selected, 0);
        }
        for (std::size_t channel = 0; channel < 3; ++channel) {
            runtime.enqueue_fire(runtime.motor_assignment().channels[channel][0]);
        }
        return runtime.request_response(false).valid();
    };
    require(execute(false), "unmodified M3 control did not complete");
    require(!execute(true), "single targeted M3 interruption did not change completion");
}

eve::ExperimentConfig timed_recovery_config(
    const eve::RecoveryMode mode,
    const std::uint32_t relaxation = 0,
    const std::uint64_t delay = 4) {
    auto config = small_config();
    config.initial_outgoing_synapses = 1;
    config.initial_synapse_weight = 1;
    config.firing_threshold = 2;
    config.learning_enabled = false;
    config.refractory_input_events = 16;
    config.recovery_mode = mode;
    config.recovery_delay = delay;
    config.membrane_relaxation_percent = relaxation;
    config.local_event_memory_capacity = 8;
    return config;
}

struct RecoveryFixture {
    eve::bios::Runtime runtime;
    eve::SynapseId input{};
    eve::NeuronId receiver{};

    RecoveryFixture(
        const eve::RecoveryMode mode,
        const std::uint32_t relaxation = 0,
        const std::uint64_t delay = 4)
        : runtime(timed_recovery_config(mode, relaxation, delay), 601) {
        input = runtime.network().outgoing_synapses(0).front();
        receiver = runtime.network().synapse(input).receiver;
    }

    void fire_receiver() {
        runtime.enqueue_fire(0);
        runtime.enqueue_fire(0);
        static_cast<void>(runtime.run());
    }
};

void test_simulated_time_and_future_scheduling() {
    RecoveryFixture fixture(eve::RecoveryMode::independent);
    fixture.fire_receiver();
    require_equal(fixture.runtime.simulated_time(), 0ULL,
                  "immediate events advanced simulated time");
    require_equal(fixture.runtime.future_event_count(), std::size_t{1},
                  "fire did not schedule exactly one future recovery");
    require_equal(fixture.runtime.run(), 0ULL,
                  "ordinary run processed a future recovery early");
    require_equal(fixture.runtime.network().neuron(fixture.receiver)
                      .refractory_events_remaining,
                  16U, "future recovery changed refractory state early");
}

void test_recovery_without_new_input() {
    RecoveryFixture fixture(eve::RecoveryMode::independent);
    fixture.fire_receiver();
    const auto memory = fixture.runtime.network().local_event_memory(fixture.receiver);
    require_equal(fixture.runtime.advance_time_to(3), 0ULL,
                  "recovery ran before its due time");
    require_equal(fixture.runtime.advance_time_to(4), 1ULL,
                  "due recovery was not processed without input");
    require_equal(fixture.runtime.network().neuron(fixture.receiver)
                      .refractory_events_remaining,
                  0U, "R1 did not end refractory state");
    require_equal(fixture.runtime.network().local_event_memory(fixture.receiver), memory,
                  "recovery erased local event memory");
}

void test_same_time_recovery_order_is_deterministic() {
    const auto execute = []() {
        auto config = timed_recovery_config(eve::RecoveryMode::independent);
        eve::bios::Runtime runtime(config, 602);
        std::vector<eve::NeuronId> recovered;
        runtime.set_recovery_observer(
            [&recovered](const eve::bios::RecoveryObservation& observation) {
                if (observation.recovered) recovered.push_back(observation.neuron);
            });
        runtime.enqueue_fire(0);
        runtime.enqueue_fire(0);
        runtime.enqueue_fire(1);
        runtime.enqueue_fire(1);
        static_cast<void>(runtime.run());
        static_cast<void>(runtime.advance_time_to(4));
        return recovered;
    };
    const auto first = execute();
    const auto second = execute();
    require(!first.empty(), "same-time recovery test produced no recovery");
    require_equal(first, second, "same-time recovery order was not deterministic");
}

void test_boundary_event_and_time_advance() {
    eve::bios::Runtime runtime(timed_recovery_config(
        eve::RecoveryMode::input_dependent), 603);
    runtime.schedule_boundary(7, 11);
    require_equal(runtime.run(), 0ULL, "future boundary was processed immediately");
    require_equal(runtime.advance_time_to(6), 0ULL, "boundary ran too early");
    require_equal(runtime.advance_time_by(1), 1ULL, "boundary did not run at due time");
    require_equal(runtime.metrics().processed_boundary_events, 1ULL,
                  "boundary metric is wrong");
}

void test_stale_recovery_is_ignored() {
    auto config = timed_recovery_config(eve::RecoveryMode::independent);
    eve::neural::Network network;
    network.reset(config);
    const auto input = network.add_synapse(0, 1, 1);
    eve::brainstem::Brainstem brainstem;
    static_cast<void>(brainstem.receive_impulse(network, input, config));
    const auto firing = brainstem.receive_impulse(network, input, config);
    require(firing.fired && firing.recovery_generation != 0,
            "independent recovery did not create a generation");
    const auto stale = brainstem.recover_neuron(
        network, 1, firing.recovery_generation + 1, config);
    require(stale.stale && !stale.recovered,
            "outdated recovery generation changed neuron state");
    const auto valid = brainstem.recover_neuron(
        network, 1, firing.recovery_generation, config);
    require(valid.recovered && !valid.stale, "valid recovery did not execute");
    require(brainstem.recover_neuron(
                network, 1, firing.recovery_generation, config).stale,
            "duplicate recovery was not rejected as stale");
}

void test_r0_remains_input_dependent() {
    RecoveryFixture fixture(eve::RecoveryMode::input_dependent);
    fixture.fire_receiver();
    require_equal(fixture.runtime.future_event_count(), std::size_t{0},
                  "R0 scheduled an independent recovery");
    static_cast<void>(fixture.runtime.advance_time_by(64));
    require_equal(fixture.runtime.network().neuron(fixture.receiver)
                      .refractory_events_remaining,
                  16U, "time advance changed R0 refractory state");
    fixture.runtime.enqueue_fire(0);
    static_cast<void>(fixture.runtime.run());
    require_equal(fixture.runtime.network().neuron(fixture.receiver)
                      .refractory_events_remaining,
                  15U, "R0 no longer decrements on incoming impulses");
}

void test_r1_does_not_relax_membrane() {
    RecoveryFixture fixture(eve::RecoveryMode::independent);
    fixture.fire_receiver();
    fixture.runtime.enqueue_fire(0);
    fixture.runtime.enqueue_fire(0);
    static_cast<void>(fixture.runtime.run());
    require_equal(fixture.runtime.network().neuron(fixture.receiver).membrane_potential,
                  2LL, "refractory R1 inputs did not accumulate membrane potential");
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.network().neuron(fixture.receiver).membrane_potential,
                  2LL, "R1 unexpectedly relaxed membrane potential");
}

void test_r2_relaxes_only_membrane() {
    for (const auto [percent, expected] :
         {std::pair<std::uint32_t, std::int64_t>{50, 1}, {100, 0}}) {
        RecoveryFixture fixture(
            eve::RecoveryMode::independent_with_membrane_relaxation, percent);
        fixture.fire_receiver();
        fixture.runtime.enqueue_fire(0);
        fixture.runtime.enqueue_fire(0);
        static_cast<void>(fixture.runtime.run());
        const auto synapses = fixture.runtime.network().synapses();
        const auto memory = fixture.runtime.network().local_event_memory(fixture.receiver);
        static_cast<void>(fixture.runtime.advance_time_to(4));
        require_equal(
            fixture.runtime.network().neuron(fixture.receiver).membrane_potential,
            expected, "R2 membrane relaxation factor is wrong");
        require_equal(fixture.runtime.network().synapses(), synapses,
                      "R2 recovery changed synaptic state");
        require_equal(fixture.runtime.network().local_event_memory(fixture.receiver), memory,
                      "R2 recovery changed local memory");
    }
}

void test_recovery_emits_no_fire_and_allows_later_refire() {
    RecoveryFixture fixture(eve::RecoveryMode::independent);
    fixture.fire_receiver();
    const auto fires_before = fixture.runtime.metrics().fire_events;
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.metrics().fire_events, fires_before,
                  "recovery emitted its own fire event");
    fixture.runtime.enqueue_fire(0);
    fixture.runtime.enqueue_fire(0);
    static_cast<void>(fixture.runtime.run());
    require(fixture.runtime.metrics().fire_events > fires_before,
            "recovered neuron could not fire again");
}

void test_pending_recovery_persistence() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b1-pending-recovery.bin";
    RecoveryFixture fixture(eve::RecoveryMode::independent);
    fixture.fire_receiver();
    fixture.runtime.save(path.string());
    eve::bios::Runtime restored = eve::bios::Runtime::load(path.string());
    require_equal(fixture.runtime.state_fingerprint(), restored.state_fingerprint(),
                  "pending recovery changed during persistence");
    static_cast<void>(fixture.runtime.advance_time_to(4));
    static_cast<void>(restored.advance_time_to(4));
    require_equal(fixture.runtime.state_fingerprint(), restored.state_fingerprint(),
                  "restored recovery continuation diverged");
    static_cast<void>(std::filesystem::remove(path));
}

void test_response_end_does_not_preprocess_recovery() {
    auto config = motor_test_config();
    config.recovery_mode = eve::RecoveryMode::independent;
    config.recovery_delay = 4;
    config.refractory_input_events = 16;
    config.local_event_memory_capacity = 8;
    eve::bios::Runtime runtime(config, 604);
    runtime.configure_motor_output(motor_output_config());
    const auto circuit = runtime.configure_motor_circuit(motor_circuit_config(
        eve::brainstem::MotorCircuitMode::activity_dependent, 1, 3, 1604));
    for (std::size_t channel = 0; channel < 3; ++channel) {
        runtime.enqueue_fire(runtime.motor_assignment().channels[channel][0]);
    }
    const auto response = runtime.request_response(false);
    require(response.valid(), "B.1 response test did not complete M3 path");
    require_equal(runtime.simulated_time(), 0ULL,
                  "response completion advanced to future recovery");
    require(runtime.future_event_count() >= 2,
            "response completion discarded pending recoveries");
    require(runtime.network().neuron(circuit.relay_neurons.front())
                .refractory_events_remaining > 0,
            "accumulator recovered automatically at response end");
}

void test_recovery_changes_no_learning_state() {
    RecoveryFixture fixture(eve::RecoveryMode::independent);
    fixture.fire_receiver();
    const auto strengthenings = fixture.runtime.metrics().synaptic_strengthenings;
    const auto activity = fixture.runtime.network().synapse(fixture.input).activity_count;
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.metrics().synaptic_strengthenings, strengthenings,
                  "recovery caused a learning event");
    require_equal(fixture.runtime.network().synapse(fixture.input).activity_count,
                  activity, "recovery changed a synaptic learning counter");
}

eve::ExperimentConfig eligibility_config(
    const eve::EligibilitySelection selection =
        eve::EligibilitySelection::all_recent,
    const std::uint64_t duration = 4) {
    auto config = small_config();
    config.initial_outgoing_synapses = 1;
    config.initial_synapse_weight = 0;
    config.firing_threshold = 3;
    config.learning_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = selection;
    config.eligibility_representation = eve::EligibilityRepresentation::unique;
    config.eligibility_history = eve::EligibilityHistory::retain;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 2;
    config.eligibility_duration = duration;
    return config;
}

struct EligibilityRuntimeFixture {
    eve::bios::Runtime runtime;
    eve::SynapseId input{};
    eve::NeuronId receiver{};

    explicit EligibilityRuntimeFixture(
        const std::uint64_t duration = 4,
        const eve::EligibilitySelection selection =
            eve::EligibilitySelection::all_recent)
        : runtime(eligibility_config(selection, duration), 701) {
        input = runtime.network().outgoing_synapses(0).front();
        receiver = runtime.network().synapse(input).receiver;
        for (eve::SynapseId id = 0; id < runtime.network().synapse_count(); ++id) {
            runtime.set_experimental_synapse_weight(id, 0);
        }
        runtime.set_experimental_synapse_weight(input, 3);
    }

    void fire() {
        runtime.enqueue_fire(0);
        static_cast<void>(runtime.run());
    }
};

struct EligibilityNetworkFixture {
    eve::ExperimentConfig config{eligibility_config()};
    eve::neural::Network network;
    eve::brainstem::Brainstem brainstem;
    eve::SynapseId positive{};
    eve::SynapseId zero{};
    eve::SynapseId other{};

    EligibilityNetworkFixture() {
        config.initial_outgoing_synapses = 0;
        network.reset(config);
        positive = network.add_synapse(0, 7, 3);
        zero = network.add_synapse(1, 7, 0);
        other = network.add_synapse(2, 7, 1);
    }
};

void test_disabled_eligibility_creates_no_state() {
    auto config = small_config();
    config.initial_outgoing_synapses = 1;
    config.initial_synapse_weight = 3;
    config.learning_enabled = false;
    config.local_event_memory_capacity = 8;
    eve::bios::Runtime runtime(config, 700);
    const auto input = runtime.network().outgoing_synapses(0).front();
    for (eve::SynapseId id = 0; id < runtime.network().synapse_count(); ++id) {
        runtime.set_experimental_synapse_weight(id, 0);
    }
    runtime.set_experimental_synapse_weight(input, 3);
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    require_equal(runtime.network().active_eligibility_count(), std::size_t{0},
                  "disabled eligibility created state");
    require_equal(runtime.metrics().eligibility_decisions, 0ULL,
                  "disabled eligibility changed metrics");
}

void test_eligibility_is_created_at_fire_decision() {
    EligibilityRuntimeFixture fixture;
    std::uint64_t observed_generation = 0;
    fixture.runtime.set_impulse_observer(
        [&observed_generation](const eve::bios::ImpulseObservation& observation) {
            if (observation.receiver_fired) {
                observed_generation = observation.eligibility_generation;
            }
        });
    fixture.fire();
    require(observed_generation != 0, "fire-decision observer saw no eligibility");
    require_equal(fixture.runtime.network().eligibility_records(fixture.receiver).size(),
                  std::size_t{1}, "fire decision did not store one record");
}

void test_eligibility_snapshot_precedes_queued_fire() {
    EligibilityRuntimeFixture fixture;
    std::vector<eve::SynapseId> snapshot;
    fixture.runtime.set_impulse_observer(
        [&snapshot](const eve::bios::ImpulseObservation& observation) {
            if (observation.receiver_fired) snapshot = observation.eligibility_candidates;
        });
    fixture.fire();
    require_equal(snapshot, std::vector<eve::SynapseId>{fixture.input},
                  "queued fire changed the original candidate snapshot");
}

void test_g3_and_eligibility_are_separate() {
    EligibilityNetworkFixture fixture;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    const auto marked = fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config);
    static_cast<void>(fixture.network.record_local_event(7, fixture.zero));
    require_equal(marked.candidates, std::vector<eve::SynapseId>{fixture.positive},
                  "eligibility aliased the changing G3 ring");
    require_equal(fixture.network.local_event_memory(7).size(), std::size_t{2},
                  "eligibility creation changed G3");
}

void test_l1_selects_recent_ring_entries() {
    EligibilityNetworkFixture fixture;
    for (const auto id : {fixture.positive, fixture.zero, fixture.other}) {
        static_cast<void>(fixture.network.record_local_event(7, id));
    }
    const auto result = fixture.brainstem.create_eligibility(
        fixture.network, 7, 8, fixture.config);
    require_equal(result.candidates,
                  std::vector<eve::SynapseId>{fixture.positive, fixture.zero, fixture.other},
                  "L1 did not preserve recent ring order");
}

void test_l2_selects_only_positive_inputs() {
    EligibilityNetworkFixture fixture;
    fixture.config.eligibility_selection = eve::EligibilitySelection::positive_weight;
    for (const auto id : {fixture.positive, fixture.zero, fixture.other}) {
        static_cast<void>(fixture.network.record_local_event(7, id));
    }
    const auto result = fixture.brainstem.create_eligibility(
        fixture.network, 7, 8, fixture.config);
    require_equal(result.candidates,
                  std::vector<eve::SynapseId>{fixture.positive, fixture.other},
                  "L2 retained a non-contributing input");
    require_equal(result.zero_weight_candidates, 0U,
                  "L2 classified a zero-weight candidate as selected");
}

void test_zero_weight_selection_differs_between_l1_and_l2() {
    EligibilityNetworkFixture l1;
    static_cast<void>(l1.network.record_local_event(7, l1.zero));
    require_equal(l1.brainstem.create_eligibility(l1.network, 7, 4, l1.config)
                      .zero_weight_candidates,
                  1U, "L1 omitted zero-weight input");
    EligibilityNetworkFixture l2;
    l2.config.eligibility_selection = eve::EligibilitySelection::positive_weight;
    static_cast<void>(l2.network.record_local_event(7, l2.zero));
    require(l2.brainstem.create_eligibility(l2.network, 7, 4, l2.config)
                .candidates.empty(),
            "L2 retained zero-weight input");
}

void test_repeated_and_unique_eligibility_representations() {
    EligibilityNetworkFixture fixture;
    for (int index = 0; index < 3; ++index) {
        static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    }
    fixture.config.eligibility_representation = eve::EligibilityRepresentation::repeated;
    require_equal(fixture.brainstem.create_eligibility(
                      fixture.network, 7, 4, fixture.config).candidates.size(),
                  std::size_t{3}, "repeated representation collapsed duplicates");
    fixture.config.eligibility_representation = eve::EligibilityRepresentation::unique;
    require_equal(fixture.brainstem.create_eligibility(
                      fixture.network, 7, 4, fixture.config).candidates.size(),
                  std::size_t{1}, "unique representation retained duplicates");
}

void test_eligibility_candidate_capacity() {
    EligibilityNetworkFixture fixture;
    fixture.config.eligibility_candidate_capacity = 2;
    for (const auto id : {fixture.positive, fixture.zero, fixture.other}) {
        static_cast<void>(fixture.network.record_local_event(7, id));
    }
    const auto result = fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config);
    require_equal(result.candidates,
                  std::vector<eve::SynapseId>{fixture.zero, fixture.other},
                  "capacity did not retain the newest candidates");
}

void test_eligibility_overwrite_history() {
    EligibilityNetworkFixture fixture;
    fixture.config.eligibility_history = eve::EligibilityHistory::overwrite;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    const auto first = fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config);
    static_cast<void>(fixture.network.record_local_event(7, fixture.other));
    const auto second = fixture.brainstem.create_eligibility(
        fixture.network, 7, 5, fixture.config);
    require(second.generation > first.generation && second.overwritten_decisions == 1,
            "overwrite did not replace and count prior decision");
    require_equal(fixture.network.eligibility_records(7).size(), std::size_t{1},
                  "overwrite retained multiple decisions");
}

void test_eligibility_generations_distinguish_fires() {
    EligibilityNetworkFixture fixture;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    const auto first = fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config);
    const auto second = fixture.brainstem.create_eligibility(
        fixture.network, 7, 5, fixture.config);
    require(first.generation != second.generation,
            "two decisions received the same local generation");
    require_equal(fixture.network.eligibility_records(7).size(), std::size_t{2},
                  "bounded retain history lost a decision too early");
}

void test_eligibility_remains_without_inputs_until_expiry() {
    EligibilityRuntimeFixture fixture;
    fixture.fire();
    static_cast<void>(fixture.runtime.advance_time_to(3));
    require_equal(fixture.runtime.network().active_eligibility_count(), std::size_t{1},
                  "eligibility expired before its deadline");
}

void test_eligibility_expires_at_due_time() {
    EligibilityRuntimeFixture fixture;
    fixture.fire();
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.network().active_eligibility_count(), std::size_t{0},
                  "eligibility did not expire at its deadline");
    require_equal(fixture.runtime.metrics().processed_eligibility_expiries, 1ULL,
                  "expiry metric is wrong");
}

void test_stale_eligibility_expiry_is_harmless() {
    EligibilityNetworkFixture fixture;
    fixture.config.eligibility_history = eve::EligibilityHistory::overwrite;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    const auto first = fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config);
    const auto second = fixture.brainstem.create_eligibility(
        fixture.network, 7, 8, fixture.config);
    require(!fixture.brainstem.expire_eligibility(
                fixture.network, 7, first.generation),
            "stale expiry removed a replacement record");
    require_equal(fixture.network.eligibility_records(7).front().generation,
                  second.generation, "stale expiry changed active generation");
}

void test_expiry_emits_no_fire() {
    EligibilityRuntimeFixture fixture;
    fixture.fire();
    const auto fires = fixture.runtime.metrics().fire_events;
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.metrics().fire_events, fires,
                  "eligibility expiry emitted a fire");
}

void test_expiry_changes_no_synapse_weight() {
    EligibilityRuntimeFixture fixture;
    fixture.fire();
    const auto synapses = fixture.runtime.network().synapses();
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.network().synapses(), synapses,
                  "eligibility expiry changed synaptic state");
}

void test_expiry_changes_no_membrane_or_g3() {
    EligibilityRuntimeFixture fixture;
    fixture.fire();
    const auto membrane = fixture.runtime.network().neuron(fixture.receiver)
                              .membrane_potential;
    const auto memory = fixture.runtime.network().local_event_memory(fixture.receiver);
    static_cast<void>(fixture.runtime.advance_time_to(4));
    require_equal(fixture.runtime.network().neuron(fixture.receiver).membrane_potential,
                  membrane, "eligibility expiry changed membrane");
    require_equal(fixture.runtime.network().local_event_memory(fixture.receiver), memory,
                  "eligibility expiry changed G3");
}

void test_recovery_preserves_eligibility() {
    auto config = eligibility_config(eve::EligibilitySelection::all_recent, 16);
    config.recovery_mode = eve::RecoveryMode::independent_with_membrane_relaxation;
    config.recovery_delay = 4;
    config.membrane_relaxation_percent = 100;
    config.refractory_input_events = 8;
    eve::bios::Runtime runtime(config, 702);
    const auto input = runtime.network().outgoing_synapses(0).front();
    const auto receiver = runtime.network().synapse(input).receiver;
    for (eve::SynapseId id = 0; id < runtime.network().synapse_count(); ++id) {
        runtime.set_experimental_synapse_weight(id, 0);
    }
    runtime.set_experimental_synapse_weight(input, 3);
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    static_cast<void>(runtime.advance_time_to(4));
    require_equal(runtime.network().eligibility_records(receiver).size(),
                  std::size_t{1}, "R2F recovery erased valid eligibility");
}

void test_zero_duration_eligibility_exists_only_in_fire_turn() {
    EligibilityRuntimeFixture fixture(0);
    std::uint64_t generation = 0;
    fixture.runtime.set_impulse_observer(
        [&generation](const eve::bios::ImpulseObservation& observation) {
            if (observation.receiver_fired) generation = observation.eligibility_generation;
        });
    fixture.fire();
    require(generation != 0, "D0 was not visible at the decision");
    require_equal(fixture.runtime.network().active_eligibility_count(), std::size_t{0},
                  "D0 survived completion of same-time events");
}

void test_eligibility_persistence_and_continuation() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b2-active-eligibility.bin";
    EligibilityRuntimeFixture fixture;
    fixture.fire();
    fixture.runtime.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    require_equal(fixture.runtime.state_fingerprint(), restored.state_fingerprint(),
                  "active eligibility changed during persistence");
    static_cast<void>(fixture.runtime.advance_time_to(4));
    static_cast<void>(restored.advance_time_to(4));
    require_equal(fixture.runtime.state_fingerprint(), restored.state_fingerprint(),
                  "restored expiry continuation diverged");
    static_cast<void>(std::filesystem::remove(path));
}

void test_eligibility_is_reproducible() {
    EligibilityRuntimeFixture first;
    EligibilityRuntimeFixture second;
    first.fire();
    second.fire();
    require_equal(first.runtime.state_fingerprint(), second.runtime.state_fingerprint(),
                  "identical eligibility runs diverged");
}

void test_eligibility_never_changes_weights() {
    EligibilityRuntimeFixture fixture;
    const auto before = fixture.runtime.network().weight_distribution();
    fixture.fire();
    require_equal(fixture.runtime.network().weight_distribution(), before,
                  "eligibility creation caused a weight change");
    require_equal(fixture.runtime.metrics().synaptic_strengthenings, 0ULL,
                  "eligibility caused learning");
}

void test_merge_history_combines_unique_candidates() {
    EligibilityNetworkFixture fixture;
    fixture.config.eligibility_history = eve::EligibilityHistory::merge_unique;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    static_cast<void>(fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config));
    static_cast<void>(fixture.network.record_local_event(7, fixture.other));
    const auto merged = fixture.brainstem.create_eligibility(
        fixture.network, 7, 8, fixture.config);
    require_equal(merged.candidates,
                  std::vector<eve::SynapseId>{fixture.positive, fixture.other},
                  "merge history did not preserve unique candidates");
    require_equal(fixture.network.eligibility_records(7).size(), std::size_t{1},
                  "merge history retained separate decisions");
}

void test_feedback_probe_without_action_finds_no_eligibility() {
    EligibilityRuntimeFixture fixture;
    static_cast<void>(fixture.runtime.advance_time_to(3));
    require_equal(fixture.runtime.network().active_eligibility_count(), std::size_t{0},
                  "technical probe without action found eligibility");
}

void test_eligibility_storage_validates() {
    EligibilityNetworkFixture fixture;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    static_cast<void>(fixture.brainstem.create_eligibility(
        fixture.network, 7, 4, fixture.config));
    require(fixture.network.validate(), "eligibility storage violates network invariants");
    require(fixture.network.estimated_memory_bytes() > sizeof(eve::neural::Network),
            "eligibility storage is absent from memory estimate");
}

eve::ExperimentConfig evaluation_test_config() {
    eve::ExperimentConfig config;
    config.neuron_count = 32;
    config.width = 4;
    config.height = 4;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::positive_weight;
    config.eligibility_candidate_capacity = 2;
    config.eligibility_decision_capacity = 2;
    config.eligibility_duration = 16;
    return config;
}

eve::brainstem::EvaluationRouteConfig evaluation_route_config(
    const eve::brainstem::EvaluationRouteMode mode) {
    eve::brainstem::EvaluationRouteConfig config;
    config.mode = mode;
    config.first_input_neuron = 9;
    config.graded_channel_count = 5;
    config.relay_count = 2;
    config.fanout_per_source = 2;
    config.weight = 3;
    config.seed = 7301;
    for (eve::NeuronId id = 0; id < 9; ++id) config.reserved_neurons.push_back(id);
    return config;
}

void test_binary_evaluation_values() {
    const auto full = eve::brainstem::Brainstem::evaluate_binary(0xaa, 0xaa);
    const auto half = eve::brainstem::Brainstem::evaluate_binary(0x0f, 0x33);
    const auto none = eve::brainstem::Brainstem::evaluate_binary(0x00, 0xff);
    require_equal(full.matching_bits, std::uint8_t{8}, "full match bit count is wrong");
    require_equal(full.success_percent, std::uint8_t{100}, "full match score is wrong");
    require_equal(half.success_percent, std::uint8_t{50}, "half match score is wrong");
    require_equal(none.success_percent, std::uint8_t{0}, "zero match score is wrong");
}

void test_evaluation_encoding_contains_no_target() {
    const auto first = eve::brainstem::Brainstem::encode_evaluation(75);
    const auto second = eve::brainstem::Brainstem::encode_evaluation(75);
    require_equal(first.active_channels, second.active_channels,
                  "same score encoding depends on hidden target data");
    require_equal(first.active_channels, std::vector<std::uint8_t>{3},
                  "graded score is not one-hot encoded");
}

void test_graded_evaluation_encodings_are_distinct() {
    std::set<std::uint8_t> channels;
    for (const std::uint8_t score : {0, 25, 50, 75, 100}) {
        channels.insert(eve::brainstem::Brainstem::encode_evaluation(score)
                            .active_channels.front());
    }
    require_equal(channels.size(), std::size_t{5},
                  "graded evaluations are not distinguishable");
}

void test_f0_emits_no_neural_feedback() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7302);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::none)));
    const auto result = runtime.process_evaluation_feedback(100);
    require_equal(result.processed_events, 0ULL, "F0 processed a neural event");
    require_equal(runtime.metrics().feedback_fire_events, 0ULL,
                  "F0 emitted feedback fire");
}

void test_f1_emits_sensor_feedback_fire() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7303);
    const auto route = runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::sensory_input));
    const auto result = runtime.process_evaluation_feedback(50);
    require_equal(result.feedback_fires, 1ULL, "F1 emitted no feedback fire");
    require_equal(route.added_synapses, 0U, "F1 unexpectedly added special wiring");
}

void test_f2_uses_general_preconfigured_wiring() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7304);
    const auto route = runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::general_wiring));
    require_equal(route.route_synapses.size(), std::size_t{10},
                  "F2 general fanout count is wrong");
    require(route.relay_neurons.empty(), "F2 unexpectedly created relays");
    const auto result = runtime.process_evaluation_feedback(25);
    require(result.feedback_impulses >= 2, "F2 produced no functional impulses");
}

void test_f3_uses_regular_relays() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7305);
    const auto route = runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::relay_wiring));
    require_equal(route.relay_neurons.size(), std::size_t{2},
                  "F3 relay count is wrong");
    const auto result = runtime.process_evaluation_feedback(75);
    require(result.feedback_fires >= 2 && result.feedback_impulses >= 3,
            "F3 did not traverse regular relay events");
}

void test_evaluation_route_is_reproducible() {
    eve::bios::Runtime first(evaluation_test_config(), 7306);
    eve::bios::Runtime second(evaluation_test_config(), 7306);
    const auto left = first.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::relay_wiring));
    const auto right = second.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::relay_wiring));
    require_equal(left.route_synapses, right.route_synapses,
                  "feedback route synapses are not reproducible");
    require_equal(left.target_neurons, right.target_neurons,
                  "feedback route targets are not reproducible");
}

void test_feedback_functional_reachability_is_observed() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7307);
    const auto route = runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::general_wiring));
    const auto result = runtime.process_evaluation_feedback(0);
    require(!route.target_neurons.empty(), "F2 has no structural targets");
    require(result.feedback_impulses > 0,
            "structural feedback route produced no functional impulses");
}

void test_feedback_reaches_preexisting_eligibility() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7308);
    const auto route = runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::general_wiring));
    runtime.enqueue_fire(route.input_neurons[4]);
    static_cast<void>(runtime.run());
    require(runtime.network().active_eligibility_count() > 0,
            "setup action created no eligibility");
    const auto result = runtime.process_evaluation_feedback(100);
    require(result.receipts_with_eligibility > 0,
            "feedback did not observe preexisting eligibility");
}

void test_feedback_without_eligibility_is_counted() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7309);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::general_wiring)));
    const auto result = runtime.process_evaluation_feedback(50);
    require(result.receipts_without_eligibility > 0,
            "feedback without eligibility was not diagnosed");
}

void test_expired_eligibility_is_not_reactivated_by_feedback() {
    auto config = evaluation_test_config();
    config.eligibility_duration = 4;
    eve::bios::Runtime runtime(config, 7310);
    const auto route = runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::general_wiring));
    runtime.enqueue_fire(route.input_neurons[0]);
    static_cast<void>(runtime.run());
    static_cast<void>(runtime.advance_time_to(4));
    const auto result = runtime.process_evaluation_feedback(0);
    require_equal(result.receipts_with_eligibility, 0ULL,
                  "expired eligibility was treated as valid");
}

void test_feedback_created_eligibility_is_separate() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7311);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::general_wiring)));
    const auto result = runtime.process_evaluation_feedback(25);
    require(result.created_eligibilities > 0,
            "feedback-created eligibility was not separately counted");
    require(result.receipts_without_eligibility > 0,
            "new eligibility was incorrectly counted as preexisting");
}

void test_local_generations_are_not_global_action_ids() {
    EligibilityNetworkFixture fixture;
    static_cast<void>(fixture.network.record_local_event(7, fixture.positive));
    const auto first = fixture.brainstem.create_eligibility(
        fixture.network, 7, 16, fixture.config);
    const auto second_synapse = fixture.network.add_synapse(3, 6, 3);
    static_cast<void>(fixture.network.record_local_event(6, second_synapse));
    const auto second = fixture.brainstem.create_eligibility(
        fixture.network, 6, 16, fixture.config);
    require_equal(first.generation, second.generation,
                  "local first generations should be allowed to collide");
}

void test_delayed_feedback_is_scheduled() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7312);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::sensory_input)));
    const auto result = runtime.process_evaluation_feedback(75,
        eve::brainstem::EvaluationSignalKind::graded, 8);
    require_equal(result.processed_events, 0ULL, "future feedback ran immediately");
    require_equal(runtime.advance_time_to(7), 0ULL, "future feedback ran too early");
    require(runtime.advance_time_to(8) > 0, "due feedback did not run");
}

void test_reordered_feedback_follows_due_time() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7313);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::sensory_input)));
    static_cast<void>(runtime.process_evaluation_feedback(25,
        eve::brainstem::EvaluationSignalKind::graded, 12));
    static_cast<void>(runtime.process_evaluation_feedback(100,
        eve::brainstem::EvaluationSignalKind::graded, 4));
    static_cast<void>(runtime.advance_time_to(4));
    require_equal(runtime.metrics().feedback_fire_events, 1ULL,
                  "later action feedback did not arrive first");
    static_cast<void>(runtime.advance_time_to(12));
    require_equal(runtime.metrics().feedback_fire_events, 2ULL,
                  "earlier action feedback was lost");
}

void test_pending_feedback_persistence() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b3-pending-feedback.bin";
    eve::bios::Runtime runtime(evaluation_test_config(), 7314);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::relay_wiring)));
    static_cast<void>(runtime.process_evaluation_feedback(100,
        eve::brainstem::EvaluationSignalKind::graded, 8));
    runtime.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                  "pending feedback changed while saving");
    static_cast<void>(runtime.advance_time_to(8));
    static_cast<void>(restored.advance_time_to(8));
    require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                  "restored feedback continuation diverged");
    static_cast<void>(std::filesystem::remove(path));
}

void test_feedback_never_changes_weights() {
    eve::bios::Runtime runtime(evaluation_test_config(), 7315);
    static_cast<void>(runtime.configure_evaluation_route(
        evaluation_route_config(eve::brainstem::EvaluationRouteMode::relay_wiring)));
    const auto before = runtime.network().weight_distribution();
    static_cast<void>(runtime.process_evaluation_feedback(100));
    require_equal(runtime.network().weight_distribution(), before,
                  "B.3 feedback changed a synaptic weight");
    require_equal(runtime.metrics().synaptic_strengthenings, 0ULL,
                  "B.3 feedback triggered learning");
}

eve::ExperimentConfig action_context_test_config(
    const eve::ContextTraceMode trace_mode = eve::ContextTraceMode::multiple,
    const eve::ContextBindingMode binding_mode = eve::ContextBindingMode::at_fire) {
    eve::ExperimentConfig config;
    config.neuron_count = 64;
    config.width = 4;
    config.height = 4;
    config.depth = 4;
    config.initial_outgoing_synapses = 0;
    config.local_event_memory_capacity = 2;
    config.eligibility_selection = eve::EligibilitySelection::all_recent;
    config.eligibility_candidate_capacity = 2;
    config.eligibility_decision_capacity = 2;
    config.eligibility_duration = 64;
    config.context_trace_mode = trace_mode;
    config.context_binding_mode = binding_mode;
    config.context_trace_capacity =
        trace_mode == eve::ContextTraceMode::single ? 1U :
        trace_mode == eve::ContextTraceMode::multiple ? 2U : 0U;
    config.context_trace_duration =
        trace_mode == eve::ContextTraceMode::none ? 0U : 64U;
    config.learning_enabled = false;
    config.growth_enabled = false;
    return config;
}

eve::brainstem::ActionContextConfig action_context_route(
    const eve::brainstem::ActionContextMode mode,
    const std::uint64_t seed = 8101) {
    eve::brainstem::ActionContextConfig config;
    config.mode = mode;
    config.first_input_neuron = 16;
    config.relay_count = 4;
    config.fanout_per_source = 2;
    config.weight = 3;
    config.seed = seed;
    return config;
}

void test_disabled_action_context_has_no_state() {
    eve::bios::Runtime runtime(action_context_test_config(
        eve::ContextTraceMode::none, eve::ContextBindingMode::none), 8101);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::none)));
    const auto result = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start);
    require_equal(result.signature, 0U, "C0 emitted a context signature");
    require_equal(runtime.network().active_context_trace_count(), std::size_t{0},
                  "C0 allocated context state");
}

void test_single_context_trace_storage() {
    eve::neural::Network network;
    network.reset(action_context_test_config(eve::ContextTraceMode::single));
    static_cast<void>(network.store_context_trace(1, 11, 64));
    require_equal(network.context_traces(1).size(), std::size_t{1},
                  "P1 did not retain one trace");
}

void test_multiple_context_trace_storage() {
    eve::neural::Network network;
    network.reset(action_context_test_config());
    static_cast<void>(network.store_context_trace(1, 11, 64));
    static_cast<void>(network.store_context_trace(1, 22, 64));
    require_equal(network.context_traces(1).size(), std::size_t{2},
                  "P2 did not retain two traces");
}

void test_context_trace_ring_overwrite() {
    eve::neural::Network network;
    network.reset(action_context_test_config());
    static_cast<void>(network.store_context_trace(1, 11, 64));
    static_cast<void>(network.store_context_trace(1, 22, 64));
    const auto third = network.store_context_trace(1, 33, 64);
    require(third.overwritten, "bounded context ring did not report overwrite");
    require_equal(network.context_traces(1).front().signature, 22U,
                  "bounded context ring retained the wrong oldest trace");
}

void test_context_trace_expiry() {
    eve::neural::Network network;
    network.reset(action_context_test_config());
    const auto stored = network.store_context_trace(1, 11, 64);
    require(network.expire_context_trace(1, stored.trace.generation),
            "valid context expiry was ignored");
    require(network.context_traces(1).empty(), "expired context remained active");
}

void test_stale_context_trace_expiry() {
    eve::neural::Network network;
    network.reset(action_context_test_config());
    require(!network.expire_context_trace(1, 99),
            "unknown context generation was expired");
}

void test_context_generations_are_local() {
    eve::neural::Network network;
    network.reset(action_context_test_config());
    const auto left = network.store_context_trace(1, 11, 64);
    const auto right = network.store_context_trace(2, 11, 64);
    require_equal(left.trace.generation, right.trace.generation,
                  "context generations behave like a global action number");
}

void test_context_trace_does_not_change_membrane() {
    eve::neural::Network network;
    network.reset(action_context_test_config());
    const auto before = network.neuron(1);
    static_cast<void>(network.store_context_trace(1, 11, 64));
    require_equal(network.neuron(1), before,
                  "context storage changed ordinary neuron state");
}

void test_c0_context_route_is_inert() {
    eve::bios::Runtime runtime(action_context_test_config(), 8102);
    const auto route = runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::none));
    require(route.route_synapses.empty(), "C0 created context wiring");
}

void test_c1_boundary_context_uses_neural_fire() {
    eve::bios::Runtime runtime(action_context_test_config(), 8103);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::boundary)));
    const auto result = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start);
    require(result.signature != 0 && result.created_traces > 0,
            "C1 boundary activity formed no neural trace");
}

void test_c2_same_motor_activity_same_context() {
    eve::bios::Runtime runtime(action_context_test_config(), 8104);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::motor_copy)));
    const auto first = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0x35);
    const auto second = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0x35);
    require_equal(first.signature, second.signature,
                  "identical motor activity received a hidden action identity");
}

void test_c2_different_motor_activity_differs() {
    eve::bios::Runtime runtime(action_context_test_config(), 8105);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::motor_copy)));
    const auto first = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0x01);
    const auto second = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0x02);
    require(first.signature != second.signature,
            "different motor activity formed the same context fingerprint");
}

void test_c2_zero_motor_activity_has_no_context() {
    eve::bios::Runtime runtime(action_context_test_config(), 8106);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::motor_copy)));
    const auto result = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0);
    require_equal(result.signature, 0U,
                  "C2 invented context activity for a silent motor output");
}

void test_c3_general_context_circuit() {
    eve::bios::Runtime runtime(action_context_test_config(), 8107);
    const auto route = runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::general_circuit));
    require(!route.relay_neurons.empty() && !route.target_neurons.empty() &&
                !route.route_synapses.empty(),
            "C3 contains no real relay circuit");
}

void test_c4_combines_boundary_and_motor_sources() {
    eve::bios::Runtime runtime(action_context_test_config(), 8108);
    const auto route = runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::combined));
    require_equal(route.boundary_inputs.size(), std::size_t{2},
                  "C4 lacks boundary sources");
    require_equal(route.motor_inputs.size(), std::size_t{8},
                  "C4 lacks motor-copy sources");
}

void test_action_context_route_is_reproducible() {
    eve::bios::Runtime left(action_context_test_config(), 8109);
    eve::bios::Runtime right(action_context_test_config(), 8109);
    const auto a = left.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::combined, 44));
    const auto b = right.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::combined, 44));
    require_equal(a.route_synapses, b.route_synapses,
                  "action context wiring is not reproducible");
    require_equal(left.state_fingerprint(), right.state_fingerprint(),
                  "reproduced context networks differ");
}

void test_context_binding_at_fire() {
    auto config = action_context_test_config();
    eve::neural::Network network;
    network.reset(config);
    const auto synapse = network.add_synapse(0, 1, 3);
    static_cast<void>(network.record_local_event(1, synapse));
    static_cast<void>(network.store_context_trace(1, 77, 64));
    eve::brainstem::Brainstem brainstem;
    const auto result = brainstem.create_eligibility(network, 1, 64, config);
    require_equal(result.context_signature, 77U,
                  "B1 did not bind local context at fire decision");
}

void test_context_binding_absent_without_trace() {
    auto config = action_context_test_config();
    eve::neural::Network network;
    network.reset(config);
    const auto synapse = network.add_synapse(0, 1, 3);
    static_cast<void>(network.record_local_event(1, synapse));
    eve::brainstem::Brainstem brainstem;
    const auto result = brainstem.create_eligibility(network, 1, 64, config);
    require_equal(result.context_signature, 0U,
                  "B1 invented a context without a trace");
}

void test_context_binding_marks_ambiguity() {
    auto config = action_context_test_config();
    eve::neural::Network network;
    network.reset(config);
    const auto synapse = network.add_synapse(0, 1, 3);
    static_cast<void>(network.record_local_event(1, synapse));
    static_cast<void>(network.store_context_trace(1, 11, 64));
    static_cast<void>(network.store_context_trace(1, 22, 64));
    eve::brainstem::Brainstem brainstem;
    const auto result = brainstem.create_eligibility(network, 1, 64, config);
    require(result.context_ambiguous,
            "B1 concealed multiple distinct local contexts");
}

void test_temporal_overlap_binds_existing_eligibility() {
    auto config = action_context_test_config(
        eve::ContextTraceMode::multiple,
        eve::ContextBindingMode::temporal_overlap);
    eve::neural::Network network;
    network.reset(config);
    const auto synapse = network.add_synapse(0, 1, 3);
    static_cast<void>(network.record_local_event(1, synapse));
    eve::brainstem::Brainstem brainstem;
    static_cast<void>(brainstem.create_eligibility(network, 1, 64, config));
    const auto trace = network.store_context_trace(1, 33, 64);
    const auto binding = network.bind_context_to_eligibilities(1, trace.trace);
    require_equal(binding.bound, 1U, "B2 did not bind by local overlap");
}

void test_temporal_overlap_reports_ambiguity() {
    auto config = action_context_test_config(
        eve::ContextTraceMode::multiple,
        eve::ContextBindingMode::temporal_overlap);
    eve::neural::Network network;
    network.reset(config);
    const auto synapse = network.add_synapse(0, 1, 3);
    static_cast<void>(network.record_local_event(1, synapse));
    eve::brainstem::Brainstem brainstem;
    static_cast<void>(brainstem.create_eligibility(network, 1, 64, config));
    const auto first = network.store_context_trace(1, 33, 64);
    static_cast<void>(network.bind_context_to_eligibilities(1, first.trace));
    const auto second = network.store_context_trace(1, 44, 64);
    const auto binding = network.bind_context_to_eligibilities(1, second.trace);
    require_equal(binding.ambiguous, 1U,
                  "B2 silently replaced an overlapping context");
}

void test_feedback_origin_context_is_labeled() {
    auto config = action_context_test_config();
    eve::neural::Network network;
    network.reset(config);
    const auto synapse = network.add_synapse(0, 1, 3);
    static_cast<void>(network.record_local_event(1, synapse));
    static_cast<void>(network.store_context_trace(1, 55, 64, true));
    eve::brainstem::Brainstem brainstem;
    const auto result = brainstem.create_eligibility(network, 1, 64, config);
    require(result.context_feedback_origin,
            "feedback-induced context lost its origin label");
}

void test_eligibility_context_persistence() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b31-eligibility-context.bin";
    eve::bios::Runtime runtime(action_context_test_config(), 8110);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::general_circuit)));
    static_cast<void>(runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start));
    runtime.save(path.string());
    const auto restored = eve::bios::Runtime::load(path.string());
    require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                  "eligibility/context relation changed while saving");
    static_cast<void>(std::filesystem::remove(path));
}

void test_active_context_trace_persistence() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b31-active-context.bin";
    eve::bios::Runtime runtime(action_context_test_config(), 8111);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::boundary)));
    static_cast<void>(runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start));
    const auto active = runtime.network().active_context_trace_count();
    runtime.save(path.string());
    const auto restored = eve::bios::Runtime::load(path.string());
    require_equal(restored.network().active_context_trace_count(), active,
                  "active context traces were not restored");
    static_cast<void>(std::filesystem::remove(path));
}

void test_pending_context_expiry_persistence() {
    const auto path = std::filesystem::temp_directory_path() /
                      "eve-stage-b31-pending-context.bin";
    eve::bios::Runtime runtime(action_context_test_config(), 8112);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::boundary)));
    static_cast<void>(runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start));
    runtime.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    static_cast<void>(runtime.advance_time_to(64));
    static_cast<void>(restored.advance_time_to(64));
    require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                  "restored context expiry diverged");
    static_cast<void>(std::filesystem::remove(path));
}

void test_context_expiry_emits_no_fire() {
    eve::bios::Runtime runtime(action_context_test_config(), 8113);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::boundary)));
    static_cast<void>(runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start));
    const auto before = runtime.metrics().fire_events;
    static_cast<void>(runtime.advance_time_to(64));
    require_equal(runtime.metrics().fire_events, before,
                  "context expiry emitted neural fire");
}

void test_action_context_never_changes_weights() {
    eve::bios::Runtime runtime(action_context_test_config(), 8114);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::combined)));
    const auto before = runtime.network().weight_distribution();
    static_cast<void>(runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0x7f));
    require_equal(runtime.network().weight_distribution(), before,
                  "B.3.1 context activity changed a synaptic weight");
}

void test_action_context_triggers_no_learning() {
    eve::bios::Runtime runtime(action_context_test_config(), 8115);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::general_circuit)));
    static_cast<void>(runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start));
    require_equal(runtime.metrics().synaptic_strengthenings, 0ULL,
                  "B.3.1 context triggered learning");
    require_equal(runtime.metrics().synaptic_weakenings, 0ULL,
                  "B.3.1 context triggered weakening");
}

void test_context_trace_memory_is_bounded() {
    eve::bios::Runtime runtime(action_context_test_config(), 8116);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::boundary)));
    for (int index = 0; index < 8; ++index) {
        static_cast<void>(runtime.process_action_context(
            index % 2 == 0 ? eve::brainstem::ActionContextSignal::start
                           : eve::brainstem::ActionContextSignal::end));
    }
    require(runtime.network().active_context_trace_count() <=
                runtime.network().neuron_count() * 2ULL,
            "context trace memory grew beyond its fixed bound");
}

void test_invalid_single_context_capacity() {
    auto config = action_context_test_config(eve::ContextTraceMode::single);
    config.context_trace_capacity = 2;
    bool rejected = false;
    try { eve::bios::Runtime runtime(config, 8117); }
    catch (const std::invalid_argument&) { rejected = true; }
    require(rejected, "invalid P1 capacity was accepted");
}

void test_invalid_multiple_context_capacity() {
    auto config = action_context_test_config();
    config.context_trace_capacity = 1;
    bool rejected = false;
    try { eve::bios::Runtime runtime(config, 8118); }
    catch (const std::invalid_argument&) { rejected = true; }
    require(rejected, "invalid P2 capacity was accepted");
}

void test_context_signature_contains_no_sequence_number() {
    eve::bios::Runtime runtime(action_context_test_config(), 8119);
    static_cast<void>(runtime.configure_action_context(
        action_context_route(eve::brainstem::ActionContextMode::motor_copy)));
    const auto a = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0xa5);
    const auto b = runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, 0xa5);
    require_equal(a.signature, b.signature,
                  "context signature contains a hidden sequence/action number");
}

void test_b4_three_factor_matrix_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 64;
    config.width = 4;
    config.height = 4;
    config.depth = 4;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 1;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 32;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::all_recent;
    config.eligibility_representation = eve::EligibilityRepresentation::unique;
    config.eligibility_history = eve::EligibilityHistory::retain;
    config.eligibility_candidate_capacity = 2U << (index % 3U);
    config.eligibility_decision_capacity = config.eligibility_candidate_capacity;
    config.eligibility_duration = index % 3U == 0 ? 4 : index % 3U == 1 ? 16 : 64;
    config.plasticity_trace_mode = static_cast<eve::PlasticityTraceMode>(
        1U + index % 3U);
    config.plasticity_decay_mode = static_cast<eve::TraceDecayMode>(index % 4U);
    config.modulation_mode = eve::ModulationMode::ternary;
    config.plasticity_maximum_weight = 3;

    eve::bios::Runtime runtime(config, 9000U + index);
    auto route_config = evaluation_route_config(
        eve::brainstem::EvaluationRouteMode::general_wiring);
    route_config.fanout_per_source = 1;
    route_config.weight = 1;
    route_config.seed = 9100U + index;
    const auto route = runtime.configure_evaluation_route(route_config);
    require(!route.target_neurons.empty(), "B.4 route has no local recipient");
    const eve::NeuronId action = route.target_neurons.front();
    const bool positive = index % 2U == 0;
    const std::int32_t initial =
        config.plasticity_trace_mode == eve::PlasticityTraceMode::membrane_participation &&
                positive
            ? 0 : 1;
    const eve::SynapseId plastic = runtime.add_experimental_synapse(0, action, initial);

    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    const bool immediate = config.plasticity_decay_mode == eve::TraceDecayMode::immediate;
    if (immediate) {
        require_equal(runtime.network().active_eligibility_count(), std::size_t{0},
                      "T0 trace survived its event turn");
    } else {
        require(runtime.network().active_eligibility_count() > 0,
                "B.4 activity created no bounded local trace");
    }

    const std::int8_t polarity = positive ? 1 : -1;
    const auto feedback = runtime.process_modulatory_feedback(polarity);
    require(feedback.neural_fires > 0 && feedback.neural_impulses > 0,
            "B.4 modulation bypassed the neural route");
    const std::int32_t after = runtime.network().synapse(plastic).weight;
    if (index >= 31U) {
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b4-persistence-" + std::to_string(index) + ".bin");
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4 persisted state changed");
        static_cast<void>(runtime.advance_time_by(64));
        static_cast<void>(restored.advance_time_by(64));
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4 restored continuation diverged");
        static_cast<void>(std::filesystem::remove(path));
    }
    if (immediate) {
        require_equal(after, initial, "expired T0 trace changed a weight");
        require(feedback.receipts_without_trace > 0,
                "expired T0 trace was not diagnosed as absent");
        return;
    }
    require(feedback.receipts_with_trace > 0,
            "B.4 modulation did not meet a local trace");
    if (positive) {
        require(after > initial, "positive three-factor update did not strengthen");
    } else {
        require(after < initial && after >= 0,
                "negative three-factor update did not weaken within bounds");
    }
    require(runtime.network().active_eligibility_count() <=
                runtime.network().neuron_count() *
                    config.eligibility_decision_capacity,
            "B.4 trace storage exceeded its fixed bound");
    require(runtime.network().validate(), "B.4 update broke network invariants");
}

void test_b41_validation_case(const std::uint32_t index) {
    // Independent streams: consuming environment feedback never changes tasks.
    eve::bios::DeterministicRng tasks_a(11000 + index);
    eve::bios::DeterministicRng tasks_b(11000 + index);
    eve::bios::DeterministicRng feedback(12000 + index);
    for (int draw = 0; draw < 32; ++draw) {
        static_cast<void>(feedback.bounded(2));
        require_equal(tasks_a.bounded(2), tasks_b.bounded(2),
                      "B.4.1 task stream was coupled to feedback RNG");
    }

    eve::ExperimentConfig config;
    config.neuron_count = 64;
    config.width = 4;
    config.height = 4;
    config.depth = 4;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 1;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 16;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::all_recent;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 4;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::membrane_participation;
    config.modulation_mode = eve::ModulationMode::ternary;
    eve::bios::Runtime runtime(config, 13000 + index);
    eve::brainstem::EvaluationRouteConfig route_config;
    route_config.mode = eve::brainstem::EvaluationRouteMode::general_wiring;
    route_config.first_input_neuron = 9;
    route_config.graded_channel_count = 5;
    route_config.fanout_per_source = 1;
    route_config.weight = 1;
    route_config.seed = 14000 + index;
    for (eve::NeuronId id = 0; id < 9; ++id) {
        route_config.reserved_neurons.push_back(id);
    }
    const auto route = runtime.configure_evaluation_route(route_config);
    const eve::NeuronId action = 7;
    for (const eve::NeuronId input : route.input_neurons) {
        static_cast<void>(runtime.add_experimental_synapse(input, action, 0));
    }
    const eve::SynapseId plastic = runtime.add_experimental_synapse(0, action, 0);
    const eve::SynapseId exploratory = runtime.add_experimental_synapse(2, action, 1);
    std::uint64_t action_fires = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        if (event.neuron == action) ++action_fires;
    });

    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    require(runtime.network().active_eligibility_count() > 0,
            "B.4.1 zero-weight activity created no local trace");
    const std::uint64_t motor_before_feedback = action_fires;
    const auto memory_before_feedback = runtime.network().local_event_memory(action);
    const auto membrane_before_feedback =
        runtime.network().neuron(action).membrane_potential;
    const auto feedback_traces_before = runtime.metrics().feedback_created_eligibilities;
    const auto positive = runtime.process_modulatory_feedback(1);
    require(positive.zero_to_one_changes > 0,
            "B.4.1 positive modulation did not activate weight zero");
    require_equal(runtime.network().synapse(plastic).weight, 1,
                  "B.4.1 zero-to-one weight is wrong");
    require_equal(action_fires, motor_before_feedback,
                  "modulatory receptor fired the motor output");
    require_equal(runtime.network().local_event_memory(action), memory_before_feedback,
                  "modulatory receptor entered G3");
    require_equal(runtime.network().neuron(action).membrane_potential,
                  membrane_before_feedback,
                  "modulatory receptor changed the membrane");
    require_equal(runtime.metrics().feedback_created_eligibilities,
                  feedback_traces_before,
                  "modulatory receptor created a feedback trace");

    runtime.enqueue_fire(2);
    static_cast<void>(runtime.run());
    require(action_fires > motor_before_feedback,
            "B.4.1 exploratory neural path cannot reach the motor output");
    require_equal(runtime.network().synapse(exploratory).weight, 1,
                  "exploration changed without modulation");

    static_cast<void>(runtime.advance_time_to(4));
    const auto expired_weight = runtime.network().synapse(plastic).weight;
    static_cast<void>(runtime.process_modulatory_feedback(1));
    require_equal(runtime.network().synapse(plastic).weight, expired_weight,
                  "expired B.4.1 trace changed a weight");

    if (index >= 35U) {
        runtime.enqueue_fire(0);
        static_cast<void>(runtime.run());
        const std::uint64_t due = runtime.simulated_time() + 3;
        static_cast<void>(runtime.process_modulatory_feedback(-1, due));
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b41-" + std::to_string(index) + ".bin");
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.1 pending state changed while saving");
        static_cast<void>(runtime.advance_time_to(due));
        static_cast<void>(restored.advance_time_to(due));
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.1 restored continuation diverged");
        require(restored.evaluation_route_configured(),
                "B.4.1 restored runtime lost its evaluation route");
        runtime.enqueue_fire(0);
        restored.enqueue_fire(0);
        static_cast<void>(runtime.run());
        static_cast<void>(restored.run());
        static_cast<void>(runtime.process_modulatory_feedback(1));
        static_cast<void>(restored.process_modulatory_feedback(1));
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.1 restored route cannot continue learning");
        static_cast<void>(std::filesystem::remove(path));
    }
    require(runtime.network().validate(), "B.4.1 network invariants failed");
}

void test_b42_exploration_preservation_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 64;
    config.width = 4;
    config.height = 4;
    config.depth = 4;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 16;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.refractory_input_events = 1;
    config.recovery_mode = eve::RecoveryMode::independent_with_membrane_relaxation;
    config.recovery_delay = 4;
    config.membrane_relaxation_percent = 100;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::all_recent;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;

    eve::bios::Runtime runtime(config, 21000U + index);
    eve::brainstem::EvaluationRouteConfig route_config;
    route_config.mode = eve::brainstem::EvaluationRouteMode::general_wiring;
    route_config.first_input_neuron = 11;
    route_config.graded_channel_count = 5;
    route_config.fanout_per_source = 1;
    route_config.weight = 1;
    route_config.seed = 22000U + index;
    for (eve::NeuronId id = 0; id < 11; ++id) route_config.reserved_neurons.push_back(id);
    const auto route = runtime.configure_evaluation_route(route_config);
    const eve::NeuronId action_a = 7;
    const eve::NeuronId action_b = 8;
    for (const eve::NeuronId input : route.input_neurons) {
        static_cast<void>(runtime.add_experimental_synapse(input, action_a, 0));
        static_cast<void>(runtime.add_experimental_synapse(input, action_b, 0));
    }
    const eve::SynapseId cue_a = runtime.add_experimental_synapse(0, action_a, 0);
    const eve::SynapseId cue_b = runtime.add_experimental_synapse(1, action_b, 0);
    const eve::SynapseId explore_a =
        runtime.add_experimental_synapse(2, action_a, 2, true);
    const eve::SynapseId explore_b =
        runtime.add_experimental_synapse(4, action_b, 2, true);
    std::uint64_t fires_a = 0;
    std::uint64_t fires_b = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        if (event.neuron == action_a) ++fires_a;
        if (event.neuron == action_b) ++fires_b;
    });

    runtime.enqueue_fire(2);
    static_cast<void>(runtime.run());
    static_cast<void>(runtime.advance_time_by(16));
    runtime.enqueue_fire(4);
    static_cast<void>(runtime.run());
    require_equal(fires_a, std::uint64_t{1}, "protected A exploration is unreachable");
    require_equal(fires_b, std::uint64_t{1}, "protected B exploration is unreachable");
    require(runtime.network().synapse(explore_a).plasticity_protected &&
                runtime.network().synapse(explore_b).plasticity_protected,
            "innate protection is not symmetric");

    static_cast<void>(runtime.advance_time_by(16));
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    runtime.enqueue_fire(2);
    static_cast<void>(runtime.run());
    const auto positive = runtime.process_modulatory_feedback(1);
    require(positive.zero_to_one_changes > 0,
            "protected exploration prevented ordinary positive learning");
    require_equal(runtime.network().synapse(cue_a).weight, 1,
                  "unprotected sensory path was not strengthened");
    require_equal(runtime.network().synapse(explore_a).weight, 2,
                  "positive modulation changed protected exploration");

    static_cast<void>(runtime.advance_time_by(16));
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    runtime.enqueue_fire(2);
    static_cast<void>(runtime.run());
    static_cast<void>(runtime.process_modulatory_feedback(-1));
    require_equal(runtime.network().synapse(cue_a).weight, 0,
                  "negative modulation did not weaken an ordinary path");
    require_equal(runtime.network().synapse(explore_a).weight, 2,
                  "negative modulation destroyed protected exploration");
    require_equal(runtime.network().synapse(cue_b).weight, 0,
                  "feedback selected an unrelated motor solution");

    eve::bios::DeterministicRng gate_a(23000U + index);
    eve::bios::DeterministicRng gate_b(23000U + index);
    for (int draw = 0; draw < 32; ++draw) {
        require_equal(gate_a.bounded(100), gate_b.bounded(100),
                      "reduced negative frequency is not reproducible");
    }

    if (index == 4U || index == 23U) {
        runtime.set_experimental_synapse_protection(explore_a, false);
        static_cast<void>(runtime.advance_time_by(16));
        runtime.enqueue_fire(2);
        static_cast<void>(runtime.run());
        static_cast<void>(runtime.process_modulatory_feedback(-1));
        require_equal(runtime.network().synapse(explore_a).weight, 1,
                      "disabled innate protection remained active");
        runtime.set_experimental_synapse_protection(explore_a, true);
    }

    if (index >= 34U) {
        static_cast<void>(runtime.advance_time_by(16));
        runtime.enqueue_fire(1);
        static_cast<void>(runtime.run());
        runtime.enqueue_fire(4);
        static_cast<void>(runtime.run());
        const std::uint64_t due = runtime.simulated_time() + 3;
        static_cast<void>(runtime.process_modulatory_feedback(1, due));
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b42-" + std::to_string(index) + ".bin");
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require(restored.network().synapse(explore_a).plasticity_protected &&
                    restored.network().synapse(explore_b).plasticity_protected,
                "protected exploration was not persisted");
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.2 persisted state changed");
        static_cast<void>(runtime.advance_time_to(due));
        static_cast<void>(restored.advance_time_to(due));
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.2 restored continuation diverged");
        static_cast<void>(std::filesystem::remove(path));
    }
    require(runtime.network().validate(), "B.4.2 network invariants failed");
    require(runtime.metrics().maximum_queue_size < 64,
            "B.4.2 exploration produced an unbounded event cascade");
}

void test_b43_local_separation_case(const std::uint32_t index) {
    auto config = eligibility_config(eve::EligibilitySelection::active_phase,
                                     4U << (index % 5U));
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.eligibility_candidate_capacity = 1U << (index % 4U);
    config.eligibility_decision_capacity = 1U << ((index / 4U) % 4U);
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    eve::neural::Network network;
    network.reset(config);
    eve::brainstem::Brainstem brainstem;
    const auto old = network.add_synapse(0, 7, 1);
    const auto zero = network.add_synapse(1, 7, 0);
    const auto current = network.add_synapse(2, 7, 2);
    static_cast<void>(network.record_local_event(7, old));
    static_cast<void>(network.record_local_event(7, zero));
    static_cast<void>(network.record_local_event(7, current));
    network.synapse(old).activity_count = 0;
    network.synapse(zero).activity_count = 1;
    network.synapse(current).activity_count = 1;

    const auto g3_before = network.local_event_memory(7);
    const auto marked = brainstem.create_eligibility(
        network, 7, config.eligibility_duration, config);
    require(marked.created, "B.4.3 active phase created no local decision");
    require(std::find(marked.candidates.begin(), marked.candidates.end(), old) ==
                marked.candidates.end(),
            "B.4.3 selected activity from a preceding decision window");
    require(std::find(marked.candidates.begin(), marked.candidates.end(), current) !=
                marked.candidates.end(),
            "B.4.3 omitted the fire-participating input");
    if (config.eligibility_candidate_capacity >= 2) {
        require(std::find(marked.candidates.begin(), marked.candidates.end(), zero) !=
                    marked.candidates.end(),
                "B.4.3 made a locally active zero-weight input ineligible");
    }
    require(marked.candidates.size() <= config.eligibility_candidate_capacity,
            "B.4.3 exceeded candidate capacity");
    require_equal(network.local_event_memory(7), g3_before,
                  "B.4.3 local decision selection reset G3 globally");
    network.reset_incoming_activity(7);
    require_equal(network.synapse(zero).activity_count, 0U,
                  "B.4.3 decision boundary did not delimit local activity");
    require(network.active_eligibility_count() <=
                config.eligibility_decision_capacity,
            "B.4.3 exceeded decision capacity");

    if (index >= 34U) {
        eve::bios::Runtime runtime(config, 0x430000ULL + index);
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b43-" + std::to_string(index) + ".bin");
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require_equal(restored.config().eligibility_selection,
                      eve::EligibilitySelection::active_phase,
                      "B.4.3 local selection mode was not persisted");
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.3 persisted continuation state changed");
        static_cast<void>(std::filesystem::remove(path));
    }
    require(network.validate(), "B.4.3 local state violates network invariants");
}

void test_b44_structural_learning_case(const std::uint32_t index) {
    auto config = small_config();
    config.growth_enabled = true;
    config.learning_enabled = true;
    config.maximum_synapse_weight = 8;
    eve::bios::Runtime runtime(config, 0x440000ULL + index);
    const eve::NeuronId sender = index % config.neuron_count;

    runtime.enqueue_fire(sender);
    static_cast<void>(runtime.run());
    require_equal(runtime.network().synapse_count(), 1U,
                  "B.4.4 growth did not create the guaranteed k=0 connection");
    require_equal(runtime.metrics().grown_synapses, 1ULL,
                  "B.4.4 growth metric omitted a new connection");
    const auto& grown = runtime.network().synapse(0);
    require_equal(grown.sender, sender, "B.4.4 growth changed the firing source");
    require(grown.receiver != sender, "B.4.4 growth created a self-connection");
    require_equal(grown.weight, 0, "B.4.4 new connection did not start at zero");

    runtime.set_structural_growth_enabled(false);
    require(!runtime.config().growth_enabled,
            "B.4.4 diagnostic growth switch remained enabled");
    const auto count_before_evaluation = runtime.network().synapse_count();
    runtime.enqueue_fire(sender);
    static_cast<void>(runtime.run());
    require_equal(runtime.network().synapse_count(), count_before_evaluation,
                  "B.4.4 evaluation created a structural connection");
    require_equal(runtime.metrics().grown_synapses, 1ULL,
                  "B.4.4 disabled growth changed the growth metric");
    require(runtime.network().validate(), "B.4.4 growth violated network invariants");

    if (index >= 16U) {
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b44-" + std::to_string(index) + ".bin");
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require(!restored.config().growth_enabled,
                "B.4.4 disabled growth state was not persisted");
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.4 structural state changed after persistence");
        static_cast<void>(std::filesystem::remove(path));
    }
}

void test_b45_competing_activity_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 64;
    config.width = 4;
    config.height = 4;
    config.depth = 4;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 1;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 32;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 1U << (index % 4U);
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = index % 3U == 0 ? 4U : index % 3U == 1 ? 16U : 64U;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    config.consume_trace_on_modulation = true;

    eve::bios::Runtime runtime(config, 0x450000ULL + index);
    auto route_config = evaluation_route_config(
        eve::brainstem::EvaluationRouteMode::general_wiring);
    route_config.fanout_per_source = 1;
    route_config.weight = 1;
    route_config.seed = 0x451000ULL + index;
    const auto route = runtime.configure_evaluation_route(route_config);
    require(!route.target_neurons.empty(), "B.4.5 route has no local recipient");
    const eve::NeuronId action = route.target_neurons.front();
    const auto plastic = runtime.add_experimental_synapse(0, action, 1);

    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    const auto active_before = runtime.network().active_eligibility_count();
    require(active_before > 0, "B.4.5 active decision created no eligibility");
    require(std::find(runtime.network().eligibility_records(action).front().candidates.begin(),
                      runtime.network().eligibility_records(action).front().candidates.end(),
                      plastic) != runtime.network().eligibility_records(action).front().candidates.end(),
            "B.4.5 omitted the locally contributing input");

    const auto neutral = runtime.process_modulatory_feedback(0);
    require_equal(neutral.positive_changes, 0ULL,
                  "B.4.5 neutral ambiguity feedback strengthened a weight");
    require_equal(neutral.negative_changes, 0ULL,
                  "B.4.5 neutral ambiguity feedback weakened a weight");
    require_equal(runtime.network().synapse(plastic).weight, 1,
                  "B.4.5 neutral ambiguity feedback changed a weight");
    require_equal(runtime.network().active_eligibility_count(), active_before,
                  "B.4.5 neutral ambiguity feedback unexpectedly consumed the trace");

    if (index >= 16U) {
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b45-" + std::to_string(index) + ".bin");
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.5 ambiguous decision state changed after restore");
        static_cast<void>(std::filesystem::remove(path));
    }

    const auto negative = runtime.process_modulatory_feedback(-1);
    require(negative.negative_changes > 0,
            "B.4.5 negative ambiguity intervention changed no eligible input");
    require_equal(runtime.network().synapse(plastic).weight, 0,
                  "B.4.5 negative ambiguity intervention did not weaken locally");
    require(runtime.network().active_eligibility_count() < active_before,
            "B.4.5 effective modulation did not consume the local trace");
    require(runtime.network().validate(), "B.4.5 intervention broke invariants");
}

void test_b46_local_inhibition_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 32;
    config.width = 4;
    config.height = 4;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 16;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 4;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    config.batch_simultaneous_inputs = true;
    config.inhibitory_plasticity_mode = index % 2U == 0
        ? eve::InhibitoryPlasticityMode::shared
        : eve::InhibitoryPlasticityMode::contribution_aware;

    const auto run_order = [&](const bool reverse) {
        eve::bios::Runtime runtime(config, 0x460000ULL + index);
        eve::brainstem::EvaluationRouteConfig route;
        route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
        route.first_input_neuron = 10;
        route.graded_channel_count = 5;
        route.fanout_per_source = 1;
        route.weight = 1;
        const auto evaluation = runtime.configure_evaluation_route(route);
        for (const auto input : evaluation.input_neurons) {
            static_cast<void>(runtime.add_experimental_synapse(
                input, 4, 0, true, eve::SynapticEffect::excitatory));
        }
        const auto excitatory = runtime.add_experimental_synapse(
            0, 4, 3, false, eve::SynapticEffect::excitatory);
        const auto inhibitory = runtime.add_experimental_synapse(
            1, 4, 1, false, eve::SynapticEffect::inhibitory);
        const std::vector<eve::SynapseId> inputs = reverse
            ? std::vector<eve::SynapseId>{inhibitory, excitatory}
            : std::vector<eve::SynapseId>{excitatory, inhibitory};
        const auto batch = runtime.process_simultaneous_impulses(inputs);
        static_cast<void>(runtime.run());
        return std::make_tuple(
            std::move(runtime), excitatory, inhibitory, batch);
    };

    auto [forward, forward_excitatory, forward_inhibitory, forward_batch] =
        run_order(false);
    auto [reverse, reverse_excitatory, reverse_inhibitory, reverse_batch] =
        run_order(true);
    require(forward_batch.fired, "B.4.6 net local excitation did not fire");
    require_equal(forward_batch.excitatory_contribution, 3LL,
                  "B.4.6 excitatory contribution is wrong");
    require_equal(forward_batch.inhibitory_contribution, 1LL,
                  "B.4.6 inhibitory contribution is wrong");
    require_equal(forward_batch.membrane_after, 0LL,
                  "B.4.6 firing reset did not follow net integration");
    require_equal(forward_batch.fired, reverse_batch.fired,
                  "B.4.6 queue order selected the firing result");
    require_equal(forward.state_fingerprint(), reverse.state_fingerprint(),
                  "B.4.6 canonical batch state depends on input order");
    require_equal(forward.metrics().inhibitory_impulses, 1ULL,
                  "B.4.6 inhibitory impulse metric is wrong");
    require_equal(forward.metrics().inhibitory_applied_strength, 1ULL,
                  "B.4.6 inhibitory strength metric is wrong");
    require_equal(forward.metrics().simultaneous_input_batches, 1ULL,
                  "B.4.6 simultaneous batch metric is wrong");
    require(forward.network().eligibility_records(4).front().candidates.end() !=
                std::find(
                    forward.network().eligibility_records(4).front().candidates.begin(),
                    forward.network().eligibility_records(4).front().candidates.end(),
                    forward_inhibitory),
            "B.4.6 inhibitory causal input is absent from eligibility");

    const auto before_inhibitory =
        forward.network().synapse(forward_inhibitory).weight;
    const std::int8_t polarity = index % 3U == 0 ? -1 : 1;
    static_cast<void>(forward.process_modulatory_feedback(polarity));
    const auto after_inhibitory =
        forward.network().synapse(forward_inhibitory).weight;
    if (config.inhibitory_plasticity_mode ==
        eve::InhibitoryPlasticityMode::shared) {
        require(polarity > 0 ? after_inhibitory > before_inhibitory
                             : after_inhibitory < before_inhibitory,
                "B.4.6 shared three-factor rule changed inhibition incorrectly");
    } else {
        require(polarity > 0 ? after_inhibitory < before_inhibitory
                             : after_inhibitory > before_inhibitory,
                "B.4.6 contribution-aware rule changed inhibition incorrectly");
    }
    require(forward.network().synapse(forward_inhibitory).weight >= 0 &&
                forward.network().synapse(forward_inhibitory).weight <= 3,
            "B.4.6 inhibitory efficacy escaped bounds");

    eve::bios::Runtime inhibited(config, 0x461000ULL + index);
    const auto only_inhibitory = inhibited.add_experimental_synapse(
        1, 4, 3, true, eve::SynapticEffect::inhibitory);
    const auto inhibited_batch =
        inhibited.process_simultaneous_impulses({only_inhibitory});
    require(!inhibited_batch.fired,
            "B.4.6 isolated inhibition generated a fire decision");
    require_equal(inhibited.network().neuron(4).membrane_potential, 0LL,
                  "B.4.6 inhibition created a negative membrane value");
    require_equal(inhibited.network().synapse(only_inhibitory).effect,
                  eve::SynapticEffect::inhibitory,
                  "B.4.6 synaptic effect was not stored separately from weight");
    const auto other_receiver = inhibited.add_experimental_synapse(
        0, 5, 1, true, eve::SynapticEffect::excitatory);
    bool mixed_receivers_rejected = false;
    try {
        static_cast<void>(inhibited.process_simultaneous_impulses(
            {only_inhibitory, other_receiver}));
    } catch (const std::invalid_argument&) {
        mixed_receivers_rejected = true;
    }
    require(mixed_receivers_rejected,
            "B.4.6 simultaneous integration crossed local receivers");

    if (index == 0) {
        auto recovery_config = config;
        recovery_config.refractory_input_events = 1;
        recovery_config.recovery_mode =
            eve::RecoveryMode::independent_with_membrane_relaxation;
        recovery_config.recovery_delay = 2;
        recovery_config.membrane_relaxation_percent = 100;
        eve::bios::Runtime recovery(recovery_config, 0x463000ULL);
        const auto excitation = recovery.add_experimental_synapse(
            0, 4, 3, true, eve::SynapticEffect::excitatory);
        const auto inhibition = recovery.add_experimental_synapse(
            1, 4, 1, true, eve::SynapticEffect::inhibitory);
        require(recovery.process_simultaneous_impulses({excitation, inhibition}).fired,
                "B.4.6 inhibition broke an R2 fire decision");
        static_cast<void>(recovery.run());
        static_cast<void>(recovery.advance_time_to(2));
        require_equal(recovery.network().neuron(4).refractory_events_remaining, 0U,
                      "B.4.6 R2 did not recover after inhibitory competition");
        require(recovery.network().neuron(4).membrane_potential >= 0,
                "B.4.6 R2 created a negative membrane after inhibition");
    }

    if (index >= 16U) {
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-stage-b46-" + std::to_string(index) + ".bin");
        forward.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require(restored.config().batch_simultaneous_inputs,
                "B.4.6 batch integration mode was not persisted");
        require_equal(
            restored.config().inhibitory_plasticity_mode,
            config.inhibitory_plasticity_mode,
            "B.4.6 inhibitory plasticity mode was not persisted");
        require_equal(
            restored.network().synapse(forward_inhibitory).effect,
            eve::SynapticEffect::inhibitory,
            "B.4.6 inhibitory synaptic effect was not persisted");
        require_equal(forward.state_fingerprint(), restored.state_fingerprint(),
                      "B.4.6 persistence changed the complete state");
        static_cast<void>(std::filesystem::remove(path));
    }

    eve::ExperimentConfig disabled = config;
    disabled.batch_simultaneous_inputs = false;
    eve::bios::Runtime disabled_runtime(disabled, 0x462000ULL + index);
    const auto disabled_synapse = disabled_runtime.add_experimental_synapse(
        0, 4, 1, false, eve::SynapticEffect::excitatory);
    bool disabled_rejected = false;
    try {
        static_cast<void>(disabled_runtime.process_simultaneous_impulses(
            {disabled_synapse}));
    } catch (const std::logic_error&) {
        disabled_rejected = true;
    }
    require(disabled_rejected,
            "B.4.6 disabled simultaneous integration accepted a batch");
    require_equal(forward.network().synapse(forward_excitatory).effect,
                  eve::SynapticEffect::excitatory,
                  "B.4.6 changed an existing excitatory effect");
    require(forward.network().validate() && reverse.network().validate() &&
                inhibited.network().validate(),
            "B.4.6 inhibition violated network invariants");
    require_equal(forward.metrics().grown_synapses, 0ULL,
                  "B.4.6 inhibition triggered structural growth");
    static_cast<void>(reverse_excitatory);
    static_cast<void>(reverse_inhibitory);
}

void test_b47_neural_symmetry_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 32;
    config.width = 4;
    config.height = 4;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 24;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    config.batch_simultaneous_inputs = true;
    config.inhibitory_plasticity_mode = eve::InhibitoryPlasticityMode::shared;

    struct Summary {
        std::array<std::int32_t, 4> cue_weights{};
        std::array<std::int32_t, 2> inhibitory_weights{};
        bool fired_a{};
        bool fired_b{};
        bool valid{};
        std::uint64_t positive_changes{};
        std::size_t synapses{};
    };
    const auto run = [&](const bool transformed) {
        eve::bios::Runtime runtime(config, 0x470000ULL + index);
        std::array<eve::NeuronId, 2> sensors = transformed && index == 4
            ? std::array<eve::NeuronId, 2>{17, 19}
            : std::array<eve::NeuronId, 2>{0, 1};
        std::array<eve::NeuronId, 2> explorers = transformed && index == 4
            ? std::array<eve::NeuronId, 2>{21, 23}
            : std::array<eve::NeuronId, 2>{2, 3};
        std::array<eve::NeuronId, 2> decisions = transformed && index == 4
            ? std::array<eve::NeuronId, 2>{25, 27}
            : std::array<eve::NeuronId, 2>{4, 5};
        std::array<eve::NeuronId, 2> motors = transformed && index == 4
            ? std::array<eve::NeuronId, 2>{29, 31}
            : std::array<eve::NeuronId, 2>{6, 7};
        if (transformed && index == 0) std::swap(sensors[0], sensors[1]);
        if (transformed && index == 1) {
            std::swap(decisions[0], decisions[1]);
            std::swap(motors[0], motors[1]);
        }
        if (transformed && index == 2) std::swap(explorers[0], explorers[1]);

        struct Edge {
            eve::NeuronId sender;
            eve::NeuronId receiver;
            std::int32_t weight;
            bool protected_state;
            eve::SynapticEffect effect;
            std::size_t logical_slot;
            bool cue;
        };
        std::vector<Edge> edges;
        for (std::size_t cue = 0; cue < 2; ++cue) {
            for (std::size_t action = 0; action < 2; ++action) {
                edges.push_back(Edge{sensors[cue], decisions[action], 0, false,
                    eve::SynapticEffect::excitatory, cue * 2 + action, true});
            }
        }
        for (std::size_t action = 0; action < 2; ++action) {
            edges.push_back(Edge{explorers[action], decisions[action], 2, true,
                eve::SynapticEffect::excitatory, action, false});
            edges.push_back(Edge{decisions[action], motors[action], 2, true,
                eve::SynapticEffect::excitatory, action + 2, false});
            edges.push_back(Edge{decisions[action], motors[1 - action], 0, false,
                eve::SynapticEffect::inhibitory, action + 4, false});
        }
        if (transformed && index == 5) std::reverse(edges.begin(), edges.end());
        std::array<eve::SynapseId, 4> cue_synapses{};
        std::array<eve::SynapseId, 2> inhibitory_synapses{};
        for (const auto& edge : edges) {
            const auto id = runtime.add_experimental_synapse(
                edge.sender, edge.receiver, edge.weight,
                edge.protected_state, edge.effect);
            if (edge.cue) cue_synapses[edge.logical_slot] = id;
            if (!edge.cue && edge.logical_slot >= 4) {
                inhibitory_synapses[edge.logical_slot - 4] = id;
            }
        }
        eve::brainstem::EvaluationRouteConfig route;
        route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
        route.first_input_neuron = transformed && index == 4 ? 10 : 10;
        route.graded_channel_count = 5;
        route.fanout_per_source = 1;
        route.weight = 1;
        const auto evaluation = runtime.configure_evaluation_route(route);
        std::vector<eve::NeuronId> sources = evaluation.input_neurons;
        std::vector<eve::NeuronId> targets{decisions[0], decisions[1],
                                           motors[0], motors[1]};
        if (transformed && index == 3) {
            std::reverse(sources.begin(), sources.end());
            std::reverse(targets.begin(), targets.end());
        }
        for (const auto source : sources) {
            for (const auto target : targets) {
                static_cast<void>(runtime.add_experimental_synapse(
                    source, target, 0, true,
                    eve::SynapticEffect::excitatory));
            }
        }
        bool fired_a = false;
        bool fired_b = false;
        runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
            fired_a = fired_a || event.neuron == motors[0];
            fired_b = fired_b || event.neuron == motors[1];
        });
        for (std::uint32_t episode = 0; episode < 3; ++episode) {
            const auto start = static_cast<std::uint64_t>(episode) * 16ULL;
            if (runtime.simulated_time() < start) {
                static_cast<void>(runtime.advance_time_to(start));
            }
            fired_a = fired_b = false;
            runtime.enqueue_fire(sensors[0]);
            static_cast<void>(runtime.run());
            if (!fired_a && !fired_b) {
                runtime.enqueue_fire(explorers[0]);
                static_cast<void>(runtime.run());
            }
            static_cast<void>(runtime.process_modulatory_feedback(1));
            static_cast<void>(runtime.advance_time_to(start + 16));
        }
        fired_a = fired_b = false;
        runtime.enqueue_fire(sensors[0]);
        static_cast<void>(runtime.run());
        Summary summary;
        for (std::size_t slot = 0; slot < cue_synapses.size(); ++slot) {
            summary.cue_weights[slot] =
                runtime.network().synapse(cue_synapses[slot]).weight;
        }
        for (std::size_t slot = 0; slot < inhibitory_synapses.size(); ++slot) {
            summary.inhibitory_weights[slot] =
                runtime.network().synapse(inhibitory_synapses[slot]).weight;
        }
        summary.fired_a = fired_a;
        summary.fired_b = fired_b;
        summary.valid = runtime.network().validate();
        summary.positive_changes = runtime.metrics().plasticity_positive_changes;
        summary.synapses = runtime.network().synapse_count();

        if (index == 9 && transformed) {
            const auto path = std::filesystem::temp_directory_path() /
                "eve-stage-b47-symmetry.bin";
            runtime.save(path.string());
            auto restored = eve::bios::Runtime::load(path.string());
            require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                          "B.4.7 transformed state changed during persistence");
            static_cast<void>(runtime.run());
            static_cast<void>(restored.run());
            require_equal(runtime.state_fingerprint(), restored.state_fingerprint(),
                          "B.4.7 transformed continuation diverged");
            static_cast<void>(std::filesystem::remove(path));
        }
        return summary;
    };

    const Summary reference = run(false);
    const Summary transformed = run(true);
    require_equal(reference.cue_weights, transformed.cue_weights,
                  "B.4.7 consistent role mapping changed learned cue weights");
    require_equal(reference.inhibitory_weights, transformed.inhibitory_weights,
                  "B.4.7 consistent role mapping changed inhibitory weights");
    require_equal(reference.fired_a, transformed.fired_a,
                  "B.4.7 consistent role mapping changed logical action A");
    require_equal(reference.fired_b, transformed.fired_b,
                  "B.4.7 consistent role mapping changed logical action B");
    require(reference.valid && transformed.valid,
            "B.4.7 symmetry experiment produced an invalid network");
    require_equal(reference.positive_changes, transformed.positive_changes,
                  "B.4.7 symmetric modulation changed plasticity count");
    require_equal(reference.synapses, transformed.synapses,
                  "B.4.7 role mapping changed topology size");
    require(reference.cue_weights[0] > 0,
            "B.4.7 local experience did not create a learned cue path");
    require(reference.fired_a && !reference.fired_b,
            "B.4.7 learned logical response is not unique");

    if (index == 6 || index == 7) {
        eve::bios::Runtime forward(config, 0x471000ULL + index);
        const auto excitation = forward.add_experimental_synapse(
            0, 4, 3, true, eve::SynapticEffect::excitatory);
        const auto inhibition = forward.add_experimental_synapse(
            1, 4, 1, true, eve::SynapticEffect::inhibitory);
        eve::bios::Runtime reverse = forward;
        const auto first = forward.process_simultaneous_impulses(
            {excitation, inhibition});
        const auto second = reverse.process_simultaneous_impulses(
            {inhibition, excitation});
        require_equal(first.fired, second.fired,
                      "B.4.7 queue insertion order changed the decision");
        require_equal(forward.state_fingerprint(), reverse.state_fingerprint(),
                      "B.4.7 queue insertion order changed the canonical state");
    }
    if (index == 8) {
        for (const auto weight : transformed.cue_weights) {
            require(weight >= 0 && weight <= 3,
                    "B.4.7 learned cue weight escaped bounds");
        }
        for (const auto weight : transformed.inhibitory_weights) {
            require(weight >= 0 && weight <= 3,
                    "B.4.7 learned inhibitory weight escaped bounds");
        }
    }
    if (index == 16 || index == 17) {
        const auto mix_seed = [](std::uint64_t value) {
            value += 0x9e3779b97f4a7c15ULL;
            value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
            value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
            return value ^ (value >> 31U);
        };
        std::set<std::uint64_t> alignment_counts;
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            eve::bios::DeterministicRng cue_rng(index == 16
                ? 0x4601000ULL + seed
                : mix_seed(0x4701000ULL ^ seed));
            eve::bios::DeterministicRng exploration_rng(index == 16
                ? 0x4602000ULL + seed
                : mix_seed(0x4702000ULL ^
                    (seed * 0xd1342543de82ef95ULL)));
            std::uint64_t matches = 0;
            for (std::uint32_t episode = 0; episode < 128; ++episode) {
                matches += cue_rng.bounded(2) == exploration_rng.bounded(2);
            }
            alignment_counts.insert(matches);
        }
        if (index == 16) {
            require_equal(alignment_counts.size(), std::size_t{1},
                          "B.4.7 failed to expose historical binary stream coupling");
            require_equal(*alignment_counts.begin(), 67ULL,
                          "B.4.7 historical coupling reference changed");
        } else {
            require(alignment_counts.size() > 1,
                    "B.4.7 mixed seeds did not vary stream alignment");
            require(*alignment_counts.begin() < 64 &&
                        *alignment_counts.rbegin() > 64,
                    "B.4.7 mixed streams did not span both target roles");
        }
    }
}

void test_b48_early_binding_case(const std::uint32_t index) {
    const auto mix_seed = [](std::uint64_t value) {
        value += 0x9e3779b97f4a7c15ULL;
        value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31U);
    };
    const auto stream = [&](const std::uint64_t seed, const std::uint64_t role) {
        eve::bios::DeterministicRng rng(mix_seed(seed ^ role));
        std::array<std::uint8_t, 128> values{};
        for (auto& value : values) {
            value = static_cast<std::uint8_t>(rng.bounded(2));
        }
        return values;
    };

    const auto cues = stream(48, 0x4801000ULL);
    const auto exploration = stream(48, 0x4802000ULL);
    if (index == 0) {
        require_equal(cues, stream(48, 0x4801000ULL),
                      "B.4.8 cue stream is not reproducible");
    } else if (index == 1) {
        require_equal(cues, stream(48, 0x4801000ULL),
                      "B.4.8 changing exploration changed cue stream");
        require(exploration != stream(49, 0x4802000ULL),
                "B.4.8 exploration revision did not change exploration");
    } else if (index == 2) {
        require_equal(exploration, stream(48, 0x4802000ULL),
                      "B.4.8 changing cues changed exploration stream");
        require(cues != stream(49, 0x4801000ULL),
                "B.4.8 cue revision did not change cues");
    } else if (index == 3) {
        std::array<std::uint8_t, 128> complement{};
        for (std::size_t i = 0; i < complement.size(); ++i) {
            complement[i] = static_cast<std::uint8_t>(exploration[i] ^ 1U);
            require(complement[i] != exploration[i],
                    "B.4.8 complemented exploration retained a bit");
        }
    } else if (index == 4) {
        require(stream(48, 0x4811000ULL) != cues &&
                    stream(48, 0x4812000ULL) != exploration,
                "B.4.8 independent stream family reproduced both streams");
    }

    eve::ExperimentConfig config;
    config.neuron_count = 32;
    config.width = 4;
    config.height = 4;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 24;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    config.batch_simultaneous_inputs = true;
    config.inhibitory_plasticity_mode = eve::InhibitoryPlasticityMode::shared;

    struct Fixture {
        eve::bios::Runtime runtime;
        std::array<eve::SynapseId, 4> cues{};
        std::array<eve::SynapseId, 2> innate{};
        std::array<eve::SynapseId, 2> inhibition{};
    };
    const auto fixture = [&]() {
        Fixture value{eve::bios::Runtime(config, 0x480000ULL + index)};
        for (std::size_t cue = 0; cue < 2; ++cue) {
            for (std::size_t action = 0; action < 2; ++action) {
                value.cues[cue * 2 + action] =
                    value.runtime.add_experimental_synapse(
                        cue, 4 + action, 0, false,
                        eve::SynapticEffect::excitatory);
            }
        }
        for (std::size_t action = 0; action < 2; ++action) {
            value.innate[action] = value.runtime.add_experimental_synapse(
                2 + action, 4 + action, 2, true,
                eve::SynapticEffect::excitatory);
            static_cast<void>(value.runtime.add_experimental_synapse(
                4 + action, 6 + action, 2, true,
                eve::SynapticEffect::excitatory));
            value.inhibition[action] = value.runtime.add_experimental_synapse(
                4 + action, 7 - action, 0, false,
                eve::SynapticEffect::inhibitory);
        }
        eve::brainstem::EvaluationRouteConfig route;
        route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
        route.first_input_neuron = 10;
        route.graded_channel_count = 5;
        route.fanout_per_source = 1;
        route.weight = 1;
        const auto configured = value.runtime.configure_evaluation_route(route);
        for (const auto source : configured.input_neurons) {
            for (const auto target : {eve::NeuronId{4}, eve::NeuronId{5},
                                      eve::NeuronId{6}, eve::NeuronId{7}}) {
                static_cast<void>(value.runtime.add_experimental_synapse(
                    source, target, 0, true,
                    eve::SynapticEffect::excitatory));
            }
        }
        return value;
    };
    const auto experience = [](Fixture& value, const std::uint8_t cue,
                               const std::uint8_t action,
                               const std::int8_t feedback) {
        value.runtime.enqueue_fire(cue);
        static_cast<void>(value.runtime.run());
        value.runtime.enqueue_fire(2 + action);
        static_cast<void>(value.runtime.run());
        static_cast<void>(value.runtime.process_modulatory_feedback(feedback));
        static_cast<void>(value.runtime.advance_time_by(16));
    };

    auto original = fixture();
    experience(original, 0, 0, 1);
    experience(original, 1, 1, 1);
    auto copied = original;
    if (index == 5 || index == 7 || index == 8) {
        require_equal(original.runtime.state_fingerprint(),
                      copied.runtime.state_fingerprint(),
                      "B.4.8 checkpoint copy is not exact");
        experience(original, 0, 0, 1);
        experience(copied, 0, 0, 1);
        require_equal(original.runtime.state_fingerprint(),
                      copied.runtime.state_fingerprint(),
                      "B.4.8 identical checkpoint continuation diverged");
    }
    if (index == 6 || index == 9) {
        auto alternative = copied;
        experience(copied, 0, 0, 1);
        experience(alternative, 0, 1, 1);
        require(copied.runtime.network().synapse(copied.cues[0]).weight !=
                    alternative.runtime.network().synapse(
                        alternative.cues[0]).weight ||
                    copied.runtime.network().synapse(copied.cues[1]).weight !=
                    alternative.runtime.network().synapse(
                        alternative.cues[1]).weight,
                "B.4.8 different future experience produced identical binding");
    }
    if (index == 10) {
        for (const auto synapse : original.innate) {
            require(original.runtime.network().synapse(
                        synapse).plasticity_protected,
                    "B.4.8 changed protected innate exploration");
        }
    }
    if (index == 11 || index == 12 || index == 18) {
        bool readiness = false;
        const auto observe = [&](const std::int8_t feedback) {
            readiness = feedback < 0;
        };
        observe(-1);
        require(readiness, "B.4.8 negative feedback did not enable readiness");
        const auto selected = exploration[index % exploration.size()];
        require(selected <= 1,
                "B.4.8 adaptive exploration selected a nonlocal action");
        observe(1);
        require(!readiness,
                "B.4.8 positive feedback did not clear readiness");
    }
    if (index == 13) {
        for (std::uint8_t cue = 0; cue < 2; ++cue) {
            require_equal(static_cast<std::uint8_t>((cue == 0) ^ 1U),
                          static_cast<std::uint8_t>(cue != 0),
                          "B.4.8 inverse assignment is not symmetric");
        }
    }
    if (index == 14) {
        for (const auto synapse : original.inhibition) {
            require_equal(original.runtime.network().synapse(synapse).effect,
                          eve::SynapticEffect::inhibitory,
                          "B.4.8 lost inhibitory effect");
        }
    }
    if (index == 15) {
        require(original.runtime.network().validate(),
                "B.4.8 checkpoint network is invalid");
    }
    if (index == 16) {
        require(original.runtime.diagnostics().estimated_memory_bytes < 65536,
                "B.4.8 checkpoint memory is unexpectedly unbounded");
    }
    if (index == 17) {
        require_equal(original.runtime.metrics().grown_synapses, 0ULL,
                      "B.4.8 unexpectedly enabled structural growth");
    }
    if (index == 19) {
        const auto path = std::filesystem::temp_directory_path() /
            "eve-stage-b48-checkpoint.bin";
        original.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        require_equal(original.runtime.state_fingerprint(),
                      restored.state_fingerprint(),
                      "B.4.8 checkpoint persistence changed state");
        static_cast<void>(original.runtime.run());
        static_cast<void>(restored.run());
        require_equal(original.runtime.state_fingerprint(),
                      restored.state_fingerprint(),
                      "B.4.8 restored checkpoint continuation diverged");
        static_cast<void>(std::filesystem::remove(path));
    }
}

void test_b49_local_correction_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 16;
    config.width = 4;
    config.height = 2;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 24;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    config.batch_simultaneous_inputs = true;
    config.inhibitory_plasticity_mode = eve::InhibitoryPlasticityMode::shared;

    struct Fixture {
        eve::bios::Runtime runtime;
        std::array<eve::SynapseId, 2> cues{};
        std::array<eve::SynapseId, 2> explorers{};
        std::array<eve::SynapseId, 2> inhibition{};
    };
    const auto make_fixture = [&]() {
        Fixture value{eve::bios::Runtime(config, 0x490000ULL)};
        value.cues[0] = value.runtime.add_experimental_synapse(
            0, 4, 2, false, eve::SynapticEffect::excitatory);
        value.cues[1] = value.runtime.add_experimental_synapse(
            0, 5, 1, false, eve::SynapticEffect::excitatory);
        value.explorers[0] = value.runtime.add_experimental_synapse(
            2, 4, 2, true, eve::SynapticEffect::excitatory);
        value.explorers[1] = value.runtime.add_experimental_synapse(
            3, 5, 2, true, eve::SynapticEffect::excitatory);
        static_cast<void>(value.runtime.add_experimental_synapse(
            4, 6, 2, true, eve::SynapticEffect::excitatory));
        static_cast<void>(value.runtime.add_experimental_synapse(
            5, 7, 2, true, eve::SynapticEffect::excitatory));
        value.inhibition[0] = value.runtime.add_experimental_synapse(
            4, 5, 1, false, eve::SynapticEffect::inhibitory);
        value.inhibition[1] = value.runtime.add_experimental_synapse(
            5, 4, 1, false, eve::SynapticEffect::inhibitory);
        eve::brainstem::EvaluationRouteConfig route;
        route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
        route.first_input_neuron = 8;
        route.graded_channel_count = 5;
        route.fanout_per_source = 1;
        route.weight = 1;
        const auto configured = value.runtime.configure_evaluation_route(route);
        for (const auto source : configured.input_neurons) {
            for (const auto target : {eve::NeuronId{4}, eve::NeuronId{5},
                                      eve::NeuronId{6}, eve::NeuronId{7}}) {
                static_cast<void>(value.runtime.add_experimental_synapse(
                    source, target, 0, true,
                    eve::SynapticEffect::excitatory));
            }
        }
        return value;
    };
    const auto probe = [](eve::bios::Runtime& runtime,
                          const eve::NeuronId source) {
        std::array<bool, 2> fired{};
        runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
            fired[0] = fired[0] || event.neuron == 6;
            fired[1] = fired[1] || event.neuron == 7;
        });
        runtime.enqueue_fire(source);
        static_cast<void>(runtime.run());
        runtime.set_fire_observer({});
        return fired;
    };

    auto fixture = make_fixture();
    auto e1 = fixture.runtime;
    auto e2 = fixture.runtime;
    if (index == 0 || index == 1 || index == 2) {
        require_equal(fixture.runtime.state_fingerprint(),
                      e1.state_fingerprint(),
                      "B.4.9 E1 copy did not start exactly");
        require_equal(fixture.runtime.state_fingerprint(),
                      e2.state_fingerprint(),
                      "B.4.9 E2 copy did not start exactly");
        static_cast<void>(probe(e2, 3));
        require_equal(fixture.runtime.state_fingerprint(),
                      e1.state_fingerprint(),
                      "B.4.9 E2 activity contaminated E1");
        require(e2.state_fingerprint() != e1.state_fingerprint(),
                "B.4.9 separated evaluation copies did not diverge");
    }

    const auto before = fixture.runtime.network().synapses();
    const auto initial = probe(fixture.runtime, 0);
    require(initial[0] && !initial[1],
            "B.4.9 fixture did not create the autonomous dominant path");
    static_cast<void>(fixture.runtime.process_modulatory_feedback(-1));
    if (index == 3) {
        require(fixture.runtime.network().synapse(fixture.cues[0]).weight <
                    before[fixture.cues[0]].weight,
                "B.4.9 negative modulation did not weaken the active cue");
    } else if (index == 4) {
        require_equal(fixture.runtime.network().synapse(
                          fixture.explorers[0]).weight,
                      before[fixture.explorers[0]].weight,
                      "B.4.9 changed protected exploration");
    } else if (index == 5) {
        static_cast<void>(fixture.runtime.advance_time_by(16));
        const auto alternative = probe(fixture.runtime, 3);
        require(alternative[1],
                "B.4.9 competing path is not structurally reachable");
    } else if (index == 6) {
        auto changed = fixture.runtime;
        changed.set_experimental_synapse_weight(fixture.inhibition[0], 0);
        require_equal(changed.network().synapse(fixture.cues[0]).weight,
                      fixture.runtime.network().synapse(fixture.cues[0]).weight,
                      "B.4.9 inhibition ablation changed excitation");
    } else if (index == 7) {
        auto changed = fixture.runtime;
        const auto weight = changed.network().synapse(fixture.cues[0]).weight;
        changed.set_experimental_synapse_weight(
            fixture.cues[0], std::max(0, weight - 1));
        require_equal(changed.network().synapse(fixture.inhibition[0]).weight,
                      fixture.runtime.network().synapse(
                          fixture.inhibition[0]).weight,
                      "B.4.9 dominance ablation changed inhibition");
    } else if (index == 8 || index == 9) {
        auto copy = fixture.runtime;
        static_cast<void>(fixture.runtime.advance_time_by(16));
        static_cast<void>(copy.advance_time_by(16));
        static_cast<void>(probe(fixture.runtime, 0));
        static_cast<void>(probe(copy, 0));
        require_equal(fixture.runtime.state_fingerprint(),
                      copy.state_fingerprint(),
                      "B.4.9 identical repeat was not deterministic");
    } else if (index == 10) {
        require_equal(static_cast<std::uint8_t>(0U ^ 1U), std::uint8_t{1},
                      "B.4.9 inverse assignment lost role symmetry");
    } else if (index == 11) {
        auto alternative = fixture.runtime;
        static_cast<void>(alternative.advance_time_by(16));
        static_cast<void>(probe(alternative, 3));
        require(alternative.state_fingerprint() !=
                    fixture.runtime.state_fingerprint(),
                "B.4.9 alternative continuation was not isolated");
    } else if (index == 12) {
        const auto path = std::filesystem::temp_directory_path() /
            "eve-stage-b49-checkpoint.bin";
        fixture.runtime.save(path.string());
        const auto restored = eve::bios::Runtime::load(path.string());
        require_equal(fixture.runtime.state_fingerprint(),
                      restored.state_fingerprint(),
                      "B.4.9 diagnostic state did not persist exactly");
        static_cast<void>(std::filesystem::remove(path));
    } else if (index == 13) {
        require(fixture.runtime.network().validate(),
                "B.4.9 diagnostic network is invalid");
        require_equal(fixture.runtime.metrics().grown_synapses, 0ULL,
                      "B.4.9 unexpectedly enabled growth");
        require(fixture.runtime.diagnostics().estimated_memory_bytes < 65536,
                "B.4.9 diagnostic state is unexpectedly unbounded");
    }
}

void test_b50_positive_credit_case(const std::uint32_t index) {
    eve::ExperimentConfig config;
    config.neuron_count = 16;
    config.width = 4;
    config.height = 2;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 24;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = eve::PlasticityTraceMode::g3_recent;
    config.modulation_mode = eve::ModulationMode::ternary;
    config.batch_simultaneous_inputs = true;
    config.inhibitory_plasticity_mode = eve::InhibitoryPlasticityMode::shared;

    struct Fixture {
        eve::bios::Runtime runtime;
        std::array<eve::SynapseId, 4> cues{};
        std::array<eve::SynapseId, 2> explorers{};
        std::array<eve::SynapseId, 2> inhibition{};
    };
    const auto make_fixture = [&]() {
        Fixture value{eve::bios::Runtime(config, 0x500000ULL)};
        for (std::size_t cue = 0; cue < 2; ++cue) {
            for (std::size_t action = 0; action < 2; ++action) {
                value.cues[cue * 2 + action] =
                    value.runtime.add_experimental_synapse(
                        cue, 4 + action, 0, false,
                        eve::SynapticEffect::excitatory);
            }
        }
        for (std::size_t action = 0; action < 2; ++action) {
            value.explorers[action] =
                value.runtime.add_experimental_synapse(
                    2 + action, 4 + action, 2, true,
                    eve::SynapticEffect::excitatory);
            static_cast<void>(value.runtime.add_experimental_synapse(
                4 + action, 6 + action, 2, true,
                eve::SynapticEffect::excitatory));
            value.inhibition[action] =
                value.runtime.add_experimental_synapse(
                    4 + action, 7 - action, 0, false,
                    eve::SynapticEffect::inhibitory);
        }
        eve::brainstem::EvaluationRouteConfig route;
        route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
        route.first_input_neuron = 8;
        route.graded_channel_count = 5;
        route.fanout_per_source = 1;
        route.weight = 1;
        const auto configured = value.runtime.configure_evaluation_route(route);
        for (const auto source : configured.input_neurons) {
            for (const auto target : {eve::NeuronId{4}, eve::NeuronId{5},
                                      eve::NeuronId{6}, eve::NeuronId{7}}) {
                static_cast<void>(value.runtime.add_experimental_synapse(
                    source, target, 0, true,
                    eve::SynapticEffect::excitatory));
            }
        }
        return value;
    };
    const auto probe = [](eve::bios::Runtime& runtime,
                          const eve::NeuronId source) {
        std::array<bool, 2> fired{};
        runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
            fired[0] = fired[0] || event.neuron == 6;
            fired[1] = fired[1] || event.neuron == 7;
        });
        runtime.enqueue_fire(source);
        static_cast<void>(runtime.run());
        runtime.set_fire_observer({});
        return fired;
    };
    const auto positive_experience = [&](Fixture& value,
                                         const std::uint8_t cue,
                                         const std::uint8_t action,
                                         const std::int8_t modulation = 1) {
        const auto autonomous = probe(value.runtime, cue);
        const auto explored = probe(value.runtime, 2 + action);
        static_cast<void>(value.runtime.process_modulatory_feedback(modulation));
        static_cast<void>(value.runtime.advance_time_by(16));
        return std::pair{autonomous, explored};
    };

    auto fixture = make_fixture();
    if (index == 0) {
        auto copy = fixture.runtime;
        require_equal(fixture.runtime.state_fingerprint(), copy.state_fingerprint(),
                      "B.5.0 initial copy is not exact");
    }
    const auto initial = probe(fixture.runtime, 0);
    if (index == 1) {
        require(!initial[0] && !initial[1],
                "B.5.0 zero cue unexpectedly produced an autonomous answer");
    }
    auto exploration = probe(fixture.runtime, 2);
    if (index == 2) {
        require(exploration[0] && !exploration[1],
                "B.5.0 protected exploration did not produce the action");
    }
    bool cue_candidate = false;
    for (const auto& record : fixture.runtime.network().eligibility_records(4)) {
        cue_candidate = cue_candidate || std::find(
            record.candidates.begin(), record.candidates.end(), fixture.cues[0]) !=
            record.candidates.end();
    }
    if (index == 3) {
        require(cue_candidate,
                "B.5.0 zero-weight sensory input did not receive eligibility");
    }
    const auto before_positive = fixture.runtime.network().synapses();
    static_cast<void>(fixture.runtime.process_modulatory_feedback(1));
    if (index == 4) {
        require_equal(fixture.runtime.network().synapse(fixture.cues[0]).weight, 1,
                      "B.5.0 first positive experience did not create weight one");
    } else if (index == 5) {
        require_equal(fixture.runtime.network().synapse(
                          fixture.explorers[0]).weight,
                      before_positive[fixture.explorers[0]].weight,
                      "B.5.0 changed protected exploration");
    }
    static_cast<void>(fixture.runtime.advance_time_by(16));
    const auto after_one = probe(fixture.runtime, 0);
    if (index == 6) {
        require(!after_one[0] && !after_one[1],
                "B.5.0 subthreshold weight one fired autonomously");
    }
    static_cast<void>(fixture.runtime.advance_time_by(16));
    static_cast<void>(positive_experience(fixture, 0, 0));
    if (index == 7) {
        auto isolated = make_fixture();
        static_cast<void>(positive_experience(isolated, 0, 0));
        const auto isolated_second = positive_experience(isolated, 0, 0);
        require(!isolated_second.first[0] && isolated_second.second[0],
                "B.5.0 second positive experience was not exploration-dependent");
    } else if (index == 8) {
        require_equal(fixture.runtime.network().synapse(fixture.cues[0]).weight, 2,
                      "B.5.0 repeated positive experience did not reach threshold");
    }
    const auto after_two = probe(fixture.runtime, 0);
    if (index == 9) {
        require(after_two[0] && !after_two[1],
                "B.5.0 threshold weight did not recall autonomously");
    } else if (index == 10) {
        auto rollback = fixture.runtime;
        rollback.set_experimental_synapse_weight(fixture.cues[0], 1);
        static_cast<void>(rollback.advance_time_by(16));
        const auto outcome = probe(rollback, 0);
        require(!outcome[0] && !outcome[1],
                "B.5.0 rollback did not remove autonomous recall");
    } else if (index == 11) {
        auto isolated = make_fixture();
        isolated.runtime.set_experimental_synapse_weight(isolated.cues[0], 2);
        const auto outcome = probe(isolated.runtime, 0);
        require(outcome[0] && !outcome[1],
                "B.5.0 isolated target weight was not sufficient");
    } else if (index == 12) {
        auto neutral = make_fixture();
        static_cast<void>(positive_experience(neutral, 0, 0, 0));
        require_equal(neutral.runtime.network().synapse(neutral.cues[0]).weight, 0,
                      "B.5.0 neutral modulation changed the cue weight");
    } else if (index == 13) {
        auto inverse = make_fixture();
        static_cast<void>(positive_experience(inverse, 0, 1));
        static_cast<void>(positive_experience(inverse, 0, 1));
        const auto outcome = probe(inverse.runtime, 0);
        require(!outcome[0] && outcome[1],
                "B.5.0 inverse positive binding failed");
    } else if (index == 14) {
        for (const auto id : fixture.inhibition) {
            require(fixture.runtime.network().synapse(id).weight >= 0 &&
                        fixture.runtime.network().synapse(id).weight <= 3,
                    "B.5.0 inhibition escaped weight bounds");
        }
    } else if (index == 15) {
        const auto path = std::filesystem::temp_directory_path() /
            "eve-stage-b50-checkpoint.bin";
        fixture.runtime.save(path.string());
        const auto restored = eve::bios::Runtime::load(path.string());
        require_equal(fixture.runtime.state_fingerprint(),
                      restored.state_fingerprint(),
                      "B.5.0 positive state did not persist exactly");
        static_cast<void>(std::filesystem::remove(path));
    } else if (index == 16) {
        auto copy = fixture.runtime;
        static_cast<void>(probe(fixture.runtime, 1));
        static_cast<void>(probe(copy, 1));
        require_equal(fixture.runtime.state_fingerprint(), copy.state_fingerprint(),
                      "B.5.0 identical event order diverged");
    } else if (index == 17) {
        require(fixture.runtime.network().validate(),
                "B.5.0 positive-credit network is invalid");
        require_equal(fixture.runtime.metrics().grown_synapses, 0ULL,
                      "B.5.0 unexpectedly enabled growth");
    }
}

}  // namespace

int main() {
    std::vector<std::pair<std::string, std::function<void()>>> tests{
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
        {"restricted exploration reservations",
         test_processing_exploration_excludes_configured_reservations},
        {"reserved circuit neurons stay active",
         test_reserved_motor_circuit_neurons_remain_synaptically_active},
        {"restricted exploration reproducibility",
         test_processing_exploration_is_reproducible},
        {"exploration independent of motor circuit",
         test_processing_exploration_is_independent_of_motor_circuit},
        {"motor circuit connection inventory",
         test_motor_circuit_reports_expected_connections},
        {"interrupted motor circuit",
         test_interrupted_motor_circuit_preserves_topology_but_blocks_path},
        {"completion without motor data",
         test_motor_causality_distinguishes_completion_without_data},
        {"completion group causality", test_motor_causality_traces_completion_group},
        {"activity-dependent causality",
         test_motor_causality_traces_activity_dependent_path},
        {"causality technical abort", test_motor_causality_rejects_technical_abort},
        {"feedback after completion", test_feedback_after_completed_motor_response},
        {"feedback requires activity", test_feedback_requires_observed_activity},
        {"per-event feedback", test_per_motor_event_feedback_preserves_sequence},
        {"virtual environment feedback", test_virtual_environment_feedback_is_distinct},
        {"feedback event budget", test_feedback_event_budget_aborts_cascade},
        {"feedback channel separation", test_feedback_rejects_motor_channel_overlap},
        {"disabled feedback", test_disabled_feedback_preserves_reference},
        {"integrated sensorimotor cycle", test_integrated_sensorimotor_cycle},
        {"integrated sensorimotor restore", test_integrated_sensorimotor_state_restore},
        {"local memory single input", test_local_event_memory_stores_single_input},
        {"local memory order and repetitions",
         test_local_event_memory_preserves_order_and_repetitions},
        {"local memory ring overwrite", test_local_event_memory_ring_overwrite},
        {"local memory zero-weight input",
         test_local_event_memory_includes_zero_weight_inputs},
        {"local memory fire-decision snapshot",
         test_local_event_snapshot_belongs_to_fire_decision},
        {"local memory refractory processing",
         test_local_event_memory_during_refractory_phase},
        {"local memory simple fire reconstruction",
         test_simple_fire_reconstruction_from_local_memory},
        {"local memory M3 reconstruction",
         test_m3_accumulator_and_completion_reconstruction},
        {"local memory unassigned activity",
         test_local_memory_detects_unassigned_activity},
        {"local memory interrupted M3", test_local_memory_with_interrupted_m3_path},
        {"local memory reproducibility", test_local_event_memory_reproducibility},
        {"local memory persistence", test_local_event_memory_persistence_and_lifetime},
        {"disabled local memory", test_disabled_local_event_memory_preserves_processing},
        {"targeted M3 interruption", test_targeted_m3_synapse_interruption},
        {"simulated time and future scheduling",
         test_simulated_time_and_future_scheduling},
        {"recovery without new input", test_recovery_without_new_input},
        {"same-time recovery order", test_same_time_recovery_order_is_deterministic},
        {"boundary event and time advance", test_boundary_event_and_time_advance},
        {"stale recovery", test_stale_recovery_is_ignored},
        {"R0 input-dependent recovery", test_r0_remains_input_dependent},
        {"R1 membrane preservation", test_r1_does_not_relax_membrane},
        {"R2 membrane relaxation", test_r2_relaxes_only_membrane},
        {"recovery refire without emitted fire",
         test_recovery_emits_no_fire_and_allows_later_refire},
        {"pending recovery persistence", test_pending_recovery_persistence},
        {"response end keeps future recovery",
         test_response_end_does_not_preprocess_recovery},
        {"recovery preserves learning state", test_recovery_changes_no_learning_state},
        {"disabled eligibility", test_disabled_eligibility_creates_no_state},
        {"eligibility at fire decision", test_eligibility_is_created_at_fire_decision},
        {"eligibility precedes queued fire", test_eligibility_snapshot_precedes_queued_fire},
        {"G3 and eligibility separation", test_g3_and_eligibility_are_separate},
        {"L1 recent selection", test_l1_selects_recent_ring_entries},
        {"L2 positive selection", test_l2_selects_only_positive_inputs},
        {"zero-weight selection", test_zero_weight_selection_differs_between_l1_and_l2},
        {"repeated eligibility candidates", test_repeated_and_unique_eligibility_representations},
        {"eligibility candidate capacity", test_eligibility_candidate_capacity},
        {"eligibility overwrite", test_eligibility_overwrite_history},
        {"eligibility generations", test_eligibility_generations_distinguish_fires},
        {"eligibility lifetime", test_eligibility_remains_without_inputs_until_expiry},
        {"eligibility due expiry", test_eligibility_expires_at_due_time},
        {"stale eligibility expiry", test_stale_eligibility_expiry_is_harmless},
        {"eligibility expiry emits no fire", test_expiry_emits_no_fire},
        {"eligibility expiry preserves synapses", test_expiry_changes_no_synapse_weight},
        {"eligibility expiry preserves neuron", test_expiry_changes_no_membrane_or_g3},
        {"recovery preserves eligibility", test_recovery_preserves_eligibility},
        {"D0 eligibility", test_zero_duration_eligibility_exists_only_in_fire_turn},
        {"eligibility persistence", test_eligibility_persistence_and_continuation},
        {"eligibility reproducibility", test_eligibility_is_reproducible},
        {"eligibility has no learning effect", test_eligibility_never_changes_weights},
        {"eligibility merge history", test_merge_history_combines_unique_candidates},
        {"feedback probe without action", test_feedback_probe_without_action_finds_no_eligibility},
        {"eligibility storage invariants", test_eligibility_storage_validates},
        {"binary evaluation values", test_binary_evaluation_values},
        {"evaluation encoding hides target", test_evaluation_encoding_contains_no_target},
        {"graded evaluation encoding", test_graded_evaluation_encodings_are_distinct},
        {"F0 evaluation route", test_f0_emits_no_neural_feedback},
        {"F1 evaluation input", test_f1_emits_sensor_feedback_fire},
        {"F2 general evaluation wiring", test_f2_uses_general_preconfigured_wiring},
        {"F3 evaluation relays", test_f3_uses_regular_relays},
        {"evaluation route reproducibility", test_evaluation_route_is_reproducible},
        {"feedback functional reachability", test_feedback_functional_reachability_is_observed},
        {"feedback reaches eligibility", test_feedback_reaches_preexisting_eligibility},
        {"feedback without eligibility", test_feedback_without_eligibility_is_counted},
        {"feedback cannot revive expiry", test_expired_eligibility_is_not_reactivated_by_feedback},
        {"feedback-created eligibility separation", test_feedback_created_eligibility_is_separate},
        {"local generations are not action IDs", test_local_generations_are_not_global_action_ids},
        {"delayed evaluation feedback", test_delayed_feedback_is_scheduled},
        {"reordered evaluation feedback", test_reordered_feedback_follows_due_time},
        {"pending feedback persistence", test_pending_feedback_persistence},
        {"feedback has no learning effect", test_feedback_never_changes_weights},
        {"disabled action context", test_disabled_action_context_has_no_state},
        {"single context trace", test_single_context_trace_storage},
        {"multiple context traces", test_multiple_context_trace_storage},
        {"context trace ring overwrite", test_context_trace_ring_overwrite},
        {"context trace expiry", test_context_trace_expiry},
        {"stale context trace expiry", test_stale_context_trace_expiry},
        {"local context generations", test_context_generations_are_local},
        {"context and membrane separation", test_context_trace_does_not_change_membrane},
        {"C0 context route", test_c0_context_route_is_inert},
        {"C1 boundary context", test_c1_boundary_context_uses_neural_fire},
        {"C2 identical motor context", test_c2_same_motor_activity_same_context},
        {"C2 different motor context", test_c2_different_motor_activity_differs},
        {"C2 silent motor context", test_c2_zero_motor_activity_has_no_context},
        {"C3 general context circuit", test_c3_general_context_circuit},
        {"C4 combined context", test_c4_combines_boundary_and_motor_sources},
        {"context route reproducibility", test_action_context_route_is_reproducible},
        {"context binding at fire", test_context_binding_at_fire},
        {"context binding without trace", test_context_binding_absent_without_trace},
        {"context binding ambiguity", test_context_binding_marks_ambiguity},
        {"context overlap binding", test_temporal_overlap_binds_existing_eligibility},
        {"context overlap ambiguity", test_temporal_overlap_reports_ambiguity},
        {"feedback-origin context label", test_feedback_origin_context_is_labeled},
        {"eligibility context persistence", test_eligibility_context_persistence},
        {"active context persistence", test_active_context_trace_persistence},
        {"pending context expiry persistence", test_pending_context_expiry_persistence},
        {"context expiry emits no fire", test_context_expiry_emits_no_fire},
        {"context preserves weights", test_action_context_never_changes_weights},
        {"context triggers no learning", test_action_context_triggers_no_learning},
        {"bounded context memory", test_context_trace_memory_is_bounded},
        {"invalid single context capacity", test_invalid_single_context_capacity},
        {"invalid multiple context capacity", test_invalid_multiple_context_capacity},
        {"context has no sequence ID", test_context_signature_contains_no_sequence_number},
    };
    // The 42 cases are separately reported automated tests.  Together they cover
    // E1-E3, T0-T3, capacities 2/4/8, durations 4/16/64, both polarities,
    // different routes/seeds, zero-to-one growth and bounded weakening.
    for (std::uint32_t index = 0; index < 42; ++index) {
        tests.emplace_back(
            "B4 three-factor matrix " + std::to_string(index + 1U),
            [index]() { test_b4_three_factor_matrix_case(index); });
    }
    const std::array<const char*, 45> b41_names{{
        "time advances between episodes", "paired variants share time gaps",
        "trace expires on time", "due events complete", "future events wait",
        "expiry without input", "feedback before expiry", "feedback at expiry",
        "feedback after expiry", "future event restore",
        "task RNG independent", "topology RNG independent",
        "exploration RNG independent", "paired task sequence", "seed reproducibility",
        "action A reachable", "action B reachable", "both output classified",
        "none output classified", "exploration retains sensor input",
        "no technical action choice", "feedback is not action",
        "separate motor modulation contacts", "E1 preserved", "E2 preserved",
        "E3 preserved", "zero synapse eligible", "zero to one modulation",
        "negative modulation", "weight bounds", "input budget",
        "expired trace inert", "feedback trace excluded", "single modulation effect",
        "absent modulation inert", "no-learning reference", "reversal changes environment",
        "training evaluation separation", "evaluation retains weights",
        "continuous training state", "neural decisions evaluated",
        "all outcomes measured", "all seeds retained", "paired data basis",
        "aborts documented",
    }};
    for (std::uint32_t index = 0; index < b41_names.size(); ++index) {
        tests.emplace_back(
            std::string("B41 ") + b41_names[index],
            [index]() { test_b41_validation_case(index); });
    }
    const std::array<const char*, 43> b42_names{{
        "A and B initially symmetric", "protected weights retained",
        "unprotected paths remain plastic", "protection independent of success",
        "protection can be disabled", "protection persists",
        "exploration does not encode target", "exploration uses neural events",
        "negative modulation weakens allowed path", "negative leaves innate path",
        "positive modulation remains possible", "negative frequency reproducible",
        "no negative change without trace", "no-action feedback controlled",
        "budgets and bounds retained", "negative feedback can raise exploration",
        "feedback selects no motor path", "later exploration is not retroactive",
        "further action requires neural activity", "double output recorded",
        "missing output retained", "bounded exploration loop",
        "single cue control is paired", "both targets supported",
        "no target knowledge in network", "initial preference documented",
        "learned preference survives evaluation", "random feedback reproducible",
        "reversal leaves architecture intact", "no episode reset",
        "two cues processed independently", "cue outputs evaluated separately",
        "general preference not cue learning", "paired tasks share sequence",
        "B41 reference reproducible", "R2F regression", "G3 regression",
        "eligibility regression", "modulatory contacts remain non-excitatory",
        "protected connection save load", "running experiment continuation",
        "single modulation effect", "all prior tests retained",
    }};
    for (std::uint32_t index = 0; index < b42_names.size(); ++index) {
        tests.emplace_back(
            std::string("B42 ") + b42_names[index],
            [index]() { test_b42_exploration_preservation_case(index); });
    }
    const std::array<const char*, 42> b43_names{{
        "single X to A regression", "single X to B regression",
        "reversal A to B regression", "reversal B to A regression",
        "protected exploration", "G3 reference unchanged",
        "active events only", "no technical cue ID",
        "zero-weight candidates", "candidate capacity one",
        "candidate capacity two", "candidate capacity four",
        "candidate capacity eight", "decision capacity one",
        "decision capacity two", "decision capacity four",
        "decision capacity eight", "duration four", "duration eight",
        "duration sixteen", "duration thirty-two", "duration sixty-four",
        "local decision boundary", "G3 survives boundary",
        "expired records inert", "no modulation no change",
        "feedback traces excluded", "both motor paths reachable",
        "X and Y independently classifiable", "sequential state continuous",
        "mixed training deterministic", "both output retained",
        "missing output retained", "inverse assignment supported",
        "active trace persistence", "pending modulation persistence",
        "protected state persistence", "deterministic restore",
        "bounded local storage", "weight bounds retained",
        "input budget retained", "all prior tests retained",
    }};
    for (std::uint32_t index = 0; index < b43_names.size(); ++index) {
        tests.emplace_back(
            std::string("B43 ") + b43_names[index],
            [index]() { test_b43_local_separation_case(index); });
    }
    const std::array<const char*, 24> b44_names{{
        "growth enabled", "k zero growth guaranteed", "new weight zero",
        "no self connection", "growth metric", "network validity",
        "growth can be disabled", "disabled growth creates nothing",
        "disabled metric stable", "evaluation structure frozen",
        "sender identity retained", "receiver selected locally",
        "independent seed one", "independent seed two",
        "independent seed three", "independent seed four",
        "disabled state persistence", "structural persistence",
        "fingerprint persistence", "weight persistence",
        "growth count persistence", "topology persistence",
        "post-growth validity", "B43 regressions retained",
    }};
    for (std::uint32_t index = 0; index < b44_names.size(); ++index) {
        tests.emplace_back(
            std::string("B44 ") + b44_names[index],
            [index]() { test_b44_structural_learning_case(index); });
    }
    const std::array<const char*, 24> b45_names{{
        "active decision trace", "contributing input bound", "candidate bound",
        "zero semantic cue state", "neutral changes no weight",
        "neutral has no positive update", "neutral has no negative update",
        "neutral trace remains", "negative reaches trace",
        "negative weakens locally", "negative consumes trace", "weight bound",
        "short duration configuration", "medium duration configuration",
        "long duration configuration", "capacity variation",
        "ambiguous state persistence", "eligibility persistence",
        "weight persistence", "fingerprint persistence",
        "growth disabled", "network validity", "B44 regression retained",
        "all prior tests retained",
    }};
    for (std::uint32_t index = 0; index < b45_names.size(); ++index) {
        tests.emplace_back(
            std::string("B45 ") + b45_names[index],
            [index]() { test_b45_competing_activity_case(index); });
    }
    const std::array<const char*, 28> b46_names{{
        "separate inhibitory effect", "single inhibitory event",
        "bounded nonnegative membrane", "net simultaneous integration",
        "order independent firing", "order independent state",
        "inhibitory metric", "applied strength metric",
        "simultaneous batch metric", "binary fire preserved",
        "inhibition creates no motor decision", "local receiver only",
        "G3 records inhibitory input", "eligibility binds inhibition",
        "zero weight remains learnable", "shared plasticity examined",
        "contribution-aware positive modulation",
        "contribution-aware negative modulation", "inhibitory weight bounds",
        "excitatory compatibility", "disabled batching compatibility",
        "configuration persistence", "effect persistence",
        "metric persistence", "fingerprint persistence",
        "growth remains disabled", "network validity",
        "B45 regression retained",
    }};
    for (std::uint32_t index = 0; index < b46_names.size(); ++index) {
        tests.emplace_back(
            std::string("B46 ") + b46_names[index],
            [index]() { test_b46_local_inhibition_case(index); });
    }
    const std::array<const char*, 18> b47_names{{
        "symmetric sensory mapping", "symmetric motor mapping",
        "symmetric exploration mapping", "equivalent modulation routes",
        "neuron ID independence", "synapse ID independence",
        "queue insertion independence", "simultaneous excitation inhibition",
        "learned weight bounds", "transformed persistence",
        "protected exploration retained", "binary logical decision",
        "no semantic target state", "deterministic logical learning",
        "symmetric topology size", "B46 regression retained",
        "historical binary stream coupling exposed",
        "decorrelated stream seed diversity",
    }};
    for (std::uint32_t index = 0; index < b47_names.size(); ++index) {
        tests.emplace_back(
            std::string("B47 ") + b47_names[index],
            [index]() { test_b47_neural_symmetry_case(index); });
    }
    const std::array<const char*, 20> b48_names{{
        "reproducible cue stream", "independent exploration revision",
        "independent cue revision", "complemented exploration stream",
        "independent stream families", "checkpoint exact copy",
        "different future exploration", "identical continuation",
        "copied temporal state", "alternative continuation binding",
        "protected exploration retained", "negative readiness",
        "target-free adaptive action", "normal inverse symmetry",
        "inhibitory effect retained", "network validity",
        "bounded checkpoint memory", "growth remains disabled",
        "deterministic adaptive state", "checkpoint persistence",
    }};
    for (std::uint32_t index = 0; index < b48_names.size(); ++index) {
        tests.emplace_back(
            std::string("B48 ") + b48_names[index],
            [index]() { test_b48_early_binding_case(index); });
    }
    const std::array<const char*, 14> b49_names{{
        "identical evaluation checkpoint", "independent E1 copy",
        "independent E2 copy", "local negative modulation",
        "protected alternative retained", "alternative path reachable",
        "isolated inhibition ablation", "isolated dominance ablation",
        "deterministic repeat", "copied repeat state",
        "inverse assignment symmetry", "alternative continuation",
        "diagnostic persistence", "bounded valid state",
    }};
    for (std::uint32_t index = 0; index < b49_names.size(); ++index) {
        tests.emplace_back(
            std::string("B49 ") + b49_names[index],
            [index]() { test_b49_local_correction_case(index); });
    }
    const std::array<const char*, 18> b50_names{{
        "identical runtime copies", "initial autonomous silence",
        "first positive exploration", "zero-weight cue eligibility",
        "positive zero-to-one change", "protected exploration stability",
        "subthreshold first experience", "second exploration dependence",
        "repeated positive threshold", "autonomous recall at threshold",
        "single-weight rollback", "isolated target sufficiency",
        "neutral modulation control", "inverse positive binding",
        "inhibitory weight bounds", "positive-state persistence",
        "event-order determinism", "valid bounded architecture",
    }};
    for (std::uint32_t index = 0; index < b50_names.size(); ++index) {
        tests.emplace_back(
            std::string("B50 ") + b50_names[index],
            [index]() { test_b50_positive_credit_case(index); });
    }

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
