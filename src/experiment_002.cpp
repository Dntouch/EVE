#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

constexpr std::uint64_t kFnvOffset = 14695981039346656037ULL;
constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

struct Variant {
    std::string family;
    std::string name;
    std::string parameter;
    eve::ExperimentConfig config;
};

struct Observation {
    std::uint64_t fires{};
    std::uint64_t sequence_hash{kFnvOffset};
    std::unordered_set<eve::NeuronId> active;
    std::vector<eve::NeuronId> preview;

    void record(const eve::bios::FireObservation& event) {
        ++fires;
        active.insert(event.neuron);
        for (unsigned shift = 0; shift < 32; shift += 8) {
            sequence_hash ^= (event.neuron >> shift) & 0xffU;
            sequence_hash *= kFnvPrime;
        }
        if (preview.size() < 64) {
            preview.push_back(event.neuron);
        }
    }
};

struct RunResult {
    bool completed{};
    std::uint32_t repetitions{};
    std::string error;
    eve::Metrics delta;
    Observation observation;
    double seconds{};
};

struct Summary {
    std::uint64_t runs{};
    std::uint64_t completed{};
    std::uint64_t inactive{};
    std::uint64_t unstable{};
    std::uint64_t events{};
    std::uint64_t fires{};
    std::uint64_t strengthenings{};
    std::uint64_t weakenings{};
    std::uint64_t suppressed{};
    double seconds{};
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

std::string hex64(const std::uint64_t value) {
    std::ostringstream stream;
    stream << std::hex << std::setw(16) << std::setfill('0') << value;
    return stream.str();
}

std::string join(const std::vector<eve::NeuronId>& values) {
    std::ostringstream stream;
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) {
            stream << '|';
        }
        stream << values[index];
    }
    return stream.str();
}

eve::Metrics difference(const eve::Metrics& after, const eve::Metrics& before) {
    eve::Metrics result;
    result.processed_events = after.processed_events - before.processed_events;
    result.transmitted_impulses =
        after.transmitted_impulses - before.transmitted_impulses;
    result.fire_events = after.fire_events - before.fire_events;
    result.completed_activity_phases =
        after.completed_activity_phases - before.completed_activity_phases;
    result.synaptic_strengthenings =
        after.synaptic_strengthenings - before.synaptic_strengthenings;
    result.synaptic_weakenings =
        after.synaptic_weakenings - before.synaptic_weakenings;
    result.refractory_suppressed_firings =
        after.refractory_suppressed_firings - before.refractory_suppressed_firings;
    result.growth_attempts = after.growth_attempts - before.growth_attempts;
    result.grown_synapses = after.grown_synapses - before.grown_synapses;
    result.processed_sensor_bytes =
        after.processed_sensor_bytes - before.processed_sensor_bytes;
    result.maximum_queue_size = after.maximum_queue_size;
    result.event_trace_hash = after.event_trace_hash;
    return result;
}

RunResult present(
    eve::bios::Runtime& runtime,
    const std::string& input,
    const std::uint32_t target_repetitions) {
    RunResult result;
    const eve::Metrics before = runtime.metrics();
    runtime.set_fire_observer([&result](const eve::bios::FireObservation& event) {
        result.observation.record(event);
    });
    const auto start = std::chrono::steady_clock::now();
    try {
        for (; result.repetitions < target_repetitions; ++result.repetitions) {
            static_cast<void>(runtime.process_utf8_text(input));
        }
        result.completed = true;
    } catch (const std::exception& error) {
        result.error = error.what();
    }
    const auto end = std::chrono::steady_clock::now();
    runtime.set_fire_observer({});
    result.seconds = std::chrono::duration<double>(end - start).count();
    result.delta = difference(runtime.metrics(), before);
    return result;
}

std::string status(const RunResult& result) {
    if (!result.completed) {
        return "unstable";
    }
    return result.delta.fire_events == 0 ? "inactive" : "stable";
}

std::string weight_distribution(const eve::bios::Runtime& runtime) {
    std::ostringstream stream;
    bool first = true;
    for (const auto& [weight, count] : runtime.network().weight_distribution()) {
        if (!first) {
            stream << ';';
        }
        first = false;
        stream << weight << ':' << count;
    }
    return stream.str();
}

std::uint64_t count_weight(
    const eve::bios::Runtime& runtime,
    const std::int32_t target) {
    return static_cast<std::uint64_t>(std::count_if(
        runtime.network().synapses().begin(),
        runtime.network().synapses().end(),
        [target](const eve::neural::Synapse& synapse) { return synapse.weight == target; }));
}

std::uint64_t positive_weight(const eve::bios::Runtime& runtime) {
    std::uint64_t total = 0;
    for (const eve::neural::Synapse& synapse : runtime.network().synapses()) {
        if (synapse.weight > 0) {
            total += static_cast<std::uint32_t>(synapse.weight);
        }
    }
    return total;
}

std::vector<Variant> variants() {
    std::vector<Variant> result;
    const auto add_weight = [&result](const std::string& name, const std::int32_t value) {
        eve::ExperimentConfig config;
        config.maximum_synapse_weight = value;
        result.push_back(Variant{
            "A", name,
            value == std::numeric_limits<std::int32_t>::max() ? "unlimited"
                                                              : std::to_string(value),
            config});
    };
    add_weight("A0", std::numeric_limits<std::int32_t>::max());
    for (const std::int32_t value : {1, 2, 4, 8, 16}) {
        add_weight("A" + std::to_string(result.size()), value);
    }

    const auto add_budget = [&result](const std::string& name, const std::uint64_t value) {
        eve::ExperimentConfig config;
        config.input_weight_budget = value;
        result.push_back(Variant{
            "B", name,
            value == std::numeric_limits<std::uint64_t>::max() ? "unlimited"
                                                               : std::to_string(value),
            config});
    };
    add_budget("B0", std::numeric_limits<std::uint64_t>::max());
    std::uint32_t budget_index = 1;
    for (const std::uint64_t value : {4ULL, 8ULL, 16ULL, 32ULL, 64ULL}) {
        add_budget("B" + std::to_string(budget_index++), value);
    }

    const auto add_refractory =
        [&result](const std::string& name, const std::uint32_t value) {
            eve::ExperimentConfig config;
            config.refractory_input_events = value;
            result.push_back(Variant{"C", name, std::to_string(value), config});
        };
    add_refractory("C0", 0);
    std::uint32_t refractory_index = 1;
    for (const std::uint32_t value : {1U, 2U, 4U, 8U, 16U}) {
        add_refractory("C" + std::to_string(refractory_index++), value);
    }
    return result;
}

void write_training_header(std::ostream& stream) {
    stream << "family,variant,parameter,seed,input,target_repetitions,completed_repetitions,"
              "status,error,events,fires,active_neurons,max_queue,grown_synapses,"
              "strengthenings,weakenings,suppressed_firings,synapses,zero_weight,at_max_weight,"
              "total_positive_weight,weight_distribution,state_fingerprint,fire_sequence_hash,"
              "fire_preview,memory_bytes,seconds,events_per_second\n";
}

void write_training(
    std::ostream& stream,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& input,
    const std::uint32_t target,
    const eve::bios::Runtime& runtime,
    const RunResult& run) {
    const bool finite_max = variant.config.maximum_synapse_weight !=
                            std::numeric_limits<std::int32_t>::max();
    const auto diagnostics = runtime.diagnostics();
    const double rate = run.seconds > 0.0 ? run.delta.processed_events / run.seconds : 0.0;
    stream << variant.family << ',' << variant.name << ',' << variant.parameter << ',' << seed
           << ',' << input << ',' << target << ',' << run.repetitions << ',' << status(run)
           << ',' << csv(run.error) << ',' << run.delta.processed_events << ','
           << run.delta.fire_events << ',' << run.observation.active.size() << ','
           << diagnostics.metrics.maximum_queue_size << ',' << run.delta.grown_synapses << ','
           << run.delta.synaptic_strengthenings << ',' << run.delta.synaptic_weakenings << ','
           << run.delta.refractory_suppressed_firings << ',' << runtime.network().synapse_count()
           << ',' << count_weight(runtime, 0) << ','
           << (finite_max ? count_weight(runtime, variant.config.maximum_synapse_weight) : 0)
           << ',' << positive_weight(runtime) << ',' << csv(weight_distribution(runtime)) << ','
           << hex64(runtime.state_fingerprint()) << ','
           << hex64(run.observation.sequence_hash) << ',' << csv(join(run.observation.preview))
           << ',' << diagnostics.estimated_memory_bytes << ',' << std::fixed
           << std::setprecision(9) << run.seconds << ',' << std::setprecision(3) << rate << '\n';
}

void write_probe_header(std::ostream& stream) {
    stream << "family,variant,parameter,seed,group,test_input,training_completed,status,error,"
              "events,fires,active_neurons,strengthenings,weakenings,grown_synapses,"
              "fire_sequence_hash,fire_preview,state_before,state_after\n";
}

void write_probe(
    std::ostream& stream,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& group,
    const std::string& input,
    const bool training_completed,
    const eve::bios::Runtime& snapshot) {
    eve::bios::Runtime probe = snapshot;
    const auto before = probe.state_fingerprint();
    const RunResult run = present(probe, input, 1);
    stream << variant.family << ',' << variant.name << ',' << variant.parameter << ',' << seed
           << ',' << group << ',' << input << ',' << (training_completed ? "yes" : "no")
           << ',' << status(run) << ',' << csv(run.error) << ',' << run.delta.processed_events
           << ',' << run.delta.fire_events << ',' << run.observation.active.size() << ','
           << run.delta.synaptic_strengthenings << ',' << run.delta.synaptic_weakenings << ','
           << run.delta.grown_synapses << ',' << hex64(run.observation.sequence_hash) << ','
           << csv(join(run.observation.preview)) << ',' << hex64(before) << ','
           << hex64(probe.state_fingerprint()) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output = argc > 1 ? argv[1] : "research/results";
        const std::uint32_t repetitions =
            argc > 2 ? static_cast<std::uint32_t>(std::stoul(argv[2])) : 512U;
        const std::string variant_filter = argc > 3 ? argv[3] : "";
        std::filesystem::create_directories(output);
        std::ofstream training(output / "experiment-002-training.csv", std::ios::trunc);
        std::ofstream recognition(output / "experiment-002-recognition.csv", std::ios::trunc);
        std::ofstream interference(output / "experiment-002-interference.csv", std::ios::trunc);
        std::ofstream interference_experiences(
            output / "experiment-002-interference-experiences.csv", std::ios::trunc);
        if (!training || !recognition || !interference || !interference_experiences) {
            throw std::runtime_error("cannot create Experiment 002 result files");
        }
        write_training_header(training);
        write_probe_header(recognition);
        write_probe_header(interference);
        write_training_header(interference_experiences);

        const std::vector<std::uint64_t> seeds{20261117, 7, 42, 99, 2026, 314159};
        const std::vector<std::string> training_inputs{"AB", "BA", "AA", "BB"};
        const std::vector<std::string> tests{"AB", "BA", "AC", "AA", "BB"};
        std::map<std::string, Summary> summaries;

        for (const Variant& variant : variants()) {
            if (!variant_filter.empty() && variant.name != variant_filter) {
                continue;
            }
            std::cerr << "Experiment 002 " << variant.name << '\n';
            for (const std::uint64_t seed : seeds) {
                for (const std::string& input : training_inputs) {
                    eve::bios::Runtime runtime(variant.config, seed);
                    const RunResult run = present(runtime, input, repetitions);
                    write_training(training, variant, seed, input, repetitions, runtime, run);
                    Summary& summary = summaries[variant.name];
                    ++summary.runs;
                    summary.completed += run.completed ? 1U : 0U;
                    summary.inactive += status(run) == "inactive" ? 1U : 0U;
                    summary.unstable += status(run) == "unstable" ? 1U : 0U;
                    summary.events += run.delta.processed_events;
                    summary.fires += run.delta.fire_events;
                    summary.strengthenings += run.delta.synaptic_strengthenings;
                    summary.weakenings += run.delta.synaptic_weakenings;
                    summary.suppressed += run.delta.refractory_suppressed_firings;
                    summary.seconds += run.seconds;
                }

                eve::bios::Runtime initial(variant.config, seed);
                eve::bios::Runtime trained = initial;
                eve::bios::Runtime reordered = initial;
                const RunResult trained_run = present(trained, "AB", repetitions);
                const RunResult reordered_run = present(reordered, "BA", repetitions);
                for (const std::string& input : tests) {
                    write_probe(
                        recognition, variant, seed, "trained-AB", input,
                        trained_run.completed, trained);
                    write_probe(
                        recognition, variant, seed, "same-volume-BA", input,
                        reordered_run.completed, reordered);
                    write_probe(recognition, variant, seed, "untrained", input, true, initial);
                }

                if (trained_run.completed) {
                    write_probe(interference, variant, seed, "before-interference", "AB", true,
                                trained);
                    bool interference_completed = true;
                    for (const std::string input : {"CD", "EF", "GH"}) {
                        const RunResult experience = present(
                            trained, input, std::max<std::uint32_t>(1, repetitions / 4));
                        write_training(
                            interference_experiences, variant, seed, input,
                            std::max<std::uint32_t>(1, repetitions / 4), trained, experience);
                        interference_completed = interference_completed && experience.completed;
                        if (!experience.completed) {
                            break;
                        }
                    }
                    write_probe(
                        interference, variant, seed, "after-interference", "AB",
                        interference_completed, trained);
                }
            }
        }

        std::ofstream summary(output / "experiment-002-summary.csv", std::ios::trunc);
        summary << "variant,runs,completed,inactive,unstable,events,fires,strengthenings,"
                   "weakenings,suppressed_firings,seconds,events_per_second\n";
        for (const auto& [name, value] : summaries) {
            const double rate = value.seconds > 0.0 ? value.events / value.seconds : 0.0;
            summary << name << ',' << value.runs << ',' << value.completed << ','
                    << value.inactive << ',' << value.unstable << ',' << value.events << ','
                    << value.fires << ',' << value.strengthenings << ',' << value.weakenings << ','
                    << value.suppressed << ',' << std::fixed << std::setprecision(9)
                    << value.seconds << ',' << std::setprecision(3) << rate << '\n';
        }
        std::cout << "Experiment 002 results written to " << output << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Experiment 002 failed: " << error.what() << '\n';
        return 1;
    }
}
