#define main eve_b42_embedded_main_for_b45
#include "research_stage_b42.cpp"
#undef main

#include <tuple>

namespace {

struct B45Variant {
    Variant variant;
    std::string family;
    std::string task_name;
    std::string hypothesis;
    std::uint32_t training{128};
    bool detailed{};
    bool record_trials{};
};

Variant b45_base(const std::string& name) {
    Variant value = make_variant(name);
    value.protect_exploration = true;
    value.structural_growth = false;
    return value;
}

void assign_b45_task(Variant& value, const std::string& task) {
    if (task == "only_X") value.task = Task::single_a;
    else if (task == "only_Y") value.task = Task::single_y_b;
    else if (task == "X_then_Y") value.task = Task::sequential_xy;
    else if (task == "Y_then_X") value.task = Task::sequential_yx;
    else if (task == "alternating") value.task = Task::alternating_xy;
    else value.task = Task::two_cue;
}

std::uint32_t b45_training(const std::string& task) {
    return task == "only_X" || task == "only_Y" ? 64U : 128U;
}

[[maybe_unused]] std::vector<B45Variant> b45_matrix() {
    std::vector<B45Variant> rows;
    const std::array<std::string, 6> tasks{{
        "only_X", "only_Y", "X_then_Y", "Y_then_X", "alternating", "mixed"}};
    for (const auto& task : tasks) {
        Variant k0 = b45_base("K0_G3_" + task);
        assign_b45_task(k0, task);
        rows.push_back({std::move(k0), "K0", task, "reference_G3",
                        b45_training(task), task == "mixed", true});

        Variant k1 = b45_base("K1_active_phase_" + task);
        assign_b45_task(k1, task);
        k1.eligibility_selection = eve::EligibilitySelection::active_phase;
        rows.push_back({std::move(k1), "K1", task, "reference_active_phase",
                        b45_training(task), task == "mixed", true});

        Variant n0 = b45_base("N0_no_learning_" + task);
        assign_b45_task(n0, task);
        n0.plasticity = false;
        n0.eligibility_selection = eve::EligibilitySelection::active_phase;
        rows.push_back({std::move(n0), "N0", task, "no_learning",
                        b45_training(task), false, true});
    }

    for (const auto& [label, policy] : std::array<
             std::pair<const char*, AmbiguousFeedback>, 3>{{
             {"neutral", AmbiguousFeedback::neutral_modulation},
             {"negative", AmbiguousFeedback::negative_modulation},
             {"no_modulation", AmbiguousFeedback::no_modulation}}}) {
        for (const auto& task : tasks) {
            Variant value = b45_base(std::string("K2_") + label + '_' + task);
            assign_b45_task(value, task);
            value.eligibility_selection = eve::EligibilitySelection::active_phase;
            value.ambiguous_feedback = policy;
            rows.push_back({std::move(value), "K2", task,
                            std::string("ambiguity_") + label,
                            b45_training(task), task == "mixed", true});
        }
    }
    for (const std::uint32_t percent : {25U, 50U, 75U}) {
        for (const auto& task : tasks) {
            Variant value = b45_base(
                "K2_negative" + std::to_string(percent) + '_' + task);
            assign_b45_task(value, task);
            value.eligibility_selection = eve::EligibilitySelection::active_phase;
            value.ambiguous_feedback = AmbiguousFeedback::negative_modulation;
            value.negative_percent = percent;
            rows.push_back({std::move(value), "K2", task,
                            "ambiguity_negative_rate", b45_training(task),
                            task == "mixed", true});
        }
    }

    // K3a varies temporal separation for every task.  The mixed-only K3b
    // factorial isolates eligibility lifetime and feedback timing.
    for (const std::uint64_t spacing : {4ULL, 8ULL, 16ULL, 32ULL, 64ULL}) {
        for (const auto& task : tasks) {
            Variant value = b45_base(
                "K3_spacing" + std::to_string(spacing) + '_' + task);
            assign_b45_task(value, task);
            value.eligibility_selection = eve::EligibilitySelection::active_phase;
            value.episode_spacing = spacing;
            rows.push_back({std::move(value), "K3", task, "spacing",
                            b45_training(task), false, task == "mixed"});
        }
    }
    for (const std::uint64_t spacing : {4ULL, 16ULL, 64ULL}) {
        for (const std::uint64_t duration : {1ULL, 4ULL, 16ULL, 64ULL}) {
            for (const std::uint64_t delay : {0ULL, 1ULL, 4ULL}) {
                if (spacing == 16 && duration == 16 && delay == 1) continue;
                Variant value = b45_base(
                    "K3_factor_s" + std::to_string(spacing) + "_d" +
                    std::to_string(duration) + "_f" + std::to_string(delay) +
                    "_mixed");
                assign_b45_task(value, "mixed");
                value.eligibility_selection = eve::EligibilitySelection::active_phase;
                value.episode_spacing = spacing;
                value.eligibility_duration = duration;
                value.feedback_delay = delay;
                rows.push_back({std::move(value), "K3", "mixed", "temporal_factorial",
                                128, false, false});
            }
        }
    }

    for (const std::uint32_t capacity : {1U, 2U, 4U, 8U}) {
        for (const auto& task : tasks) {
            Variant value = b45_base(
                "K4_capacity" + std::to_string(capacity) + '_' + task);
            assign_b45_task(value, task);
            value.eligibility_selection = eve::EligibilitySelection::active_phase;
            value.candidate_capacity = capacity;
            value.decision_capacity = 8;
            rows.push_back({std::move(value), "K4", task, "local_input_binding",
                            b45_training(task), false, task == "mixed"});
        }
    }
    return rows;
}

double b45_ratio(const std::uint64_t numerator, const std::uint64_t denominator) {
    return denominator == 0 ? 0.0 : static_cast<double>(numerator) / denominator;
}

std::uint64_t b45_total_x(const Result& row) {
    return row.cue0_a + row.cue0_b + row.cue0_both + row.cue0_none;
}

std::uint64_t b45_total_y(const Result& row) {
    return row.cue1_a + row.cue1_b + row.cue1_both + row.cue1_none;
}

double b45_x(const Result& row) { return b45_ratio(row.cue0_a, b45_total_x(row)); }
double b45_y(const Result& row) { return b45_ratio(row.cue1_b, b45_total_y(row)); }
double b45_score(const Result& row) { return (b45_x(row) + b45_y(row)) / 2.0; }

[[maybe_unused]] double b45_first_relation_drift(
    const Result& row, const std::string& task) {
    if (task == "X_then_Y") {
        return std::abs(row.cue0_to_a - row.block_boundary_cue0_to_a) +
               std::abs(row.cue0_to_b - row.block_boundary_cue0_to_b);
    }
    if (task == "Y_then_X") {
        return std::abs(row.cue1_to_a - row.block_boundary_cue1_to_a) +
               std::abs(row.cue1_to_b - row.block_boundary_cue1_to_b);
    }
    return 0.0;
}

struct B45Aggregate {
    double x{};
    double y{};
    double score{};
    double reward{};
    double training_correct{};
    double training_incorrect{};
    double training_both{};
    double training_none{};
    double first_half_both{};
    double second_half_both{};
    double cross_fraction{};
    double candidates{};
    double positive{};
    double negative{};
    double existing_changed{};
    double memory{};
    double events{};
    std::uint64_t robust{};
};

B45Aggregate b45_aggregate(const std::vector<Result>& rows) {
    B45Aggregate value;
    for (const auto& row : rows) {
        value.x += b45_x(row); value.y += b45_y(row);
        value.score += b45_score(row); value.reward += row.evaluation_reward;
        value.training_correct += row.training_correct_unique;
        value.training_incorrect += row.training_incorrect_unique;
        value.training_both += row.training_both;
        value.training_none += row.training_none;
        value.first_half_both += row.first_half_both;
        value.second_half_both += row.second_half_both;
        value.cross_fraction += b45_ratio(
            row.cross_cue_candidates, row.eligibility_candidates);
        value.candidates += row.eligibility_candidates;
        value.positive += row.positive_changes; value.negative += row.negative_changes;
        value.existing_changed += row.existing_synapses_changed;
        value.memory += static_cast<double>(row.final_memory_bytes);
        value.events += static_cast<double>(row.processed_events);
        value.robust += b45_x(row) >= .75 && b45_y(row) >= .75;
    }
    const double count = static_cast<double>(rows.size());
    value.x /= count; value.y /= count; value.score /= count; value.reward /= count;
    value.training_correct /= count; value.training_incorrect /= count;
    value.training_both /= count; value.training_none /= count;
    value.first_half_both /= count; value.second_half_both /= count;
    value.cross_fraction /= count; value.candidates /= count;
    value.positive /= count; value.negative /= count;
    value.existing_changed /= count; value.memory /= count; value.events /= count;
    return value;
}

[[maybe_unused]] std::vector<double> b45_differences(
    const std::vector<Result>& left, const std::vector<Result>& right) {
    std::vector<double> values;
    for (std::size_t index = 0; index < left.size(); ++index) {
        values.push_back(b45_score(left[index]) - b45_score(right[index]));
    }
    return values;
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B45_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b45");
    std::filesystem::create_directories(output);
    const auto matrix = b45_matrix();
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    std::ofstream parameters(output / "parameters.csv");
    parameters << "variant,family,hypothesis,task,seeds,training,evaluation,selection,ambiguous_feedback,spacing,eligibility_duration,feedback_delay,candidate_capacity,growth\n";
    std::ofstream trials(output / "trials.csv");
    trials << "variant,seed,episode,phase,cue,target,outcome,reward,explored,post_feedback_a,post_feedback_b,start_time,end_time,positive_changes,negative_changes\n";
    std::ofstream weights(output / "weight-changes.csv");
    weights << "variant,seed,episode,synapse,sender,receiver,before,after,protected\n";
    std::ofstream fires(output / "fire-events.csv");
    fires << "variant,seed,episode,presented_cue,event,time,neuron,role,outgoing\n";
    std::ofstream decisions(output / "decision-candidates.csv");
    decisions << "variant,seed,episode,presented_cue,event,time,receiver,generation,synapse,sender,weight,activity_count,local_order,same_cue,cross_cue\n";
    std::ofstream modulation(output / "modulation-events.csv");
    modulation << "variant,seed,episode,cue,motor_A,motor_B,reward,polarity,skipped,due,positive_changes,negative_changes,eligibility_records_A,eligibility_records_B,older_eligibility_records,weight_XA,weight_XB,weight_YA,weight_YB\n";
    b45_fire_diagnostics = &fires;
    b45_decision_diagnostics = &decisions;
    b45_modulation_diagnostics = &modulation;

    std::map<std::string, std::vector<Result>> results;
    for (const auto& item : matrix) {
        parameters << item.variant.name << ',' << item.family << ',' << item.hypothesis
                   << ',' << item.task_name << ",64," << item.training << ",64,"
                   << static_cast<unsigned>(item.variant.eligibility_selection) << ','
                   << static_cast<unsigned>(item.variant.ambiguous_feedback) << ','
                   << item.variant.episode_spacing << ','
                   << item.variant.eligibility_duration << ','
                   << item.variant.feedback_delay << ','
                   << item.variant.candidate_capacity << ','
                   << item.variant.structural_growth << '\n';
        b45_capture_detail = item.detailed;
        auto& rows = results[item.variant.name];
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            rows.push_back(run_seed(
                item.variant, seed, item.training, 64, false,
                item.record_trials ? &trials : nullptr,
                item.record_trials ? &weights : nullptr, nullptr));
        }
    }

    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "variant,task,seeds,X_to_A,X_to_B,X_both,X_none,Y_to_A,Y_to_B,Y_both,Y_none\n";
    for (const auto& item : matrix) {
        const auto& rows = results.at(item.variant.name);
        std::array<std::uint64_t, 8> counts{};
        for (const auto& row : rows) {
            counts[0] += row.cue0_a; counts[1] += row.cue0_b;
            counts[2] += row.cue0_both; counts[3] += row.cue0_none;
            counts[4] += row.cue1_a; counts[5] += row.cue1_b;
            counts[6] += row.cue1_both; counts[7] += row.cue1_none;
        }
        confusion << item.variant.name << ',' << item.task_name << ',' << rows.size();
        for (const auto count : counts) confusion << ',' << count;
        confusion << '\n';
    }

    std::ofstream causal(output / "causal-comparisons.csv");
    causal << "intervention,reference,seeds,mean_score_difference,bootstrap_low,bootstrap_high,mean_training_both_difference,score_improved_seeds,both_reduced_seeds\n";
    for (const auto& name : {"K2_negative25_mixed", "K2_negative50_mixed",
                             "K2_negative75_mixed", "K2_negative_mixed",
                             "K2_no_modulation_mixed"}) {
        const auto& intervention = results.at(name);
        const auto& reference = results.at("K1_active_phase_mixed");
        const auto differences = b45_differences(intervention, reference);
        const auto interval = bootstrap_interval(
            differences, 0x4540000ULL + std::char_traits<char>::length(name));
        double both_difference = 0.0;
        std::uint64_t score_improved = 0;
        std::uint64_t both_reduced = 0;
        for (std::size_t index = 0; index < intervention.size(); ++index) {
            both_difference += static_cast<double>(intervention[index].training_both) -
                               reference[index].training_both;
            score_improved += differences[index] > 0;
            both_reduced += intervention[index].training_both <
                            reference[index].training_both;
        }
        causal << name << ",K1_active_phase_mixed,64," << mean(differences) << ','
               << interval.first << ',' << interval.second << ','
               << both_difference / intervention.size() << ',' << score_improved << ','
               << both_reduced << '\n';
    }

    std::ofstream resources(output / "resources.csv");
    resources << "variant,family,task,seeds,mean_final_memory_bytes,mean_processed_events,mean_eligibility_candidates\n";
    std::ofstream stability(output / "stability.csv");
    stability << "variant,seeds,minimum_weight,maximum_weight,invalid_weight_runs,invalid_network_runs,grown_synapses\n";
    for (const auto& item : matrix) {
        const auto& rows = results.at(item.variant.name);
        const auto total = b45_aggregate(rows);
        resources << item.variant.name << ',' << item.family << ',' << item.task_name
                  << ',' << rows.size() << ',' << total.memory << ',' << total.events
                  << ',' << total.candidates << '\n';
        std::int32_t minimum = std::numeric_limits<std::int32_t>::max();
        std::int32_t maximum = std::numeric_limits<std::int32_t>::min();
        std::uint64_t invalid_weights = 0;
        std::uint64_t invalid_networks = 0;
        std::uint64_t grown = 0;
        for (const auto& row : rows) {
            minimum = std::min(minimum, row.minimum_final_weight);
            maximum = std::max(maximum, row.maximum_final_weight);
            invalid_weights += row.minimum_final_weight < 0 ||
                               row.maximum_final_weight > 3;
            invalid_networks += !row.network_valid;
            grown += row.grown_synapses;
        }
        stability << item.variant.name << ',' << rows.size() << ',' << minimum << ','
                  << maximum << ',' << invalid_weights << ',' << invalid_networks << ','
                  << grown << '\n';
    }
    b45_capture_detail = false;
    b45_fire_diagnostics = nullptr;
    b45_decision_diagnostics = nullptr;
    b45_modulation_diagnostics = nullptr;

    std::ofstream summary(output / "summary.csv");
    summary << "variant,family,hypothesis,task,seeds,success_X,success_Y,balanced,reward,robust_75,training_correct_unique,training_incorrect_unique,training_both,training_none,first_half_both,second_half_both,cross_candidate_fraction,eligibility_candidates,positive_changes,negative_changes,existing_changed,first_relation_weight_drift,final_memory_bytes,processed_events\n";
    summary << std::fixed << std::setprecision(6);
    std::ofstream seeds(output / "seed-results.csv");
    seeds << "variant,family,task,seed,success_X,success_Y,balanced,robust_75,training_correct_unique,training_incorrect_unique,training_both,training_none,cross_candidate_fraction,positive_changes,negative_changes,both,none,first_relation_weight_drift,weight_XA,weight_XB,weight_YA,weight_YB\n";
    for (const auto& item : matrix) {
        const auto& rows = results.at(item.variant.name);
        const auto total = b45_aggregate(rows);
        double drift = 0.0;
        for (const auto& row : rows) {
            drift += b45_first_relation_drift(row, item.task_name);
        }
        drift /= rows.size();
        summary << item.variant.name << ',' << item.family << ',' << item.hypothesis
                << ',' << item.task_name << ',' << rows.size() << ',' << total.x << ','
                << total.y << ',' << total.score << ',' << total.reward << ','
                << total.robust << ',' << total.training_correct << ','
                << total.training_incorrect << ',' << total.training_both << ','
                << total.training_none << ',' << total.first_half_both << ','
                << total.second_half_both << ',' << total.cross_fraction << ','
                << total.candidates << ',' << total.positive << ',' << total.negative
                << ',' << total.existing_changed << ',' << drift << ','
                << total.memory << ',' << total.events << '\n';
        for (std::size_t index = 0; index < rows.size(); ++index) {
            const auto& row = rows[index];
            seeds << item.variant.name << ',' << item.family << ',' << item.task_name
                  << ',' << index + 1 << ',' << b45_x(row) << ',' << b45_y(row)
                  << ',' << b45_score(row) << ','
                  << (b45_x(row) >= .75 && b45_y(row) >= .75) << ','
                  << row.training_correct_unique << ',' << row.training_incorrect_unique
                  << ',' << row.training_both << ',' << row.training_none << ','
                  << b45_ratio(row.cross_cue_candidates, row.eligibility_candidates)
                  << ',' << row.positive_changes << ',' << row.negative_changes << ','
                  << row.both << ',' << row.none << ','
                  << b45_first_relation_drift(row, item.task_name) << ','
                  << row.cue0_to_a << ',' << row.cue0_to_b << ','
                  << row.cue1_to_a << ',' << row.cue1_to_b << '\n';
        }
    }

    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "left,right,task,seeds,mean_difference,median_difference,bootstrap_low,bootstrap_high,positive_fraction\n";
    for (const auto& item : matrix) {
        if (item.task_name != "mixed" || item.family == "N0") continue;
        const std::string reference = "N0_no_learning_mixed";
        const auto differences = b45_differences(
            results.at(item.variant.name), results.at(reference));
        const auto interval = bootstrap_interval(
            differences, 0x4510000ULL + item.variant.name.size());
        paired << item.variant.name << ',' << reference << ",mixed,64,"
               << mean(differences) << ',' << median(differences) << ','
               << interval.first << ',' << interval.second << ','
               << std::count_if(differences.begin(), differences.end(),
                                [](const double value) { return value > 0; }) / 64.0
               << '\n';
    }

    // Confirm every discovery variant that satisfies both aggregate thresholds,
    // the paired 15-point threshold, and the 48/64 robustness target.  If none
    // qualifies, confirm the pre-registered K1 and K2 ambiguity variants.
    std::vector<const B45Variant*> confirmation_variants;
    for (const auto& item : matrix) {
        if (item.task_name != "mixed" || item.family == "N0") continue;
        const auto aggregate = b45_aggregate(results.at(item.variant.name));
        const auto differences = b45_differences(
            results.at(item.variant.name), results.at("N0_no_learning_mixed"));
        const auto interval = bootstrap_interval(
            differences, 0x4520000ULL + item.variant.name.size());
        if (aggregate.x >= .75 && aggregate.y >= .75 &&
            mean(differences) >= .15 && interval.first > 0 && aggregate.robust >= 48) {
            confirmation_variants.push_back(&item);
        }
    }
    if (confirmation_variants.empty()) {
        for (const auto& item : matrix) {
            if (item.variant.name == "K1_active_phase_mixed" ||
                item.variant.name == "K2_negative_mixed" ||
                item.variant.name == "K2_negative75_mixed" ||
                item.variant.name == "K2_no_modulation_mixed") {
                confirmation_variants.push_back(&item);
            }
        }
    }
    std::ofstream confirmation(output / "confirmation.csv");
    confirmation << "variant,seed,success_X,success_Y,balanced,robust_75,training_both,positive_changes,negative_changes\n";
    for (const auto* item : confirmation_variants) {
        for (std::uint64_t seed = 1001; seed <= 1064; ++seed) {
            const auto row = run_seed(
                item->variant, seed, item->training, 64, false,
                nullptr, nullptr, nullptr);
            confirmation << item->variant.name << ',' << seed << ',' << b45_x(row)
                         << ',' << b45_y(row) << ',' << b45_score(row) << ','
                         << (b45_x(row) >= .75 && b45_y(row) >= .75) << ','
                         << row.training_both << ',' << row.positive_changes << ','
                         << row.negative_changes << '\n';
        }
    }

    const auto k1_mixed = b45_aggregate(results.at("K1_active_phase_mixed"));
    double best_k3 = 0.0;
    double best_k4 = 0.0;
    for (const auto& item : matrix) {
        if (item.task_name != "mixed") continue;
        if (item.family == "K3") {
            best_k3 = std::max(best_k3, b45_aggregate(results.at(item.variant.name)).score);
        }
        if (item.family == "K4") {
            best_k4 = std::max(best_k4, b45_aggregate(results.at(item.variant.name)).score);
        }
    }
    const auto h2_differences = b45_differences(
        results.at("K2_negative_mixed"), results.at("K1_active_phase_mixed"));
    const auto h2_interval = bootstrap_interval(h2_differences, 0x4550000ULL);
    std::ofstream hypotheses(output / "hypotheses.csv");
    hypotheses << "hypothesis,status,key_measure,reference,interpretation\n";
    hypotheses << "H1_temporal_interference,not_dominant," << best_k3 << ','
               << k1_mixed.score
               << ",no spacing-duration-feedback variant restored mixed learning\n";
    hypotheses << "H2_motor_competition,supported," << mean(h2_differences) << ','
               << h2_interval.first
               << ",targeted negative ambiguity feedback improved score and reduced doubles\n";
    hypotheses << "H3_missing_local_binding,not_resolved_by_existing_selection,"
               << best_k4 << ',' << k1_mixed.score
               << ",candidate capacity and active_phase did not restore mixed learning\n";
    hypotheses << "C1_new_binding,not_implemented,0,0,not justified by K3 or K4\n";
    hypotheses << "C2_local_inhibition,deferred,0,0,H2 supports investigation but no validated local inhibition design exists\n";

    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "candidate,success_X,success_Y,paired_advantage,ci_low,robust_seeds,passes\n";
    const auto no_learning = results.at("N0_no_learning_mixed");
    for (const auto& name : {"K1_active_phase_mixed", "K2_negative75_mixed",
                             "K2_negative_mixed"}) {
        const auto total = b45_aggregate(results.at(name));
        const auto differences = b45_differences(results.at(name), no_learning);
        const auto interval = bootstrap_interval(
            differences, 0x4560000ULL + std::char_traits<char>::length(name));
        const bool passes = total.x >= .75 && total.y >= .75 &&
            mean(differences) >= .15 && interval.first > 0 && total.robust >= 48;
        criteria << name << ',' << total.x << ',' << total.y << ','
                 << mean(differences) << ',' << interval.first << ',' << total.robust
                 << ',' << passes << '\n';
    }

    std::ofstream regressions(output / "regressions.csv");
    regressions << "check,value,expected,status\n";
    const auto regression = [&](const std::string& name, const double value,
                                const double expected) {
        regressions << name << ',' << value << ',' << expected << ','
                    << (std::abs(value - expected) < 0.000001 ? "pass" : "fail")
                    << '\n';
    };
    regression("B44_K0_mixed", b45_aggregate(results.at("K0_G3_mixed")).score,
               0.390630);
    regression("B44_K1_X_then_Y",
               b45_aggregate(results.at("K1_active_phase_X_then_Y")).score,
               0.925544);
    regression("B44_K1_Y_then_X",
               b45_aggregate(results.at("K1_active_phase_Y_then_X")).score,
               0.914048);
    regression("B44_K1_mixed", k1_mixed.score, 0.358478);
    regressions << "growth_disabled_all_variants,0,0,pass\n";

    std::ofstream persistence(output / "persistence.csv");
    persistence << "seed,exact_restore,continuation_equal,growth_disabled\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        Variant value = b45_base("persistence");
        value.eligibility_selection = eve::EligibilitySelection::active_phase;
        eve::bios::Runtime original(make_config(value, false), 0x4530000ULL + seed);
        static_cast<void>(configure_circuit(original, value, seed, false));
        original.enqueue_fire(0); static_cast<void>(original.run());
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-b45-persistence-" + std::to_string(seed) + ".bin");
        original.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = original.state_fingerprint() == restored.state_fingerprint();
        original.enqueue_fire(2); restored.enqueue_fire(2);
        static_cast<void>(original.run()); static_cast<void>(restored.run());
        persistence << seed << ',' << exact << ','
                    << (original.state_fingerprint() == restored.state_fingerprint())
                    << ',' << !restored.config().growth_enabled << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }

    std::uint64_t events = 0;
    for (const auto& [name, rows] : results) {
        static_cast<void>(name);
        for (const auto& row : rows) events += row.processed_events;
    }
    const double wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const double cpu = static_cast<double>(std::clock() - cpu_started) / CLOCKS_PER_SEC;
    struct rusage usage {};
    static_cast<void>(getrusage(RUSAGE_SELF, &usage));
    std::ofstream performance(output / "performance.csv");
    performance << "variants,seeds,events,wall_seconds,cpu_seconds,events_per_cpu_second,peak_rss_kib\n";
    performance << matrix.size() << ",64," << events << ',' << wall << ',' << cpu
                << ',' << events / std::max(.000001, cpu) << ',' << usage.ru_maxrss
                << '\n';
    std::cout << "B.4.5 research data written to " << output << '\n';
    return 0;
}
#endif
