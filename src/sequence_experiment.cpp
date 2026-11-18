#include "eve/bios/runtime.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Case {
    std::string name;
    std::string pattern;
    std::uint32_t repetitions;
};

void run_case(const Case& experiment, const std::uint64_t seed) {
    eve::ExperimentConfig config;
    config.growth_enabled = false;
    config.maximum_events_per_run = 100'000;
    eve::bios::Runtime runtime(config, seed);

    bool completed = true;
    std::string error;
    try {
        for (std::uint32_t index = 0; index < experiment.repetitions; ++index) {
            static_cast<void>(runtime.process_utf8_text(experiment.pattern));
        }
    } catch (const std::exception& exception) {
        completed = false;
        error = exception.what();
    }

    const eve::bios::Diagnostics diagnostics = runtime.diagnostics();
    std::cout << experiment.name << ',' << experiment.pattern << ','
              << experiment.repetitions << ',' << (completed ? "yes" : "no") << ','
              << diagnostics.metrics.processed_events << ','
              << diagnostics.metrics.fire_events << ','
              << diagnostics.metrics.completed_activity_phases << ','
              << diagnostics.metrics.synaptic_strengthenings << ','
              << "0x" << std::hex << std::setw(16) << std::setfill('0')
              << diagnostics.metrics.event_trace_hash << ',' << "0x" << std::setw(16)
              << runtime.network().state_fingerprint() << ',' << "0x" << std::setw(16)
              << diagnostics.state_fingerprint << std::dec << ',' << error << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    try {
        std::uint64_t seed = 1;
        if (argc == 3 && std::string(argv[1]) == "--seed") {
            std::size_t consumed = 0;
            seed = std::stoull(argv[2], &consumed, 10);
            if (consumed != std::string(argv[2]).size()) {
                throw std::invalid_argument("invalid seed");
            }
        } else if (argc != 1) {
            throw std::invalid_argument("usage: eve_sequence_experiment [--seed N]");
        }

        const std::vector<Case> cases{
            {"single-ab", "AB", 1},
            {"single-ba", "BA", 1},
            {"repeated-ab", "AB", 8},
            {"repeated-ba", "BA", 8},
        };
        std::cout << "case,pattern,repetitions,completed,events,fires,phases,"
                     "strengthenings,event_hash,neural_state,complete_state,error\n";
        for (const Case& experiment : cases) {
            run_case(experiment, seed);
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
