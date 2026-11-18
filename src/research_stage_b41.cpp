#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <limits>
#include <numeric>
#include <string>
#include <vector>
#include <sys/resource.h>

namespace {

enum class FeedbackPolicy { none, assessment, correct, random, reversed };

struct Variant {
    std::string name;
    eve::PlasticityTraceMode trace{eve::PlasticityTraceMode::disabled};
    FeedbackPolicy feedback{FeedbackPolicy::assessment};
    std::uint64_t episode_gap{16};
    std::uint64_t feedback_delay{1};
    std::uint64_t trace_duration{16};
    std::uint32_t exploration_percent{60};
    bool suppress_action{};
    bool old_learning{};
    bool repeated_input{};
    std::uint64_t input_budget{32};
    std::uint32_t reversal_episode{std::numeric_limits<std::uint32_t>::max()};
};

struct EpisodePlan {
    std::uint8_t cue{};
    std::uint8_t exploration_roll{};
    std::uint8_t exploration_action{};
    std::int8_t random_feedback{};
};

struct SeedResult {
    double early_reward{};
    double evaluation_reward{};
    std::uint64_t correct{};
    std::uint64_t action_a{};
    std::uint64_t action_b{};
    std::uint64_t both{};
    std::uint64_t none{};
    std::uint64_t positive_changes{};
    std::uint64_t negative_changes{};
    std::uint64_t zero_to_one{};
    std::uint64_t expired_traces{};
    std::uint64_t consumed_traces{};
    std::uint64_t overwritten_traces{};
    std::uint64_t maximum_queue{};
    std::int32_t cue_a_to_a{};
    std::int32_t cue_a_to_b{};
    std::int32_t cue_b_to_a{};
    std::int32_t cue_b_to_b{};
    std::uint64_t processed_events{};
};

double mean(const std::vector<double>& values) {
    return values.empty() ? 0.0 :
        std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double median(std::vector<double> values) {
    if (values.empty()) return 0.0;
    std::sort(values.begin(), values.end());
    const std::size_t middle = values.size() / 2;
    return values.size() % 2 == 0
        ? (values[middle - 1] + values[middle]) / 2.0 : values[middle];
}

std::pair<double, double> bootstrap_interval(
    const std::vector<double>& paired_differences,
    const std::uint64_t seed) {
    if (paired_differences.empty()) return {0.0, 0.0};
    eve::bios::DeterministicRng rng(seed);
    std::vector<double> samples;
    samples.reserve(2000);
    for (std::size_t repetition = 0; repetition < 2000; ++repetition) {
        double sum = 0.0;
        for (std::size_t draw = 0; draw < paired_differences.size(); ++draw) {
            sum += paired_differences[rng.bounded(
                static_cast<std::uint32_t>(paired_differences.size()))];
        }
        samples.push_back(sum / paired_differences.size());
    }
    std::sort(samples.begin(), samples.end());
    return {samples[49], samples[1949]};
}

std::vector<EpisodePlan> make_plan(
    const std::uint64_t seed, const std::size_t episodes) {
    eve::bios::DeterministicRng tasks(0x100000ULL + seed);
    eve::bios::DeterministicRng exploration(0x200000ULL + seed);
    eve::bios::DeterministicRng feedback(0x300000ULL + seed);
    std::vector<EpisodePlan> plan;
    plan.reserve(episodes);
    for (std::size_t index = 0; index < episodes; ++index) {
        plan.push_back(EpisodePlan{
            static_cast<std::uint8_t>(tasks.bounded(2)),
            static_cast<std::uint8_t>(exploration.bounded(100)),
            static_cast<std::uint8_t>(exploration.bounded(2)),
            static_cast<std::int8_t>(feedback.bounded(2) == 0 ? -1 : 1),
        });
    }
    return plan;
}

eve::ExperimentConfig make_config(const Variant& variant, const bool large) {
    eve::ExperimentConfig config;
    config.neuron_count = large ? 256 : 64;
    config.width = large ? 8 : 4;
    config.height = large ? 8 : 4;
    config.depth = 4;
    config.initial_outgoing_synapses = large ? 4 : 0;
    config.initial_synapse_weight = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = variant.input_budget;
    config.learning_enabled = variant.old_learning;
    config.growth_enabled = false;
    config.refractory_input_events = 1;
    config.recovery_mode = eve::RecoveryMode::independent_with_membrane_relaxation;
    config.recovery_delay = 4;
    config.membrane_relaxation_percent = 100;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::all_recent;
    config.eligibility_representation = eve::EligibilityRepresentation::unique;
    config.eligibility_history = eve::EligibilityHistory::retain;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = variant.trace_duration;
    config.plasticity_trace_mode = variant.trace;
    config.plasticity_decay_mode = eve::TraceDecayMode::constant;
    config.modulation_mode = variant.trace == eve::PlasticityTraceMode::disabled
        ? eve::ModulationMode::none : eve::ModulationMode::ternary;
    config.plasticity_positive_step = 1;
    config.plasticity_negative_step = 1;
    config.plasticity_maximum_weight = 3;
    return config;
}

eve::SynapseId ensure_synapse(
    eve::bios::Runtime& runtime, const eve::NeuronId sender,
    const eve::NeuronId receiver, const std::int32_t weight) {
    const auto existing = runtime.network().find_synapse(sender, receiver);
    if (existing.has_value()) {
        runtime.set_experimental_synapse_weight(*existing, weight);
        return *existing;
    }
    return runtime.add_experimental_synapse(sender, receiver, weight);
}

struct Circuit {
    eve::NeuronId action_a{7};
    eve::NeuronId action_b{8};
    std::array<eve::SynapseId, 4> cue_synapses{};
    eve::brainstem::EvaluationRouteResult route;
};

Circuit configure_circuit(
    eve::bios::Runtime& runtime, const std::uint64_t seed, const bool large) {
    Circuit circuit;
    if (large) {
        eve::brainstem::MotorOutputConfig motor;
        motor.response_stimulus_neuron = 8;
        motor.first_motor_neuron = 16;
        motor.contacts_per_channel = 1;
        motor.assignment_seed = 0x400000ULL + seed;
        runtime.configure_motor_output(motor);
        eve::brainstem::InnateWiringConfig innate;
        innate.topology = eve::brainstem::InnateTopology::functional_paths;
        innate.initial_weight = 2;
        innate.topology_seed = 0x500000ULL + seed;
        static_cast<void>(runtime.configure_innate_wiring(innate));
        circuit.action_a = runtime.motor_assignment().channels[0][0];
        circuit.action_b = runtime.motor_assignment().channels[1][0];
    }

    eve::brainstem::EvaluationRouteConfig route;
    route.mode = eve::brainstem::EvaluationRouteMode::general_wiring;
    route.first_input_neuron = 9;
    route.graded_channel_count = 5;
    route.fanout_per_source = 1;
    route.weight = 2;
    route.seed = 0x600000ULL + seed;
    for (eve::NeuronId neuron = 0; neuron < 16; ++neuron) {
        route.reserved_neurons.push_back(neuron);
    }
    route.reserved_neurons.push_back(circuit.action_a);
    route.reserved_neurons.push_back(circuit.action_b);
    circuit.route = runtime.configure_evaluation_route(route);

    // Modulation reaches a separate zero-weight contact.  It can update local
    // traces but cannot itself make the motor neuron fire.
    for (const eve::NeuronId input : circuit.route.input_neurons) {
        static_cast<void>(ensure_synapse(runtime, input, circuit.action_a, 0));
        static_cast<void>(ensure_synapse(runtime, input, circuit.action_b, 0));
    }
    circuit.cue_synapses = {
        ensure_synapse(runtime, 0, circuit.action_a, 0),
        ensure_synapse(runtime, 0, circuit.action_b, 0),
        ensure_synapse(runtime, 1, circuit.action_a, 0),
        ensure_synapse(runtime, 1, circuit.action_b, 0),
    };
    static_cast<void>(ensure_synapse(runtime, 2, circuit.action_a, 2));
    static_cast<void>(ensure_synapse(runtime, 3, circuit.action_a, 2));
    static_cast<void>(ensure_synapse(runtime, 4, circuit.action_b, 2));
    static_cast<void>(ensure_synapse(runtime, 5, circuit.action_b, 2));
    return circuit;
}

SeedResult run_seed(
    const Variant& variant,
    const std::uint64_t seed,
    const std::uint32_t training_episodes,
    const std::uint32_t evaluation_episodes,
    const bool large,
    std::ofstream* trials,
    std::ofstream* timing,
    std::ofstream* changes) {
    const auto plans = make_plan(seed, training_episodes + evaluation_episodes);
    eve::bios::Runtime runtime(make_config(variant, large), 0x700000ULL + seed);
    const Circuit circuit = configure_circuit(runtime, seed, large);
    bool fired_a = false;
    bool fired_b = false;
    std::uint64_t first_motor_time = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        if (event.neuron == circuit.action_a || event.neuron == circuit.action_b) {
            if (!fired_a && !fired_b) first_motor_time = event.simulated_time;
            fired_a = fired_a || event.neuron == circuit.action_a;
            fired_b = fired_b || event.neuron == circuit.action_b;
        }
    });

    SeedResult output;
    std::vector<double> early;
    std::vector<double> evaluation;
    std::uint64_t episode_start = 0;
    for (std::uint32_t episode = 0;
         episode < training_episodes + evaluation_episodes; ++episode) {
        const bool evaluating = episode >= training_episodes;
        if (runtime.simulated_time() < episode_start) {
            static_cast<void>(runtime.advance_time_to(episode_start));
        }
        const auto before_metrics = runtime.metrics();
        const auto weights_before = runtime.network().synapses();
        const std::uint64_t start_time = runtime.simulated_time();
        const std::size_t traces_at_start = runtime.network().active_eligibility_count();
        fired_a = false;
        fired_b = false;
        first_motor_time = 0;
        const auto& plan = plans[episode];
        const eve::NeuronId cue = variant.repeated_input ? 0 : plan.cue;
        if (!variant.suppress_action) {
            runtime.enqueue_fire(cue);
            static_cast<void>(runtime.run());
            if (!fired_a && !fired_b &&
                plan.exploration_roll < variant.exploration_percent) {
                const eve::NeuronId exploratory = plan.exploration_action == 0
                    ? static_cast<eve::NeuronId>(2 + (episode & 1U))
                    : static_cast<eve::NeuronId>(4 + (episode & 1U));
                runtime.enqueue_fire(exploratory);
                static_cast<void>(runtime.run());
            }
        }
        const std::uint64_t after_action_time = runtime.simulated_time();
        const bool reversed = episode >= variant.reversal_episode;
        const bool want_a = (cue == 0) != reversed;
        const bool correct = want_a ? fired_a && !fired_b : fired_b && !fired_a;
        const int reward = correct ? 1 : -1;
        const std::string outcome = fired_a && fired_b ? "both" :
            fired_a ? "A" : fired_b ? "B" : "none";

        std::uint64_t feedback_time = runtime.simulated_time();
        if (evaluating || variant.feedback == FeedbackPolicy::assessment) {
            static_cast<void>(runtime.process_evaluation_feedback(50));
            feedback_time = runtime.simulated_time();
        } else if (variant.feedback != FeedbackPolicy::none) {
            std::int8_t polarity = static_cast<std::int8_t>(reward);
            if (variant.feedback == FeedbackPolicy::random) {
                polarity = plan.random_feedback;
            } else if (variant.feedback == FeedbackPolicy::reversed) {
                polarity = static_cast<std::int8_t>(-polarity);
            }
            const std::uint64_t due = runtime.simulated_time() + variant.feedback_delay;
            static_cast<void>(runtime.process_modulatory_feedback(polarity, due));
            if (runtime.simulated_time() < due) {
                static_cast<void>(runtime.advance_time_to(due));
            }
            feedback_time = due;
        }

        if (variant.episode_gap != 0) {
            const std::uint64_t next = episode_start + variant.episode_gap;
            if (runtime.simulated_time() < next) {
                static_cast<void>(runtime.advance_time_to(next));
            }
            episode_start = next;
        } else {
            episode_start = runtime.simulated_time();
        }

        const auto& after_metrics = runtime.metrics();
        const std::size_t traces_at_end = runtime.network().active_eligibility_count();
        if (episode < std::min<std::uint32_t>(32, training_episodes)) {
            early.push_back(reward);
        }
        if (evaluating) {
            evaluation.push_back(reward);
            output.correct += correct ? 1U : 0U;
            if (fired_a && fired_b) ++output.both;
            else if (fired_a) ++output.action_a;
            else if (fired_b) ++output.action_b;
            else ++output.none;
        }
        if (trials != nullptr) {
            *trials << variant.name << ',' << seed << ','
                    << (large ? 256 : 64) << ','
                    << (evaluating ? "evaluation" : "training") << ','
                    << episode << ',' << static_cast<unsigned>(cue) << ','
                    << static_cast<unsigned>(plan.exploration_roll) << ','
                    << static_cast<unsigned>(plan.exploration_action) << ','
                    << outcome << ',' << reward << ',' << (reversed ? 1 : 0) << ','
                    << start_time << ','
                    << after_action_time << ',' << feedback_time << ','
                    << runtime.simulated_time() << ',' << traces_at_start << ','
                    << traces_at_end << ','
                    << (after_metrics.processed_eligibility_expiries -
                        before_metrics.processed_eligibility_expiries) << ','
                    << (after_metrics.plasticity_consumed_traces -
                        before_metrics.plasticity_consumed_traces) << ','
                    << (after_metrics.eligibility_overwritten_decisions -
                        before_metrics.eligibility_overwritten_decisions) << '\n';
        }
        if (timing != nullptr && seed == 1) {
            *timing << variant.name << ',' << episode << ',' << start_time << ','
                    << after_action_time << ',' << first_motor_time << ','
                    << feedback_time << ',' << runtime.simulated_time() << ','
                    << traces_at_start << ',' << traces_at_end << ','
                    << runtime.diagnostics().ready_events << ','
                    << runtime.diagnostics().future_events << '\n';
        }
        if (changes != nullptr && !evaluating) {
            const auto& after_weights = runtime.network().synapses();
            for (eve::SynapseId id = 0; id < after_weights.size(); ++id) {
                if (weights_before[id].weight == after_weights[id].weight) continue;
                const bool motor = after_weights[id].receiver == circuit.action_a ||
                                   after_weights[id].receiver == circuit.action_b;
                *changes << variant.name << ',' << seed << ',' << episode << ','
                         << runtime.simulated_time() << ',' << id << ','
                         << after_weights[id].sender << ',' << after_weights[id].receiver
                         << ',' << weights_before[id].weight << ','
                         << after_weights[id].weight << ',' << (motor ? 1 : 0) << '\n';
            }
        }
    }
    output.early_reward = mean(early);
    output.evaluation_reward = mean(evaluation);
    output.positive_changes = runtime.metrics().plasticity_positive_changes;
    output.negative_changes = runtime.metrics().plasticity_negative_changes;
    output.zero_to_one = runtime.metrics().plasticity_zero_to_one_changes;
    output.expired_traces = runtime.metrics().processed_eligibility_expiries;
    output.consumed_traces = runtime.metrics().plasticity_consumed_traces;
    output.overwritten_traces = runtime.metrics().eligibility_overwritten_decisions;
    output.maximum_queue = runtime.metrics().maximum_queue_size;
    output.cue_a_to_a = runtime.network().synapse(circuit.cue_synapses[0]).weight;
    output.cue_a_to_b = runtime.network().synapse(circuit.cue_synapses[1]).weight;
    output.cue_b_to_a = runtime.network().synapse(circuit.cue_synapses[2]).weight;
    output.cue_b_to_b = runtime.network().synapse(circuit.cue_synapses[3]).weight;
    output.processed_events = runtime.metrics().processed_events;
    return output;
}

std::vector<Variant> variants() {
    using T = eve::PlasticityTraceMode;
    std::vector<Variant> result{
        {"K0_no_plasticity", T::disabled, FeedbackPolicy::assessment},
        {"K1_old_learning", T::disabled, FeedbackPolicy::assessment, 16, 1, 16, 60,
         false, true},
        {"K2_correct_E1", T::g3_recent, FeedbackPolicy::correct},
        {"K3_random_feedback", T::g3_recent, FeedbackPolicy::random},
        {"K4_reversed_feedback", T::g3_recent, FeedbackPolicy::reversed},
        {"K5_delayed_feedback", T::g3_recent, FeedbackPolicy::correct, 16, 16},
        {"K6_modulation_without_trace", T::g3_recent, FeedbackPolicy::correct,
         16, 1, 16, 60, true},
        {"K7_trace_without_modulation", T::g3_recent, FeedbackPolicy::none},
        {"K8_feedback_without_action", T::g3_recent, FeedbackPolicy::correct,
         16, 1, 16, 60, true},
        {"K9_feedback_created_traces", T::pre_post_fire, FeedbackPolicy::correct,
         16, 1, 16, 60, true},
        {"K10_zero_weight", T::membrane_participation, FeedbackPolicy::correct},
        {"K11_budget_4", T::g3_recent, FeedbackPolicy::correct,
         16, 1, 16, 60, false, false, false, 4},
        {"K12_sensor_without_exploration", T::g3_recent, FeedbackPolicy::correct,
         16, 1, 16, 0},
        {"K13_identical_start", T::g3_recent, FeedbackPolicy::correct},
        {"K14_absent_feedback", T::g3_recent, FeedbackPolicy::none},
        {"K15_repeated_input", T::g3_recent, FeedbackPolicy::correct,
         16, 1, 16, 60, false, false, true},
        {"K16_no_time_progress", T::g3_recent, FeedbackPolicy::correct, 0, 0},
        {"K17_correct_time", T::g3_recent, FeedbackPolicy::correct},
        {"K18_gap_64", T::g3_recent, FeedbackPolicy::correct, 64},
        {"K19_trace_4", T::g3_recent, FeedbackPolicy::correct, 16, 1, 4},
        {"K20_256_neurons", T::g3_recent, FeedbackPolicy::correct},
        {"K21_separate_contacts", T::g3_recent, FeedbackPolicy::correct},
        {"K22_assessment_no_change", T::g3_recent, FeedbackPolicy::assessment},
        {"E1_G3", T::g3_recent, FeedbackPolicy::correct},
        {"E2_pre_post", T::pre_post_fire, FeedbackPolicy::correct},
        {"E3_membrane", T::membrane_participation, FeedbackPolicy::correct},
        {"X0_none", T::g3_recent, FeedbackPolicy::correct, 16, 1, 16, 0},
        {"X1_low", T::g3_recent, FeedbackPolicy::correct, 16, 1, 16, 25},
        {"X2_medium", T::g3_recent, FeedbackPolicy::correct, 16, 1, 16, 60},
        {"X3_E4_new", T::g3_recent, FeedbackPolicy::correct, 16, 1, 16, 90},
    };
    Variant reversal{"environment_reversal", T::g3_recent,
                     FeedbackPolicy::correct};
    reversal.reversal_episode = 64;
    result.push_back(reversal);
    return result;
}

void run_causal_probe(const std::uint64_t seed, std::ofstream& output) {
    const Variant variant{"causal_probe", eve::PlasticityTraceMode::g3_recent,
                          FeedbackPolicy::correct};
    eve::bios::Runtime runtime(make_config(variant, false), 0x900000ULL + seed);
    const Circuit circuit = configure_circuit(runtime, seed, false);
    bool action_a = false;
    bool action_b = false;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        action_a = action_a || event.neuron == circuit.action_a;
        action_b = action_b || event.neuron == circuit.action_b;
    });

    action_a = action_b = false;
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    const bool reachable_before = action_a && !action_b;
    runtime.enqueue_fire(2);
    static_cast<void>(runtime.run());
    static_cast<void>(runtime.process_modulatory_feedback(1));
    static_cast<void>(runtime.advance_time_by(16));

    action_a = action_b = false;
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    if (!action_a && !action_b) {
        runtime.enqueue_fire(2);
        static_cast<void>(runtime.run());
    }
    static_cast<void>(runtime.process_modulatory_feedback(1));
    static_cast<void>(runtime.advance_time_by(16));

    action_a = action_b = false;
    runtime.enqueue_fire(0);
    static_cast<void>(runtime.run());
    const bool reachable_after = action_a && !action_b;
    const std::int32_t learned_weight =
        runtime.network().synapse(circuit.cue_synapses[0]).weight;
    static_cast<void>(runtime.advance_time_by(16));

    const auto path = std::filesystem::temp_directory_path() /
        ("eve-b41-causal-" + std::to_string(seed) + ".bin");
    runtime.save(path.string());
    auto ablated = eve::bios::Runtime::load(path.string());
    ablated.set_experimental_synapse_weight(circuit.cue_synapses[0], 0);
    bool ablated_a = false;
    bool ablated_b = false;
    ablated.set_fire_observer([&](const eve::bios::FireObservation& event) {
        ablated_a = ablated_a || event.neuron == circuit.action_a;
        ablated_b = ablated_b || event.neuron == circuit.action_b;
    });
    ablated.enqueue_fire(0);
    static_cast<void>(ablated.run());
    static_cast<void>(std::filesystem::remove(path));
    output << seed << ',' << (reachable_before ? 1 : 0) << ',' << learned_weight
           << ',' << (reachable_after ? 1 : 0) << ',' << (ablated_a ? 1 : 0)
           << ',' << (ablated_b ? 1 : 0) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b41");
    std::filesystem::create_directories(output);

    std::ofstream tasks(output / "tasks.csv");
    tasks << "seed,episode,cue,exploration_roll,exploration_action,random_feedback,"
             "task_seed,exploration_seed,feedback_seed\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        const auto plan = make_plan(seed, 192);
        for (std::size_t episode = 0; episode < plan.size(); ++episode) {
            tasks << seed << ',' << episode << ','
                  << static_cast<unsigned>(plan[episode].cue) << ','
                  << static_cast<unsigned>(plan[episode].exploration_roll) << ','
                  << static_cast<unsigned>(plan[episode].exploration_action) << ','
                  << static_cast<int>(plan[episode].random_feedback) << ','
                  << (0x100000ULL + seed) << ',' << (0x200000ULL + seed) << ','
                  << (0x300000ULL + seed) << '\n';
        }
    }

    std::ofstream trials(output / "trials.csv");
    trials << "variant,seed,neurons,phase,episode,cue,exploration_roll,"
              "exploration_action,outcome,reward,reversed,start_time,action_time,feedback_time,"
              "end_time,traces_start,traces_end,expired,consumed,overwritten\n";
    std::ofstream timing(output / "timing-audit.csv");
    timing << "variant,episode,start_time,action_time,first_motor_time,feedback_time,"
              "end_time,traces_start,traces_end,due_events,future_events\n";
    std::ofstream changes(output / "weight-changes.csv");
    changes << "variant,seed,episode,time,synapse,sender,receiver,before,after,motor_path\n";

    std::map<std::string, std::vector<SeedResult>> all_results;
    const auto matrix = variants();
    const auto begin = std::chrono::steady_clock::now();
    const std::clock_t cpu_begin = std::clock();
    for (const Variant& variant : matrix) {
        auto& results = all_results[variant.name];
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const bool large = variant.name == "K20_256_neurons";
            results.push_back(run_seed(
                variant, seed, 128, 64, large, &trials, &timing, &changes));
        }
    }

    std::ofstream summary(output / "summary.csv");
    summary << "variant,seeds,early_reward,evaluation_reward,delta,delta_median,"
               "delta_vs_K0,bootstrap_low,bootstrap_high,learning_seed_fraction,"
               "correct,A,B,both,none,positive_changes,negative_changes,zero_to_one,"
               "expired,consumed,overwritten,maximum_queue\n";
    summary << std::fixed << std::setprecision(6);
    const auto& reference = all_results.at("K0_no_plasticity");
    for (const auto& [name, results] : all_results) {
        std::vector<double> early;
        std::vector<double> evaluation;
        std::vector<double> delta;
        std::vector<double> paired;
        std::uint64_t learning_seeds = 0;
        SeedResult totals;
        for (std::size_t index = 0; index < results.size(); ++index) {
            early.push_back(results[index].early_reward);
            evaluation.push_back(results[index].evaluation_reward);
            delta.push_back(results[index].evaluation_reward - results[index].early_reward);
            paired.push_back(results[index].evaluation_reward -
                             reference[index].evaluation_reward);
            if (delta.back() > 0) ++learning_seeds;
            totals.correct += results[index].correct;
            totals.action_a += results[index].action_a;
            totals.action_b += results[index].action_b;
            totals.both += results[index].both;
            totals.none += results[index].none;
            totals.positive_changes += results[index].positive_changes;
            totals.negative_changes += results[index].negative_changes;
            totals.zero_to_one += results[index].zero_to_one;
            totals.expired_traces += results[index].expired_traces;
            totals.consumed_traces += results[index].consumed_traces;
            totals.overwritten_traces += results[index].overwritten_traces;
            totals.maximum_queue = std::max(totals.maximum_queue,
                                            results[index].maximum_queue);
        }
        const auto interval = bootstrap_interval(paired, 0x800000ULL + name.size());
        summary << name << ',' << results.size() << ',' << mean(early) << ','
                << mean(evaluation) << ',' << mean(delta) << ',' << median(delta) << ','
                << mean(paired) << ',' << interval.first << ',' << interval.second << ','
                << static_cast<double>(learning_seeds) / results.size() << ','
                << totals.correct << ',' << totals.action_a << ',' << totals.action_b << ','
                << totals.both << ',' << totals.none << ',' << totals.positive_changes << ','
                << totals.negative_changes << ',' << totals.zero_to_one << ','
                << totals.expired_traces << ',' << totals.consumed_traces << ','
                << totals.overwritten_traces << ',' << totals.maximum_queue << '\n';
    }
    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "left,right,seeds,mean_difference,median_difference,bootstrap_low,"
              "bootstrap_high,positive_seed_fraction\n";
    paired << std::fixed << std::setprecision(6);
    const std::array<std::pair<std::string, std::string>, 5> comparisons{{
        {"E2_pre_post", "K0_no_plasticity"},
        {"E2_pre_post", "K3_random_feedback"},
        {"E2_pre_post", "K4_reversed_feedback"},
        {"E1_G3", "E2_pre_post"},
        {"E2_pre_post", "E3_membrane"},
    }};
    for (const auto& comparison : comparisons) {
        std::vector<double> differences;
        std::size_t positive = 0;
        const auto& left = all_results.at(comparison.first);
        const auto& right = all_results.at(comparison.second);
        for (std::size_t index = 0; index < left.size(); ++index) {
            differences.push_back(left[index].evaluation_reward -
                                  right[index].evaluation_reward);
            if (differences.back() > 0) ++positive;
        }
        const auto interval = bootstrap_interval(
            differences, 0xa00000ULL + comparison.first.size());
        paired << comparison.first << ',' << comparison.second << ','
               << differences.size() << ',' << mean(differences) << ','
               << median(differences) << ',' << interval.first << ','
               << interval.second << ','
               << static_cast<double>(positive) / differences.size() << '\n';
    }

    std::ofstream pathways(output / "motor-pathways.csv");
    pathways << "variant,seed,cue_a_to_a,cue_a_to_b,cue_b_to_a,cue_b_to_b\n";
    for (const auto& [name, results] : all_results) {
        for (std::size_t index = 0; index < results.size(); ++index) {
            pathways << name << ',' << index + 1 << ','
                     << results[index].cue_a_to_a << ',' << results[index].cue_a_to_b
                     << ',' << results[index].cue_b_to_a << ','
                     << results[index].cue_b_to_b << '\n';
        }
    }

    // Extended training is executed for every primary learning/control variant,
    // never selected by its short-run result.
    std::ofstream extended(output / "extended.csv");
    extended << "variant,seed,training_episodes,evaluation_reward,positive_changes,"
                "negative_changes,zero_to_one\n";
    const std::array<std::string, 6> extended_names{{
        "K0_no_plasticity", "E1_G3", "E2_pre_post", "E3_membrane",
        "K3_random_feedback", "K4_reversed_feedback"}};
    std::uint64_t extended_events = 0;
    std::uint64_t extended_episodes = 0;
    for (const std::string& name : extended_names) {
        const Variant& variant = *std::find_if(
            matrix.begin(), matrix.end(), [&](const Variant& item) {
                return item.name == name;
            });
        for (const std::uint32_t count : {512U, 2048U}) {
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                const auto result = run_seed(
                    variant, seed, count, 64, false, nullptr, nullptr, nullptr);
                extended_events += result.processed_events;
                extended_episodes += count + 64U;
                extended << name << ',' << seed << ',' << count << ','
                         << result.evaluation_reward << ',' << result.positive_changes
                         << ',' << result.negative_changes << ',' << result.zero_to_one
                         << '\n';
            }
        }
    }

    std::ofstream resources(output / "resources.csv");
    resources << "neurons,synapses,estimated_bytes,bytes_per_neuron,bytes_per_synapse,"
                 "elapsed_seconds,projection\n";
    for (const bool large : {false, true}) {
        const Variant reference_variant{"resource", eve::PlasticityTraceMode::g3_recent,
                                        FeedbackPolicy::correct};
        eve::bios::Runtime runtime(make_config(reference_variant, large), 1);
        static_cast<void>(configure_circuit(runtime, 1, large));
        const auto diagnostics = runtime.diagnostics();
        resources << diagnostics.neuron_count << ',' << diagnostics.synapse_count << ','
                  << diagnostics.estimated_memory_bytes << ','
                  << static_cast<double>(diagnostics.estimated_memory_bytes) /
                         diagnostics.neuron_count << ','
                  << static_cast<double>(diagnostics.estimated_memory_bytes) /
                         std::max(1U, diagnostics.synapse_count) << ",0,measured\n";
    }
    const double elapsed = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - begin).count();
    for (const std::uint64_t neurons : {10000ULL, 100000ULL, 1000000ULL}) {
        resources << neurons << ",0," << neurons * 870ULL << ",870,0," << elapsed
                  << ",linear_from_256_measurement\n";
    }
    std::ofstream causal(output / "causal-weight-effect.csv");
    causal << "seed,reachable_before,learned_weight,reachable_after,"
              "ablated_action_a,ablated_action_b\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        run_causal_probe(seed, causal);
    }
    std::ofstream reachability(output / "reachability.csv");
    reachability << "neurons,seed,action_a,action_b,a_reachable,b_reachable,"
                    "a_incoming,b_incoming,a_weight_sum,b_weight_sum\n";
    for (const bool large : {false, true}) {
        const Variant reference_variant{"reachability",
            eve::PlasticityTraceMode::disabled, FeedbackPolicy::assessment};
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            eve::bios::Runtime runtime(make_config(reference_variant, large),
                                       0xb00000ULL + seed);
            const Circuit circuit = configure_circuit(runtime, seed, large);
            bool a = false;
            bool b = false;
            runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
                a = a || event.neuron == circuit.action_a;
                b = b || event.neuron == circuit.action_b;
            });
            runtime.enqueue_fire(2);
            static_cast<void>(runtime.run());
            const bool a_reachable = a;
            static_cast<void>(runtime.advance_time_by(16));
            a = b = false;
            runtime.enqueue_fire(4);
            static_cast<void>(runtime.run());
            const bool b_reachable = b;
            std::uint64_t sum_a = 0;
            std::uint64_t sum_b = 0;
            for (const auto id : runtime.network().incoming_synapses(circuit.action_a)) {
                const auto weight = runtime.network().synapse(id).weight;
                if (weight > 0) sum_a += static_cast<std::uint64_t>(weight);
            }
            for (const auto id : runtime.network().incoming_synapses(circuit.action_b)) {
                const auto weight = runtime.network().synapse(id).weight;
                if (weight > 0) sum_b += static_cast<std::uint64_t>(weight);
            }
            reachability << (large ? 256 : 64) << ',' << seed << ','
                         << circuit.action_a << ',' << circuit.action_b << ','
                         << (a_reachable ? 1 : 0) << ',' << (b_reachable ? 1 : 0)
                         << ',' << runtime.network().neuron(circuit.action_a).incoming_count
                         << ',' << runtime.network().neuron(circuit.action_b).incoming_count
                         << ',' << sum_a << ',' << sum_b << '\n';
        }
    }
    std::uint64_t main_events = 0;
    for (const auto& [name, results] : all_results) {
        static_cast<void>(name);
        for (const auto& result : results) main_events += result.processed_events;
    }
    const double cpu_seconds = static_cast<double>(std::clock() - cpu_begin) /
                               CLOCKS_PER_SEC;
    struct rusage usage {};
    static_cast<void>(getrusage(RUSAGE_SELF, &usage));
    const std::uint64_t main_episodes =
        static_cast<std::uint64_t>(matrix.size()) * 64ULL * 192ULL;
    const std::uint64_t executed_episodes =
        main_episodes + extended_episodes + 64ULL * 3ULL + 128ULL * 2ULL;
    std::ofstream performance(output / "performance.csv");
    performance << "executed_episodes,main_events,extended_events,wall_seconds,cpu_seconds,"
                   "cpu_seconds_per_episode,main_events_per_cpu_second,peak_rss_kib\n";
    performance << executed_episodes << ',' << main_events << ',' << extended_events << ','
                << elapsed << ',' << cpu_seconds << ','
                << cpu_seconds / executed_episodes << ','
                << main_events / std::max(0.000001, cpu_seconds) << ','
                << usage.ru_maxrss << '\n';
    std::cout << "B.4.1 validation data written to " << output << '\n';
}
