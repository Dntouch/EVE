#define main eve_b42_embedded_main_for_b44
#include "research_stage_b42.cpp"
#undef main

#include <tuple>

namespace {

struct B44Variant {
    Variant variant;
    std::string group;
    std::string task_name;
    std::uint32_t training{128};
};

Variant b44_base(const std::string& name) {
    Variant value = make_variant(name);
    value.protect_exploration = true;
    return value;
}

void assign_task(Variant& value, const std::string& task) {
    if (task == "only_X") value.task = Task::single_a;
    else if (task == "only_Y") value.task = Task::single_y_b;
    else if (task == "X_then_Y") value.task = Task::sequential_xy;
    else if (task == "Y_then_X") value.task = Task::sequential_yx;
    else if (task == "alternating") value.task = Task::alternating_xy;
    else value.task = Task::two_cue;
}

std::vector<B44Variant> b44_matrix() {
    std::vector<B44Variant> rows;
    const std::array<std::string, 6> tasks{{
        "only_X", "only_Y", "X_then_Y", "Y_then_X", "alternating", "mixed"}};
    for (const std::string group : {"S0", "S1", "S2", "S3", "S4"}) {
        for (const auto& task : tasks) {
            Variant value = b44_base(group + "_" + task);
            assign_task(value, task);
            if (group == "S1" || group == "S4") value.structural_growth = true;
            if (group == "S2") {
                // Pilot-matched to S1's mean final structural increment for
                // the same task, without inspecting behavioural outcomes.
                value.static_extra_synapses = task == "only_X" || task == "only_Y"
                    ? 62U : task == "X_then_Y" || task == "Y_then_X"
                    ? 104U : task == "alternating" ? 110U : 111U;
            }
            if (group == "S3" || group == "S4") {
                value.eligibility_selection = eve::EligibilitySelection::active_phase;
            }
            rows.push_back(B44Variant{
                std::move(value), group, task,
                task == "only_X" || task == "only_Y" ? 64U : 128U});
        }
    }
    for (const auto& task : tasks) {
        Variant no_learning = b44_base("N0_no_learning_" + task);
        assign_task(no_learning, task);
        no_learning.plasticity = false;
        rows.push_back({std::move(no_learning), "N0", task,
                        task == "only_X" || task == "only_Y" ? 64U : 128U});
        Variant growth_no_learning = b44_base("N1_growth_no_learning_" + task);
        assign_task(growth_no_learning, task);
        growth_no_learning.plasticity = false;
        growth_no_learning.structural_growth = true;
        rows.push_back({std::move(growth_no_learning), "N1", task,
                        task == "only_X" || task == "only_Y" ? 64U : 128U});
    }
    return rows;
}

double safe_ratio(const std::uint64_t numerator, const std::uint64_t denominator) {
    return denominator == 0 ? 0.0 : static_cast<double>(numerator) / denominator;
}

std::uint64_t total_x(const Result& row) {
    return row.cue0_a + row.cue0_b + row.cue0_both + row.cue0_none;
}

std::uint64_t total_y(const Result& row) {
    return row.cue1_a + row.cue1_b + row.cue1_both + row.cue1_none;
}

double success_x(const Result& row) { return safe_ratio(row.cue0_a, total_x(row)); }
double success_y(const Result& row) { return safe_ratio(row.cue1_b, total_y(row)); }
double balanced(const Result& row) { return (success_x(row) + success_y(row)) / 2.0; }

struct Aggregate {
    double x{};
    double y{};
    double score{};
    double reward{};
    double synapses_before{};
    double synapses_after{};
    double grown{};
    double activated{};
    double structural_changed{};
    double nonzero{};
    double structural_impulses{};
    double existing_changed{};
    double memory_bytes{};
    double cross_fraction{};
    double positive{};
    double negative{};
    double both{};
    double none{};
    double first_relation_weight_drift{};
    std::uint64_t simultaneous{};
};

Aggregate aggregate(const std::vector<Result>& rows) {
    Aggregate value;
    for (const auto& row : rows) {
        value.x += success_x(row); value.y += success_y(row);
        value.score += balanced(row); value.reward += row.evaluation_reward;
        value.synapses_before += row.initial_synapses;
        value.synapses_after += row.final_synapses;
        value.grown += row.grown_synapses;
        value.activated += row.structural_synapses_activated;
        value.structural_changed += row.structural_synapses_changed;
        value.nonzero += row.structural_synapses_nonzero;
        value.structural_impulses += row.structural_impulses;
        value.existing_changed += row.existing_synapses_changed;
        value.memory_bytes += static_cast<double>(row.final_memory_bytes);
        value.cross_fraction += safe_ratio(
            row.cross_cue_candidates, row.eligibility_candidates);
        value.positive += row.positive_changes; value.negative += row.negative_changes;
        value.both += row.both; value.none += row.none;
        value.simultaneous += success_x(row) >= 0.75 && success_y(row) >= 0.75;
    }
    const double count = static_cast<double>(rows.size());
    value.x /= count; value.y /= count; value.score /= count; value.reward /= count;
    value.synapses_before /= count; value.synapses_after /= count;
    value.grown /= count; value.activated /= count;
    value.structural_changed /= count; value.nonzero /= count;
    value.structural_impulses /= count; value.existing_changed /= count;
    value.memory_bytes /= count;
    value.cross_fraction /= count; value.positive /= count; value.negative /= count;
    value.both /= count; value.none /= count;
    return value;
}

void write_successful_seed_analysis(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<Result>>& results) {
    std::ofstream detail(output / "b43-successful-seeds.csv");
    detail << "order,seed,successful,success_X,success_Y,balanced,weight_XA_before_second,weight_XB_before_second,weight_YA_before_second,weight_YB_before_second,weight_XA_final,weight_XB_final,weight_YA_final,weight_YB_final,first_relation_weight_drift,positive_changes,negative_changes,cross_candidate_fraction,both,none\n";
    std::ofstream groups(output / "b43-success-groups.csv");
    groups << "order,group,seeds,success_X,success_Y,balanced,first_relation_weight_drift,positive_changes,negative_changes,cross_candidate_fraction,both,none\n";
    for (const auto& [order, name] : std::vector<std::pair<std::string, std::string>>{
             {"X_then_Y", "S0_X_then_Y"}, {"Y_then_X", "S0_Y_then_X"}}) {
        std::array<Aggregate, 2> grouped{};
        std::array<std::uint64_t, 2> counts{};
        const auto& rows = results.at(name);
        for (std::size_t index = 0; index < rows.size(); ++index) {
            const auto& row = rows[index];
            const bool successful = success_x(row) >= 0.75 && success_y(row) >= 0.75;
            const double first_drift = order == "X_then_Y"
                ? std::abs(row.cue0_to_a - row.block_boundary_cue0_to_a) +
                  std::abs(row.cue0_to_b - row.block_boundary_cue0_to_b)
                : std::abs(row.cue1_to_a - row.block_boundary_cue1_to_a) +
                  std::abs(row.cue1_to_b - row.block_boundary_cue1_to_b);
            detail << order << ',' << index + 1 << ',' << successful << ','
                   << success_x(row) << ',' << success_y(row) << ',' << balanced(row)
                   << ',' << row.block_boundary_cue0_to_a << ','
                   << row.block_boundary_cue0_to_b << ','
                   << row.block_boundary_cue1_to_a << ','
                   << row.block_boundary_cue1_to_b << ','
                   << row.cue0_to_a << ',' << row.cue0_to_b << ','
                   << row.cue1_to_a << ',' << row.cue1_to_b << ',' << first_drift << ','
                   << row.positive_changes << ',' << row.negative_changes << ','
                   << safe_ratio(row.cross_cue_candidates, row.eligibility_candidates)
                   << ',' << row.both << ',' << row.none << '\n';
            const std::size_t group = successful ? 1 : 0;
            ++counts[group];
            grouped[group].x += success_x(row); grouped[group].y += success_y(row);
            grouped[group].score += balanced(row);
            grouped[group].first_relation_weight_drift += first_drift;
            grouped[group].positive += row.positive_changes;
            grouped[group].negative += row.negative_changes;
            grouped[group].cross_fraction += safe_ratio(
                row.cross_cue_candidates, row.eligibility_candidates);
            grouped[group].both += row.both; grouped[group].none += row.none;
        }
        for (std::size_t group = 0; group < 2; ++group) {
            const double count = static_cast<double>(counts[group]);
            groups << order << ',' << (group ? "successful" : "unsuccessful") << ','
                   << counts[group] << ',' << grouped[group].x / count << ','
                   << grouped[group].y / count << ',' << grouped[group].score / count
                   << ',' << grouped[group].first_relation_weight_drift / count
                   << ',' << grouped[group].positive / count << ','
                   << grouped[group].negative / count << ','
                   << grouped[group].cross_fraction / count << ','
                   << grouped[group].both / count << ',' << grouped[group].none / count
                   << '\n';
        }
    }
}

void write_persistence_probe(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,growth_enabled,synapses,exact_restore,continuation_equal\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        Variant variant = b44_base("persistence");
        variant.structural_growth = true;
        eve::bios::Runtime original(make_config(variant, false), 0x4600000ULL + seed);
        static_cast<void>(configure_circuit(original, variant, seed, false));
        original.enqueue_fire(0); static_cast<void>(original.run());
        original.enqueue_fire(2); static_cast<void>(original.run());
        const auto path = std::filesystem::temp_directory_path() /
            ("eve-b44-persistence-" + std::to_string(seed) + ".bin");
        original.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = original.state_fingerprint() == restored.state_fingerprint();
        original.enqueue_fire(1); restored.enqueue_fire(1);
        static_cast<void>(original.run()); static_cast<void>(restored.run());
        const bool continuation = original.state_fingerprint() == restored.state_fingerprint();
        file << seed << ',' << restored.config().growth_enabled << ','
             << restored.network().synapse_count() << ',' << exact << ','
             << continuation << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b44");
    std::filesystem::create_directories(output);
    const auto matrix = b44_matrix();
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    std::ofstream parameters(output / "parameters.csv");
    parameters << "variant,group,task,seeds,training,evaluation,growth,static_extra,selection,growth_seed_base\n";
    std::ofstream trials(output / "trials.csv");
    trials << "variant,seed,episode,phase,cue,target,outcome,reward,explored,post_feedback_a,post_feedback_b,start_time,end_time,positive_changes,negative_changes\n";
    std::ofstream weights(output / "weight-changes.csv");
    weights << "variant,seed,episode,synapse,sender,receiver,before,after,protected\n";
    std::ofstream growth(output / "growth-events.csv");
    growth << "variant,seed,episode,kind,synapse,sender,receiver,weight_after_episode\n";
    b44_growth_diagnostics = &growth;

    std::map<std::string, std::vector<Result>> results;
    for (const auto& item : matrix) {
        parameters << item.variant.name << ',' << item.group << ',' << item.task_name
                   << ",64," << item.training << ",64,"
                   << item.variant.structural_growth << ','
                   << item.variant.static_extra_synapses << ','
                   << static_cast<unsigned>(item.variant.eligibility_selection) << ','
                   << "(0x1700000+seed)^0xd1b54a32d192ed03\n";
        auto& rows = results[item.variant.name];
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            rows.push_back(run_seed(item.variant, seed, item.training, 64, false,
                                    &trials, &weights, nullptr));
        }
    }
    b44_growth_diagnostics = nullptr;

    std::ofstream summary(output / "summary.csv");
    summary << "variant,group,task,seeds,success_X,success_Y,balanced,reward,simultaneous_75,synapses_before,synapses_after,grown,structural_activated,structural_changed,structural_nonzero,structural_impulses,existing_changed,final_memory_bytes,cross_candidate_fraction,positive_changes,negative_changes,both,none\n";
    summary << std::fixed << std::setprecision(6);
    std::ofstream seed_rows(output / "seed-results.csv");
    seed_rows << "variant,group,task,seed,success_X,success_Y,balanced,reward,simultaneous_75,synapses_before,synapses_after,grown,structural_activated,structural_changed,structural_nonzero,structural_impulses,existing_changed,final_memory_bytes,minimum_weight,maximum_weight,cross_candidate_fraction,positive_changes,negative_changes,both,none\n";
    for (const auto& item : matrix) {
        const auto& rows = results.at(item.variant.name);
        const auto total = aggregate(rows);
        summary << item.variant.name << ',' << item.group << ',' << item.task_name
                << ',' << rows.size() << ',' << total.x << ',' << total.y << ','
                << total.score << ',' << total.reward << ',' << total.simultaneous
                << ',' << total.synapses_before << ',' << total.synapses_after << ','
                << total.grown << ',' << total.activated << ','
                << total.structural_changed << ',' << total.nonzero << ','
                << total.structural_impulses << ',' << total.existing_changed << ','
                << total.memory_bytes << ',' << total.cross_fraction << ',' << total.positive << ','
                << total.negative << ',' << total.both << ',' << total.none << '\n';
        for (std::size_t index = 0; index < rows.size(); ++index) {
            const auto& row = rows[index];
            seed_rows << item.variant.name << ',' << item.group << ',' << item.task_name
                      << ',' << index + 1 << ',' << success_x(row) << ','
                      << success_y(row) << ',' << balanced(row) << ','
                      << row.evaluation_reward << ','
                      << (success_x(row) >= .75 && success_y(row) >= .75) << ','
                      << row.initial_synapses << ',' << row.final_synapses << ','
                      << row.grown_synapses << ',' << row.structural_synapses_activated
                      << ',' << row.structural_synapses_changed << ','
                      << row.structural_synapses_nonzero << ','
                      << row.structural_impulses << ',' << row.existing_synapses_changed
                      << ',' << row.final_memory_bytes << ',' << row.minimum_final_weight
                      << ',' << row.maximum_final_weight << ','
                      << safe_ratio(row.cross_cue_candidates,
                                           row.eligibility_candidates)
                      << ',' << row.positive_changes << ',' << row.negative_changes
                      << ',' << row.both << ',' << row.none << '\n';
        }
    }

    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "left,right,task,seeds,mean_difference,median_difference,bootstrap_low,bootstrap_high,positive_fraction\n";
    for (const auto& task : {"X_then_Y", "Y_then_X", "alternating", "mixed"}) {
        for (const auto& group : {"S1", "S2", "S3", "S4"}) {
            const std::string left_name = std::string(group) + '_' + task;
            const std::string right_name = std::string("S0_") + task;
            std::vector<double> differences;
            std::uint64_t positive = 0;
            for (std::size_t index = 0; index < 64; ++index) {
                differences.push_back(balanced(results.at(left_name)[index]) -
                                      balanced(results.at(right_name)[index]));
                positive += differences.back() > 0;
            }
            const auto interval = bootstrap_interval(
                differences, 0x4610000ULL + left_name.size() + task[0]);
            paired << left_name << ',' << right_name << ',' << task << ",64,"
                   << mean(differences) << ',' << median(differences) << ','
                   << interval.first << ',' << interval.second << ','
                   << static_cast<double>(positive) / 64.0 << '\n';
        }
        for (const auto& group : {"S0", "S1", "S2", "S3", "S4"}) {
            const std::string left_name = std::string(group) + '_' + task;
            const std::string right_name = std::string("N0_no_learning_") + task;
            std::vector<double> differences;
            std::uint64_t positive = 0;
            for (std::size_t index = 0; index < 64; ++index) {
                differences.push_back(balanced(results.at(left_name)[index]) -
                                      balanced(results.at(right_name)[index]));
                positive += differences.back() > 0;
            }
            const auto interval = bootstrap_interval(
                differences, 0x4620000ULL + left_name.size() + task[0]);
            paired << left_name << ',' << right_name << ',' << task << ",64,"
                   << mean(differences) << ',' << median(differences) << ','
                   << interval.first << ',' << interval.second << ','
                   << static_cast<double>(positive) / 64.0 << '\n';
        }
    }

    write_successful_seed_analysis(output, results);

    // Pre-registered confirmation set: all structural sequential/mixed groups,
    // irrespective of discovery-set rank, so selection cannot tune the result.
    std::ofstream confirmation(output / "confirmation.csv");
    confirmation << "variant,seed,success_X,success_Y,balanced,simultaneous_75,grown,structural_nonzero\n";
    for (const auto& item : matrix) {
        if ((item.group != "S1" && item.group != "S2" &&
             item.group != "S3" && item.group != "S4") ||
            (item.task_name != "X_then_Y" && item.task_name != "Y_then_X" &&
             item.task_name != "mixed")) continue;
        for (std::uint64_t seed = 1001; seed <= 1064; ++seed) {
            const auto row = run_seed(
                item.variant, seed, item.training, 64, false, nullptr, nullptr, nullptr);
            confirmation << item.variant.name << ',' << seed << ',' << success_x(row)
                         << ',' << success_y(row) << ',' << balanced(row) << ','
                         << (success_x(row) >= .75 && success_y(row) >= .75) << ','
                         << row.grown_synapses << ','
                         << row.structural_synapses_nonzero << '\n';
        }
    }

    write_persistence_probe(output);
    std::ofstream causal(output / "causal-diagnosis.csv");
    causal << "task,seed,S3_without_growth,S4_with_growth,difference,grown_synapses,nonzero_new_synapses,interpretation\n";
    for (const auto& task : {"X_then_Y", "Y_then_X", "alternating", "mixed"}) {
        const auto& without = results.at(std::string("S3_") + task);
        const auto& with = results.at(std::string("S4_") + task);
        for (std::size_t index = 0; index < 64; ++index) {
            causal << task << ',' << index + 1 << ',' << balanced(without[index])
                   << ',' << balanced(with[index]) << ','
                   << balanced(with[index]) - balanced(without[index]) << ','
                   << with[index].grown_synapses << ','
                   << with[index].structural_synapses_nonzero << ','
                   << (with[index].structural_synapses_nonzero == 0 &&
                               balanced(with[index]) == balanced(without[index])
                           ? "growth_functionally_inert" : "inspect") << '\n';
        }
    }

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
    const double wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const double cpu = static_cast<double>(std::clock() - cpu_started) / CLOCKS_PER_SEC;
    struct rusage usage {};
    static_cast<void>(getrusage(RUSAGE_SELF, &usage));
    std::ofstream performance(output / "performance.csv");
    performance << "variants,seeds,experiences,events,wall_seconds,cpu_seconds,events_per_cpu_second,maximum_queue,peak_rss_kib\n";
    performance << matrix.size() << ",64," << experiences << ',' << events << ','
                << wall << ',' << cpu << ',' << events / std::max(.000001, cpu)
                << ',' << maximum_queue << ',' << usage.ru_maxrss << '\n';

    std::ofstream controls(output / "controls.csv");
    controls << "control,status\nS0,executed\nS1,executed\nS2,executed\nS3,executed\nS4,executed\nN0,executed\nN1,executed\n";

    std::ofstream resources(output / "resources.csv");
    resources << "variant,group,task,seeds,mean_synapses_before,mean_synapses_after,mean_final_memory_bytes,mean_processed_events,mean_maximum_queue\n";
    for (const auto& item : matrix) {
        const auto& rows = results.at(item.variant.name);
        double processed = 0.0;
        double queue = 0.0;
        for (const auto& row : rows) {
            processed += static_cast<double>(row.processed_events);
            queue += static_cast<double>(row.maximum_queue);
        }
        const auto total = aggregate(rows);
        resources << item.variant.name << ',' << item.group << ',' << item.task_name
                  << ',' << rows.size() << ',' << total.synapses_before << ','
                  << total.synapses_after << ',' << total.memory_bytes << ','
                  << processed / rows.size() << ',' << queue / rows.size() << '\n';
    }

    std::ofstream stability(output / "stability.csv");
    stability << "variant,seeds,minimum_weight,maximum_weight,invalid_weight_runs,invalid_network_runs\n";
    for (const auto& item : matrix) {
        const auto& rows = results.at(item.variant.name);
        std::int32_t minimum = std::numeric_limits<std::int32_t>::max();
        std::int32_t maximum = std::numeric_limits<std::int32_t>::min();
        std::uint64_t invalid_weights = 0;
        std::uint64_t invalid_networks = 0;
        for (const auto& row : rows) {
            minimum = std::min(minimum, row.minimum_final_weight);
            maximum = std::max(maximum, row.maximum_final_weight);
            invalid_weights += row.minimum_final_weight < 0 ||
                row.maximum_final_weight > 8;
            invalid_networks += !row.network_valid;
        }
        stability << item.variant.name << ',' << rows.size() << ',' << minimum << ','
                  << maximum << ',' << invalid_weights << ',' << invalid_networks << '\n';
    }

    std::ofstream regressions(output / "regressions.csv");
    regressions << "check,value,expected,status\n";
    regressions << "S0_only_X_success," << aggregate(results.at("S0_only_X")).x
                << ",1," << (aggregate(results.at("S0_only_X")).x == 1.0 ? "pass" : "fail") << '\n';
    regressions << "S0_only_Y_success," << aggregate(results.at("S0_only_Y")).y
                << ",1," << (aggregate(results.at("S0_only_Y")).y == 1.0 ? "pass" : "fail") << '\n';
    regressions << "S1_equals_S0_X_then_Y,"
                << (aggregate(results.at("S1_X_then_Y")).score == aggregate(results.at("S0_X_then_Y")).score)
                << ",1,"
                << (aggregate(results.at("S1_X_then_Y")).score == aggregate(results.at("S0_X_then_Y")).score ? "pass" : "fail") << '\n';
    regressions << "S4_equals_S3_X_then_Y,"
                << (aggregate(results.at("S4_X_then_Y")).score == aggregate(results.at("S3_X_then_Y")).score)
                << ",1,"
                << (aggregate(results.at("S4_X_then_Y")).score == aggregate(results.at("S3_X_then_Y")).score ? "pass" : "fail") << '\n';
    std::cout << "B.4.4 research data written to " << output << '\n';
    return 0;
}
