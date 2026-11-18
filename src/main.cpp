#include "eve/bios/runtime.hpp"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace {

std::uint64_t parse_u64(const std::string& value, const std::string& option) {
    std::size_t consumed = 0;
    const std::uint64_t parsed = std::stoull(value, &consumed, 10);
    if (consumed != value.size()) {
        throw std::invalid_argument("invalid value for " + option + ": " + value);
    }
    return parsed;
}

void print_help(const char* program) {
    std::cout
        << "Usage: " << program << " [options]\n"
        << "  --input TEXT          raw UTF-8 input (default: AB)\n"
        << "  --repeat N            process the input N times (default: 1)\n"
        << "  --seed N              deterministic initial seed (default: 1)\n"
        << "  --no-learning         disable synaptic strengthening\n"
        << "  --no-growth           disable structural growth\n"
        << "  --reset MODE          zero or subtract (default: zero)\n"
        << "  --max-events N        event limit per byte\n"
        << "  --trace PATH          write a detailed event log\n"
        << "  --save PATH           save the complete state after the run\n"
        << "  --load PATH           restore a complete state before the run\n"
        << "  --help                show this help\n";
}

void print_diagnostics(const eve::bios::Diagnostics& diagnostics) {
    const eve::Metrics& metrics = diagnostics.metrics;
    std::cout << "neurons=" << diagnostics.neuron_count << '\n'
              << "synapses=" << diagnostics.synapse_count << '\n'
              << "fire_events=" << metrics.fire_events << '\n'
              << "completed_activity_phases=" << metrics.completed_activity_phases << '\n'
              << "synaptic_strengthenings=" << metrics.synaptic_strengthenings << '\n'
              << "grown_synapses=" << metrics.grown_synapses << '\n'
              << "processed_events=" << metrics.processed_events << '\n'
              << "transmitted_impulses=" << metrics.transmitted_impulses << '\n'
              << "processed_sensor_bytes=" << metrics.processed_sensor_bytes << '\n'
              << "estimated_memory_bytes=" << diagnostics.estimated_memory_bytes << '\n'
              << "queued_events=" << diagnostics.queued_events << '\n'
              << "event_trace_hash=0x" << std::hex << std::setw(16)
              << std::setfill('0') << metrics.event_trace_hash << '\n'
              << "state_fingerprint=0x" << std::setw(16)
              << diagnostics.state_fingerprint << std::dec << '\n'
              << "weight_distribution=";
    bool first = true;
    for (const auto& [weight, count] : diagnostics.weight_distribution) {
        if (!first) {
            std::cout << ',';
        }
        first = false;
        std::cout << weight << ':' << count;
    }
    std::cout << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    try {
        eve::ExperimentConfig config;
        std::string input = "AB";
        std::string trace_path;
        std::string save_path;
        std::string load_path;
        std::uint64_t seed = 1;
        std::uint64_t repeat = 1;

        for (int index = 1; index < argc; ++index) {
            const std::string option = argv[index];
            const auto require_value = [&]() -> std::string {
                if (++index >= argc) {
                    throw std::invalid_argument("missing value for " + option);
                }
                return argv[index];
            };

            if (option == "--help") {
                print_help(argv[0]);
                return 0;
            }
            if (option == "--input") {
                input = require_value();
            } else if (option == "--repeat") {
                repeat = parse_u64(require_value(), option);
            } else if (option == "--seed") {
                seed = parse_u64(require_value(), option);
            } else if (option == "--no-learning") {
                config.learning_enabled = false;
            } else if (option == "--no-growth") {
                config.growth_enabled = false;
            } else if (option == "--reset") {
                const std::string mode = require_value();
                if (mode == "zero") {
                    config.reset_mode = eve::ResetMode::to_zero;
                } else if (mode == "subtract") {
                    config.reset_mode = eve::ResetMode::subtract_threshold;
                } else {
                    throw std::invalid_argument("reset mode must be zero or subtract");
                }
            } else if (option == "--max-events") {
                config.maximum_events_per_run = parse_u64(require_value(), option);
            } else if (option == "--trace") {
                trace_path = require_value();
            } else if (option == "--save") {
                save_path = require_value();
            } else if (option == "--load") {
                load_path = require_value();
            } else {
                throw std::invalid_argument("unknown option: " + option);
            }
        }
        if (repeat == 0) {
            throw std::invalid_argument("--repeat must be greater than zero");
        }

        eve::bios::Runtime runtime = load_path.empty()
                                         ? eve::bios::Runtime(config, seed)
                                         : eve::bios::Runtime::load(load_path);
        std::ofstream trace_file;
        if (!trace_path.empty()) {
            trace_file.open(trace_path, std::ios::trunc);
            if (!trace_file) {
                throw std::runtime_error("cannot open trace file: " + trace_path);
            }
            runtime.set_trace_stream(&trace_file);
        }

        for (std::uint64_t iteration = 0; iteration < repeat; ++iteration) {
            static_cast<void>(runtime.process_utf8_text(input));
        }
        if (!save_path.empty()) {
            runtime.save(save_path);
        }
        print_diagnostics(runtime.diagnostics());
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
