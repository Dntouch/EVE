#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

struct Variant {
    std::string name;
    std::int32_t maximum_weight;
};

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

void set_geometry(eve::ExperimentConfig& config, const std::uint32_t neurons) {
    config.neuron_count = neurons;
    if (neurons == 256) {
        config.width = 8;
        config.height = 8;
        config.depth = 4;
    } else if (neurons == 1024) {
        config.width = 16;
        config.height = 8;
        config.depth = 8;
    } else if (neurons == 10'000) {
        config.width = 25;
        config.height = 20;
        config.depth = 20;
    } else {
        throw std::invalid_argument("unsupported benchmark network size");
    }
}

std::vector<Variant> variants() {
    return {
        {"C5", std::numeric_limits<std::int32_t>::max()},
        {"C5+A1", 1},
        {"C5+A2", 2},
        {"C5+A3", 4},
        {"C5+A4", 8},
        {"C5+A5", 16},
    };
}

void write_row(
    std::ostream& stream,
    const std::string& mode,
    const Variant& variant,
    const std::uint32_t neurons,
    const std::uint64_t seed,
    const bool completed,
    const std::string& error,
    const eve::bios::Runtime& runtime,
    const double seconds,
    const double cpu_seconds) {
    const eve::Metrics& metrics = runtime.metrics();
    const double processed_rate = seconds > 0.0 ? metrics.processed_events / seconds : 0.0;
    const double synaptic_rate =
        seconds > 0.0 ? metrics.transmitted_impulses / seconds : 0.0;
    const double cpu_percent = seconds > 0.0 ? 100.0 * cpu_seconds / seconds : 0.0;
    stream << mode << ',' << variant.name << ',' << neurons << ','
           << runtime.network().synapse_count() << ',' << seed << ','
           << (completed ? "yes" : "no") << ',' << csv(error) << ','
           << metrics.processed_events
           << ',' << metrics.transmitted_impulses << ',' << metrics.fire_events << ','
           << metrics.refractory_suppressed_firings << ',' << metrics.synaptic_strengthenings
           << ',' << metrics.rejected_strengthenings_at_weight_limit << ','
           << metrics.maximum_queue_size << ',' << runtime.diagnostics().estimated_memory_bytes
           << ',' << sizeof(eve::neural::Neuron) << ',' << sizeof(eve::neural::Synapse) << ','
           << sizeof(eve::bios::Event) << ',' << std::fixed << std::setprecision(9) << seconds
           << ',' << cpu_seconds << ',' << std::setprecision(3) << cpu_percent << ','
           << processed_rate << ',' << synaptic_rate << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output =
            argc > 1 ? argv[1] : "research/results/experiment-003-performance.csv";
        std::ofstream stream(output, std::ios::trunc);
        if (!stream) {
            throw std::runtime_error("cannot create benchmark output: " + output.string());
        }
        stream << "mode,variant,neurons,synapses,seed,completed,error,processed_events,"
                  "synaptic_events,fire_events,suppressed_firings,strengthenings,rejected,"
                  "max_queue,estimated_memory_bytes,neuron_struct_bytes,synapse_struct_bytes,"
                  "event_struct_bytes,seconds,cpu_seconds,cpu_percent,processed_events_per_second,"
                  "synaptic_events_per_second\n";

        const std::vector<std::uint64_t> seeds{20261117, 7, 42, 99, 2026, 314159};
        for (const Variant& variant : variants()) {
            std::cerr << "Experiment 003 benchmark " << variant.name << '\n';
            for (const std::uint32_t neurons : {256U, 1024U, 10'000U}) {
                for (const std::uint64_t seed : seeds) {
                    eve::ExperimentConfig static_config;
                    set_geometry(static_config, neurons);
                    static_config.maximum_synapse_weight = variant.maximum_weight;
                    static_config.refractory_input_events = 16;
                    static_config.learning_enabled = false;
                    static_config.growth_enabled = false;
                    static_config.maximum_events_per_run = 2'000'000;
                    eve::bios::Runtime static_runtime(static_config, seed);
                    const auto static_start = Clock::now();
                    const std::clock_t static_cpu_start = std::clock();
                    constexpr std::uint64_t kFires = 250'000;
                    constexpr std::uint64_t kBatch = 4096;
                    for (std::uint64_t first = 0; first < kFires; first += kBatch) {
                        const std::uint64_t count = std::min(kBatch, kFires - first);
                        for (std::uint64_t offset = 0; offset < count; ++offset) {
                            static_runtime.enqueue_fire(static_cast<eve::NeuronId>(
                                (first + offset) % neurons));
                        }
                        static_cast<void>(static_runtime.run());
                    }
                    const double static_seconds =
                        std::chrono::duration<double>(Clock::now() - static_start).count();
                    const double static_cpu = static_cast<double>(
                                                  std::clock() - static_cpu_start) /
                                              CLOCKS_PER_SEC;
                    write_row(
                        stream, "static", variant, neurons, seed, true, "", static_runtime,
                        static_seconds, static_cpu);

                    eve::ExperimentConfig active_config = static_config;
                    active_config.learning_enabled = true;
                    eve::bios::Runtime active_runtime(active_config, seed);
                    bool completed = true;
                    std::string error;
                    const auto active_start = Clock::now();
                    const std::clock_t active_cpu_start = std::clock();
                    try {
                        for (std::uint32_t repetition = 0; repetition < 512; ++repetition) {
                            static_cast<void>(active_runtime.process_utf8_text("AB"));
                        }
                    } catch (const std::exception& exception) {
                        completed = false;
                        error = exception.what();
                    }
                    const double active_seconds =
                        std::chrono::duration<double>(Clock::now() - active_start).count();
                    const double active_cpu = static_cast<double>(
                                                  std::clock() - active_cpu_start) /
                                              CLOCKS_PER_SEC;
                    write_row(
                        stream, "active-learning", variant, neurons, seed, completed, error,
                        active_runtime, active_seconds, active_cpu);
                }
            }
        }
        std::cout << "Experiment 003 benchmark written to " << output << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Experiment 003 benchmark failed: " << error.what() << '\n';
        return 1;
    }
}
