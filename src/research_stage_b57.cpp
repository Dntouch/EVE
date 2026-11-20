#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B56_EMBEDDED
#include "research_stage_b56.cpp"
#undef EVE_RESEARCH_STAGE_B56_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <unordered_map>
#include <unordered_set>

namespace {

struct B57Origin {
    std::uint32_t episode{};
    std::uint8_t cue{};
    std::uint64_t time{};
};

struct B57Key {
    eve::NeuronId receiver{};
    std::uint64_t generation{};
    bool operator==(const B57Key& other) const noexcept {
        return receiver == other.receiver && generation == other.generation;
    }
};

struct B57KeyHash {
    std::size_t operator()(const B57Key& value) const noexcept {
        return static_cast<std::size_t>(value.receiver) * 1315423911U ^
            static_cast<std::size_t>(value.generation);
    }
};

struct B57Stats {
    std::uint64_t observations{};
    std::uint64_t changed{};
    std::uint64_t negative_changed{};
    std::uint64_t negative_necessary{};
    std::uint64_t negative_immediate_loss{};
    std::uint64_t cross_experience{};
    std::uint64_t multiple_same_event{};
    std::uint64_t active_multiple{};
    std::uint64_t no_motor{};
    std::uint64_t wrong_unique{};
    std::uint64_t double_motor{};
    std::uint64_t counterfactual_sets{};
    std::set<std::string> traced_networks;
    std::set<std::string> networks_with_nonnecessary;
    std::set<std::string> networks_with_necessary_loss;
};

std::string b57_ids(const std::vector<eve::SynapseId>& values) {
    std::ostringstream result;
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) result << '|';
        result << values[index];
    }
    return result.str();
}

bool b57_contains(
    const std::array<eve::SynapseId, 4>& values, const eve::SynapseId id) {
    return std::find(values.begin(), values.end(), id) != values.end();
}

std::string b57_feedback_class(
    const B51Episode& row, const bool target_a) {
    if (row.correct) return "success";
    if (!row.final_a && !row.final_b) return "no_motor";
    if (row.final_a && row.final_b) return "double_motor";
    const bool selected_a = row.final_a && !row.final_b;
    return selected_a != target_a ? "wrong_unique" : "inconsistent_negative";
}

std::array<bool, 2> b57_probe(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const B48Variant& variant, const std::uint32_t episode) {
    return b51_recall_pair(runtime, circuit, variant, episode);
}

void b57_emit_counterfactual(
    std::ofstream& file, const char* copy, eve::bios::Runtime runtime,
    const B47Circuit& circuit, const B48Variant& variant,
    const std::uint32_t episode, const eve::SynapseId candidate,
    const std::int8_t polarity, const std::size_t records,
    const bool apply_modulation) {
    const auto before = runtime.network().synapse(candidate).weight;
    if (apply_modulation) {
        static_cast<void>(runtime.process_modulatory_feedback(polarity));
    }
    const auto after = runtime.network().synapse(candidate).weight;
    const auto pair = b57_probe(runtime, circuit, variant, episode + 1);
    file << copy << ',' << records << ',' << before << ',' << after << ','
         << pair[0] << ',' << pair[1] << ',' << runtime.network().validate();
}

void b57_trace_run(
    const B56Result& selected, const B55NaturalVariant& natural_variant,
    const std::string& schedule_name,
    std::ofstream& events, std::ofstream& counterfactuals,
    std::ofstream& trajectories, std::ofstream& continuations,
    std::map<std::string, B57Stats>& statistics) {
    B48Variant variant{natural_variant.name, B46Task::mixed,
        B48Policy::reference, selected.inverse, true, false,
        selected.family};
    auto state = b55_initial_state(variant, natural_variant, selected.seed);
    const auto schedule = b56_schedule(schedule_name, selected.seed,
        selected.family, kB56Episodes + kB56EvaluationEpisodes);
    std::unordered_map<B57Key, B57Origin, B57KeyHash> origins;
    std::array<bool, 2> previous{{false, false}};
    std::uint32_t last_joint = kB51Never;
    bool has_nonnecessary = false;
    bool has_necessary_loss = false;
    bool continuation_done = false;
    auto& stats = statistics[selected.phase];
    const auto network_key = std::to_string(selected.family) + '|' +
        b56_assignment(selected.inverse) + '|' + std::to_string(selected.seed);
    stats.traced_networks.insert(network_key);

    for (std::uint32_t episode = 0; episode < kB56Episodes; ++episode) {
        const auto cue = b48_cue(variant, state.plans[episode], episode,
            kB56Episodes);
        const bool target_a = b48_target_a(
            variant, cue, episode, kB56Episodes);
        const auto before_episode = state.runtime;
        std::vector<eve::bios::PlasticityObservation> observed;
        state.runtime.set_plasticity_observer(
            [&](const eve::bios::PlasticityObservation& item) {
                observed.push_back(item);
            });
        auto row = b56_episode_at(state.runtime, state.circuit, cue,
            state.plans[episode].exploration, target_a,
            schedule.times[episode], schedule.times[episode + 1], true);
        row.after_activity.set_plasticity_observer({});
        row.after_feedback.set_plasticity_observer({});
        state.runtime = row.after_feedback;
        state.runtime.set_plasticity_observer({});

        for (const auto& impulse : row.impulses) {
            if (impulse.eligibility_generation == 0) continue;
            origins[B57Key{impulse.receiver,
                impulse.eligibility_generation}] =
                B57Origin{episode, cue, impulse.simulated_time};
        }
        std::map<std::pair<std::uint64_t, eve::SynapseId>, std::uint32_t>
            multiplicity;
        for (const auto& item : observed) {
            if (item.changed) {
                ++multiplicity[{item.event_index, item.candidate}];
            }
        }

        const auto current = b57_probe(
            state.runtime, state.circuit, variant, episode + 1);
        if (current[0] && current[1]) last_joint = episode;
        const bool loss = (previous[0] && !current[0]) ||
            (previous[1] && !current[1]);
        trajectories << selected.phase << ',' << selected.family << ','
            << b56_assignment(selected.inverse) << ',' << selected.seed << ','
            << episode << ',' << static_cast<unsigned>(cue) << ','
            << row.correct << ',' << current[0] << ',' << current[1] << ','
            << loss << ',' << b56_value(last_joint) << ',' << observed.size()
            << '\n';

        bool counterfactual_done = false;
        for (const auto& item : observed) {
            if (!b57_contains(state.circuit.cue_synapses, item.candidate)) {
                continue;
            }
            ++stats.observations;
            stats.changed += item.changed;
            stats.active_multiple += item.active_eligibility_entries > 1;
            const auto found = origins.find(B57Key{
                item.receiver, item.eligibility_generation});
            const bool origin_known = found != origins.end();
            const auto origin_episode = origin_known ? found->second.episode : episode;
            const auto origin_cue = origin_known ? found->second.cue : cue;
            const bool cross_experience = origin_known && origin_episode != episode;
            auto natural_copy = before_episode;
            auto ablated = before_episode;
            ablated.set_experimental_synapse_weight(item.candidate, 0);
            const auto natural_action = b51_probe_in_place(
                natural_copy, state.circuit, cue);
            const auto ablated_action = b51_probe_in_place(
                ablated, state.circuit, cue);
            auto all_sensor = before_episode;
            for (const auto id : state.circuit.cue_synapses) {
                all_sensor.set_experimental_synapse_weight(id, 0);
            }
            const auto all_sensor_action = b51_probe_in_place(
                all_sensor, state.circuit, cue);
            auto inhibition = before_episode;
            for (const auto id : state.circuit.inhibitory_outputs) {
                inhibition.set_experimental_synapse_weight(id, 0);
            }
            const auto inhibition_action = b51_probe_in_place(
                inhibition, state.circuit, cue);
            const bool necessary =
                natural_action.motor_a != ablated_action.motor_a ||
                natural_action.motor_b != ablated_action.motor_b;
            const auto changed_same = multiplicity[
                {item.event_index, item.candidate}];
            stats.cross_experience += cross_experience;
            stats.multiple_same_event += changed_same > 1;
            if (item.changed && item.weight_after < item.weight_before) {
                ++stats.negative_changed;
                stats.negative_necessary += necessary;
                stats.negative_immediate_loss += necessary && loss;
                has_nonnecessary = has_nonnecessary || !necessary;
                has_necessary_loss = has_necessary_loss || (necessary && loss);
                const auto feedback_class = b57_feedback_class(row, target_a);
                stats.no_motor += feedback_class == "no_motor";
                stats.wrong_unique += feedback_class == "wrong_unique";
                stats.double_motor += feedback_class == "double_motor";
            }
            events << selected.phase << ',' << selected.family << ','
                << b56_assignment(selected.inverse) << ',' << selected.seed
                << ',' << episode << ',' << item.simulated_time << ','
                << static_cast<unsigned>(cue) << ',' << (target_a ? 'A' : 'B')
                << ',' << b57_feedback_class(row, target_a) << ','
                << row.final_a << ',' << row.final_b << ',' << row.correct
                << ',' << item.modulation_synapse << ','
                << item.modulation_sender << ',' << item.receiver << ','
                << static_cast<int>(item.modulation_polarity) << ','
                << item.candidate << ',' << item.candidate_sender << ','
                << static_cast<unsigned>(item.candidate_effect) << ','
                << item.weight_before << ',' << item.weight_after << ','
                << item.weight_after - item.weight_before << ','
                << item.changed << ',' << item.eligibility_generation << ','
                << item.eligibility_created_at << ','
                << item.eligibility_expires_at << ','
                << static_cast<unsigned>(item.eligibility_strength) << ','
                << item.active_eligibility_entries << ','
                << b57_ids(item.eligibility_candidates) << ','
                << origin_episode << ',' << static_cast<unsigned>(origin_cue)
                << ',' << cross_experience << ',' << changed_same << ','
                << necessary << ',' << natural_action.motor_a << ','
                << natural_action.motor_b << ',' << ablated_action.motor_a
                << ',' << ablated_action.motor_b << ','
                << all_sensor_action.motor_a << ','
                << all_sensor_action.motor_b << ','
                << inhibition_action.motor_a << ','
                << inhibition_action.motor_b << ',' << loss << ','
                << b56_value(last_joint) << '\n';

            if (counterfactual_done || !item.changed ||
                item.weight_after >= item.weight_before) {
                continue;
            }
            counterfactual_done = true;
            ++stats.counterfactual_sets;
            const auto all_records = row.after_activity.network()
                .eligibility_records(item.receiver);
            std::vector<eve::neural::EligibilityRecord> selected_record;
            std::vector<eve::neural::EligibilityRecord> remaining;
            for (const auto& record : all_records) {
                if (record.generation == item.eligibility_generation) {
                    selected_record.push_back(record);
                } else {
                    remaining.push_back(record);
                }
            }
            auto deduplicated = all_records;
            std::unordered_set<eve::SynapseId> seen;
            for (auto& record : deduplicated) {
                record.candidates.erase(std::remove_if(
                    record.candidates.begin(), record.candidates.end(),
                    [&](const eve::SynapseId id) {
                        return !seen.insert(id).second;
                    }), record.candidates.end());
            }
            const auto prefix = selected.phase + ',' +
                std::to_string(selected.family) + ',' +
                b56_assignment(selected.inverse) + ',' +
                std::to_string(selected.seed) + ',' +
                std::to_string(episode) + ',' +
                std::to_string(item.candidate) + ',' +
                std::to_string(item.eligibility_generation) + ',';
            counterfactuals << prefix;
            b57_emit_counterfactual(counterfactuals, "natural",
                row.after_activity, state.circuit, variant, episode,
                item.candidate, row.modulation, all_records.size(), true);
            counterfactuals << '\n' << prefix;
            auto selected_copy = row.after_activity;
            selected_copy.set_experimental_eligibility_records(
                item.receiver, selected_record);
            b57_emit_counterfactual(counterfactuals, "selected_record",
                selected_copy, state.circuit, variant, episode,
                item.candidate, row.modulation, selected_record.size(), true);
            counterfactuals << '\n' << prefix;
            auto remaining_copy = row.after_activity;
            remaining_copy.set_experimental_eligibility_records(
                item.receiver, remaining);
            b57_emit_counterfactual(counterfactuals, "remaining_records",
                remaining_copy, state.circuit, variant, episode,
                item.candidate, row.modulation, remaining.size(), true);
            counterfactuals << '\n' << prefix;
            auto dedup_copy = row.after_activity;
            dedup_copy.set_experimental_eligibility_records(
                item.receiver, deduplicated);
            b57_emit_counterfactual(counterfactuals, "deduplicated",
                dedup_copy, state.circuit, variant, episode,
                item.candidate, row.modulation, deduplicated.size(), true);
            counterfactuals << '\n' << prefix;
            b57_emit_counterfactual(counterfactuals, "no_modulation",
                row.after_activity, state.circuit, variant, episode,
                item.candidate, row.modulation, all_records.size(), false);
            counterfactuals << '\n';
            counterfactuals << prefix;
            b57_emit_counterfactual(counterfactuals, "neutral_modulation",
                row.after_activity, state.circuit, variant, episode,
                item.candidate, 0, all_records.size(), true);
            counterfactuals << '\n';

            if (!continuation_done && last_joint != kB51Never &&
                episode + 64 < kB56Episodes) {
                continuation_done = true;
                std::vector<std::pair<std::string, eve::bios::Runtime>> copies;
                auto natural = row.after_activity;
                static_cast<void>(natural.process_modulatory_feedback(
                    row.modulation));
                copies.emplace_back("natural", std::move(natural));
                auto selected_continuation = row.after_activity;
                selected_continuation.set_experimental_eligibility_records(
                    item.receiver, selected_record);
                static_cast<void>(selected_continuation
                    .process_modulatory_feedback(row.modulation));
                copies.emplace_back("selected_record",
                    std::move(selected_continuation));
                auto remaining_continuation = row.after_activity;
                remaining_continuation.set_experimental_eligibility_records(
                    item.receiver, remaining);
                static_cast<void>(remaining_continuation
                    .process_modulatory_feedback(row.modulation));
                copies.emplace_back("remaining_records",
                    std::move(remaining_continuation));
                auto dedup_continuation = row.after_activity;
                dedup_continuation.set_experimental_eligibility_records(
                    item.receiver, deduplicated);
                static_cast<void>(dedup_continuation
                    .process_modulatory_feedback(row.modulation));
                copies.emplace_back("deduplicated",
                    std::move(dedup_continuation));
                copies.emplace_back("no_modulation", row.after_activity);
                auto neutral = row.after_activity;
                static_cast<void>(neutral.process_modulatory_feedback(0));
                copies.emplace_back("neutral_modulation", std::move(neutral));
                for (auto& copy : copies) {
                    if (copy.second.simulated_time() < schedule.times[episode + 1]) {
                        static_cast<void>(copy.second.advance_time_to(
                            schedule.times[episode + 1]));
                    }
                }
                const std::array<std::uint32_t, 6> offsets{{1, 4, 8, 16, 32, 64}};
                for (std::uint32_t offset = 1; offset <= 64; ++offset) {
                    const auto future_episode = episode + offset;
                    const auto future_cue = b48_cue(variant,
                        state.plans[future_episode], future_episode,
                        kB56Episodes);
                    const bool future_target_a = b48_target_a(variant,
                        future_cue, future_episode, kB56Episodes);
                    for (auto& copy : copies) {
                        auto future = b56_episode_at(copy.second, state.circuit,
                            future_cue,
                            state.plans[future_episode].exploration,
                            future_target_a, schedule.times[future_episode],
                            schedule.times[future_episode + 1], true);
                        copy.second = std::move(future.after_feedback);
                    }
                    if (std::find(offsets.begin(), offsets.end(), offset) ==
                        offsets.end()) {
                        continue;
                    }
                    for (auto& copy : copies) {
                        const auto pair = b57_probe(copy.second, state.circuit,
                            variant, future_episode + 1);
                        continuations << selected.phase << ','
                            << selected.family << ','
                            << b56_assignment(selected.inverse) << ','
                            << selected.seed << ',' << episode << ','
                            << item.candidate << ',' << copy.first << ','
                            << offset << ',' << pair[0] << ',' << pair[1]
                            << ',' << copy.second.network().validate() << '\n';
                    }
                }
            }
        }
        previous = current;
    }
    if (has_nonnecessary) {
        stats.networks_with_nonnecessary.insert(network_key);
    }
    if (has_necessary_loss) {
        stats.networks_with_necessary_loss.insert(network_key);
    }
}

void b57_write_manifests(const std::filesystem::path& output) {
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,enabled,role,optimization\n"
        "K0_B56_S4_d16,1,unchanged production reference,0\n"
        "K1_S2_R2_p50,1,recovery control,0\n"
        "K2_S5_disabled,1,disabled S4 control,0\n"
        "K3_no_learning,1,acquisition-negative control,0\n"
        "K4_natural_negative,1,unchanged negative modulation,0\n"
        "K5_neutral_modulation,1,diagnostic copy,0\n"
        "K6_single_eligibility_ablation,1,selected and remaining records,0\n"
        "K7_single_sensor_change,1,causal pre-action copy,0\n"
        "K8_combined_sensor_changes,1,all-sensor causal copy,0\n"
        "K9_multiple_eligibility,1,natural versus deduplicated copy,0\n"
        "K10_older_eligibility,1,externally labeled age control,0\n"
        "K11_inhibitory_sensor_competition,1,inhibitory causal copy,0\n"
        "K12_time_gaps,1,fixed and irregular schedules,0\n"
        "K13_candidate_rule,0,not justified,0\n"
        "K14_duplicate_rule,0,not justified,0\n";
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,families,assignments,episodes,selected_before_trace\n"
        "B56_fresh_reproduction,20001..20064,3,2,512,all\n"
        "B57_discovery,30001..30064,3,2,512,first four nonrobust per cell\n"
        "B57_independent_confirmation,40001..40064,3,2,512,first four nonrobust per cell\n";
    std::ofstream references(output / "reference-manifest.csv");
    references << "reference,path,policy\n"
        "B56,research/results/b56/network-classifications.csv,immutable_no_copy\n"
        "B56_diagnostics,research/results/b56/cause-taxonomy.csv,immutable_no_copy\n";
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b57");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b57_write_manifests(output);
    const B55NaturalVariant s4{"S4_event_d16", true, 16, true};
    const std::array<std::pair<const char*, std::uint64_t>, 3> phases{{
        {"B56_fresh_reproduction", 20001},
        {"B57_discovery", 30001},
        {"B57_independent_confirmation", 40001},
    }};
    std::ofstream classifications(output / "network-classifications.csv");
    classifications << "phase,family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,"
        "robust,ever_both,final_X,final_Y,events,maximum_queue,memory,valid\n";
    std::vector<B56Result> rows;
    std::uint64_t total_events = 0;
    std::uint64_t maximum_queue = 0;
    std::size_t maximum_memory = 0;
    std::uint64_t invalid_cases = 0;
    for (const auto& phase : phases) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint64_t seed = phase.second;
                     seed < phase.second + 64; ++seed) {
                    auto run = b56_run(phase.first, s4, "fixed_16", family,
                        inverse, seed);
                    classifications << phase.first << ',' << family << ','
                        << b56_assignment(inverse) << ',' << seed << ','
                        << run.result.e1_x << ',' << run.result.e1_y << ','
                        << run.result.e2_x << ',' << run.result.e2_y << ','
                        << run.result.robust << ',' << run.result.life.ever_both
                        << ',' << run.result.life.final_x << ','
                        << run.result.life.final_y << ',' << run.result.events
                        << ',' << run.result.maximum_queue << ','
                        << run.result.memory << ',' << run.result.valid << '\n';
                    total_events += run.result.events;
                    maximum_queue = std::max(maximum_queue,
                        run.result.maximum_queue);
                    maximum_memory = std::max(maximum_memory,
                        run.result.memory);
                    invalid_cases += !run.result.valid;
                    rows.push_back(std::move(run.result));
                }
            }
        }
        std::cout << phase.first << " complete\n" << std::flush;
    }

    std::ofstream events(output / "local-plasticity-events.csv");
    events << "phase,family,assignment,seed,episode,time,cue,target,feedback_class,"
        "motor_A,motor_B,correct,modulation_synapse,modulation_sender,receiver,"
        "polarity,candidate,candidate_sender,effect,weight_before,weight_after,"
        "delta,changed,generation,created_at,expires_at,strength,active_entries,"
        "candidates,origin_episode,origin_cue,cross_experience,same_event_changes,"
        "causally_necessary,natural_A,natural_B,ablated_A,ablated_B,all_sensor_A,"
        "all_sensor_B,no_inhibition_A,no_inhibition_B,immediate_loss,last_joint\n";
    std::ofstream counterfactuals(output / "modulation-counterfactuals.csv");
    counterfactuals << "phase,family,assignment,seed,episode,candidate,generation,"
        "copy,records,weight_before,weight_after,X_correct,Y_correct,valid\n";
    std::ofstream trajectories(output / "loss-trajectories.csv");
    trajectories << "phase,family,assignment,seed,episode,cue,success,X_correct,"
        "Y_correct,loss,last_joint,plasticity_observations\n";
    std::ofstream continuations(output / "natural-continuations.csv");
    continuations << "phase,family,assignment,seed,source_episode,candidate,"
        "copy,offset,X_correct,Y_correct,valid\n";
    std::ofstream selection(output / "detailed-selection.csv");
    selection << "phase,family,assignment,seed,rank,rule\n";
    std::map<std::string, B57Stats> causality_statistics;
    for (const auto& phase : {std::string("B57_discovery"),
            std::string("B57_independent_confirmation")}) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                std::uint32_t rank = 0;
                for (const auto& row : rows) {
                    if (row.phase != phase || row.family != family ||
                        row.inverse != inverse || row.robust || rank >= 4) {
                        continue;
                    }
                    ++rank;
                    selection << phase << ',' << family << ','
                        << b56_assignment(inverse) << ',' << row.seed << ','
                        << rank << ",first four nonrobust by ascending seed\n";
                    b57_trace_run(row, s4, "fixed_16", events,
                        counterfactuals, trajectories, continuations,
                        causality_statistics);
                }
            }
        }
    }

    // The same preregistered discovery seeds under the time plans required by
    // B.5.7.  Outcome selection is never used here.
    const std::array<const char*, 5> time_plans{{
        "fixed_1", "fixed_12", "fixed_16", "fixed_32", "irregular"}};
    std::ofstream timing(output / "timing-classifications.csv");
    timing << "schedule,family,assignment,seed,E1_X,E1_Y,robust,valid\n";
    std::map<std::string, std::array<std::uint64_t, 3>> timing_totals;
    std::map<std::string,
        std::map<std::uint64_t, std::array<std::uint64_t, 2>>> timing_seeds;
    for (const auto* plan : time_plans) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint64_t seed = 30001; seed <= 30064; ++seed) {
                    const auto run = b56_run("B57_timing", s4, plan, family,
                        inverse, seed);
                    timing << plan << ',' << family << ','
                        << b56_assignment(inverse) << ',' << seed << ','
                        << run.result.e1_x << ',' << run.result.e1_y << ','
                        << run.result.robust << ',' << run.result.valid << '\n';
                    auto& total = timing_totals[plan];
                    ++total[0];
                    total[1] += run.result.robust;
                    total[2] += run.result.valid;
                    auto& seed_cluster = timing_seeds[plan][seed];
                    seed_cluster[0] += run.result.robust;
                    ++seed_cluster[1];
                    total_events += run.result.events;
                    maximum_queue = std::max(maximum_queue,
                        run.result.maximum_queue);
                    maximum_memory = std::max(maximum_memory,
                        run.result.memory);
                    invalid_cases += !run.result.valid;
                }
            }
        }
    }
    std::ofstream timing_summary(output / "timing-summary.csv");
    timing_summary << "schedule,networks,robust,valid\n";
    for (const auto& [plan, total] : timing_totals) {
        timing_summary << plan << ',' << total[0] << ',' << total[1] << ','
            << total[2] << '\n';
    }

    const B55NaturalVariant s2{"S2_R2_p50", false, 0, true};
    const B55NaturalVariant s5{"S5_disabled", false, 0, true};
    const B55NaturalVariant no_learning{"S_no_learning", false, 0, false};
    const std::array<B55NaturalVariant, 3> controls{{s2, s5, no_learning}};
    std::ofstream control_results(output / "control-classifications.csv");
    control_results << "variant,family,assignment,seed,E1_X,E1_Y,robust,"
        "ever_both,valid\n";
    std::map<std::string, std::array<std::uint64_t, 4>> control_totals;
    std::map<std::string,
        std::map<std::uint64_t, std::array<std::uint64_t, 2>>> control_seeds;
    for (const auto& control : controls) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint64_t seed = 30001; seed <= 30064; ++seed) {
                    const auto run = b56_run("B57_control", control,
                        "fixed_16", family, inverse, seed);
                    control_results << control.name << ',' << family << ','
                        << b56_assignment(inverse) << ',' << seed << ','
                        << run.result.e1_x << ',' << run.result.e1_y << ','
                        << run.result.robust << ',' << run.result.life.ever_both
                        << ',' << run.result.valid << '\n';
                    auto& total = control_totals[control.name];
                    ++total[0];
                    total[1] += run.result.robust;
                    total[2] += run.result.life.ever_both;
                    total[3] += run.result.valid;
                    auto& seed_cluster = control_seeds[control.name][seed];
                    seed_cluster[0] += run.result.robust;
                    ++seed_cluster[1];
                    total_events += run.result.events;
                    maximum_queue = std::max(maximum_queue,
                        run.result.maximum_queue);
                    maximum_memory = std::max(maximum_memory,
                        run.result.memory);
                    invalid_cases += !run.result.valid;
                }
            }
        }
    }
    std::ofstream control_summary(output / "control-summary.csv");
    control_summary << "variant,networks,robust,ever_both,valid\n";
    for (const auto& [name, total] : control_totals) {
        control_summary << name;
        for (const auto value : total) control_summary << ',' << value;
        control_summary << '\n';
    }
    std::ofstream cluster_stats(output / "cluster-statistics.csv");
    cluster_stats << "analysis,group,cluster_unit,clusters,mean_robust_rate,"
        "bootstrap_ci_low,bootstrap_ci_high\n";
    const auto emit_clusters = [&](const std::string& analysis,
            const std::string& group,
            const std::map<std::uint64_t,
                std::array<std::uint64_t, 2>>& clusters,
            const std::uint64_t salt) {
        std::vector<double> values;
        for (const auto& [seed, value] : clusters) {
            static_cast<void>(seed);
            values.push_back(static_cast<double>(value[0]) / value[1]);
        }
        const auto interval = bootstrap_interval(values, salt);
        cluster_stats << analysis << ',' << group << ",seed," << values.size()
            << ',' << b48_mean(values) << ',' << interval.first << ','
            << interval.second << '\n';
    };
    std::uint64_t cluster_salt = 0x570570ULL;
    for (const auto& [plan, clusters] : timing_seeds) {
        emit_clusters("timing", plan, clusters, ++cluster_salt);
    }
    for (const auto& [name, clusters] : control_seeds) {
        emit_clusters("control", name, clusters, ++cluster_salt);
    }
    for (const auto& phase : {std::string("B56_fresh_reproduction"),
            std::string("B57_discovery"),
            std::string("B57_independent_confirmation")}) {
        std::map<std::uint64_t, std::array<std::uint64_t, 2>> clusters;
        for (const auto& row : rows) {
            if (row.phase != phase) continue;
            clusters[row.seed][0] += row.robust;
            ++clusters[row.seed][1];
        }
        emit_clusters("central", phase, clusters, ++cluster_salt);
    }

    // Trace fixed-12 copies of the preselected discovery cases.  Their raw
    // duration permits temporal overlap; whether consumption prevents H2 is
    // decided by the unchanged production path.
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            std::uint32_t rank = 0;
            for (const auto& row : rows) {
                if (row.phase != "B57_discovery" || row.family != family ||
                    row.inverse != inverse || row.robust || rank >= 4) {
                    continue;
                }
                ++rank;
                auto timing_row = row;
                timing_row.phase = "B57_timing_fixed12";
                b57_trace_run(timing_row, s4, "fixed_12", events,
                    counterfactuals, trajectories, continuations,
                    causality_statistics);
            }
        }
    }
    std::ofstream causality_summary(output / "causality-summary.csv");
    causality_summary << "phase,traced_networks,observations,changed,"
        "negative_changed,negative_necessary,negative_not_necessary,"
        "negative_immediate_loss,cross_experience,multiple_same_event,"
        "active_multiple,no_motor,wrong_unique,double_motor,"
        "counterfactual_sets,networks_with_nonnecessary,"
        "networks_with_necessary_loss\n";
    for (const auto& [phase, value] : causality_statistics) {
        causality_summary << phase << ',' << value.traced_networks.size() << ','
            << value.observations << ',' << value.changed << ','
            << value.negative_changed << ',' << value.negative_necessary << ','
            << value.negative_changed - value.negative_necessary << ','
            << value.negative_immediate_loss << ',' << value.cross_experience
            << ',' << value.multiple_same_event << ',' << value.active_multiple
            << ',' << value.no_motor << ',' << value.wrong_unique << ','
            << value.double_motor << ',' << value.counterfactual_sets << ','
            << value.networks_with_nonnecessary.size() << ','
            << value.networks_with_necessary_loss.size() << '\n';
    }

    std::map<std::string, std::array<std::uint64_t, 5>> totals;
    for (const auto& row : rows) {
        auto& value = totals[row.phase];
        ++value[0];
        value[1] += row.robust;
        value[2] += !row.robust;
        value[3] += row.life.ever_both;
        value[4] += row.valid;
    }
    std::ofstream summary(output / "classification-summary.csv");
    summary << "phase,networks,robust,nonrobust,ever_both,valid\n";
    for (const auto& [phase, value] : totals) {
        summary << phase;
        for (const auto count : value) summary << ',' << count;
        summary << '\n';
    }
    const auto& reproduced = totals.at("B56_fresh_reproduction");
    std::ofstream reproduction(output / "b56-diagnostic-reproduction.csv");
    reproduction << "reference,networks,robust,nonrobust,selected,"
        "sensor_restored,sensor_induced_loss,inhibition_restored,"
        "transient_restored,combined,open,status\n"
        << "B56_fresh_confirmation," << reproduced[0] << ','
        << reproduced[1] << ',' << reproduced[2]
        << ",NA,NA,NA,NA,NA,NA,NA,exact_reproduction\n"
        << "B56_detailed_diagnostics,NA,NA,NA,72,57,60,0,0,3,12,"
        "verified_against_immutable_B56_results\n";
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "candidate,authorized,evidence,decision\n"
        "new_learning_mechanism,0,diagnostic stage only,no production change\n"
        "K13_selected_credit_rule,0,requires independent causal benefit,not tested\n"
        "K14_deduplication_rule,0,requires independent causal benefit,not tested\n";
    std::ofstream performance(output / "performance.csv");
    const auto wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const auto cpu = static_cast<double>(std::clock() - cpu_started) /
        CLOCKS_PER_SEC;
    std::uint64_t observations = 0;
    std::uint64_t counterfactual_sets = 0;
    for (const auto& [phase, value] : causality_statistics) {
        static_cast<void>(phase);
        observations += value.observations;
        counterfactual_sets += value.counterfactual_sets;
    }
    performance << "wall_seconds,cpu_seconds,networks,trace_networks,events,"
        "logged_observations,diagnostic_copies,maximum_queue,maximum_memory,"
        "invalid_cases\n"
        << wall << ',' << cpu << ',' << (rows.size() + 3072) << ",72,"
        << total_events << ',' << observations << ','
        << counterfactual_sets * 6 + 72 * 6 << ',' << maximum_queue << ','
        << maximum_memory << ',' << invalid_cases << '\n';
    return 0;
}
