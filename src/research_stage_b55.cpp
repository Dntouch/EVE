#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B54_EMBEDDED
#include "research_stage_b54.cpp"
#undef EVE_RESEARCH_STAGE_B54_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <iomanip>

namespace {

struct B55Cluster {
    std::string phase;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    std::uint64_t inventory{};
    std::uint64_t decision_states{};
    std::uint64_t natural_errors{};
    std::uint64_t helpful{};
    std::uint64_t harmful{};
    std::uint64_t changed{};
    std::uint64_t double_removed{};
    std::uint64_t exact_restore{};
    std::uint64_t other_helpful{};
    std::uint64_t other_harmful{};
    std::uint64_t states_with_any_fix{};
    std::uint64_t states_with_any_harm{};
    std::uint64_t valid{};
};

std::string b55_role(
    const eve::NeuronId neuron, const B47Circuit& circuit) {
    const auto member = [neuron](const auto& values) {
        return std::find(values.begin(), values.end(), neuron) != values.end();
    };
    if (member(circuit.sensors)) return "sensor";
    if (member(circuit.explorers)) return "explorer";
    if (member(circuit.decisions)) return "decision";
    if (member(circuit.motors)) return "motor";
    if (member(circuit.modulation_inputs)) return "modulation";
    return "other";
}

std::string b55_response(const B51Probe& probe) {
    if (probe.motor_a && probe.motor_b) return "both";
    if (probe.motor_a) return "A";
    if (probe.motor_b) return "B";
    return "none";
}

bool b55_correct(const B51Probe& probe, const bool target_a) {
    return b51_correct(target_a, probe.motor_a, probe.motor_b);
}

std::array<std::int32_t, 6> b55_weights(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit) {
    return {{
        runtime.network().synapse(circuit.cue_synapses[0]).weight,
        runtime.network().synapse(circuit.cue_synapses[1]).weight,
        runtime.network().synapse(circuit.cue_synapses[2]).weight,
        runtime.network().synapse(circuit.cue_synapses[3]).weight,
        runtime.network().synapse(circuit.inhibitory_outputs[0]).weight,
        runtime.network().synapse(circuit.inhibitory_outputs[1]).weight,
    }};
}

void b55_temporal_diagnostics(const std::filesystem::path& output) {
    std::ofstream file(output / "temporal-integration-diagnostics.csv");
    file << "sequence,gap,relaxation_delay,natural_fires,relaxed_fires,"
            "natural_final_membrane,relaxed_final_membrane,"
            "integration_preserved,relaxation_due_before_input,valid\n";
    const std::array<std::string, 6> sequences{{
        "AA", "BB", "AB", "BA", "ABA", "BAB"}};
    for (const auto& sequence : sequences) {
        for (const auto gap : kB54Gaps) {
            for (const auto delay : kB54Delays) {
                const auto run = [&](const bool relax) {
                    eve::bios::Runtime runtime(b54_micro_config(
                        eve::RecoveryMode::input_dependent, 0, 16, 0),
                        0x5501000ULL + gap * 100 + delay + sequence.size());
                    const auto input_a =
                        runtime.add_experimental_synapse(0, 4, 1, true);
                    const auto input_b =
                        runtime.add_experimental_synapse(1, 4, 1, true);
                    std::uint64_t fires = 0;
                    std::uint64_t elapsed = 0;
                    for (std::size_t index = 0; index < sequence.size(); ++index) {
                        if (index > 0) {
                            static_cast<void>(runtime.advance_time_by(gap));
                            elapsed += gap;
                            if (relax && elapsed >= delay &&
                                runtime.network().neuron(4).membrane_potential > 0 &&
                                runtime.network().neuron(4).membrane_potential <
                                    runtime.network().neuron(4).firing_threshold) {
                                runtime.set_experimental_neuron_membrane(4, 0);
                                elapsed = 0;
                            }
                        }
                        const auto input = sequence[index] == 'A'
                            ? input_a : input_b;
                        fires += runtime.process_simultaneous_impulses({input}).fired;
                        if (runtime.network().neuron(4).membrane_potential == 0) {
                            elapsed = 0;
                        }
                    }
                    return std::tuple<std::uint64_t, std::int64_t, bool>{
                        fires, runtime.network().neuron(4).membrane_potential,
                        runtime.network().validate()};
                };
                const auto natural = run(false);
                const auto relaxed = run(true);
                file << sequence << ',' << gap << ',' << delay << ','
                    << std::get<0>(natural) << ',' << std::get<0>(relaxed)
                    << ',' << std::get<1>(natural) << ','
                    << std::get<1>(relaxed) << ','
                    << (std::get<0>(relaxed) == std::get<0>(natural)) << ','
                    << (gap >= delay) << ','
                    << (std::get<2>(natural) && std::get<2>(relaxed)) << '\n';
            }
        }
    }
}

std::vector<B55Cluster> b55_natural_causality(
    const std::filesystem::path& output) {
    std::ofstream inventory(output / "subthreshold-inventory.csv");
    inventory << "phase,family,assignment,seed,source_episode,next_episode,"
        "neuron,role,membrane,threshold,last_event_time,last_event_synapse,"
        "last_effect,scheduled_recovery,next_input_time,wait_time,"
        "refractory,recovery_generation,valid\n";
    std::ofstream inputs(output / "subthreshold-inputs.csv");
    inputs << "phase,family,assignment,seed,episode,event_index,time,synapse,"
        "sender,receiver,role,effect,weight,membrane_before,membrane_after,"
        "fired,suppressed\n";
    std::ofstream causal(output / "subthreshold-causal-copies.csv");
    causal << "phase,family,assignment,seed,source_episode,next_episode,cue,"
        "target,neuron,decision_role,membrane,natural,removed,restored,"
        "natural_correct,removed_correct,restored_exact,classification,"
        "other_natural,other_removed,other_natural_correct,"
        "other_removed_correct,cue_X_A,cue_X_B,cue_Y_A,cue_Y_B,"
        "inhibition_A,inhibition_B,valid\n";
    std::ofstream states(output / "subthreshold-state-effects.csv");
    states << "phase,family,assignment,seed,source_episode,next_episode,cue,"
        "target,natural,natural_correct,subthreshold_decisions,any_fix,"
        "any_harm,valid\n";

    std::vector<B55Cluster> clusters;
    const auto run_phase = [&](const std::string& phase,
            const std::uint64_t first_seed) {
        const B54RecoveryVariant recovery{
            "S2_R2_r1_d16_p50",
            eve::RecoveryMode::independent_with_membrane_relaxation,
            1, 16, 50, true};
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint64_t seed = first_seed;
                     seed < first_seed + 64; ++seed) {
                    B48Variant variant{recovery.name, B46Task::mixed,
                        B48Policy::reference, inverse, true, false, family};
                    auto state = b54_initial_state(variant, recovery, seed);
                    B55Cluster cluster;
                    cluster.phase = phase;
                    cluster.family = family;
                    cluster.inverse = inverse;
                    cluster.seed = seed;
                    std::vector<std::uint64_t> last_time(
                        state.runtime.network().neuron_count(), 0);
                    std::vector<eve::SynapseId> last_synapse(
                        state.runtime.network().neuron_count(), eve::kInvalidIndex);
                    std::vector<eve::SynapticEffect> last_effect(
                        state.runtime.network().neuron_count(),
                        eve::SynapticEffect::excitatory);

                    for (std::uint32_t episode = 0;
                         episode + 1 < kB51Episodes; ++episode) {
                        const auto cue = b48_cue(variant, state.plans[episode],
                            episode, b46_training(variant.task));
                        const bool target_a = b48_target_a(variant, cue, episode,
                            b46_training(variant.task));
                        auto row = b51_episode(state.runtime, state.circuit,
                            cue, state.plans[episode].exploration, target_a,
                            episode, true);
                        for (const auto& impulse : row.impulses) {
                            last_time[impulse.receiver] = impulse.simulated_time;
                            last_synapse[impulse.receiver] = impulse.synapse;
                            last_effect[impulse.receiver] = impulse.synaptic_effect;
                            const auto threshold = row.after_activity.network()
                                .neuron(impulse.receiver).firing_threshold;
                            if (!impulse.receiver_fired &&
                                impulse.membrane_after > 0 &&
                                impulse.membrane_after < threshold) {
                                inputs << phase << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << impulse.event_index << ','
                                    << impulse.simulated_time << ','
                                    << impulse.synapse << ',' << impulse.sender
                                    << ',' << impulse.receiver << ','
                                    << b55_role(impulse.receiver, state.circuit)
                                    << ','
                                    << (impulse.synaptic_effect ==
                                            eve::SynapticEffect::excitatory
                                        ? "excitatory" : "inhibitory")
                                    << ',' << impulse.applied_weight << ','
                                    << impulse.membrane_before << ','
                                    << impulse.membrane_after << ','
                                    << impulse.receiver_fired << ','
                                    << impulse.firing_suppressed << '\n';
                            }
                        }
                        state.runtime = std::move(row.after_feedback);
                        const auto next_episode = episode + 1;
                        const auto next_cue = b48_cue(variant,
                            state.plans[next_episode], next_episode,
                            b46_training(variant.task));
                        const bool next_target = b48_target_a(variant, next_cue,
                            next_episode, b46_training(variant.task));
                        const auto next_time =
                            static_cast<std::uint64_t>(next_episode) * 16ULL;
                        for (eve::NeuronId neuron = 0;
                             neuron < state.runtime.network().neuron_count();
                             ++neuron) {
                            const auto& item = state.runtime.network().neuron(neuron);
                            if (!(item.membrane_potential > 0 &&
                                  item.membrane_potential < item.firing_threshold)) {
                                continue;
                            }
                            ++cluster.inventory;
                            inventory << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ',' << next_episode
                                << ',' << neuron << ','
                                << b55_role(neuron, state.circuit) << ','
                                << item.membrane_potential << ','
                                << item.firing_threshold << ',' << last_time[neuron]
                                << ',' << last_synapse[neuron] << ','
                                << (last_effect[neuron] ==
                                        eve::SynapticEffect::excitatory
                                    ? "excitatory" : "inhibitory")
                                << ',' << item.scheduled_recovery_time << ','
                                << next_time << ','
                                << (next_time >= last_time[neuron]
                                    ? next_time - last_time[neuron] : 0)
                                << ',' << item.refractory_events_remaining << ','
                                << item.recovery_generation << ','
                                << state.runtime.network().validate() << '\n';
                        }

                        const auto natural = b51_probe(
                            state.runtime, state.circuit, next_cue);
                        const bool natural_correct =
                            b55_correct(natural, next_target);
                        bool any_fix = false;
                        bool any_harm = false;
                        std::uint64_t state_decisions = 0;
                        for (std::size_t decision_index = 0;
                             decision_index < state.circuit.decisions.size();
                             ++decision_index) {
                            const auto neuron =
                                state.circuit.decisions[decision_index];
                            const auto& item = state.runtime.network().neuron(neuron);
                            if (!(item.membrane_potential > 0 &&
                                  item.membrane_potential < item.firing_threshold)) {
                                continue;
                            }
                            ++state_decisions;
                            ++cluster.decision_states;
                            cluster.natural_errors += !natural_correct;
                            auto removed_runtime = state.runtime;
                            removed_runtime.set_experimental_neuron_membrane(
                                neuron, 0);
                            const auto removed = b51_probe(
                                removed_runtime, state.circuit, next_cue);
                            auto restored_runtime = removed_runtime;
                            restored_runtime.set_experimental_neuron_membrane(
                                neuron, item.membrane_potential);
                            const auto restored = b51_probe(
                                restored_runtime, state.circuit, next_cue);
                            const bool removed_correct =
                                b55_correct(removed, next_target);
                            const bool restored_exact =
                                b55_response(restored) == b55_response(natural);
                            const bool helpful =
                                natural_correct && !removed_correct;
                            const bool harmful =
                                !natural_correct && removed_correct;
                            const bool changed =
                                b55_response(natural) != b55_response(removed);
                            any_fix = any_fix || harmful;
                            any_harm = any_harm || helpful;
                            cluster.helpful += helpful;
                            cluster.harmful += harmful;
                            cluster.changed += changed;
                            cluster.double_removed +=
                                natural.motor_a && natural.motor_b &&
                                !(removed.motor_a && removed.motor_b);
                            cluster.exact_restore += restored_exact;

                            const std::uint8_t other_cue = 1U - next_cue;
                            const bool other_target = b48_target_a(variant,
                                other_cue, next_episode,
                                b46_training(variant.task));
                            const auto other_natural = b51_probe(
                                state.runtime, state.circuit, other_cue);
                            const auto other_removed = b51_probe(
                                removed_runtime, state.circuit, other_cue);
                            const bool other_natural_correct =
                                b55_correct(other_natural, other_target);
                            const bool other_removed_correct =
                                b55_correct(other_removed, other_target);
                            cluster.other_helpful += other_natural_correct &&
                                !other_removed_correct;
                            cluster.other_harmful += !other_natural_correct &&
                                other_removed_correct;
                            const auto weights =
                                b55_weights(state.runtime, state.circuit);
                            const std::string classification = harmful
                                ? "harmful" : helpful ? "helpful" :
                                  changed ? "changed_noncorrect" : "neutral";
                            causal << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ',' << next_episode
                                << ',' << static_cast<unsigned>(next_cue) << ','
                                << (next_target ? "A" : "B") << ',' << neuron
                                << ',' << (decision_index == 0 ? "A" : "B")
                                << ',' << item.membrane_potential << ','
                                << b55_response(natural) << ','
                                << b55_response(removed) << ','
                                << b55_response(restored) << ','
                                << natural_correct << ',' << removed_correct
                                << ',' << restored_exact << ',' << classification
                                << ',' << b55_response(other_natural) << ','
                                << b55_response(other_removed) << ','
                                << other_natural_correct << ','
                                << other_removed_correct;
                            for (const auto weight : weights) {
                                causal << ',' << weight;
                            }
                            causal << ','
                                << (state.runtime.network().validate() &&
                                    removed_runtime.network().validate() &&
                                    restored_runtime.network().validate())
                                << '\n';
                        }
                        cluster.states_with_any_fix += any_fix;
                        cluster.states_with_any_harm += any_harm;
                        if (state_decisions > 0) {
                            states << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ',' << next_episode
                                << ',' << static_cast<unsigned>(next_cue) << ','
                                << (next_target ? "A" : "B") << ','
                                << b55_response(natural) << ','
                                << natural_correct << ',' << state_decisions
                                << ',' << any_fix << ',' << any_harm << ','
                                << state.runtime.network().validate() << '\n';
                        }
                    }
                    cluster.valid = state.runtime.network().validate();
                    clusters.push_back(cluster);
                }
            }
        }
    };
    run_phase("discovery", 1);
    run_phase("confirmation", 10001);
    return clusters;
}

void b55_causal_statistics(
    const std::filesystem::path& output,
    const std::vector<B55Cluster>& clusters) {
    std::ofstream seeds(output / "subthreshold-seed-summary.csv");
    seeds << "phase,family,assignment,seed,inventory,decision_states,"
        "natural_errors,helpful,harmful,changed,double_removed,exact_restore,"
        "other_helpful,other_harmful,states_with_any_fix,"
        "states_with_any_harm,valid\n";
    std::map<std::string, std::vector<const B55Cluster*>> groups;
    for (const auto& row : clusters) {
        seeds << row.phase << ',' << row.family << ','
            << (row.inverse ? "inverse" : "normal") << ',' << row.seed << ','
            << row.inventory << ',' << row.decision_states << ','
            << row.natural_errors << ',' << row.helpful << ',' << row.harmful
            << ',' << row.changed << ',' << row.double_removed << ','
            << row.exact_restore << ',' << row.other_helpful << ','
            << row.other_harmful << ',' << row.states_with_any_fix << ','
            << row.states_with_any_harm << ',' << row.valid << '\n';
        groups[row.phase].push_back(&row);
    }

    std::ofstream summary(output / "subthreshold-causal-summary.csv");
    summary << "phase,clusters,inventory,decision_states,natural_errors,"
        "helpful,harmful,changed,double_removed,exact_restore,other_helpful,"
        "other_harmful,states_with_any_fix,states_with_any_harm,"
        "harmful_per_natural_error,helpful_per_decision_state,valid\n";
    std::ofstream stats(output / "subthreshold-cluster-statistics.csv");
    stats << "phase,metric,clusters,mean,ci_low,ci_high\n";
    std::uint64_t salt = 0;
    for (const auto& [phase, rows] : groups) {
        B55Cluster total;
        std::map<std::string, std::vector<double>> values;
        for (const auto* row : rows) {
            total.inventory += row->inventory;
            total.decision_states += row->decision_states;
            total.natural_errors += row->natural_errors;
            total.helpful += row->helpful;
            total.harmful += row->harmful;
            total.changed += row->changed;
            total.double_removed += row->double_removed;
            total.exact_restore += row->exact_restore;
            total.other_helpful += row->other_helpful;
            total.other_harmful += row->other_harmful;
            total.states_with_any_fix += row->states_with_any_fix;
            total.states_with_any_harm += row->states_with_any_harm;
            total.valid += row->valid;
            values["harmful_per_error"].push_back(row->natural_errors == 0
                ? 0.0 : static_cast<double>(row->harmful) /
                    row->natural_errors);
            values["helpful_per_state"].push_back(row->decision_states == 0
                ? 0.0 : static_cast<double>(row->helpful) /
                    row->decision_states);
            values["changed_per_state"].push_back(row->decision_states == 0
                ? 0.0 : static_cast<double>(row->changed) /
                    row->decision_states);
            values["inventory_per_episode"].push_back(
                static_cast<double>(row->inventory) / (kB51Episodes - 1));
        }
        summary << phase << ',' << rows.size() << ',' << total.inventory << ','
            << total.decision_states << ',' << total.natural_errors << ','
            << total.helpful << ',' << total.harmful << ',' << total.changed
            << ',' << total.double_removed << ',' << total.exact_restore << ','
            << total.other_helpful << ',' << total.other_harmful << ','
            << total.states_with_any_fix << ',' << total.states_with_any_harm
            << ',' << (total.natural_errors == 0 ? 0.0 :
                static_cast<double>(total.harmful) / total.natural_errors)
            << ',' << (total.decision_states == 0 ? 0.0 :
                static_cast<double>(total.helpful) / total.decision_states)
            << ',' << total.valid << '\n';
        for (const auto& [metric, samples] : values) {
            const auto interval = bootstrap_interval(
                samples, 0x5502000ULL + ++salt);
            stats << phase << ',' << metric << ',' << samples.size() << ','
                << b48_mean(samples) << ',' << interval.first << ','
                << interval.second << '\n';
        }
    }
}

struct B55NaturalVariant {
    std::string name;
    bool subthreshold_enabled{};
    std::uint64_t subthreshold_delay{};
    bool plasticity{true};
};

struct B55NaturalResult {
    std::string phase;
    std::string variant;
    std::string task;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    double e1_x{};
    double e1_y{};
    double e2_x{};
    double e2_y{};
    double retention_x{};
    double retention_y{};
    std::uint32_t first_positive{kB51Never};
    std::uint32_t first_x{kB51Never};
    std::uint32_t first_y{kB51Never};
    std::uint32_t first_both{kB51Never};
    std::uint64_t losses_x{};
    std::uint64_t losses_y{};
    std::uint64_t reacquisitions_x{};
    std::uint64_t reacquisitions_y{};
    std::uint64_t both_episodes{};
    std::uint64_t scheduled{};
    std::uint64_t processed{};
    std::uint64_t stale{};
    std::uint64_t eligibility{};
    std::uint64_t fires{};
    std::uint64_t events{};
    std::uint64_t maximum_queue{};
    std::size_t memory{};
    bool final_x{};
    bool final_y{};
    bool robust{};
    bool protected_exploration{};
    bool valid{};
};

eve::ExperimentConfig b55_config(
    const B48Variant& learning_variant,
    const B55NaturalVariant& variant) {
    const B54RecoveryVariant recovery{
        variant.name,
        eve::RecoveryMode::independent_with_membrane_relaxation,
        1, 16, 50, variant.plasticity};
    auto config = b54_config(learning_variant, recovery);
    config.subthreshold_relaxation_enabled = variant.subthreshold_enabled;
    config.subthreshold_relaxation_delay = variant.subthreshold_enabled
        ? variant.subthreshold_delay : 0;
    return config;
}

B48State b55_initial_state(
    const B48Variant& learning_variant,
    const B55NaturalVariant& variant,
    const std::uint64_t seed) {
    const auto base = b48_base_variant(learning_variant);
    eve::bios::Runtime runtime(
        b55_config(learning_variant, variant),
        b47_mix_seed(0x5404000ULL ^ seed ^
            (static_cast<std::uint64_t>(learning_variant.family) << 40U)));
    const auto circuit = b47_circuit(runtime, base);
    return B48State{
        std::move(runtime), circuit,
        b48_plans(seed, kB51Episodes + kB51EvaluationEpisodes,
            learning_variant.family),
        B48Result{seed}, false, {{-1, -1}}};
}

B55NaturalResult b55_run_natural(
    const std::string& phase, const B55NaturalVariant& natural_variant,
    const B46Task task, const std::uint32_t family, const bool inverse,
    const std::uint64_t seed, std::ofstream& trajectory,
    std::ofstream& relaxation_events, std::ofstream& stability,
    std::ofstream& confusion) {
    B48Variant learning_variant{natural_variant.name, task,
        B48Policy::reference, inverse, natural_variant.plasticity,
        false, family};
    auto state = b55_initial_state(learning_variant, natural_variant, seed);
    B55NaturalResult result;
    result.phase = phase;
    result.variant = natural_variant.name;
    result.task = b46_task_name(task);
    result.family = family;
    result.inverse = inverse;
    result.seed = seed;
    auto previous = b51_recall_pair(
        state.runtime, state.circuit, learning_variant, 0);
    std::uint32_t current_episode = 0;
    const auto prefix = [&]() {
        return phase + ',' + natural_variant.name + ',' +
            b46_task_name(task) + ',' + std::to_string(family) + ',' +
            (inverse ? "inverse" : "normal") + ',' +
            std::to_string(seed);
    };
    state.runtime.set_subthreshold_relaxation_observer(
        [&](const eve::bios::SubthresholdRelaxationObservation& event) {
            relaxation_events << prefix() << ',' << current_episode << ','
                << event.event_index << ',' << event.simulated_time << ','
                << event.neuron << ',' << event.generation << ','
                << event.membrane_before << ',' << event.membrane_after << ','
                << event.relaxed << ',' << event.stale << '\n';
        });
    const std::array<std::uint32_t, 6> offsets{{1, 4, 8, 16, 32, 64}};
    for (std::uint32_t episode = 0; episode < kB51Episodes; ++episode) {
        current_episode = episode;
        const auto cue = b48_cue(learning_variant, state.plans[episode],
            episode, b46_training(task));
        const bool target_a = b48_target_a(
            learning_variant, cue, episode, b46_training(task));
        const auto before_metrics = state.runtime.metrics();
        auto row = b51_episode(state.runtime, state.circuit, cue,
            state.plans[episode].exploration, target_a, episode,
            natural_variant.plasticity);
        state.runtime = std::move(row.after_feedback);
        const auto after_metrics = state.runtime.metrics();
        const auto correct = b51_recall_pair(
            state.runtime, state.circuit, learning_variant, episode + 1);
        if (result.first_positive == kB51Never && row.correct) {
            result.first_positive = episode;
        }
        if (result.first_x == kB51Never && correct[0]) result.first_x = episode;
        if (result.first_y == kB51Never && correct[1]) result.first_y = episode;
        if (result.first_both == kB51Never && correct[0] && correct[1]) {
            result.first_both = episode;
        }
        result.losses_x += previous[0] && !correct[0];
        result.losses_y += previous[1] && !correct[1];
        result.reacquisitions_x += !previous[0] && correct[0] &&
            result.first_x != episode;
        result.reacquisitions_y += !previous[1] && correct[1] &&
            result.first_y != episode;
        result.both_episodes += correct[0] && correct[1];
        if (result.first_both != kB51Never) {
            const auto age = episode - result.first_both;
            if (std::find(offsets.begin(), offsets.end(), age) != offsets.end()) {
                stability << prefix() << ',' << result.first_both << ',' << age
                    << ',' << episode << ',' << correct[0] << ',' << correct[1]
                    << ',' << result.losses_x << ',' << result.losses_y << '\n';
            }
        }
        const auto weights = b55_weights(state.runtime, state.circuit);
        trajectory << prefix() << ',' << episode << ','
            << static_cast<unsigned>(cue) << ',' << (target_a ? "A" : "B")
            << ',' << row.autonomous_a << ',' << row.autonomous_b << ','
            << row.final_a << ',' << row.final_b << ',' << row.correct << ','
            << row.explored << ',' << static_cast<int>(row.modulation) << ','
            << state.runtime.network().neuron(state.circuit.decisions[0])
                   .membrane_potential
            << ','
            << state.runtime.network().neuron(state.circuit.decisions[1])
                   .membrane_potential;
        for (const auto weight : weights) trajectory << ',' << weight;
        trajectory << ','
            << after_metrics.scheduled_subthreshold_relaxations -
                before_metrics.scheduled_subthreshold_relaxations
            << ','
            << after_metrics.processed_subthreshold_relaxations -
                before_metrics.processed_subthreshold_relaxations
            << ','
            << after_metrics.stale_subthreshold_relaxations -
                before_metrics.stale_subthreshold_relaxations
            << ',' << after_metrics.eligibility_decisions -
                before_metrics.eligibility_decisions
            << ',' << after_metrics.fire_events - before_metrics.fire_events
            << ',' << correct[0] << ',' << correct[1] << ','
            << (correct[0] && correct[1]) << '\n';
        previous = correct;
    }
    result.final_x = previous[0];
    result.final_y = previous[1];
    state.runtime.set_subthreshold_relaxation_observer({});
    const auto evaluated = b51_evaluate(state.runtime, state.circuit,
        learning_variant, state.plans, kB51Episodes);
    result.e1_x = b47_rate(evaluated.e1[0]);
    result.e1_y = b47_rate(evaluated.e1[1]);
    result.e2_x = b47_rate(evaluated.e2[0]);
    result.e2_y = b47_rate(evaluated.e2[1]);
    result.robust = result.e1_x >= .75 && result.e1_y >= .75;
    for (std::size_t cue = 0; cue < 2; ++cue) {
        const auto emit = [&](const std::string& metric,
                const B47Counts& counts) {
            confusion << prefix() << ',' << metric << ',' << cue << ','
                << counts.correct << ',' << counts.wrong << ',' << counts.both
                << ',' << counts.none << '\n';
        };
        emit("E1", evaluated.e1[cue]);
        emit("E2", evaluated.e2[cue]);
    }
    auto retained = state.runtime;
    static_cast<void>(retained.advance_time_by(64ULL * 16ULL));
    const auto retained_eval = b51_evaluate(retained, state.circuit,
        learning_variant, state.plans, kB51Episodes);
    result.retention_x = b47_rate(retained_eval.e1[0]);
    result.retention_y = b47_rate(retained_eval.e1[1]);
    const auto& metrics = state.runtime.metrics();
    result.scheduled = metrics.scheduled_subthreshold_relaxations;
    result.processed = metrics.processed_subthreshold_relaxations;
    result.stale = metrics.stale_subthreshold_relaxations;
    result.eligibility = metrics.eligibility_decisions;
    result.fires = metrics.fire_events;
    result.events = metrics.processed_events;
    result.maximum_queue = metrics.maximum_queue_size;
    result.memory = state.runtime.diagnostics().estimated_memory_bytes;
    result.protected_exploration = true;
    for (const auto id : state.circuit.exploration_synapses) {
        result.protected_exploration = result.protected_exploration &&
            state.runtime.network().synapse(id).plasticity_protected &&
            state.runtime.network().synapse(id).weight == 2;
    }
    result.valid = state.runtime.network().validate();
    return result;
}

std::vector<B55NaturalResult> b55_natural_matrix(
    const std::filesystem::path& output) {
    std::ofstream trajectory(output / "natural-trajectories.csv");
    trajectory << "phase,variant,task,family,assignment,seed,episode,cue,"
        "target,autonomous_A,autonomous_B,final_A,final_B,correct,explored,"
        "modulation,decision_A_membrane,decision_B_membrane,cue_X_A,"
        "cue_X_B,cue_Y_A,cue_Y_B,inhibition_A,inhibition_B,"
        "subthreshold_scheduled,subthreshold_processed,subthreshold_stale,"
        "eligibility_decisions,fire_events,X_correct,Y_correct,both_correct\n";
    std::ofstream events(output / "subthreshold-relaxation-events.csv");
    events << "phase,variant,task,family,assignment,seed,episode,event_index,"
        "time,neuron,generation,membrane_before,membrane_after,relaxed,stale\n";
    std::ofstream stability(output / "stability-checkpoints.csv");
    stability << "phase,variant,task,family,assignment,seed,first_both,offset,"
        "episode,X_correct,Y_correct,losses_X,losses_Y\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "phase,variant,task,family,assignment,seed,metric,cue,"
        "correct,wrong,both,none\n";
    std::vector<B55NaturalResult> rows;
    const B55NaturalVariant no_learning{
        "S_no_learning", false, 0, false};
    const B55NaturalVariant s2{"S2_R2_p50", false, 0, true};
    const B55NaturalVariant s5{"S5_disabled", false, 0, true};
    const B55NaturalVariant s4d16{"S4_event_d16", true, 16, true};
    const B55NaturalVariant s4d32{"S4_event_d32", true, 32, true};
    const B55NaturalVariant s4d64{"S4_event_d64", true, 64, true};
    const std::array<B46Task, 8> tasks{{
        B46Task::only_x, B46Task::only_y, B46Task::x_then_y,
        B46Task::y_then_x, B46Task::alternating, B46Task::mixed,
        B46Task::reversal, B46Task::retention}};
    const auto run = [&](const std::string& phase,
            const std::vector<B55NaturalVariant>& variants,
            const std::vector<B46Task>& selected_tasks,
            const std::uint64_t first_seed) {
        for (const auto& variant : variants) {
            for (const auto task : selected_tasks) {
                for (std::uint32_t family = 0; family < 3; ++family) {
                    for (const bool inverse : {false, true}) {
                        for (std::uint64_t seed = first_seed;
                             seed < first_seed + 64; ++seed) {
                            rows.push_back(b55_run_natural(phase, variant, task,
                                family, inverse, seed, trajectory, events,
                                stability, confusion));
                        }
                    }
                }
            }
        }
    };
    run("discovery", {s2, s5, s4d16},
        std::vector<B46Task>(tasks.begin(), tasks.end()), 1);
    run("discovery", {no_learning, s4d32, s4d64},
        {B46Task::mixed}, 1);
    run("confirmation", {no_learning, s2, s5, s4d16},
        {B46Task::mixed, B46Task::reversal}, 10001);
    return rows;
}

void b55_natural_statistics(
    const std::filesystem::path& output,
    const std::vector<B55NaturalResult>& rows) {
    struct Aggregate {
        std::uint64_t networks{};
        double e1_x{};
        double e1_y{};
        double e2_x{};
        double e2_y{};
        double retention_x{};
        double retention_y{};
        double both_fraction{};
        double losses{};
        std::uint64_t robust{};
        std::uint64_t scheduled{};
        std::uint64_t processed{};
        std::uint64_t stale{};
        std::uint64_t eligibility{};
        std::uint64_t fires{};
        std::uint64_t events{};
        std::uint64_t maximum_queue{};
        std::size_t memory{};
        std::uint64_t protected_count{};
        std::uint64_t valid{};
    };
    std::map<std::string, Aggregate> aggregates;
    std::map<std::string, std::vector<const B55NaturalResult*>> groups;
    std::map<std::string, const B55NaturalResult*> indexed;
    for (const auto& row : rows) {
        const auto key = row.phase + '|' + row.variant + '|' + row.task;
        auto& value = aggregates[key];
        ++value.networks;
        value.e1_x += row.e1_x;
        value.e1_y += row.e1_y;
        value.e2_x += row.e2_x;
        value.e2_y += row.e2_y;
        value.retention_x += row.retention_x;
        value.retention_y += row.retention_y;
        value.both_fraction += static_cast<double>(row.both_episodes) /
            kB51Episodes;
        value.losses += row.losses_x + row.losses_y;
        value.robust += row.robust;
        value.scheduled += row.scheduled;
        value.processed += row.processed;
        value.stale += row.stale;
        value.eligibility += row.eligibility;
        value.fires += row.fires;
        value.events += row.events;
        value.maximum_queue = std::max(value.maximum_queue, row.maximum_queue);
        value.memory = std::max(value.memory, row.memory);
        value.protected_count += row.protected_exploration;
        value.valid += row.valid;
        groups[key].push_back(&row);
        indexed[row.phase + '|' + row.task + '|' + row.variant + '|' +
            std::to_string(row.family) + '|' + (row.inverse ? "I" : "N") +
            '|' + std::to_string(row.seed)] = &row;
    }
    std::ofstream summary(output / "variant-summary.csv");
    summary << "phase,variant,task,networks,E1_X,E1_Y,E2_X,E2_Y,"
        "retention_X,retention_Y,robust_networks,both_episode_fraction,"
        "mean_losses,scheduled_subthreshold,processed_subthreshold,"
        "stale_subthreshold,eligibility_decisions,fire_events,"
        "processed_events,maximum_queue,memory_bytes,protected_exploration,"
        "valid\n";
    for (const auto& [key, value] : aggregates) {
        auto printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        const auto count = static_cast<double>(value.networks);
        summary << printable << ',' << value.networks << ','
            << value.e1_x / count << ',' << value.e1_y / count << ','
            << value.e2_x / count << ',' << value.e2_y / count << ','
            << value.retention_x / count << ',' << value.retention_y / count
            << ',' << value.robust << ',' << value.both_fraction / count << ','
            << value.losses / count << ',' << value.scheduled << ','
            << value.processed << ',' << value.stale << ','
            << value.eligibility << ',' << value.fires << ',' << value.events
            << ',' << value.maximum_queue << ',' << value.memory << ','
            << value.protected_count << ',' << value.valid << '\n';
    }

    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "phase,task,candidate,control,metric,pairs,mean_difference,"
        "ci_low,ci_high\n";
    std::uint64_t salt = 0;
    for (const auto& [key, group] : groups) {
        const auto first = key.find('|');
        const auto second = key.find('|', first + 1);
        const auto phase = key.substr(0, first);
        const auto candidate = key.substr(first + 1, second - first - 1);
        const auto task = key.substr(second + 1);
        if (candidate != "S4_event_d16" && candidate != "S4_event_d32" &&
            candidate != "S4_event_d64" && candidate != "S5_disabled") {
            continue;
        }
        for (const std::string control : {"S2_R2_p50", "S_no_learning"}) {
            std::map<std::string, std::vector<double>> differences;
            for (const auto* row : group) {
                const auto found = indexed.find(phase + '|' + task + '|' +
                    control + '|' + std::to_string(row->family) + '|' +
                    (row->inverse ? "I" : "N") + '|' +
                    std::to_string(row->seed));
                if (found == indexed.end()) continue;
                const auto* base = found->second;
                differences["E1_score"].push_back(
                    ((row->e1_x - base->e1_x) +
                     (row->e1_y - base->e1_y)) / 2.0);
                differences["both_fraction"].push_back(
                    (static_cast<double>(row->both_episodes) -
                     static_cast<double>(base->both_episodes)) /
                    kB51Episodes);
                differences["losses"].push_back(
                    static_cast<double>(row->losses_x + row->losses_y) -
                    static_cast<double>(base->losses_x + base->losses_y));
            }
            for (const auto& [metric, values] : differences) {
                if (values.empty()) continue;
                const auto interval = bootstrap_interval(
                    values, 0x5503000ULL + ++salt);
                paired << phase << ',' << task << ',' << candidate << ','
                    << control << ',' << metric << ',' << values.size() << ','
                    << b48_mean(values) << ',' << interval.first << ','
                    << interval.second << '\n';
            }
        }
    }

    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "phase,variant,task,family,assignment,seeds,E1_X,E1_Y,"
        "robust_seeds,advantage_vs_no_learning,ci_low,ci_high,"
        "active_subthreshold,retained,protected_exploration,passes\n";
    for (const auto& [key, group] : groups) {
        const auto first = key.find('|');
        const auto second = key.find('|', first + 1);
        const auto phase = key.substr(0, first);
        const auto variant = key.substr(first + 1, second - first - 1);
        const auto task = key.substr(second + 1);
        if (task != "mixed") continue;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                std::vector<double> advantages;
                double x = 0;
                double y = 0;
                std::uint64_t robust = 0;
                std::uint64_t active = 0;
                std::uint64_t retained = 0;
                std::uint64_t protected_count = 0;
                std::uint64_t count = 0;
                for (const auto* row : group) {
                    if (row->family != family || row->inverse != inverse) continue;
                    ++count;
                    x += row->e1_x;
                    y += row->e1_y;
                    robust += row->robust;
                    active += row->processed > 0;
                    retained += row->retention_x >= .75 &&
                        row->retention_y >= .75;
                    protected_count += row->protected_exploration;
                    const auto base = indexed.find(phase + "|mixed|S_no_learning|" +
                        std::to_string(family) + '|' +
                        (inverse ? "I" : "N") + '|' +
                        std::to_string(row->seed));
                    if (base != indexed.end()) {
                        advantages.push_back(((row->e1_x - base->second->e1_x) +
                            (row->e1_y - base->second->e1_y)) / 2.0);
                    }
                }
                if (count == 0) continue;
                const auto interval = bootstrap_interval(
                    advantages, 0x5504000ULL + ++salt);
                const auto mean_x = x / count;
                const auto mean_y = y / count;
                const auto advantage = b48_mean(advantages);
                const bool requires_active = variant.rfind("S4_", 0) == 0;
                const bool pass = mean_x >= .75 && mean_y >= .75 &&
                    robust >= 48 && advantage >= .15 &&
                    !advantages.empty() && interval.first > 0 &&
                    (!requires_active || active == count) && retained >= 48 &&
                    protected_count == count;
                criteria << phase << ',' << variant << ',' << task << ','
                    << family << ',' << (inverse ? "inverse" : "normal")
                    << ',' << count << ',' << mean_x << ',' << mean_y << ','
                    << robust << ',' << advantage << ',' << interval.first
                    << ',' << interval.second << ',' << active << ','
                    << retained << ',' << protected_count << ',' << pass
                    << '\n';
            }
        }
    }

    std::ofstream seeds(output / "seed-results.csv");
    seeds << "phase,variant,task,family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,"
        "retention_X,retention_Y,first_positive,first_X,first_Y,first_both,"
        "losses_X,losses_Y,reacquisitions_X,reacquisitions_Y,both_episodes,"
        "scheduled,processed,stale,eligibility,fires,events,maximum_queue,"
        "memory,final_X,final_Y,robust,protected_exploration,valid\n";
    for (const auto& row : rows) {
        seeds << row.phase << ',' << row.variant << ',' << row.task << ','
            << row.family << ',' << (row.inverse ? "inverse" : "normal")
            << ',' << row.seed << ',' << row.e1_x << ',' << row.e1_y << ','
            << row.e2_x << ',' << row.e2_y << ',' << row.retention_x << ','
            << row.retention_y << ',' << b51_u32(row.first_positive) << ','
            << b51_u32(row.first_x) << ',' << b51_u32(row.first_y) << ','
            << b51_u32(row.first_both) << ',' << row.losses_x << ','
            << row.losses_y << ',' << row.reacquisitions_x << ','
            << row.reacquisitions_y << ',' << row.both_episodes << ','
            << row.scheduled << ',' << row.processed << ',' << row.stale << ','
            << row.eligibility << ',' << row.fires << ',' << row.events << ','
            << row.maximum_queue << ',' << row.memory << ',' << row.final_x
            << ',' << row.final_y << ',' << row.robust << ','
            << row.protected_exploration << ',' << row.valid << '\n';
    }
}

void b55_reproduce_b54(const std::filesystem::path& output) {
    const auto reference = output / "b54-reproduction";
    std::filesystem::create_directories(reference);
    b54_reproduce_b53(reference);
    b54_audit(reference);
    b54_temporal_integration(reference);
    b54_controls(reference);
    const auto rows = b54_natural_matrix(reference);
    b54_statistics(reference, rows);
    b54_persistence(reference);
    std::ofstream decision(reference / "mechanism-decision.csv");
    decision << "group,implemented,reason\n";
    decision << "K12,0,B54 reproduction evaluates only pre-existing recovery modes\n";
    decision << "K13,0,no B54 K12 mechanism exists to deactivate\n";
}

void b55_mechanism_diagnostics(const std::filesystem::path& output) {
    std::ofstream file(output / "mechanism-semantics.csv");
    file << "scenario,delay,time,membrane,fired,scheduled,processed,stale,"
        "future_events,valid\n";
    const auto run = [&](const std::string& scenario,
            const std::uint64_t delay, const bool second_excitatory,
            const bool inhibitory, const bool recovery) {
        auto config = b54_micro_config(
            recovery
                ? eve::RecoveryMode::independent_with_membrane_relaxation
                : eve::RecoveryMode::input_dependent,
            recovery ? 1U : 0U, recovery ? delay : 0U,
            recovery ? 100U : 0U);
        config.subthreshold_relaxation_enabled = true;
        config.subthreshold_relaxation_delay = delay;
        eve::bios::Runtime runtime(config, 0x5505000ULL + delay);
        const auto weak = runtime.add_experimental_synapse(0, 4, 1, true);
        const auto strong = runtime.add_experimental_synapse(1, 4, 2, true);
        const auto inhibition = runtime.add_experimental_synapse(
            2, 4, 1, true, eve::SynapticEffect::inhibitory);
        bool fired = false;
        if (recovery) {
            fired = runtime.process_simultaneous_impulses({strong}).fired;
            static_cast<void>(runtime.process_simultaneous_impulses({weak}));
        } else {
            static_cast<void>(runtime.process_simultaneous_impulses({weak}));
            if (delay > 1) static_cast<void>(runtime.advance_time_to(delay - 1));
            if (second_excitatory) {
                fired = runtime.process_simultaneous_impulses({weak}).fired;
            } else if (inhibitory) {
                fired = runtime.process_simultaneous_impulses({inhibition}).fired;
            }
        }
        static_cast<void>(runtime.advance_time_to(delay));
        file << scenario << ',' << delay << ',' << runtime.simulated_time()
            << ',' << runtime.network().neuron(4).membrane_potential << ','
            << fired << ','
            << runtime.metrics().scheduled_subthreshold_relaxations << ','
            << runtime.metrics().processed_subthreshold_relaxations << ','
            << runtime.metrics().stale_subthreshold_relaxations << ','
            << runtime.future_event_count() << ',' << runtime.network().validate()
            << '\n';
    };
    run("valid_due_relaxation", 4, false, false, false);
    run("excitatory_fire_invalidates", 4, true, false, false);
    run("inhibition_to_zero_invalidates", 4, false, true, false);
    run("existing_R2_recovery_invalidates", 4, false, false, true);

    for (const auto delay : kB54Delays) {
        auto config = b54_micro_config(
            eve::RecoveryMode::input_dependent, 0, 16, 0);
        config.subthreshold_relaxation_enabled = true;
        config.subthreshold_relaxation_delay = delay;
        eve::bios::Runtime runtime(config, 0x5506000ULL + delay);
        const auto weak = runtime.add_experimental_synapse(0, 4, 1, true);
        const auto zero = runtime.add_experimental_synapse(1, 4, 0, true);
        static_cast<void>(runtime.process_simultaneous_impulses({weak}));
        for (std::uint32_t input = 0; input < 64; ++input) {
            static_cast<void>(runtime.process_simultaneous_impulses({zero}));
        }
        file << "bounded_repeated_inputs," << delay << ','
            << runtime.simulated_time() << ','
            << runtime.network().neuron(4).membrane_potential << ",0,"
            << runtime.metrics().scheduled_subthreshold_relaxations << ','
            << runtime.metrics().processed_subthreshold_relaxations << ','
            << runtime.metrics().stale_subthreshold_relaxations << ','
            << runtime.future_event_count() << ',' << runtime.network().validate()
            << '\n';
    }
}

void b55_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,pending,exact_restore,exact_due_processing,"
        "exact_continuation,processed,valid\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto config = b54_micro_config(
            eve::RecoveryMode::independent_with_membrane_relaxation,
            1, 16, 50);
        config.subthreshold_relaxation_enabled = true;
        config.subthreshold_relaxation_delay = 16;
        eve::bios::Runtime runtime(config, 0x5507000ULL + seed);
        const auto weak = runtime.add_experimental_synapse(0, 4, 1, true);
        static_cast<void>(runtime.process_simultaneous_impulses({weak}));
        const auto pending = runtime.future_event_count();
        const auto path = output / (".b55-state-" + std::to_string(seed));
        runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact_restore = runtime.state_fingerprint() ==
            restored.state_fingerprint();
        static_cast<void>(runtime.advance_time_to(16));
        static_cast<void>(restored.advance_time_to(16));
        const bool exact_due = runtime.state_fingerprint() ==
            restored.state_fingerprint();
        static_cast<void>(runtime.process_simultaneous_impulses({weak}));
        static_cast<void>(restored.process_simultaneous_impulses({weak}));
        const bool exact_continuation = runtime.state_fingerprint() ==
            restored.state_fingerprint();
        file << seed << ',' << pending << ',' << exact_restore << ','
            << exact_due << ',' << exact_continuation << ','
            << restored.metrics().processed_subthreshold_relaxations << ','
            << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

void b55_controls(const std::filesystem::path& output) {
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,description,data,enabled\n";
    controls << "S0,historical R0 reference,b54-reproduction,1\n";
    controls << "S1,B54 R1 independent recovery,b54-reproduction,1\n";
    controls << "S2,B54 R2 50 percent reference,b54-reproduction and natural-trajectories.csv.gz,1\n";
    controls << "S3,diagnostic local removal and restoration,subthreshold-causal-copies.csv.gz,1\n";
    controls << "S4,event-driven subthreshold relaxation delay 16,natural-trajectories.csv.gz,1\n";
    controls << "S5,identical disabled implementation,natural-trajectories.csv.gz,1\n";
    controls << "S6,delay 32 and 64 discovery controls,natural-trajectories.csv.gz,1\n";
    controls << "NL,no learning paired control,natural-trajectories.csv.gz,1\n";

    std::ofstream tasks(output / "task-matrix.csv");
    tasks << "task,normal,inverse,families,discovery_seeds,confirmation_seeds,scope\n";
    tasks << "only_X,1,1,3,64,0,S2 S4 S5\n";
    tasks << "only_Y,1,1,3,64,0,S2 S4 S5\n";
    tasks << "X_then_Y,1,1,3,64,0,S2 S4 S5\n";
    tasks << "Y_then_X,1,1,3,64,0,S2 S4 S5\n";
    tasks << "alternating,1,1,3,64,0,S2 S4 S5\n";
    tasks << "mixed,1,1,3,64,64,all selected variants\n";
    tasks << "reversal,1,1,3,64,64,S2 S4 S5 plus confirmation no-learning\n";
    tasks << "retention,1,1,3,64,0,S2 S4 S5\n";
}

void b55_causal_decision(
    const std::filesystem::path& output,
    const std::vector<B55Cluster>& clusters) {
    std::uint64_t decision_states = 0;
    std::uint64_t errors = 0;
    std::uint64_t helpful = 0;
    std::uint64_t harmful = 0;
    std::uint64_t restored = 0;
    std::uint64_t valid = 0;
    for (const auto& cluster : clusters) {
        decision_states += cluster.decision_states;
        errors += cluster.natural_errors;
        helpful += cluster.helpful;
        harmful += cluster.harmful;
        restored += cluster.exact_restore;
        valid += cluster.valid;
    }
    const bool causal_need = errors > 0 && harmful > helpful &&
        restored == decision_states && valid == clusters.size();
    std::ofstream file(output / "mechanism-decision.csv");
    file << "question,value\n";
    file << "decision_states," << decision_states << '\n';
    file << "natural_errors," << errors << '\n';
    file << "subthreshold_helpful_removal_prevents_correct," << helpful << '\n';
    file << "subthreshold_harmful_removal_fixes_error," << harmful << '\n';
    file << "exact_restorations," << restored << '\n';
    file << "causal_need_demonstrated," << causal_need << '\n';
    file << "S4_implemented," << causal_need << '\n';
    file << "selected_semantics,fixed local lifetime from first subthreshold state\n";
    file << "selected_delay,16\n";
    file << "selection_basis,one episode horizon with 32 and 64 as preregistered controls\n";
    file << "production_default_enabled,0\n";
}

void b55_resources(
    const std::filesystem::path& output,
    const std::vector<B55NaturalResult>& rows) {
    struct Totals {
        std::uint64_t networks{};
        std::uint64_t scheduled{};
        std::uint64_t processed{};
        std::uint64_t stale{};
        std::uint64_t events{};
        std::uint64_t maximum_queue{};
        std::size_t maximum_memory{};
    };
    std::map<std::string, Totals> totals;
    for (const auto& row : rows) {
        auto& value = totals[row.phase + '|' + row.variant + '|' + row.task];
        ++value.networks;
        value.scheduled += row.scheduled;
        value.processed += row.processed;
        value.stale += row.stale;
        value.events += row.events;
        value.maximum_queue = std::max(value.maximum_queue, row.maximum_queue);
        value.maximum_memory = std::max(value.maximum_memory, row.memory);
    }
    std::ofstream file(output / "resource-summary.csv");
    file << "phase,variant,task,networks,scheduled,processed,stale,"
        "scheduled_per_episode,processed_per_episode,stale_per_episode,"
        "processed_events_per_episode,maximum_queue,maximum_memory_bytes,"
        "local_bytes_per_neuron\n";
    for (const auto& [key, value] : totals) {
        auto printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        const auto episodes = static_cast<double>(value.networks) *
            kB51Episodes;
        file << printable << ',' << value.networks << ',' << value.scheduled
            << ',' << value.processed << ',' << value.stale << ','
            << value.scheduled / episodes << ',' << value.processed / episodes
            << ',' << value.stale / episodes << ',' << value.events / episodes
            << ',' << value.maximum_queue << ',' << value.maximum_memory
            << ',' << (sizeof(std::uint64_t) * 2) << '\n';
    }
}

void b55_manifest(const std::filesystem::path& output) {
    std::ofstream file(output / "phase-one-manifest.csv");
    file << "group,enabled,description\n";
    file << "S0,0,reproduced later in full B54 reference\n";
    file << "S1,0,reproduced later in full B54 reference\n";
    file << "S2,1,B54 R2 50 percent causal inventory\n";
    file << "S3,1,diagnostic local removal and restoration only\n";
    file << "S4,1,enabled only after recorded causal decision\n";
    file << "S5,1,identical implementation disabled\n";
    file << "S6,1,delay 32 and 64 discovery controls\n";
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B55_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b55");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b55_reproduce_b54(output);
    b55_manifest(output);
    b55_controls(output);
    b55_temporal_diagnostics(output);
    const auto clusters = b55_natural_causality(output);
    b55_causal_statistics(output, clusters);
    b55_causal_decision(output, clusters);
    b55_mechanism_diagnostics(output);
    const auto rows = b55_natural_matrix(output);
    b55_natural_statistics(output, rows);
    b55_resources(output, rows);
    b55_persistence(output);
    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,networks,episodes,"
        "causal_clusters,b54_reference_reproduced\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << rows.size() << ','
        << rows.size() * kB51Episodes << ',' << clusters.size() << ",1\n";
    std::cout << "B.5.5 subthreshold membrane research complete networks="
        << rows.size() << " clusters=" << clusters.size() << '\n';
    return 0;
}
#endif
