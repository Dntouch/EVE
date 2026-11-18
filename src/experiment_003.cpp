#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

constexpr std::uint64_t kFnvOffset = 14695981039346656037ULL;
constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

struct Variant {
    std::string name;
    std::uint32_t refractory{};
    std::int32_t maximum_weight{std::numeric_limits<std::int32_t>::max()};

    [[nodiscard]] eve::ExperimentConfig config() const {
        eve::ExperimentConfig result;
        result.refractory_input_events = refractory;
        result.maximum_synapse_weight = maximum_weight;
        return result;
    }
};

struct Observation {
    std::uint64_t fires{};
    std::uint64_t sequence_hash{kFnvOffset};
    std::unordered_set<eve::NeuronId> active;
    std::unordered_map<std::uint64_t, std::uint64_t> transitions;
    std::vector<eve::NeuronId> preview;
    bool has_previous{};
    eve::NeuronId previous{};

    void record(
        const eve::bios::FireObservation& event,
        const bool include_sequence_details) {
        ++fires;
        active.insert(event.neuron);
        if (!include_sequence_details) {
            return;
        }
        if (has_previous) {
            ++transitions[(static_cast<std::uint64_t>(previous) << 32U) | event.neuron];
        }
        previous = event.neuron;
        has_previous = true;
        for (unsigned shift = 0; shift < 32; shift += 8) {
            sequence_hash ^= (event.neuron >> shift) & 0xffU;
            sequence_hash *= kFnvPrime;
        }
        if (preview.size() < 64) {
            preview.push_back(event.neuron);
        }
    }

    [[nodiscard]] std::string top_transition() const {
        std::uint64_t best_key = 0;
        std::uint64_t best_count = 0;
        for (const auto& [key, count] : transitions) {
            if (count > best_count || (count == best_count && key < best_key)) {
                best_key = key;
                best_count = count;
            }
        }
        if (best_count == 0) {
            return "";
        }
        return std::to_string(best_key >> 32U) + ">" +
               std::to_string(best_key & 0xffffffffU) + ":" +
               std::to_string(best_count);
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

struct StateSummary {
    std::uint64_t neurons{kFnvOffset};
    std::uint64_t weights{kFnvOffset};
    std::uint64_t structure{kFnvOffset};
};

void mix(std::uint64_t& hash, const std::uint64_t value) {
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash ^= (value >> shift) & 0xffU;
        hash *= kFnvPrime;
    }
}

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

std::string join(std::vector<eve::NeuronId> values, const bool sort_values = false) {
    if (sort_values) {
        std::sort(values.begin(), values.end());
    }
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
    result.rejected_strengthenings_at_weight_limit =
        after.rejected_strengthenings_at_weight_limit -
        before.rejected_strengthenings_at_weight_limit;
    result.synaptic_weakenings = after.synaptic_weakenings - before.synaptic_weakenings;
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
    const std::uint32_t repetitions,
    const bool include_sequence_details = false) {
    RunResult result;
    const eve::Metrics before = runtime.metrics();
    runtime.set_fire_observer([&result, include_sequence_details](
                                  const eve::bios::FireObservation& event) {
        result.observation.record(event, include_sequence_details);
    });
    const auto start = std::chrono::steady_clock::now();
    try {
        for (; result.repetitions < repetitions; ++result.repetitions) {
            static_cast<void>(runtime.process_utf8_text(input));
        }
        result.completed = true;
    } catch (const std::exception& error) {
        result.error = error.what();
    }
    result.seconds = std::chrono::duration<double>(
                         std::chrono::steady_clock::now() - start)
                         .count();
    runtime.set_fire_observer({});
    result.delta = difference(runtime.metrics(), before);
    return result;
}

std::string run_status(const RunResult& run) {
    if (!run.completed) {
        return "unstable";
    }
    return run.delta.fire_events == 0 ? "inactive" : "stable";
}

StateSummary summarize_state(const eve::bios::Runtime& runtime) {
    StateSummary result;
    for (const eve::neural::Neuron& neuron : runtime.network().neurons()) {
        mix(result.neurons, static_cast<std::uint64_t>(neuron.membrane_potential));
        mix(result.neurons, neuron.input_event_count);
        mix(result.neurons, neuron.refractory_events_remaining);
    }
    for (const eve::neural::Synapse& synapse : runtime.network().synapses()) {
        mix(result.weights, static_cast<std::uint32_t>(synapse.weight));
        mix(result.weights, synapse.activity_count);
        mix(result.structure, synapse.sender);
        mix(result.structure, synapse.receiver);
    }
    return result;
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

std::uint64_t count_weight(const eve::bios::Runtime& runtime, const std::int32_t weight) {
    return static_cast<std::uint64_t>(std::count_if(
        runtime.network().synapses().begin(), runtime.network().synapses().end(),
        [weight](const eve::neural::Synapse& synapse) { return synapse.weight == weight; }));
}

std::uint64_t learned_grown_synapses(const eve::bios::Runtime& runtime) {
    const auto& synapses = runtime.network().synapses();
    if (synapses.size() <= 1024) {
        return 0;
    }
    return static_cast<std::uint64_t>(std::count_if(
        synapses.begin() + 1024, synapses.end(),
        [](const eve::neural::Synapse& synapse) { return synapse.weight > 0; }));
}

std::int32_t maximum_weight(const eve::bios::Runtime& runtime) {
    std::int32_t result = 0;
    for (const eve::neural::Synapse& synapse : runtime.network().synapses()) {
        result = std::max(result, synapse.weight);
    }
    return result;
}

std::int32_t minimum_weight(const eve::bios::Runtime& runtime) {
    if (runtime.network().synapses().empty()) {
        return 0;
    }
    std::int32_t result = std::numeric_limits<std::int32_t>::max();
    for (const eve::neural::Synapse& synapse : runtime.network().synapses()) {
        result = std::min(result, synapse.weight);
    }
    return result;
}

double average_weight(const eve::bios::Runtime& runtime) {
    if (runtime.network().synapses().empty()) {
        return 0.0;
    }
    std::uint64_t total = 0;
    for (const eve::neural::Synapse& synapse : runtime.network().synapses()) {
        total += static_cast<std::uint32_t>(synapse.weight);
    }
    return static_cast<double>(total) / runtime.network().synapse_count();
}

std::vector<Variant> variants() {
    const auto unlimited = std::numeric_limits<std::int32_t>::max();
    return {
        {"origin", 0, unlimited},
        {"A1", 0, 1},
        {"A2", 0, 2},
        {"C5", 16, unlimited},
        {"C5+A1", 16, 1},
        {"C5+A2", 16, 2},
        {"C5+A3", 16, 4},
        {"C5+A4", 16, 8},
        {"C5+A5", 16, 16},
    };
}

void write_training_header(std::ostream& stream) {
    stream << "variant,refractory_events,max_weight,seed,input,checkpoint,completed_total,"
              "stage_completed,status,error,stage_events,total_events,stage_fires,total_fires,"
              "stage_suppressed,total_suppressed,stage_strengthenings,total_strengthenings,"
              "stage_rejected,total_rejected,stage_grown,total_grown,active_neurons,max_queue,"
              "synapses,weight_zero,weight_zero_percent,weight_one,weight_two,at_max_weight,"
              "saturation_percent,learned_grown_synapses,min_observed_weight,max_observed_weight,"
              "average_weight,weight_distribution,state_fingerprint,"
              "neuron_state,weight_state,structure_state,memory_bytes,stage_seconds,"
              "stage_events_per_second\n";
}

void write_training(
    std::ostream& stream,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& input,
    const std::uint32_t checkpoint,
    const std::uint32_t completed_total,
    const eve::bios::Runtime& runtime,
    const RunResult& run) {
    const eve::Metrics& total = runtime.metrics();
    const bool finite = variant.maximum_weight != std::numeric_limits<std::int32_t>::max();
    const std::uint64_t at_max = finite ? count_weight(runtime, variant.maximum_weight) : 0;
    const std::uint64_t zero = count_weight(runtime, 0);
    const double saturation = runtime.network().synapse_count() == 0
                                  ? 0.0
                                  : 100.0 * at_max / runtime.network().synapse_count();
    const double rate = run.seconds > 0.0 ? run.delta.processed_events / run.seconds : 0.0;
    const StateSummary state = summarize_state(runtime);
    stream << variant.name << ',' << variant.refractory << ','
           << (finite ? std::to_string(variant.maximum_weight) : "unlimited") << ',' << seed
           << ',' << input << ',' << checkpoint << ',' << completed_total << ','
           << run.repetitions << ',' << run_status(run) << ',' << csv(run.error) << ','
           << run.delta.processed_events << ',' << total.processed_events << ','
           << run.delta.fire_events << ',' << total.fire_events << ','
           << run.delta.refractory_suppressed_firings << ','
           << total.refractory_suppressed_firings << ','
           << run.delta.synaptic_strengthenings << ',' << total.synaptic_strengthenings << ','
           << run.delta.rejected_strengthenings_at_weight_limit << ','
           << total.rejected_strengthenings_at_weight_limit << ',' << run.delta.grown_synapses
           << ',' << total.grown_synapses << ',' << run.observation.active.size() << ','
           << total.maximum_queue_size << ',' << runtime.network().synapse_count() << ','
           << zero << ',' << std::fixed << std::setprecision(6)
           << (100.0 * zero / runtime.network().synapse_count()) << ','
           << count_weight(runtime, 1) << ',' << count_weight(runtime, 2) << ',' << at_max
           << ',' << saturation << ',' << learned_grown_synapses(runtime) << ','
           << minimum_weight(runtime) << ',' << maximum_weight(runtime) << ','
           << average_weight(runtime) << ',' << csv(weight_distribution(runtime)) << ','
           << hex64(runtime.state_fingerprint()) << ',' << hex64(state.neurons) << ','
           << hex64(state.weights) << ',' << hex64(state.structure) << ','
           << runtime.diagnostics().estimated_memory_bytes << ',' << std::setprecision(9)
           << run.seconds << ',' << std::setprecision(3) << rate << '\n';
    stream.flush();
}

void write_probe_header(std::ostream& stream) {
    stream << "variant,seed,group,stage,test_input,training_status,status,error,events,fires,"
              "active_neurons,fired_neurons,sequence_hash,sequence_preview,top_transition,"
              "strengthenings,rejected,grown_synapses,state_before,state_after\n";
}

void write_probe(
    std::ostream& stream,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& group,
    const std::string& stage,
    const std::string& input,
    const bool training_completed,
    const eve::bios::Runtime& snapshot) {
    if (!training_completed) {
        stream << variant.name << ',' << seed << ',' << group << ',' << stage << ',' << input
               << ",unstable,not-run,training did not complete,0,0,0,,,,,0,0,0,,\n";
        stream.flush();
        return;
    }
    eve::bios::Runtime probe = snapshot;
    const std::uint64_t before = probe.state_fingerprint();
    const RunResult run = present(probe, input, 1, true);
    std::vector<eve::NeuronId> active(
        run.observation.active.begin(), run.observation.active.end());
    stream << variant.name << ',' << seed << ',' << group << ',' << stage << ',' << input
           << ",complete," << run_status(run) << ',' << csv(run.error) << ','
           << run.delta.processed_events << ',' << run.delta.fire_events << ','
           << active.size() << ',' << csv(join(active, true)) << ','
           << hex64(run.observation.sequence_hash) << ',' << csv(join(run.observation.preview))
           << ',' << csv(run.observation.top_transition()) << ','
           << run.delta.synaptic_strengthenings << ','
           << run.delta.rejected_strengthenings_at_weight_limit << ','
           << run.delta.grown_synapses << ',' << hex64(before) << ','
           << hex64(probe.state_fingerprint()) << '\n';
    stream.flush();
}

void write_experience_header(std::ostream& stream) {
    stream << "variant,seed,stage,input,target_repetitions,completed_repetitions,status,error,"
              "events,fires,suppressed,strengthenings,rejected,grown_synapses,active_neurons,"
              "max_queue,synapses,saturation_percent\n";
}

void write_experience(
    std::ostream& stream,
    const Variant& variant,
    const std::uint64_t seed,
    const std::string& stage,
    const std::string& input,
    const std::uint32_t target,
    const eve::bios::Runtime& runtime,
    const RunResult& run) {
    const bool finite = variant.maximum_weight != std::numeric_limits<std::int32_t>::max();
    const std::uint64_t at_max = finite ? count_weight(runtime, variant.maximum_weight) : 0;
    const double saturation = 100.0 * at_max / runtime.network().synapse_count();
    stream << variant.name << ',' << seed << ',' << stage << ',' << input << ',' << target
           << ',' << run.repetitions << ',' << run_status(run) << ',' << csv(run.error) << ','
           << run.delta.processed_events << ',' << run.delta.fire_events << ','
           << run.delta.refractory_suppressed_firings << ','
           << run.delta.synaptic_strengthenings << ','
           << run.delta.rejected_strengthenings_at_weight_limit << ','
           << run.delta.grown_synapses << ',' << run.observation.active.size() << ','
           << runtime.metrics().maximum_queue_size << ',' << runtime.network().synapse_count()
           << ',' << std::fixed << std::setprecision(6) << saturation << '\n';
    stream.flush();
}

std::string run_training_case(
    const Variant variant,
    const std::uint64_t seed,
    const std::string input,
    const std::vector<std::uint32_t> checkpoints) {
    std::ostringstream rows;
    eve::bios::Runtime runtime(variant.config(), seed);
    std::uint32_t completed_total = 0;
    for (const std::uint32_t checkpoint : checkpoints) {
        const RunResult run = present(runtime, input, checkpoint - completed_total);
        completed_total += run.repetitions;
        write_training(
            rows, variant, seed, input, checkpoint, completed_total, runtime, run);
        if (!run.completed) {
            break;
        }
    }
    return rows.str();
}

struct BehaviorRows {
    std::string recognition;
    std::string interference;
    std::string experiences;
};

BehaviorRows run_behavior_case(
    const Variant variant,
    const std::uint64_t seed,
    const std::vector<std::string> tests) {
    std::ostringstream recognition;
    std::ostringstream interference;
    std::ostringstream experiences;
    eve::bios::Runtime initial(variant.config(), seed);
    eve::bios::Runtime trained_ab = initial;
    eve::bios::Runtime trained_ba = initial;
    const RunResult ab_training = present(trained_ab, "AB", 512);
    const RunResult ba_training = present(trained_ba, "BA", 512);
    for (const std::string& input : tests) {
        write_probe(
            recognition, variant, seed, "trained-AB", "after-512", input,
            ab_training.completed, trained_ab);
        write_probe(
            recognition, variant, seed, "trained-BA", "after-512", input,
            ba_training.completed, trained_ba);
        write_probe(
            recognition, variant, seed, "untrained", "initial", input, true, initial);
    }

    if (!ab_training.completed) {
        write_probe(
            interference, variant, seed, "trained-AB", "before-interference", "AB",
            false, trained_ab);
        return {recognition.str(), interference.str(), experiences.str()};
    }
    write_probe(
        interference, variant, seed, "trained-AB", "before-interference", "AB", true,
        trained_ab);
    bool stable = true;
    for (const std::string input : {"CD", "EF", "GH"}) {
        const RunResult run = present(trained_ab, input, 128);
        write_experience(
            experiences, variant, seed, "to-128", input, 128, trained_ab, run);
        stable = stable && run.completed;
        if (!stable) {
            break;
        }
    }
    write_probe(
        interference, variant, seed, "trained-AB", "after-128-each", "AB", stable,
        trained_ab);
    if (!stable) {
        return {recognition.str(), interference.str(), experiences.str()};
    }
    for (const std::string input : {"CD", "EF", "GH"}) {
        const RunResult run = present(trained_ab, input, 384);
        write_experience(
            experiences, variant, seed, "to-512", input, 384, trained_ab, run);
        stable = stable && run.completed;
        if (!stable) {
            break;
        }
    }
    write_probe(
        interference, variant, seed, "trained-AB", "after-512-each", "AB", stable,
        trained_ab);
    return {recognition.str(), interference.str(), experiences.str()};
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output = argc > 1 ? argv[1] : "research/results";
        const std::string variant_filter = argc > 2 ? argv[2] : "";
        std::filesystem::create_directories(output);
        std::ofstream training(output / "experiment-003-training.csv", std::ios::trunc);
        std::ofstream recognition(output / "experiment-003-recognition.csv", std::ios::trunc);
        std::ofstream interference(output / "experiment-003-interference.csv", std::ios::trunc);
        std::ofstream experiences(
            output / "experiment-003-interference-experiences.csv", std::ios::trunc);
        if (!training || !recognition || !interference || !experiences) {
            throw std::runtime_error("cannot create Experiment 003 result files");
        }
        write_training_header(training);
        write_probe_header(recognition);
        write_probe_header(interference);
        write_experience_header(experiences);

        const std::vector<std::uint64_t> seeds{20261117, 7, 42, 99, 2026, 314159};
        const std::vector<std::string> inputs{"AB", "BA", "AA", "BB"};
        const std::vector<std::string> tests{"AB", "BA", "AC", "AA", "BB"};
        const std::vector<std::uint32_t> checkpoints{512, 1024, 4096, 16384};

        for (const Variant& variant : variants()) {
            if (!variant_filter.empty() && variant.name != variant_filter) {
                continue;
            }
            std::cerr << "Experiment 003 " << variant.name << '\n';
            std::vector<std::future<std::string>> training_jobs;
            for (const std::uint64_t seed : seeds) {
                for (const std::string& input : inputs) {
                    training_jobs.push_back(std::async(
                        std::launch::async, run_training_case, variant, seed, input,
                        checkpoints));
                }
            }
            for (std::future<std::string>& job : training_jobs) {
                training << job.get();
                training.flush();
            }

            std::vector<std::future<BehaviorRows>> behavior_jobs;
            for (const std::uint64_t seed : seeds) {
                behavior_jobs.push_back(std::async(
                    std::launch::async, run_behavior_case, variant, seed, tests));
            }
            for (std::future<BehaviorRows>& job : behavior_jobs) {
                const BehaviorRows rows = job.get();
                recognition << rows.recognition;
                interference << rows.interference;
                experiences << rows.experiences;
                recognition.flush();
                interference.flush();
                experiences.flush();
            }
        }
        std::cout << "Experiment 003 results written to " << output << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Experiment 003 failed: " << error.what() << '\n';
        return 1;
    }
}
