#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B48_EMBEDDED
#include "research_stage_b48.cpp"
#undef EVE_RESEARCH_STAGE_B48_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <map>

namespace {

struct B49Result {
    std::uint64_t seed{};
    std::array<B47Counts, 2> e1{};
    std::array<B47Counts, 2> e2{};
    std::uint64_t shared_fingerprint{};
    bool identical_start{};
    std::uint64_t e1_events{};
    std::uint64_t e2_events{};
    std::uint64_t e2_explorations{};
    std::array<std::int32_t, 4> cue_weights{};
    std::array<std::int32_t, 2> inhibition_weights{};
    std::uint64_t maximum_queue{};
    std::size_t memory{};
    bool valid{};
};

struct B49Variant {
    std::string name;
    std::string group;
    B46Task task{B46Task::mixed};
    B48Policy policy{B48Policy::reference};
    bool inverse{};
    bool plasticity{true};
    bool fixed_inhibition{};
    std::uint32_t family{};
};

struct B49Probe {
    bool motor_a{};
    bool motor_b{};
    std::uint64_t decision_a{};
    std::uint64_t decision_b{};
    std::vector<eve::bios::ImpulseObservation> impulses;
};

B48Variant b49_training_variant(const B49Variant& variant) {
    return B48Variant{variant.name, variant.task, variant.policy,
        variant.inverse, variant.plasticity, variant.fixed_inhibition,
        variant.family};
}

double b49_rate(const B47Counts& counts) {
    return b47_rate(counts);
}

double b49_x(const B49Result& result, const bool e1) {
    return b49_rate((e1 ? result.e1 : result.e2)[0]);
}

double b49_y(const B49Result& result, const bool e1) {
    return b49_rate((e1 ? result.e1 : result.e2)[1]);
}

double b49_score(const B49Result& result, const bool e1) {
    return (b49_x(result, e1) + b49_y(result, e1)) / 2.0;
}

B49Probe b49_probe(
    eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const std::uint8_t cue, std::ostream* events = nullptr,
    const std::string& label = {}) {
    B49Probe result;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        result.motor_a = result.motor_a || event.neuron == circuit.motors[0];
        result.motor_b = result.motor_b || event.neuron == circuit.motors[1];
        result.decision_a += event.neuron == circuit.decisions[0];
        result.decision_b += event.neuron == circuit.decisions[1];
        if (events != nullptr) {
            *events << label << ",fire," << event.event_index << ','
                << event.simulated_time << ',' << event.neuron
                << ",-1,0,0,0,0,0\n";
        }
    });
    runtime.set_impulse_observer([&](const eve::bios::ImpulseObservation& event) {
        result.impulses.push_back(event);
        if (events != nullptr) {
            *events << label << ",impulse," << event.event_index << ','
                << event.simulated_time << ',' << event.receiver << ','
                << event.synapse << ',' << event.applied_weight << ','
                << (event.synaptic_effect == eve::SynapticEffect::inhibitory)
                << ',' << event.membrane_before << ',' << event.membrane_after
                << ',' << event.eligibility_candidates.size() << '\n';
        }
    });
    runtime.enqueue_fire(circuit.sensors[cue]);
    static_cast<void>(runtime.run());
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    return result;
}

B49Result b49_evaluate(
    const eve::bios::Runtime& trained, const B47Circuit& circuit,
    const B49Variant& variant, const std::uint64_t seed,
    const std::vector<B48Plan>& plans, std::ostream* trials = nullptr) {
    eve::bios::Runtime e1_runtime = trained;
    eve::bios::Runtime e2_runtime = trained;
    B49Result result;
    result.seed = seed;
    result.shared_fingerprint = trained.state_fingerprint();
    result.identical_start = e1_runtime.state_fingerprint() ==
            e2_runtime.state_fingerprint() &&
        result.shared_fingerprint == e1_runtime.state_fingerprint();
    const auto e1_events_before = e1_runtime.metrics().processed_events;
    const auto e2_events_before = e2_runtime.metrics().processed_events;
    const std::uint32_t training = b46_training(variant.task);
    const auto training_variant = b49_training_variant(variant);

    for (std::uint32_t offset = 0; offset < 64; ++offset) {
        const auto& plan = plans[training + offset];
        const auto cue = b48_cue(
            training_variant, plan, training + offset, training);
        const bool target_a = b48_target_a(
            training_variant, cue, training + offset, training);
        const auto e1_start = e1_runtime.simulated_time() + 16;
        const auto e2_start = e2_runtime.simulated_time() + 16;
        const auto e1 = b49_probe(e1_runtime, circuit, cue);
        b47_record(result.e1[cue], target_a, e1.motor_a, e1.motor_b);
        const auto e2_autonomous = b49_probe(e2_runtime, circuit, cue);
        bool e2_a = e2_autonomous.motor_a;
        bool e2_b = e2_autonomous.motor_b;
        bool explored = false;
        if (!e2_a && !e2_b) {
            explored = true;
            ++result.e2_explorations;
            bool extra_a = false;
            bool extra_b = false;
            e2_runtime.set_fire_observer(
                [&](const eve::bios::FireObservation& event) {
                    extra_a = extra_a || event.neuron == circuit.motors[0];
                    extra_b = extra_b || event.neuron == circuit.motors[1];
                });
            e2_runtime.enqueue_fire(circuit.explorers[plan.exploration]);
            static_cast<void>(e2_runtime.run());
            e2_runtime.set_fire_observer({});
            e2_a = e2_a || extra_a;
            e2_b = e2_b || extra_b;
        }
        b47_record(result.e2[cue], target_a, e2_a, e2_b);
        if (trials != nullptr) {
            *trials << variant.name << ',' << variant.group << ','
                << variant.family << ',' << seed << ',' << offset << ','
                << static_cast<unsigned>(cue) << ',' << (target_a ? 'A' : 'B')
                << ',' << b47_outcome(e1.motor_a, e1.motor_b) << ','
                << b47_outcome(e2_a, e2_b) << ',' << explored << ','
                << result.shared_fingerprint << '\n';
        }
        static_cast<void>(e1_runtime.advance_time_to(e1_start));
        static_cast<void>(e2_runtime.advance_time_to(e2_start));
    }
    result.e1_events = e1_runtime.metrics().processed_events - e1_events_before;
    result.e2_events = e2_runtime.metrics().processed_events - e2_events_before;
    for (std::size_t index = 0; index < 4; ++index) {
        result.cue_weights[index] =
            trained.network().synapse(circuit.cue_synapses[index]).weight;
    }
    for (std::size_t index = 0; index < 2; ++index) {
        result.inhibition_weights[index] =
            trained.network().synapse(circuit.inhibitory_outputs[index]).weight;
    }
    result.maximum_queue = std::max(
        e1_runtime.metrics().maximum_queue_size,
        e2_runtime.metrics().maximum_queue_size);
    result.memory = trained.diagnostics().estimated_memory_bytes;
    result.valid = trained.network().validate() &&
        e1_runtime.network().validate() && e2_runtime.network().validate();
    return result;
}

B49Result b49_run(
    const B49Variant& variant, const std::uint64_t seed,
    const std::vector<bool>* replay = nullptr,
    std::vector<bool>* observed = nullptr, std::ostream* trials = nullptr) {
    auto training_variant = b49_training_variant(variant);
    auto state = b48_initial_state(training_variant, seed);
    b48_train_range(state, training_variant, 0, b46_training(variant.task),
        replay, observed);
    return b49_evaluate(state.runtime, state.circuit, variant, seed,
        state.plans, trials);
}

std::string b49_key(const B49Variant& variant) {
    return variant.name + "_f" + std::to_string(variant.family);
}

void b49_write_results(
    const std::filesystem::path& output, const std::string& filename,
    const std::map<std::string, std::vector<B49Result>>& results,
    const std::map<std::string, B49Variant>& variants) {
    std::ofstream summary(output / filename);
    summary << "variant,group,family,task,inverse,plasticity,seeds,E1_X,E1_Y,E1_score,E1_robust,E2_X,E2_Y,E2_score,E2_robust,E2_explorations,E1_events,E2_events,memory,maximum_queue,identical_start,invalid\n";
    for (const auto& [key, rows] : results) {
        const auto& variant = variants.at(key);
        double e1x = 0.0, e1y = 0.0, e2x = 0.0, e2y = 0.0;
        double explorations = 0.0, e1events = 0.0, e2events = 0.0, memory = 0.0;
        std::uint64_t e1robust = 0, e2robust = 0, invalid = 0;
        std::uint64_t queue = 0, identical = 0;
        for (const auto& row : rows) {
            const auto ax = b49_x(row, true), ay = b49_y(row, true);
            const auto sx = b49_x(row, false), sy = b49_y(row, false);
            e1x += ax; e1y += ay; e2x += sx; e2y += sy;
            e1robust += ax >= .75 && ay >= .75;
            e2robust += sx >= .75 && sy >= .75;
            explorations += row.e2_explorations;
            e1events += row.e1_events; e2events += row.e2_events;
            memory += row.memory;
            queue = std::max(queue, row.maximum_queue);
            identical += row.identical_start;
            invalid += !row.valid;
        }
        const double count = static_cast<double>(rows.size());
        summary << variant.name << ',' << variant.group << ',' << variant.family
            << ',' << b46_task_name(variant.task) << ',' << variant.inverse << ','
            << variant.plasticity << ',' << rows.size() << ',' << e1x / count
            << ',' << e1y / count << ',' << (e1x + e1y) / (2 * count) << ','
            << e1robust << ',' << e2x / count << ',' << e2y / count << ','
            << (e2x + e2y) / (2 * count) << ',' << e2robust << ','
            << explorations / count << ',' << e1events / count << ','
            << e2events / count << ',' << memory / count << ',' << queue << ','
            << identical << ',' << invalid << '\n';
    }
}

void b49_write_seed_data(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<B49Result>>& results,
    const std::map<std::string, B49Variant>& variants) {
    std::ofstream seeds(output / "seed-results.csv");
    seeds << "variant,group,family,seed,E1_X,E1_Y,E1_score,E2_X,E2_Y,E2_score,E1_robust,E2_robust,E2_explorations,shared_fingerprint,identical_start,cue_XA,cue_XB,cue_YA,cue_YB,inhib_AtoB,inhib_BtoA,E1_events,E2_events,memory,maximum_queue,valid\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "variant,group,family,seed,stage,cue,correct,wrong,both,none\n";
    for (const auto& [key, rows] : results) {
        const auto& variant = variants.at(key);
        for (const auto& row : rows) {
            const auto e1x = b49_x(row, true), e1y = b49_y(row, true);
            const auto e2x = b49_x(row, false), e2y = b49_y(row, false);
            seeds << variant.name << ',' << variant.group << ',' << variant.family
                << ',' << row.seed << ',' << e1x << ',' << e1y << ','
                << (e1x + e1y) / 2 << ',' << e2x << ',' << e2y << ','
                << (e2x + e2y) / 2 << ',' << (e1x >= .75 && e1y >= .75)
                << ',' << (e2x >= .75 && e2y >= .75) << ','
                << row.e2_explorations << ',' << row.shared_fingerprint << ','
                << row.identical_start;
            for (const auto weight : row.cue_weights) seeds << ',' << weight;
            for (const auto weight : row.inhibition_weights) seeds << ',' << weight;
            seeds << ',' << row.e1_events << ',' << row.e2_events << ','
                << row.memory << ',' << row.maximum_queue << ',' << row.valid
                << '\n';
            for (std::size_t cue = 0; cue < 2; ++cue) {
                const auto emit = [&](const char* stage, const B47Counts& counts) {
                    confusion << variant.name << ',' << variant.group << ','
                        << variant.family << ',' << row.seed << ',' << stage << ','
                        << cue << ',' << counts.correct << ',' << counts.wrong
                        << ',' << counts.both << ',' << counts.none << '\n';
                };
                emit("E1", row.e1[cue]);
                emit("E2", row.e2[cue]);
            }
        }
    }
}

void b49_paired(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<B49Result>>& results,
    const std::map<std::string, B49Variant>& variants) {
    std::ofstream file(output / "paired-comparisons.csv");
    file << "left,right,family,inverse,metric,seeds,mean_difference,ci_low,ci_high,positive_fraction\n";
    std::uint64_t salt = 0;
    const auto compare = [&](const std::string& left, const std::string& right,
                             const bool e1) {
        const auto& a = results.at(left);
        const auto& b = results.at(right);
        std::vector<double> differences;
        std::size_t positive = 0;
        for (std::size_t index = 0; index < a.size(); ++index) {
            const auto value = b49_score(a[index], e1) -
                b49_score(b[index], e1);
            differences.push_back(value);
            positive += value > 0;
        }
        const auto interval = bootstrap_interval(
            differences, 0x4905000ULL + ++salt);
        const auto& variant = variants.at(left);
        file << variant.name << ',' << variants.at(right).name << ','
            << variant.family << ',' << variant.inverse << ','
            << (e1 ? "E1" : "E2") << ',' << a.size() << ','
            << b48_mean(differences) << ',' << interval.first << ','
            << interval.second << ','
            << static_cast<double>(positive) / a.size() << '\n';
    };
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto suffix = std::string(inverse ? "inverse" : "normal") +
                "_f" + std::to_string(family);
            for (const auto& pair : {
                     std::pair<std::string, std::string>{
                         "K5_b48_adaptive_" + suffix,
                         "K0_reference_" + suffix},
                     {"K4_random_extra_" + suffix,
                         "K0_reference_" + suffix},
                     {"K6_redistributed_" + suffix,
                         "K5_b48_adaptive_" + suffix}}) {
                compare(pair.first, pair.second, true);
                compare(pair.first, pair.second, false);
            }
        }
    }
}

void b49_reference_reproduction(const std::filesystem::path& output) {
    std::ofstream file(output / "b48-reference-reproduction.csv");
    file << "family,assignment,seed,legacy_E1_X,legacy_E1_Y,"
            "separate_E1_X,separate_E1_Y,legacy_E2_X,legacy_E2_Y,"
            "separate_E2_X,separate_E2_Y,E2_exact,weights_exact,valid\n";
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            B48Variant legacy_variant{"B48_reference", B46Task::mixed,
                B48Policy::reference, inverse, true, false, family};
            B49Variant separate_variant{"B49_reference", "K0",
                B46Task::mixed, B48Policy::reference, inverse, true, false,
                family};
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                const auto legacy = b48_run(legacy_variant, seed);
                const auto separate = b49_run(separate_variant, seed);
                const auto counts_equal = [](const B47Counts& left,
                                             const B47Counts& right) {
                    return left.correct == right.correct &&
                        left.wrong == right.wrong &&
                        left.both == right.both && left.none == right.none;
                };
                const bool e2_exact =
                    counts_equal(legacy.supported[0], separate.e2[0]) &&
                    counts_equal(legacy.supported[1], separate.e2[1]);
                const bool weights_exact =
                    legacy.cue_weights == separate.cue_weights &&
                    legacy.inhibition_weights == separate.inhibition_weights;
                file << family << ',' << (inverse ? "inverse" : "normal")
                    << ',' << seed << ',' << b47_rate(legacy.autonomous[0])
                    << ',' << b47_rate(legacy.autonomous[1]) << ','
                    << b49_x(separate, true) << ',' << b49_y(separate, true)
                    << ',' << b47_rate(legacy.supported[0]) << ','
                    << b47_rate(legacy.supported[1]) << ','
                    << b49_x(separate, false) << ','
                    << b49_y(separate, false) << ',' << e2_exact << ','
                    << weights_exact << ',' << (legacy.valid && separate.valid)
                    << '\n';
            }
        }
    }
}

bool b49_unique_wrong(const B49Probe& probe, const bool target_a) {
    return target_a ? probe.motor_b && !probe.motor_a
                    : probe.motor_a && !probe.motor_b;
}

void b49_failure_diagnostics(
    const std::filesystem::path& output, std::ostream& event_chain) {
    std::ofstream failures(output / "single-error-diagnostics.csv");
    failures << "assignment,seed,source_episode,cue,target,initial,repeat,initial_decision_A,initial_decision_B,initial_impulses,eligibility_before,eligibility_after,changed_cues,changed_inhibition,dominant_weight_before,dominant_weight_after,competitor_weight_before,competitor_weight_after,same_wrong,alternative,none,both\n";
    std::ofstream causal(output / "competition-ablations.csv");
    causal << "assignment,seed,cue,intervention,outcome,decision_A,decision_B,structural_probe,valid\n";
    std::ofstream repeated(output / "repeated-negative.csv");
    repeated << "assignment,seed,cue,repetition,outcome,cue_to_A,cue_to_B,inhib_AtoB,inhib_BtoA,negative_changes\n";
    std::ofstream timing(output / "timing-diagnostics.csv");
    timing << "assignment,seed,cue,diagnostic,value,outcome,decision_A,decision_B,membrane_A,membrane_B\n";
    std::ofstream alternatives(output / "alternative-binding.csv");
    alternatives << "assignment,seed,cue,target,pre_exploration,random_action,was_alternative,positive,eligibility_before_feedback,changed_cues,changed_inhibition,cue_to_A_before,cue_to_B_before,cue_to_A_after,cue_to_B_after,autonomous_next,autonomous_correct,retained_other_cue\n";

    for (const bool inverse : {false, true}) {
        B49Variant variant{
            "diagnostic", "D", B46Task::mixed, B48Policy::reference,
            inverse, true, false, 0};
        const auto training_variant = b49_training_variant(variant);
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            auto state = b48_initial_state(training_variant, seed);
            std::array<std::optional<eve::bios::Runtime>, 2> candidates;
            std::array<std::uint32_t, 2> candidate_episodes{};
            for (std::uint32_t episode = 0; episode < 128; ++episode) {
                auto before_episode = state.runtime;
                const auto outcome = b48_episode(
                    state, training_variant, episode, nullptr, nullptr,
                    nullptr, nullptr);
                const bool autonomous_unique =
                    outcome.autonomous_a != outcome.autonomous_b;
                const bool autonomous_correct = outcome.target_a
                    ? outcome.autonomous_a && !outcome.autonomous_b
                    : outcome.autonomous_b && !outcome.autonomous_a;
                if (autonomous_unique && !autonomous_correct &&
                    !candidates[outcome.cue].has_value()) {
                    candidates[outcome.cue] = std::move(before_episode);
                    candidate_episodes[outcome.cue] = episode;
                }
            }
            for (std::uint8_t cue = 0; cue < 2; ++cue) {
                if (!candidates[cue].has_value()) continue;
                auto decision_state = *candidates[cue];
                const auto source_episode = candidate_episodes[cue];
                const bool target_a = b48_target_a(
                    training_variant, cue, source_episode, 128);
                const auto before_weights = decision_state.network().synapses();
                const auto eligibility_before =
                    decision_state.network().active_eligibility_count();
                const auto label = std::string(inverse ? "inverse" : "normal") +
                    ',' + std::to_string(seed) + ',' + std::to_string(cue) +
                    ",initial";
                const auto initial = b49_probe(decision_state, state.circuit,
                    cue, seed <= 4 ? &event_chain : nullptr, label);
                if (!b49_unique_wrong(initial, target_a)) continue;
                const auto chosen = initial.motor_a ? 0U : 1U;
                const auto dominant_id = state.circuit.cue_synapses[cue * 2 + chosen];
                const auto competitor_id =
                    state.circuit.cue_synapses[cue * 2 + (1U - chosen)];
                const auto negative_before =
                    decision_state.metrics().plasticity_negative_changes;
                const auto post_decision = decision_state;
                static_cast<void>(decision_state.process_modulatory_feedback(-1));
                const auto post_feedback = decision_state;
                const auto eligibility_after =
                    decision_state.network().active_eligibility_count();
                std::uint32_t changed_cues = 0, changed_inhibition = 0;
                for (const auto id : state.circuit.cue_synapses) {
                    changed_cues += before_weights[id].weight !=
                        decision_state.network().synapse(id).weight;
                }
                for (const auto id : state.circuit.inhibitory_outputs) {
                    changed_inhibition += before_weights[id].weight !=
                        decision_state.network().synapse(id).weight;
                }
                if (seed <= 4) {
                    for (const auto id : {dominant_id, competitor_id,
                             state.circuit.inhibitory_outputs[0],
                             state.circuit.inhibitory_outputs[1]}) {
                        if (before_weights[id].weight ==
                            decision_state.network().synapse(id).weight) {
                            continue;
                        }
                        event_chain << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << static_cast<unsigned>(cue)
                            << ",negative,modulation,-1,"
                            << decision_state.simulated_time() << ",-1," << id
                            << ',' << decision_state.network().synapse(id).weight
                            << ',' << (decision_state.network().synapse(id).effect ==
                                eve::SynapticEffect::inhibitory)
                            << ",0,0," << eligibility_after << '\n';
                    }
                }
                static_cast<void>(decision_state.advance_time_by(16));
                const auto repeat = b49_probe(decision_state, state.circuit, cue,
                    seed <= 4 ? &event_chain : nullptr,
                    std::string(inverse ? "inverse" : "normal") + ',' +
                        std::to_string(seed) + ',' + std::to_string(cue) +
                        ",repeat");
                const bool same = (chosen == 0 && repeat.motor_a && !repeat.motor_b) ||
                    (chosen == 1 && repeat.motor_b && !repeat.motor_a);
                const bool alternative = (chosen == 0 && repeat.motor_b && !repeat.motor_a) ||
                    (chosen == 1 && repeat.motor_a && !repeat.motor_b);
                failures << (inverse ? "inverse" : "normal") << ',' << seed
                    << ',' << source_episode << ','
                    << static_cast<unsigned>(cue) << ','
                    << (target_a ? 'A' : 'B') << ','
                    << b47_outcome(initial.motor_a, initial.motor_b) << ','
                    << b47_outcome(repeat.motor_a, repeat.motor_b) << ','
                    << initial.decision_a << ',' << initial.decision_b << ','
                    << initial.impulses.size() << ',' << eligibility_before << ','
                    << eligibility_after << ',' << changed_cues << ','
                    << changed_inhibition << ',' << before_weights[dominant_id].weight
                    << ',' << post_feedback.network().synapse(dominant_id).weight
                    << ',' << before_weights[competitor_id].weight << ','
                    << post_feedback.network().synapse(competitor_id).weight << ','
                    << same << ',' << alternative << ','
                    << (!repeat.motor_a && !repeat.motor_b) << ','
                    << (repeat.motor_a && repeat.motor_b) << '\n';

                for (const std::string intervention : {
                         "control", "disable_inhibition", "reduce_dominant",
                         "reset_inhibitory_weights", "probe_underleg_path"}) {
                    auto copy = post_feedback;
                    bool structural_probe = false;
                    if (intervention == "disable_inhibition") {
                        for (const auto id : state.circuit.inhibitory_outputs) {
                            copy.set_experimental_synapse_weight(id, 0);
                            copy.set_experimental_synapse_effect(
                                id, eve::SynapticEffect::excitatory);
                        }
                    } else if (intervention == "reduce_dominant") {
                        const auto weight = copy.network().synapse(dominant_id).weight;
                        copy.set_experimental_synapse_weight(
                            dominant_id, std::max(0, weight - 1));
                    } else if (intervention == "reset_inhibitory_weights") {
                        for (const auto id : state.circuit.inhibitory_outputs) {
                            copy.set_experimental_synapse_weight(id, 0);
                        }
                    } else if (intervention == "probe_underleg_path") {
                        structural_probe = true;
                        bool fired = false;
                        copy.set_fire_observer([&](const eve::bios::FireObservation& event) {
                            fired = fired || event.neuron ==
                                state.circuit.motors[1U - chosen];
                        });
                        copy.enqueue_fire(state.circuit.explorers[1U - chosen]);
                        static_cast<void>(copy.run());
                        copy.set_fire_observer({});
                        causal << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << static_cast<unsigned>(cue) << ','
                            << intervention << ',' << (fired ? "reachable" : "blocked")
                            << ",0,0,1," << copy.network().validate() << '\n';
                        continue;
                    }
                    static_cast<void>(copy.advance_time_by(16));
                    const auto outcome = b49_probe(copy, state.circuit, cue);
                    causal << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << static_cast<unsigned>(cue) << ',' << intervention
                        << ',' << b47_outcome(outcome.motor_a, outcome.motor_b)
                        << ',' << outcome.decision_a << ',' << outcome.decision_b
                        << ',' << structural_probe << ','
                        << copy.network().validate() << '\n';
                }

                auto repeated_state = post_feedback;
                for (std::uint32_t repetition = 1; repetition <= 4; ++repetition) {
                    static_cast<void>(repeated_state.advance_time_by(16));
                    const auto outcome = b49_probe(
                        repeated_state, state.circuit, cue);
                    if (b49_unique_wrong(outcome, target_a)) {
                        static_cast<void>(
                            repeated_state.process_modulatory_feedback(-1));
                    }
                    repeated << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << static_cast<unsigned>(cue) << ',' << repetition
                        << ',' << b47_outcome(outcome.motor_a, outcome.motor_b);
                    for (const auto id : {
                             state.circuit.cue_synapses[cue * 2],
                             state.circuit.cue_synapses[cue * 2 + 1],
                             state.circuit.inhibitory_outputs[0],
                             state.circuit.inhibitory_outputs[1]}) {
                        repeated << ',' << repeated_state.network().synapse(id).weight;
                    }
                    repeated << ',' << repeated_state.metrics().plasticity_negative_changes
                        - negative_before << '\n';
                }

                for (const std::uint64_t gap : {0ULL, 4ULL, 8ULL, 16ULL, 32ULL}) {
                    auto copy = post_feedback;
                    if (gap != 0) static_cast<void>(copy.advance_time_by(gap));
                    const auto outcome = b49_probe(copy, state.circuit, cue);
                    timing << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << static_cast<unsigned>(cue) << ",repeat_gap,"
                        << gap << ','
                        << b47_outcome(outcome.motor_a, outcome.motor_b) << ','
                        << outcome.decision_a << ',' << outcome.decision_b << ','
                        << copy.network().neuron(state.circuit.decisions[0]).membrane_potential
                        << ',' << copy.network().neuron(
                            state.circuit.decisions[1]).membrane_potential << '\n';
                }

                for (const std::uint64_t delay : {0ULL, 4ULL, 8ULL, 16ULL, 32ULL}) {
                    auto copy = post_decision;
                    if (delay != 0) static_cast<void>(copy.advance_time_by(delay));
                    static_cast<void>(copy.process_modulatory_feedback(-1));
                    static_cast<void>(copy.advance_time_by(16));
                    const auto outcome = b49_probe(copy, state.circuit, cue);
                    timing << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << static_cast<unsigned>(cue)
                        << ",feedback_delay," << delay << ','
                        << b47_outcome(outcome.motor_a, outcome.motor_b) << ','
                        << outcome.decision_a << ',' << outcome.decision_b << ','
                        << copy.network().neuron(
                            state.circuit.decisions[0]).membrane_potential << ','
                        << copy.network().neuron(
                            state.circuit.decisions[1]).membrane_potential << '\n';
                }

                auto alternative_state = post_feedback;
                static_cast<void>(alternative_state.advance_time_by(16));
                const auto pre_exploration = b49_probe(
                    alternative_state, state.circuit, cue);
                const auto random_action =
                    state.plans[(source_episode + 1) % state.plans.size()].exploration;
                bool action_a = pre_exploration.motor_a;
                bool action_b = pre_exploration.motor_b;
                alternative_state.set_fire_observer(
                    [&](const eve::bios::FireObservation& event) {
                        action_a = action_a || event.neuron == state.circuit.motors[0];
                        action_b = action_b || event.neuron == state.circuit.motors[1];
                    });
                alternative_state.enqueue_fire(
                    state.circuit.explorers[random_action]);
                static_cast<void>(alternative_state.run());
                alternative_state.set_fire_observer({});
                const bool positive = target_a ? action_a && !action_b
                                               : action_b && !action_a;
                const auto alternative_before =
                    alternative_state.network().synapses();
                const auto alternative_eligibility =
                    alternative_state.network().active_eligibility_count();
                static_cast<void>(alternative_state.process_modulatory_feedback(
                    positive ? 1 : -1));
                std::uint32_t alternative_changed_cues = 0;
                std::uint32_t alternative_changed_inhibition = 0;
                for (const auto id : state.circuit.cue_synapses) {
                    alternative_changed_cues += alternative_before[id].weight !=
                        alternative_state.network().synapse(id).weight;
                }
                for (const auto id : state.circuit.inhibitory_outputs) {
                    alternative_changed_inhibition +=
                        alternative_before[id].weight !=
                        alternative_state.network().synapse(id).weight;
                }
                static_cast<void>(alternative_state.advance_time_by(16));
                const auto next = b49_probe(alternative_state, state.circuit, cue);
                const auto other = b49_probe(
                    alternative_state, state.circuit, 1U - cue);
                const bool other_target_a = b48_target_a(
                    training_variant, 1U - cue, source_episode, 128);
                const bool other_retained = other_target_a
                    ? other.motor_a && !other.motor_b
                    : other.motor_b && !other.motor_a;
                const bool autonomous_correct = target_a
                    ? next.motor_a && !next.motor_b
                    : next.motor_b && !next.motor_a;
                alternatives << (inverse ? "inverse" : "normal") << ',' << seed
                    << ',' << static_cast<unsigned>(cue) << ','
                    << (target_a ? 'A' : 'B') << ','
                    << b47_outcome(pre_exploration.motor_a,
                        pre_exploration.motor_b) << ','
                    << static_cast<unsigned>(random_action) << ','
                    << (random_action != chosen) << ',' << positive << ','
                    << alternative_eligibility << ','
                    << alternative_changed_cues << ','
                    << alternative_changed_inhibition << ','
                    << alternative_before[
                        state.circuit.cue_synapses[cue * 2]].weight << ','
                    << alternative_before[
                        state.circuit.cue_synapses[cue * 2 + 1]].weight << ','
                    << alternative_state.network().synapse(
                        state.circuit.cue_synapses[cue * 2]).weight << ','
                    << alternative_state.network().synapse(
                        state.circuit.cue_synapses[cue * 2 + 1]).weight << ','
                    << b47_outcome(next.motor_a, next.motor_b) << ','
                    << autonomous_correct << ',' << other_retained << '\n';
            }
        }
    }
}

void b49_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,checkpoint_fingerprint,exact_restore,exact_continuation,valid\n";
    B49Variant variant{"persistence", "P", B46Task::mixed,
        B48Policy::reference, false, true, false, 0};
    const auto training_variant = b49_training_variant(variant);
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto state = b48_initial_state(training_variant, seed);
        b48_train_range(state, training_variant, 0, 128);
        const auto path = output / (".b49-state-" + std::to_string(seed));
        state.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        auto original = state.runtime;
        static_cast<void>(original.run());
        static_cast<void>(restored.run());
        file << seed << ',' << state.runtime.state_fingerprint() << ',' << exact
            << ',' << (original.state_fingerprint() == restored.state_fingerprint())
            << ',' << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B49_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b49");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    std::ofstream trials(output / "evaluation-trials.csv");
    trials << "variant,group,family,seed,trial,cue,target,E1_outcome,E2_outcome,E2_explored,shared_fingerprint\n";
    std::map<std::string, std::vector<B49Result>> discovery;
    std::map<std::string, B49Variant> variants;
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
                {"K4_random_extra_" + assignment, "K4", B46Task::mixed,
                    B48Policy::random_extra, inverse, true, false, family},
                {"K5_b48_adaptive_" + assignment, "K5", B46Task::mixed,
                    B48Policy::adaptive_after_negative, inverse, true, false, family},
                {"K6_redistributed_" + assignment, "K6", B46Task::mixed,
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
            }
        }
    }
    b49_write_results(output, "summary.csv", discovery, variants);
    b49_write_seed_data(output, discovery, variants);
    b49_paired(output, discovery, variants);
    b49_reference_reproduction(output);

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
                    confirmation[b49_key(variant)].push_back(
                        b49_run(variant, seed));
                }
            }
        }
    }
    b49_write_results(
        output, "confirmation.csv", confirmation, confirmation_variants);

    std::map<std::string, std::vector<B49Result>> tasks;
    std::map<std::string, B49Variant> task_variants;
    for (const bool inverse : {false, true}) {
        for (const auto task : {B46Task::only_x, B46Task::only_y,
                 B46Task::x_then_y, B46Task::y_then_x,
                 B46Task::alternating, B46Task::mixed,
                 B46Task::reversal, B46Task::retention}) {
            for (const auto policy : {B48Policy::reference,
                                      B48Policy::adaptive_after_negative}) {
                B49Variant variant{
                    std::string(policy == B48Policy::reference ? "task_K0_"
                                                               : "task_K5_") +
                        (inverse ? "inverse_" : "normal_") + b46_task_name(task),
                    policy == B48Policy::reference ? "K0" : "K5", task,
                    policy, inverse, true, false, 0};
                task_variants.emplace(b49_key(variant), variant);
                for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                    tasks[b49_key(variant)].push_back(b49_run(variant, seed));
                }
            }
        }
    }
    b49_write_results(output, "task-matrix.csv", tasks, task_variants);

    std::ofstream event_chain(output / "event-chains.csv");
    event_chain << "assignment,seed,cue,phase,kind,event,time,neuron,synapse,weight,inhibitory,membrane_before,membrane_after,eligibility_candidates\n";
    b49_failure_diagnostics(output, event_chain);
    b49_persistence(output);

    std::ofstream mechanism(output / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K7,0,negative modulation changes the immediate decision while inhibition and timing ablations do not; successful alternatives are not reliably consolidated and repeated negatives erode activity; no single isolated activation-state bottleneck justifies a new local state\n";
    mechanism << "K8,0,no K7 state exists to deactivate\n";

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
            x += sx; y += sy; robust += sx >= .75 && sy >= .75;
            differences.push_back(b49_score(rows[index], true) -
                b49_score(controls[index], true));
        }
        x /= rows.size(); y /= rows.size();
        const auto interval = bootstrap_interval(differences, salt);
        const auto advantage = b48_mean(differences);
        const bool pass = x >= .75 && y >= .75 && robust >= 48 &&
            advantage >= .15 && interval.first > 0;
        criteria << phase << ',' << candidate.group << ',' << candidate.family
            << ',' << (candidate.inverse ? "inverse" : "normal") << ',' << x
            << ',' << y << ',' << robust << ',' << advantage << ','
            << interval.first << ',' << interval.second << ',' << pass << '\n';
    };
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto suffix = std::string(inverse ? "inverse" : "normal") +
                "_f" + std::to_string(family);
            emit_criteria("discovery", variants.at("K5_b48_adaptive_" + suffix),
                discovery.at("K5_b48_adaptive_" + suffix),
                discovery.at("K1_no_learning_" + suffix),
                0x4906000ULL + family * 2 + inverse);
            emit_criteria("confirmation",
                confirmation_variants.at("confirmation_K5_" + suffix),
                confirmation.at("confirmation_K5_" + suffix),
                confirmation.at("confirmation_K1_" + suffix),
                0x4907000ULL + family * 2 + inverse);
        }
    }

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    const double wall = std::chrono::duration<double>(ended - started).count();
    const double cpu = static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC;
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,discovery_runs,confirmation_runs,task_runs\n";
    performance << wall << ',' << cpu << ',' << usage.ru_maxrss << ','
        << 3 * 2 * 7 * 64 << ',' << 3 * 2 * 3 * 64 << ','
        << 2 * 8 * 2 * 64 << '\n';
    std::cout << "B.4.9 discovery=" << 3 * 2 * 7 * 64
              << " confirmation=" << 3 * 2 * 3 * 64
              << " K7=not_justified\n";
    return 0;
}
#endif
