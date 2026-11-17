#include "eve/bios/runtime.hpp"
#include "eve/neural/network.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

namespace {

using Clock = std::chrono::steady_clock;

struct Options {
    std::uint32_t neurons{256};
    std::uint32_t trial{1};
    std::uint64_t seed{20261117};
    std::uint64_t target_synaptic_events{1'000'000};
    bool header_only{};
};

std::uint64_t parse_u64(const std::string& text, const std::string& option) {
    std::size_t consumed = 0;
    const std::uint64_t value = std::stoull(text, &consumed, 10);
    if (consumed != text.size()) {
        throw std::invalid_argument("invalid value for " + option + ": " + text);
    }
    return value;
}

Options parse_options(const int argc, char** argv) {
    Options options;
    for (int index = 1; index < argc; ++index) {
        const std::string option = argv[index];
        const auto value = [&]() -> std::string {
            if (++index >= argc) {
                throw std::invalid_argument("missing value for " + option);
            }
            return argv[index];
        };
        if (option == "--header") {
            options.header_only = true;
        } else if (option == "--neurons") {
            const auto parsed = parse_u64(value(), option);
            if (parsed > std::numeric_limits<std::uint32_t>::max()) {
                throw std::invalid_argument("neuron count exceeds the supported index range");
            }
            options.neurons = static_cast<std::uint32_t>(parsed);
        } else if (option == "--trial") {
            options.trial = static_cast<std::uint32_t>(parse_u64(value(), option));
        } else if (option == "--seed") {
            options.seed = parse_u64(value(), option);
        } else if (option == "--synaptic-events") {
            options.target_synaptic_events = parse_u64(value(), option);
        } else {
            throw std::invalid_argument("unknown option: " + option);
        }
    }
    return options;
}

eve::ExperimentConfig benchmark_config(const std::uint32_t neurons) {
    eve::ExperimentConfig config;
    config.neuron_count = neurons;
    config.initial_outgoing_synapses = 4;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.maximum_events_per_run = 1'000'000;

    switch (neurons) {
        case 256:
            config.width = 8;
            config.height = 8;
            config.depth = 4;
            break;
        case 1'024:
            config.width = 16;
            config.height = 8;
            config.depth = 8;
            break;
        case 10'000:
            config.width = 25;
            config.height = 20;
            config.depth = 20;
            break;
        case 100'000:
            config.width = 50;
            config.height = 50;
            config.depth = 40;
            break;
        case 1'000'000:
            config.width = 100;
            config.height = 100;
            config.depth = 100;
            break;
        default:
            throw std::invalid_argument("no benchmark geometry is defined for this network size");
    }
    return config;
}

std::uint64_t current_rss_bytes() {
#if defined(__linux__)
    std::ifstream status("/proc/self/status");
    std::string key;
    while (status >> key) {
        if (key == "VmRSS:") {
            std::uint64_t kibibytes = 0;
            std::string unit;
            status >> kibibytes >> unit;
            return kibibytes * 1024ULL;
        }
        std::string rest;
        std::getline(status, rest);
    }
#endif
    return 0;
}

std::uint64_t peak_rss_bytes() {
#if defined(__unix__) || defined(__APPLE__)
    rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
#if defined(__APPLE__)
        return static_cast<std::uint64_t>(usage.ru_maxrss);
#else
        return static_cast<std::uint64_t>(usage.ru_maxrss) * 1024ULL;
#endif
    }
#endif
    return 0;
}

double seconds_between(const Clock::time_point start, const Clock::time_point end) {
    return std::chrono::duration<double>(end - start).count();
}

double cpu_seconds_between(const std::clock_t start, const std::clock_t end) {
    return static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC);
}

void print_header() {
    std::cout
        << "neurons,initial_synapses,trial,seed,target_synaptic_events,"
           "initialization_seconds,initialization_cpu_seconds,initialization_cpu_percent,"
           "processing_seconds,processing_cpu_seconds,processing_cpu_percent,"
           "instrumentation_seconds,total_seconds,"
           "synaptic_events,fire_events,synaptic_events_per_second,fire_events_per_second,"
           "grown_synapses,estimated_memory_bytes,rss_after_initialization_bytes,"
           "rss_increment_bytes,peak_rss_bytes,bytes_per_neuron_rss,neuron_struct_bytes,"
           "synapse_struct_bytes,event_struct_bytes,state_fingerprint\n";
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const Options options = parse_options(argc, argv);
        if (options.header_only) {
            print_header();
            return 0;
        }
        if (options.target_synaptic_events == 0) {
            throw std::invalid_argument("--synaptic-events must be greater than zero");
        }

        const eve::ExperimentConfig config = benchmark_config(options.neurons);
        const std::uint64_t baseline_rss = current_rss_bytes();
        const Clock::time_point total_start = Clock::now();
        const Clock::time_point initialization_start = total_start;
        const std::clock_t initialization_cpu_start = std::clock();
        auto runtime = std::make_unique<eve::bios::Runtime>(config, options.seed);
        const std::clock_t initialization_cpu_end = std::clock();
        const Clock::time_point initialization_end = Clock::now();

        const std::uint64_t rss_after_initialization = current_rss_bytes();
        const std::uint64_t rss_increment =
            rss_after_initialization > baseline_rss ? rss_after_initialization - baseline_rss : 0;
        const std::size_t estimated_memory = runtime->diagnostics().estimated_memory_bytes;

        const std::uint64_t fires_needed =
            (options.target_synaptic_events + config.initial_outgoing_synapses - 1U) /
            config.initial_outgoing_synapses;
        constexpr std::uint64_t kBatchSize = 4096;
        std::uint64_t fires_enqueued = 0;
        const Clock::time_point processing_start = Clock::now();
        const std::clock_t processing_cpu_start = std::clock();
        while (fires_enqueued < fires_needed) {
            const std::uint64_t batch =
                std::min(kBatchSize, fires_needed - fires_enqueued);
            for (std::uint64_t offset = 0; offset < batch; ++offset) {
                const eve::NeuronId neuron = static_cast<eve::NeuronId>(
                    (fires_enqueued + offset) % config.neuron_count);
                runtime->enqueue_fire(neuron);
            }
            fires_enqueued += batch;
            static_cast<void>(runtime->run());
        }
        const std::clock_t processing_cpu_end = std::clock();
        const Clock::time_point processing_end = Clock::now();

        const eve::bios::Diagnostics diagnostics = runtime->diagnostics();
        const double initialization_seconds =
            seconds_between(initialization_start, initialization_end);
        const double initialization_cpu =
            cpu_seconds_between(initialization_cpu_start, initialization_cpu_end);
        const double processing_seconds = seconds_between(processing_start, processing_end);
        const double processing_cpu =
            cpu_seconds_between(processing_cpu_start, processing_cpu_end);
        const double total_seconds = seconds_between(total_start, processing_end);
        const double instrumentation_seconds =
            std::max(0.0, total_seconds - initialization_seconds - processing_seconds);
        const double initialization_cpu_percent =
            initialization_seconds > 0 ? initialization_cpu / initialization_seconds * 100.0 : 0;
        const double processing_cpu_percent =
            processing_seconds > 0 ? processing_cpu / processing_seconds * 100.0 : 0;
        const double synaptic_rate = processing_seconds > 0
                                         ? diagnostics.metrics.transmitted_impulses /
                                               processing_seconds
                                         : 0;
        const double fire_rate =
            processing_seconds > 0 ? diagnostics.metrics.fire_events / processing_seconds : 0;
        const double bytes_per_neuron =
            config.neuron_count > 0 ? static_cast<double>(rss_increment) / config.neuron_count : 0;

        std::cout << std::fixed << std::setprecision(6) << config.neuron_count << ','
                  << runtime->network().synapse_count() << ',' << options.trial << ','
                  << options.seed << ',' << options.target_synaptic_events << ','
                  << initialization_seconds << ',' << initialization_cpu << ','
                  << initialization_cpu_percent << ',' << processing_seconds << ','
                  << processing_cpu << ',' << processing_cpu_percent << ','
                  << instrumentation_seconds << ',' << total_seconds << ','
                  << diagnostics.metrics.transmitted_impulses << ','
                  << diagnostics.metrics.fire_events << ',' << synaptic_rate << ',' << fire_rate
                  << ',' << diagnostics.metrics.grown_synapses << ',' << estimated_memory << ','
                  << rss_after_initialization << ',' << rss_increment << ',' << peak_rss_bytes()
                  << ',' << bytes_per_neuron << ',' << sizeof(eve::neural::Neuron) << ','
                  << sizeof(eve::neural::Synapse) << ',' << sizeof(eve::bios::Event) << ','
                  << std::hex << diagnostics.state_fingerprint << std::dec << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
