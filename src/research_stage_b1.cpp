#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
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
constexpr std::array<std::uint64_t, 4> kDurations{{1, 4, 16, 64}};
constexpr std::uint64_t kMotorSeed = 0x41322d4d4f544f52ULL;
constexpr std::uint64_t kTopologySalt = 0x41322d544f504f4cULL;
constexpr std::uint64_t kExplorationSalt = 0x41322d4558504c4fULL;
constexpr std::uint64_t kCircuitSalt = 0x41322d4349524355ULL;

struct RecoveryVariant {
    const char* name;
    eve::RecoveryMode mode;
    std::uint32_t relaxation;
};

constexpr std::array<RecoveryVariant, 4> kRecoveryVariants{{
    {"R0", eve::RecoveryMode::input_dependent, 0},
    {"R1", eve::RecoveryMode::independent, 0},
    {"R2H", eve::RecoveryMode::independent_with_membrane_relaxation, 50},
    {"R2F", eve::RecoveryMode::independent_with_membrane_relaxation, 100},
}};

struct Setup {
    eve::brainstem::InnateWiringResult wiring;
    eve::brainstem::MotorCircuitResult circuit;
};

struct PhaseCapture {
    eve::brainstem::MotorResponse response;
    eve::bios::MotorCausality technical;
    eve::bios::LocalMemoryCausality local;
    std::vector<eve::bios::ImpulseObservation> impulses;
    std::uint64_t events{};
    std::uint64_t fires{};
    std::uint64_t suppressions{};
    std::uint64_t transmitted{};
    std::uint32_t data_events{};
    std::uint32_t refractory_before{};
    std::uint32_t refractory_after{};
    std::int64_t membrane_sum_before{};
    std::int64_t membrane_sum_after{};
    double seconds{};
};

const char* status_name(const eve::brainstem::MotorResponseStatus status) {
    switch (status) {
    case eve::brainstem::MotorResponseStatus::no_response: return "no_response";
    case eve::brainstem::MotorResponseStatus::completed: return "completed";
    case eve::brainstem::MotorResponseStatus::aborted: return "aborted";
    }
    return "unknown";
}

std::string hex64(const std::uint64_t value) {
    std::ostringstream output;
    output << std::hex << std::setw(16) << std::setfill('0') << value;
    return output.str();
}

eve::ExperimentConfig experiment_config(
    const eve::RecoveryMode mode,
    const std::uint64_t duration,
    const std::uint32_t relaxation) {
    eve::ExperimentConfig config;
    config.neuron_count = 256;
    config.width = 8;
    config.height = 8;
    config.depth = 4;
    config.initial_outgoing_synapses = 4;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.refractory_input_events = 16;
    config.recovery_mode = mode;
    config.recovery_delay = duration;
    config.membrane_relaxation_percent = relaxation;
    config.local_event_memory_capacity = 8;
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

Setup configure_runtime(eve::bios::Runtime& runtime, const std::uint64_t seed) {
    runtime.configure_motor_output(motor_config());
    eve::brainstem::InnateWiringConfig wiring;
    wiring.topology = eve::brainstem::InnateTopology::functional_paths;
    wiring.initial_weight = 3;
    wiring.topology_seed = seed ^ kTopologySalt;
    Setup setup;
    setup.wiring = runtime.configure_innate_wiring(wiring);
    eve::brainstem::MotorCircuitConfig circuit;
    circuit.mode = eve::brainstem::MotorCircuitMode::activity_dependent;
    circuit.motor_weight = 1;
    circuit.completion_weight = 3;
    circuit.relay_count = 1;
    circuit.seed = seed ^ kCircuitSalt;
    circuit.path_enabled = true;
    setup.circuit = runtime.configure_motor_circuit(circuit);
    return setup;
}

eve::brainstem::ExplorationConfig exploration_config(
    const std::uint64_t seed, const std::uint32_t phase) {
    eve::brainstem::ExplorationConfig config;
    config.mode = eve::brainstem::ExplorationMode::distributed;
    config.target_policy =
        eve::brainstem::ExplorationTargetPolicy::processing_neurons_only;
    config.activation_count = 16;
    config.activation_budget = 16;
    config.seed = (seed ^ kExplorationSalt) + (phase - 1U);
    config.center = {4, 4, 2};
    config.radius = 2;
    return config;
}

std::uint32_t count_refractory(const eve::bios::Runtime& runtime) {
    return static_cast<std::uint32_t>(std::count_if(
        runtime.network().neurons().begin(), runtime.network().neurons().end(),
        [](const eve::neural::Neuron& neuron) {
            return neuron.refractory_events_remaining != 0;
        }));
}

std::int64_t membrane_sum(const eve::bios::Runtime& runtime) {
    return std::accumulate(
        runtime.network().neurons().begin(), runtime.network().neurons().end(),
        std::int64_t{0}, [](const std::int64_t sum, const eve::neural::Neuron& neuron) {
            return sum + neuron.membrane_potential;
        });
}

std::string phase_input(const std::string& pattern, const std::uint32_t phase) {
    if (pattern == "ALT") return phase % 2 == 0 ? "AB" : "BA";
    return pattern;
}

std::string role_of(
    const eve::bios::Runtime& runtime,
    const Setup& setup,
    const eve::NeuronId neuron) {
    if (neuron < 8) return "sensor-" + std::to_string(neuron);
    if (neuron == runtime.motor_assignment().config.response_stimulus_neuron) {
        return "response-stimulus";
    }
    if (!setup.circuit.relay_neurons.empty() &&
        neuron == setup.circuit.relay_neurons.front()) return "accumulator";
    if (neuron == runtime.motor_assignment().completion_contact) return "completion";
    for (std::size_t channel = 0;
         channel < runtime.motor_assignment().channels.size(); ++channel) {
        const auto& contacts = runtime.motor_assignment().channels[channel];
        if (std::find(contacts.begin(), contacts.end(), neuron) != contacts.end()) {
            return "data-" + std::to_string(channel);
        }
    }
    for (const auto& path : setup.wiring.paths) {
        if (neuron == path.first_relay || neuron == path.second_relay) {
            return "innate-relay";
        }
    }
    return "processing";
}

PhaseCapture execute_phase(
    eve::bios::Runtime& runtime,
    const Setup& setup,
    const std::uint64_t seed,
    const std::uint32_t phase,
    const std::string& input,
    std::ostream* diagnostic = nullptr,
    const std::string& diagnostic_prefix = {}) {
    PhaseCapture capture;
    capture.refractory_before = count_refractory(runtime);
    capture.membrane_sum_before = membrane_sum(runtime);
    const auto metrics_before = runtime.metrics();
    runtime.set_impulse_observer(
        [&capture, &runtime, &setup, diagnostic, &diagnostic_prefix, phase](
            const eve::bios::ImpulseObservation& observation) {
            capture.impulses.push_back(observation);
            if (diagnostic == nullptr) return;
            const std::string role = role_of(runtime, setup, observation.receiver);
            if (role == "processing") return;
            *diagnostic << diagnostic_prefix << ',' << phase << ",impulse,"
                        << observation.event_index << ',' << observation.simulated_time << ','
                        << role << ',' << observation.receiver << ','
                        << observation.synapse << ',' << observation.applied_weight << ','
                        << observation.membrane_before << ',' << observation.membrane_after
                        << ',' << observation.refractory_before << ','
                        << observation.refractory_after << ','
                        << (observation.receiver_fired ? 1 : 0) << ','
                        << (observation.firing_suppressed ? 1 : 0) << '\n';
        });
    runtime.set_recovery_observer(
        [&runtime, &setup, diagnostic, &diagnostic_prefix, phase](
            const eve::bios::RecoveryObservation& observation) {
            if (diagnostic == nullptr) return;
            const std::string role = role_of(runtime, setup, observation.neuron);
            if (role == "processing") return;
            *diagnostic << diagnostic_prefix << ',' << phase << ",recovery,"
                        << observation.event_index << ',' << observation.simulated_time << ','
                        << role << ',' << observation.neuron << ",-1,0,"
                        << observation.membrane_before << ',' << observation.membrane_after
                        << ',' << observation.refractory_before << ','
                        << observation.refractory_after << ",0,0\n";
        });
    const auto start = std::chrono::steady_clock::now();
    static_cast<void>(runtime.process_utf8_text(input));
    static_cast<void>(runtime.enqueue_exploration(exploration_config(seed, phase)));
    capture.response = runtime.request_response(true, 100'000);
    capture.technical = eve::bios::Runtime::analyze_motor_causality(
        capture.response, setup.circuit, capture.impulses);
    capture.local = runtime.analyze_local_motor_memory(
        capture.response, setup.circuit, capture.impulses);
    static_cast<void>(runtime.run());
    capture.seconds = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - start).count();
    runtime.set_impulse_observer({});
    runtime.set_recovery_observer({});
    const auto& after = runtime.metrics();
    capture.events = after.processed_events - metrics_before.processed_events;
    capture.fires = after.fire_events - metrics_before.fire_events;
    capture.suppressions = after.refractory_suppressed_firings -
                           metrics_before.refractory_suppressed_firings;
    capture.transmitted = after.transmitted_impulses -
                          metrics_before.transmitted_impulses;
    capture.data_events = static_cast<std::uint32_t>(std::count_if(
        capture.response.activations.begin(), capture.response.activations.end(),
        [](const eve::brainstem::MotorActivation& activation) {
            return activation.channel >= 0;
        }));
    capture.refractory_after = count_refractory(runtime);
    capture.membrane_sum_after = membrane_sum(runtime);
    return capture;
}

void write_parameters(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write B.1 parameters");
    output << "parameter,value\nbase_branch,codex/research-stage-b-local-event-memory\n"
           << "neuron_count,256\nregular_initial_synapses,1024\ngeometry,8x8x4\n"
           << "innate_topology,V2\ninnate_weight,3\nmotor_circuit,M3\n"
           << "motor_weight,1\ncompletion_weight,3\nfiring_threshold,3\n"
           << "refractory_events_r0,16\nlocal_event_memory,G3:8\n"
           << "learning_enabled,0\ngrowth_enabled,0\nexploration,E4-new-16\n"
           << "seed_count,64\ndurations,T1:1|T2:4|T3:16|T4:64\n"
           << "r2_relaxation,half:50|full:100\nprimary_gap,16\n"
           << "primary_repetitions,8\nlong_repetitions,2|32|128\n";
}

void run_timing_matrix(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write B.1 timing matrix");
    output << "seed,recovery,duration,relaxation,gap_class,gap,first_data,"
              "first_proven,second_data,second_proven,second_status,"
              "refractory_before_second,refractory_after_second,"
              "recoveries_in_gap,suppressions_second,pending_after_second,"
              "membrane_before_second,membrane_after_second,total_events,"
              "state_fingerprint\n";
    for (const std::uint64_t seed : kSeeds) {
        for (const auto& variant : kRecoveryVariants) {
            const std::vector<std::uint64_t> durations =
                variant.mode == eve::RecoveryMode::input_dependent
                    ? std::vector<std::uint64_t>{16}
                    : std::vector<std::uint64_t>(kDurations.begin(), kDurations.end());
            for (const std::uint64_t duration : durations) {
                for (const auto& [gap_class, gap] :
                     std::array<std::pair<const char*, std::uint64_t>, 5>{{
                         {"none", 0}, {"short", duration / 2}, {"equal", duration},
                         {"double", duration * 2}, {"long", duration * 4},
                     }}) {
                    eve::bios::Runtime runtime(
                        experiment_config(variant.mode, duration, variant.relaxation), seed);
                    const Setup setup = configure_runtime(runtime, seed);
                    const PhaseCapture first = execute_phase(
                        runtime, setup, seed, 1, "AB");
                    const auto recoveries_before =
                        runtime.metrics().processed_recovery_events;
                    static_cast<void>(runtime.advance_time_by(gap));
                    const auto recoveries = runtime.metrics().processed_recovery_events -
                                            recoveries_before;
                    const std::uint32_t refractory_before = count_refractory(runtime);
                    const std::int64_t membrane_before = membrane_sum(runtime);
                    const PhaseCapture second = execute_phase(
                        runtime, setup, seed, 2, "AB");
                    output << seed << ',' << variant.name << ',' << duration << ','
                           << variant.relaxation << ',' << gap_class << ',' << gap << ','
                           << (first.data_events != 0 ? 1 : 0) << ','
                           << (first.technical.proven_circuit_path ? 1 : 0) << ','
                           << (second.data_events != 0 ? 1 : 0) << ','
                           << (second.technical.proven_circuit_path ? 1 : 0) << ','
                           << status_name(second.response.status) << ','
                           << refractory_before << ',' << second.refractory_after << ','
                           << recoveries << ',' << second.suppressions << ','
                           << runtime.future_event_count() << ',' << membrane_before << ','
                           << second.membrane_sum_after << ','
                           << runtime.metrics().processed_events << ','
                           << hex64(runtime.state_fingerprint()) << '\n';
                }
            }
        }
    }
}

struct Control {
    const char* name;
    const char* recovery;
    eve::RecoveryMode mode;
    std::uint32_t relaxation;
    bool restored;
};

constexpr std::array<Control, 4> kControls{{
    {"K0", "R0", eve::RecoveryMode::input_dependent, 0, true},
    {"K1", "R0", eve::RecoveryMode::input_dependent, 0, false},
    {"K2", "R1", eve::RecoveryMode::independent, 0, false},
    {"K3", "R2F", eve::RecoveryMode::independent_with_membrane_relaxation, 100, false},
}};

void write_repetition_row(
    std::ostream& output,
    const std::uint64_t seed,
    const Control& control,
    const std::string& pattern,
    const std::uint32_t target_repetitions,
    const std::uint32_t phase,
    const std::string& input,
    const PhaseCapture& capture,
    const eve::bios::Runtime& runtime,
    const std::uint64_t gap_recoveries) {
    output << seed << ',' << control.name << ',' << control.recovery << ','
           << (control.restored ? "restored" : "continued") << ',' << pattern << ','
           << target_repetitions << ',' << phase << ',' << input << ','
           << runtime.simulated_time() << ',' << status_name(capture.response.status) << ','
           << capture.data_events << ','
           << (capture.technical.completed_without_prior_data ? 1 : 0) << ','
           << (capture.technical.proven_circuit_path ? 1 : 0) << ','
           << (capture.local.full_path_reconstructable ? 1 : 0) << ','
           << capture.refractory_before << ',' << capture.refractory_after << ','
           << capture.suppressions << ',' << gap_recoveries << ','
           << capture.membrane_sum_before << ',' << capture.membrane_sum_after << ','
           << capture.events << ',' << capture.fires << ',' << capture.transmitted << ','
           << runtime.future_event_count() << ',' << std::setprecision(9)
           << capture.seconds << ',' << hex64(runtime.state_fingerprint()) << '\n';
}

void run_repetitions(
    const std::filesystem::path& result_path,
    const std::filesystem::path& diagnostic_path) {
    std::ofstream output(result_path);
    std::ofstream diagnostic(diagnostic_path);
    if (!output || !diagnostic) throw std::runtime_error("cannot write repetitions");
    output << "seed,control,recovery,state_mode,input_pattern,target_repetitions,phase,"
              "input,simulated_time,response_status,data_events,completion_without_data,"
              "proven_m3_path,local_m3_path,refractory_before,refractory_after,"
              "suppressions,gap_recoveries,membrane_before,membrane_after,events,fires,"
              "transmitted,pending_future,seconds,state_fingerprint\n";
    diagnostic << "seed,control,recovery,input_pattern,phase,event_kind,event_index,"
                  "simulated_time,role,neuron,synapse,weight,membrane_before,"
                  "membrane_after,refractory_before,refractory_after,fired,suppressed\n";
    const std::array<std::string, 5> patterns{{"AB", "BA", "AA", "BB", "ALT"}};

    for (const std::uint64_t seed : kSeeds) {
        for (const Control& control : kControls) {
            for (const std::string& pattern : patterns) {
                const auto config = experiment_config(control.mode, 16, control.relaxation);
                eve::bios::Runtime continued(config, seed);
                const Setup continued_setup = configure_runtime(continued, seed);
                const auto state_path = std::filesystem::temp_directory_path() /
                    ("eve-b1-" + std::to_string(seed) + "-" + control.name + "-" +
                     pattern + ".bin");
                if (control.restored) continued.save(state_path.string());
                for (std::uint32_t phase = 1; phase <= 8; ++phase) {
                    eve::bios::Runtime restored = control.restored
                        ? eve::bios::Runtime::load(state_path.string())
                        : eve::bios::Runtime(config, seed);
                    eve::bios::Runtime* runtime = &continued;
                    Setup setup = continued_setup;
                    if (control.restored) {
                        setup = configure_runtime(restored, seed);
                        runtime = &restored;
                    }
                    std::ostringstream prefix;
                    prefix << seed << ',' << control.name << ',' << control.recovery << ','
                           << pattern;
                    const bool detailed =
                        std::find(kEstablishedSeeds.begin(), kEstablishedSeeds.end(), seed) !=
                            kEstablishedSeeds.end() && phase <= 2;
                    const std::string input = phase_input(pattern, phase - 1);
                    const PhaseCapture capture = execute_phase(
                        *runtime, setup, seed, phase, input,
                        detailed ? &diagnostic : nullptr, prefix.str());
                    const auto before = runtime->metrics().processed_recovery_events;
                    static_cast<void>(runtime->advance_time_by(16));
                    const auto gap_recoveries =
                        runtime->metrics().processed_recovery_events - before;
                    write_repetition_row(
                        output, seed, control, pattern, 8, phase, input, capture,
                        *runtime, gap_recoveries);
                }
                if (control.restored) {
                    static_cast<void>(std::filesystem::remove(state_path));
                }
            }
        }
    }

    for (const std::uint64_t seed : kEstablishedSeeds) {
        for (const Control& control : kControls) {
            for (const std::uint32_t repetitions : {2U, 32U, 128U}) {
                eve::bios::Runtime runtime(
                    experiment_config(control.mode, 16, control.relaxation), seed);
                const Setup setup = configure_runtime(runtime, seed);
                const auto state_path = std::filesystem::temp_directory_path() /
                    ("eve-b1-long-" + std::to_string(seed) + "-" + control.name +
                     "-" + std::to_string(repetitions) + ".bin");
                if (control.restored) runtime.save(state_path.string());
                for (std::uint32_t phase = 1; phase <= repetitions; ++phase) {
                    eve::bios::Runtime restored = control.restored
                        ? eve::bios::Runtime::load(state_path.string())
                        : eve::bios::Runtime(experiment_config(
                              control.mode, 16, control.relaxation), seed);
                    eve::bios::Runtime* active = &runtime;
                    Setup active_setup = setup;
                    if (control.restored) {
                        active_setup = configure_runtime(restored, seed);
                        active = &restored;
                    }
                    const PhaseCapture capture = execute_phase(
                        *active, active_setup, seed, phase, "AB");
                    const auto before = active->metrics().processed_recovery_events;
                    static_cast<void>(active->advance_time_by(16));
                    const auto gap_recoveries =
                        active->metrics().processed_recovery_events - before;
                    write_repetition_row(
                        output, seed, control, "AB", repetitions, phase, "AB", capture,
                        *active, gap_recoveries);
                }
                if (control.restored) {
                    static_cast<void>(std::filesystem::remove(state_path));
                }
            }
        }
    }
}

void run_integrity(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write integrity results");
    output << "seed,recovery,relaxation,response_status,proven_path,"
              "synapses_equal,local_memories_equal,weights_changed,activity_changed,"
              "membranes_changed,refractory_before,refractory_after,recoveries,"
              "pending_before,pending_after,persistence_pending_equal,"
              "persistence_continuation_equal\n";
    for (const std::uint64_t seed : kSeeds) {
        for (const auto& variant : std::array<RecoveryVariant, 3>{{
                 {"R1", eve::RecoveryMode::independent, 0},
                 {"R2H", eve::RecoveryMode::independent_with_membrane_relaxation, 50},
                 {"R2F", eve::RecoveryMode::independent_with_membrane_relaxation, 100},
             }}) {
            eve::bios::Runtime runtime(
                experiment_config(variant.mode, 16, variant.relaxation), seed);
            const Setup setup = configure_runtime(runtime, seed);
            const PhaseCapture capture = execute_phase(runtime, setup, seed, 1, "AB");
            const auto synapses_before = runtime.network().synapses();
            std::vector<std::vector<eve::SynapseId>> memories_before;
            std::vector<std::int64_t> membranes_before;
            for (eve::NeuronId neuron = 0; neuron < runtime.network().neuron_count(); ++neuron) {
                memories_before.push_back(runtime.network().local_event_memory(neuron));
                membranes_before.push_back(runtime.network().neuron(neuron).membrane_potential);
            }
            const auto pending_before = runtime.future_event_count();
            const auto refractory_before = count_refractory(runtime);

            bool persistence_pending_equal = true;
            bool persistence_continuation_equal = true;
            if (std::find(kEstablishedSeeds.begin(), kEstablishedSeeds.end(), seed) !=
                kEstablishedSeeds.end()) {
                const auto state_path = std::filesystem::temp_directory_path() /
                    ("eve-b1-persist-" + std::to_string(seed) + "-" + variant.name + ".bin");
                runtime.save(state_path.string());
                eve::bios::Runtime restored = eve::bios::Runtime::load(state_path.string());
                persistence_pending_equal =
                    runtime.state_fingerprint() == restored.state_fingerprint();
                static_cast<void>(runtime.advance_time_by(16));
                static_cast<void>(restored.advance_time_by(16));
                persistence_continuation_equal =
                    runtime.state_fingerprint() == restored.state_fingerprint();
                static_cast<void>(std::filesystem::remove(state_path));
            } else {
                static_cast<void>(runtime.advance_time_by(16));
            }

            std::uint32_t weights_changed = 0;
            std::uint32_t activity_changed = 0;
            const auto& after_synapses = runtime.network().synapses();
            for (std::size_t index = 0; index < after_synapses.size(); ++index) {
                weights_changed += after_synapses[index].weight != synapses_before[index].weight;
                activity_changed += after_synapses[index].activity_count !=
                                    synapses_before[index].activity_count;
            }
            bool memories_equal = true;
            std::uint32_t membranes_changed = 0;
            for (eve::NeuronId neuron = 0; neuron < runtime.network().neuron_count(); ++neuron) {
                memories_equal = memories_equal &&
                    runtime.network().local_event_memory(neuron) == memories_before[neuron];
                membranes_changed += runtime.network().neuron(neuron).membrane_potential !=
                                     membranes_before[neuron];
            }
            output << seed << ',' << variant.name << ',' << variant.relaxation << ','
                   << status_name(capture.response.status) << ','
                   << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                   << (after_synapses == synapses_before ? 1 : 0) << ','
                   << (memories_equal ? 1 : 0) << ',' << weights_changed << ','
                   << activity_changed << ',' << membranes_changed << ','
                   << refractory_before << ',' << count_refractory(runtime) << ','
                   << runtime.metrics().processed_recovery_events << ','
                   << pending_before << ',' << runtime.future_event_count() << ','
                   << (persistence_pending_equal ? 1 : 0) << ','
                   << (persistence_continuation_equal ? 1 : 0) << '\n';
        }
    }
}

void run_feedback(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write feedback results");
    output << "seed,recovery,feedback,phase,response_status,data_events,proven_path,"
              "feedback_events,feedback_processed,feedback_aborted,gap_recoveries,"
              "refractory_after,pending_future,total_events,state_fingerprint\n";
    const std::array<RecoveryVariant, 3> variants{{
        {"R0", eve::RecoveryMode::input_dependent, 0},
        {"R1", eve::RecoveryMode::independent, 0},
        {"R2F", eve::RecoveryMode::independent_with_membrane_relaxation, 100},
    }};
    for (const std::uint64_t seed : kEstablishedSeeds) {
        for (const auto& variant : variants) {
            for (const auto feedback_mode : {
                     eve::brainstem::FeedbackMode::none,
                     eve::brainstem::FeedbackMode::after_completion,
                     eve::brainstem::FeedbackMode::per_motor_event,
                 }) {
                eve::bios::Runtime runtime(
                    experiment_config(variant.mode, 16, variant.relaxation), seed);
                const Setup setup = configure_runtime(runtime, seed);
                const char* feedback_name = feedback_mode == eve::brainstem::FeedbackMode::none
                    ? "F0" : (feedback_mode ==
                        eve::brainstem::FeedbackMode::after_completion ? "F1" : "F2");
                for (std::uint32_t phase = 1; phase <= 8; ++phase) {
                    const PhaseCapture capture = execute_phase(
                        runtime, setup, seed, phase, "AB");
                    eve::brainstem::FeedbackConfig feedback;
                    feedback.mode = feedback_mode;
                    feedback.first_feedback_neuron = 9;
                    feedback.event_budget = 100'000;
                    const auto feedback_result = runtime.process_motor_feedback(
                        capture.response, feedback);
                    const auto before = runtime.metrics().processed_recovery_events;
                    static_cast<void>(runtime.advance_time_by(16));
                    const auto recoveries = runtime.metrics().processed_recovery_events - before;
                    output << seed << ',' << variant.name << ',' << feedback_name << ','
                           << phase << ',' << status_name(capture.response.status) << ','
                           << capture.data_events << ','
                           << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                           << feedback_result.sensory_events.size() << ','
                           << feedback_result.processed_events << ','
                           << (feedback_result.aborted ? 1 : 0) << ',' << recoveries << ','
                           << count_refractory(runtime) << ',' << runtime.future_event_count()
                           << ',' << runtime.metrics().processed_events << ','
                           << hex64(runtime.state_fingerprint()) << '\n';
                }
            }
        }
    }
}

void run_performance(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write performance results");
    output << "neurons,synapses,recovery,repetition,scheduled_recoveries,"
              "processed_recoveries,stale_recoveries,maximum_queue,memory_bytes,"
              "event_struct_bytes,neuron_struct_bytes,seconds,events_per_second,"
              "extrapolated_100k_bytes,extrapolated_1m_bytes\n";
    struct Size { std::uint32_t neurons; std::uint32_t width; std::uint32_t height;
                  std::uint32_t depth; };
    const std::array<Size, 3> sizes{{
        {256, 8, 8, 4}, {1024, 16, 16, 4}, {10000, 25, 20, 20},
    }};
    for (const Size& size : sizes) {
        for (const auto& variant : kRecoveryVariants) {
            for (std::uint32_t repetition = 1; repetition <= 3; ++repetition) {
                auto config = experiment_config(variant.mode, 16, variant.relaxation);
                config.neuron_count = size.neurons;
                config.width = size.width;
                config.height = size.height;
                config.depth = size.depth;
                config.initial_synapse_weight = 1;
                config.firing_threshold = 3;
                config.maximum_events_per_run = 1'000'000;
                eve::bios::Runtime runtime(config, 700 + repetition);
                const auto start = std::chrono::steady_clock::now();
                const std::uint32_t sources = std::min<std::uint32_t>(64, size.neurons);
                for (std::uint32_t repeat = 0; repeat < 3; ++repeat) {
                    for (eve::NeuronId source = 0; source < sources; ++source) {
                        runtime.enqueue_fire(source);
                    }
                }
                static_cast<void>(runtime.run());
                static_cast<void>(runtime.advance_time_by(16));
                const double seconds = std::chrono::duration<double>(
                    std::chrono::steady_clock::now() - start).count();
                const auto diagnostics = runtime.diagnostics();
                const double rate = seconds == 0.0 ? 0.0
                    : static_cast<double>(runtime.metrics().processed_events) / seconds;
                const std::uint64_t per_neuron = sizeof(eve::neural::Neuron) +
                    8ULL * sizeof(eve::SynapseId) +
                    2ULL * sizeof(std::uint32_t) + sizeof(std::uint64_t);
                output << size.neurons << ',' << runtime.network().synapse_count() << ','
                       << variant.name << ',' << repetition << ','
                       << runtime.metrics().scheduled_recovery_events << ','
                       << runtime.metrics().processed_recovery_events << ','
                       << runtime.metrics().stale_recovery_events << ','
                       << runtime.metrics().maximum_queue_size << ','
                       << diagnostics.estimated_memory_bytes << ',' << sizeof(eve::bios::Event)
                       << ',' << sizeof(eve::neural::Neuron) << ','
                       << std::setprecision(9) << seconds << ',' << rate << ','
                       << per_neuron * 100'000ULL << ',' << per_neuron * 1'000'000ULL
                       << '\n';
            }
        }
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output_directory =
            argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(output_directory);
        write_parameters(output_directory / "research-stage-b1-parameters.csv");
        run_timing_matrix(output_directory / "research-stage-b1-timing.csv");
        run_repetitions(
            output_directory / "research-stage-b1-repetitions.csv",
            output_directory / "research-stage-b1-neuron-diagnostics.csv");
        run_integrity(output_directory / "research-stage-b1-integrity.csv");
        run_feedback(output_directory / "research-stage-b1-feedback.csv");
        run_performance(output_directory / "research-stage-b1-performance.csv");
        std::cout << "wrote B.1 research data to " << output_directory << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage B.1 failed: " << exception.what() << '\n';
        return 1;
    }
}
