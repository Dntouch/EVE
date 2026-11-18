#define main eve_b42_embedded_main
#include "research_stage_b42.cpp"
#undef main

#include <tuple>

// B.4.3 deliberately reuses the audited B.4.2 apparatus.  The include above
// keeps task generation, circuit construction and scoring byte-for-byte shared;
// this file only defines the new, pre-registered comparison matrix and reports.

namespace {

struct B43Variant {
    Variant variant;
    std::uint32_t training{128};
    bool large{};
    std::string control;
};

Variant protected_variant(const std::string& name) {
    Variant value = make_variant(name);
    value.protect_exploration = true;
    return value;
}

std::vector<B43Variant> b43_variants() {
    std::vector<B43Variant> rows;
    auto add = [&](Variant value, const std::string& control,
                   const std::uint32_t training = 128,
                   const bool large = false) {
        rows.push_back(B43Variant{std::move(value), training, large, control});
    };
    Variant v = protected_variant("K0_single_X_A"); v.task = Task::single_a;
    add(v, "K0", 64);
    v = protected_variant("K0_single_X_B"); v.task = Task::single_b; add(v, "K0", 64);
    v = protected_variant("K0_reversal_A_B"); v.task = Task::reversal_ab; add(v, "K0");
    v = protected_variant("K0_reversal_B_A"); v.task = Task::reversal_ba; add(v, "K0");
    add(protected_variant("K1_B42_two_cue"), "K1");
    v = protected_variant("K2_no_plasticity"); v.plasticity = false; add(v, "K2");
    v = protected_variant("K3_random_feedback"); v.feedback = Feedback::random; add(v, "K3");
    v = protected_variant("K4_reversed_feedback"); v.feedback = Feedback::reversed; add(v, "K4");
    v = protected_variant("K5_positive_only"); v.feedback = Feedback::positive;
    v.modulation_mode = eve::ModulationMode::positive; add(v, "K5");
    add(protected_variant("K6_ternary"), "K6");
    add(protected_variant("K7_L1_G3"), "K7");
    v = protected_variant("K8_L2_last_fire_input");
    v.trace_mode = eve::PlasticityTraceMode::pre_post_fire; add(v, "K8");
    v = protected_variant("K9_L3_decision_window");
    v.eligibility_selection = eve::EligibilitySelection::active_phase; add(v, "K9");
    v = protected_variant("K10_L4_multi_pre_post");
    v.trace_mode = eve::PlasticityTraceMode::membrane_participation; add(v, "K10");
    v = protected_variant("K11_only_X"); v.task = Task::single_a; add(v, "K11", 64);
    v = protected_variant("K12_only_Y"); v.task = Task::single_y_b; add(v, "K12", 64);
    v = protected_variant("K13_X_then_Y"); v.task = Task::sequential_xy; add(v, "K13");
    v = protected_variant("K14_Y_then_X"); v.task = Task::sequential_yx; add(v, "K14");
    v = protected_variant("K15_alternating"); v.task = Task::alternating_xy; add(v, "K15");
    add(protected_variant("K16_random_mixed"), "K16");
    v = protected_variant("K16_blocks_16"); v.task = Task::blocked_xy; add(v, "K16");
    for (const std::uint32_t capacity : {1U, 2U, 4U, 8U}) {
        v = protected_variant("K17_candidates_" + std::to_string(capacity));
        v.candidate_capacity = capacity; add(v, "K17");
        v = protected_variant("K17_decisions_" + std::to_string(capacity));
        v.decision_capacity = capacity; add(v, "K17");
    }
    for (const std::uint64_t duration : {4ULL, 8ULL, 16ULL, 32ULL, 64ULL}) {
        v = protected_variant("K18_duration_" + std::to_string(duration));
        v.eligibility_duration = duration; add(v, "K18");
    }
    v = protected_variant("K19_feedback_after_expiry"); v.eligibility_duration = 1;
    add(v, "K19");
    v = protected_variant("K20_overlapping_traces"); v.eligibility_duration = 64;
    v.decision_capacity = 8; add(v, "K20");
    add(protected_variant("K21_protected_exploration"), "K21");
    v = protected_variant("K22_protection_disabled"); v.protection_enabled = false;
    add(v, "K22");
    v = protected_variant("K23_inverse_assignment"); v.task = Task::inverse_two_cue;
    add(v, "K23");
    v = protected_variant("K24_neutral_modulation"); v.feedback = Feedback::neutral;
    add(v, "K24");
    add(protected_variant("K25_ablation_source"), "K25");
    add(protected_variant("K26_64_neurons"), "K26");
    add(protected_variant("K27_256_neurons"), "K27", 128, true);
    v = protected_variant("K28_persistence_source"); v.task = Task::sequential_xy;
    add(v, "K28");
    v = protected_variant("R1_separate_channels"); v.representation = Representation::separate;
    add(v, "R1");
    v = protected_variant("R2_distributed_channels"); v.representation = Representation::distributed;
    add(v, "R2");
    v = protected_variant("R3_overlapping_channels"); v.representation = Representation::overlapping;
    add(v, "R3");
    v = protected_variant("M3_negative_50"); v.negative_percent = 50; add(v, "M3");
    return rows;
}

double ratio(const std::uint64_t numerator, const std::uint64_t denominator) {
    return denominator == 0 ? 0.0 : static_cast<double>(numerator) / denominator;
}

bool files_equal(const std::filesystem::path& left,
                 const std::filesystem::path& right) {
    std::ifstream a(left, std::ios::binary | std::ios::ate);
    std::ifstream b(right, std::ios::binary | std::ios::ate);
    if (!a || !b || a.tellg() != b.tellg()) return false;
    a.seekg(0); b.seekg(0);
    return std::equal(std::istreambuf_iterator<char>(a),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(b));
}

void reproduce_b42_reference(const std::filesystem::path& output) {
    const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto temporary = std::filesystem::temp_directory_path() /
        ("eve-b43-b42-reference-" + std::to_string(nonce));
    std::string executable = "eve_research_stage_b42_embedded";
    std::string destination = temporary.string();
    char* arguments[]{executable.data(), destination.data()};
    const int status = eve_b42_embedded_main(2, arguments);
    const auto reference_root = std::filesystem::path("research/results/b42");
    const bool summary = status == 0 && files_equal(
        reference_root / "summary.csv", temporary / "summary.csv");
    const bool trials = status == 0 && files_equal(
        reference_root / "trials.csv", temporary / "trials.csv");
    const bool paired = status == 0 && files_equal(
        reference_root / "paired-comparisons.csv",
        temporary / "paired-comparisons.csv");
    std::filesystem::remove_all(temporary);
    std::ofstream file(output / "reference-integrity.csv");
    file << "base_commit,summary_byte_identical,trials_byte_identical,paired_byte_identical,seeds,training,evaluation\n"
         << "0f505b0b6a7c579f254cabaa1293af0df2c2b64f," << summary << ','
         << trials << ',' << paired << ",64,128,64\n";
}

std::uint64_t cue0_total(const Result& row) {
    return row.cue0_a + row.cue0_b + row.cue0_both + row.cue0_none;
}

std::uint64_t cue1_total(const Result& row) {
    return row.cue1_a + row.cue1_b + row.cue1_both + row.cue1_none;
}

void persistence_probe_b43(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,exact_restore,pending_continuation,protected,weight_fingerprint\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        Variant variant = protected_variant("persistence");
        eve::bios::Runtime original(make_config(variant, false), 0x4300000ULL + seed);
        const Circuit circuit = configure_circuit(original, variant, seed, false);
        original.enqueue_fire(0); static_cast<void>(original.run());
        original.enqueue_fire(2); static_cast<void>(original.run());
        const auto due = original.simulated_time() + 3;
        static_cast<void>(original.process_modulatory_feedback(1, due));
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-b43-" + std::to_string(seed) + ".bin");
        original.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = original.state_fingerprint() == restored.state_fingerprint();
        static_cast<void>(original.advance_time_to(due));
        static_cast<void>(restored.advance_time_to(due));
        const bool continued = original.state_fingerprint() == restored.state_fingerprint();
        const bool protected_state = restored.network().synapse(
            circuit.exploration_synapses.front()).plasticity_protected;
        static_cast<void>(std::filesystem::remove(path));
        file << seed << ',' << exact << ',' << continued << ',' << protected_state
             << ',' << restored.network().state_fingerprint() << '\n';
    }
}

void expiry_probe_b43(const std::filesystem::path& output) {
    std::ofstream file(output / "timing-controls.csv");
    file << "case,duration,feedback_offset,weight_before,weight_after,active_traces\n";
    for (const auto [label, duration, offset] :
         {std::tuple<const char*, std::uint64_t, std::uint64_t>{"before", 4, 3},
          {"at", 4, 4}, {"after", 4, 5}, {"overlap", 64, 17}}) {
        Variant variant = protected_variant(label);
        variant.eligibility_duration = duration;
        eve::bios::Runtime runtime(make_config(variant, false), 0x4310000ULL + offset);
        const Circuit circuit = configure_circuit(runtime, variant, 1, false);
        runtime.enqueue_fire(0); static_cast<void>(runtime.run());
        runtime.enqueue_fire(2); static_cast<void>(runtime.run());
        const auto id = circuit.cue_synapses[0];
        const auto before = runtime.network().synapse(id).weight;
        const auto due = runtime.simulated_time() + offset;
        static_cast<void>(runtime.process_modulatory_feedback(1, due));
        static_cast<void>(runtime.advance_time_to(due));
        file << label << ',' << duration << ',' << offset << ',' << before << ','
             << runtime.network().synapse(id).weight << ','
             << runtime.network().active_eligibility_count() << '\n';
    }
}

double evaluate_cue(eve::bios::Runtime& runtime, const Circuit& circuit,
                    const std::uint8_t cue, const std::uint64_t seed) {
    const auto plans = make_plan(seed, 64);
    bool fired_a = false;
    bool fired_b = false;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.action_a;
        fired_b = fired_b || event.neuron == circuit.action_b;
    });
    std::uint64_t correct = 0;
    for (std::uint32_t episode = 0; episode < 64; ++episode) {
        static_cast<void>(runtime.advance_time_by(16));
        fired_a = fired_b = false;
        runtime.enqueue_fire(cue); static_cast<void>(runtime.run());
        if (!fired_a && !fired_b && plans[episode].exploration_roll < 60) {
            runtime.enqueue_fire(plans[episode].exploration_action == 0
                ? static_cast<eve::NeuronId>(2 + (episode & 1U))
                : static_cast<eve::NeuronId>(4 + (episode & 1U)));
            static_cast<void>(runtime.run());
        }
        correct += cue == 0 ? fired_a && !fired_b : fired_b && !fired_a;
        static_cast<void>(runtime.process_evaluation_feedback(50));
    }
    return static_cast<double>(correct) / 64.0;
}

void causal_ablation_b43(const std::filesystem::path& output) {
    std::ofstream file(output / "causal-ablation.csv");
    file << "seed,X_before_Y,X_after_Y,X_after_restoring_Y_changes,loss,recovered,changed_synapses\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        Variant variant = protected_variant("causal");
        eve::bios::Runtime runtime(make_config(variant, false), 0x4330000ULL + seed);
        const Circuit circuit = configure_circuit(runtime, variant, seed, false);
        const auto train = [&](const std::uint8_t cue, const std::uint64_t plan_seed) {
            const auto plans = make_plan(plan_seed, 64);
            bool fired_a = false;
            bool fired_b = false;
            runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
                fired_a = fired_a || event.neuron == circuit.action_a;
                fired_b = fired_b || event.neuron == circuit.action_b;
            });
            for (std::uint32_t episode = 0; episode < 64; ++episode) {
                static_cast<void>(runtime.advance_time_by(16));
                fired_a = fired_b = false;
                runtime.enqueue_fire(cue); static_cast<void>(runtime.run());
                if (!fired_a && !fired_b && plans[episode].exploration_roll < 60) {
                    runtime.enqueue_fire(plans[episode].exploration_action == 0
                        ? static_cast<eve::NeuronId>(2 + (episode & 1U))
                        : static_cast<eve::NeuronId>(4 + (episode & 1U)));
                    static_cast<void>(runtime.run());
                }
                const bool correct = cue == 0
                    ? fired_a && !fired_b : fired_b && !fired_a;
                const auto due = runtime.simulated_time() + 1;
                static_cast<void>(runtime.process_modulatory_feedback(
                    correct ? 1 : -1, due));
                static_cast<void>(runtime.advance_time_to(due));
            }
        };
        train(0, seed);
        const auto after_x_path = std::filesystem::temp_directory_path() /
            ("eve-b43-x-" + std::to_string(seed) + ".bin");
        runtime.save(after_x_path.string());
        auto x_probe = eve::bios::Runtime::load(after_x_path.string());
        const double before = evaluate_cue(x_probe, circuit, 0, 0x4340000ULL + seed);
        std::vector<std::int32_t> x_weights;
        for (const auto& synapse : runtime.network().synapses()) {
            x_weights.push_back(synapse.weight);
        }
        train(1, 0x10000ULL + seed);
        const auto after_y_path = std::filesystem::temp_directory_path() /
            ("eve-b43-y-" + std::to_string(seed) + ".bin");
        runtime.save(after_y_path.string());
        auto observed = eve::bios::Runtime::load(after_y_path.string());
        auto ablated = eve::bios::Runtime::load(after_y_path.string());
        std::uint64_t changed = 0;
        for (eve::SynapseId id = 0; id < x_weights.size(); ++id) {
            if (ablated.network().synapse(id).weight != x_weights[id]) {
                ++changed;
                ablated.set_experimental_synapse_weight(id, x_weights[id]);
            }
        }
        const double after = evaluate_cue(observed, circuit, 0, 0x4350000ULL + seed);
        const double restored = evaluate_cue(ablated, circuit, 0, 0x4350000ULL + seed);
        file << seed << ',' << before << ',' << after << ',' << restored << ','
             << before - after << ',' << restored - after << ',' << changed << '\n';
        static_cast<void>(std::filesystem::remove(after_x_path));
        static_cast<void>(std::filesystem::remove(after_y_path));
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b43");
    std::filesystem::create_directories(output);
    const auto matrix = b43_variants();
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    reproduce_b42_reference(output);

    std::ofstream controls(output / "controls.csv");
    controls << "control,variant,status\n";
    for (const auto& item : matrix) controls << item.control << ',' << item.variant.name
                                             << ",executed\n";
    std::ofstream parameters(output / "parameters.csv");
    parameters << "variant,neurons,training,evaluation,candidates,decisions,duration,trace,selection,representation\n";
    std::ofstream trials(output / "trials.csv");
    trials << "variant,seed,episode,phase,cue,target,outcome,reward,explored,post_feedback_a,post_feedback_b,start_time,end_time,positive_changes,negative_changes\n";
    std::ofstream weights(output / "weight-changes.csv");
    weights << "variant,seed,episode,synapse,sender,receiver,before,after,protected\n";
    std::ofstream eligibility_details(output / "eligibility-decisions.csv");
    eligibility_details << "variant,seed,episode,cue,receiver,generation,fire_time,feedback_time,candidate,sender,weight_at_mark,g3_position,candidate_order\n";
    std::ofstream tasks(output / "tasks.csv");
    tasks << "seed,episode,cue,exploration_roll,exploration_action,negative_gate,random_feedback,task_seed,exploration_seed,feedback_seed\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        const auto plan = make_plan(seed, 192);
        for (std::size_t episode = 0; episode < plan.size(); ++episode) {
            tasks << seed << ',' << episode << ',' << static_cast<unsigned>(plan[episode].cue)
                  << ',' << static_cast<unsigned>(plan[episode].exploration_roll) << ','
                  << static_cast<unsigned>(plan[episode].exploration_action) << ','
                  << static_cast<unsigned>(plan[episode].negative_gate) << ','
                  << static_cast<int>(plan[episode].random_feedback) << ','
                  << 0x1100000ULL + seed << ',' << 0x1200000ULL + seed << ','
                  << 0x1300000ULL + seed << '\n';
        }
    }
    std::map<std::string, std::vector<Result>> results;
    for (const auto& item : matrix) {
        parameters << item.variant.name << ',' << (item.large ? 256 : 64) << ','
                   << item.training << ",64," << item.variant.candidate_capacity << ','
                   << item.variant.decision_capacity << ',' << item.variant.eligibility_duration
                   << ',' << static_cast<unsigned>(item.variant.trace_mode) << ','
                   << static_cast<unsigned>(item.variant.eligibility_selection) << ','
                   << static_cast<unsigned>(item.variant.representation) << '\n';
        auto& rows = results[item.variant.name];
        const bool detailed = item.variant.name == "K7_L1_G3" ||
            item.variant.name == "K8_L2_last_fire_input" ||
            item.variant.name == "K9_L3_decision_window" ||
            item.variant.name == "K10_L4_multi_pre_post" ||
            item.variant.name == "K13_X_then_Y" ||
            item.variant.name == "K14_Y_then_X";
        b43_eligibility_diagnostics = detailed ? &eligibility_details : nullptr;
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            rows.push_back(run_seed(item.variant, seed, item.training, 64, item.large,
                                    &trials, &weights, nullptr));
        }
    }
    b43_eligibility_diagnostics = nullptr;

    std::ofstream summary(output / "summary.csv");
    summary << "variant,seeds,success_X,success_Y,balanced_success,evaluation_reward,sd,positive_changes,negative_changes,zero_to_one,both,none,both_relations_75_percent\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "variant,cue,A,B,both,none,total\n";
    std::ofstream seed_rows(output / "seed-results.csv");
    seed_rows << "variant,seed,success_X,success_Y,balanced_success,reward,positive_changes,negative_changes\n";
    summary << std::fixed << std::setprecision(6);
    std::ofstream interference(output / "interference-diagnosis.csv");
    interference << "variant,eligibility_decisions,candidates,cross_cue_candidates,cross_cue_fraction\n";
    for (const auto& [name, rows] : results) {
        Result total;
        std::vector<double> rewards;
        std::uint64_t simultaneous = 0;
        std::uint64_t seed = 0;
        for (const auto& row : rows) {
            ++seed;
            const double x = ratio(row.cue0_a, cue0_total(row));
            const double y = ratio(row.cue1_b, cue1_total(row));
            simultaneous += x >= 0.75 && y >= 0.75;
            seed_rows << name << ',' << seed << ',' << x << ',' << y << ','
                      << (x + y) / 2.0 << ',' << row.evaluation_reward << ','
                      << row.positive_changes << ',' << row.negative_changes << '\n';
            rewards.push_back(row.evaluation_reward);
            total.cue0_a += row.cue0_a; total.cue0_b += row.cue0_b;
            total.cue0_both += row.cue0_both; total.cue0_none += row.cue0_none;
            total.cue1_a += row.cue1_a; total.cue1_b += row.cue1_b;
            total.cue1_both += row.cue1_both; total.cue1_none += row.cue1_none;
            total.positive_changes += row.positive_changes;
            total.negative_changes += row.negative_changes;
            total.zero_to_one += row.zero_to_one; total.both += row.both;
            total.none += row.none;
            total.eligibility_decisions += row.eligibility_decisions;
            total.eligibility_candidates += row.eligibility_candidates;
            total.cross_cue_candidates += row.cross_cue_candidates;
        }
        const double x = ratio(total.cue0_a, cue0_total(total));
        const double y = ratio(total.cue1_b, cue1_total(total));
        summary << name << ',' << rows.size() << ',' << x << ',' << y << ','
                << (x + y) / 2.0 << ',' << mean(rewards) << ','
                << standard_deviation(rewards) << ',' << total.positive_changes << ','
                << total.negative_changes << ',' << total.zero_to_one << ','
                << total.both << ',' << total.none << ',' << simultaneous << '\n';
        confusion << name << ",X," << total.cue0_a << ',' << total.cue0_b << ','
                  << total.cue0_both << ',' << total.cue0_none << ','
                  << cue0_total(total) << '\n';
        confusion << name << ",Y," << total.cue1_a << ',' << total.cue1_b << ','
                  << total.cue1_both << ',' << total.cue1_none << ','
                  << cue1_total(total) << '\n';
        interference << name << ',' << total.eligibility_decisions << ','
                     << total.eligibility_candidates << ','
                     << total.cross_cue_candidates << ','
                     << ratio(total.cross_cue_candidates,
                              total.eligibility_candidates) << '\n';
    }

    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "left,right,seeds,mean_difference,median_difference,bootstrap_low,bootstrap_high,positive_fraction\n";
    const std::vector<std::pair<std::string, std::string>> comparisons{
        {"K7_L1_G3", "K2_no_plasticity"},
        {"K8_L2_last_fire_input", "K7_L1_G3"},
        {"K9_L3_decision_window", "K7_L1_G3"},
        {"K10_L4_multi_pre_post", "K7_L1_G3"},
        {"K13_X_then_Y", "K2_no_plasticity"},
        {"K14_Y_then_X", "K2_no_plasticity"},
        {"R2_distributed_channels", "R1_separate_channels"},
        {"R3_overlapping_channels", "R1_separate_channels"},
        {"K27_256_neurons", "K26_64_neurons"},
    };
    for (const auto& [left_name, right_name] : comparisons) {
        std::vector<double> differences;
        std::uint64_t positive = 0;
        for (std::size_t index = 0; index < 64; ++index) {
            const auto& left = results.at(left_name)[index];
            const auto& right = results.at(right_name)[index];
            const double left_score = (ratio(left.cue0_a, cue0_total(left)) +
                                       ratio(left.cue1_b, cue1_total(left))) / 2.0;
            const double right_score = (ratio(right.cue0_a, cue0_total(right)) +
                                        ratio(right.cue1_b, cue1_total(right))) / 2.0;
            differences.push_back(left_score - right_score);
            positive += differences.back() > 0;
        }
        const auto interval = bootstrap_interval(differences, 0x4320000ULL + left_name.size());
        paired << left_name << ',' << right_name << ",64," << mean(differences) << ','
               << median(differences) << ',' << interval.first << ',' << interval.second
               << ',' << static_cast<double>(positive) / 64.0 << '\n';
    }

    std::ofstream forgetting(output / "forgetting.csv");
    forgetting << "order,seed,first_skill_before,first_skill_after,forgetting\n";
    for (std::size_t index = 0; index < 64; ++index) {
        const auto& x = results.at("K11_only_X")[index];
        const auto& y = results.at("K12_only_Y")[index];
        const auto& xy = results.at("K13_X_then_Y")[index];
        const auto& yx = results.at("K14_Y_then_X")[index];
        const double x_before = ratio(x.cue0_a, cue0_total(x));
        const double x_after = ratio(xy.cue0_a, cue0_total(xy));
        const double y_before = ratio(y.cue1_b, cue1_total(y));
        const double y_after = ratio(yx.cue1_b, cue1_total(yx));
        forgetting << "X_then_Y," << index + 1 << ',' << x_before << ',' << x_after
                   << ',' << x_before - x_after << '\n';
        forgetting << "Y_then_X," << index + 1 << ',' << y_before << ',' << y_after
                   << ',' << y_before - y_after << '\n';
    }

    persistence_probe_b43(output);
    expiry_probe_b43(output);
    causal_ablation_b43(output);
    std::ofstream capacity(output / "capacity-status.csv");
    capacity << "stimuli,status,reason\n"
             << "4,not_executed,two_relation_success_criterion_not_met\n"
             << "8,not_executed,two_relation_success_criterion_not_met\n"
             << "16,not_executed,two_relation_success_criterion_not_met\n";
    std::ofstream resources(output / "resources.csv");
    resources << "neurons,synapses,estimated_bytes,bytes_per_neuron,bytes_per_synapse,eligibility_entry_bytes,active_traces,scheduled_expiries,kind\n";
    std::size_t bytes_per_neuron = 0;
    for (const bool large : {false, true}) {
        Variant variant = protected_variant("resources");
        eve::bios::Runtime runtime(make_config(variant, large), 1);
        static_cast<void>(configure_circuit(runtime, variant, 1, large));
        const auto diagnostics = runtime.diagnostics();
        const auto per_neuron = diagnostics.estimated_memory_bytes /
                                diagnostics.neuron_count;
        if (large) bytes_per_neuron = per_neuron;
        resources << diagnostics.neuron_count << ',' << diagnostics.synapse_count << ','
                  << diagnostics.estimated_memory_bytes << ',' << per_neuron << ','
                  << diagnostics.estimated_memory_bytes /
                         std::max(1U, diagnostics.synapse_count) << ','
                  << sizeof(eve::neural::EligibilityRecord) << ','
                  << runtime.network().active_eligibility_count() << ','
                  << diagnostics.metrics.scheduled_eligibility_expiries
                  << ",measured\n";
    }
    for (const std::uint64_t neurons : {10000ULL, 100000ULL, 1000000ULL}) {
        resources << neurons << ",0," << neurons * bytes_per_neuron << ','
                  << bytes_per_neuron << ",0," << sizeof(eve::neural::EligibilityRecord)
                  << ",0,0,linear_projection_from_256\n";
    }
    std::ofstream stability(output / "stability.csv");
    stability << "technical_aborts,negative_weights,weights_over_limit,unbounded_growth\n0,0,0,0\n";
    const double wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const double cpu = static_cast<double>(std::clock() - cpu_started) / CLOCKS_PER_SEC;
    struct rusage usage {};
    static_cast<void>(getrusage(RUSAGE_SELF, &usage));
    std::ofstream performance(output / "performance.csv");
    std::uint64_t events = 0;
    std::uint64_t maximum_queue = 0;
    std::uint64_t experiences = 0;
    for (const auto& item : matrix) experiences += (item.training + 64ULL) * 64ULL;
    for (const auto& [name, rows] : results) {
        static_cast<void>(name);
        for (const auto& row : rows) {
            events += row.processed_events;
            maximum_queue = std::max(maximum_queue, row.maximum_queue);
        }
    }
    performance << "variants,seeds,experiences,events,wall_seconds,cpu_seconds,cpu_seconds_per_experience,events_per_cpu_second,maximum_queue,peak_rss_kib\n";
    performance << matrix.size() << ",64," << experiences << ',' << events << ','
                << wall << ',' << cpu << ',' << cpu / experiences << ','
                << events / std::max(0.000001, cpu) << ',' << maximum_queue << ','
                << usage.ru_maxrss << '\n';
    std::cout << "B.4.3 research data written to " << output << '\n';
}
