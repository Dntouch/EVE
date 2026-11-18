#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B50_EMBEDDED
#include "research_stage_b50.cpp"
#undef EVE_RESEARCH_STAGE_B50_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <array>
#include <limits>
#include <optional>

namespace {

constexpr std::uint32_t kB51Episodes = 512;
constexpr std::uint32_t kB51EvaluationEpisodes = 64;
constexpr std::uint32_t kB51Never = std::numeric_limits<std::uint32_t>::max();

struct B51Probe {
    bool motor_a{};
    bool motor_b{};
    std::uint64_t decision_a{};
    std::uint64_t decision_b{};
    std::array<std::int64_t, 2> decision_peak{{0, 0}};
    std::array<std::int64_t, 2> motor_peak{{0, 0}};
    std::uint64_t inhibitory_impulses{};
};

struct B51Episode {
    explicit B51Episode(const eve::bios::Runtime& runtime)
        : before(runtime), after_activity(runtime), after_feedback(runtime) {}

    eve::bios::Runtime before;
    eve::bios::Runtime after_activity;
    eve::bios::Runtime after_feedback;
    std::vector<eve::neural::Synapse> before_weights;
    std::vector<eve::neural::Synapse> after_weights;
    std::vector<eve::bios::ImpulseObservation> impulses;
    std::uint8_t cue{};
    std::uint8_t planned_action{};
    std::uint8_t executed_action{2};
    bool target_a{};
    bool autonomous_a{};
    bool autonomous_b{};
    bool final_a{};
    bool final_b{};
    bool explored{};
    bool correct{};
    std::int8_t modulation{};
    std::array<std::uint64_t, 2> decision_fires{{0, 0}};
    std::array<std::uint64_t, 2> motor_fires{{0, 0}};
    std::array<std::int64_t, 2> decision_peak{{0, 0}};
    std::array<std::int64_t, 2> motor_peak{{0, 0}};
    eve::bios::ModulatoryFeedbackResult feedback;
};

struct B51PathLife {
    std::uint32_t first_success{kB51Never};
    std::uint32_t second_success{kB51Never};
    std::uint32_t first_increment{kB51Never};
    std::uint32_t second_increment{kB51Never};
    std::uint32_t first_acquisition{kB51Never};
    std::uint32_t first_loss{kB51Never};
    std::uint32_t first_reacquisition{kB51Never};
    std::uint32_t successful_experiences{};
    std::uint32_t positive_increments{};
    std::uint32_t negative_between_increments{};
    std::uint32_t competing_positive_between_increments{};
    std::uint32_t losses{};
    std::uint32_t reacquisitions{};
    std::uint32_t current_duration{};
    std::uint32_t longest_duration{};
    bool ever_acquired{};
    bool currently_correct{};
};

struct B51Evaluation {
    std::array<B47Counts, 2> e1{};
    std::array<B47Counts, 2> e2{};
    bool identical_start{};
};

struct B51ClusterResult {
    std::string phase;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    std::uint32_t checkpoint{};
    double e1_x{};
    double e1_y{};
    double e2_x{};
    double e2_y{};
    bool robust{};
    bool valid{};
};

bool b51_correct(
    const bool target_a, const bool motor_a, const bool motor_b) {
    return target_a ? motor_a && !motor_b : motor_b && !motor_a;
}

std::uint32_t b51_u32(const std::uint32_t value) {
    return value == kB51Never ? std::numeric_limits<std::uint32_t>::max() : value;
}

B51Probe b51_probe_in_place(
    eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const std::uint8_t cue) {
    B51Probe result;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        result.motor_a = result.motor_a || event.neuron == circuit.motors[0];
        result.motor_b = result.motor_b || event.neuron == circuit.motors[1];
        result.decision_a += event.neuron == circuit.decisions[0];
        result.decision_b += event.neuron == circuit.decisions[1];
    });
    runtime.set_impulse_observer(
        [&](const eve::bios::ImpulseObservation& event) {
            for (std::size_t index = 0; index < 2; ++index) {
                if (event.receiver == circuit.decisions[index]) {
                    result.decision_peak[index] = std::max(
                        result.decision_peak[index], event.membrane_after);
                }
                if (event.receiver == circuit.motors[index]) {
                    result.motor_peak[index] = std::max(
                        result.motor_peak[index], event.membrane_after);
                }
            }
            result.inhibitory_impulses +=
                event.synaptic_effect == eve::SynapticEffect::inhibitory;
        });
    runtime.enqueue_fire(circuit.sensors[cue]);
    static_cast<void>(runtime.run());
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    return result;
}

B51Probe b51_probe(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const std::uint8_t cue) {
    return b51_probe_in_place(runtime, circuit, cue);
}

B51Episode b51_episode(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const std::uint8_t cue, const std::uint8_t planned_action,
    const bool target_a, const std::uint32_t episode,
    const bool plasticity = true,
    const std::optional<std::int8_t> modulation_override = std::nullopt,
    const bool force_exploration = false) {
    const auto start = static_cast<std::uint64_t>(episode) * 16ULL;
    if (runtime.simulated_time() < start) {
        static_cast<void>(runtime.advance_time_to(start));
    }
    B51Episode result(runtime);
    result.cue = cue;
    result.planned_action = planned_action;
    result.target_a = target_a;
    result.before_weights = runtime.network().synapses();
    std::uint8_t phase = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        if (phase > 1) return;
        for (std::size_t index = 0; index < 2; ++index) {
            if (event.neuron == circuit.decisions[index]) {
                ++result.decision_fires[index];
            }
            if (event.neuron == circuit.motors[index]) {
                ++result.motor_fires[index];
                if (index == 0) result.final_a = true;
                else result.final_b = true;
                if (phase == 0 && index == 0) result.autonomous_a = true;
                if (phase == 0 && index == 1) result.autonomous_b = true;
            }
        }
    });
    runtime.set_impulse_observer(
        [&](const eve::bios::ImpulseObservation& event) {
            if (phase > 1) return;
            result.impulses.push_back(event);
            for (std::size_t index = 0; index < 2; ++index) {
                if (event.receiver == circuit.decisions[index]) {
                    result.decision_peak[index] = std::max(
                        result.decision_peak[index], event.membrane_after);
                }
                if (event.receiver == circuit.motors[index]) {
                    result.motor_peak[index] = std::max(
                        result.motor_peak[index], event.membrane_after);
                }
            }
        });
    runtime.enqueue_fire(circuit.sensors[cue]);
    static_cast<void>(runtime.run());
    result.explored = force_exploration ||
        (!result.autonomous_a && !result.autonomous_b);
    if (result.explored) {
        phase = 1;
        result.executed_action = planned_action;
        runtime.enqueue_fire(circuit.explorers[planned_action]);
        static_cast<void>(runtime.run());
    }
    result.correct = b51_correct(target_a, result.final_a, result.final_b);
    result.modulation = modulation_override.value_or(
        static_cast<std::int8_t>(result.correct ? 1 : -1));
    result.after_activity = runtime;
    phase = 2;
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    if (plasticity) {
        result.feedback = runtime.process_modulatory_feedback(result.modulation);
    }
    result.after_weights = runtime.network().synapses();
    if (runtime.simulated_time() < start + 16ULL) {
        static_cast<void>(runtime.advance_time_to(start + 16ULL));
    }
    result.after_feedback = std::move(runtime);
    return result;
}

B48State b51_initial_state(
    const B48Variant& variant, const std::uint64_t seed,
    const std::uint64_t cue_revision = 0,
    const std::uint64_t exploration_revision = 0) {
    auto state = b48_initial_state(variant, seed);
    state.plans = b48_plans(seed, kB51Episodes + kB51EvaluationEpisodes,
        variant.family, cue_revision, exploration_revision);
    return state;
}

B51Evaluation b51_evaluate(
    const eve::bios::Runtime& trained, const B47Circuit& circuit,
    const B48Variant& variant, const std::vector<B48Plan>& plans,
    const std::uint32_t checkpoint) {
    eve::bios::Runtime e1 = trained;
    eve::bios::Runtime e2 = trained;
    B51Evaluation result;
    result.identical_start = e1.state_fingerprint() == e2.state_fingerprint() &&
        e1.state_fingerprint() == trained.state_fingerprint();
    for (std::uint32_t offset = 0; offset < kB51EvaluationEpisodes; ++offset) {
        const auto episode = checkpoint + offset;
        const auto cue = b48_cue(
            variant, plans[episode], episode, b46_training(variant.task));
        const bool target_a = b48_target_a(
            variant, cue, episode, b46_training(variant.task));
        const auto e1_probe = b51_probe_in_place(e1, circuit, cue);
        b47_record(result.e1[cue], target_a,
            e1_probe.motor_a, e1_probe.motor_b);
        auto e2_probe = b51_probe_in_place(e2, circuit, cue);
        bool final_a = e2_probe.motor_a;
        bool final_b = e2_probe.motor_b;
        if (!final_a && !final_b) {
            e2.set_fire_observer([&](const eve::bios::FireObservation& event) {
                final_a = final_a || event.neuron == circuit.motors[0];
                final_b = final_b || event.neuron == circuit.motors[1];
            });
            e2.enqueue_fire(circuit.explorers[plans[episode].exploration]);
            static_cast<void>(e2.run());
            e2.set_fire_observer({});
        }
        b47_record(result.e2[cue], target_a, final_a, final_b);
        static_cast<void>(e1.advance_time_by(16));
        static_cast<void>(e2.advance_time_by(16));
    }
    return result;
}

std::array<bool, 2> b51_recall_pair(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const B48Variant& variant, const std::uint32_t episode) {
    std::array<bool, 2> correct{};
    for (std::uint8_t cue = 0; cue < 2; ++cue) {
        const auto probe = b51_probe(runtime, circuit, cue);
        const bool target_a = b48_target_a(
            variant, cue, episode, b46_training(variant.task));
        correct[cue] = b51_correct(target_a, probe.motor_a, probe.motor_b);
    }
    return correct;
}

void b51_write_b50_reproduction(const std::filesystem::path& output) {
    const auto reference = output / "b50-reproduction";
    std::filesystem::create_directories(reference);
    b50_write_reference_matrix(reference);
    b50_credit_diagnostics(reference);
    b50_interference_diagnostics(reference);
    b50_statistics(reference);
    b50_persistence(reference);
    std::filesystem::copy_file(reference / "task-matrix.csv",
        output / "task-matrix.csv",
        std::filesystem::copy_options::overwrite_existing);
    std::ofstream decision(reference / "mechanism-decision.csv");
    decision << "group,implemented,reason\n";
    decision << "K9,0,the target cue receives local eligibility; a second positive increment from weight one to threshold two is necessary and sufficient in 364 paired copies; no missing candidate-assignment mechanism is isolated\n";
    decision << "K10,0,no K9 mechanism exists to deactivate\n";
}

void b51_validated_repetition(const std::filesystem::path& output) {
    std::ofstream file(output / "validated-repetitions.csv");
    file << "event_id,family,assignment,seed,source_episode,repetition,cue,target,autonomous,explored,exploration_action,final,correct,applied_modulation,target_weight_before,target_weight_after,autonomous_recall,recall_correct\n";
    std::ofstream comparison(output / "validated-repetition-comparison.csv");
    std::array<std::uint64_t, 4> correct_at{};
    std::uint64_t events = 0;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            B48Variant variant{"B51_validated_repetition", B46Task::mixed,
                B48Policy::reference, inverse, true, false, family};
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                auto state = b51_initial_state(variant, seed);
                bool selected = false;
                for (std::uint32_t episode = 0; episode < 128; ++episode) {
                    const auto cue = b48_cue(
                        variant, state.plans[episode], episode, 128);
                    const bool target_a = b48_target_a(
                        variant, cue, episode, 128);
                    const auto before = state.runtime;
                    auto natural = b51_episode(state.runtime, state.circuit,
                        cue, state.plans[episode].exploration, target_a, episode);
                    state.runtime = natural.after_feedback;
                    if (selected || natural.autonomous_a || natural.autonomous_b ||
                        !natural.explored || !natural.correct) continue;
                    selected = true;
                    ++events;
                    auto repeated = before;
                    const auto target_id = state.circuit.cue_synapses[
                        cue * 2 + (target_a ? 0U : 1U)];
                    for (std::uint32_t repetition = 1; repetition <= 4;
                         ++repetition) {
                        const auto weight_before =
                            repeated.network().synapse(target_id).weight;
                        auto observed = b51_episode(repeated, state.circuit,
                            cue, natural.executed_action, target_a,
                            episode + repetition - 1);
                        repeated = observed.after_feedback;
                        const auto weight_after =
                            repeated.network().synapse(target_id).weight;
                        const auto recall = b51_probe(
                            repeated, state.circuit, cue);
                        const bool recall_correct = b51_correct(
                            target_a, recall.motor_a, recall.motor_b);
                        correct_at[repetition - 1] += recall_correct;
                        file << events << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << episode << ',' << repetition << ','
                            << static_cast<unsigned>(cue) << ','
                            << (target_a ? 'A' : 'B') << ','
                            << b47_outcome(observed.autonomous_a,
                                           observed.autonomous_b) << ','
                            << observed.explored << ','
                            << static_cast<unsigned>(observed.executed_action)
                            << ',' << b47_outcome(observed.final_a,
                                                 observed.final_b)
                            << ',' << observed.correct << ','
                            << static_cast<int>(observed.modulation) << ','
                            << weight_before << ',' << weight_after << ','
                            << b47_outcome(recall.motor_a, recall.motor_b) << ','
                            << recall_correct << '\n';
                    }
                    break;
                }
            }
        }
    }
    comparison << "repetition,B50_historical_correct,B51_validated_correct,total,changed\n";
    const std::array<std::uint64_t, 4> historical{{0, 364, 384, 384}};
    for (std::size_t index = 0; index < 4; ++index) {
        comparison << index + 1 << ',' << historical[index] << ','
            << correct_at[index] << ',' << events << ','
            << (historical[index] != correct_at[index]) << '\n';
    }
}

std::string b51_loss_cause(
    const B51Episode& row, const B47Circuit& circuit,
    const std::uint8_t lost_cue, const bool lost_target_a) {
    bool target_decrease = false;
    bool sensor_change = false;
    bool inhibition_change = false;
    const auto target_id = circuit.cue_synapses[
        lost_cue * 2 + (lost_target_a ? 0U : 1U)];
    for (eve::SynapseId id = 0; id < row.after_weights.size(); ++id) {
        const auto before = row.before_weights[id].weight;
        const auto after = row.after_weights[id].weight;
        if (before == after) continue;
        if (id == target_id && after < before) target_decrease = true;
        sensor_change = sensor_change || std::find(
            circuit.cue_synapses.begin(), circuit.cue_synapses.end(), id) !=
            circuit.cue_synapses.end();
        inhibition_change = inhibition_change || std::find(
            circuit.inhibitory_outputs.begin(),
            circuit.inhibitory_outputs.end(), id) !=
            circuit.inhibitory_outputs.end();
    }
    if (target_decrease) return "negative_target_weakening";
    if (row.correct && row.cue != lost_cue && inhibition_change) {
        return "competing_positive_with_inhibition";
    }
    if (row.correct && row.cue != lost_cue && sensor_change) {
        return "competing_positive_sensor";
    }
    if (inhibition_change) return "inhibitory_change";
    if (sensor_change) return "other_sensor_change";
    return "no_weight_change";
}

eve::bios::Runtime b51_continue(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const B48Variant& variant, const std::vector<B48Plan>& plans,
    const std::uint32_t begin, const std::uint32_t count,
    const bool plasticity = true,
    const std::optional<eve::SynapseId> preserve = std::nullopt,
    const std::optional<std::int32_t> preserve_weight = std::nullopt) {
    const auto end = std::min<std::uint32_t>(kB51Episodes, begin + count);
    for (std::uint32_t episode = begin; episode < end; ++episode) {
        const auto cue = b48_cue(
            variant, plans[episode], episode, b46_training(variant.task));
        const bool target_a = b48_target_a(
            variant, cue, episode, b46_training(variant.task));
        auto row = b51_episode(runtime, circuit, cue,
            plans[episode].exploration, target_a, episode, plasticity);
        runtime = std::move(row.after_feedback);
        if (preserve && preserve_weight) {
            runtime.set_experimental_synapse_weight(*preserve, *preserve_weight);
        }
    }
    return runtime;
}

void b51_emit_controlled_continuations(
    std::ofstream& file, const std::string& cluster,
    const eve::bios::Runtime& acquired, const B47Circuit& circuit,
    const B48Variant& variant, const std::vector<B48Plan>& original_plans,
    const std::uint64_t seed, const std::uint8_t learned_cue,
    const std::uint32_t acquired_episode) {
    const auto target_a = b48_target_a(
        variant, learned_cue, acquired_episode, b46_training(variant.task));
    const auto target_id = circuit.cue_synapses[
        learned_cue * 2 + (target_a ? 0U : 1U)];
    const auto target_weight = acquired.network().synapse(target_id).weight;
    const auto alternative_exploration = b48_plans(seed,
        kB51Episodes + kB51EvaluationEpisodes, variant.family, 0, 1);
    const auto alternative_cues = b48_plans(seed,
        kB51Episodes + kB51EvaluationEpisodes, variant.family, 1, 0);
    std::vector<B48Plan> same_cues_other_exploration = original_plans;
    std::vector<B48Plan> other_cues_same_exploration = original_plans;
    std::vector<B48Plan> dense = original_plans;
    std::vector<B48Plan> every_two = original_plans;
    std::vector<B48Plan> every_eight = original_plans;
    for (std::uint32_t episode = acquired_episode + 1;
         episode < std::min<std::uint32_t>(kB51Episodes,
             acquired_episode + 65); ++episode) {
        same_cues_other_exploration[episode].exploration =
            alternative_exploration[episode].exploration;
        other_cues_same_exploration[episode].cue = alternative_cues[episode].cue;
        dense[episode].cue = learned_cue;
        if ((episode - acquired_episode) % 2 == 0) {
            every_two[episode].cue = learned_cue;
        }
        if ((episode - acquired_episode) % 8 == 0) {
            every_eight[episode].cue = learned_cue;
        }
    }
    const auto emit = [&](const char* name, const std::vector<B48Plan>& plans,
                          const bool plasticity,
                          const bool preserve_weight) {
        auto runtime = b51_continue(acquired, circuit, variant, plans,
            acquired_episode + 1, 64, plasticity,
            preserve_weight ? std::optional<eve::SynapseId>(target_id)
                            : std::nullopt,
            preserve_weight ? std::optional<std::int32_t>(target_weight)
                            : std::nullopt);
        const auto correct = b51_recall_pair(
            runtime, circuit, variant, acquired_episode + 64);
        file << cluster << ',' << static_cast<unsigned>(learned_cue) << ','
            << acquired_episode << ',' << name << ",64,"
            << correct[learned_cue] << ',' << correct[1U - learned_cue]
            << ',' << (correct[0] && correct[1]) << ','
            << runtime.network().synapse(target_id).weight << '\n';
    };
    emit("G1_natural", original_plans, true, false);
    emit("G2_other_exploration", same_cues_other_exploration, true, false);
    emit("G3_other_cues", other_cues_same_exploration, true, false);
    emit("G4_dense_learned_cue", dense, true, false);
    emit("G4_every_2_learned_cue", every_two, true, false);
    emit("G4_every_8_learned_cue", every_eight, true, false);
    emit("G5_no_plasticity", original_plans, false, false);
    emit("G6_preserve_target_weight", original_plans, true, true);
}

void b51_run_natural(
    const std::filesystem::path& output,
    std::vector<B51ClusterResult>& cluster_results) {
    std::ofstream trajectory(output / "natural-trajectories.csv");
    trajectory << "phase,family,assignment,seed,episode,cue,target,autonomous,explored,exploration_action,final,correct,modulation,eligibility_impulses,changed_synapses,cue_X_A,cue_X_B,cue_Y_A,cue_Y_B,inhibition_A,inhibition_B,decision_A_fires,decision_B_fires,motor_A_fires,motor_B_fires,decision_A_peak,decision_B_peak,motor_A_peak,motor_B_peak\n";
    std::ofstream eligibility(output / "eligibility-events.csv");
    eligibility << "phase,family,assignment,seed,episode,cue,impulse,synapse,receiver,generation,candidate,role,weight,membrane_before,membrane_after\n";
    std::ofstream changes(output / "natural-weight-changes.csv");
    changes << "phase,family,assignment,seed,episode,cue,target,modulation,synapse,sender,receiver,effect,role,before,after,delta\n";
    std::ofstream transitions(output / "weight-transitions.csv");
    transitions << "phase,family,assignment,seed,episode,path_cue,path_target,synapse,role,modulation,before,after,activity_cue,activity_final,activity_correct,pre_outcome,post_outcome,pre_correct,post_correct,decision_A_pre_peak,decision_B_pre_peak,decision_A_post_peak,decision_B_post_peak,inhibition_A,inhibition_B\n";
    std::ofstream causal(output / "threshold-causal-copies.csv");
    causal << "phase,family,assignment,seed,episode,path_cue,synapse,role,before,after,post_outcome,post_correct,rollback_outcome,rollback_correct,rollback_necessary,isolated_outcome,isolated_correct,isolated_sufficient\n";
    std::ofstream ability(output / "ability-events.csv");
    ability << "phase,family,assignment,seed,episode,cue,event,cause,target_weight,competitor_weight,inhibition_A,inhibition_B,other_correct\n";
    std::ofstream stability(output / "stability-checkpoints.csv");
    stability << "phase,family,assignment,seed,cue,acquired_episode,offset,episode,correct,other_correct,target_weight,losses_since_acquisition\n";
    std::ofstream losses(output / "loss-causal-copies.csv");
    losses << "phase,family,assignment,seed,episode,lost_cue,cause,intervention,lost_correct,other_correct,target_weight,inhibition_A,inhibition_B\n";
    std::ofstream loss_future(output / "loss-continuations.csv");
    loss_future << "phase,family,assignment,seed,episode,lost_cue,cause,intervention,future_episodes,lost_correct,other_correct,both_correct\n";
    std::ofstream controlled(output / "controlled-continuations.csv");
    controlled << "cluster,cue,acquired_episode,variant,future_episodes,learned_correct,other_correct,both_correct,target_weight\n";
    std::ofstream checkpoints(output / "checkpoint-evaluation.csv");
    checkpoints << "phase,family,assignment,seed,checkpoint,E1_X,E1_Y,E2_X,E2_Y,E1_robust,E2_robust,identical_start,valid\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "phase,family,assignment,seed,checkpoint,metric,cue,correct,wrong,both,none\n";
    std::ofstream lifecycle(output / "path-lifecycle.csv");
    lifecycle << "phase,family,assignment,seed,cue,target,episodes,first_success,second_success,first_increment,second_increment,successful_experiences,positive_increments,negative_between_increments,competing_positive_between_increments,first_acquisition,first_loss,first_reacquisition,losses,reacquisitions,longest_duration,final_correct,final_target_weight\n";
    std::ofstream seed_summary(output / "seed-summary.csv");
    seed_summary << "phase,family,assignment,seed,episodes,first_both,both_total,both_longest,X_acquired,Y_acquired,X_losses,Y_losses,X_reacquisitions,Y_reacquisitions,final_X,final_Y,valid,technical_abort\n";

    for (const std::string phase : {"discovery", "confirmation"}) {
        const std::uint64_t first_seed = phase == "discovery" ? 1 : 10001;
        const std::uint64_t last_seed = first_seed + 63;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                B48Variant variant{"B51_natural", B46Task::mixed,
                    B48Policy::reference, inverse, true, false, family};
                for (std::uint64_t seed = first_seed; seed <= last_seed; ++seed) {
                    auto state = b51_initial_state(variant, seed);
                    std::array<B51PathLife, 2> life{};
                    const auto initial_correct = b51_recall_pair(
                        state.runtime, state.circuit, variant, 0);
                    life[0].currently_correct = initial_correct[0];
                    life[1].currently_correct = initial_correct[1];
                    std::uint32_t first_both = kB51Never;
                    std::uint32_t both_total = 0;
                    std::uint32_t both_current = 0;
                    std::uint32_t both_longest = 0;
                    std::array<bool, 2> continuation_emitted{{false, false}};
                    const auto cluster = phase + "_f" + std::to_string(family) +
                        "_" + (inverse ? "inverse" : "normal") + "_s" +
                        std::to_string(seed);

                    for (std::uint32_t episode = 0;
                         episode < kB51Episodes; ++episode) {
                        const auto cue = b48_cue(variant, state.plans[episode],
                            episode, b46_training(variant.task));
                        const bool target_a = b48_target_a(variant, cue,
                            episode, b46_training(variant.task));
                        auto row = b51_episode(state.runtime, state.circuit,
                            cue, state.plans[episode].exploration,
                            target_a, episode);
                        state.runtime = row.after_feedback;
                        std::uint32_t changed_count = 0;
                        for (eve::SynapseId id = 0;
                             id < row.after_weights.size(); ++id) {
                            if (row.before_weights[id].weight !=
                                row.after_weights[id].weight) ++changed_count;
                        }
                        trajectory << phase << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << episode << ','
                            << static_cast<unsigned>(cue) << ','
                            << (target_a ? 'A' : 'B') << ','
                            << b47_outcome(row.autonomous_a, row.autonomous_b)
                            << ',' << row.explored << ','
                            << static_cast<unsigned>(row.executed_action) << ','
                            << b47_outcome(row.final_a, row.final_b) << ','
                            << row.correct << ',' << static_cast<int>(row.modulation)
                            << ',' << row.impulses.size() << ',' << changed_count;
                        for (const auto id : state.circuit.cue_synapses) {
                            trajectory << ',' << state.runtime.network().synapse(id).weight;
                        }
                        for (const auto id : state.circuit.inhibitory_outputs) {
                            trajectory << ',' << state.runtime.network().synapse(id).weight;
                        }
                        trajectory << ',' << row.decision_fires[0] << ','
                            << row.decision_fires[1] << ',' << row.motor_fires[0]
                            << ',' << row.motor_fires[1] << ','
                            << row.decision_peak[0] << ',' << row.decision_peak[1]
                            << ',' << row.motor_peak[0] << ','
                            << row.motor_peak[1] << '\n';

                        for (std::size_t impulse_index = 0;
                             impulse_index < row.impulses.size(); ++impulse_index) {
                            const auto& impulse = row.impulses[impulse_index];
                            for (const auto candidate : impulse.eligibility_candidates) {
                                eligibility << phase << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << static_cast<unsigned>(cue) << ','
                                    << impulse_index << ',' << impulse.synapse << ','
                                    << impulse.receiver << ','
                                    << impulse.eligibility_generation << ','
                                    << candidate << ',' << b50_synapse_role(
                                        state.circuit, candidate, cue, target_a)
                                    << ',' << row.before_weights[candidate].weight
                                    << ',' << impulse.membrane_before << ','
                                    << impulse.membrane_after << '\n';
                            }
                        }

                        const auto after_correct = b51_recall_pair(
                            state.runtime, state.circuit, variant, episode);
                        for (eve::SynapseId id = 0;
                             id < row.after_weights.size(); ++id) {
                            const auto before_weight = row.before_weights[id].weight;
                            const auto after_weight = row.after_weights[id].weight;
                            if (before_weight == after_weight) continue;
                            changes << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ','
                                << static_cast<unsigned>(cue) << ','
                                << (target_a ? 'A' : 'B') << ','
                                << static_cast<int>(row.modulation) << ',' << id
                                << ',' << row.after_weights[id].sender << ','
                                << row.after_weights[id].receiver << ','
                                << (row.after_weights[id].effect ==
                                    eve::SynapticEffect::inhibitory ?
                                    "inhibitory" : "excitatory") << ','
                                << b50_synapse_role(state.circuit, id, cue, target_a)
                                << ',' << before_weight << ',' << after_weight
                                << ',' << (after_weight - before_weight) << '\n';

                            const auto found = std::find(
                                state.circuit.cue_synapses.begin(),
                                state.circuit.cue_synapses.end(), id);
                            if (found == state.circuit.cue_synapses.end()) continue;
                            const auto offset = static_cast<std::size_t>(
                                std::distance(state.circuit.cue_synapses.begin(), found));
                            const auto path_cue = static_cast<std::uint8_t>(offset / 2);
                            const auto path_action = offset % 2;
                            const bool path_target_a = b48_target_a(variant,
                                path_cue, episode, b46_training(variant.task));
                            const auto pre = b51_probe(
                                row.before, state.circuit, path_cue);
                            const auto post = b51_probe(
                                state.runtime, state.circuit, path_cue);
                            const bool pre_correct = b51_correct(path_target_a,
                                pre.motor_a, pre.motor_b);
                            const bool post_correct = b51_correct(path_target_a,
                                post.motor_a, post.motor_b);
                            transitions << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ','
                                << static_cast<unsigned>(path_cue) << ','
                                << (path_target_a ? 'A' : 'B') << ',' << id << ','
                                << (path_action == (path_target_a ? 0U : 1U) ?
                                    "target" : "competitor") << ','
                                << static_cast<int>(row.modulation) << ','
                                << before_weight << ',' << after_weight << ','
                                << static_cast<unsigned>(cue) << ','
                                << b47_outcome(row.final_a, row.final_b) << ','
                                << row.correct << ','
                                << b47_outcome(pre.motor_a, pre.motor_b) << ','
                                << b47_outcome(post.motor_a, post.motor_b) << ','
                                << pre_correct << ',' << post_correct << ','
                                << pre.decision_peak[0] << ','
                                << pre.decision_peak[1] << ','
                                << post.decision_peak[0] << ','
                                << post.decision_peak[1] << ','
                                << state.runtime.network().synapse(
                                    state.circuit.inhibitory_outputs[0]).weight
                                << ',' << state.runtime.network().synapse(
                                    state.circuit.inhibitory_outputs[1]).weight
                                << '\n';

                            if (!pre_correct && post_correct) {
                                auto rollback = state.runtime;
                                rollback.set_experimental_synapse_weight(
                                    id, before_weight);
                                const auto rollback_probe = b51_probe(
                                    rollback, state.circuit, path_cue);
                                auto isolated = state.runtime;
                                for (eve::SynapseId changed_id = 0;
                                     changed_id < row.after_weights.size();
                                     ++changed_id) {
                                    if (row.before_weights[changed_id].weight !=
                                        row.after_weights[changed_id].weight) {
                                        isolated.set_experimental_synapse_weight(
                                            changed_id,
                                            row.before_weights[changed_id].weight);
                                    }
                                }
                                isolated.set_experimental_synapse_weight(
                                    id, after_weight);
                                const auto isolated_probe = b51_probe(
                                    isolated, state.circuit, path_cue);
                                const bool rollback_correct = b51_correct(
                                    path_target_a, rollback_probe.motor_a,
                                    rollback_probe.motor_b);
                                const bool isolated_correct = b51_correct(
                                    path_target_a, isolated_probe.motor_a,
                                    isolated_probe.motor_b);
                                causal << phase << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << static_cast<unsigned>(path_cue) << ','
                                    << id << ','
                                    << (path_action == (path_target_a ? 0U : 1U) ?
                                        "target" : "competitor") << ','
                                    << before_weight << ','
                                    << after_weight << ','
                                    << b47_outcome(post.motor_a, post.motor_b)
                                    << ',' << post_correct << ','
                                    << b47_outcome(rollback_probe.motor_a,
                                                   rollback_probe.motor_b)
                                    << ',' << rollback_correct << ','
                                    << (!rollback_correct) << ','
                                    << b47_outcome(isolated_probe.motor_a,
                                                   isolated_probe.motor_b)
                                    << ',' << isolated_correct << ','
                                    << isolated_correct << '\n';
                            }
                        }

                        const auto target_id = state.circuit.cue_synapses[
                            cue * 2 + (target_a ? 0U : 1U)];
                        auto& active_life = life[cue];
                        if (row.correct) {
                            ++active_life.successful_experiences;
                            if (active_life.first_success == kB51Never) {
                                active_life.first_success = episode;
                            } else if (active_life.second_success == kB51Never) {
                                active_life.second_success = episode;
                            }
                        }
                        if (row.after_weights[target_id].weight >
                            row.before_weights[target_id].weight) {
                            ++active_life.positive_increments;
                            if (active_life.first_increment == kB51Never) {
                                active_life.first_increment = episode;
                            } else if (active_life.second_increment == kB51Never) {
                                active_life.second_increment = episode;
                            }
                        }
                        for (std::uint8_t path_cue = 0; path_cue < 2; ++path_cue) {
                            auto& path = life[path_cue];
                            if (path.first_increment != kB51Never &&
                                path.second_increment == kB51Never &&
                                episode > path.first_increment) {
                                path.negative_between_increments +=
                                    row.modulation < 0;
                                path.competing_positive_between_increments +=
                                    row.modulation > 0 && cue != path_cue;
                            }
                            const bool previous = path.currently_correct;
                            const bool current = after_correct[path_cue];
                            const bool path_target_a = b48_target_a(variant,
                                path_cue, episode, b46_training(variant.task));
                            const auto path_target_id = state.circuit.cue_synapses[
                                path_cue * 2 + (path_target_a ? 0U : 1U)];
                            const auto path_competitor_id = state.circuit.cue_synapses[
                                path_cue * 2 + (path_target_a ? 1U : 0U)];
                            if (!previous && current) {
                                const char* event_name = path.ever_acquired ?
                                    "reacquisition" : "first_acquisition";
                                if (!path.ever_acquired) {
                                    path.first_acquisition = episode;
                                    path.ever_acquired = true;
                                } else {
                                    ++path.reacquisitions;
                                    if (path.first_reacquisition == kB51Never) {
                                        path.first_reacquisition = episode;
                                    }
                                }
                                ability << phase << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << static_cast<unsigned>(path_cue) << ','
                                    << event_name << ",weight_or_state_transition,"
                                    << state.runtime.network().synapse(
                                        path_target_id).weight << ','
                                    << state.runtime.network().synapse(
                                        path_competitor_id).weight << ','
                                    << state.runtime.network().synapse(
                                        state.circuit.inhibitory_outputs[0]).weight
                                    << ',' << state.runtime.network().synapse(
                                        state.circuit.inhibitory_outputs[1]).weight
                                    << ',' << after_correct[1U - path_cue] << '\n';
                                if (phase == "discovery" &&
                                    !continuation_emitted[path_cue]) {
                                    b51_emit_controlled_continuations(controlled,
                                        cluster, state.runtime, state.circuit,
                                        variant, state.plans, seed, path_cue,
                                        episode);
                                    continuation_emitted[path_cue] = true;
                                }
                            } else if (previous && !current) {
                                ++path.losses;
                                if (path.first_loss == kB51Never) {
                                    path.first_loss = episode;
                                }
                                const auto cause = b51_loss_cause(
                                    row, state.circuit, path_cue, path_target_a);
                                ability << phase << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << static_cast<unsigned>(path_cue)
                                    << ",loss," << cause << ','
                                    << state.runtime.network().synapse(
                                        path_target_id).weight << ','
                                    << state.runtime.network().synapse(
                                        path_competitor_id).weight << ','
                                    << state.runtime.network().synapse(
                                        state.circuit.inhibitory_outputs[0]).weight
                                    << ',' << state.runtime.network().synapse(
                                        state.circuit.inhibitory_outputs[1]).weight
                                    << ',' << after_correct[1U - path_cue] << '\n';

                                if (path.losses == 1) {
                                    std::vector<std::pair<std::string,
                                        eve::bios::Runtime>> interventions;
                                    interventions.emplace_back("D0_natural",
                                        state.runtime);
                                    auto rollback_sensor = state.runtime;
                                    auto rollback_inhibition = state.runtime;
                                    auto isolated_competing = state.runtime;
                                    for (eve::SynapseId id = 0;
                                         id < row.after_weights.size(); ++id) {
                                        if (row.before_weights[id].weight ==
                                            row.after_weights[id].weight) continue;
                                        const bool sensor = std::find(
                                            state.circuit.cue_synapses.begin(),
                                            state.circuit.cue_synapses.end(), id) !=
                                            state.circuit.cue_synapses.end();
                                        const bool inhibition = std::find(
                                            state.circuit.inhibitory_outputs.begin(),
                                            state.circuit.inhibitory_outputs.end(), id) !=
                                            state.circuit.inhibitory_outputs.end();
                                        if (sensor) rollback_sensor.set_experimental_synapse_weight(
                                            id, row.before_weights[id].weight);
                                        if (inhibition) rollback_inhibition.set_experimental_synapse_weight(
                                            id, row.before_weights[id].weight);
                                        isolated_competing.set_experimental_synapse_weight(
                                            id, row.before_weights[id].weight);
                                    }
                                    for (eve::SynapseId id = 0;
                                         id < row.after_weights.size(); ++id) {
                                        if (row.before_weights[id].weight ==
                                            row.after_weights[id].weight) continue;
                                        const auto role = b50_synapse_role(
                                            state.circuit, id, path_cue,
                                            path_target_a);
                                        if (role != "cue_target") {
                                            isolated_competing.set_experimental_synapse_weight(
                                                id, row.after_weights[id].weight);
                                        }
                                    }
                                    interventions.emplace_back(
                                        "rollback_sensor", rollback_sensor);
                                    interventions.emplace_back(
                                        "rollback_inhibition", rollback_inhibition);
                                    interventions.emplace_back(
                                        "isolated_competing", isolated_competing);
                                    auto neutral = b51_episode(row.before,
                                        state.circuit, cue,
                                        row.planned_action, target_a, episode,
                                        true, std::int8_t{0});
                                    interventions.emplace_back(
                                        "neutral_modulation", neutral.after_feedback);
                                    auto no_plasticity = b51_episode(row.before,
                                        state.circuit, cue,
                                        row.planned_action, target_a, episode,
                                        false);
                                    interventions.emplace_back(
                                        "plasticity_disabled", no_plasticity.after_feedback);
                                    for (const auto& intervention : interventions) {
                                        const auto immediate = b51_recall_pair(
                                            intervention.second, state.circuit,
                                            variant, episode);
                                        losses << phase << ',' << family << ','
                                            << (inverse ? "inverse" : "normal")
                                            << ',' << seed << ',' << episode << ','
                                            << static_cast<unsigned>(path_cue) << ','
                                            << cause << ',' << intervention.first
                                            << ',' << immediate[path_cue] << ','
                                            << immediate[1U - path_cue] << ','
                                            << intervention.second.network().synapse(
                                                path_target_id).weight << ','
                                            << intervention.second.network().synapse(
                                                state.circuit.inhibitory_outputs[0]).weight
                                            << ',' << intervention.second.network().synapse(
                                                state.circuit.inhibitory_outputs[1]).weight
                                            << '\n';
                                        auto future = b51_continue(
                                            intervention.second, state.circuit,
                                            variant, state.plans, episode + 1, 16);
                                        const auto future_correct = b51_recall_pair(
                                            future, state.circuit, variant,
                                            episode + 16);
                                        loss_future << phase << ',' << family << ','
                                            << (inverse ? "inverse" : "normal")
                                            << ',' << seed << ',' << episode << ','
                                            << static_cast<unsigned>(path_cue) << ','
                                            << cause << ',' << intervention.first
                                            << ",16," << future_correct[path_cue]
                                            << ',' << future_correct[1U - path_cue]
                                            << ',' << (future_correct[0] &&
                                                       future_correct[1]) << '\n';
                                    }
                                }
                            }
                            path.currently_correct = current;
                            path.current_duration = current ?
                                path.current_duration + 1 : 0;
                            path.longest_duration = std::max(
                                path.longest_duration, path.current_duration);
                            if (path.first_acquisition != kB51Never) {
                                const auto offset = episode - path.first_acquisition;
                                if (offset == 0 || offset == 1 || offset == 4 ||
                                    offset == 8 || offset == 16 || offset == 32 ||
                                    offset == 64) {
                                    stability << phase << ',' << family << ','
                                        << (inverse ? "inverse" : "normal") << ','
                                        << seed << ','
                                        << static_cast<unsigned>(path_cue) << ','
                                        << path.first_acquisition << ',' << offset
                                        << ',' << episode << ',' << current << ','
                                        << after_correct[1U - path_cue] << ','
                                        << state.runtime.network().synapse(
                                            path_target_id).weight << ','
                                        << path.losses << '\n';
                                }
                            }
                        }

                        const bool both = after_correct[0] && after_correct[1];
                        if (both) {
                            if (first_both == kB51Never) first_both = episode;
                            ++both_total;
                            ++both_current;
                            both_longest = std::max(both_longest, both_current);
                        } else {
                            both_current = 0;
                        }

                        if (episode + 1 == 128 || episode + 1 == 256 ||
                            episode + 1 == 512) {
                            const auto checkpoint = episode + 1;
                            const auto evaluated = b51_evaluate(state.runtime,
                                state.circuit, variant, state.plans, checkpoint);
                            const double e1_x = b47_rate(evaluated.e1[0]);
                            const double e1_y = b47_rate(evaluated.e1[1]);
                            const double e2_x = b47_rate(evaluated.e2[0]);
                            const double e2_y = b47_rate(evaluated.e2[1]);
                            const bool e1_robust = e1_x >= .75 && e1_y >= .75;
                            const bool e2_robust = e2_x >= .75 && e2_y >= .75;
                            checkpoints << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << checkpoint << ',' << e1_x << ','
                                << e1_y << ',' << e2_x << ',' << e2_y << ','
                                << e1_robust << ',' << e2_robust << ','
                                << evaluated.identical_start << ','
                                << state.runtime.network().validate() << '\n';
                            for (std::uint8_t evaluated_cue = 0;
                                 evaluated_cue < 2; ++evaluated_cue) {
                                const auto emit_confusion = [&](const char* metric,
                                        const B47Counts& counts) {
                                    confusion << phase << ',' << family << ','
                                        << (inverse ? "inverse" : "normal")
                                        << ',' << seed << ',' << checkpoint << ','
                                        << metric << ','
                                        << static_cast<unsigned>(evaluated_cue)
                                        << ',' << counts.correct << ','
                                        << counts.wrong << ',' << counts.both
                                        << ',' << counts.none << '\n';
                                };
                                emit_confusion("E1", evaluated.e1[evaluated_cue]);
                                emit_confusion("E2", evaluated.e2[evaluated_cue]);
                            }
                            cluster_results.push_back(B51ClusterResult{
                                phase, family, inverse, seed, checkpoint,
                                e1_x, e1_y, e2_x, e2_y, e1_robust,
                                state.runtime.network().validate()});
                        }
                    }

                    const auto final_correct = b51_recall_pair(
                        state.runtime, state.circuit, variant, kB51Episodes);
                    for (std::uint8_t cue = 0; cue < 2; ++cue) {
                        const bool target_a = b48_target_a(variant, cue,
                            kB51Episodes, b46_training(variant.task));
                        const auto target_id = state.circuit.cue_synapses[
                            cue * 2 + (target_a ? 0U : 1U)];
                        const auto& path = life[cue];
                        lifecycle << phase << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << static_cast<unsigned>(cue) << ','
                            << (target_a ? 'A' : 'B') << ',' << kB51Episodes << ','
                            << b51_u32(path.first_success) << ','
                            << b51_u32(path.second_success) << ','
                            << b51_u32(path.first_increment) << ','
                            << b51_u32(path.second_increment) << ','
                            << path.successful_experiences << ','
                            << path.positive_increments << ','
                            << path.negative_between_increments << ','
                            << path.competing_positive_between_increments << ','
                            << b51_u32(path.first_acquisition) << ','
                            << b51_u32(path.first_loss) << ','
                            << b51_u32(path.first_reacquisition) << ','
                            << path.losses << ',' << path.reacquisitions << ','
                            << path.longest_duration << ',' << final_correct[cue]
                            << ',' << state.runtime.network().synapse(target_id).weight
                            << '\n';
                    }
                    seed_summary << phase << ',' << family << ','
                        << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << kB51Episodes << ',' << b51_u32(first_both)
                        << ',' << both_total << ',' << both_longest << ','
                        << life[0].ever_acquired << ',' << life[1].ever_acquired
                        << ',' << life[0].losses << ',' << life[1].losses << ','
                        << life[0].reacquisitions << ',' << life[1].reacquisitions
                        << ',' << final_correct[0] << ',' << final_correct[1]
                        << ',' << state.runtime.network().validate() << ",0\n";
                }
            }
        }
    }
}

void b51_write_statistics(
    const std::filesystem::path& output,
    const std::vector<B51ClusterResult>& rows) {
    std::ofstream summary(output / "cluster-statistics.csv");
    summary << "phase,family,assignment,checkpoint,metric,clusters,mean,ci_low,ci_high\n";
    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "phase,family,assignment,checkpoint,E1_X,E1_Y,robust_seeds,advantage_vs_no_learning,ci_low,ci_high,passes\n";
    std::uint64_t salt = 0;
    for (const std::string phase : {"discovery", "confirmation"}) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (const std::uint32_t checkpoint : {128U, 256U, 512U}) {
                    std::map<std::string, std::vector<double>> values;
                    std::uint64_t robust = 0;
                    for (const auto& row : rows) {
                        if (row.phase != phase || row.family != family ||
                            row.inverse != inverse ||
                            row.checkpoint != checkpoint) continue;
                        values["E1_X"].push_back(row.e1_x);
                        values["E1_Y"].push_back(row.e1_y);
                        values["E1_score"].push_back((row.e1_x + row.e1_y) / 2.0);
                        values["E2_X"].push_back(row.e2_x);
                        values["E2_Y"].push_back(row.e2_y);
                        values["E2_score"].push_back((row.e2_x + row.e2_y) / 2.0);
                        values["E1_robust"].push_back(row.robust ? 1.0 : 0.0);
                        robust += row.robust;
                    }
                    for (const auto& [name, samples] : values) {
                        const auto interval = bootstrap_interval(
                            samples, 0x5100000ULL + ++salt);
                        summary << phase << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ','
                            << checkpoint << ',' << name << ',' << samples.size()
                            << ',' << b48_mean(samples) << ',' << interval.first
                            << ',' << interval.second << '\n';
                    }
                    const auto& scores = values["E1_score"];
                    const auto score_interval = bootstrap_interval(
                        scores, 0x5101000ULL + ++salt);
                    const auto mean_x = b48_mean(values["E1_X"]);
                    const auto mean_y = b48_mean(values["E1_Y"]);
                    const auto advantage = b48_mean(scores);
                    const bool pass = mean_x >= .75 && mean_y >= .75 &&
                        robust >= 48 && advantage >= .15 &&
                        score_interval.first > 0;
                    criteria << phase << ',' << family << ','
                        << (inverse ? "inverse" : "normal") << ','
                        << checkpoint << ',' << mean_x << ',' << mean_y << ','
                        << robust << ',' << advantage << ','
                        << score_interval.first << ',' << score_interval.second
                        << ',' << pass << '\n';
                }
            }
        }
    }
}

struct B51Accumulator {
    double sum{};
    std::uint64_t count{};
};

void b51_lifecycle_statistics(const std::filesystem::path& output) {
    using ClusterMap = std::map<std::string, B51Accumulator>;
    std::map<std::string, std::map<std::string, ClusterMap>> metrics;
    const auto add = [&](const std::string& phase, const std::string& metric,
                         const std::string& cluster, const double value) {
        auto& accumulator = metrics[phase][metric][cluster];
        accumulator.sum += value;
        ++accumulator.count;
    };
    std::string line;
    {
        std::ifstream file(output / "path-lifecycle.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            const auto cluster = row[0] + "_f" + row[1] + "_" + row[2] +
                "_s" + row[3];
            add(row[0], "path_ever_acquired", cluster,
                row[15] != "4294967295");
            add(row[0], "path_ever_lost", cluster,
                row[16] != "4294967295");
            add(row[0], "path_ever_reacquired", cluster,
                row[17] != "4294967295");
            add(row[0], "path_final_correct", cluster, std::stod(row[21]));
            add(row[0], "path_positive_increments", cluster,
                std::stod(row[12]));
            add(row[0], "path_losses", cluster, std::stod(row[18]));
        }
    }
    {
        std::ifstream file(output / "stability-checkpoints.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            const auto cluster = row[0] + "_f" + row[1] + "_" + row[2] +
                "_s" + row[3];
            add(row[0], "retention_offset_" + row[6], cluster,
                std::stod(row[8]));
        }
    }
    {
        std::ifstream file(output / "seed-summary.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            const auto cluster = row[0] + "_f" + row[1] + "_" + row[2] +
                "_s" + row[3];
            add(row[0], "network_ever_both", cluster,
                row[5] != "4294967295");
            add(row[0], "network_simultaneous_fraction", cluster,
                std::stod(row[6]) / 512.0);
            add(row[0], "network_final_both", cluster,
                std::stod(row[14]) * std::stod(row[15]));
        }
    }
    {
        std::ifstream file(output / "loss-causal-copies.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            const auto cluster = row[0] + "_f" + row[1] + "_" + row[2] +
                "_s" + row[3];
            add(row[0], "first_loss_rescue_" + row[7], cluster,
                std::stod(row[8]));
        }
    }
    std::map<std::string, ClusterMap> controlled_learned;
    std::map<std::string, ClusterMap> controlled_both;
    {
        std::ifstream file(output / "controlled-continuations.csv");
        std::getline(file, line);
        while (std::getline(file, line)) {
            const auto row = b50_fields(line);
            const auto phase = row[0].substr(0, row[0].find('_'));
            add(phase, "controlled_" + row[3] + "_learned", row[0],
                std::stod(row[5]));
            add(phase, "controlled_" + row[3] + "_both", row[0],
                std::stod(row[7]));
            auto& learned = controlled_learned[row[3]][row[0]];
            learned.sum += std::stod(row[5]);
            ++learned.count;
            auto& both = controlled_both[row[3]][row[0]];
            both.sum += std::stod(row[7]);
            ++both.count;
        }
    }

    std::ofstream summary(output / "lifecycle-statistics.csv");
    summary << "phase,metric,clusters,mean,ci_low,ci_high\n";
    std::uint64_t salt = 0;
    for (const auto& [phase, phase_metrics] : metrics) {
        for (const auto& [metric, clusters] : phase_metrics) {
            std::vector<double> samples;
            for (const auto& [cluster, value] : clusters) {
                static_cast<void>(cluster);
                samples.push_back(value.sum / value.count);
            }
            const auto interval = bootstrap_interval(
                samples, 0x5102000ULL + ++salt);
            summary << phase << ',' << metric << ',' << samples.size() << ','
                << b48_mean(samples) << ',' << interval.first << ','
                << interval.second << '\n';
        }
    }

    std::ofstream paired(output / "controlled-paired-comparisons.csv");
    paired << "metric,candidate,control,clusters,mean_difference,ci_low,ci_high\n";
    const auto emit_paired = [&](const char* metric,
            const std::map<std::string, ClusterMap>& values) {
        const auto& control = values.at("G1_natural");
        for (const auto& [variant, clusters] : values) {
            if (variant == "G1_natural") continue;
            std::vector<double> differences;
            for (const auto& [cluster, value] : clusters) {
                const auto found = control.find(cluster);
                if (found == control.end()) continue;
                differences.push_back(value.sum / value.count -
                    found->second.sum / found->second.count);
            }
            const auto interval = bootstrap_interval(
                differences, 0x5103000ULL + ++salt);
            paired << metric << ',' << variant << ",G1_natural,"
                << differences.size() << ',' << b48_mean(differences) << ','
                << interval.first << ',' << interval.second << '\n';
        }
    };
    emit_paired("learned_correct", controlled_learned);
    emit_paired("both_correct", controlled_both);
}

void b51_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,checkpoint,fingerprint,exact_restore,exact_continuation,valid\n";
    B48Variant variant{"B51_persistence", B46Task::mixed,
        B48Policy::reference, false, true, false, 0};
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto state = b51_initial_state(variant, seed);
        state.runtime = b51_continue(state.runtime, state.circuit,
            variant, state.plans, 0, 512);
        const auto path = output / (".b51-state-" + std::to_string(seed));
        state.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        auto original = state.runtime;
        static_cast<void>(original.run());
        static_cast<void>(restored.run());
        file << seed << ",512," << state.runtime.state_fingerprint() << ','
            << exact << ',' << (original.state_fingerprint() ==
                restored.state_fingerprint()) << ','
            << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b51");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    b51_write_b50_reproduction(output);
    b51_validated_repetition(output);
    std::vector<B51ClusterResult> cluster_results;
    b51_run_natural(output, cluster_results);
    b51_write_statistics(output, cluster_results);
    b51_lifecycle_statistics(output);
    b51_persistence(output);

    std::ofstream mechanism(output / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K9,0,B51 is causal diagnosis of the unchanged system; no new local mechanism is assumed\n";
    mechanism << "K10,0,no K9 mechanism exists to deactivate\n";

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,natural_networks,natural_episodes,checkpoints\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << 2 * 3 * 2 * 64 << ','
        << 2 * 3 * 2 * 64 * kB51Episodes << ','
        << cluster_results.size() << '\n';
    std::cout << "B.5.1 natural learning lifecycle complete K9=not_assumed\n";
    return 0;
}
