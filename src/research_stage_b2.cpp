#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr std::array<std::uint64_t, 64> kSeeds{{
    20261117, 7, 42, 99, 2026, 314159, 1, 2, 3, 5, 11, 13, 17, 19, 23, 29,
    31, 37, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
    107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
    181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
    263, 269, 271, 277,
}};
constexpr std::array<std::uint64_t, 6> kEstablishedSeeds{{
    20261117, 7, 42, 99, 2026, 314159,
}};
// Deliberately unchanged from B/B.1 so every established success seed remains
// a paired comparison rather than a newly sampled topology.
constexpr std::uint64_t kMotorSeed = 0x41322d4d4f544f52ULL;
constexpr std::uint64_t kTopologySalt = 0x41322d544f504f4cULL;
constexpr std::uint64_t kExplorationSalt = 0x41322d4558504c4fULL;
constexpr std::uint64_t kCircuitSalt = 0x41322d4349524355ULL;

struct Setup {
    eve::brainstem::MotorCircuitResult circuit;
};

struct Capture {
    eve::brainstem::MotorResponse response;
    eve::bios::MotorCausality technical;
    std::set<eve::SynapseId> marked;
    std::set<eve::SynapseId> technical_candidates;
    std::uint64_t decisions{};
    std::uint64_t candidates{};
    std::uint64_t positive{};
    std::uint64_t zero{};
    std::uint64_t overwrites{};
};

const char* selection_name(const eve::EligibilitySelection selection) {
    switch (selection) {
    case eve::EligibilitySelection::none: return "L0";
    case eve::EligibilitySelection::all_recent: return "L1";
    case eve::EligibilitySelection::positive_weight: return "L2";
    }
    return "unknown";
}

const char* representation_name(const eve::EligibilityRepresentation representation) {
    return representation == eve::EligibilityRepresentation::unique
        ? "unique" : "repeated";
}

const char* history_name(const eve::EligibilityHistory history) {
    switch (history) {
    case eve::EligibilityHistory::overwrite: return "overwrite";
    case eve::EligibilityHistory::retain: return "retain";
    case eve::EligibilityHistory::merge_unique: return "merge_unique";
    }
    return "unknown";
}

eve::ExperimentConfig config_for(
    const eve::EligibilitySelection selection,
    const std::uint32_t capacity,
    const std::uint64_t duration,
    const eve::EligibilityRepresentation representation =
        eve::EligibilityRepresentation::unique,
    const eve::EligibilityHistory history = eve::EligibilityHistory::retain,
    const eve::RecoveryMode recovery =
        eve::RecoveryMode::independent_with_membrane_relaxation) {
    eve::ExperimentConfig config;
    config.neuron_count = 256;
    config.width = 8;
    config.height = 8;
    config.depth = 4;
    config.initial_outgoing_synapses = 4;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.firing_threshold = 3;
    config.refractory_input_events = 16;
    config.recovery_mode = recovery;
    config.recovery_delay = 16;
    config.membrane_relaxation_percent =
        recovery == eve::RecoveryMode::independent_with_membrane_relaxation ? 100 : 0;
    config.local_event_memory_capacity = selection == eve::EligibilitySelection::none &&
            capacity == 0 ? 8 : 8;
    config.eligibility_selection = selection;
    config.eligibility_representation = representation;
    config.eligibility_history = history;
    config.eligibility_candidate_capacity = capacity;
    config.eligibility_decision_capacity = selection == eve::EligibilitySelection::none
        ? 0 : 2;
    config.eligibility_duration = selection == eve::EligibilitySelection::none
        ? 0 : duration;
    config.maximum_events_per_run = 100'000;
    config.learning_enabled = false;
    config.growth_enabled = false;
    return config;
}

eve::brainstem::MotorOutputConfig motor_config() {
    eve::brainstem::MotorOutputConfig config;
    config.contacts_per_channel = 1;
    config.assignment_seed = kMotorSeed;
    config.response_stimulus_neuron = 8;
    config.first_motor_neuron = 18;
    return config;
}

Setup configure(eve::bios::Runtime& runtime, const std::uint64_t seed,
                const bool path_enabled = true) {
    runtime.configure_motor_output(motor_config());
    eve::brainstem::InnateWiringConfig wiring;
    wiring.topology = eve::brainstem::InnateTopology::functional_paths;
    wiring.initial_weight = 3;
    wiring.topology_seed = seed ^ kTopologySalt;
    static_cast<void>(runtime.configure_innate_wiring(wiring));
    eve::brainstem::MotorCircuitConfig circuit;
    circuit.mode = eve::brainstem::MotorCircuitMode::activity_dependent;
    circuit.motor_weight = 1;
    circuit.completion_weight = 3;
    circuit.relay_count = 1;
    circuit.seed = seed ^ kCircuitSalt;
    circuit.path_enabled = path_enabled;
    return Setup{runtime.configure_motor_circuit(circuit)};
}

eve::brainstem::ExplorationConfig exploration(
    const std::uint64_t seed, const std::uint32_t phase) {
    eve::brainstem::ExplorationConfig config;
    config.mode = eve::brainstem::ExplorationMode::distributed;
    config.target_policy =
        eve::brainstem::ExplorationTargetPolicy::processing_neurons_only;
    config.activation_count = 16;
    config.activation_budget = 16;
    config.seed = (seed ^ kExplorationSalt) + phase;
    config.center = {4, 4, 2};
    config.radius = 2;
    return config;
}

Capture act(
    eve::bios::Runtime& runtime,
    const Setup& setup,
    const std::uint64_t seed,
    const std::uint32_t phase) {
    Capture capture;
    std::vector<eve::bios::ImpulseObservation> impulses;
    const auto before = runtime.metrics();
    runtime.set_impulse_observer(
        [&capture, &impulses, &setup](const eve::bios::ImpulseObservation& observation) {
            impulses.push_back(observation);
            if (observation.eligibility_generation != 0) {
                capture.marked.insert(
                    observation.eligibility_candidates.begin(),
                    observation.eligibility_candidates.end());
            }
            if (observation.receiver_fired &&
                std::find(setup.circuit.circuit_synapses.begin(),
                          setup.circuit.circuit_synapses.end(), observation.synapse) !=
                    setup.circuit.circuit_synapses.end()) {
                capture.technical_candidates.insert(observation.synapse);
            }
        });
    static_cast<void>(runtime.process_utf8_text("AB"));
    static_cast<void>(runtime.enqueue_exploration(exploration(seed, phase)));
    capture.response = runtime.request_response(true, 100'000);
    capture.technical = eve::bios::Runtime::analyze_motor_causality(
        capture.response, setup.circuit, impulses);
    static_cast<void>(runtime.run());
    runtime.set_impulse_observer({});
    const auto& after = runtime.metrics();
    capture.decisions = after.eligibility_decisions - before.eligibility_decisions;
    capture.candidates = after.eligibility_candidates - before.eligibility_candidates;
    capture.positive = after.eligibility_positive_candidates -
                       before.eligibility_positive_candidates;
    capture.zero = after.eligibility_zero_weight_candidates -
                   before.eligibility_zero_weight_candidates;
    capture.overwrites = after.eligibility_overwritten_decisions -
                         before.eligibility_overwritten_decisions;
    return capture;
}

std::size_t active_candidates(const eve::bios::Runtime& runtime) {
    std::size_t result = 0;
    for (eve::NeuronId neuron = 0; neuron < runtime.network().neuron_count(); ++neuron) {
        for (const auto& record : runtime.network().eligibility_records(neuron)) {
            result += record.candidates.size();
        }
    }
    return result;
}

std::size_t intersection_size(
    const std::set<eve::SynapseId>& left,
    const std::set<eve::SynapseId>& right) {
    std::vector<eve::SynapseId> intersection;
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                          std::back_inserter(intersection));
    return intersection.size();
}

void write_parameters(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "parameter,value\nbase_branch,codex/research-stage-b1-neural-recovery\n"
           "neurons,256\nregular_start_synapses,1024\ninnate,V2:3\n"
           "motor,M3:1:3\nthreshold,3\nlocal_memory,G3:8\n"
           "recovery,R2F:16\nexploration,E4-new:16\nlearning,disabled\n"
           "growth,disabled\nseeds,64\nselection,L0|L1|L2\n"
           "candidate_capacity,0|2|4|8\nduration,0|4|16|64\n"
           "standard_representation,unique\nstandard_history,retain:2\n"
           "same_time_order,expiry-before-technical-probe\n";
}

void run_selection_matrix(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,selection,capacity,duration,representation,history,completed,"
           "proven_m3,decisions,candidate_events,unique_marked,positive,zero,"
           "technical_relevant,relevant_marked,irrelevant_marked,coverage,selectivity,"
           "active_records,active_candidates,overwrites,weight_changes\n";
    for (const std::uint64_t seed : kSeeds) {
        for (const auto selection : {eve::EligibilitySelection::none,
                                     eve::EligibilitySelection::all_recent,
                                     eve::EligibilitySelection::positive_weight}) {
            const std::vector<std::uint32_t> capacities =
                selection == eve::EligibilitySelection::none
                    ? std::vector<std::uint32_t>{0}
                    : std::vector<std::uint32_t>{2, 4, 8};
            const std::vector<std::uint64_t> durations =
                selection == eve::EligibilitySelection::none
                    ? std::vector<std::uint64_t>{0}
                    : std::vector<std::uint64_t>{0, 4, 16, 64};
            for (const auto capacity : capacities) {
                for (const auto duration : durations) {
                    eve::bios::Runtime runtime(
                        config_for(selection, capacity, duration), seed);
                    const Setup setup = configure(runtime, seed);
                    const auto weights = runtime.network().weight_distribution();
                    const Capture capture = act(runtime, setup, seed, 1);
                    const auto relevant = intersection_size(
                        capture.marked, capture.technical_candidates);
                    const auto irrelevant = capture.marked.size() - relevant;
                    const double coverage = capture.technical_candidates.empty() ? 0.0 :
                        static_cast<double>(relevant) /
                        static_cast<double>(capture.technical_candidates.size());
                    const double selectivity = capture.marked.empty() ? 0.0 :
                        static_cast<double>(relevant) /
                        static_cast<double>(capture.marked.size());
                    out << seed << ',' << selection_name(selection) << ',' << capacity << ','
                        << duration << ",unique,retain," << (capture.response.valid() ? 1 : 0)
                        << ',' << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                        << capture.decisions << ',' << capture.candidates << ','
                        << capture.marked.size() << ',' << capture.positive << ','
                        << capture.zero << ',' << capture.technical_candidates.size() << ','
                        << relevant << ',' << irrelevant << ',' << std::setprecision(9)
                        << coverage << ',' << selectivity << ','
                        << runtime.network().active_eligibility_count() << ','
                        << active_candidates(runtime) << ',' << capture.overwrites << ','
                        << (runtime.network().weight_distribution() == weights ? 0 : 1)
                        << '\n';
                }
            }
        }
    }
}

void run_variant_comparison(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,representation,history,phase,decisions,candidates,unique_marked,"
           "active_records,active_candidates,overwrites,proven_m3\n";
    for (const auto seed : kSeeds) {
        for (const auto representation : {eve::EligibilityRepresentation::repeated,
                                          eve::EligibilityRepresentation::unique}) {
            for (const auto history : {eve::EligibilityHistory::overwrite,
                                       eve::EligibilityHistory::retain,
                                       eve::EligibilityHistory::merge_unique}) {
                eve::bios::Runtime runtime(config_for(
                    eve::EligibilitySelection::all_recent, 8, 64,
                    representation, history), seed);
                const Setup setup = configure(runtime, seed);
                for (std::uint32_t phase = 1; phase <= 2; ++phase) {
                    const Capture capture = act(runtime, setup, seed, phase);
                    out << seed << ',' << representation_name(representation) << ','
                        << history_name(history) << ',' << phase << ','
                        << capture.decisions << ',' << capture.candidates << ','
                        << capture.marked.size() << ','
                        << runtime.network().active_eligibility_count() << ','
                        << active_candidates(runtime) << ',' << capture.overwrites << ','
                        << (capture.technical.proven_circuit_path ? 1 : 0) << '\n';
                    static_cast<void>(runtime.advance_time_by(16));
                }
            }
        }
    }
}

void run_feedback_timing(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,duration,probe,time,active_records,active_candidates,expired,stale,"
           "weights_unchanged,action_present\n";
    for (const auto seed : kSeeds) {
        for (const std::uint64_t duration : {0ULL, 4ULL, 16ULL, 64ULL}) {
            eve::bios::Runtime runtime(config_for(
                eve::EligibilitySelection::positive_weight, 8, duration), seed);
            const Setup setup = configure(runtime, seed);
            const auto weights = runtime.network().weight_distribution();
            static_cast<void>(act(runtime, setup, seed, 1));
            const auto row = [&](const char* probe, const bool action) {
                out << seed << ',' << duration << ',' << probe << ','
                    << runtime.simulated_time() << ','
                    << runtime.network().active_eligibility_count() << ','
                    << active_candidates(runtime) << ','
                    << runtime.metrics().processed_eligibility_expiries << ','
                    << runtime.metrics().stale_eligibility_expiries << ','
                    << (runtime.network().weight_distribution() == weights ? 1 : 0) << ','
                    << (action ? 1 : 0) << '\n';
            };
            row("immediate", true);
            if (duration != 0) {
                static_cast<void>(runtime.advance_time_to(duration - 1));
                row("before_expiry", true);
                static_cast<void>(runtime.advance_time_to(duration));
                row("exact_expiry_after_expiry_event", true);
                static_cast<void>(runtime.advance_time_by(1));
                row("after_expiry", true);
            }
            static_cast<void>(act(runtime, setup, seed, 2));
            row("after_next_action", true);
        }
        eve::bios::Runtime no_action(config_for(
            eve::EligibilitySelection::positive_weight, 8, 16), seed);
        static_cast<void>(configure(no_action, seed));
        out << seed << ",16,without_action,0,0,0,0,0,1,0\n";
    }
}

void run_repetitions(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,repetitions,phase,selection,duration,completed,proven_m3,decisions,"
           "active_records,active_candidates,overwrites,expired,pending_events\n";
    for (const auto seed : kEstablishedSeeds) {
        for (const std::uint32_t repetitions : {2U, 8U, 32U, 128U}) {
            for (const auto selection : {eve::EligibilitySelection::all_recent,
                                         eve::EligibilitySelection::positive_weight}) {
                eve::bios::Runtime runtime(config_for(selection, 8, 64), seed);
                const Setup setup = configure(runtime, seed);
                for (std::uint32_t phase = 1; phase <= repetitions; ++phase) {
                    const Capture capture = act(runtime, setup, seed, phase);
                    out << seed << ',' << repetitions << ',' << phase << ','
                        << selection_name(selection) << ",64,"
                        << (capture.response.valid() ? 1 : 0) << ','
                        << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                        << capture.decisions << ','
                        << runtime.network().active_eligibility_count() << ','
                        << active_candidates(runtime) << ',' << capture.overwrites << ','
                        << runtime.metrics().processed_eligibility_expiries << ','
                        << runtime.future_event_count() << '\n';
                    static_cast<void>(runtime.advance_time_by(16));
                }
            }
        }
    }
}

void run_controls(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,control,completed,proven_m3,decisions,candidates,active_records,"
           "weight_changes,notes\n";
    for (const auto seed : kSeeds) {
        for (int control = 0; control <= 7; ++control) {
            auto selection = control <= 1
                ? eve::EligibilitySelection::none
                : (control == 4 ? eve::EligibilitySelection::all_recent
                                : eve::EligibilitySelection::positive_weight);
            auto recovery = control == 6 ? eve::RecoveryMode::independent
                                         : eve::RecoveryMode::independent_with_membrane_relaxation;
            auto config = config_for(selection, selection == eve::EligibilitySelection::none
                                                ? 0U : 8U,
                                     selection == eve::EligibilitySelection::none ? 0 : 16,
                                     eve::EligibilityRepresentation::unique,
                                     eve::EligibilityHistory::retain, recovery);
            if (control == 0) config.local_event_memory_capacity = 0;
            eve::bios::Runtime runtime(config, seed);
            const Setup setup = configure(runtime, seed, control != 3);
            if (control == 5 && !setup.circuit.circuit_synapses.empty()) {
                runtime.set_experimental_synapse_weight(
                    setup.circuit.circuit_synapses.front(), 0);
            }
            const auto weights = runtime.network().weight_distribution();
            Capture capture;
            if (control != 7) capture = act(runtime, setup, seed, 1);
            out << seed << ",K" << control << ','
                << (capture.response.valid() ? 1 : 0) << ','
                << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                << capture.decisions << ',' << capture.candidates << ','
                << runtime.network().active_eligibility_count() << ','
                << (runtime.network().weight_distribution() == weights ? 0 : 1) << ','
                << (control == 4 ? "zero-weight-candidates-visible-in-L1" :
                    control == 7 ? "probe-without-action" : "control") << '\n';
        }
    }
}

void run_persistence(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,pending_equal,continuation_equal,active_before,active_after,"
           "expired_after,stale_after\n";
    for (const auto seed : kSeeds) {
        eve::bios::Runtime runtime(config_for(
            eve::EligibilitySelection::positive_weight, 8, 64), seed);
        const Setup setup = configure(runtime, seed);
        static_cast<void>(act(runtime, setup, seed, 1));
        const auto temp = std::filesystem::temp_directory_path() /
            ("eve-b2-" + std::to_string(seed) + ".bin");
        runtime.save(temp.string());
        auto restored = eve::bios::Runtime::load(temp.string());
        const bool pending_equal = runtime.state_fingerprint() ==
                                   restored.state_fingerprint();
        const auto active_before = runtime.network().active_eligibility_count();
        static_cast<void>(runtime.advance_time_to(64));
        static_cast<void>(restored.advance_time_to(64));
        out << seed << ',' << (pending_equal ? 1 : 0) << ','
            << (runtime.state_fingerprint() == restored.state_fingerprint() ? 1 : 0) << ','
            << active_before << ',' << runtime.network().active_eligibility_count() << ','
            << runtime.metrics().processed_eligibility_expiries << ','
            << runtime.metrics().stale_eligibility_expiries << '\n';
        static_cast<void>(std::filesystem::remove(temp));
    }
}

void run_performance(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "neurons,synapses,enabled,memory_bytes,eligibility_bytes_estimate,"
           "bytes_per_neuron,record_payload_bytes,events,fire_events,impulses,"
           "eligibility_decisions,scheduled_expiries,processed_expiries,maximum_queue,"
           "seconds,events_per_second\n";
    struct Size { std::uint32_t neurons, width, height, depth; };
    const std::array<Size, 2> measured{{{256, 8, 8, 4}, {10000, 25, 20, 20}}};
    for (const auto& size : measured) {
        for (const bool enabled : {false, true}) {
            auto config = config_for(
                enabled ? eve::EligibilitySelection::positive_weight
                        : eve::EligibilitySelection::none,
                enabled ? 8U : 0U, enabled ? 16U : 0U);
            config.neuron_count = size.neurons;
            config.width = size.width;
            config.height = size.height;
            config.depth = size.depth;
            config.initial_synapse_weight = 1;
            eve::bios::Runtime runtime(config, 900 + size.neurons + enabled);
            const auto start = std::chrono::steady_clock::now();
            for (eve::NeuronId source = 0; source < std::min(64U, size.neurons); ++source) {
                runtime.enqueue_fire(source);
            }
            static_cast<void>(runtime.run());
            static_cast<void>(runtime.advance_time_by(16));
            const double seconds = std::chrono::duration<double>(
                std::chrono::steady_clock::now() - start).count();
            const auto diagnostics = runtime.diagnostics();
            const std::uint64_t eligibility_bytes = enabled ?
                static_cast<std::uint64_t>(size.neurons) *
                    (2ULL * 8ULL * sizeof(eve::SynapseId) +
                     2ULL * (sizeof(std::uint32_t) + 2ULL * sizeof(std::uint64_t)) +
                     sizeof(std::uint32_t) + 2ULL * sizeof(std::uint64_t)) : 0;
            out << size.neurons << ',' << runtime.network().synapse_count() << ','
                << (enabled ? 1 : 0) << ',' << diagnostics.estimated_memory_bytes << ','
                << eligibility_bytes << ','
                << (enabled ? eligibility_bytes / size.neurons : 0) << ','
                << sizeof(eve::neural::EligibilityRecord) << ','
                << runtime.metrics().processed_events << ','
                << runtime.metrics().fire_events << ','
                << runtime.metrics().transmitted_impulses << ','
                << runtime.metrics().eligibility_decisions << ','
                << runtime.metrics().scheduled_eligibility_expiries << ','
                << runtime.metrics().processed_eligibility_expiries << ','
                << runtime.metrics().maximum_queue_size << ',' << std::setprecision(9)
                << seconds << ',' << (seconds == 0.0 ? 0.0 :
                    runtime.metrics().processed_events / seconds) << '\n';
        }
    }
    const std::uint64_t bytes_per_neuron =
        2ULL * 8ULL * sizeof(eve::SynapseId) +
        2ULL * (sizeof(std::uint32_t) + 2ULL * sizeof(std::uint64_t)) +
        sizeof(std::uint32_t) + 2ULL * sizeof(std::uint64_t);
    for (const std::uint64_t neurons : {100000ULL, 1000000ULL}) {
        out << neurons << ",0,1,0," << neurons * bytes_per_neuron << ','
            << bytes_per_neuron << ',' << sizeof(eve::neural::EligibilityRecord)
            << ",0,0,0,0,0,0,0,0,0\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path directory =
            argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(directory);
        write_parameters(directory / "research-stage-b2-parameters.csv");
        run_selection_matrix(directory / "research-stage-b2-selection.csv");
        run_variant_comparison(directory / "research-stage-b2-variants.csv");
        run_feedback_timing(directory / "research-stage-b2-feedback-timing.csv");
        run_repetitions(directory / "research-stage-b2-repetitions.csv");
        run_controls(directory / "research-stage-b2-controls.csv");
        run_persistence(directory / "research-stage-b2-persistence.csv");
        run_performance(directory / "research-stage-b2-performance.csv");
        std::cout << "wrote B.2 research data to " << directory << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage B.2 failed: " << exception.what() << '\n';
        return 1;
    }
}
