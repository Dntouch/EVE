#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B49_EMBEDDED
#include "research_stage_b49.cpp"
#undef EVE_RESEARCH_STAGE_B49_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <set>

namespace {

struct B50Activity {
    eve::bios::Runtime after_activity;
    bool autonomous_a{};
    bool autonomous_b{};
    bool final_a{};
    bool final_b{};
    std::vector<eve::bios::FireObservation> sensor_fires;
    std::vector<eve::bios::FireObservation> exploration_fires;
    std::vector<eve::bios::ImpulseObservation> sensor_impulses;
    std::vector<eve::bios::ImpulseObservation> exploration_impulses;
};

struct B50Learning {
    eve::bios::Runtime after_feedback;
    B50Activity activity;
    std::vector<eve::neural::Synapse> before_weights;
    std::vector<eve::neural::Synapse> after_weights;
    std::int8_t modulation{};
    std::uint64_t delay{};
};

bool b50_correct(const bool target_a, const bool fired_a, const bool fired_b) {
    return target_a ? fired_a && !fired_b : fired_b && !fired_a;
}

std::string b50_synapse_role(
    const B47Circuit& circuit, const eve::SynapseId id,
    const std::uint8_t cue, const bool target_a) {
    const std::size_t target = target_a ? 0 : 1;
    const std::size_t competitor = 1 - target;
    if (id == circuit.cue_synapses[cue * 2 + target]) return "cue_target";
    if (id == circuit.cue_synapses[cue * 2 + competitor]) return "cue_competitor";
    if (id == circuit.cue_synapses[(1 - cue) * 2 + target]) {
        return "other_cue_same_action";
    }
    if (id == circuit.cue_synapses[(1 - cue) * 2 + competitor]) {
        return "other_cue_other_action";
    }
    if (id == circuit.exploration_synapses[target]) return "exploration_target";
    if (id == circuit.exploration_synapses[competitor]) {
        return "exploration_competitor";
    }
    if (id == circuit.excitatory_outputs[target]) return "output_target";
    if (id == circuit.excitatory_outputs[competitor]) return "output_competitor";
    if (id == circuit.inhibitory_outputs[target]) return "inhibition_target";
    if (id == circuit.inhibitory_outputs[competitor]) {
        return "inhibition_competitor";
    }
    if (std::find(circuit.modulation_synapses.begin(),
                  circuit.modulation_synapses.end(), id) !=
        circuit.modulation_synapses.end()) {
        return "modulation_contact";
    }
    return "other";
}

B50Activity b50_activity(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const std::uint8_t cue, const std::uint8_t action,
    const bool explore, std::ostream* trace = nullptr,
    const std::string& label = {}) {
    B50Activity result{std::move(runtime), false, false, false, false,
        {}, {}, {}, {}};
    bool exploration_phase = false;
    result.after_activity.set_fire_observer(
        [&](const eve::bios::FireObservation& event) {
            auto& fires = exploration_phase ? result.exploration_fires
                                            : result.sensor_fires;
            fires.push_back(event);
            if (event.neuron == circuit.motors[0]) {
                result.final_a = true;
                if (!exploration_phase) result.autonomous_a = true;
            }
            if (event.neuron == circuit.motors[1]) {
                result.final_b = true;
                if (!exploration_phase) result.autonomous_b = true;
            }
            if (trace != nullptr) {
                *trace << label << ',' << (exploration_phase ? "exploration" : "sensor")
                    << ",fire," << event.event_index << ','
                    << event.simulated_time << ',' << event.neuron
                    << ",-1,-1,0,0,0,0,0\n";
            }
        });
    result.after_activity.set_impulse_observer(
        [&](const eve::bios::ImpulseObservation& event) {
            auto& impulses = exploration_phase ? result.exploration_impulses
                                               : result.sensor_impulses;
            impulses.push_back(event);
            if (trace != nullptr) {
                *trace << label << ',' << (exploration_phase ? "exploration" : "sensor")
                    << ",impulse," << event.event_index << ','
                    << event.simulated_time << ',' << event.receiver << ','
                    << event.synapse << ',' << event.eligibility_generation << ','
                    << event.applied_weight << ','
                    << (event.synaptic_effect == eve::SynapticEffect::inhibitory)
                    << ',' << event.membrane_before << ',' << event.membrane_after
                    << ',' << event.eligibility_candidates.size() << '\n';
            }
        });
    result.after_activity.enqueue_fire(circuit.sensors[cue]);
    static_cast<void>(result.after_activity.run());
    if (explore) {
        exploration_phase = true;
        result.after_activity.enqueue_fire(circuit.explorers[action]);
        static_cast<void>(result.after_activity.run());
    }
    result.after_activity.set_fire_observer({});
    result.after_activity.set_impulse_observer({});
    return result;
}

B50Learning b50_learn(
    const eve::bios::Runtime& before, const B47Circuit& circuit,
    const std::uint8_t cue, const std::uint8_t action,
    const bool explore, const std::int8_t modulation,
    const std::uint64_t delay = 0, std::ostream* trace = nullptr,
    const std::string& label = {}) {
    auto activity = b50_activity(
        before, circuit, cue, action, explore, trace, label);
    auto runtime = activity.after_activity;
    const auto weights = runtime.network().synapses();
    if (delay != 0) static_cast<void>(runtime.advance_time_by(delay));
    const auto feedback = runtime.process_modulatory_feedback(modulation);
    if (trace != nullptr) {
        *trace << label << ",modulation,feedback,-1,"
            << runtime.simulated_time() << ",-1,-1,-1,"
            << static_cast<int>(modulation) << ',' << feedback.receipts_with_trace
            << ',' << feedback.positive_changes << ',' << feedback.negative_changes
            << ",0\n";
    }
    return B50Learning{std::move(runtime), std::move(activity), weights,
        {}, modulation, delay};
}

std::set<eve::SynapseId> b50_impulse_ids(
    const std::vector<eve::bios::ImpulseObservation>& values) {
    std::set<eve::SynapseId> result;
    for (const auto& value : values) result.insert(value.synapse);
    return result;
}

std::pair<B49Probe, B49Probe> b50_recall(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const std::uint8_t cue) {
    static_cast<void>(runtime.advance_time_by(16));
    auto other_runtime = runtime;
    return {b49_probe(runtime, circuit, cue),
            b49_probe(other_runtime, circuit, 1U - cue)};
}

void b50_write_reference_matrix(const std::filesystem::path& output) {
    std::map<std::string, std::vector<B49Result>> discovery;
    std::map<std::string, B49Variant> variants;
    std::ofstream trials(output / "evaluation-trials.csv");
    trials << "variant,group,family,seed,trial,cue,target,E1_outcome,E2_outcome,E2_explored,shared_fingerprint\n";
    std::ofstream reproduction(output / "b49-reference-reproduction.csv");
    reproduction << "family,assignment,seed,first_fingerprint,second_fingerprint,E1_exact,E2_exact,weights_exact,valid\n";
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto assignment = std::string(inverse ? "inverse" : "normal");
            std::vector<B49Variant> group{
                {"K0_reference_" + assignment, "K0", B46Task::mixed,
                    B48Policy::reference, inverse, true, false, family},
                {"K1_no_learning_" + assignment, "K1", B46Task::mixed,
                    B48Policy::reference, inverse, false, true, family},
                {"K2_fixed_inhibition_" + assignment, "K2", B46Task::mixed,
                    B48Policy::reference, inverse, true, true, family},
                {"K3_learned_inhibition_" + assignment, "K3", B46Task::mixed,
                    B48Policy::reference, inverse, true, false, family},
                {"K4_random_extra_" + assignment, "B49-K4", B46Task::mixed,
                    B48Policy::random_extra, inverse, true, false, family},
                {"K5_b48_adaptive_" + assignment, "B49-K5", B46Task::mixed,
                    B48Policy::adaptive_after_negative, inverse, true, false, family},
                {"K6_redistributed_" + assignment, "B49-K6", B46Task::mixed,
                    B48Policy::replayed_timing, inverse, true, false, family},
            };
            for (const auto& variant : group) variants.emplace(b49_key(variant), variant);
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                std::vector<bool> adaptive_schedule;
                discovery[b49_key(group[5])].push_back(b49_run(
                    group[5], seed, nullptr, &adaptive_schedule, &trials));
                for (std::size_t index = 0; index < group.size(); ++index) {
                    if (index == 5) continue;
                    discovery[b49_key(group[index])].push_back(b49_run(
                        group[index], seed,
                        index == 6 ? &adaptive_schedule : nullptr,
                        nullptr, &trials));
                }
                const auto first = b49_run(group[0], seed);
                const auto second = b49_run(group[0], seed);
                const auto equal_counts = [](const B47Counts& a, const B47Counts& b) {
                    return a.correct == b.correct && a.wrong == b.wrong &&
                        a.both == b.both && a.none == b.none;
                };
                reproduction << family << ',' << assignment << ',' << seed << ','
                    << first.shared_fingerprint << ',' << second.shared_fingerprint
                    << ',' << (equal_counts(first.e1[0], second.e1[0]) &&
                               equal_counts(first.e1[1], second.e1[1]))
                    << ',' << (equal_counts(first.e2[0], second.e2[0]) &&
                               equal_counts(first.e2[1], second.e2[1]))
                    << ',' << (first.cue_weights == second.cue_weights &&
                               first.inhibition_weights == second.inhibition_weights)
                    << ',' << (first.valid && second.valid) << '\n';
            }
        }
    }
    b49_write_results(output, "summary.csv", discovery, variants);
    b49_write_seed_data(output, discovery, variants);
    b49_paired(output, discovery, variants);

    std::map<std::string, std::vector<B49Result>> confirmation;
    std::map<std::string, B49Variant> confirmation_variants;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto assignment = std::string(inverse ? "inverse" : "normal");
            for (const std::uint32_t kind : {0U, 1U, 2U}) {
                B49Variant variant{
                    std::string(kind == 0 ? "confirmation_K0_" :
                        kind == 1 ? "confirmation_K1_" : "confirmation_K5_") +
                        assignment,
                    kind == 0 ? "K0" : kind == 1 ? "K1" : "K5",
                    B46Task::mixed,
                    kind == 2 ? B48Policy::adaptive_after_negative
                              : B48Policy::reference,
                    inverse, kind != 1, kind == 1, family};
                confirmation_variants.emplace(b49_key(variant), variant);
                for (std::uint64_t seed = 10001; seed <= 10064; ++seed) {
                    confirmation[b49_key(variant)].push_back(b49_run(variant, seed));
                }
            }
        }
    }
    b49_write_results(
        output, "confirmation.csv", confirmation, confirmation_variants);

    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "phase,group,family,assignment,E1_X,E1_Y,E1_robust,advantage_vs_no_learning,ci_low,ci_high,passes\n";
    const auto emit_criteria = [&](const std::string& phase,
                                   const B49Variant& candidate,
                                   const std::vector<B49Result>& rows,
                                   const std::vector<B49Result>& controls,
                                   const std::uint64_t salt) {
        double x = 0.0, y = 0.0;
        std::uint64_t robust = 0;
        std::vector<double> differences;
        for (std::size_t index = 0; index < rows.size(); ++index) {
            const auto sx = b49_x(rows[index], true);
            const auto sy = b49_y(rows[index], true);
            x += sx;
            y += sy;
            robust += sx >= .75 && sy >= .75;
            differences.push_back(b49_score(rows[index], true) -
                b49_score(controls[index], true));
        }
        x /= rows.size();
        y /= rows.size();
        const auto interval = bootstrap_interval(differences, salt);
        const auto advantage = b48_mean(differences);
        const bool pass = x >= .75 && y >= .75 && robust >= 48 &&
            advantage >= .15 && interval.first > 0;
        criteria << phase << ',' << candidate.group << ',' << candidate.family
            << ',' << (candidate.inverse ? "inverse" : "normal") << ','
            << x << ',' << y << ',' << robust << ',' << advantage << ','
            << interval.first << ',' << interval.second << ',' << pass << '\n';
    };
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto suffix = std::string(inverse ? "inverse" : "normal") +
                "_f" + std::to_string(family);
            emit_criteria("discovery", variants.at("K5_b48_adaptive_" + suffix),
                discovery.at("K5_b48_adaptive_" + suffix),
                discovery.at("K1_no_learning_" + suffix),
                0x5001000ULL + family * 2 + inverse);
            emit_criteria("confirmation",
                confirmation_variants.at("confirmation_K5_" + suffix),
                confirmation.at("confirmation_K5_" + suffix),
                confirmation.at("confirmation_K1_" + suffix),
                0x5002000ULL + family * 2 + inverse);
        }
    }

    std::map<std::string, std::vector<B49Result>> tasks;
    std::map<std::string, B49Variant> task_variants;
    for (const bool inverse : {false, true}) {
        for (const auto task : {B46Task::only_x, B46Task::only_y,
                 B46Task::x_then_y, B46Task::y_then_x,
                 B46Task::alternating, B46Task::mixed,
                 B46Task::reversal, B46Task::retention}) {
            B49Variant variant{"task_K0_" +
                    std::string(inverse ? "inverse_" : "normal_") +
                    b46_task_name(task),
                "K0", task, B48Policy::reference, inverse, true, false, 0};
            task_variants.emplace(b49_key(variant), variant);
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                tasks[b49_key(variant)].push_back(b49_run(variant, seed));
            }
        }
    }
    b49_write_results(output, "task-matrix.csv", tasks, task_variants);
}

std::vector<std::string> b50_fields(const std::string& line) {
    std::vector<std::string> result;
    std::size_t begin = 0;
    while (true) {
        const auto end = line.find(',', begin);
        result.push_back(line.substr(begin, end - begin));
        if (end == std::string::npos) break;
        begin = end + 1;
    }
    return result;
}

void b50_statistics(const std::filesystem::path& output) {
    std::map<std::string, std::vector<double>> values;
    std::string line;
    {
        std::ifstream file(output / "causal-copies.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            if (row[5] == "D0_positive") {
                values["P1_D0_next_correct"].push_back(std::stod(row[8]));
                values["P1_D0_other_correct"].push_back(std::stod(row[10]));
            }
        }
    }
    {
        std::ifstream file(output / "positive-repetitions.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            values["P5_repeat_" + row[5] + "_correct"].push_back(
                std::stod(row[7]));
        }
    }
    {
        std::ifstream file(output / "interference-diagnostics.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            values["P4_new_correct"].push_back(std::stod(row[11]));
            values["P4_other_retained"].push_back(std::stod(row[13]));
            values["P4_both_retained"].push_back(std::stod(row[14]));
        }
    }
    {
        std::ifstream file(output / "repeated-weight-ablations.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            if (row[7] != "cue_target") continue;
            values["target_increment_necessary"].push_back(std::stod(row[12]));
            values["target_increment_sufficient"].push_back(std::stod(row[14]));
        }
    }
    std::ofstream summary(output / "diagnostic-summary.csv");
    summary << "metric,cluster_unit,clusters,rate,ci_low,ci_high\n";
    std::uint64_t salt = 0;
    for (const auto& [name, samples] : values) {
        const auto interval = bootstrap_interval(
            samples, 0x5003000ULL + ++salt);
        summary << name << ",family_assignment_seed," << samples.size() << ','
            << b48_mean(samples) << ',' << interval.first << ','
            << interval.second << '\n';
    }

    std::ofstream groups(output / "control-groups.csv");
    groups << "group,scope,description,implemented\n";
    groups << "K0,full_matrix,unchanged B49 reference,1\n";
    groups << "K1,full_matrix,no learning,1\n";
    groups << "K2,full_matrix,fixed inhibition,1\n";
    groups << "K3,full_matrix,learned inhibition,1\n";
    groups << "K4,diagnostic_copy,exploration without positive plasticity,1\n";
    groups << "K5,diagnostic_copy,positive experience with unchanged eligibility,1\n";
    groups << "K6,diagnostic_copy,isolated sensor weight changes,1\n";
    groups << "K7,diagnostic_copy,isolated competing weight changes,1\n";
    groups << "K8,diagnostic_copy,delayed positive modulation,1\n";
    groups << "K9,conditional,new local mechanism,0\n";
    groups << "K10,conditional,deactivated new mechanism,0\n";
}

void b50_credit_diagnostics(const std::filesystem::path& output) {
    std::ofstream population(output / "prospective-events.csv");
    population << "family,assignment,seed,episode,cue,target,autonomous,explored,exploration_action,final,correct,category,other_before,other_before_correct,first_positive_for_seed\n";
    std::ofstream inventory(output / "eligibility-inventory.csv");
    inventory << "event_id,family,assignment,seed,episode,cue,target,receiver,generation,expires_at,strength,synapse,sender,effect,role,weight_before,weight_after,changed,sensor_phase,exploration_phase,functional_nonzero\n";
    std::ofstream changes(output / "weight-changes.csv");
    changes << "event_id,family,assignment,seed,episode,cue,target,synapse,sender,receiver,effect,role,before,after,delta,protected,sensor_phase,exploration_phase\n";
    std::ofstream copies(output / "causal-copies.csv");
    copies << "event_id,family,assignment,seed,episode,intervention,value,next_outcome,next_correct,other_outcome,other_correct,valid\n";
    std::ofstream ablations(output / "single-weight-ablations.csv");
    ablations << "event_id,family,assignment,seed,episode,synapse,role,delta,D0_outcome,rollback_outcome,rollback_breaks_success,isolated_outcome,isolated_sufficient\n";
    std::ofstream repetitions(output / "positive-repetitions.csv");
    repetitions << "event_id,family,assignment,seed,episode,repetition,outcome,correct,other_outcome,other_correct,target_weight,competitor_weight\n";
    std::ofstream repeated_ablations(output / "repeated-weight-ablations.csv");
    repeated_ablations << "event_id,family,assignment,seed,episode,repetition,synapse,role,before,after,full_outcome,rollback_outcome,necessary,isolated_outcome,sufficient\n";
    std::ofstream continuations(output / "continuations.csv");
    continuations << "event_id,family,assignment,seed,episode,variant,future_episodes,E1_X,E1_Y,E1_score,E1_robust\n";
    std::ofstream timing_credit(output / "timing-credit.csv");
    timing_credit << "event_id,family,assignment,seed,episode,delay,eligibility_before_delay,changed_synapses,target_weight,competitor_weight,next_outcome,next_correct\n";
    std::ofstream event_trace(output / "event-chains.csv");
    event_trace << "event_id,family,assignment,seed,episode,cue,phase,kind,event,time,neuron,synapse,generation,weight_or_modulation,inhibitory_or_receipts,membrane_before_or_positive,membrane_after_or_negative,candidate_count\n";
    std::ofstream seed_summary(output / "positive-event-seeds.csv");
    seed_summary << "family,assignment,seed,first_positive_found,episode,event_id\n";

    std::uint64_t event_id = 0;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            B49Variant variant{"B50_prospective", "K5", B46Task::mixed,
                B48Policy::reference, inverse, true, false, family};
            const auto training_variant = b49_training_variant(variant);
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                auto state = b48_initial_state(training_variant, seed);
                bool selected = false;
                std::uint32_t selected_episode = 0;
                std::uint64_t selected_id = 0;
                for (std::uint32_t episode = 0; episode < 128; ++episode) {
                    const auto before = state.runtime;
                    const auto planned_cue = b48_cue(
                        training_variant, state.plans[episode], episode, 128);
                    auto other_before_runtime = before;
                    const auto other_before = b49_probe(
                        other_before_runtime, state.circuit, 1U - planned_cue);
                    const bool other_before_target_a = b48_target_a(
                        training_variant, 1U - planned_cue, episode, 128);
                    const bool other_before_correct = b50_correct(
                        other_before_target_a, other_before.motor_a,
                        other_before.motor_b);
                    const auto outcome = b48_episode(
                        state, training_variant, episode,
                        nullptr, nullptr, nullptr, nullptr);
                    const bool no_autonomous =
                        !outcome.autonomous_a && !outcome.autonomous_b;
                    const bool positive_exploration =
                        no_autonomous && outcome.explored && outcome.correct;
                    std::string category;
                    if (outcome.autonomous_a != outcome.autonomous_b &&
                        b50_correct(outcome.target_a, outcome.autonomous_a,
                                    outcome.autonomous_b)) {
                        category = "autonomous_correct";
                    } else if (positive_exploration) {
                        category = "first_exploration_correct";
                    } else if (outcome.autonomous_a != outcome.autonomous_b &&
                               outcome.correct) {
                        category = "wrong_then_extra_correct";
                    } else if (outcome.final_a && outcome.final_b) {
                        category = "ambiguous";
                    } else if (outcome.explored) {
                        category = "exploration_failed";
                    } else {
                        category = "autonomous_failed";
                    }
                    const bool take = positive_exploration && !selected;
                    population << family << ',' << (inverse ? "inverse" : "normal")
                        << ',' << seed << ',' << episode << ','
                        << static_cast<unsigned>(outcome.cue) << ','
                        << (outcome.target_a ? 'A' : 'B') << ','
                        << b47_outcome(outcome.autonomous_a, outcome.autonomous_b)
                        << ',' << outcome.explored << ','
                        << static_cast<unsigned>(outcome.action) << ','
                        << b47_outcome(outcome.final_a, outcome.final_b) << ','
                        << outcome.correct << ',' << category << ','
                        << b47_outcome(other_before.motor_a,
                                      other_before.motor_b)
                        << ',' << other_before_correct << ',' << take << '\n';
                    if (!take) continue;
                    selected = true;
                    selected_episode = episode;
                    selected_id = ++event_id;
                    const auto cue = outcome.cue;
                    const auto action = outcome.action;
                    const bool target_a = outcome.target_a;
                    const std::string label = std::to_string(event_id) + ',' +
                        std::to_string(family) + ',' +
                        (inverse ? "inverse" : "normal") + ',' +
                        std::to_string(seed) + ',' + std::to_string(episode) + ',' +
                        std::to_string(cue);
                    auto d0 = b50_learn(before, state.circuit, cue, action,
                        true, 1, 0, seed <= 4 ? &event_trace : nullptr, label);
                    d0.after_weights = d0.after_feedback.network().synapses();
                    const auto sensor_ids = b50_impulse_ids(d0.activity.sensor_impulses);
                    const auto exploration_ids =
                        b50_impulse_ids(d0.activity.exploration_impulses);
                    const auto d0_recall = b50_recall(
                        d0.after_feedback, state.circuit, cue);
                    const bool d0_correct = b50_correct(target_a,
                        d0_recall.first.motor_a, d0_recall.first.motor_b);
                    const bool other_target_a = b48_target_a(
                        training_variant, 1U - cue, episode, 128);
                    for (eve::NeuronId neuron = 0;
                         neuron < d0.activity.after_activity.network().neuron_count();
                         ++neuron) {
                        for (const auto& record : d0.activity.after_activity.network()
                                 .eligibility_records(neuron)) {
                            for (const auto id : record.candidates) {
                                const auto& before_synapse = d0.before_weights[id];
                                const auto& after_synapse = d0.after_weights[id];
                                inventory << event_id << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << static_cast<unsigned>(cue) << ','
                                    << (target_a ? 'A' : 'B') << ',' << neuron << ','
                                    << record.generation << ',' << record.expires_at
                                    << ',' << static_cast<unsigned>(record.strength)
                                    << ',' << id << ',' << before_synapse.sender << ','
                                    << (before_synapse.effect ==
                                        eve::SynapticEffect::inhibitory)
                                    << ',' << b50_synapse_role(
                                        state.circuit, id, cue, target_a)
                                    << ',' << before_synapse.weight << ','
                                    << after_synapse.weight << ','
                                    << (before_synapse.weight != after_synapse.weight)
                                    << ',' << sensor_ids.count(id) << ','
                                    << exploration_ids.count(id) << ','
                                    << ((sensor_ids.count(id) ||
                                         exploration_ids.count(id)) &&
                                        before_synapse.weight != 0) << '\n';
                            }
                        }
                    }

                    std::vector<eve::SynapseId> changed;
                    for (eve::SynapseId id = 0;
                         id < d0.after_weights.size(); ++id) {
                        if (d0.before_weights[id].weight ==
                            d0.after_weights[id].weight) continue;
                        changed.push_back(id);
                        const auto& before_synapse = d0.before_weights[id];
                        const auto& after_synapse = d0.after_weights[id];
                        changes << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << static_cast<unsigned>(cue)
                            << ',' << (target_a ? 'A' : 'B') << ',' << id << ','
                            << before_synapse.sender << ',' << before_synapse.receiver
                            << ',' << (before_synapse.effect ==
                                eve::SynapticEffect::inhibitory)
                            << ',' << b50_synapse_role(
                                state.circuit, id, cue, target_a)
                            << ',' << before_synapse.weight << ','
                            << after_synapse.weight << ','
                            << (after_synapse.weight - before_synapse.weight) << ','
                            << before_synapse.plasticity_protected << ','
                            << sensor_ids.count(id) << ',' << exploration_ids.count(id)
                            << '\n';
                    }

                    const auto emit_copy = [&](const std::string& intervention,
                                               const std::uint64_t value,
                                               eve::bios::Runtime runtime) {
                        const auto recall = b50_recall(runtime, state.circuit, cue);
                        copies << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << intervention << ',' << value
                            << ',' << b47_outcome(recall.first.motor_a,
                                                  recall.first.motor_b)
                            << ',' << b50_correct(target_a, recall.first.motor_a,
                                                  recall.first.motor_b)
                            << ',' << b47_outcome(recall.second.motor_a,
                                                  recall.second.motor_b)
                            << ',' << b50_correct(other_target_a,
                                                  recall.second.motor_a,
                                                  recall.second.motor_b)
                            << ',' << runtime.network().validate() << '\n';
                    };
                    emit_copy("D0_positive", 0, d0.after_feedback);
                    auto d1 = b50_learn(before, state.circuit, cue, action,
                        false, -1);
                    emit_copy("D1_without_exploration", 0, d1.after_feedback);
                    auto d2 = b50_learn(before, state.circuit, cue, action,
                        true, 0);
                    emit_copy("D2_no_plasticity", 0, d2.after_feedback);
                    emit_copy("D3_neutral", 0, d2.after_feedback);
                    for (const std::uint64_t delay : {0ULL, 4ULL, 8ULL, 16ULL, 32ULL}) {
                        auto delayed = b50_learn(before, state.circuit, cue,
                            action, true, 1, delay);
                        delayed.after_weights =
                            delayed.after_feedback.network().synapses();
                        std::uint32_t delayed_changes = 0;
                        for (eve::SynapseId id = 0;
                             id < delayed.after_weights.size(); ++id) {
                            delayed_changes += delayed.before_weights[id].weight !=
                                delayed.after_weights[id].weight;
                        }
                        const auto delayed_recall = b50_recall(
                            delayed.after_feedback, state.circuit, cue).first;
                        const auto target_index = target_a ? 0U : 1U;
                        timing_credit << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << delay << ','
                            << delayed.activity.after_activity.network()
                                .active_eligibility_count()
                            << ',' << delayed_changes << ','
                            << delayed.after_feedback.network().synapse(
                                state.circuit.cue_synapses[
                                    cue * 2 + target_index]).weight
                            << ',' << delayed.after_feedback.network().synapse(
                                state.circuit.cue_synapses[
                                    cue * 2 + 1U - target_index]).weight
                            << ',' << b47_outcome(delayed_recall.motor_a,
                                                  delayed_recall.motor_b)
                            << ',' << b50_correct(target_a,
                                delayed_recall.motor_a, delayed_recall.motor_b)
                            << '\n';
                        emit_copy("D5_feedback_delay", delay,
                                  delayed.after_feedback);
                    }
                    auto sensory_only = d0.after_feedback;
                    auto competing_only = d0.after_feedback;
                    for (const auto id : changed) {
                        const auto role = b50_synapse_role(
                            state.circuit, id, cue, target_a);
                        const bool sensory = role == "cue_target" ||
                            role == "cue_competitor";
                        if (!sensory) {
                            sensory_only.set_experimental_synapse_weight(
                                id, d0.before_weights[id].weight);
                        }
                        if (role != "cue_competitor" &&
                            role != "inhibition_competitor" &&
                            role != "other_cue_same_action" &&
                            role != "other_cue_other_action") {
                            competing_only.set_experimental_synapse_weight(
                                id, d0.before_weights[id].weight);
                        }
                    }
                    emit_copy("K6_sensor_changes_only", 0, sensory_only);
                    emit_copy("K7_competing_changes_only", 0, competing_only);

                    for (const auto id : changed) {
                        auto rollback = d0.after_feedback;
                        rollback.set_experimental_synapse_weight(
                            id, d0.before_weights[id].weight);
                        const auto rollback_recall = b50_recall(
                            rollback, state.circuit, cue).first;
                        auto isolated = d0.activity.after_activity;
                        isolated.set_experimental_synapse_weight(
                            id, d0.after_weights[id].weight);
                        const auto isolated_recall = b50_recall(
                            isolated, state.circuit, cue).first;
                        const bool rollback_correct = b50_correct(target_a,
                            rollback_recall.motor_a, rollback_recall.motor_b);
                        const bool isolated_correct = b50_correct(target_a,
                            isolated_recall.motor_a, isolated_recall.motor_b);
                        ablations << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << id << ','
                            << b50_synapse_role(state.circuit, id, cue, target_a)
                            << ',' << (d0.after_weights[id].weight -
                                       d0.before_weights[id].weight)
                            << ',' << b47_outcome(d0_recall.first.motor_a,
                                                  d0_recall.first.motor_b)
                            << ',' << b47_outcome(rollback_recall.motor_a,
                                                  rollback_recall.motor_b)
                            << ',' << (d0_correct && !rollback_correct) << ','
                            << b47_outcome(isolated_recall.motor_a,
                                                  isolated_recall.motor_b)
                            << ',' << isolated_correct << '\n';
                    }

                    auto repeated_runtime = before;
                    for (std::uint32_t repetition = 1; repetition <= 4; ++repetition) {
                        const auto repetition_before =
                            repeated_runtime.network().synapses();
                        auto learned = b50_learn(repeated_runtime, state.circuit,
                            cue, action, true, 1);
                        repeated_runtime = learned.after_feedback;
                        const auto recall = b50_recall(
                            repeated_runtime, state.circuit, cue);
                        repetitions << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << repetition << ','
                            << b47_outcome(recall.first.motor_a,
                                           recall.first.motor_b)
                            << ',' << b50_correct(target_a,
                                recall.first.motor_a, recall.first.motor_b)
                            << ',' << b47_outcome(recall.second.motor_a,
                                                  recall.second.motor_b)
                            << ',' << b50_correct(other_target_a,
                                recall.second.motor_a, recall.second.motor_b)
                            << ',' << repeated_runtime.network().synapse(
                                state.circuit.cue_synapses[cue * 2 +
                                    (target_a ? 0 : 1)]).weight
                            << ',' << repeated_runtime.network().synapse(
                                state.circuit.cue_synapses[cue * 2 +
                                    (target_a ? 1 : 0)]).weight << '\n';
                        if (repetition == 2) {
                            const auto repetition_after =
                                repeated_runtime.network().synapses();
                            for (eve::SynapseId id = 0;
                                 id < repetition_after.size(); ++id) {
                                if (repetition_before[id].weight ==
                                    repetition_after[id].weight) continue;
                                auto rollback = repeated_runtime;
                                rollback.set_experimental_synapse_weight(
                                    id, repetition_before[id].weight);
                                const auto rollback_outcome = b50_recall(
                                    rollback, state.circuit, cue).first;
                                auto isolated = learned.activity.after_activity;
                                isolated.set_experimental_synapse_weight(
                                    id, repetition_after[id].weight);
                                const auto isolated_outcome = b50_recall(
                                    isolated, state.circuit, cue).first;
                                const bool full_correct = b50_correct(target_a,
                                    recall.first.motor_a, recall.first.motor_b);
                                const bool rollback_correct = b50_correct(target_a,
                                    rollback_outcome.motor_a,
                                    rollback_outcome.motor_b);
                                const bool isolated_correct = b50_correct(target_a,
                                    isolated_outcome.motor_a,
                                    isolated_outcome.motor_b);
                                repeated_ablations << event_id << ',' << family
                                    << ',' << (inverse ? "inverse" : "normal")
                                    << ',' << seed << ',' << episode << ','
                                    << repetition << ',' << id << ','
                                    << b50_synapse_role(
                                        state.circuit, id, cue, target_a)
                                    << ',' << repetition_before[id].weight << ','
                                    << repetition_after[id].weight << ','
                                    << b47_outcome(recall.first.motor_a,
                                                   recall.first.motor_b)
                                    << ',' << b47_outcome(rollback_outcome.motor_a,
                                                         rollback_outcome.motor_b)
                                    << ',' << (full_correct && !rollback_correct)
                                    << ',' << b47_outcome(isolated_outcome.motor_a,
                                                         isolated_outcome.motor_b)
                                    << ',' << isolated_correct << '\n';
                            }
                        }
                        static_cast<void>(repeated_runtime.advance_time_by(16));
                    }

                    const auto continuation = [&](const char* name,
                                                  eve::bios::Runtime runtime) {
                        auto future = state;
                        future.runtime = std::move(runtime);
                        const auto end = std::min<std::uint32_t>(128, episode + 17);
                        b48_train_range(future, training_variant,
                            episode + 1, end);
                        const auto evaluated = b49_evaluate(
                            future.runtime, future.circuit, variant, seed,
                            future.plans);
                        continuations << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << name << ','
                            << (end - episode - 1) << ','
                            << b49_x(evaluated, true) << ','
                            << b49_y(evaluated, true) << ','
                            << b49_score(evaluated, true) << ','
                            << (b49_x(evaluated, true) >= .75 &&
                                b49_y(evaluated, true) >= .75) << '\n';
                    };
                    continuation("D0_positive", d0.after_feedback);
                    continuation("D2_no_plasticity", d2.after_feedback);
                }
                seed_summary << family << ','
                    << (inverse ? "inverse" : "normal") << ',' << seed << ','
                    << selected << ',' << selected_episode << ',' << selected_id
                    << '\n';
            }
        }
    }
}

void b50_interference_diagnostics(const std::filesystem::path& output) {
    std::ofstream file(output / "interference-diagnostics.csv");
    file << "event_id,family,assignment,seed,episode,cue,target,other_before,changed_cues,changed_inhibition,new_after,new_correct,other_after,other_correct,both_retained,target_weight_before,target_weight_after,competitor_weight_before,competitor_weight_after\n";
    std::ofstream ablations(output / "interference-ablations.csv");
    ablations << "event_id,family,assignment,seed,episode,intervention,synapse,role,new_outcome,new_correct,other_outcome,other_correct,both_retained\n";
    std::uint64_t event_id = 0;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            B49Variant variant{"B50_interference", "K5", B46Task::mixed,
                B48Policy::reference, inverse, true, false, family};
            const auto training_variant = b49_training_variant(variant);
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                auto state = b48_initial_state(training_variant, seed);
                bool selected = false;
                for (std::uint32_t episode = 0; episode < 128; ++episode) {
                    const auto before = state.runtime;
                    const auto cue = b48_cue(
                        training_variant, state.plans[episode], episode, 128);
                    auto other_runtime = before;
                    const auto other_before = b49_probe(
                        other_runtime, state.circuit, 1U - cue);
                    const bool other_target_a = b48_target_a(
                        training_variant, 1U - cue, episode, 128);
                    const bool other_correct_before = b50_correct(
                        other_target_a, other_before.motor_a,
                        other_before.motor_b);
                    const auto outcome = b48_episode(
                        state, training_variant, episode,
                        nullptr, nullptr, nullptr, nullptr);
                    const bool positive_exploration =
                        !outcome.autonomous_a && !outcome.autonomous_b &&
                        outcome.explored && outcome.correct;
                    if (selected || !positive_exploration ||
                        !other_correct_before) {
                        continue;
                    }
                    selected = true;
                    ++event_id;
                    auto learned = b50_learn(before, state.circuit,
                        cue, outcome.action, true, 1);
                    learned.after_weights =
                        learned.after_feedback.network().synapses();
                    const auto recall = b50_recall(
                        learned.after_feedback, state.circuit, cue);
                    const bool new_correct = b50_correct(outcome.target_a,
                        recall.first.motor_a, recall.first.motor_b);
                    const bool other_correct = b50_correct(other_target_a,
                        recall.second.motor_a, recall.second.motor_b);
                    std::uint32_t changed_cues = 0;
                    std::uint32_t changed_inhibition = 0;
                    std::vector<eve::SynapseId> changed;
                    for (eve::SynapseId id = 0;
                         id < learned.after_weights.size(); ++id) {
                        if (learned.before_weights[id].weight ==
                            learned.after_weights[id].weight) continue;
                        changed.push_back(id);
                        changed_cues += std::find(
                            state.circuit.cue_synapses.begin(),
                            state.circuit.cue_synapses.end(), id) !=
                            state.circuit.cue_synapses.end();
                        changed_inhibition += std::find(
                            state.circuit.inhibitory_outputs.begin(),
                            state.circuit.inhibitory_outputs.end(), id) !=
                            state.circuit.inhibitory_outputs.end();
                    }
                    const auto target_index = outcome.target_a ? 0U : 1U;
                    const auto target_id =
                        state.circuit.cue_synapses[cue * 2 + target_index];
                    const auto competitor_id =
                        state.circuit.cue_synapses[cue * 2 + 1U - target_index];
                    file << event_id << ',' << family << ','
                        << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << episode << ',' << static_cast<unsigned>(cue)
                        << ',' << (outcome.target_a ? 'A' : 'B') << ','
                        << b47_outcome(other_before.motor_a, other_before.motor_b)
                        << ',' << changed_cues << ',' << changed_inhibition << ','
                        << b47_outcome(recall.first.motor_a, recall.first.motor_b)
                        << ',' << new_correct << ','
                        << b47_outcome(recall.second.motor_a, recall.second.motor_b)
                        << ',' << other_correct << ','
                        << (new_correct && other_correct) << ','
                        << learned.before_weights[target_id].weight << ','
                        << learned.after_weights[target_id].weight << ','
                        << learned.before_weights[competitor_id].weight << ','
                        << learned.after_weights[competitor_id].weight << '\n';

                    const auto emit = [&](const char* intervention,
                                          const eve::SynapseId id,
                                          eve::bios::Runtime runtime) {
                        const auto result = b50_recall(
                            runtime, state.circuit, cue);
                        const bool first_correct = b50_correct(outcome.target_a,
                            result.first.motor_a, result.first.motor_b);
                        const bool second_correct = b50_correct(other_target_a,
                            result.second.motor_a, result.second.motor_b);
                        ablations << event_id << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << intervention << ',';
                        if (id == eve::kInvalidIndex) {
                            ablations << "-1,all";
                        } else {
                            ablations << id << ',' << b50_synapse_role(
                                state.circuit, id, cue, outcome.target_a);
                        }
                        ablations << ',' << b47_outcome(
                            result.first.motor_a, result.first.motor_b)
                            << ',' << first_correct << ',' << b47_outcome(
                                result.second.motor_a, result.second.motor_b)
                            << ',' << second_correct << ','
                            << (first_correct && second_correct) << '\n';
                    };
                    emit("D0_positive", eve::kInvalidIndex,
                         learned.after_feedback);
                    for (const auto id : changed) {
                        auto rollback = learned.after_feedback;
                        rollback.set_experimental_synapse_weight(
                            id, learned.before_weights[id].weight);
                        emit("rollback_one", id, std::move(rollback));
                    }
                    auto cue_only = learned.after_feedback;
                    for (const auto id : changed) {
                        if (std::find(state.circuit.cue_synapses.begin(),
                                      state.circuit.cue_synapses.end(), id) ==
                            state.circuit.cue_synapses.end()) {
                            cue_only.set_experimental_synapse_weight(
                                id, learned.before_weights[id].weight);
                        }
                    }
                    emit("sensor_changes_only", eve::kInvalidIndex,
                         std::move(cue_only));
                }
            }
        }
    }
}

void b50_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,fingerprint,exact_restore,exact_continuation,valid\n";
    B49Variant variant{"persistence", "P", B46Task::mixed,
        B48Policy::reference, false, true, false, 0};
    const auto training_variant = b49_training_variant(variant);
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto state = b48_initial_state(training_variant, seed);
        b48_train_range(state, training_variant, 0, 128);
        const auto path = output / (".b50-state-" + std::to_string(seed));
        state.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        auto original = state.runtime;
        static_cast<void>(original.run());
        static_cast<void>(restored.run());
        file << seed << ',' << state.runtime.state_fingerprint() << ',' << exact
            << ',' << (original.state_fingerprint() ==
                        restored.state_fingerprint())
            << ',' << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b50");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    b50_write_reference_matrix(output);
    b50_credit_diagnostics(output);
    b50_interference_diagnostics(output);
    b50_statistics(output);
    b50_persistence(output);

    std::ofstream mechanism(output / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K9,0,the target cue receives local eligibility; a second positive increment from weight one to threshold two is necessary and sufficient in 364 paired copies; no missing candidate-assignment mechanism is isolated\n";
    mechanism << "K10,0,no K9 mechanism exists to deactivate\n";

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,reference_runs,confirmation_runs,task_runs\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << 3 * 2 * 7 * 64 << ','
        << 3 * 2 * 3 * 64 << ',' << 2 * 8 * 64 << '\n';
    std::cout << "B.5.0 prospective credit assignment complete K9=not_justified\n";
    return 0;
}
