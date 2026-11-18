#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
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

void mix(std::uint64_t& hash, const std::uint64_t value) {
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash ^= (value >> shift) & 0xffU;
        hash *= kFnvPrime;
    }
}

std::string hex64(const std::uint64_t value) {
    std::ostringstream stream;
    stream << std::hex << std::setw(16) << std::setfill('0') << value;
    return stream.str();
}

std::string csv(const std::string& value) {
    if (value.find_first_of(",\"\n\r") == std::string::npos) {
        return value;
    }
    std::string escaped = "\"";
    for (const char character : value) {
        if (character == '"') {
            escaped += "\"\"";
        } else {
            escaped += character;
        }
    }
    escaped += '"';
    return escaped;
}

class ActivityRecorder {
public:
    void observe(const eve::bios::FireObservation& event) {
        ++fires_;
        mix(hash_, event.neuron);
        unique_.insert(event.neuron);
        ++neuron_counts_[event.neuron];
        if (has_previous_) {
            const std::uint64_t key =
                (static_cast<std::uint64_t>(previous_) << 32U) | event.neuron;
            ++bigrams_[key];
        }
        previous_ = event.neuron;
        has_previous_ = true;
        if (preview_.size() < 64) {
            preview_.push_back(event.neuron);
        }
    }

    [[nodiscard]] std::uint64_t fires() const noexcept { return fires_; }
    [[nodiscard]] std::uint64_t hash() const noexcept { return hash_; }
    [[nodiscard]] std::size_t unique_count() const noexcept { return unique_.size(); }

    [[nodiscard]] std::string unique_neurons() const {
        std::vector<eve::NeuronId> values(unique_.begin(), unique_.end());
        std::sort(values.begin(), values.end());
        return join(values);
    }

    [[nodiscard]] std::string preview() const { return join(preview_); }

    [[nodiscard]] std::string most_common_neuron() const {
        eve::NeuronId best = 0;
        std::uint64_t count = 0;
        for (const auto& [neuron, occurrences] : neuron_counts_) {
            if (occurrences > count || (occurrences == count && neuron < best)) {
                best = neuron;
                count = occurrences;
            }
        }
        return count == 0 ? "" : std::to_string(best) + ":" + std::to_string(count);
    }

    [[nodiscard]] std::string most_common_bigram() const {
        std::uint64_t best = 0;
        std::uint64_t count = 0;
        for (const auto& [bigram, occurrences] : bigrams_) {
            if (occurrences > count || (occurrences == count && bigram < best)) {
                best = bigram;
                count = occurrences;
            }
        }
        if (count == 0) {
            return "";
        }
        const auto first = static_cast<eve::NeuronId>(best >> 32U);
        const auto second = static_cast<eve::NeuronId>(best & 0xffffffffU);
        return std::to_string(first) + ">" + std::to_string(second) + ":" +
               std::to_string(count);
    }

private:
    static std::string join(const std::vector<eve::NeuronId>& values) {
        std::ostringstream stream;
        for (std::size_t index = 0; index < values.size(); ++index) {
            if (index != 0) {
                stream << '|';
            }
            stream << values[index];
        }
        return stream.str();
    }

    std::uint64_t fires_{};
    std::uint64_t hash_{kFnvOffset};
    bool has_previous_{};
    eve::NeuronId previous_{};
    std::unordered_set<eve::NeuronId> unique_;
    std::unordered_map<eve::NeuronId, std::uint64_t> neuron_counts_;
    std::unordered_map<std::uint64_t, std::uint64_t> bigrams_;
    std::vector<eve::NeuronId> preview_;
};

struct StateSummary {
    std::uint64_t neural{};
    std::uint64_t neurons{};
    std::uint64_t weights{};
    std::uint64_t structure{};
    std::string weight_distribution;
};

StateSummary summarize(const eve::bios::Runtime& runtime) {
    StateSummary result;
    result.neural = runtime.network().state_fingerprint();
    result.neurons = kFnvOffset;
    result.weights = kFnvOffset;
    result.structure = kFnvOffset;

    for (const eve::neural::Neuron& neuron : runtime.network().neurons()) {
        mix(result.neurons, static_cast<std::uint64_t>(neuron.membrane_potential));
        mix(result.neurons, neuron.input_event_count);
    }
    for (const eve::neural::Synapse& synapse : runtime.network().synapses()) {
        mix(result.weights, static_cast<std::uint32_t>(synapse.weight));
        mix(result.weights, synapse.activity_count);
        mix(result.structure, synapse.sender);
        mix(result.structure, synapse.receiver);
    }

    const auto distribution = runtime.network().weight_distribution();
    std::ostringstream weights;
    bool first = true;
    for (const auto& [weight, count] : distribution) {
        if (!first) {
            weights << ';';
        }
        first = false;
        weights << weight << ':' << count;
    }
    result.weight_distribution = weights.str();
    return result;
}

eve::Metrics difference(const eve::Metrics& after, const eve::Metrics& before) {
    eve::Metrics delta;
    delta.processed_events = after.processed_events - before.processed_events;
    delta.transmitted_impulses = after.transmitted_impulses - before.transmitted_impulses;
    delta.fire_events = after.fire_events - before.fire_events;
    delta.completed_activity_phases =
        after.completed_activity_phases - before.completed_activity_phases;
    delta.synaptic_strengthenings =
        after.synaptic_strengthenings - before.synaptic_strengthenings;
    delta.growth_attempts = after.growth_attempts - before.growth_attempts;
    delta.grown_synapses = after.grown_synapses - before.grown_synapses;
    delta.processed_sensor_bytes =
        after.processed_sensor_bytes - before.processed_sensor_bytes;
    delta.event_trace_hash = after.event_trace_hash;
    return delta;
}

bool present(
    eve::bios::Runtime& runtime,
    const std::string& input,
    const std::uint32_t repetitions,
    ActivityRecorder& recorder,
    std::uint32_t& completed,
    std::string& error) {
    runtime.set_fire_observer([&recorder](const eve::bios::FireObservation& event) {
        recorder.observe(event);
    });
    try {
        for (std::uint32_t index = 0; index < repetitions; ++index) {
            static_cast<void>(runtime.process_utf8_text(input));
            ++completed;
        }
        runtime.set_fire_observer({});
        return true;
    } catch (const std::exception& exception) {
        runtime.set_fire_observer({});
        error = exception.what();
        return false;
    }
}

std::ofstream open_csv(const std::filesystem::path& path) {
    std::ofstream stream(path, std::ios::trunc);
    if (!stream) {
        throw std::runtime_error("cannot create result file: " + path.string());
    }
    return stream;
}

void write_training_row(
    std::ostream& stream,
    const std::string& label,
    const std::string& input,
    const std::uint32_t target_repetitions,
    const std::uint32_t completed_repetitions,
    const bool success,
    const std::string& error,
    const eve::bios::Runtime& runtime,
    const ActivityRecorder& activity) {
    const StateSummary state = summarize(runtime);
    const eve::Metrics& metrics = runtime.metrics();
    stream << csv(label) << ',' << csv(input) << ',' << target_repetitions << ','
           << completed_repetitions << ',' << (success ? "yes" : "no") << ',' << csv(error)
           << ',' << runtime.network().synapse_count() << ',' << metrics.processed_events << ','
           << metrics.fire_events << ',' << metrics.completed_activity_phases << ','
           << metrics.synaptic_strengthenings << ',' << metrics.grown_synapses << ','
           << csv(state.weight_distribution) << ',' << hex64(state.neural) << ','
           << hex64(state.neurons) << ',' << hex64(state.weights) << ','
           << hex64(state.structure) << ',' << activity.fires() << ','
           << activity.unique_count() << ',' << hex64(activity.hash()) << ','
           << csv(activity.unique_neurons()) << ',' << csv(activity.most_common_neuron()) << ','
           << csv(activity.most_common_bigram()) << ',' << csv(activity.preview()) << '\n';
}

void experiment_a(const std::filesystem::path& output, const std::uint64_t seed) {
    std::ofstream stream = open_csv(output / "research-002-experiment-a.csv");
    stream << "stage,input,target_repetitions,completed_repetitions,success,error,synapses,"
              "processed_events,fire_events,phases,strengthenings,grown_synapses,"
              "weight_distribution,neural_state,neuron_state,weight_state,structure_state,"
              "stage_fires,unique_fired,fire_sequence_hash,fired_neurons,top_neuron,"
              "top_bigram,fire_preview\n";

    const std::vector<std::uint32_t> checkpoints{1, 8, 32, 128, 512};
    const auto run_variant = [&](const std::string& label, const eve::ExperimentConfig& config) {
        eve::bios::Runtime runtime(config, seed);
        std::uint32_t completed_total = 0;
        bool still_running = true;
        std::string previous_error;
        for (const std::uint32_t checkpoint : checkpoints) {
            ActivityRecorder recorder;
            std::string error = previous_error;
            bool success = false;
            if (still_running) {
                std::uint32_t completed_stage = 0;
                success = present(
                    runtime,
                    "AB",
                    checkpoint - completed_total,
                    recorder,
                    completed_stage,
                    error);
                completed_total += completed_stage;
                still_running = success;
                previous_error = error;
            }
            write_training_row(
                stream,
                label,
                "AB",
                checkpoint,
                completed_total,
                success,
                error,
                runtime,
                recorder);
        }
    };

    eve::ExperimentConfig full;
    run_variant("learning-and-growth", full);
    eve::ExperimentConfig learning_only;
    learning_only.growth_enabled = false;
    run_variant("learning-only", learning_only);
    eve::ExperimentConfig static_network;
    static_network.learning_enabled = false;
    static_network.growth_enabled = false;
    run_variant("static-control", static_network);
}

void experiment_b(const std::filesystem::path& output, const std::uint64_t seed) {
    std::ofstream stream = open_csv(output / "research-002-experiment-b.csv");
    stream << "stage,input,target_repetitions,completed_repetitions,success,error,synapses,"
              "processed_events,fire_events,phases,strengthenings,grown_synapses,"
              "weight_distribution,neural_state,neuron_state,weight_state,structure_state,"
              "stage_fires,unique_fired,fire_sequence_hash,fired_neurons,top_neuron,"
              "top_bigram,fire_preview\n";
    for (const std::string sequence : {"AB", "BA", "AA", "BB"}) {
        eve::bios::Runtime runtime({}, seed);
        ActivityRecorder recorder;
        std::uint32_t completed = 0;
        std::string error;
        const bool success = present(runtime, sequence, 8, recorder, completed, error);
        write_training_row(
            stream,
            "sequence-comparison",
            sequence,
            8,
            completed,
            success,
            error,
            runtime,
            recorder);
    }
}

void write_response_header(std::ostream& stream) {
    stream << "cohort,test_input,success,error,pre_neural_state,post_neural_state,"
              "pre_neuron_state,post_neuron_state,pre_weight_state,post_weight_state,"
              "pre_structure_state,post_structure_state,pre_synapses,post_synapses,"
              "events,fires,phases,strengthenings,grown_synapses,unique_fired,"
              "fire_sequence_hash,fired_neurons,top_neuron,top_bigram,fire_preview\n";
}

struct ResponseResult {
    eve::Metrics metrics;
    std::size_t unique_fired{};
    std::uint64_t sequence_hash{};
};

ResponseResult write_response(
    std::ostream& stream,
    const std::string& cohort,
    const std::string& input,
    eve::bios::Runtime& runtime) {
    const StateSummary before_state = summarize(runtime);
    const eve::Metrics before_metrics = runtime.metrics();
    const std::uint32_t before_synapses = runtime.network().synapse_count();
    ActivityRecorder recorder;
    std::uint32_t completed = 0;
    std::string error;
    const bool success = present(runtime, input, 1, recorder, completed, error);
    const StateSummary after_state = summarize(runtime);
    const eve::Metrics delta = difference(runtime.metrics(), before_metrics);

    stream << csv(cohort) << ',' << csv(input) << ',' << (success ? "yes" : "no") << ','
           << csv(error) << ',' << hex64(before_state.neural) << ','
           << hex64(after_state.neural) << ',' << hex64(before_state.neurons) << ','
           << hex64(after_state.neurons) << ',' << hex64(before_state.weights) << ','
           << hex64(after_state.weights) << ',' << hex64(before_state.structure) << ','
           << hex64(after_state.structure) << ',' << before_synapses << ','
           << runtime.network().synapse_count() << ',' << delta.processed_events << ','
           << delta.fire_events << ',' << delta.completed_activity_phases << ','
           << delta.synaptic_strengthenings << ',' << delta.grown_synapses << ','
           << recorder.unique_count() << ',' << hex64(recorder.hash()) << ','
           << csv(recorder.unique_neurons()) << ',' << csv(recorder.most_common_neuron()) << ','
           << csv(recorder.most_common_bigram()) << ',' << csv(recorder.preview()) << '\n';
    return ResponseResult{delta, recorder.unique_count(), recorder.hash()};
}

void write_unavailable_response(
    std::ostream& stream,
    const std::string& cohort,
    const std::string& input,
    const eve::bios::Runtime& runtime,
    const std::string& error) {
    const StateSummary state = summarize(runtime);
    stream << csv(cohort) << ',' << csv(input) << ",no," << csv(error) << ','
           << hex64(state.neural) << ',' << hex64(state.neural) << ','
           << hex64(state.neurons) << ',' << hex64(state.neurons) << ','
           << hex64(state.weights) << ',' << hex64(state.weights) << ','
           << hex64(state.structure) << ',' << hex64(state.structure) << ','
           << runtime.network().synapse_count() << ',' << runtime.network().synapse_count()
           << ",0,0,0,0,0,0," << hex64(kFnvOffset) << ",,,,\n";
}

void experiment_c(
    const std::filesystem::path& output,
    const std::filesystem::path& work,
    const std::uint64_t seed) {
    const std::filesystem::path trained_path = work / "recognition-trained.state";
    const std::filesystem::path control_path = work / "recognition-control.state";

    eve::bios::Runtime trained({}, seed);
    ActivityRecorder training_activity;
    std::uint32_t completed = 0;
    std::string error;
    if (!present(trained, "AB", 8, training_activity, completed, error)) {
        throw std::runtime_error("recognition training failed: " + error);
    }
    trained.save(trained_path.string());
    eve::bios::Runtime control({}, seed);
    control.save(control_path.string());

    std::ofstream stream = open_csv(output / "research-002-experiment-c.csv");
    write_response_header(stream);
    std::map<std::string, ResponseResult> trained_results;
    std::map<std::string, ResponseResult> control_results;
    for (const std::string input : {"AB", "BA", "AC", "AA", "BB"}) {
        eve::bios::Runtime trained_test = eve::bios::Runtime::load(trained_path.string());
        trained_results.emplace(
            input, write_response(stream, "trained-ABx8", input, trained_test));
        eve::bios::Runtime control_test = eve::bios::Runtime::load(control_path.string());
        control_results.emplace(
            input, write_response(stream, "untrained-control", input, control_test));
    }

    std::ofstream summary = open_csv(output / "research-002-recognition-summary.csv");
    summary << "metric,trained_known_ab,trained_other_mean,specificity_ratio,"
               "untrained_known_ab,trained_to_control_ratio\n";
    const std::vector<std::string> other_inputs{"BA", "AC", "AA", "BB"};
    const auto write_metric = [&](
                                  const std::string& name,
                                  const auto extract) {
        const double trained_known = extract(trained_results.at("AB"));
        double trained_other = 0;
        for (const std::string& input : other_inputs) {
            trained_other += extract(trained_results.at(input));
        }
        trained_other /= static_cast<double>(other_inputs.size());
        const double control_known = extract(control_results.at("AB"));
        const double specificity = trained_other > 0 ? trained_known / trained_other : 0;
        const double amplification = control_known > 0 ? trained_known / control_known : 0;
        summary << name << ',' << std::fixed << std::setprecision(6) << trained_known << ','
                << trained_other << ',' << specificity << ',' << control_known << ','
                << amplification << '\n';
    };
    write_metric("processed_events", [](const ResponseResult& result) {
        return static_cast<double>(result.metrics.processed_events);
    });
    write_metric("fire_events", [](const ResponseResult& result) {
        return static_cast<double>(result.metrics.fire_events);
    });
    write_metric("unique_fired_neurons", [](const ResponseResult& result) {
        return static_cast<double>(result.unique_fired);
    });
}

void experiment_d(
    const std::filesystem::path& output,
    const std::filesystem::path& work,
    const std::uint64_t seed) {
    const std::filesystem::path trained_path = work / "persistence-trained.state";
    const std::filesystem::path interference_path = work / "persistence-interference.state";
    eve::bios::Runtime trained({}, seed);
    ActivityRecorder training_activity;
    std::uint32_t completed = 0;
    std::string error;
    if (!present(trained, "AB", 8, training_activity, completed, error)) {
        throw std::runtime_error("persistence training failed: " + error);
    }
    trained.save(trained_path.string());

    eve::bios::Runtime interference = eve::bios::Runtime::load(trained_path.string());
    ActivityRecorder interference_activity;
    bool interference_success = true;
    std::uint32_t interference_completed = 0;
    for (const std::string input : {"CD", "EF", "GH"}) {
        std::uint32_t input_completed = 0;
        if (!present(
                interference,
                input,
                8,
                interference_activity,
                input_completed,
                error)) {
            interference_completed += input_completed;
            error = "after " + std::to_string(interference_completed) +
                    "/24 pattern presentations during " + input + ": " + error;
            interference_success = false;
            break;
        }
        interference_completed += input_completed;
    }
    if (interference_success) {
        interference.save(interference_path.string());
    }

    std::ofstream stream = open_csv(output / "research-002-experiment-d.csv");
    write_response_header(stream);
    eve::bios::Runtime immediate = eve::bios::Runtime::load(trained_path.string());
    static_cast<void>(
        write_response(stream, "immediately-after-AB-training", "AB", immediate));
    if (interference_success) {
        eve::bios::Runtime after_interference =
            eve::bios::Runtime::load(interference_path.string());
        static_cast<void>(write_response(stream, "after-CD-EF-GH", "AB", after_interference));
    } else {
        write_unavailable_response(
            stream,
            "after-CD-EF-GH",
            "AB",
            interference,
            "interference phase aborted: " + error);
    }
}

void experiment_e(const std::filesystem::path& output) {
    std::ofstream stream = open_csv(output / "research-002-experiment-e.csv");
    stream << "run,seed,training_success,training_error,training_completed,trained_neural_state,"
              "trained_weight_state,trained_structure_state,test_events,test_fires,"
              "test_strengthenings,test_grown_synapses,test_fire_sequence_hash,"
              "test_fired_neurons,test_top_bigram\n";
    const std::vector<std::pair<std::string, std::uint64_t>> runs{
        {"same-seed-1", 20261117},
        {"same-seed-2", 20261117},
        {"different-seed-7", 7},
        {"different-seed-42", 42},
        {"different-seed-99", 99},
    };
    for (const auto& [label, seed] : runs) {
        eve::bios::Runtime runtime({}, seed);
        ActivityRecorder training_activity;
        std::uint32_t completed = 0;
        std::string error;
        const bool success = present(runtime, "AB", 8, training_activity, completed, error);
        const StateSummary trained_state = summarize(runtime);
        const eve::Metrics before = runtime.metrics();
        ActivityRecorder test_activity;
        std::uint32_t test_completed = 0;
        std::string test_error;
        const bool test_success =
            success && present(runtime, "AB", 1, test_activity, test_completed, test_error);
        const eve::Metrics delta = difference(runtime.metrics(), before);
        if (!test_success && error.empty()) {
            error = test_error;
        }
        stream << csv(label) << ',' << seed << ',' << (success ? "yes" : "no") << ','
               << csv(error) << ',' << completed << ',' << hex64(trained_state.neural) << ','
               << hex64(trained_state.weights) << ',' << hex64(trained_state.structure) << ','
               << delta.processed_events << ',' << delta.fire_events << ','
               << delta.synaptic_strengthenings << ',' << delta.grown_synapses << ','
               << hex64(test_activity.hash()) << ',' << csv(test_activity.unique_neurons()) << ','
               << csv(test_activity.most_common_bigram()) << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path output =
            argc >= 2 ? std::filesystem::path(argv[1])
                      : std::filesystem::path("research/results");
        if (argc > 2) {
            throw std::invalid_argument("usage: eve_research_002 [output-directory]");
        }
        std::filesystem::create_directories(output);
        const std::filesystem::path work = output / ".research-002-state-work";
        std::filesystem::create_directories(work);
        constexpr std::uint64_t seed = 20261117;

        experiment_a(output, seed);
        experiment_b(output, seed);
        experiment_c(output, work, seed);
        experiment_d(output, work, seed);
        experiment_e(output);
        std::filesystem::remove_all(work);
        std::cout << "Wrote Research 002 results to " << output << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
