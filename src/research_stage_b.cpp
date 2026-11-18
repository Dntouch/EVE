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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using eve::brainstem::ExplorationMode;
using eve::brainstem::ExplorationTargetPolicy;
using eve::brainstem::MotorCircuitMode;

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
constexpr std::array<std::uint32_t, 5> kCapacities{{0, 2, 4, 8, 16}};
constexpr std::uint64_t kMotorSeed = 0x41322d4d4f544f52ULL;
constexpr std::uint64_t kTopologySalt = 0x41322d544f504f4cULL;
constexpr std::uint64_t kExplorationSalt = 0x41322d4558504c4fULL;
constexpr std::uint64_t kCircuitSalt = 0x41322d4349524355ULL;

struct Capture {
    eve::brainstem::ExplorationResult exploration;
    eve::brainstem::MotorResponse response;
    eve::bios::MotorCausality technical;
    eve::bios::LocalMemoryCausality local;
    std::vector<eve::bios::FireObservation> fires;
    std::vector<eve::bios::ImpulseObservation> impulses;
};

const char* variant_name(const std::uint32_t capacity) {
    switch (capacity) {
    case 0: return "G0";
    case 2: return "G1";
    case 4: return "G2";
    case 8: return "G3";
    case 16: return "G4";
    default: return "G?";
    }
}

const char* status_name(const eve::brainstem::MotorResponseStatus status) {
    switch (status) {
    case eve::brainstem::MotorResponseStatus::no_response: return "no_response";
    case eve::brainstem::MotorResponseStatus::completed: return "completed";
    case eve::brainstem::MotorResponseStatus::aborted: return "aborted";
    }
    return "unknown";
}

std::string csv(const std::string& value) {
    if (value.find_first_of(",\"\n\r") == std::string::npos) return value;
    std::string output = "\"";
    for (const char character : value) {
        output += character == '\"' ? "\"\"" : std::string(1, character);
    }
    return output + '\"';
}

std::string ids(const std::vector<eve::SynapseId>& values) {
    std::ostringstream output;
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) output << '|';
        output << values[index];
    }
    return output.str();
}

std::string hex64(const std::uint64_t value) {
    std::ostringstream output;
    output << std::hex << std::setw(16) << std::setfill('0') << value;
    return output.str();
}

eve::ExperimentConfig experiment_config(const std::uint32_t capacity) {
    eve::ExperimentConfig config;
    config.neuron_count = 256;
    config.width = 8;
    config.height = 8;
    config.depth = 4;
    config.initial_outgoing_synapses = 4;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.refractory_input_events = 16;
    config.local_event_memory_capacity = capacity;
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

eve::brainstem::InnateWiringConfig wiring_config(const std::uint64_t seed) {
    eve::brainstem::InnateWiringConfig config;
    config.topology = eve::brainstem::InnateTopology::functional_paths;
    config.initial_weight = 3;
    config.topology_seed = seed ^ kTopologySalt;
    return config;
}

eve::brainstem::MotorCircuitConfig circuit_config(
    const std::uint64_t seed, const bool enabled) {
    eve::brainstem::MotorCircuitConfig config;
    config.mode = MotorCircuitMode::activity_dependent;
    config.motor_weight = 1;
    config.completion_weight = 3;
    config.relay_count = 1;
    config.seed = seed ^ kCircuitSalt;
    config.path_enabled = enabled;
    return config;
}

eve::brainstem::MotorCircuitResult configure_runtime(
    eve::bios::Runtime& runtime, const std::uint64_t seed, const bool enabled) {
    runtime.configure_motor_output(motor_config());
    static_cast<void>(runtime.configure_innate_wiring(wiring_config(seed)));
    return runtime.configure_motor_circuit(circuit_config(seed, enabled));
}

std::vector<eve::NeuronId> common_reservations(const std::uint64_t seed) {
    eve::bios::Runtime runtime(experiment_config(0), seed);
    runtime.configure_motor_output(motor_config());
    const auto circuit = runtime.configure_motor_circuit(circuit_config(seed, true));
    return circuit.relay_neurons;
}

eve::brainstem::ExplorationConfig exploration_config(const std::uint64_t seed) {
    eve::brainstem::ExplorationConfig config;
    config.mode = ExplorationMode::distributed;
    config.target_policy = ExplorationTargetPolicy::processing_neurons_only;
    config.activation_count = 16;
    config.activation_budget = 16;
    config.seed = seed ^ kExplorationSalt;
    config.center = {4, 4, 2};
    config.radius = 2;
    config.additional_reserved_neurons = common_reservations(seed);
    return config;
}

Capture execute(
    eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const std::uint64_t seed,
    const bool diagnostic = true) {
    Capture capture;
    if (diagnostic) {
        runtime.set_fire_observer(
            [&capture](const eve::bios::FireObservation& observation) {
                capture.fires.push_back(observation);
            });
        runtime.set_impulse_observer(
            [&capture](const eve::bios::ImpulseObservation& observation) {
                capture.impulses.push_back(observation);
            });
    }
    static_cast<void>(runtime.process_utf8_text("AB"));
    capture.exploration = runtime.enqueue_exploration(exploration_config(seed));
    capture.response = runtime.request_response(true, 100'000);
    capture.technical = eve::bios::Runtime::analyze_motor_causality(
        capture.response, circuit, capture.impulses);
    capture.local = runtime.analyze_local_motor_memory(
        capture.response, circuit, capture.impulses);
    if (capture.response.valid() && runtime.diagnostics().queued_events != 0) {
        static_cast<void>(runtime.run());
    }
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    return capture;
}

std::string neuron_role(
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const eve::NeuronId neuron) {
    if (!circuit.relay_neurons.empty() && neuron == circuit.relay_neurons.front()) {
        return "accumulator";
    }
    if (neuron == runtime.motor_assignment().completion_contact) return "completion";
    for (std::size_t channel = 0;
         channel < runtime.motor_assignment().channels.size(); ++channel) {
        const auto& contacts = runtime.motor_assignment().channels[channel];
        if (std::find(contacts.begin(), contacts.end(), neuron) != contacts.end()) {
            return "data-" + std::to_string(channel);
        }
    }
    return "processing";
}

std::uint32_t relevant_entries(
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const eve::NeuronId receiver,
    const std::vector<eve::SynapseId>& memory) {
    const std::set<eve::SynapseId> circuit_ids(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    return static_cast<std::uint32_t>(std::count_if(
        memory.begin(), memory.end(), [&](const eve::SynapseId id) {
            const auto& synapse = runtime.network().synapse(id);
            return circuit_ids.find(id) != circuit_ids.end() &&
                   synapse.receiver == receiver && synapse.weight > 0;
        }));
}

void write_parameters(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write B parameters");
    output << "parameter,value\n"
           << "base_branch,codex/research-stage-a2-action-causality\n"
           << "neuron_count,256\ngeometry,8x8x4\nregular_initial_synapses,1024\n"
           << "regular_initial_weight,0\ninnate_topology,V2\ninnate_weight,3\n"
           << "motor_circuit,M3\nmotor_weight,1\ncompletion_weight,3\n"
           << "firing_threshold,3\nrefractory_events,16\nlearning_enabled,0\n"
           << "growth_enabled,0\ninput,AB\nexploration,E4-new-16\n"
           << "event_budget,100000\nseed_count,64\na2_seed_count,32\n"
           << "memory_variants,G0:0|G1:2|G2:4|G3:8|G4:16\n"
           << "memory_entry_bytes,4\nmanagement_bytes_per_neuron,16\n"
           << "memory_lifetime,persisted-neuronal-state-until-overwritten\n";
}

void write_snapshots(
    std::ostream& output,
    const std::uint64_t seed,
    const std::uint32_t capacity,
    const char* control,
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const Capture& capture) {
    const std::set<eve::SynapseId> circuit_ids(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    for (const auto& impulse : capture.impulses) {
        if (!impulse.receiver_fired) continue;
        const std::string role = neuron_role(runtime, circuit, impulse.receiver);
        if (role == "processing") continue;
        std::set<eve::SynapseId> distinct(
            impulse.local_event_snapshot.begin(), impulse.local_event_snapshot.end());
        std::uint32_t inactive = 0;
        std::uint32_t unassigned = 0;
        for (const eve::SynapseId id : impulse.local_event_snapshot) {
            const auto& synapse = runtime.network().synapse(id);
            if (synapse.weight == 0) ++inactive;
            if (circuit_ids.find(id) == circuit_ids.end() ||
                synapse.receiver != impulse.receiver) {
                ++unassigned;
            }
        }
        output << seed << ',' << variant_name(capacity) << ',' << control << ','
               << impulse.event_index << ',' << role << ',' << impulse.receiver << ','
               << impulse.synapse << ',' << impulse.applied_weight << ','
               << impulse.membrane_before << ',' << impulse.membrane_after << ','
               << impulse.refractory_before << ',' << impulse.refractory_after << ','
               << impulse.local_event_snapshot.size() << ',' << distinct.size() << ','
               << relevant_entries(runtime, circuit, impulse.receiver,
                                   impulse.local_event_snapshot)
               << ',' << inactive << ',' << unassigned << ','
               << csv(ids(impulse.local_event_snapshot)) << '\n';
    }
}

void write_retention_line(
    std::ostream& output,
    const std::uint64_t seed,
    const std::uint32_t capacity,
    const std::string& role,
    const std::string& stage,
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const eve::NeuronId neuron,
    const std::vector<eve::SynapseId>& memory) {
    std::set<eve::SynapseId> distinct(memory.begin(), memory.end());
    output << seed << ',' << variant_name(capacity) << ',' << role << ',' << stage << ','
           << neuron << ',' << memory.size() << ',' << distinct.size() << ','
           << relevant_entries(runtime, circuit, neuron, memory) << ','
           << runtime.network().local_event_overwrites(neuron) << ','
           << csv(ids(memory)) << '\n';
}

void run_primary(
    const std::filesystem::path& result_path,
    const std::filesystem::path& snapshot_path,
    const std::filesystem::path& retention_path) {
    std::ofstream result(result_path);
    std::ofstream snapshots(snapshot_path);
    std::ofstream retention(retention_path);
    if (!result || !snapshots || !retention) {
        throw std::runtime_error("cannot write B primary results");
    }
    result << "seed,variant,capacity,control,path_enabled,response_status,response_value,"
              "data_events,technical_completed_after_data,technical_proven_path,"
              "local_accumulator_fired,local_completion_fired,required_accumulator_inputs,"
              "retained_accumulator_inputs,retained_completion_inputs,"
              "local_accumulator_reconstructable,local_completion_reconstructable,"
              "local_full_path,technical_local_agreement,inactive_entries,"
              "unassigned_entries,memory_records,memory_overwrites,overwrite_share,"
              "total_events,total_fires,transmitted_impulses,memory_bytes,seconds,"
              "synaptic_events_per_second,state_fingerprint\n";
    snapshots << "seed,variant,control,event_index,role,neuron,trigger_synapse,"
                 "trigger_weight,membrane_before,membrane_after,refractory_before,"
                 "refractory_after,snapshot_entries,distinct_synapses,relevant_entries,"
                 "inactive_entries,unassigned_entries,synapse_order\n";
    retention << "seed,variant,role,stage,neuron,entries,distinct_synapses,"
                 "relevant_entries,total_overwrites,synapse_order\n";

    for (const std::uint64_t seed : kSeeds) {
        for (const std::uint32_t capacity : kCapacities) {
            for (const bool enabled : {true, false}) {
                const char* control = !enabled ? "K2" : (capacity == 0 ? "K0" : "K1");
                eve::bios::Runtime runtime(experiment_config(capacity), seed);
                const auto circuit = configure_runtime(runtime, seed, enabled);
                const auto start = std::chrono::steady_clock::now();
                const Capture capture = execute(runtime, circuit, seed, true);
                const double seconds = std::chrono::duration<double>(
                    std::chrono::steady_clock::now() - start).count();
                const auto& metrics = runtime.metrics();
                const auto diagnostics = runtime.diagnostics();
                const auto data_count = static_cast<std::uint32_t>(std::count_if(
                    capture.response.activations.begin(), capture.response.activations.end(),
                    [](const eve::brainstem::MotorActivation& activation) {
                        return activation.channel >= 0;
                    }));
                const double overwrite_share = metrics.local_event_memory_records == 0 ? 0.0
                    : static_cast<double>(metrics.local_event_memory_overwrites) /
                      static_cast<double>(metrics.local_event_memory_records);
                const double rate = seconds == 0.0 ? 0.0
                    : static_cast<double>(metrics.transmitted_impulses) / seconds;
                result << seed << ',' << variant_name(capacity) << ',' << capacity << ','
                       << control << ',' << (enabled ? 1 : 0) << ','
                       << status_name(capture.response.status) << ','
                       << static_cast<unsigned>(capture.response.value) << ',' << data_count << ','
                       << (capture.technical.completed_after_data ? 1 : 0) << ','
                       << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                       << (capture.local.accumulator_fired ? 1 : 0) << ','
                       << (capture.local.completion_fired ? 1 : 0) << ','
                       << capture.local.required_accumulator_inputs << ','
                       << capture.local.retained_accumulator_inputs << ','
                       << capture.local.retained_completion_inputs << ','
                       << (capture.local.accumulator_reconstructable ? 1 : 0) << ','
                       << (capture.local.completion_reconstructable ? 1 : 0) << ','
                       << (capture.local.full_path_reconstructable ? 1 : 0) << ','
                       << (capture.technical.proven_circuit_path ==
                                   capture.local.full_path_reconstructable ? 1 : 0)
                       << ',' << capture.local.inactive_entries << ','
                       << capture.local.unassigned_entries << ','
                       << metrics.local_event_memory_records << ','
                       << metrics.local_event_memory_overwrites << ','
                       << std::setprecision(9) << overwrite_share << ','
                       << metrics.processed_events << ',' << metrics.fire_events << ','
                       << metrics.transmitted_impulses << ','
                       << diagnostics.estimated_memory_bytes << ',' << seconds << ',' << rate
                       << ',' << hex64(runtime.state_fingerprint()) << '\n';
                write_snapshots(
                    snapshots, seed, capacity, control, runtime, circuit, capture);

                if (!enabled || capacity == 0 || circuit.relay_neurons.empty()) continue;
                const eve::NeuronId accumulator = circuit.relay_neurons.front();
                const eve::NeuronId completion =
                    runtime.motor_assignment().completion_contact;
                for (const auto& impulse : capture.impulses) {
                    if (!impulse.receiver_fired) continue;
                    if (impulse.receiver == accumulator || impulse.receiver == completion) {
                        write_retention_line(
                            retention, seed, capacity,
                            impulse.receiver == accumulator ? "accumulator" : "completion",
                            "at-fire-decision", runtime, circuit, impulse.receiver,
                            impulse.local_event_snapshot);
                    }
                }
                write_retention_line(
                    retention, seed, capacity, "accumulator", "after-completion",
                    runtime, circuit, accumulator,
                    runtime.network().local_event_memory(accumulator));
                write_retention_line(
                    retention, seed, capacity, "completion", "after-completion",
                    runtime, circuit, completion,
                    runtime.network().local_event_memory(completion));

                std::uint32_t extra = 0;
                const std::set<eve::SynapseId> circuit_ids(
                    circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
                for (const eve::SynapseId id : runtime.network().incoming_synapses(accumulator)) {
                    if (circuit_ids.find(id) != circuit_ids.end()) continue;
                    runtime.enqueue_fire(runtime.network().synapse(id).sender);
                    if (++extra == 3) break;
                }
                if (extra != 0) static_cast<void>(runtime.run());
                write_retention_line(
                    retention, seed, capacity, "accumulator", "after-extra-inputs",
                    runtime, circuit, accumulator,
                    runtime.network().local_event_memory(accumulator));
                write_retention_line(
                    retention, seed, capacity, "completion", "after-extra-inputs",
                    runtime, circuit, completion,
                    runtime.network().local_event_memory(completion));

                eve::brainstem::FeedbackConfig feedback;
                feedback.mode = eve::brainstem::FeedbackMode::after_completion;
                feedback.first_feedback_neuron = 9;
                feedback.event_budget = 100'000;
                static_cast<void>(runtime.process_motor_feedback(capture.response, feedback));
                write_retention_line(
                    retention, seed, capacity, "accumulator", "after-self-feedback",
                    runtime, circuit, accumulator,
                    runtime.network().local_event_memory(accumulator));
                write_retention_line(
                    retention, seed, capacity, "completion", "after-self-feedback",
                    runtime, circuit, completion,
                    runtime.network().local_event_memory(completion));
            }
        }
    }
}

void run_interruptions(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write B interruption results");
    output << "seed,variant,selected_synapse,sender,receiver,baseline_status,"
              "baseline_proven_path,baseline_local_path,interrupted_status,"
              "interrupted_proven_path,interrupted_local_path,baseline_events,"
              "interrupted_events,event_delta,completion_changed\n";
    for (const std::uint64_t seed : kSeeds) {
        eve::bios::Runtime baseline(experiment_config(8), seed);
        const auto baseline_circuit = configure_runtime(baseline, seed, true);
        const Capture baseline_capture = execute(baseline, baseline_circuit, seed, true);
        if (!baseline_capture.technical.proven_circuit_path ||
            baseline_circuit.relay_neurons.empty()) {
            continue;
        }
        const eve::NeuronId accumulator = baseline_circuit.relay_neurons.front();
        eve::SynapseId selected = eve::kInvalidIndex;
        for (const auto& impulse : baseline_capture.impulses) {
            if (!impulse.receiver_fired || impulse.receiver != accumulator) continue;
            for (const eve::SynapseId id : impulse.local_event_snapshot) {
                const auto& synapse = baseline.network().synapse(id);
                if (synapse.receiver == accumulator && synapse.weight > 0 &&
                    std::find(baseline_circuit.circuit_synapses.begin(),
                              baseline_circuit.circuit_synapses.end(), id) !=
                        baseline_circuit.circuit_synapses.end()) {
                    selected = id;
                    break;
                }
            }
            if (selected != eve::kInvalidIndex) break;
        }
        if (selected == eve::kInvalidIndex) continue;
        const auto selected_synapse = baseline.network().synapse(selected);

        eve::bios::Runtime interrupted(experiment_config(8), seed);
        const auto interrupted_circuit = configure_runtime(interrupted, seed, true);
        interrupted.set_experimental_synapse_weight(selected, 0);
        const Capture interrupted_capture = execute(
            interrupted, interrupted_circuit, seed, true);
        const std::int64_t event_delta =
            static_cast<std::int64_t>(interrupted.metrics().processed_events) -
            static_cast<std::int64_t>(baseline.metrics().processed_events);
        output << seed << ",G3," << selected << ',' << selected_synapse.sender << ','
               << selected_synapse.receiver << ','
               << status_name(baseline_capture.response.status) << ','
               << (baseline_capture.technical.proven_circuit_path ? 1 : 0) << ','
               << (baseline_capture.local.full_path_reconstructable ? 1 : 0) << ','
               << status_name(interrupted_capture.response.status) << ','
               << (interrupted_capture.technical.proven_circuit_path ? 1 : 0) << ','
               << (interrupted_capture.local.full_path_reconstructable ? 1 : 0) << ','
               << baseline.metrics().processed_events << ','
               << interrupted.metrics().processed_events << ',' << event_delta << ','
               << (baseline_capture.response.status != interrupted_capture.response.status
                       ? 1 : 0)
               << '\n';
    }
}

void run_repetitions(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write B repetition results");
    output << "state_mode,seed,variant,repetition,response_status,data_events,"
              "technical_proven_path,local_full_path,memory_records,memory_overwrites,"
              "refractory_suppressions,total_events,state_fingerprint\n";
    for (const std::uint64_t seed : kEstablishedSeeds) {
        for (const std::uint32_t capacity : {0U, 4U, 8U}) {
            eve::bios::Runtime continued(experiment_config(capacity), seed);
            const auto circuit = configure_runtime(continued, seed, true);
            const auto state_path = std::filesystem::temp_directory_path() /
                ("eve-b-" + std::to_string(seed) + "-" +
                 std::to_string(capacity) + ".bin");
            continued.save(state_path.string());
            for (const char* state_mode : {"restored", "continued"}) {
                for (std::uint32_t repetition = 1; repetition <= 8; ++repetition) {
                    eve::bios::Runtime restored = eve::bios::Runtime::load(state_path.string());
                    eve::bios::Runtime* runtime = &continued;
                    eve::brainstem::MotorCircuitResult active_circuit = circuit;
                    if (std::string(state_mode) == "restored") {
                        active_circuit = configure_runtime(restored, seed, true);
                        runtime = &restored;
                    }
                    const Capture capture = execute(
                        *runtime, active_circuit, seed, true);
                    const auto data_count = std::count_if(
                        capture.response.activations.begin(), capture.response.activations.end(),
                        [](const eve::brainstem::MotorActivation& activation) {
                            return activation.channel >= 0;
                        });
                    output << state_mode << ',' << seed << ','
                           << variant_name(capacity) << ',' << repetition << ','
                           << status_name(capture.response.status) << ',' << data_count << ','
                           << (capture.technical.proven_circuit_path ? 1 : 0) << ','
                           << (capture.local.full_path_reconstructable ? 1 : 0) << ','
                           << runtime->metrics().local_event_memory_records << ','
                           << runtime->metrics().local_event_memory_overwrites << ','
                           << runtime->metrics().refractory_suppressed_firings << ','
                           << runtime->metrics().processed_events << ','
                           << hex64(runtime->state_fingerprint()) << '\n';
                }
            }
            static_cast<void>(std::filesystem::remove(state_path));
        }
    }
}

void run_performance(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write B performance results");
    output << "seed,variant,capacity,diagnosis,id_entry_bytes,id_storage_bytes,"
              "management_storage_bytes,total_local_storage_bytes,"
              "extrapolated_1m_neurons_bytes,runtime_memory_bytes,diagnostic_memory_bytes,"
              "total_events,transmitted_impulses,seconds,synaptic_events_per_second\n";
    for (const std::uint64_t seed : kEstablishedSeeds) {
        for (const std::uint32_t capacity : kCapacities) {
            for (const bool diagnostic : {false, true}) {
                eve::bios::Runtime runtime(experiment_config(capacity), seed);
                const auto circuit = configure_runtime(runtime, seed, true);
                const auto start = std::chrono::steady_clock::now();
                const Capture capture = execute(runtime, circuit, seed, diagnostic);
                const double seconds = std::chrono::duration<double>(
                    std::chrono::steady_clock::now() - start).count();
                std::size_t nested_snapshot_bytes = 0;
                for (const auto& impulse : capture.impulses) {
                    nested_snapshot_bytes += impulse.local_event_snapshot.capacity() *
                                             sizeof(eve::SynapseId);
                }
                const std::size_t diagnostic_bytes =
                    capture.impulses.capacity() * sizeof(eve::bios::ImpulseObservation) +
                    capture.fires.capacity() * sizeof(eve::bios::FireObservation) +
                    nested_snapshot_bytes;
                const std::size_t id_storage = 256ULL * capacity * sizeof(eve::SynapseId);
                const std::size_t management = 256ULL *
                    (2ULL * sizeof(std::uint32_t) + sizeof(std::uint64_t));
                const std::size_t local_total = id_storage + management;
                const std::uint64_t extrapolated = 1'000'000ULL *
                    (capacity * sizeof(eve::SynapseId) +
                     2ULL * sizeof(std::uint32_t) + sizeof(std::uint64_t));
                const double rate = seconds == 0.0 ? 0.0
                    : static_cast<double>(runtime.metrics().transmitted_impulses) / seconds;
                output << seed << ',' << variant_name(capacity) << ',' << capacity << ','
                       << (diagnostic ? "on" : "off") << ',' << sizeof(eve::SynapseId) << ','
                       << id_storage << ',' << management << ',' << local_total << ','
                       << extrapolated << ',' << runtime.diagnostics().estimated_memory_bytes
                       << ',' << diagnostic_bytes << ','
                       << runtime.metrics().processed_events << ','
                       << runtime.metrics().transmitted_impulses << ','
                       << std::setprecision(9) << seconds << ',' << rate << '\n';
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
        write_parameters(output_directory / "research-stage-b-parameters.csv");
        run_primary(
            output_directory / "research-stage-b-primary.csv",
            output_directory / "research-stage-b-fire-snapshots.csv",
            output_directory / "research-stage-b-retention.csv");
        run_interruptions(output_directory / "research-stage-b-interruptions.csv");
        run_repetitions(output_directory / "research-stage-b-repetitions.csv");
        run_performance(output_directory / "research-stage-b-performance.csv");
        std::cout << "wrote B research data to " << output_directory << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage B failed: " << exception.what() << '\n';
        return 1;
    }
}
