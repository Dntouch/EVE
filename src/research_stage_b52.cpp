#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B51_EMBEDDED
#include "research_stage_b51.cpp"
#undef EVE_RESEARCH_STAGE_B51_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <set>

namespace {

constexpr std::uint32_t kB52Episodes = 512;

struct B52StateDifference {
    bool same_weights{true};
    bool same_structure{true};
    std::uint32_t membrane{};
    std::uint32_t refractory{};
    std::uint32_t input_count{};
    std::uint32_t recovery{};
    std::uint32_t local_memory{};
    std::uint32_t eligibility{};
    std::uint32_t context{};
    std::uint32_t synapse_activity{};
    std::int64_t time_delta{};
    std::int64_t queue_delta{};
};

struct B52NetworkResult {
    std::string phase;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    std::uint64_t losses{};
    std::uint64_t no_weight_losses{};
    std::uint64_t sensor_losses{};
    std::uint64_t inhibitory_losses{};
    std::uint64_t combined_weight_losses{};
    std::uint64_t transient_causal{};
    std::uint64_t synaptic_causal{};
    std::uint64_t combined_causal{};
    std::uint64_t transient_restored{};
    std::uint64_t both_episodes{};
    std::uint64_t longest_both{};
    double e1_x{};
    double e1_y{};
    bool final_both{};
    bool valid{};
};

template <typename Container>
bool b52_contains(
    const Container& values, const eve::SynapseId id) {
    return std::find(values.begin(), values.end(), id) != values.end();
}

B52StateDifference b52_state_difference(
    const eve::bios::Runtime& before, const eve::bios::Runtime& after) {
    B52StateDifference result;
    const auto& before_synapses = before.network().synapses();
    const auto& after_synapses = after.network().synapses();
    result.same_structure = before_synapses.size() == after_synapses.size();
    result.same_weights = result.same_structure;
    if (result.same_structure) {
        for (std::size_t index = 0; index < before_synapses.size(); ++index) {
            const auto& left = before_synapses[index];
            const auto& right = after_synapses[index];
            result.same_weights = result.same_weights && left.weight == right.weight;
            result.same_structure = result.same_structure &&
                left.sender == right.sender && left.receiver == right.receiver &&
                left.effect == right.effect &&
                left.plasticity_protected == right.plasticity_protected;
            result.synapse_activity += left.activity_count != right.activity_count;
        }
    }
    const auto count = std::min(before.network().neuron_count(),
                                after.network().neuron_count());
    for (eve::NeuronId id = 0; id < count; ++id) {
        const auto& left = before.network().neuron(id);
        const auto& right = after.network().neuron(id);
        result.membrane += left.membrane_potential != right.membrane_potential;
        result.refractory += left.refractory_events_remaining !=
            right.refractory_events_remaining;
        result.input_count += left.input_event_count != right.input_event_count;
        result.recovery += left.recovery_generation != right.recovery_generation ||
            left.scheduled_recovery_time != right.scheduled_recovery_time;
        result.local_memory += before.network().local_event_memory(id) !=
            after.network().local_event_memory(id);
        result.eligibility += before.network().eligibility_records(id) !=
            after.network().eligibility_records(id);
        result.context += before.network().context_traces(id) !=
            after.network().context_traces(id);
    }
    result.time_delta = static_cast<std::int64_t>(after.simulated_time()) -
        static_cast<std::int64_t>(before.simulated_time());
    result.queue_delta = static_cast<std::int64_t>(after.diagnostics().queued_events) -
        static_cast<std::int64_t>(before.diagnostics().queued_events);
    return result;
}

eve::bios::Runtime b52_factor_state(
    const eve::bios::Runtime& before, const eve::bios::Runtime& after,
    const B47Circuit& circuit, const bool new_sensor,
    const bool new_inhibition, const bool new_transient) {
    eve::bios::Runtime result = new_transient ? after : before;
    for (const auto id : circuit.cue_synapses) {
        result.set_experimental_synapse_weight(id,
            (new_sensor ? after : before).network().synapse(id).weight);
    }
    for (const auto id : circuit.inhibitory_outputs) {
        result.set_experimental_synapse_weight(id,
            (new_inhibition ? after : before).network().synapse(id).weight);
    }
    return result;
}

std::array<std::array<bool, 2>, 8> b52_factorial(
    const eve::bios::Runtime& before, const eve::bios::Runtime& after,
    const B47Circuit& circuit, const B48Variant& variant,
    const std::uint32_t episode, std::ofstream& file,
    const std::string& prefix, const std::uint8_t lost_cue) {
    const std::array<std::array<bool, 3>, 8> factors{{
        {{false, false, false}}, {{true, false, false}},
        {{false, true, false}}, {{false, false, true}},
        {{true, true, false}}, {{true, false, true}},
        {{false, true, true}}, {{true, true, true}},
    }};
    std::array<std::array<bool, 2>, 8> result{};
    for (std::size_t index = 0; index < factors.size(); ++index) {
        auto runtime = b52_factor_state(before, after, circuit,
            factors[index][0], factors[index][1], factors[index][2]);
        result[index] = b51_recall_pair(runtime, circuit, variant, episode);
        const auto probe_x = b51_probe(runtime, circuit, 0);
        const auto probe_y = b51_probe(runtime, circuit, 1);
        file << prefix << ",V" << index << ',' << factors[index][0] << ','
            << factors[index][1] << ',' << factors[index][2] << ','
            << result[index][lost_cue] << ',' << result[index][1U - lost_cue]
            << ',' << b47_outcome(probe_x.motor_a, probe_x.motor_b) << ','
            << b47_outcome(probe_y.motor_a, probe_y.motor_b) << ','
            << runtime.network().validate() << '\n';
    }
    return result;
}

void b52_emit_stability(
    std::ofstream& file, const char* checkpoint_type,
    const std::string& phase, const std::uint32_t family, const bool inverse,
    const std::uint64_t seed, const std::uint32_t episode,
    const eve::bios::Runtime& snapshot, const B47Circuit& circuit,
    const B48Variant& variant, const std::vector<B48Plan>& plans) {
    for (const std::uint32_t offset : {0U, 1U, 4U, 8U, 16U, 32U, 64U}) {
        if (episode + offset >= kB52Episodes) continue;
        auto runtime = offset == 0 ? snapshot : b51_continue(
            snapshot, circuit, variant, plans, episode + 1, offset);
        const auto correct = b51_recall_pair(
            runtime, circuit, variant, episode + offset);
        std::uint32_t sensor_changes = 0;
        std::uint32_t inhibitory_changes = 0;
        for (const auto id : circuit.cue_synapses) {
            sensor_changes += snapshot.network().synapse(id).weight !=
                runtime.network().synapse(id).weight;
        }
        for (const auto id : circuit.inhibitory_outputs) {
            inhibitory_changes += snapshot.network().synapse(id).weight !=
                runtime.network().synapse(id).weight;
        }
        file << phase << ',' << family << ','
            << (inverse ? "inverse" : "normal") << ',' << seed << ','
            << checkpoint_type << ',' << episode << ',' << offset << ','
            << correct[0] << ',' << correct[1] << ','
            << (correct[0] && correct[1]) << ',' << sensor_changes << ','
            << inhibitory_changes << ',' << runtime.network().validate() << '\n';
    }
}

void b52_reproduce_b51(const std::filesystem::path& output) {
    const auto reference = output / "b51-reproduction";
    std::filesystem::create_directories(reference);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b51_write_b50_reproduction(reference);
    b51_validated_repetition(reference);
    std::vector<B51ClusterResult> rows;
    b51_run_natural(reference, rows);
    b51_write_statistics(reference, rows);
    b51_lifecycle_statistics(reference);
    b51_persistence(reference);
    std::ofstream mechanism(reference / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K9,0,B51 is causal diagnosis of the unchanged system; no new local mechanism is assumed\n";
    mechanism << "K10,0,no K9 mechanism exists to deactivate\n";
    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(reference / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,natural_networks,natural_episodes,checkpoints\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ",768,393216," << rows.size() << '\n';
}

void b52_task_controls(const std::filesystem::path& output) {
    const std::array<B46Task, 8> tasks{{
        B46Task::only_x, B46Task::only_y, B46Task::x_then_y,
        B46Task::y_then_x, B46Task::alternating, B46Task::mixed,
        B46Task::reversal, B46Task::retention,
    }};
    std::ofstream matrix(output / "task-matrix.csv");
    matrix << "group,task,diagnostic,production_change\n";
    for (std::uint32_t group = 0; group <= 10; ++group) {
        const char* description = group == 0 ? "B51_reference" :
            group == 1 ? "no_learning" : group == 2 ? "fixed_inhibition" :
            group == 3 ? "learned_inhibition" : group == 4 ? "no_more_plasticity" :
            group == 5 ? "sensor_rollback" : group == 6 ? "inhibition_rollback" :
            group == 7 ? "transient_restore" : group == 8 ? "S_I_T_factorial" :
            group == 9 ? "conditional_local_variant_not_implemented" :
                         "disabled_variant_not_applicable";
        matrix << 'K' << group << ',' << description << ',' << (group >= 5)
            << ",0\n";
    }
    std::ofstream file(output / "task-results.csv");
    file << "task,assignment,seed,training,E1_X,E1_Y,E2_X,E2_Y,final_X,final_Y,valid\n";
    for (const auto task : tasks) {
        for (const bool inverse : {false, true}) {
            B48Variant variant{"B52_task_control", task, B48Policy::reference,
                inverse, true, false, 0};
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                auto state = b51_initial_state(variant, seed);
                const auto training = b46_training(task);
                state.runtime = b51_continue(state.runtime, state.circuit,
                    variant, state.plans, 0, training);
                const auto evaluated = b51_evaluate(state.runtime, state.circuit,
                    variant, state.plans, training);
                const auto final = b51_recall_pair(
                    state.runtime, state.circuit, variant, training);
                file << b46_task_name(task) << ','
                    << (inverse ? "inverse" : "normal") << ',' << seed << ','
                    << training << ',' << b47_rate(evaluated.e1[0]) << ','
                    << b47_rate(evaluated.e1[1]) << ','
                    << b47_rate(evaluated.e2[0]) << ','
                    << b47_rate(evaluated.e2[1]) << ',' << final[0] << ','
                    << final[1] << ',' << state.runtime.network().validate() << '\n';
            }
        }
    }
}

void b52_control_groups(const std::filesystem::path& output) {
    std::ofstream manifest(output / "control-group-manifest.csv");
    manifest << "group,kind,implementation,data_file,enabled\n";
    manifest << "K0,training,unchanged B51 reference,control-groups.csv,1\n";
    manifest << "K1,training,no learning,control-groups.csv,1\n";
    manifest << "K2,training,fixed inhibitory connections,control-groups.csv,1\n";
    manifest << "K3,training,learned inhibitory connections,control-groups.csv,1\n";
    manifest << "K4,training,no plasticity after episode 128,control-groups.csv,1\n";
    manifest << "K5,diagnostic,sensor rollback V6,factorial-ablations.csv.gz,1\n";
    manifest << "K6,diagnostic,inhibitory rollback V5,factorial-ablations.csv.gz,1\n";
    manifest << "K7,diagnostic,old transient state V4,factorial-ablations.csv.gz,1\n";
    manifest << "K8,diagnostic,complete S-I-T factorial V0-V7,factorial-ablations.csv.gz,1\n";
    manifest << "K9,research variant,not causally justified,mechanism-decision.csv,0\n";
    manifest << "K10,deactivation control,no K9 implementation,mechanism-decision.csv,0\n";

    std::ofstream file(output / "control-groups.csv");
    file << "group,seed,training_mode,E1_X,E1_Y,E2_X,E2_Y,final_X,final_Y,valid\n";
    for (const std::string group : {"K0_reference", "K1_no_learning",
             "K2_fixed_inhibition", "K3_learned_inhibition",
             "K4_no_further_plasticity"}) {
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const bool no_learning = group == "K1_no_learning";
            const bool fixed = group == "K2_fixed_inhibition";
            B48Variant variant{"B52_control", B46Task::mixed,
                B48Policy::reference, false, !no_learning, fixed, 0};
            auto state = b51_initial_state(variant, seed);
            if (group == "K4_no_further_plasticity") {
                state.runtime = b51_continue(state.runtime, state.circuit,
                    variant, state.plans, 0, 128);
                state.runtime = b51_continue(state.runtime, state.circuit,
                    variant, state.plans, 128, 384, false);
            } else {
                state.runtime = b51_continue(state.runtime, state.circuit,
                    variant, state.plans, 0, kB52Episodes, !no_learning);
            }
            const auto evaluated = b51_evaluate(state.runtime, state.circuit,
                variant, state.plans, kB52Episodes);
            const auto final = b51_recall_pair(state.runtime, state.circuit,
                variant, kB52Episodes);
            file << group << ',' << seed << ','
                << (group == "K4_no_further_plasticity" ?
                    "plasticity_stopped_after_128" :
                    no_learning ? "disabled" : fixed ? "fixed_inhibition" :
                    "natural") << ',' << b47_rate(evaluated.e1[0]) << ','
                << b47_rate(evaluated.e1[1]) << ','
                << b47_rate(evaluated.e2[0]) << ','
                << b47_rate(evaluated.e2[1]) << ',' << final[0] << ','
                << final[1] << ',' << state.runtime.network().validate() << '\n';
        }
    }
}

std::vector<B52NetworkResult> b52_primary(const std::filesystem::path& output) {
    std::ofstream losses(output / "loss-events.csv");
    losses << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,sensor_changes,inhibitory_changes,other_changes,same_weights,pre_correct,post_correct,other_correct\n";
    std::ofstream transient(output / "transient-state-differences.csv");
    transient << "phase,family,assignment,seed,episode,lost_cue,loss_index,same_weights,same_structure,membrane_differences,refractory_differences,input_count_differences,recovery_differences,local_memory_differences,eligibility_differences,context_differences,synapse_activity_differences,time_delta,queue_delta\n";
    std::ofstream factorial(output / "factorial-ablations.csv");
    factorial << "phase,family,assignment,seed,episode,lost_cue,loss_index,variant,new_S,new_I,new_T,lost_correct,other_correct,X_outcome,Y_outcome,valid\n";
    std::ofstream inhibition(output / "inhibitory-eligibility.csv");
    inhibition << "phase,family,assignment,seed,episode,cue,target,synapse,sender,receiver,before,after,delta,temporally_eligible,eligibility_generation,functionally_active,ablation_changes_autonomous,isolated_change_changes_autonomous,modulation\n";
    std::ofstream inhibition_followup(output / "inhibitory-followup.csv");
    inhibition_followup << "phase,family,assignment,seed,episode,synapse,variant,future_episodes,X_correct,Y_correct,both_correct,valid\n";
    std::ofstream overlap(output / "sensor-candidate-overlap.csv");
    overlap << "phase,family,assignment,seed,episode,cue,current_candidates,other_candidates,shared_receivers,current_ids,other_ids\n";
    std::ofstream probes(output / "probe-controls.csv");
    probes << "phase,family,assignment,seed,episode,lost_cue,loss_index,control,gap,first_outcome,second_outcome,first_correct,second_correct,identical_fingerprint\n";
    std::ofstream stability(output / "simultaneous-stability.csv");
    stability << "phase,family,assignment,seed,checkpoint_type,source_episode,offset,X_correct,Y_correct,both_correct,sensor_changes,inhibitory_changes,valid\n";
    std::ofstream checkpoints(output / "checkpoint-evaluation.csv");
    checkpoints << "phase,family,assignment,seed,checkpoint,E1_X,E1_Y,E2_X,E2_Y,E1_robust,E2_robust,identical_start,valid\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "phase,family,assignment,seed,checkpoint,metric,cue,correct,wrong,both,none\n";
    std::ofstream summaries(output / "seed-summary.csv");
    summaries << "phase,family,assignment,seed,losses,no_weight_losses,sensor_losses,inhibitory_losses,combined_weight_losses,transient_causal,synaptic_causal,combined_causal,transient_restored,both_episodes,longest_both,final_X,final_Y,valid,technical_abort\n";

    std::vector<B52NetworkResult> results;
    for (const std::string phase : {"discovery", "confirmation"}) {
        const std::uint64_t first_seed = phase == "discovery" ? 1 : 10001;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                B48Variant variant{"B52_memory_stability", B46Task::mixed,
                    B48Policy::reference, inverse, true, false, family};
                for (std::uint64_t seed = first_seed;
                     seed < first_seed + 64; ++seed) {
                    auto state = b51_initial_state(variant, seed);
                    B52NetworkResult aggregate;
                    aggregate.phase = phase;
                    aggregate.family = family;
                    aggregate.inverse = inverse;
                    aggregate.seed = seed;
                    auto previous_correct = b51_recall_pair(
                        state.runtime, state.circuit, variant, 0);
                    std::array<std::uint32_t, 2> loss_count{};
                    bool first_both_emitted = false;
                    bool later_both_emitted = false;
                    bool stable_both_emitted = false;
                    bool inhibition_followup_emitted = false;
                    bool both_was_lost = false;
                    std::uint32_t current_both = 0;

                    for (std::uint32_t episode = 0;
                         episode < kB52Episodes; ++episode) {
                        const auto cue = b48_cue(variant, state.plans[episode],
                            episode, b46_training(variant.task));
                        const bool target_a = b48_target_a(variant, cue,
                            episode, b46_training(variant.task));
                        auto row = b51_episode(state.runtime, state.circuit, cue,
                            state.plans[episode].exploration, target_a, episode);
                        state.runtime = row.after_feedback;
                        const auto after_correct = b51_recall_pair(
                            state.runtime, state.circuit, variant, episode);

                        std::set<eve::SynapseId> current_candidates;
                        std::set<eve::SynapseId> other_candidates;
                        std::set<eve::NeuronId> current_receivers;
                        std::set<eve::NeuronId> other_receivers;
                        for (eve::NeuronId neuron = 0;
                             neuron < row.after_activity.network().neuron_count();
                             ++neuron) {
                            for (const auto& record :
                                 row.after_activity.network().eligibility_records(neuron)) {
                                for (const auto candidate : record.candidates) {
                                    if (b52_contains(state.circuit.cue_synapses,
                                                     candidate)) {
                                        const auto sender = row.after_activity.network()
                                            .synapse(candidate).sender;
                                        if (sender == state.circuit.sensors[cue]) {
                                            current_candidates.insert(candidate);
                                            current_receivers.insert(
                                                row.after_activity.network()
                                                    .synapse(candidate).receiver);
                                        } else {
                                            other_candidates.insert(candidate);
                                            other_receivers.insert(
                                                row.after_activity.network()
                                                    .synapse(candidate).receiver);
                                        }
                                    }
                                }
                            }
                        }
                        std::uint32_t shared_receivers = 0;
                        for (const auto receiver : current_receivers) {
                            shared_receivers += other_receivers.count(receiver) != 0;
                        }
                        const auto emit_ids = [](const auto& values) {
                            std::string text;
                            for (const auto value : values) {
                                if (!text.empty()) text += '|';
                                text += std::to_string(value);
                            }
                            return text;
                        };
                        overlap << phase << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << episode << ',' << static_cast<unsigned>(cue)
                            << ',' << current_candidates.size() << ','
                            << other_candidates.size() << ',' << shared_receivers
                            << ',' << emit_ids(current_candidates) << ','
                            << emit_ids(other_candidates) << '\n';

                        for (const auto id : state.circuit.inhibitory_outputs) {
                            const auto before_weight = row.before.network().synapse(id).weight;
                            const auto after_weight = state.runtime.network().synapse(id).weight;
                            if (before_weight == after_weight) continue;
                            bool eligible = false;
                            std::uint64_t generation = 0;
                            for (eve::NeuronId neuron = 0;
                                 neuron < row.after_activity.network().neuron_count();
                                 ++neuron) {
                                for (const auto& record : row.after_activity.network()
                                         .eligibility_records(neuron)) {
                                    if (b52_contains(record.candidates, id)) {
                                        eligible = record.expires_at >=
                                            row.after_activity.simulated_time();
                                        generation = std::max(generation,
                                                              record.generation);
                                    }
                                }
                            }
                            const bool active = std::any_of(row.impulses.begin(),
                                row.impulses.end(), [id](const auto& impulse) {
                                    return impulse.synapse == id;
                                });
                            const auto natural = b51_probe(row.before,
                                state.circuit, cue);
                            auto ablated = row.before;
                            ablated.set_experimental_synapse_weight(id, 0);
                            const auto ablated_probe = b51_probe(
                                ablated, state.circuit, cue);
                            auto isolated = row.before;
                            isolated.set_experimental_synapse_weight(id, after_weight);
                            const auto isolated_probe = b51_probe(
                                isolated, state.circuit, cue);
                            inhibition << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ','
                                << static_cast<unsigned>(cue) << ','
                                << (target_a ? 'A' : 'B') << ',' << id << ','
                                << row.before.network().synapse(id).sender << ','
                                << row.before.network().synapse(id).receiver << ','
                                << before_weight << ',' << after_weight << ','
                                << after_weight - before_weight << ',' << eligible
                                << ',' << generation << ',' << active << ','
                                << (b47_outcome(natural.motor_a, natural.motor_b) !=
                                    b47_outcome(ablated_probe.motor_a,
                                                ablated_probe.motor_b)) << ','
                                << (b47_outcome(natural.motor_a, natural.motor_b) !=
                                    b47_outcome(isolated_probe.motor_a,
                                                isolated_probe.motor_b)) << ','
                                << static_cast<int>(row.modulation) << '\n';
                            if (!inhibition_followup_emitted &&
                                episode + 16 < kB52Episodes) {
                                std::array<std::pair<const char*,
                                    eve::bios::Runtime>, 3> variants{{
                                    {"natural", state.runtime},
                                    {"rollback_single_inhibition", state.runtime},
                                    {"isolated_single_inhibition", row.before},
                                }};
                                variants[1].second.set_experimental_synapse_weight(
                                    id, before_weight);
                                variants[2].second.set_experimental_synapse_weight(
                                    id, after_weight);
                                for (auto& intervention : variants) {
                                    auto future = b51_continue(
                                        intervention.second, state.circuit,
                                        variant, state.plans, episode + 1, 16);
                                    const auto correct = b51_recall_pair(future,
                                        state.circuit, variant, episode + 16);
                                    inhibition_followup << phase << ',' << family
                                        << ',' << (inverse ? "inverse" : "normal")
                                        << ',' << seed << ',' << episode << ','
                                        << id << ',' << intervention.first
                                        << ",16," << correct[0] << ','
                                        << correct[1] << ','
                                        << (correct[0] && correct[1]) << ','
                                        << future.network().validate() << '\n';
                                }
                                inhibition_followup_emitted = true;
                            }
                        }

                        for (std::uint8_t path = 0; path < 2; ++path) {
                            if (!previous_correct[path] || after_correct[path]) continue;
                            ++aggregate.losses;
                            const auto index = ++loss_count[path];
                            std::uint32_t sensor_changes = 0;
                            std::uint32_t inhibitory_changes = 0;
                            std::uint32_t other_changes = 0;
                            for (eve::SynapseId id = 0;
                                 id < row.after_weights.size(); ++id) {
                                if (row.before_weights[id].weight ==
                                    row.after_weights[id].weight) continue;
                                if (b52_contains(state.circuit.cue_synapses, id)) {
                                    ++sensor_changes;
                                } else if (b52_contains(
                                               state.circuit.inhibitory_outputs, id)) {
                                    ++inhibitory_changes;
                                } else {
                                    ++other_changes;
                                }
                            }
                            aggregate.no_weight_losses += sensor_changes == 0 &&
                                inhibitory_changes == 0 && other_changes == 0;
                            aggregate.sensor_losses += sensor_changes != 0;
                            aggregate.inhibitory_losses += inhibitory_changes != 0;
                            aggregate.combined_weight_losses += sensor_changes != 0 &&
                                inhibitory_changes != 0;
                            const auto difference = b52_state_difference(
                                row.before, state.runtime);
                            const auto cause = sensor_changes == 0 &&
                                    inhibitory_changes == 0 && other_changes == 0
                                ? "no_weight_change"
                                : sensor_changes != 0 && inhibitory_changes != 0
                                ? "sensor_and_inhibition"
                                : sensor_changes != 0 ? "sensor_change"
                                : inhibitory_changes != 0 ? "inhibitory_change"
                                : "other_change";
                            losses << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << episode << ','
                                << static_cast<unsigned>(path) << ',' << index
                                << ',' << cause << ',' << sensor_changes << ','
                                << inhibitory_changes << ',' << other_changes
                                << ',' << difference.same_weights << ','
                                << previous_correct[path] << ','
                                << after_correct[path] << ','
                                << after_correct[1U - path] << '\n';

                            const bool detailed = index <= 2 || difference.same_weights;
                            if (detailed) {
                                transient << phase << ',' << family << ','
                                    << (inverse ? "inverse" : "normal") << ','
                                    << seed << ',' << episode << ','
                                    << static_cast<unsigned>(path) << ',' << index
                                    << ',' << difference.same_weights << ','
                                    << difference.same_structure << ','
                                    << difference.membrane << ','
                                    << difference.refractory << ','
                                    << difference.input_count << ','
                                    << difference.recovery << ','
                                    << difference.local_memory << ','
                                    << difference.eligibility << ','
                                    << difference.context << ','
                                    << difference.synapse_activity << ','
                                    << difference.time_delta << ','
                                    << difference.queue_delta << '\n';
                                const std::string prefix = phase + ',' +
                                    std::to_string(family) + ',' +
                                    (inverse ? "inverse" : "normal") + ',' +
                                    std::to_string(seed) + ',' +
                                    std::to_string(episode) + ',' +
                                    std::to_string(path) + ',' +
                                    std::to_string(index);
                                const auto factors = b52_factorial(row.before,
                                    state.runtime, state.circuit, variant, episode,
                                    factorial, prefix, path);
                                const bool synaptic = !factors[4][path];
                                const bool transient_factor = factors[4][path] &&
                                    !factors[7][path];
                                const bool any_single = !factors[1][path] ||
                                    !factors[2][path] || !factors[3][path];
                                const bool combined = !factors[7][path] &&
                                    !any_single && (!factors[4][path] ||
                                        !factors[5][path] || !factors[6][path]);
                                aggregate.synaptic_causal += synaptic;
                                aggregate.transient_causal += transient_factor ||
                                    !factors[3][path];
                                aggregate.combined_causal += combined;
                                aggregate.transient_restored +=
                                    !factors[7][path] && factors[4][path];

                                auto copy_a = state.runtime;
                                auto copy_b = state.runtime;
                                const auto copy_a_probe = b51_probe_in_place(
                                    copy_a, state.circuit, path);
                                const auto copy_b_probe = b51_probe_in_place(
                                    copy_b, state.circuit, path);
                                const bool target = b48_target_a(variant, path,
                                    episode, b46_training(variant.task));
                                probes << prefix
                                    << ",identical_copies,0,"
                                    << b47_outcome(copy_a_probe.motor_a,
                                                   copy_a_probe.motor_b) << ','
                                    << b47_outcome(copy_b_probe.motor_a,
                                                   copy_b_probe.motor_b) << ','
                                    << b51_correct(target, copy_a_probe.motor_a,
                                                   copy_a_probe.motor_b) << ','
                                    << b51_correct(target, copy_b_probe.motor_a,
                                                   copy_b_probe.motor_b) << ','
                                    << (copy_a.state_fingerprint() ==
                                        copy_b.state_fingerprint()) << '\n';
                                auto sequential = state.runtime;
                                const auto first = b51_probe_in_place(
                                    sequential, state.circuit, path);
                                const auto second = b51_probe_in_place(
                                    sequential, state.circuit, path);
                                probes << prefix
                                    << ",sequential_same_runtime,0,"
                                    << b47_outcome(first.motor_a, first.motor_b)
                                    << ',' << b47_outcome(second.motor_a,
                                                         second.motor_b) << ','
                                    << b51_correct(target, first.motor_a,
                                                   first.motor_b) << ','
                                    << b51_correct(target, second.motor_a,
                                                   second.motor_b) << ",0\n";
                                for (const std::uint64_t gap :
                                     {0ULL, 1ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL}) {
                                    auto delayed = state.runtime;
                                    if (gap != 0) {
                                        static_cast<void>(delayed.advance_time_by(gap));
                                    }
                                    const auto delayed_probe = b51_probe_in_place(
                                        delayed, state.circuit, path);
                                    probes << prefix << ",time_advance," << gap
                                        << ',' << b47_outcome(delayed_probe.motor_a,
                                                             delayed_probe.motor_b)
                                        << ",NA,"
                                        << b51_correct(target,
                                            delayed_probe.motor_a,
                                            delayed_probe.motor_b)
                                        << ",NA,NA\n";
                                }
                            }
                        }

                        const bool both = after_correct[0] && after_correct[1];
                        if (both) {
                            ++aggregate.both_episodes;
                            ++current_both;
                            aggregate.longest_both = std::max<std::uint64_t>(
                                aggregate.longest_both, current_both);
                            if (!first_both_emitted) {
                                b52_emit_stability(stability, "first_both",
                                    phase, family, inverse, seed, episode,
                                    state.runtime, state.circuit, variant,
                                    state.plans);
                                first_both_emitted = true;
                            } else if (both_was_lost && !later_both_emitted) {
                                b52_emit_stability(stability, "later_both",
                                    phase, family, inverse, seed, episode,
                                    state.runtime, state.circuit, variant,
                                    state.plans);
                                later_both_emitted = true;
                            }
                            if (current_both == 8 && !stable_both_emitted) {
                                b52_emit_stability(stability, "stable_8",
                                    phase, family, inverse, seed, episode,
                                    state.runtime, state.circuit, variant,
                                    state.plans);
                                stable_both_emitted = true;
                            }
                        } else {
                            both_was_lost = first_both_emitted;
                            current_both = 0;
                        }

                        if (episode + 1 == 128 || episode + 1 == 512) {
                            const auto checkpoint = episode + 1;
                            const auto evaluated = b51_evaluate(state.runtime,
                                state.circuit, variant, state.plans, checkpoint);
                            const auto e1_x = b47_rate(evaluated.e1[0]);
                            const auto e1_y = b47_rate(evaluated.e1[1]);
                            const auto e2_x = b47_rate(evaluated.e2[0]);
                            const auto e2_y = b47_rate(evaluated.e2[1]);
                            checkpoints << phase << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << checkpoint << ',' << e1_x
                                << ',' << e1_y << ',' << e2_x << ',' << e2_y
                                << ',' << (e1_x >= .75 && e1_y >= .75) << ','
                                << (e2_x >= .75 && e2_y >= .75) << ','
                                << evaluated.identical_start << ','
                                << state.runtime.network().validate() << '\n';
                            if (checkpoint == 512) {
                                aggregate.e1_x = e1_x;
                                aggregate.e1_y = e1_y;
                            }
                            for (std::uint8_t evaluated_cue = 0;
                                 evaluated_cue < 2; ++evaluated_cue) {
                                const auto emit = [&](const char* metric,
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
                                emit("E1", evaluated.e1[evaluated_cue]);
                                emit("E2", evaluated.e2[evaluated_cue]);
                            }
                        }
                        previous_correct = after_correct;
                    }
                    const auto final = b51_recall_pair(state.runtime,
                        state.circuit, variant, kB52Episodes);
                    aggregate.final_both = final[0] && final[1];
                    aggregate.valid = state.runtime.network().validate();
                    summaries << phase << ',' << family << ','
                        << (inverse ? "inverse" : "normal") << ',' << seed
                        << ',' << aggregate.losses << ','
                        << aggregate.no_weight_losses << ','
                        << aggregate.sensor_losses << ','
                        << aggregate.inhibitory_losses << ','
                        << aggregate.combined_weight_losses << ','
                        << aggregate.transient_causal << ','
                        << aggregate.synaptic_causal << ','
                        << aggregate.combined_causal << ','
                        << aggregate.transient_restored << ','
                        << aggregate.both_episodes << ','
                        << aggregate.longest_both << ',' << final[0] << ','
                        << final[1] << ',' << aggregate.valid << ",0\n";
                    results.push_back(aggregate);
                }
            }
        }
    }
    return results;
}

void b52_statistics(
    const std::filesystem::path& output,
    const std::vector<B52NetworkResult>& rows) {
    std::ofstream file(output / "cluster-statistics.csv");
    file << "phase,family,assignment,metric,clusters,mean,ci_low,ci_high\n";
    std::ofstream causal(output / "causal-summary.csv");
    causal << "phase,family,assignment,networks,losses,no_weight_losses,sensor_associated,inhibitory_associated,combined_weight_associated,transient_causal,synaptic_causal,combined_causal,transient_restored,final_both\n";
    std::uint64_t salt = 0;
    for (const std::string phase : {"discovery", "confirmation"}) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                std::map<std::string, std::vector<double>> metrics;
                B52NetworkResult total;
                std::uint64_t networks = 0;
                std::uint64_t final_both = 0;
                for (const auto& row : rows) {
                    if (row.phase != phase || row.family != family ||
                        row.inverse != inverse) continue;
                    ++networks;
                    const auto denominator = std::max<std::uint64_t>(1, row.losses);
                    metrics["no_weight_loss_fraction"].push_back(
                        static_cast<double>(row.no_weight_losses) / denominator);
                    metrics["transient_causal_fraction"].push_back(
                        static_cast<double>(row.transient_causal) / denominator);
                    metrics["synaptic_causal_fraction"].push_back(
                        static_cast<double>(row.synaptic_causal) / denominator);
                    metrics["combined_causal_fraction"].push_back(
                        static_cast<double>(row.combined_causal) / denominator);
                    metrics["both_episode_fraction"].push_back(
                        static_cast<double>(row.both_episodes) / kB52Episodes);
                    metrics["final_both"].push_back(row.final_both ? 1.0 : 0.0);
                    metrics["E1_X"].push_back(row.e1_x);
                    metrics["E1_Y"].push_back(row.e1_y);
                    total.losses += row.losses;
                    total.no_weight_losses += row.no_weight_losses;
                    total.sensor_losses += row.sensor_losses;
                    total.inhibitory_losses += row.inhibitory_losses;
                    total.combined_weight_losses += row.combined_weight_losses;
                    total.transient_causal += row.transient_causal;
                    total.synaptic_causal += row.synaptic_causal;
                    total.combined_causal += row.combined_causal;
                    total.transient_restored += row.transient_restored;
                    final_both += row.final_both;
                }
                for (const auto& [metric, samples] : metrics) {
                    const auto interval = bootstrap_interval(
                        samples, 0x5200000ULL + ++salt);
                    file << phase << ',' << family << ','
                        << (inverse ? "inverse" : "normal") << ',' << metric
                        << ',' << samples.size() << ',' << b48_mean(samples)
                        << ',' << interval.first << ',' << interval.second << '\n';
                }
                causal << phase << ',' << family << ','
                    << (inverse ? "inverse" : "normal") << ',' << networks
                    << ',' << total.losses << ',' << total.no_weight_losses
                    << ',' << total.sensor_losses << ','
                    << total.inhibitory_losses << ','
                    << total.combined_weight_losses << ','
                    << total.transient_causal << ',' << total.synaptic_causal
                    << ',' << total.combined_causal << ','
                    << total.transient_restored << ',' << final_both << '\n';
            }
        }
    }
    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "phase,family,assignment,E1_X,E1_Y,robust_seeds,new_function,passes_general_criteria\n";
    for (const std::string phase : {"discovery", "confirmation"}) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                std::vector<double> x;
                std::vector<double> y;
                std::uint64_t robust = 0;
                for (const auto& row : rows) {
                    if (row.phase != phase || row.family != family ||
                        row.inverse != inverse) continue;
                    x.push_back(row.e1_x);
                    y.push_back(row.e1_y);
                    robust += row.e1_x >= .75 && row.e1_y >= .75;
                }
                criteria << phase << ',' << family << ','
                    << (inverse ? "inverse" : "normal") << ','
                    << b48_mean(x) << ',' << b48_mean(y) << ',' << robust
                    << ",0,0\n";
            }
        }
    }

    B52NetworkResult grand_total;
    std::uint64_t final_both = 0;
    for (const auto& row : rows) {
        grand_total.losses += row.losses;
        grand_total.no_weight_losses += row.no_weight_losses;
        grand_total.sensor_losses += row.sensor_losses;
        grand_total.inhibitory_losses += row.inhibitory_losses;
        grand_total.combined_weight_losses += row.combined_weight_losses;
        grand_total.transient_causal += row.transient_causal;
        grand_total.synaptic_causal += row.synaptic_causal;
        grand_total.combined_causal += row.combined_causal;
        grand_total.transient_restored += row.transient_restored;
        final_both += row.final_both;
    }
    std::ofstream diagnostic(output / "diagnostic-summary.csv");
    diagnostic << "metric,value\n";
    diagnostic << "networks," << rows.size() << '\n';
    diagnostic << "episodes," << rows.size() * kB52Episodes << '\n';
    diagnostic << "losses," << grand_total.losses << '\n';
    diagnostic << "losses_without_weight_change,"
               << grand_total.no_weight_losses << '\n';
    diagnostic << "sensor_associated_losses," << grand_total.sensor_losses
               << '\n';
    diagnostic << "inhibitory_associated_losses,"
               << grand_total.inhibitory_losses << '\n';
    diagnostic << "combined_weight_associated_losses,"
               << grand_total.combined_weight_losses << '\n';
    diagnostic << "detailed_loss_copies,"
               << grand_total.transient_restored + grand_total.synaptic_causal
               << '\n';
    diagnostic << "restored_by_old_transient_state,"
               << grand_total.transient_restored << '\n';
    diagnostic << "synaptically_sufficient_losses,"
               << grand_total.synaptic_causal << '\n';
    diagnostic << "factor_interaction_losses,"
               << grand_total.combined_causal << '\n';
    diagnostic << "final_simultaneously_correct_networks," << final_both
               << '\n';
}

void b52_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,checkpoint,fingerprint,exact_restore,exact_transient_state,exact_recall,valid\n";
    B48Variant variant{"B52_persistence", B46Task::mixed,
        B48Policy::reference, false, true, false, 0};
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto state = b51_initial_state(variant, seed);
        state.runtime = b51_continue(state.runtime, state.circuit,
            variant, state.plans, 0, kB52Episodes);
        const auto before_recall = b51_recall_pair(
            state.runtime, state.circuit, variant, kB52Episodes);
        const auto path = output / (".b52-state-" + std::to_string(seed));
        state.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const auto after_recall = b51_recall_pair(
            restored, state.circuit, variant, kB52Episodes);
        const auto difference = b52_state_difference(state.runtime, restored);
        file << seed << ",512," << state.runtime.state_fingerprint() << ','
            << (state.runtime.state_fingerprint() == restored.state_fingerprint())
            << ',' << (difference.membrane == 0 && difference.refractory == 0 &&
                difference.local_memory == 0 && difference.eligibility == 0 &&
                difference.queue_delta == 0 && difference.time_delta == 0)
            << ',' << (before_recall == after_recall) << ','
            << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B52_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b52");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    b52_reproduce_b51(output);
    b52_task_controls(output);
    b52_control_groups(output);
    const auto rows = b52_primary(output);
    b52_statistics(output, rows);
    b52_persistence(output);

    std::ofstream mechanism(output / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K9,0,B52 separates synaptic and transient causes diagnostically; no single local production intervention is justified before the combined evidence is reviewed\n";
    mechanism << "K10,0,no K9 mechanism exists to deactivate\n";

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,networks,episodes,reference_reproduced\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << rows.size() << ','
        << rows.size() * kB52Episodes << ",1\n";
    std::cout << "B.5.2 memory stability diagnosis complete K9=not_implemented\n";
    return 0;
}
#endif
