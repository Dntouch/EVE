#include "eve/bios/runtime.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using eve::brainstem::ExplorationMode;
using eve::brainstem::FeedbackMode;
using eve::brainstem::InnateTopology;
using eve::brainstem::MotorCircuitMode;

constexpr std::array<std::uint64_t, 6> kSeeds{{20261117, 7, 42, 99, 2026, 314159}};
constexpr std::uint64_t kMotorSeed = 0x41312d4d4f544f52ULL;
constexpr std::uint64_t kTopologySalt = 0x41312d544f504f4cULL;
constexpr std::uint64_t kExplorationSalt = 0x41312d4558504c4fULL;
constexpr std::uint64_t kCircuitSalt = 0x41312d4349524355ULL;

struct Case {
    const char* phase;
    const char* name;
    InnateTopology topology;
    std::int32_t innate_weight;
    ExplorationMode exploration;
    std::uint32_t exploration_count;
    MotorCircuitMode circuit;
    FeedbackMode feedback;
    bool learning;
};

std::string csv(const std::string& value) {
    if (value.find_first_of(",\"\n\r") == std::string::npos) {
        return value;
    }
    std::string result = "\"";
    for (const char character : value) {
        result += character == '\"' ? "\"\"" : std::string(1, character);
    }
    return result + '\"';
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

const char* topology_name(const InnateTopology value) {
    switch (value) {
    case InnateTopology::none: return "V0";
    case InnateTopology::structural_paths: return "V1";
    case InnateTopology::functional_paths: return "V2";
    case InnateTopology::varied_functional_paths: return "V3";
    }
    return "unknown";
}

const char* exploration_name(const ExplorationMode value) {
    switch (value) {
    case ExplorationMode::none: return "E0";
    case ExplorationMode::single: return "E1";
    case ExplorationMode::multiple: return "E2";
    case ExplorationMode::localized: return "E3";
    case ExplorationMode::distributed: return "E4";
    }
    return "unknown";
}

const char* circuit_name(const MotorCircuitMode value) {
    switch (value) {
    case MotorCircuitMode::none: return "M0";
    case MotorCircuitMode::direct_completion_path: return "M1";
    case MotorCircuitMode::completion_group: return "M2";
    case MotorCircuitMode::activity_dependent: return "M3";
    }
    return "unknown";
}

const char* feedback_name(const FeedbackMode value) {
    switch (value) {
    case FeedbackMode::none: return "R0";
    case FeedbackMode::after_completion: return "R1";
    case FeedbackMode::per_motor_event: return "R2";
    case FeedbackMode::virtual_environment: return "R3";
    }
    return "unknown";
}

const char* status_name(const eve::brainstem::MotorResponseStatus value) {
    switch (value) {
    case eve::brainstem::MotorResponseStatus::no_response: return "no_response";
    case eve::brainstem::MotorResponseStatus::completed: return "completed";
    case eve::brainstem::MotorResponseStatus::aborted: return "aborted";
    }
    return "unknown";
}

eve::ExperimentConfig experiment_config(const bool learning) {
    eve::ExperimentConfig config;
    config.neuron_count = 256;
    config.width = 8;
    config.height = 8;
    config.depth = 4;
    config.initial_outgoing_synapses = 4;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = learning ? 1 : 3;
    config.refractory_input_events = 16;
    config.maximum_events_per_run = 100'000;
    config.learning_enabled = learning;
    config.growth_enabled = learning;
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

eve::brainstem::InnateWiringConfig wiring_config(const Case& item, const std::uint64_t seed) {
    eve::brainstem::InnateWiringConfig config;
    config.topology = item.topology;
    config.initial_weight = item.innate_weight;
    config.topology_seed = seed ^ kTopologySalt;
    return config;
}

eve::brainstem::ExplorationConfig exploration_config(
    const Case& item, const std::uint64_t seed) {
    eve::brainstem::ExplorationConfig config;
    config.mode = item.exploration;
    config.activation_count = item.exploration_count;
    config.activation_budget = item.exploration_count;
    config.seed = seed ^ kExplorationSalt;
    config.center = {4, 4, 2};
    config.radius = 2;
    return config;
}

eve::brainstem::MotorCircuitConfig circuit_config(
    const Case& item, const std::uint64_t seed) {
    eve::brainstem::MotorCircuitConfig config;
    config.mode = item.circuit;
    config.motor_weight = item.learning ? 1 : 3;
    config.completion_weight = item.learning ? 1 : 3;
    config.relay_count = 4;
    config.seed = seed ^ kCircuitSalt;
    return config;
}

eve::brainstem::FeedbackConfig feedback_config(const Case& item) {
    eve::brainstem::FeedbackConfig config;
    config.mode = item.feedback;
    config.first_feedback_neuron = 9;
    config.event_budget = 100'000;
    config.environment_state = 0x5a;
    return config;
}

std::uint32_t active_channels(const eve::brainstem::MotorResponse& response) {
    std::uint32_t count = 0;
    for (const bool active : response.activated_channels) {
        count += active ? 1U : 0U;
    }
    return count;
}

std::uint32_t data_activations(const eve::brainstem::MotorResponse& response) {
    std::uint32_t count = 0;
    for (const auto& activation : response.activations) {
        count += activation.channel >= 0 ? 1U : 0U;
    }
    return count;
}

std::string activation_order(const eve::brainstem::MotorResponse& response) {
    std::ostringstream output;
    for (std::size_t index = 0; index < response.activations.size(); ++index) {
        if (index != 0) output << '|';
        if (response.activations[index].channel < 0) output << 'C';
        else output << static_cast<unsigned>(response.activations[index].channel);
    }
    return output.str();
}

void write_result_header(std::ostream& output) {
    output << "phase,configuration,seed,learning,topology,innate_weight,exploration,"
              "exploration_requested,circuit,feedback,regular_synapses,innate_synapses,"
              "circuit_synapses,total_synapses,reachable_contacts,maximum_path,"
              "exploration_fires,response_status,response_value,data_motor_events,"
              "active_channels,activation_order,response_events,feedback_events,"
              "feedback_sensor_events,feedback_aborted,environment_before,environment_after,"
              "total_events,total_fires,transmitted_impulses,strengthenings,grown_synapses,"
              "maximum_queue,memory_bytes,state_fingerprint,seconds,events_per_second,"
              "synaptic_events_per_second,error\n";
}

void run_case(std::ostream& output, const Case& item, const std::uint64_t seed) {
    eve::bios::Runtime runtime(experiment_config(item.learning), seed);
    eve::brainstem::InnateWiringResult wiring;
    eve::brainstem::MotorCircuitResult circuit;
    eve::brainstem::ExplorationResult exploration;
    eve::brainstem::MotorResponse response;
    eve::brainstem::FeedbackResult feedback;
    eve::brainstem::MotorReachability reachability;
    std::string error;
    const auto start = std::chrono::steady_clock::now();
    try {
        runtime.configure_motor_output(motor_config());
        wiring = runtime.configure_innate_wiring(wiring_config(item, seed));
        reachability = runtime.motor_reachability();
        circuit = runtime.configure_motor_circuit(circuit_config(item, seed));
        static_cast<void>(runtime.process_utf8_text("AB"));
        exploration = runtime.enqueue_exploration(exploration_config(item, seed));
        response = runtime.request_response(true, 100'000);
        if (response.valid() && runtime.diagnostics().queued_events != 0) {
            static_cast<void>(runtime.run());
        }
        feedback = runtime.process_motor_feedback(response, feedback_config(item));
    } catch (const std::exception& exception) {
        error = exception.what();
        response.status = eve::brainstem::MotorResponseStatus::aborted;
    }
    const double seconds = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - start).count();
    const auto diagnostics = runtime.diagnostics();
    const auto& metrics = runtime.metrics();
    const double throughput = seconds == 0.0 ? 0.0 : metrics.processed_events / seconds;
    const double synaptic_throughput =
        seconds == 0.0 ? 0.0 : metrics.transmitted_impulses / seconds;
    output << item.phase << ',' << item.name << ',' << seed << ',' << (item.learning ? 1 : 0)
           << ',' << topology_name(item.topology) << ',' << item.innate_weight << ','
           << exploration_name(item.exploration) << ',' << item.exploration_count << ','
           << circuit_name(item.circuit) << ',' << feedback_name(item.feedback) << ','
           << 1024 << ',' << wiring.added_synapses << ',' << circuit.added_synapses << ','
           << diagnostics.synapse_count << ',' << reachability.reachable_contacts << ','
           << reachability.maximum_shortest_path << ',' << exploration.activated_neurons.size()
           << ',' << status_name(response.status) << ',' << hex_byte(response.value) << ','
           << data_activations(response) << ',' << active_channels(response) << ','
           << csv(activation_order(response)) << ',' << response.processed_events << ','
           << feedback.processed_events << ',' << feedback.sensory_events.size() << ','
           << (feedback.aborted ? 1 : 0) << ',' << hex_byte(feedback.environment_before) << ','
           << hex_byte(feedback.environment_after) << ',' << metrics.processed_events << ','
           << metrics.fire_events << ',' << metrics.transmitted_impulses << ','
           << metrics.synaptic_strengthenings << ',' << metrics.grown_synapses << ','
           << metrics.maximum_queue_size << ',' << diagnostics.estimated_memory_bytes << ','
           << hex64(runtime.state_fingerprint()) << ',' << std::setprecision(9) << seconds << ','
           << throughput << ',' << synaptic_throughput << ',' << csv(error) << '\n';
}

void write_parameters(const std::filesystem::path& path, const std::vector<Case>& cases) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write parameter CSV: " + path.string());
    output << "phase,configuration,topology,innate_weight,exploration,exploration_count,"
              "circuit,feedback,learning,neuron_count,regular_synapses,geometry,"
              "initial_regular_weight,maximum_weight,refractory_events,event_budget,"
              "motor_contacts_per_channel,first_feedback_neuron,first_motor_neuron,input\n";
    for (const Case& item : cases) {
        output << item.phase << ',' << item.name << ',' << topology_name(item.topology) << ','
               << item.innate_weight << ',' << exploration_name(item.exploration) << ','
               << item.exploration_count << ',' << circuit_name(item.circuit) << ','
               << feedback_name(item.feedback) << ',' << (item.learning ? 1 : 0)
               << ",256,1024,8x8x4,0," << (item.learning ? 1 : 3)
               << ",16,100000,1,9,18,AB\n";
    }
}

void write_paths(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) throw std::runtime_error("cannot write path CSV: " + path.string());
    output << "variant,seed,source,first_relay,second_relay,motor_target,weight\n";
    const std::array<Case, 4> variants{{
        {"I", "V1", InnateTopology::structural_paths, 0, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V2-weight-1", InnateTopology::functional_paths, 1, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V2-weight-3", InnateTopology::functional_paths, 3, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V3-weight-3", InnateTopology::varied_functional_paths, 3,
         ExplorationMode::none, 0, MotorCircuitMode::none, FeedbackMode::none, false},
    }};
    for (const Case& item : variants) {
        for (const std::uint64_t seed : kSeeds) {
            eve::bios::Runtime runtime(experiment_config(false), seed);
            runtime.configure_motor_output(motor_config());
            const auto result = runtime.configure_innate_wiring(wiring_config(item, seed));
            for (const auto& path_item : result.paths) {
                output << item.name << ',' << seed << ',' << path_item.source << ','
                       << path_item.first_relay << ',' << path_item.second_relay << ','
                       << path_item.motor_target << ',' << item.innate_weight << '\n';
            }
        }
    }
}

std::vector<Case> cases() {
    return {
        {"I", "V0", InnateTopology::none, 0, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V1", InnateTopology::structural_paths, 0, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V2-weight-1", InnateTopology::functional_paths, 1, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V2-weight-3", InnateTopology::functional_paths, 3, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"I", "V3-weight-3", InnateTopology::varied_functional_paths, 3,
         ExplorationMode::none, 0, MotorCircuitMode::none, FeedbackMode::none, false},
        {"II", "E0", InnateTopology::functional_paths, 3, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"II", "E1", InnateTopology::functional_paths, 3, ExplorationMode::single, 1,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"II", "E2", InnateTopology::functional_paths, 3, ExplorationMode::multiple, 8,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"II", "E3", InnateTopology::functional_paths, 3, ExplorationMode::localized, 8,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"II", "E4", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 8,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"III", "M0", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"III", "M1", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::direct_completion_path, FeedbackMode::none, false},
        {"III", "M2", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::completion_group, FeedbackMode::none, false},
        {"III", "M3", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::activity_dependent, FeedbackMode::none, false},
        {"IV", "R0", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::completion_group, FeedbackMode::none, false},
        {"IV", "R1", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::completion_group, FeedbackMode::after_completion, false},
        {"IV", "R2", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::completion_group, FeedbackMode::per_motor_event, false},
        {"IV", "R3", InnateTopology::functional_paths, 3, ExplorationMode::distributed, 16,
         MotorCircuitMode::completion_group, FeedbackMode::virtual_environment, false},
        {"integration", "reference-A", InnateTopology::none, 0, ExplorationMode::none, 0,
         MotorCircuitMode::none, FeedbackMode::none, false},
        {"integration", "stage-I", InnateTopology::functional_paths, 3,
         ExplorationMode::none, 0, MotorCircuitMode::none, FeedbackMode::none, false},
        {"integration", "stage-II", InnateTopology::functional_paths, 3,
         ExplorationMode::distributed, 16, MotorCircuitMode::none, FeedbackMode::none, false},
        {"integration", "stage-III", InnateTopology::functional_paths, 3,
         ExplorationMode::distributed, 16, MotorCircuitMode::completion_group,
         FeedbackMode::none, false},
        {"integration", "stage-IV", InnateTopology::functional_paths, 3,
         ExplorationMode::distributed, 16, MotorCircuitMode::completion_group,
         FeedbackMode::after_completion, false},
        {"control", "without-wiring", InnateTopology::none, 0,
         ExplorationMode::distributed, 16, MotorCircuitMode::completion_group,
         FeedbackMode::after_completion, false},
        {"control", "without-exploration", InnateTopology::functional_paths, 3,
         ExplorationMode::none, 0, MotorCircuitMode::completion_group,
         FeedbackMode::after_completion, false},
        {"control", "without-motor-circuit", InnateTopology::functional_paths, 3,
         ExplorationMode::distributed, 16, MotorCircuitMode::none,
         FeedbackMode::after_completion, false},
        {"control", "without-feedback", InnateTopology::functional_paths, 3,
         ExplorationMode::distributed, 16, MotorCircuitMode::completion_group,
         FeedbackMode::none, false},
        {"performance", "stage-IV-existing-rules", InnateTopology::functional_paths, 1,
         ExplorationMode::distributed, 16, MotorCircuitMode::completion_group,
         FeedbackMode::after_completion, true},
    };
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output_directory =
            argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(output_directory);
        const auto experiment_cases = cases();
        write_parameters(output_directory / "research-stage-a1-parameters.csv", experiment_cases);
        write_paths(output_directory / "research-stage-a1-paths.csv");
        const auto result_path = output_directory / "research-stage-a1-results.csv";
        std::ofstream output(result_path);
        if (!output) throw std::runtime_error("cannot write result CSV: " + result_path.string());
        write_result_header(output);
        for (const Case& item : experiment_cases) {
            for (const std::uint64_t seed : kSeeds) run_case(output, item, seed);
        }
        std::cout << "wrote " << result_path << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage A.1 failed: " << exception.what() << '\n';
        return 1;
    }
}
