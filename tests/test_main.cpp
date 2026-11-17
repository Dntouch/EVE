#include "eve/bios/runtime.hpp"
#include "eve/brainstem/brainstem.hpp"
#include "eve/neural/network.hpp"
#include "eve/types.hpp"

#include <cstdint>
#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
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
    static_cast<void>(first.process_utf8_text("ABAB"));
    static_cast<void>(second.process_utf8_text("ABAB"));
    static_cast<void>(different.process_utf8_text("ABAB"));
    require_equal(first.state_fingerprint(), second.state_fingerprint(), "same seed diverged");
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
        {"growth probability and partner selection", test_growth_probability_and_partner_selection},
        {"reproducible execution", test_reproducible_execution},
        {"state persistence", test_persistence_round_trip},
        {"raw UTF-8 sensory processing", test_utf8_byte_sensory_processing},
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
