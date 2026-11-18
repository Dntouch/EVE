#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace {

struct Variant {
    std::string name;
    eve::PlasticityTraceMode trace;
    eve::ModulationMode modulation;
    std::uint64_t delay{};
    bool reverse{};
};

struct Aggregate {
    std::vector<double> early;
    std::vector<double> evaluation;
    std::uint64_t positive_changes{};
    std::uint64_t negative_changes{};
    std::uint64_t zero_to_one{};
};

eve::ExperimentConfig experiment_config(const Variant& variant) {
    eve::ExperimentConfig config;
    config.neuron_count = 64;
    config.width = 4;
    config.height = 4;
    config.depth = 4;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 1;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 32;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::all_recent;
    config.eligibility_representation = eve::EligibilityRepresentation::unique;
    config.eligibility_history = eve::EligibilityHistory::retain;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = variant.trace;
    config.plasticity_decay_mode = eve::TraceDecayMode::constant;
    config.modulation_mode = variant.modulation;
    config.plasticity_positive_step = 1;
    config.plasticity_negative_step = 1;
    config.plasticity_maximum_weight = 3;
    return config;
}

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

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b4");
    std::filesystem::create_directories(output);
    std::ofstream trials(output / "raw_trials.csv");
    trials << "variant,seed,phase,episode,cue,action_a,action_b,outcome,reward,"
              "positive_changes,negative_changes,zero_to_one\n";

    const std::vector<Variant> variants{
        {"K0_no_plasticity", eve::PlasticityTraceMode::disabled,
         eve::ModulationMode::none, 0, false},
        {"K1_unmodulated_old_learning", eve::PlasticityTraceMode::disabled,
         eve::ModulationMode::none, 0, false},
        {"K2_correct_feedback", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K3_random_modulation", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K4_reversed_feedback", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K5_time_shifted", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 16, false},
        {"K6_modulation_without_trace", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K7_trace_without_modulation", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::none, 0, false},
        {"K8_feedback_without_action", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K9_feedback_created_traces", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K10_zero_synapses", eve::PlasticityTraceMode::membrane_participation,
         eve::ModulationMode::ternary, 0, false},
        {"K11_resource_run", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K12_no_exploration", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K13_identical_start", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"K14_absent_feedback", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::none, 0, false},
        {"K15_repeated_inputs", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"E1_G3", eve::PlasticityTraceMode::g3_recent,
         eve::ModulationMode::ternary, 0, false},
        {"E2_pre_post", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, false},
        {"E3_membrane", eve::PlasticityTraceMode::membrane_participation,
         eve::ModulationMode::ternary, 0, false},
        {"reward_reversal", eve::PlasticityTraceMode::pre_post_fire,
         eve::ModulationMode::ternary, 0, true},
    };
    std::map<std::string, Aggregate> aggregates;

    for (const Variant& variant : variants) {
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            auto config = experiment_config(variant);
            if (variant.name == "K1_unmodulated_old_learning") {
                config.learning_enabled = true;
            }
            eve::bios::Runtime runtime(config, 10000 + seed);
            eve::brainstem::EvaluationRouteConfig route_config;
            route_config.mode = eve::brainstem::EvaluationRouteMode::general_wiring;
            route_config.first_input_neuron = 9;
            route_config.graded_channel_count = 5;
            route_config.fanout_per_source = 1;
            route_config.weight = 1;
            route_config.seed = 20000 + seed;
            for (eve::NeuronId id = 0; id < 9; ++id) {
                route_config.reserved_neurons.push_back(id);
            }
            const auto route = runtime.configure_evaluation_route(route_config);
            const eve::NeuronId action_a = route.target_neurons.at(0);
            const eve::NeuronId action_b = route.target_neurons.at(1);
            for (eve::NeuronId source = 0; source < 8; ++source) {
                const bool prefers_a = ((source + seed) % 2U) == 0;
                static_cast<void>(runtime.add_experimental_synapse(
                    source, action_a, prefers_a ? 1 : 0));
                static_cast<void>(runtime.add_experimental_synapse(
                    source, action_b, prefers_a ? 0 : 1));
            }
            bool fired_a = false;
            bool fired_b = false;
            runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
                fired_a = fired_a || event.neuron == action_a;
                fired_b = fired_b || event.neuron == action_b;
            });
            eve::bios::DeterministicRng environment_rng(30000 + seed);
            std::vector<double> early_rewards;
            std::vector<double> evaluation_rewards;
            for (std::uint32_t episode = 0; episode < 192; ++episode) {
                const bool evaluation = episode >= 128;
                const std::uint32_t cue = environment_rng.bounded(2);
                const eve::NeuronId source = variant.name == "K15_repeated_inputs"
                    ? static_cast<eve::NeuronId>(cue * 4U)
                    : static_cast<eve::NeuronId>(cue * 4U + environment_rng.bounded(4));
                fired_a = false;
                fired_b = false;
                const bool suppress_action =
                    variant.name == "K8_feedback_without_action" ||
                    variant.name == "K9_feedback_created_traces" ||
                    variant.name == "K12_no_exploration" ||
                    variant.name == "K6_modulation_without_trace";
                if (!suppress_action) {
                    runtime.enqueue_fire(source);
                    static_cast<void>(runtime.run());
                }
                const bool reversed = variant.reverse && episode >= 64;
                const bool want_a = (cue == 0) != reversed;
                const bool correct = want_a ? (fired_a && !fired_b)
                                            : (fired_b && !fired_a);
                const int reward = correct ? 1 : -1;
                std::string outcome = fired_a && fired_b ? "both" :
                    fired_a ? "A" : fired_b ? "B" : "none";
                if (!evaluation && variant.modulation != eve::ModulationMode::none) {
                    std::int8_t polarity = static_cast<std::int8_t>(reward);
                    if (variant.name == "K3_random_modulation") {
                        polarity = environment_rng.bounded(2) == 0 ? -1 : 1;
                    } else if (variant.name == "K4_reversed_feedback") {
                        polarity = static_cast<std::int8_t>(-polarity);
                    }
                    const std::uint64_t due = runtime.simulated_time() + variant.delay;
                    static_cast<void>(runtime.process_modulatory_feedback(polarity, due));
                    if (variant.delay != 0) {
                        static_cast<void>(runtime.advance_time_to(due));
                    }
                }
                if (episode < 32) early_rewards.push_back(reward);
                if (evaluation) evaluation_rewards.push_back(reward);
                const auto& metrics = runtime.metrics();
                trials << variant.name << ',' << seed << ','
                       << (evaluation ? "evaluation" : "training") << ','
                       << episode << ',' << cue << ',' << (fired_a ? 1 : 0) << ','
                       << (fired_b ? 1 : 0) << ',' << outcome << ',' << reward << ','
                       << metrics.plasticity_positive_changes << ','
                       << metrics.plasticity_negative_changes << ','
                       << metrics.plasticity_zero_to_one_changes << '\n';
            }
            Aggregate& aggregate = aggregates[variant.name];
            aggregate.early.push_back(mean(early_rewards));
            aggregate.evaluation.push_back(mean(evaluation_rewards));
            aggregate.positive_changes += runtime.metrics().plasticity_positive_changes;
            aggregate.negative_changes += runtime.metrics().plasticity_negative_changes;
            aggregate.zero_to_one += runtime.metrics().plasticity_zero_to_one_changes;
        }
    }

    std::ofstream summary(output / "summary.csv");
    summary << "variant,seeds,early_reward_mean,evaluation_reward_mean,delta_mean,"
               "delta_median,learning_seed_fraction,positive_changes,negative_changes,"
               "zero_to_one\n";
    summary << std::fixed << std::setprecision(6);
    for (const auto& [name, data] : aggregates) {
        std::vector<double> deltas;
        std::size_t learning = 0;
        for (std::size_t index = 0; index < data.early.size(); ++index) {
            const double delta = data.evaluation[index] - data.early[index];
            deltas.push_back(delta);
            if (delta > 0.0) ++learning;
        }
        summary << name << ',' << data.early.size() << ',' << mean(data.early) << ','
                << mean(data.evaluation) << ',' << mean(deltas) << ','
                << median(deltas) << ','
                << static_cast<double>(learning) / data.early.size() << ','
                << data.positive_changes << ',' << data.negative_changes << ','
                << data.zero_to_one << '\n';
    }

    std::ofstream resources(output / "resources.csv");
    resources << "neurons,estimated_bytes,bytes_per_neuron,projection_basis\n";
    const Variant reference{"resource", eve::PlasticityTraceMode::pre_post_fire,
                            eve::ModulationMode::ternary, 0, false};
    eve::bios::Runtime sample(experiment_config(reference), 1);
    const auto bytes = sample.diagnostics().estimated_memory_bytes;
    const double per_neuron = static_cast<double>(bytes) / 64.0;
    resources << "64," << bytes << ',' << per_neuron << ",measured\n";
    for (const std::uint64_t neurons : {256ULL, 10000ULL, 100000ULL, 1000000ULL}) {
        resources << neurons << ',' << static_cast<std::uint64_t>(per_neuron * neurons)
                  << ',' << per_neuron << ",linear_estimate\n";
    }
    std::cout << "B.4 raw data written to " << output << '\n';
}
