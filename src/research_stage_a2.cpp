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

constexpr std::array<std::uint64_t, 32> kSeeds{{
    20261117, 7, 42, 99, 2026, 314159, 1, 2, 3, 5, 11, 13, 17, 19, 23, 29,
    31, 37, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
}};
constexpr std::array<std::uint64_t, 6> kEstablishedSeeds{{
    20261117, 7, 42, 99, 2026, 314159,
}};
constexpr std::uint64_t kMotorSeed = 0x41322d4d4f544f52ULL;
constexpr std::uint64_t kTopologySalt = 0x41322d544f504f4cULL;
constexpr std::uint64_t kExplorationSalt = 0x41322d4558504c4fULL;
constexpr std::uint64_t kCircuitSalt = 0x41322d4349524355ULL;

struct Capture {
    eve::brainstem::ExplorationResult exploration;
    eve::brainstem::MotorResponse response;
    eve::bios::MotorCausality causality;
    std::vector<eve::bios::FireObservation> fires;
    std::vector<eve::bios::ImpulseObservation> impulses;
};

const char* mode_name(const MotorCircuitMode mode) {
    switch (mode) {
    case MotorCircuitMode::none: return "M0";
    case MotorCircuitMode::direct_completion_path: return "M1";
    case MotorCircuitMode::completion_group: return "M2";
    case MotorCircuitMode::activity_dependent: return "M3";
    }
    return "unknown";
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

std::string hex_byte(const std::uint8_t value) {
    std::ostringstream output;
    output << "0x" << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<unsigned>(value);
    return output.str();
}

std::string hex64(const std::uint64_t value) {
    std::ostringstream output;
    output << std::hex << std::setw(16) << std::setfill('0') << value;
    return output.str();
}

std::string activation_order(const eve::brainstem::MotorResponse& response) {
    std::ostringstream output;
    for (std::size_t index = 0; index < response.activations.size(); ++index) {
        if (index != 0) output << '|';
        const auto channel = response.activations[index].channel;
        if (channel < 0) output << 'C';
        else output << static_cast<unsigned>(channel);
    }
    return output.str();
}

std::uint32_t data_events(const eve::brainstem::MotorResponse& response) {
    return static_cast<std::uint32_t>(std::count_if(
        response.activations.begin(), response.activations.end(),
        [](const eve::brainstem::MotorActivation& item) { return item.channel >= 0; }));
}

std::uint32_t active_channels(const eve::brainstem::MotorResponse& response) {
    return static_cast<std::uint32_t>(
        std::count(response.activated_channels.begin(),
                   response.activated_channels.end(), true));
}

eve::ExperimentConfig experiment_config() {
    eve::ExperimentConfig config;
    config.neuron_count = 256;
    config.width = 8;
    config.height = 8;
    config.depth = 4;
    config.initial_outgoing_synapses = 4;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.refractory_input_events = 16;
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
    const MotorCircuitMode mode,
    const std::uint64_t seed,
    const bool enabled) {
    eve::brainstem::MotorCircuitConfig config;
    config.mode = mode;
    config.motor_weight = mode == MotorCircuitMode::activity_dependent ? 1 : 3;
    config.completion_weight = 3;
    config.relay_count = 4;
    config.seed = seed ^ kCircuitSalt;
    config.path_enabled = enabled;
    return config;
}

eve::brainstem::ExplorationConfig exploration_config(
    const ExplorationMode mode,
    const ExplorationTargetPolicy policy,
    const std::uint32_t count,
    const std::uint64_t seed,
    const std::vector<eve::NeuronId>& common_reservations = {}) {
    eve::brainstem::ExplorationConfig config;
    config.mode = mode;
    config.target_policy = policy;
    config.activation_count = count;
    config.activation_budget = std::max(1U, count);
    config.seed = seed ^ kExplorationSalt;
    config.center = {4, 4, 2};
    config.radius = 2;
    config.additional_reserved_neurons = common_reservations;
    return config;
}

eve::brainstem::MotorCircuitResult configure_runtime(
    eve::bios::Runtime& runtime,
    const std::uint64_t seed,
    const MotorCircuitMode mode,
    const bool path_enabled) {
    runtime.configure_motor_output(motor_config());
    static_cast<void>(runtime.configure_innate_wiring(wiring_config(seed)));
    return runtime.configure_motor_circuit(circuit_config(mode, seed, path_enabled));
}

std::vector<eve::NeuronId> common_circuit_reservations(const std::uint64_t seed) {
    std::vector<eve::NeuronId> result;
    for (const MotorCircuitMode mode : {
             MotorCircuitMode::completion_group,
             MotorCircuitMode::activity_dependent,
         }) {
        eve::bios::Runtime runtime(experiment_config(), seed);
        runtime.configure_motor_output(motor_config());
        const auto circuit = runtime.configure_motor_circuit(
            circuit_config(mode, seed, true));
        result.insert(result.end(), circuit.relay_neurons.begin(),
                      circuit.relay_neurons.end());
    }
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

Capture execute_response(
    eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const eve::brainstem::ExplorationConfig& exploration,
    const std::string& input,
    const bool detailed_diagnosis) {
    Capture capture;
    if (detailed_diagnosis) {
        runtime.set_fire_observer(
            [&capture](const eve::bios::FireObservation& observation) {
                capture.fires.push_back(observation);
            });
        runtime.set_impulse_observer(
            [&capture](const eve::bios::ImpulseObservation& observation) {
                capture.impulses.push_back(observation);
            });
    }
    static_cast<void>(runtime.process_utf8_text(input));
    capture.exploration = runtime.enqueue_exploration(exploration);
    capture.response = runtime.request_response(true, 100'000);
    capture.causality = eve::bios::Runtime::analyze_motor_causality(
        capture.response, circuit, capture.impulses);
    if (capture.response.valid() && runtime.diagnostics().queued_events != 0) {
        static_cast<void>(runtime.run());
    }
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    return capture;
}

struct DirectSelection {
    std::uint32_t data{};
    std::uint32_t completion{};
    std::uint32_t relay{};
};

DirectSelection direct_selection(
    const eve::brainstem::ExplorationResult& exploration,
    const eve::brainstem::MotorAssignment& assignment,
    const eve::brainstem::MotorCircuitResult& circuit) {
    DirectSelection result;
    for (const eve::NeuronId selected : exploration.activated_neurons) {
        if (selected == assignment.completion_contact) ++result.completion;
        if (std::find(circuit.relay_neurons.begin(), circuit.relay_neurons.end(),
                      selected) != circuit.relay_neurons.end()) {
            ++result.relay;
        }
        for (const auto& channel : assignment.channels) {
            if (std::find(channel.begin(), channel.end(), selected) != channel.end()) {
                ++result.data;
            }
        }
    }
    return result;
}

void write_common_result(
    std::ostream& output,
    const std::string& prefix,
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const Capture& capture,
    const double seconds) {
    const DirectSelection direct = direct_selection(
        capture.exploration, runtime.motor_assignment(), circuit);
    if (capture.exploration.config.target_policy ==
            ExplorationTargetPolicy::processing_neurons_only &&
        (direct.data != 0 || direct.completion != 0 || direct.relay != 0)) {
        throw std::logic_error(
            "restricted exploration directly selected a reserved motor neuron");
    }
    const auto diagnostics = runtime.diagnostics();
    const auto& metrics = runtime.metrics();
    const double synaptic_rate = seconds == 0.0
        ? 0.0 : static_cast<double>(metrics.transmitted_impulses) / seconds;
    output << prefix << ',' << capture.exploration.eligible_neuron_count << ','
           << capture.exploration.reserved_neuron_count << ','
           << capture.exploration.activated_neurons.size() << ',' << direct.data << ','
           << direct.completion << ',' << direct.relay << ','
           << status_name(capture.response.status) << ','
           << hex_byte(capture.response.value) << ',' << data_events(capture.response) << ','
           << active_channels(capture.response) << ','
           << csv(activation_order(capture.response)) << ','
           << (capture.causality.completed_without_prior_data ? 1 : 0) << ','
           << (capture.causality.completed_after_data ? 1 : 0) << ','
           << (capture.causality.proven_circuit_path ? 1 : 0) << ','
           << capture.causality.first_data_event << ','
           << capture.causality.completion_event << ','
           << capture.causality.events_between_data_and_completion << ','
           << capture.causality.observed_circuit_impulses << ','
           << capture.causality.firing_circuit_impulses << ','
           << capture.causality.suppressed_circuit_impulses << ','
           << capture.response.processed_events << ',' << metrics.processed_events << ','
           << metrics.fire_events << ',' << metrics.transmitted_impulses << ','
           << metrics.maximum_queue_size << ',' << diagnostics.synapse_count << ','
           << diagnostics.estimated_memory_bytes << ',' << hex64(runtime.state_fingerprint())
           << ',' << std::setprecision(9) << seconds << ',' << synaptic_rate;
}

const char* common_columns() {
    return "eligible_neurons,reserved_neurons,exploration_events,"
           "direct_exploration_data,direct_exploration_completion,"
           "direct_exploration_relay,response_status,response_value,data_motor_events,"
           "active_channels,activation_order,completion_without_data,"
           "completion_after_data,proven_circuit_path,first_data_event,"
           "completion_event,events_between,circuit_impulses,firing_circuit_impulses,"
           "suppressed_circuit_impulses,"
           "response_events,total_events,total_fires,transmitted_impulses,maximum_queue,"
           "synapse_count,memory_bytes,state_fingerprint,seconds,synaptic_events_per_second";
}

void write_parameters(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write A.2 parameters");
    output << "parameter,value\n"
           << "neuron_count,256\nregular_initial_synapses,1024\ngeometry,8x8x4\n"
           << "regular_initial_weight,0\ninnate_topology,V2\ninnate_weight,3\n"
           << "firing_threshold,3\nrefractory_events,16\nlearning_enabled,0\n"
           << "growth_enabled,0\nevent_budget,100000\nmotor_contacts_per_channel,1\n"
           << "response_neuron,8\nfeedback_neurons,9-17\nfirst_motor_neuron,18\n"
           << "primary_exploration,E4-new-16\nrepetitions,8\nprimary_seed_count,32\n"
           << "established_seeds,20261117|7|42|99|2026|314159\n";
}

void write_diagnostic_example(
    std::ostream& output,
    const std::uint64_t seed,
    const MotorCircuitMode mode,
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const Capture& capture) {
    const std::set<eve::SynapseId> circuit_ids(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    const std::set<eve::NeuronId> relay_ids(
        circuit.relay_neurons.begin(), circuit.relay_neurons.end());
    struct Line {
        std::uint64_t event_index{};
        std::string text;
    };
    std::vector<Line> lines;
    for (const auto& fire : capture.fires) {
        std::string role = "processing";
        if (fire.neuron == runtime.motor_assignment().completion_contact) role = "completion";
        for (std::size_t channel = 0;
             channel < runtime.motor_assignment().channels.size(); ++channel) {
            const auto& contacts = runtime.motor_assignment().channels[channel];
            if (std::find(contacts.begin(), contacts.end(), fire.neuron) != contacts.end()) {
                role = "data-" + std::to_string(channel);
            }
        }
        if (relay_ids.find(fire.neuron) != relay_ids.end()) role = "circuit-relay";
        if (role == "processing") continue;
        std::ostringstream line;
        line << seed << ',' << mode_name(mode) << ',' << fire.event_index
             << ",fire,-1," << fire.neuron << ",-1,0,0,0,0,0,1," << role;
        lines.push_back(Line{fire.event_index, line.str()});
    }
    for (const auto& impulse : capture.impulses) {
        if (circuit_ids.find(impulse.synapse) == circuit_ids.end()) continue;
        std::ostringstream line;
        line << seed << ',' << mode_name(mode) << ',' << impulse.event_index
             << ",impulse," << impulse.sender << ',' << impulse.receiver << ','
             << impulse.synapse << ',' << impulse.applied_weight << ','
             << impulse.membrane_before << ',' << impulse.membrane_after << ','
             << impulse.refractory_before << ',' << impulse.refractory_after << ','
             << (impulse.receiver_fired ? 1 : 0) << ",circuit-synapse";
        lines.push_back(Line{impulse.event_index, line.str()});
    }
    std::stable_sort(lines.begin(), lines.end(),
                     [](const Line& left, const Line& right) {
                         return left.event_index < right.event_index;
                     });
    for (const Line& line : lines) {
        output << line.text << '\n';
    }
}

void run_exploration_study(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write exploration results");
    output << "variant,seed,target_policy,requested," << common_columns() << '\n';
    struct Variant {
        const char* name;
        ExplorationMode mode;
        ExplorationTargetPolicy policy;
        std::uint32_t count;
    };
    const std::array<Variant, 5> variants{{
        {"E0", ExplorationMode::none, ExplorationTargetPolicy::processing_neurons_only, 0},
        {"E4-old-16", ExplorationMode::distributed, ExplorationTargetPolicy::all_neurons, 16},
        {"E4-new-8", ExplorationMode::distributed,
         ExplorationTargetPolicy::processing_neurons_only, 8},
        {"E4-new-16", ExplorationMode::distributed,
         ExplorationTargetPolicy::processing_neurons_only, 16},
        {"E4-new-32", ExplorationMode::distributed,
         ExplorationTargetPolicy::processing_neurons_only, 32},
    }};
    for (const Variant& variant : variants) {
        for (const std::uint64_t seed : kEstablishedSeeds) {
            eve::bios::Runtime runtime(experiment_config(), seed);
            const auto circuit = configure_runtime(
                runtime, seed, MotorCircuitMode::none, true);
            const auto common = common_circuit_reservations(seed);
            const auto config = exploration_config(
                variant.mode, variant.policy, variant.count, seed, common);
            const auto start = std::chrono::steady_clock::now();
            const Capture capture = execute_response(runtime, circuit, config, "AB", true);
            const double seconds = std::chrono::duration<double>(
                std::chrono::steady_clock::now() - start).count();
            const std::string prefix = std::string(variant.name) + ',' +
                std::to_string(seed) + ',' +
                (variant.policy == ExplorationTargetPolicy::all_neurons ? "all" : "processing") +
                ',' + std::to_string(variant.count);
            write_common_result(output, prefix, runtime, circuit, capture, seconds);
            output << '\n';
        }
    }
}

void run_causality_study(
    const std::filesystem::path& result_path,
    const std::filesystem::path& diagnostic_path) {
    std::ofstream output(result_path);
    std::ofstream diagnostic(diagnostic_path);
    if (!output || !diagnostic) throw std::runtime_error("cannot write causality results");
    output << "seed,motor_circuit,control,path_enabled," << common_columns() << '\n';
    diagnostic << "seed,motor_circuit,event_index,event_kind,sender,receiver,synapse,"
                  "weight,membrane_before,membrane_after,refractory_before,"
                  "refractory_after,receiver_fired,role\n";
    std::set<MotorCircuitMode> documented;
    for (const std::uint64_t seed : kSeeds) {
        const auto common = common_circuit_reservations(seed);
        for (const MotorCircuitMode mode : {
                 MotorCircuitMode::none,
                 MotorCircuitMode::direct_completion_path,
                 MotorCircuitMode::completion_group,
                 MotorCircuitMode::activity_dependent,
             }) {
            const std::array<const char*, 2> controls{{"K1", "K2"}};
            const std::size_t control_count = mode == MotorCircuitMode::none ? 1U : 2U;
            for (std::size_t control_index = 0; control_index < control_count; ++control_index) {
                const char* control = mode == MotorCircuitMode::none
                    ? "K0" : controls[control_index];
                const bool enabled = std::string(control) != "K2";
                eve::bios::Runtime runtime(experiment_config(), seed);
                const auto circuit = configure_runtime(runtime, seed, mode, enabled);
                const auto exploration = exploration_config(
                    ExplorationMode::distributed,
                    ExplorationTargetPolicy::processing_neurons_only,
                    16, seed, common);
                const auto start = std::chrono::steady_clock::now();
                const Capture capture = execute_response(
                    runtime, circuit, exploration, "AB", true);
                const double seconds = std::chrono::duration<double>(
                    std::chrono::steady_clock::now() - start).count();
                std::ostringstream prefix;
                prefix << seed << ',' << mode_name(mode) << ',' << control << ','
                       << (enabled ? 1 : 0);
                write_common_result(output, prefix.str(), runtime, circuit, capture, seconds);
                output << '\n';
                if (capture.causality.proven_circuit_path &&
                    documented.insert(mode).second) {
                    write_diagnostic_example(
                        diagnostic, seed, mode, runtime, circuit, capture);
                }
            }
        }
    }
}

void run_repetition_study(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write repetition results");
    output << "state_mode,input,seed,motor_circuit,repetition," << common_columns() << '\n';
    for (const char* state_mode : {"restored", "continued"}) {
        for (const std::string input : {"AB", "BA"}) {
            for (const std::uint64_t seed : kEstablishedSeeds) {
                const auto common = common_circuit_reservations(seed);
                for (const MotorCircuitMode mode : {
                         MotorCircuitMode::none,
                         MotorCircuitMode::direct_completion_path,
                         MotorCircuitMode::completion_group,
                         MotorCircuitMode::activity_dependent,
                     }) {
                    eve::bios::Runtime continued(experiment_config(), seed);
                    static_cast<void>(configure_runtime(continued, seed, mode, true));
                    const auto state_path = std::filesystem::temp_directory_path() /
                        ("eve-a2-" + std::to_string(seed) + "-" + mode_name(mode) + ".bin");
                    continued.save(state_path.string());
                    for (std::uint32_t repetition = 1; repetition <= 8; ++repetition) {
                        eve::bios::Runtime restored = state_mode == std::string("restored")
                            ? eve::bios::Runtime::load(state_path.string())
                            : eve::bios::Runtime(experiment_config(), seed);
                        eve::bios::Runtime* runtime = &continued;
                        eve::brainstem::MotorCircuitResult circuit;
                        if (state_mode == std::string("restored")) {
                            circuit = configure_runtime(restored, seed, mode, true);
                            runtime = &restored;
                        } else {
                            circuit.config = circuit_config(mode, seed, true);
                            circuit = runtime->configure_motor_circuit(circuit.config);
                        }
                        const auto exploration = exploration_config(
                            ExplorationMode::distributed,
                            ExplorationTargetPolicy::processing_neurons_only,
                            16, seed, common);
                        const auto start = std::chrono::steady_clock::now();
                        const Capture capture = execute_response(
                            *runtime, circuit, exploration, input, true);
                        const double seconds = std::chrono::duration<double>(
                            std::chrono::steady_clock::now() - start).count();
                        std::ostringstream prefix;
                        prefix << state_mode << ',' << input << ',' << seed << ','
                               << mode_name(mode) << ',' << repetition;
                        write_common_result(
                            output, prefix.str(), *runtime, circuit, capture, seconds);
                        output << '\n';
                    }
                    static_cast<void>(std::filesystem::remove(state_path));
                }
            }
        }
    }
}

void run_feedback_study(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write feedback results");
    output << "seed,motor_circuit,feedback,first_status,first_value,first_order,"
              "first_proven_path,feedback_sensor_events,feedback_processed_events,"
              "fires_added_by_feedback,second_status,second_value,second_order,"
              "second_proven_path,total_events,total_fires,state_fingerprint\n";
    for (const std::uint64_t seed : kEstablishedSeeds) {
        const auto common = common_circuit_reservations(seed);
        for (const MotorCircuitMode mode : {
                 MotorCircuitMode::none,
                 MotorCircuitMode::direct_completion_path,
                 MotorCircuitMode::completion_group,
                 MotorCircuitMode::activity_dependent,
             }) {
            for (const bool feedback_enabled : {false, true}) {
                eve::bios::Runtime runtime(experiment_config(), seed);
                const auto circuit = configure_runtime(runtime, seed, mode, true);
                const auto first = execute_response(
                    runtime, circuit,
                    exploration_config(ExplorationMode::distributed,
                        ExplorationTargetPolicy::processing_neurons_only,
                        16, seed, common),
                    "AB", true);
                const std::uint64_t fires_before = runtime.metrics().fire_events;
                eve::brainstem::FeedbackConfig feedback;
                feedback.mode = feedback_enabled
                    ? eve::brainstem::FeedbackMode::after_completion
                    : eve::brainstem::FeedbackMode::none;
                feedback.first_feedback_neuron = 9;
                feedback.event_budget = 100'000;
                const auto feedback_result = runtime.process_motor_feedback(
                    first.response, feedback);
                const std::uint64_t fires_added =
                    runtime.metrics().fire_events - fires_before;
                const auto second = execute_response(
                    runtime, circuit,
                    exploration_config(ExplorationMode::distributed,
                        ExplorationTargetPolicy::processing_neurons_only,
                        16, seed + 1U, common),
                    "AB", true);
                output << seed << ',' << mode_name(mode) << ','
                       << (feedback_enabled ? "R1" : "R0") << ','
                       << status_name(first.response.status) << ','
                       << hex_byte(first.response.value) << ','
                       << csv(activation_order(first.response)) << ','
                       << (first.causality.proven_circuit_path ? 1 : 0) << ','
                       << feedback_result.sensory_events.size() << ','
                       << feedback_result.processed_events << ',' << fires_added << ','
                       << status_name(second.response.status) << ','
                       << hex_byte(second.response.value) << ','
                       << csv(activation_order(second.response)) << ','
                       << (second.causality.proven_circuit_path ? 1 : 0) << ','
                       << runtime.metrics().processed_events << ','
                       << runtime.metrics().fire_events << ','
                       << hex64(runtime.state_fingerprint()) << '\n';
            }
        }
    }
}

void run_performance_study(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write performance results");
    output << "seed,diagnosis,response_status,proven_circuit_path,total_events,"
              "transmitted_impulses,maximum_queue,runtime_memory_bytes,"
              "diagnostic_memory_bytes,diagnostic_impulses,seconds,"
              "synaptic_events_per_second,state_fingerprint\n";
    for (const std::uint64_t seed : kSeeds) {
        const auto common = common_circuit_reservations(seed);
        for (const bool detailed : {false, true}) {
            eve::bios::Runtime runtime(experiment_config(), seed);
            const auto circuit = configure_runtime(
                runtime, seed, MotorCircuitMode::completion_group, true);
            const auto start = std::chrono::steady_clock::now();
            const Capture capture = execute_response(
                runtime, circuit,
                exploration_config(ExplorationMode::distributed,
                    ExplorationTargetPolicy::processing_neurons_only,
                    16, seed, common),
                "AB", detailed);
            const double seconds = std::chrono::duration<double>(
                std::chrono::steady_clock::now() - start).count();
            const std::size_t diagnostic_memory =
                capture.impulses.capacity() * sizeof(eve::bios::ImpulseObservation) +
                capture.fires.capacity() * sizeof(eve::bios::FireObservation);
            const auto diagnostics = runtime.diagnostics();
            const double rate = seconds == 0.0 ? 0.0
                : static_cast<double>(runtime.metrics().transmitted_impulses) / seconds;
            output << seed << ',' << (detailed ? "detailed" : "off") << ','
                   << status_name(capture.response.status) << ','
                   << (capture.causality.proven_circuit_path ? 1 : 0) << ','
                   << runtime.metrics().processed_events << ','
                   << runtime.metrics().transmitted_impulses << ','
                   << runtime.metrics().maximum_queue_size << ','
                   << diagnostics.estimated_memory_bytes << ',' << diagnostic_memory << ','
                   << capture.impulses.size() << ',' << std::setprecision(9) << seconds << ','
                   << rate << ',' << hex64(runtime.state_fingerprint()) << '\n';
        }
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output_directory =
            argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(output_directory);
        write_parameters(output_directory / "research-stage-a2-parameters.csv");
        run_exploration_study(output_directory / "research-stage-a2-exploration.csv");
        run_causality_study(
            output_directory / "research-stage-a2-causality.csv",
            output_directory / "research-stage-a2-diagnostics.csv");
        run_repetition_study(output_directory / "research-stage-a2-repetitions.csv");
        run_feedback_study(output_directory / "research-stage-a2-feedback.csv");
        run_performance_study(output_directory / "research-stage-a2-performance.csv");
        std::cout << "wrote A.2 research data to " << output_directory << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage A.2 failed: " << exception.what() << '\n';
        return 1;
    }
}
