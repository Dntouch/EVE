#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
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
constexpr std::array<std::uint64_t, 6> kEstablished{{20261117, 7, 42, 99, 2026, 314159}};
constexpr std::uint64_t kMotorSeed = 0x41322d4d4f544f52ULL;
constexpr std::uint64_t kTopologySalt = 0x41322d544f504f4cULL;
constexpr std::uint64_t kExplorationSalt = 0x41322d4558504c4fULL;
constexpr std::uint64_t kCircuitSalt = 0x41322d4349524355ULL;
constexpr std::uint64_t kFeedbackSalt = 0x42332d4645454442ULL;

struct Setup { eve::brainstem::MotorCircuitResult circuit; };

eve::ExperimentConfig experiment_config(
    const eve::EligibilitySelection selection = eve::EligibilitySelection::positive_weight,
    const std::uint64_t duration = 64,
    const eve::RecoveryMode recovery =
        eve::RecoveryMode::independent_with_membrane_relaxation) {
    eve::ExperimentConfig config;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.firing_threshold = 3;
    config.refractory_input_events = 16;
    config.recovery_mode = recovery;
    config.recovery_delay = 16;
    config.membrane_relaxation_percent =
        recovery == eve::RecoveryMode::independent_with_membrane_relaxation ? 100 : 0;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = selection;
    config.eligibility_candidate_capacity = selection == eve::EligibilitySelection::none ? 0 : 2;
    config.eligibility_decision_capacity = selection == eve::EligibilitySelection::none ? 0 : 2;
    config.eligibility_duration = selection == eve::EligibilitySelection::none ? 0 : duration;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.maximum_events_per_run = 100'000;
    return config;
}

Setup configure_action(eve::bios::Runtime& runtime, const std::uint64_t seed) {
    eve::brainstem::MotorOutputConfig motor;
    motor.assignment_seed = kMotorSeed;
    motor.response_stimulus_neuron = 8;
    motor.first_motor_neuron = 18;
    runtime.configure_motor_output(motor);
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
    return Setup{runtime.configure_motor_circuit(circuit)};
}

eve::brainstem::EvaluationRouteResult configure_feedback(
    eve::bios::Runtime& runtime,
    const Setup& setup,
    const std::uint64_t seed,
    const eve::brainstem::EvaluationRouteMode mode) {
    eve::brainstem::EvaluationRouteConfig config;
    config.mode = mode;
    config.first_input_neuron = 9;
    config.graded_channel_count = 5;
    config.relay_count = 4;
    config.fanout_per_source = 4;
    config.weight = 3;
    config.seed = seed ^ kFeedbackSalt;
    for (eve::NeuronId neuron = 0; neuron < 9; ++neuron) config.reserved_neurons.push_back(neuron);
    for (const auto& channel : runtime.motor_assignment().channels) {
        config.reserved_neurons.insert(config.reserved_neurons.end(), channel.begin(), channel.end());
    }
    config.reserved_neurons.push_back(runtime.motor_assignment().completion_contact);
    config.reserved_neurons.insert(config.reserved_neurons.end(),
        setup.circuit.relay_neurons.begin(), setup.circuit.relay_neurons.end());
    return runtime.configure_evaluation_route(config);
}

eve::brainstem::ExplorationConfig exploration(const std::uint64_t seed, const std::uint32_t phase) {
    eve::brainstem::ExplorationConfig config;
    config.mode = eve::brainstem::ExplorationMode::distributed;
    config.target_policy = eve::brainstem::ExplorationTargetPolicy::processing_neurons_only;
    config.activation_count = 16;
    config.activation_budget = 16;
    config.seed = (seed ^ kExplorationSalt) + phase;
    config.center = {4, 4, 2};
    config.radius = 2;
    return config;
}

eve::brainstem::MotorResponse action(eve::bios::Runtime& runtime, const std::uint64_t seed,
                                     const std::uint32_t phase) {
    static_cast<void>(runtime.process_utf8_text("AB"));
    static_cast<void>(runtime.enqueue_exploration(exploration(seed, phase)));
    auto response = runtime.request_response(true, 100'000);
    static_cast<void>(runtime.run());
    return response;
}

std::uint8_t quantized_score(const std::uint8_t score) {
    return static_cast<std::uint8_t>(std::min(100U, ((score + 12U) / 25U) * 25U));
}

const char* route_name(const eve::brainstem::EvaluationRouteMode mode) {
    switch (mode) {
    case eve::brainstem::EvaluationRouteMode::none: return "F0";
    case eve::brainstem::EvaluationRouteMode::sensory_input: return "F1";
    case eve::brainstem::EvaluationRouteMode::general_wiring: return "F2";
    case eve::brainstem::EvaluationRouteMode::relay_wiring: return "F3";
    }
    return "unknown";
}

void write_parameters(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "parameter,value\nbase_branch,codex/research-stage-b2-local-eligibility\n"
           "neurons,256\nregular_synapses,1024\ninnate,V2:3\nmotor,M3:1:3\n"
           "memory,G3:8\nrecovery,R2F:16\neligibility,L2:2:2:64\n"
           "routes,F0|F1|F2|F3\nencoding,one-hot:0|25|50|75|100\n"
           "feedback_learning,disabled\nseeds,64\n";
}

void run_routes(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,route,score,actual,target,completed,route_synapses,relays,targets,"
           "feedback_fires,feedback_impulses,with_eligibility,without_eligibility,"
           "multiple_eligibility,new_eligibility,events,queue_max,weights_changed\n";
    for (const auto seed : kSeeds) {
        for (const auto mode : {eve::brainstem::EvaluationRouteMode::none,
                                eve::brainstem::EvaluationRouteMode::sensory_input,
                                eve::brainstem::EvaluationRouteMode::general_wiring,
                                eve::brainstem::EvaluationRouteMode::relay_wiring}) {
            for (const std::uint8_t forced_score : {0, 25, 50, 75, 100}) {
                eve::bios::Runtime runtime(experiment_config(), seed);
                const Setup setup = configure_action(runtime, seed);
                const auto route = configure_feedback(runtime, setup, seed, mode);
                const auto response = action(runtime, seed, 1);
                const auto evaluation = eve::brainstem::Brainstem::evaluate_binary(response.value, 0x41);
                const auto weights = runtime.network().weight_distribution();
                const auto feedback = runtime.process_evaluation_feedback(forced_score);
                out << seed << ',' << route_name(mode) << ',' << unsigned(forced_score) << ','
                    << unsigned(evaluation.actual) << ',' << unsigned(evaluation.target) << ','
                    << (response.valid() ? 1 : 0) << ',' << route.route_synapses.size() << ','
                    << route.relay_neurons.size() << ',' << route.target_neurons.size() << ','
                    << feedback.feedback_fires << ',' << feedback.feedback_impulses << ','
                    << feedback.receipts_with_eligibility << ','
                    << feedback.receipts_without_eligibility << ','
                    << feedback.receipts_with_multiple_eligibilities << ','
                    << feedback.created_eligibilities << ',' << feedback.processed_events << ','
                    << runtime.metrics().maximum_queue_size << ','
                    << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
            }
        }
    }
}

void run_delays(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,duration,delay_class,delay,time,active_before,with_eligibility,"
           "without_eligibility,new_eligibility,expired,stale,weights_changed\n";
    for (const auto seed : kSeeds) {
        for (const std::uint64_t duration : {4ULL, 16ULL, 64ULL}) {
            for (const auto& item : std::array<std::pair<const char*, std::uint64_t>, 4>{{
                     {"immediate", 0}, {"before", duration - 1},
                     {"exact", duration}, {"after", duration + 1}}}) {
                eve::bios::Runtime runtime(experiment_config(
                    eve::EligibilitySelection::positive_weight, duration), seed);
                const Setup setup = configure_action(runtime, seed);
                static_cast<void>(configure_feedback(runtime, setup, seed,
                    eve::brainstem::EvaluationRouteMode::relay_wiring));
                const auto response = action(runtime, seed, 1);
                const auto score = quantized_score(
                    eve::brainstem::Brainstem::evaluate_binary(response.value, 0x41).success_percent);
                const auto active = runtime.network().active_eligibility_count();
                const auto before = runtime.metrics();
                const auto weights = runtime.network().weight_distribution();
                if (item.second == 0) {
                    static_cast<void>(runtime.process_evaluation_feedback(score));
                } else {
                    static_cast<void>(runtime.process_evaluation_feedback(score,
                        eve::brainstem::EvaluationSignalKind::graded, item.second));
                    static_cast<void>(runtime.advance_time_to(item.second));
                }
                out << seed << ',' << duration << ',' << item.first << ',' << item.second << ','
                    << runtime.simulated_time() << ',' << active << ','
                    << runtime.metrics().feedback_receipts_with_eligibility -
                           before.feedback_receipts_with_eligibility << ','
                    << runtime.metrics().feedback_receipts_without_eligibility -
                           before.feedback_receipts_without_eligibility << ','
                    << runtime.metrics().feedback_created_eligibilities -
                           before.feedback_created_eligibilities << ','
                    << runtime.metrics().processed_eligibility_expiries -
                           before.processed_eligibility_expiries << ','
                    << runtime.metrics().stale_eligibility_expiries -
                           before.stale_eligibility_expiries << ','
                    << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
            }
        }
    }
}

void run_sequences(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,repetitions,mode,phase,completed,score,with_eligibility,without_eligibility,"
           "new_eligibility,active_markings,overwrites,pending,weights_changed\n";
    for (const auto seed : kEstablished) {
        for (const std::uint32_t repetitions : {2U, 8U, 32U, 128U}) {
            for (const std::string mode : {"N0", "N1", "N2", "N3", "N4", "N5"}) {
                eve::bios::Runtime runtime(experiment_config(), seed);
                const Setup setup = configure_action(runtime, seed);
                static_cast<void>(configure_feedback(runtime, setup, seed,
                    eve::brainstem::EvaluationRouteMode::relay_wiring));
                const auto weights = runtime.network().weight_distribution();
                for (std::uint32_t phase = 1; phase <= repetitions; ++phase) {
                    const auto before = runtime.metrics();
                    const auto response = action(runtime, seed, phase);
                    auto score = quantized_score(
                        eve::brainstem::Brainstem::evaluate_binary(response.value, 0x41).success_percent);
                    if (mode == "N5") score = phase % 2 == 0 ? 0 : 100;
                    if (mode != "N4") {
                        std::uint64_t due = 0;
                        if (mode == "N1") due = runtime.simulated_time() + 4;
                        if (mode == "N2") due = runtime.simulated_time() + 16;
                        if (mode == "N3") due = runtime.simulated_time() +
                            (phase % 2 == 0 ? 4 : 16);
                        static_cast<void>(runtime.process_evaluation_feedback(score,
                            eve::brainstem::EvaluationSignalKind::graded, due));
                    }
                    static_cast<void>(runtime.advance_time_by(16));
                    out << seed << ',' << repetitions << ',' << mode << ',' << phase << ','
                        << (response.valid() ? 1 : 0) << ',' << unsigned(score) << ','
                        << runtime.metrics().feedback_receipts_with_eligibility -
                               before.feedback_receipts_with_eligibility << ','
                        << runtime.metrics().feedback_receipts_without_eligibility -
                               before.feedback_receipts_without_eligibility << ','
                        << runtime.metrics().feedback_created_eligibilities -
                               before.feedback_created_eligibilities << ','
                        << runtime.network().active_eligibility_count() << ','
                        << runtime.metrics().eligibility_overwritten_decisions -
                               before.eligibility_overwritten_decisions << ','
                        << runtime.future_event_count() << ','
                        << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
                }
            }
        }
    }
}

void run_assignment(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "variant,neural_information,global_action_id,assessment\n"
           "Z0,current-valid-eligibility,0,temporally-ambiguous\n"
           "Z1,regular-boundary-stimulus,0,boundary-observable-but-not-shared-identity\n"
           "Z2,local-context-hypothesis,0,not-reliable-without-new-neural-context-route\n";
}

void run_controls(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,control,selection,action,delay,with_eligibility,without_eligibility,"
           "new_eligibility,active,weight_changes,assessment\n";
    for (const auto seed : kSeeds) {
        for (int control = 0; control <= 6; ++control) {
            const auto selection = control == 0 ? eve::EligibilitySelection::none :
                control == 1 ? eve::EligibilitySelection::all_recent :
                               eve::EligibilitySelection::positive_weight;
            const std::uint64_t duration = control == 6 ? 4 : 64;
            eve::bios::Runtime runtime(experiment_config(selection, duration), seed);
            const Setup setup = configure_action(runtime, seed);
            static_cast<void>(configure_feedback(runtime, setup, seed,
                eve::brainstem::EvaluationRouteMode::relay_wiring));
            if (control == 3 && !setup.circuit.circuit_synapses.empty()) {
                runtime.set_experimental_synapse_weight(setup.circuit.circuit_synapses.front(), 0);
            }
            const auto weights = runtime.network().weight_distribution();
            if (control != 4) static_cast<void>(action(runtime, seed, 1));
            if (control == 5) {
                static_cast<void>(action(runtime, seed, 2));
            }
            if (control == 6) static_cast<void>(runtime.advance_time_by(4));
            const auto before = runtime.metrics();
            static_cast<void>(runtime.process_evaluation_feedback(50));
            out << seed << ",S" << control << ','
                << (selection == eve::EligibilitySelection::none ? "none" :
                    selection == eve::EligibilitySelection::all_recent ? "L1" : "L2")
                << ',' << (control == 4 ? 0 : 1) << ','
                << (control == 6 ? 4 : 0) << ','
                << runtime.metrics().feedback_receipts_with_eligibility -
                       before.feedback_receipts_with_eligibility << ','
                << runtime.metrics().feedback_receipts_without_eligibility -
                       before.feedback_receipts_without_eligibility << ','
                << runtime.metrics().feedback_created_eligibilities -
                       before.feedback_created_eligibilities << ','
                << runtime.network().active_eligibility_count() << ','
                << (runtime.network().weight_distribution() == weights ? 0 : 1) << ','
                << (control == 4 ? "no-valid-action" :
                    control == 5 ? "later-action-overlap" :
                    control == 6 ? "expired-before-feedback" : "control") << '\n';
        }
    }
}

void run_persistence(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,pending_equal,continuation_equal,feedback_fires,feedback_impulses,"
           "weight_changes\n";
    for (const auto seed : kSeeds) {
        eve::bios::Runtime runtime(experiment_config(), seed);
        const Setup setup = configure_action(runtime, seed);
        static_cast<void>(configure_feedback(runtime, setup, seed,
            eve::brainstem::EvaluationRouteMode::relay_wiring));
        static_cast<void>(action(runtime, seed, 1));
        const auto weights = runtime.network().weight_distribution();
        static_cast<void>(runtime.process_evaluation_feedback(75,
            eve::brainstem::EvaluationSignalKind::graded, 8));
        const auto temp = std::filesystem::temp_directory_path() /
            ("eve-b3-" + std::to_string(seed) + ".bin");
        runtime.save(temp.string());
        auto restored = eve::bios::Runtime::load(temp.string());
        const bool pending = runtime.state_fingerprint() == restored.state_fingerprint();
        static_cast<void>(runtime.advance_time_to(8));
        static_cast<void>(restored.advance_time_to(8));
        out << seed << ',' << (pending ? 1 : 0) << ','
            << (runtime.state_fingerprint() == restored.state_fingerprint() ? 1 : 0) << ','
            << runtime.metrics().feedback_fire_events << ','
            << runtime.metrics().feedback_impulses << ','
            << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
        static_cast<void>(std::filesystem::remove(temp));
    }
}

void run_performance(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "neurons,measured,route,added_synapses,feedback_events,maximum_queue,memory_bytes,"
           "seconds,events_per_second,estimated_route_bytes\n";
    for (const auto mode : {eve::brainstem::EvaluationRouteMode::none,
                            eve::brainstem::EvaluationRouteMode::sensory_input,
                            eve::brainstem::EvaluationRouteMode::general_wiring,
                            eve::brainstem::EvaluationRouteMode::relay_wiring}) {
        eve::bios::Runtime runtime(experiment_config(), 8801);
        const Setup setup = configure_action(runtime, 8801);
        const auto route = configure_feedback(runtime, setup, 8801, mode);
        const auto start = std::chrono::steady_clock::now();
        const auto result = runtime.process_evaluation_feedback(100);
        const double seconds = std::chrono::duration<double>(
            std::chrono::steady_clock::now() - start).count();
        out << "256,1," << route_name(mode) << ',' << route.added_synapses << ','
            << result.processed_events << ',' << runtime.metrics().maximum_queue_size << ','
            << runtime.diagnostics().estimated_memory_bytes << ',' << std::setprecision(9)
            << seconds << ',' << (seconds == 0 ? 0 : result.processed_events / seconds) << ','
            << route.route_synapses.size() * sizeof(eve::neural::Synapse) << '\n';
    }
    for (const std::uint64_t neurons : {10000ULL, 100000ULL, 1000000ULL}) {
        const std::uint64_t route_synapses = 5 + 4 * 4;
        out << neurons << ",0,F3," << route_synapses
            << ",0,0,0,0,0," << route_synapses * sizeof(eve::neural::Synapse) << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path directory = argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(directory);
        write_parameters(directory / "research-stage-b3-parameters.csv");
        run_routes(directory / "research-stage-b3-routes.csv");
        run_delays(directory / "research-stage-b3-delays.csv");
        run_sequences(directory / "research-stage-b3-sequences.csv");
        run_assignment(directory / "research-stage-b3-assignment.csv");
        run_controls(directory / "research-stage-b3-controls.csv");
        run_persistence(directory / "research-stage-b3-persistence.csv");
        run_performance(directory / "research-stage-b3-performance.csv");
        std::cout << "wrote B.3 research data to " << directory << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage B.3 failed: " << exception.what() << '\n';
        return 1;
    }
}
