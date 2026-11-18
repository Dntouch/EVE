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
#include <utility>
#include <vector>

namespace {

struct Variant {
    const char* name;
    std::uint32_t contacts;
};

constexpr std::array<Variant, 3> kVariants{{
    {"M1", 1},
    {"M2", 4},
    {"M3", 8},
}};
constexpr std::array<std::uint64_t, 3> kSeeds{{42, 2026, 20261118}};
constexpr std::uint64_t kAssignmentSeed = 0x4556452d53544147ULL;

std::string csv(const std::string& value) {
    if (value.find_first_of(",\"\n\r") == std::string::npos) {
        return value;
    }
    std::string result = "\"";
    for (const char character : value) {
        result += character == '"' ? "\"\"" : std::string(1, character);
    }
    return result + '"';
}

std::string hex_byte(const std::uint8_t value) {
    std::ostringstream stream;
    stream << "0x" << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<unsigned>(value);
    return stream.str();
}

std::string hex64(const std::uint64_t value) {
    std::ostringstream stream;
    stream << std::hex << std::setw(16) << std::setfill('0') << value;
    return stream.str();
}

const char* status_name(const eve::brainstem::MotorResponseStatus status) {
    switch (status) {
    case eve::brainstem::MotorResponseStatus::no_response:
        return "no_response";
    case eve::brainstem::MotorResponseStatus::completed:
        return "completed";
    case eve::brainstem::MotorResponseStatus::aborted:
        return "aborted";
    }
    return "unknown";
}

std::string channels(const eve::brainstem::MotorResponse& response) {
    std::ostringstream stream;
    bool first = true;
    for (std::size_t channel = 0; channel < response.activated_channels.size(); ++channel) {
        if (!response.activated_channels[channel]) {
            continue;
        }
        if (!first) {
            stream << '|';
        }
        first = false;
        stream << channel;
    }
    return stream.str();
}

std::string activation_order(const eve::brainstem::MotorResponse& response) {
    std::ostringstream stream;
    for (std::size_t index = 0; index < response.activations.size(); ++index) {
        if (index != 0) {
            stream << '|';
        }
        const auto& activation = response.activations[index];
        if (activation.channel < 0) {
            stream << 'C';
        } else {
            stream << static_cast<unsigned>(activation.channel);
        }
    }
    return stream.str();
}

std::string activation_neurons(const eve::brainstem::MotorResponse& response) {
    std::ostringstream stream;
    for (std::size_t index = 0; index < response.activations.size(); ++index) {
        if (index != 0) {
            stream << '|';
        }
        stream << response.activations[index].neuron;
    }
    return stream.str();
}

eve::ExperimentConfig experiment_config(const bool existing_learning) {
    eve::ExperimentConfig config;
    config.learning_enabled = existing_learning;
    config.growth_enabled = existing_learning;
    config.maximum_synapse_weight = existing_learning ? 1 : 0;
    config.refractory_input_events = existing_learning ? 16 : 0;
    config.maximum_events_per_run = 250'000;
    return config;
}

eve::brainstem::MotorOutputConfig motor_config(const Variant& variant) {
    eve::brainstem::MotorOutputConfig config;
    config.contacts_per_channel = variant.contacts;
    config.assignment_seed = kAssignmentSeed;
    config.response_stimulus_neuron = 8;
    return config;
}

void write_header(std::ostream& output) {
    output << "experiment,mode,variant,contacts_per_channel,seed,input,repetition,"
              "prompted,status,valid,value,activated_channels,activation_order,"
              "activation_neurons,response_events,total_events,total_fires,"
              "transmitted_impulses,synaptic_strengthenings,grown_synapses,"
              "queued_events,memory_bytes,state_fingerprint,seconds,error\n";
}

void write_row(
    std::ostream& output,
    const std::string& experiment,
    const std::string& mode,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& input,
    const std::uint32_t repetition,
    const bool prompted,
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorResponse& response,
    const double seconds,
    const std::string& error = {}) {
    const auto diagnostics = runtime.diagnostics();
    const auto& metrics = runtime.metrics();
    output << experiment << ',' << mode << ',' << variant.name << ',' << variant.contacts
           << ',' << seed << ',' << csv(input) << ',' << repetition << ','
           << (prompted ? 1 : 0) << ',' << status_name(response.status) << ','
           << (response.valid() ? 1 : 0) << ',' << hex_byte(response.value) << ','
           << csv(channels(response)) << ',' << csv(activation_order(response)) << ','
           << csv(activation_neurons(response)) << ',' << response.processed_events << ','
           << metrics.processed_events << ',' << metrics.fire_events << ','
           << metrics.transmitted_impulses << ',' << metrics.synaptic_strengthenings << ','
           << metrics.grown_synapses << ',' << diagnostics.queued_events << ','
           << diagnostics.estimated_memory_bytes << ','
           << hex64(runtime.state_fingerprint()) << ',' << std::setprecision(9) << seconds
           << ',' << csv(error) << '\n';
}

void run_single(
    std::ostream& output,
    const std::string& experiment,
    const std::string& mode,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& input,
    const bool prompted) {
    const bool learning = mode == "existing_rules";
    eve::bios::Runtime runtime(experiment_config(learning), seed);
    runtime.configure_motor_output(motor_config(variant));
    eve::brainstem::MotorResponse response;
    std::string error;
    const auto start = std::chrono::steady_clock::now();
    try {
        if (!input.empty()) {
            static_cast<void>(runtime.process_utf8_text(input));
        }
        response = runtime.request_response(prompted, 100'000);
    } catch (const std::exception& exception) {
        response.status = eve::brainstem::MotorResponseStatus::aborted;
        error = exception.what();
    }
    const double seconds = std::chrono::duration<double>(
                               std::chrono::steady_clock::now() - start)
                               .count();
    write_row(output, experiment, mode, variant, seed, input, 1, prompted, runtime,
              response, seconds, error);
}

void run_repetition_series(
    std::ostream& output,
    const std::string& experiment,
    const std::string& mode,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& input,
    const std::uint32_t repetitions) {
    const bool learning = mode == "existing_rules";
    eve::bios::Runtime runtime(experiment_config(learning), seed);
    runtime.configure_motor_output(motor_config(variant));
    for (std::uint32_t repetition = 1; repetition <= repetitions; ++repetition) {
        eve::brainstem::MotorResponse response;
        std::string error;
        const auto start = std::chrono::steady_clock::now();
        try {
            static_cast<void>(runtime.process_utf8_text(input));
            response = runtime.request_response(true, 100'000);
            if (response.valid() && runtime.diagnostics().queued_events != 0) {
                static_cast<void>(runtime.run());
            }
        } catch (const std::exception& exception) {
            response.status = eve::brainstem::MotorResponseStatus::aborted;
            error = exception.what();
        }
        const double seconds = std::chrono::duration<double>(
                                   std::chrono::steady_clock::now() - start)
                                   .count();
        write_row(output, experiment, mode, variant, seed, input, repetition, true,
                  runtime, response, seconds, error);
        if (!error.empty()) {
            break;
        }
    }
}

void write_assignments(const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("cannot write assignment CSV: " + path.string());
    }
    output << "variant,contacts_per_channel,role,channel,contact_index,neuron,"
              "assignment_seed,response_stimulus_neuron\n";
    for (const Variant& variant : kVariants) {
        eve::bios::Runtime runtime(experiment_config(false), 1);
        runtime.configure_motor_output(motor_config(variant));
        const auto& assignment = runtime.motor_assignment();
        for (std::size_t channel = 0; channel < assignment.channels.size(); ++channel) {
            for (std::size_t index = 0; index < assignment.channels[channel].size(); ++index) {
                output << variant.name << ',' << variant.contacts << ",data," << channel
                       << ',' << index << ',' << assignment.channels[channel][index] << ','
                       << kAssignmentSeed << ',' << assignment.config.response_stimulus_neuron
                       << '\n';
            }
        }
        output << variant.name << ',' << variant.contacts
               << ",completion,-1,0," << assignment.completion_contact << ','
               << kAssignmentSeed << ',' << assignment.config.response_stimulus_neuron
               << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output_directory =
            argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(output_directory);
        write_assignments(output_directory / "research-stage-a-assignments.csv");

        const auto result_path = output_directory / "research-stage-a-results.csv";
        std::ofstream output(result_path);
        if (!output) {
            throw std::runtime_error("cannot write result CSV: " + result_path.string());
        }
        write_header(output);

        for (const Variant& variant : kVariants) {
            for (const std::uint64_t seed : kSeeds) {
                run_single(output, "A1", "isolated", variant, seed, "", false);
            }
        }

        const std::array<std::string, 8> inputs{{
            "00", "01", "10", "11", "A", "B", "AB", "BA",
        }};
        for (const std::string& input : inputs) {
            for (const std::uint64_t seed : kSeeds) {
                run_single(output, "A2", "isolated", kVariants[1], seed, input, false);
                run_single(output, "A3", "isolated", kVariants[1], seed, input, true);
            }
        }

        for (const std::string mode : {"isolated", "existing_rules"}) {
            for (const std::uint64_t seed : kSeeds) {
                run_repetition_series(output, "A4", mode, kVariants[1], seed, "AB", 8);
            }
            for (const Variant& variant : kVariants) {
                for (const std::uint64_t seed : kSeeds) {
                    run_repetition_series(output, "A5", mode, variant, seed, "AB", 8);
                }
            }
        }
        std::cout << "wrote " << result_path << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "research stage A failed: " << error.what() << '\n';
        return 1;
    }
}
