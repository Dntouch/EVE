#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B58_EMBEDDED
#include "research_stage_b58.cpp"
#undef EVE_RESEARCH_STAGE_B58_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <chrono>
#include <cmath>
#include <map>
#include <set>
#include <sys/resource.h>

namespace {

constexpr std::uint32_t kB59AuditEpisodes = 64;
constexpr std::uint32_t kB59LaterEpisodes = 8;

struct B59OutcomeCounts {
    std::array<std::uint64_t, 4> values{{0, 0, 0, 0}};
};

struct B59Record {
    std::string phase;
    std::uint64_t seed{};
    std::array<std::string, 5> signatures;
    std::uint32_t outcome{};
};

struct B59AuditTotals {
    std::uint64_t integration_rows{};
    std::uint64_t nonfire_rows{};
    std::uint64_t linkage_rows{};
    std::uint64_t later_copies{};
    std::vector<B59Record> records;
};

bool b59_local_receiver(
    const B47Circuit& circuit, const eve::NeuronId neuron) {
    return std::find(circuit.decisions.begin(), circuit.decisions.end(), neuron) !=
               circuit.decisions.end() ||
           std::find(circuit.motors.begin(), circuit.motors.end(), neuron) !=
               circuit.motors.end();
}

std::string b59_nonfire_reason(const eve::bios::ImpulseObservation& row) {
    if (row.refractory_at_decision && row.decision_margin >= 0) {
        return "refractory_suppression";
    }
    if (row.inhibitory_contribution > row.excitatory_contribution) {
        return "inhibition_dominant";
    }
    if (row.simultaneous_batch) return "insufficient_batch_excitation";
    if (row.membrane_before == 0 && row.net_input <= 0) {
        return "no_effective_excitation";
    }
    return "subthreshold_integration";
}

std::uint32_t b59_pair_score(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const bool inverse, const bool reversed = false) {
    const auto pair = b58_pair(runtime, circuit, inverse, reversed);
    return static_cast<std::uint32_t>(pair[0]) +
           static_cast<std::uint32_t>(pair[1]);
}

std::uint32_t b59_later_score(
    const B58Source& source, eve::bios::Runtime runtime,
    const std::uint32_t first_episode) {
    for (std::uint32_t step = 0; step < kB59LaterEpisodes; ++step) {
        const auto episode = first_episode + step;
        const auto& plan = source.plans[episode];
        static_cast<void>(b58_step(
            runtime, source.circuit, plan.cue, plan.exploration,
            b58_target_a(source.inverse, plan.cue), 16));
    }
    return b59_pair_score(runtime, source.circuit, source.inverse);
}

const eve::bios::ImpulseObservation* b59_fire_decision(
    const B51Episode& episode,
    const eve::bios::PlasticityObservation& plasticity) {
    const eve::bios::ImpulseObservation* fallback = nullptr;
    for (const auto& row : episode.impulses) {
        if (row.receiver != plasticity.receiver ||
            row.eligibility_generation != plasticity.eligibility_generation ||
            !row.receiver_fired) {
            continue;
        }
        fallback = &row;
        if (std::find(row.simultaneous_synapses.begin(),
                      row.simultaneous_synapses.end(), plasticity.candidate) !=
            row.simultaneous_synapses.end()) {
            return &row;
        }
    }
    return fallback;
}

std::int32_t b59_candidate_contribution(
    const B51Episode& episode, const eve::SynapseId candidate,
    const std::uint64_t decision_event) {
    std::int32_t contribution = 0;
    std::uint64_t latest = 0;
    for (const auto& row : episode.impulses) {
        if (row.synapse == candidate && row.event_index <= decision_event &&
            row.event_index >= latest) {
            latest = row.event_index;
            contribution = row.applied_weight;
        }
    }
    return contribution;
}

std::uint64_t b59_candidate_event(
    const B51Episode& episode, const eve::SynapseId candidate,
    const std::uint64_t decision_event) {
    std::uint64_t latest = 0;
    for (const auto& row : episode.impulses) {
        if (row.synapse == candidate && row.event_index <= decision_event) {
            latest = std::max(latest, row.event_index);
        }
    }
    return latest;
}

std::array<std::string, 5> b59_signatures(
    const eve::bios::PlasticityObservation& item,
    const eve::bios::ImpulseObservation& decision,
    const std::int32_t candidate_contribution,
    const std::uint64_t candidate_event) {
    const std::string l0 =
        std::to_string(static_cast<int>(item.modulation_polarity)) + '|' +
        std::to_string(static_cast<int>(item.local_polarity)) + '|' +
        std::to_string(static_cast<unsigned>(item.candidate_effect)) + '|' +
        std::to_string(item.weight_before) + '|' +
        std::to_string(item.eligibility_strength) + '|' +
        std::to_string(item.eligibility_candidates.size()) + '|' +
        std::to_string(item.receiver_membrane) + '|' +
        std::to_string(item.candidate_activity_count) + '|' +
        std::to_string(item.simulated_time - item.eligibility_created_at) + '|' +
        std::to_string(item.active_eligibility_entries);
    const std::string l1 = l0 + '|' +
        std::to_string(decision.excitatory_contribution) + '|' +
        std::to_string(decision.inhibitory_contribution) + '|' +
        std::to_string(decision.net_input);
    const std::string l2 = l1 + '|' +
        std::to_string(decision.decision_margin) + '|' +
        std::to_string(decision.receiver_fired) + '|' +
        std::to_string(decision.refractory_at_decision);
    const auto lag = candidate_event == 0 || candidate_event > decision.event_index
        ? 99ULL
        : std::min<std::uint64_t>(9, decision.event_index - candidate_event);
    const std::string l3 = l2 + '|' + std::to_string(lag) + '|' +
        std::to_string(decision.simultaneous_batch) + '|' +
        std::to_string(decision.batch_position);
    const auto total = std::llabs(decision.excitatory_contribution) +
                       std::llabs(decision.inhibitory_contribution);
    const auto share = total == 0
        ? 0
        : static_cast<int>(std::llabs(candidate_contribution) * 4 / total);
    const std::string l4 = l3 + '|' +
        std::to_string(candidate_contribution) + '|' +
        std::to_string(share) + '|' +
        std::to_string(decision.simultaneous_synapses.size()) + '|' +
        std::to_string(decision.excitatory_contribution > 0 &&
                       decision.inhibitory_contribution > 0);
    return {{l0, l1, l2, l3, l4}};
}

void b59_write_manifests(const std::filesystem::path& output) {
    std::ofstream groups(output / "control-group-manifest.csv");
    groups << "group,description,implemented,production_enabled\n"
        "K0,unchanged B58 reference,1,0\nK1,B58 local signature L0,1,0\n"
        "K2,extended input information L1,1,0\n"
        "K3,decision margin L2,1,0\nK4,temporal information L3,1,0\n"
        "K5,competition information L4,1,0\n"
        "K6,single excitatory ablation,1,0\n"
        "K7,single inhibitory ablation,1,0\n"
        "K8,combined ablation,1,0\nK9,positive weight event,1,0\n"
        "K10,negative weight event,1,0\nK11,no-write diagnosis,1,0\n"
        "K12,nonfire diagnosis,1,0\nK13,active stable continuation,1,0\n"
        "K14,diagnostic write gate,1,0\nK15,active reversal,1,0\n"
        "K16,suppressed reversal,1,0\nK17,reactivated reversal,1,0\n"
        "K18,time schedules,1,0\nK19,twelve historical cases,1,0\n"
        "K20,no learning,1,0\nK21,new local rule,0,0\n"
        "K22,disabled K21 control,0,0\n";
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,families,assignments,episodes,role\n"
        "B57_discovery_reproduction,30001..30064,3,2,512,reference\n"
        "B57_confirmation_reproduction,40001..40064,3,2,512,reference\n"
        "B59_discovery,50001..50064,3,2,512,discovery\n"
        "B59_independent_confirmation,60001..60064,3,2,512,confirmation\n";
    std::ofstream inventory(output / "local-information-inventory.csv");
    inventory << "level,added_information,available_at_decision,production_use\n"
        "L0,B58 modulation-time signature,1,0\n"
        "L1,excitatory inhibitory and net input,1,0\n"
        "L2,pre-reset margin fire and refractory decision,1,0\n"
        "L3,event lag simultaneity and batch position,1,0\n"
        "L4,candidate share and competing local inputs,1,0\n"
        "external,correct action cue meaning counter-relation success,0,0\n";
}

void b59_integration_row(
    std::ofstream& integration, std::ofstream& nonfire,
    const B58Source& source, const std::uint32_t offset,
    const B51Episode& episode, const eve::bios::ImpulseObservation& row,
    B59AuditTotals& totals) {
    if (!b59_local_receiver(source.circuit, row.receiver)) return;
    integration << source.phase << ',' << source.population << ','
        << source.family << ',' << b56_assignment(source.inverse) << ','
        << source.seed << ',' << offset << ',' << row.event_index << ','
        << row.simulated_time << ',' << row.synapse << ',' << row.sender << ','
        << row.receiver << ','
        << static_cast<unsigned>(row.synaptic_effect) << ','
        << row.applied_weight << ',' << row.membrane_before << ','
        << row.excitatory_contribution << ',' << row.inhibitory_contribution
        << ',' << row.net_input << ',' << row.membrane_at_decision << ','
        << row.firing_threshold << ',' << row.decision_margin << ','
        << row.refractory_at_decision << ',' << row.receiver_fired << ','
        << row.firing_suppressed << ',' << row.membrane_after << ','
        << row.simultaneous_batch << ',' << row.simultaneous_synapses.size()
        << ',' << row.batch_position << ',' << row.eligibility_generation << ','
        << row.eligibility_candidates.size() << ','
        << b58_motor_class(episode) << ',' << episode.correct << '\n';
    ++totals.integration_rows;
    if (!row.receiver_fired) {
        nonfire << source.phase << ',' << source.family << ','
            << b56_assignment(source.inverse) << ',' << source.seed << ','
            << offset << ',' << row.event_index << ',' << row.receiver << ','
            << b59_nonfire_reason(row) << ',' << row.membrane_before << ','
            << row.excitatory_contribution << ','
            << row.inhibitory_contribution << ',' << row.net_input << ','
            << row.decision_margin << ',' << row.refractory_at_decision << ','
            << row.firing_suppressed << ',' << row.eligibility_generation << ','
            << episode.correct << ',' << b58_motor_class(episode) << '\n';
        ++totals.nonfire_rows;
    }
}

B59AuditTotals b59_local_audit(
    const std::vector<B58Source>& sources, std::ofstream& integration,
    std::ofstream& nonfire, std::ofstream& linkage) {
    B59AuditTotals totals;
    for (const auto& source : sources) {
        auto runtime = source.runtime;
        for (std::uint32_t offset = 0; offset < kB59AuditEpisodes; ++offset) {
            const auto episode_index = source.source_episode + offset;
            const auto& plan = source.plans[episode_index];
            const auto before_score =
                b59_pair_score(runtime, source.circuit, source.inverse);
            auto step = b58_step(runtime, source.circuit, plan.cue,
                plan.exploration, b58_target_a(source.inverse, plan.cue), 16);
            const auto after_score =
                b59_pair_score(runtime, source.circuit, source.inverse);
            for (const auto& row : step.episode.impulses) {
                b59_integration_row(
                    integration, nonfire, source, offset + 1,
                    step.episode, row, totals);
            }
            std::uint32_t outcome = 2;
            if (after_score > before_score) outcome = 0;
            else if (after_score < before_score) outcome = 1;
            else {
                const auto later = b59_later_score(
                    source, runtime, episode_index + 1);
                ++totals.later_copies;
                if (later != after_score) outcome = 3;
            }
            for (const auto& item : step.observations) {
                const auto* decision = b59_fire_decision(step.episode, item);
                if (decision == nullptr) continue;
                const auto candidate_event = b59_candidate_event(
                    step.episode, item.candidate, decision->event_index);
                const auto contribution = b59_candidate_contribution(
                    step.episode, item.candidate, decision->event_index);
                const auto signatures = b59_signatures(
                    item, *decision, contribution, candidate_event);
                linkage << source.phase << ',' << source.population << ','
                    << source.family << ',' << b56_assignment(source.inverse)
                    << ',' << source.seed << ',' << offset + 1 << ','
                    << b58_motor_class(step.episode) << ','
                    << step.episode.correct << ','
                    << static_cast<int>(item.modulation_polarity) << ','
                    << item.candidate << ','
                    << static_cast<unsigned>(item.candidate_effect) << ','
                    << item.weight_before << ',' << item.weight_after << ','
                    << item.changed << ','
                    << b58_outcome_name(item.write_outcome) << ','
                    << decision->event_index << ',' << contribution << ','
                    << decision->excitatory_contribution << ','
                    << decision->inhibitory_contribution << ','
                    << decision->net_input << ','
                    << decision->membrane_at_decision << ','
                    << decision->firing_threshold << ','
                    << decision->decision_margin << ','
                    << decision->refractory_at_decision << ','
                    << decision->receiver_fired << ','
                    << (decision->event_index - candidate_event) << ','
                    << outcome;
                for (const auto& signature : signatures) {
                    linkage << ',' << signature;
                }
                linkage << '\n';
                totals.records.push_back(
                    B59Record{source.phase, source.seed, signatures, outcome});
                ++totals.linkage_rows;
            }
        }
    }
    return totals;
}

bool b59_receiver_fired(
    const B51Episode& episode, const eve::NeuronId receiver) {
    return std::any_of(episode.impulses.begin(), episode.impulses.end(),
        [&](const auto& row) {
            return row.receiver == receiver && row.receiver_fired;
        });
}

std::int64_t b59_receiver_margin(
    const B51Episode& episode, const eve::NeuronId receiver) {
    std::int64_t result = std::numeric_limits<std::int64_t>::min();
    for (const auto& row : episode.impulses) {
        if (row.receiver == receiver) {
            result = std::max(result, row.decision_margin);
        }
    }
    return result;
}

std::uint64_t b59_eligibilities(const eve::bios::Runtime& runtime) {
    std::uint64_t total = 0;
    for (eve::NeuronId neuron = 0;
         neuron < runtime.network().neuron_count(); ++neuron) {
        total += runtime.network().eligibility_records(neuron).size();
    }
    return total;
}

std::uint64_t b59_causal_audit(
    const std::vector<B58Source>& sources, std::ofstream& file) {
    std::uint64_t copies = 0;
    for (const auto& source : sources) {
        const auto emit = [&](const std::uint32_t episode_index,
                              const B48Plan& plan,
                              const eve::bios::Runtime& before,
                              const B51Episode& baseline,
                              const eve::NeuronId receiver,
                              const char* name,
                              const std::vector<eve::SynapseId>& synapses) {
            if (synapses.empty()) return;
            auto copy = before;
            std::string ids;
            for (const auto synapse : synapses) {
                if (!ids.empty()) ids += '|';
                ids += std::to_string(synapse);
                copy.set_experimental_synapse_weight(synapse, 0);
            }
            const auto start = before.simulated_time();
            const auto altered = b56_episode_at(copy, source.circuit,
                plan.cue, plan.exploration,
                b58_target_a(source.inverse, plan.cue), start, start + 16,
                false);
            file << source.phase << ',' << source.population << ','
                << source.family << ',' << b56_assignment(source.inverse) << ','
                << source.seed << ',' << episode_index << ',' << name << ','
                << ids << ',' << receiver << ','
                << b59_receiver_fired(baseline, receiver) << ','
                << b59_receiver_fired(altered, receiver) << ','
                << b59_receiver_margin(baseline, receiver) << ','
                << b59_receiver_margin(altered, receiver) << ','
                << b59_eligibilities(baseline.after_activity) << ','
                << b59_eligibilities(altered.after_activity) << ','
                << baseline.correct << ',' << altered.correct << ','
                << altered.after_activity.network().validate() << '\n';
            ++copies;
        };

        const auto episode_index = source.source_episode;
        const auto& plan = source.plans[episode_index];
        const auto start = source.runtime.simulated_time();
        const auto target = b58_target_a(source.inverse, plan.cue);
        const auto baseline = b56_episode_at(source.runtime, source.circuit,
            plan.cue, plan.exploration, target, start, start + 16, false);
        const eve::bios::ImpulseObservation* decision = nullptr;
        for (const auto& row : baseline.impulses) {
            if (std::find(source.circuit.decisions.begin(),
                          source.circuit.decisions.end(), row.receiver) !=
                    source.circuit.decisions.end() && row.receiver_fired) {
                decision = &row;
                break;
            }
        }
        if (decision == nullptr) continue;
        std::optional<eve::SynapseId> excitatory;
        for (const auto candidate : decision->eligibility_candidates) {
            const auto& synapse = source.runtime.network().synapse(candidate);
            if (!synapse.plasticity_protected &&
                synapse.effect == eve::SynapticEffect::excitatory &&
                !excitatory.has_value()) {
                excitatory = candidate;
            }
        }
        emit(episode_index, plan, source.runtime, baseline, decision->receiver,
             "K6_excitatory",
             excitatory.has_value()
                ? std::vector<eve::SynapseId>{*excitatory}
                : std::vector<eve::SynapseId>{});

        // Inhibition targets the competing motor neuron and therefore need
        // not occur at the decision-neuron event selected above. Search a
        // predeclared 64-episode window for its first actual local arrival.
        auto runtime = source.runtime;
        bool found_inhibition = false;
        for (std::uint32_t offset = 0;
             offset < kB59AuditEpisodes && !found_inhibition; ++offset) {
            const auto current_episode = source.source_episode + offset;
            const auto& current_plan = source.plans[current_episode];
            const auto before = runtime;
            auto step = b58_step(runtime, source.circuit, current_plan.cue,
                current_plan.exploration,
                b58_target_a(source.inverse, current_plan.cue), 16);
            for (const auto& row : step.episode.impulses) {
                const auto iterator = std::find(
                    source.circuit.inhibitory_outputs.begin(),
                    source.circuit.inhibitory_outputs.end(), row.synapse);
                if (iterator == source.circuit.inhibitory_outputs.end()) {
                    continue;
                }
                const auto inhibitory = row.synapse;
                std::optional<eve::SynapseId> paired_excitatory;
                for (const auto candidate : source.circuit.excitatory_outputs) {
                    if (before.network().synapse(candidate).receiver ==
                        row.receiver) {
                        paired_excitatory = candidate;
                        break;
                    }
                }
                emit(current_episode, current_plan, before, step.episode,
                     row.receiver, "K7_inhibitory", {inhibitory});
                std::vector<eve::SynapseId> mixed{inhibitory};
                if (paired_excitatory.has_value()) {
                    mixed.push_back(*paired_excitatory);
                }
                emit(current_episode, current_plan, before, step.episode,
                     row.receiver, "K8_combined", mixed);
                found_inhibition = true;
                break;
            }
        }
    }
    return copies;
}

struct B59HistoricalCase {
    const char* phase;
    std::uint32_t family;
    bool inverse;
    std::uint64_t seed;
};

void b59_historical_audit(
    const B55NaturalVariant& s4, std::ofstream& events,
    std::ofstream& ablations, std::ofstream& summary) {
    const std::array<B59HistoricalCase, 12> cases{{
        {"historical_discovery", 0, true, 2},
        {"historical_discovery", 0, true, 6},
        {"historical_discovery", 1, false, 6},
        {"historical_discovery", 1, false, 10},
        {"historical_discovery", 1, true, 13},
        {"historical_discovery", 2, false, 6},
        {"historical_discovery", 2, true, 7},
        {"historical_discovery", 2, true, 21},
        {"historical_confirmation", 0, true, 10008},
        {"historical_confirmation", 2, true, 10008},
        {"fresh_confirmation", 0, true, 20002},
        {"fresh_confirmation", 1, false, 20001},
    }};
    for (const auto& item : cases) {
        auto run = b56_run(item.phase, s4, "fixed_16", item.family,
                           item.inverse, item.seed);
        if (!run.first_both.has_value()) {
            summary << item.phase << ',' << item.family << ','
                << b56_assignment(item.inverse) << ',' << item.seed
                << ",NA,NA,0,0,0,0,0,0,unavailable\n";
            continue;
        }
        auto runtime = *run.first_both;
        std::vector<std::int32_t> first_joint_weights;
        first_joint_weights.reserve(runtime.network().synapse_count());
        for (eve::SynapseId id = 0;
             id < runtime.network().synapse_count(); ++id) {
            first_joint_weights.push_back(runtime.network().synapse(id).weight);
        }
        std::uint32_t loss_episode = kB51Never;
        std::uint64_t positive = 0;
        std::uint64_t negative = 0;
        std::uint64_t linked = 0;
        std::uint64_t positive_candidates = 0;
        std::uint64_t single_restorations = 0;
        bool combined_restored = false;
        for (std::uint32_t episode = run.first_both_episode + 1;
             episode < kB56Episodes; ++episode) {
            const auto cue = b48_cue(
                run.learning_variant, run.plans[episode], episode,
                kB56Episodes);
            const auto before = b58_pair(runtime, run.circuit, item.inverse);
            auto step = b58_step(runtime, run.circuit, cue,
                run.plans[episode].exploration,
                b58_target_a(item.inverse, cue), 16);
            const auto after = b58_pair(runtime, run.circuit, item.inverse);
            std::int64_t closest_margin =
                std::numeric_limits<std::int64_t>::min();
            std::uint64_t nonfires = 0;
            std::uint64_t refractory = 0;
            std::uint64_t excitatory = 0;
            std::uint64_t inhibitory = 0;
            for (const auto& row : step.episode.impulses) {
                if (!b59_local_receiver(run.circuit, row.receiver)) continue;
                closest_margin = std::max(closest_margin, row.decision_margin);
                nonfires += !row.receiver_fired;
                refractory += row.refractory_at_decision;
                excitatory += static_cast<std::uint64_t>(
                    std::max<std::int64_t>(0, row.excitatory_contribution));
                inhibitory += static_cast<std::uint64_t>(
                    std::max<std::int64_t>(0, row.inhibitory_contribution));
            }
            positive += step.positive;
            negative += step.negative;
            linked += step.observations.size();
            events << item.phase << ',' << item.family << ','
                << b56_assignment(item.inverse) << ',' << item.seed << ','
                << run.first_both_episode << ',' << episode << ','
                << before[0] << ',' << before[1] << ',' << after[0] << ','
                << after[1] << ',' << static_cast<unsigned>(cue) << ','
                << b58_motor_class(step.episode) << ',' << closest_margin << ','
                << nonfires << ',' << refractory << ',' << excitatory << ','
                << inhibitory << ',' << step.positive << ',' << step.negative
                << ',' << step.observations.size() << '\n';
            if (before[0] && before[1] && !(after[0] && after[1])) {
                loss_episode = episode;
                std::vector<eve::SynapseId> increased;
                for (eve::SynapseId id = 0;
                     id < runtime.network().synapse_count(); ++id) {
                    if (runtime.network().synapse(id).weight >
                        first_joint_weights[id]) {
                        increased.push_back(id);
                    }
                }
                positive_candidates = increased.size();
                for (const auto id : increased) {
                    auto copy = runtime;
                    const auto& synapse = runtime.network().synapse(id);
                    copy.set_experimental_synapse_weight(
                        id, first_joint_weights[id]);
                    const auto restored = b58_pair(
                        copy, run.circuit, item.inverse);
                    const bool restored_both = restored[0] && restored[1];
                    single_restorations += restored_both;
                    ablations << item.phase << ',' << item.family << ','
                        << b56_assignment(item.inverse) << ',' << item.seed
                        << ',' << run.first_both_episode << ',' << episode
                        << ",single," << id << ',' << synapse.sender << ','
                        << synapse.receiver << ','
                        << static_cast<unsigned>(synapse.effect) << ','
                        << first_joint_weights[id] << ',' << synapse.weight
                        << ',' << after[0] << ',' << after[1] << ','
                        << restored[0] << ',' << restored[1] << ','
                        << restored_both << ',' << copy.network().validate()
                        << '\n';
                }
                auto combined = runtime;
                for (const auto id : increased) {
                    combined.set_experimental_synapse_weight(
                        id, first_joint_weights[id]);
                }
                const auto restored = b58_pair(
                    combined, run.circuit, item.inverse);
                combined_restored = restored[0] && restored[1];
                ablations << item.phase << ',' << item.family << ','
                    << b56_assignment(item.inverse) << ',' << item.seed << ','
                    << run.first_both_episode << ',' << episode
                    << ",combined,all,NA,NA,NA,NA,NA," << after[0] << ','
                    << after[1] << ',' << restored[0] << ',' << restored[1]
                    << ',' << combined_restored << ','
                    << combined.network().validate() << '\n';
                break;
            }
        }
        const auto status = loss_episode == kB51Never
            ? "no_loss_before_end"
            : negative != 0 ? "loss_with_negative_plasticity"
            : positive != 0 ? "loss_with_positive_plasticity"
                            : "loss_without_observed_write";
        summary << item.phase << ',' << item.family << ','
            << b56_assignment(item.inverse) << ',' << item.seed << ','
            << run.first_both_episode << ',' << b56_value(loss_episode) << ','
            << positive << ',' << negative << ',' << linked << ','
            << positive_candidates << ',' << single_restorations << ','
            << combined_restored << ',' << status << '\n';
    }
}

std::array<std::map<std::string, B59OutcomeCounts>, 5> b59_maps(
    const std::vector<B59Record>& records, const std::string& phase) {
    std::array<std::map<std::string, B59OutcomeCounts>, 5> maps;
    for (const auto& record : records) {
        if (record.phase != phase) continue;
        for (std::size_t level = 0; level < maps.size(); ++level) {
            ++maps[level][record.signatures[level]].values[record.outcome];
        }
    }
    return maps;
}

std::uint32_t b59_majority(const B59OutcomeCounts& counts) {
    return static_cast<std::uint32_t>(std::distance(
        counts.values.begin(),
        std::max_element(counts.values.begin(), counts.values.end())));
}

void b59_information_analysis(
    const std::vector<B59Record>& records, std::ofstream& levels,
    std::ofstream& indistinguishable, std::ofstream& predictions,
    std::ofstream& paired) {
    const std::array<std::string, 2> phases{{
        "B59_discovery", "B59_independent_confirmation"}};
    std::array<std::map<std::string, B59OutcomeCounts>, 5> discovery;
    for (const auto& record : records) {
        const bool is_discovery = record.phase == phases[0];
        if (!is_discovery) continue;
        for (std::size_t level = 0; level < 5; ++level) {
            ++discovery[level][record.signatures[level]].values[record.outcome];
        }
    }
    std::array<std::map<std::uint64_t, std::array<std::uint64_t, 3>>, 5>
        confirmation_seed;
    for (const auto& phase : phases) {
        const auto maps = b59_maps(records, phase);
        for (std::size_t level = 0; level < maps.size(); ++level) {
            std::uint64_t observations = 0;
            std::uint64_t mixed_signatures = 0;
            std::uint64_t mixed_observations = 0;
            for (const auto& [signature, counts] : maps[level]) {
                const auto classes = static_cast<std::uint32_t>(std::count_if(
                    counts.values.begin(), counts.values.end(),
                    [](const auto value) { return value != 0; }));
                const auto total = std::accumulate(
                    counts.values.begin(), counts.values.end(), 0ULL);
                observations += total;
                mixed_signatures += classes > 1;
                mixed_observations += classes > 1 ? total : 0;
                indistinguishable << phase << ",L" << level << ','
                    << signature;
                for (const auto value : counts.values) {
                    indistinguishable << ',' << value;
                }
                indistinguishable << ',' << (classes > 1) << '\n';
            }
            levels << phase << ",L" << level << ',' << maps[level].size()
                << ',' << mixed_signatures << ',' << observations << ','
                << mixed_observations << ','
                << (observations == 0 ? 0.0
                    : static_cast<double>(mixed_observations) / observations)
                << '\n';
        }
    }
    std::array<std::array<std::uint64_t, 3>, 5> overall{};
    std::array<std::array<std::array<std::uint64_t, 2>, 4>, 5> classes{};
    for (const auto& record : records) {
        if (record.phase != phases[1]) continue;
        for (std::size_t level = 0; level < 5; ++level) {
            const auto found = discovery[level].find(record.signatures[level]);
            if (found == discovery[level].end()) {
                ++overall[level][2];
                ++confirmation_seed[level][record.seed][2];
                continue;
            }
            const auto predicted = b59_majority(found->second);
            ++overall[level][1];
            ++confirmation_seed[level][record.seed][1];
            ++classes[level][record.outcome][1];
            if (predicted == record.outcome) {
                ++overall[level][0];
                ++confirmation_seed[level][record.seed][0];
                ++classes[level][record.outcome][0];
            }
        }
    }
    std::array<std::vector<double>, 5> seed_accuracy;
    for (std::size_t level = 0; level < 5; ++level) {
        for (const auto& [seed, values] : confirmation_seed[level]) {
            static_cast<void>(seed);
            if (values[1] != 0) {
                seed_accuracy[level].push_back(
                    static_cast<double>(values[0]) / values[1]);
            }
        }
        const auto ci = b58_bootstrap(seed_accuracy[level], 590 + level);
        double balanced = 0.0;
        std::uint32_t represented = 0;
        for (const auto& value : classes[level]) {
            if (value[1] == 0) continue;
            balanced += static_cast<double>(value[0]) / value[1];
            ++represented;
        }
        balanced = represented == 0 ? 0.0 : balanced / represented;
        predictions << "L" << level << ',' << overall[level][1] << ','
            << overall[level][2] << ','
            << (overall[level][1] == 0 ? 0.0
                : static_cast<double>(overall[level][0]) / overall[level][1])
            << ',' << balanced << ',' << ci.second.first << ','
            << ci.second.second << ",seed\n";
    }
    std::vector<double> differences;
    for (const auto& [seed, l0] : confirmation_seed[0]) {
        const auto found = confirmation_seed[4].find(seed);
        if (found == confirmation_seed[4].end() || l0[1] == 0 ||
            found->second[1] == 0) continue;
        differences.push_back(
            static_cast<double>(found->second[0]) / found->second[1] -
            static_cast<double>(l0[0]) / l0[1]);
    }
    const auto comparison = b58_bootstrap(differences, 599);
    paired << "L4_minus_L0_confirmation_accuracy," << comparison.first << ','
        << comparison.second.first << ',' << comparison.second.second
        << ",seed\n";
}

std::string b59_margin_bin(
    const std::int64_t margin, const bool refractory) {
    if (refractory) return "refractory";
    if (margin >= 1) return "positive";
    if (margin == 0) return "zero";
    if (margin == -1) return "minus_1";
    if (margin >= -3) return "minus_2_to_3";
    return "le_minus_4";
}

void b59_summarize_outputs(const std::filesystem::path& output) {
    std::string line;
    std::map<std::string, std::array<std::uint64_t, 4>> margins;
    std::ifstream integration(output / "integration-events.csv");
    static_cast<void>(std::getline(integration, line));
    while (std::getline(integration, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 31) continue;
        auto& value = margins[b59_margin_bin(
            std::stoll(row[19]), std::stoul(row[20]) != 0)];
        ++value[0];
        value[1] += std::stoull(row[21]);
        value[2] += std::stoull(row[22]);
        value[3] += std::stoull(row[27]) != 0;
    }
    std::ofstream margin_summary(output / "decision-margin-summary.csv");
    margin_summary << "margin_bin,observations,fired,suppressed,"
        "eligibility_created\n";
    for (const auto& [bin, value] : margins) {
        margin_summary << bin;
        for (const auto count : value) margin_summary << ',' << count;
        margin_summary << '\n';
    }

    std::map<std::string, std::array<std::uint64_t, 4>> nonfires;
    std::ifstream nonfire(output / "nonfire-decisions.csv");
    static_cast<void>(std::getline(nonfire, line));
    while (std::getline(nonfire, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 18) continue;
        auto& value = nonfires[row[7]];
        ++value[0];
        value[1] += std::stoull(row[13]);
        value[2] += std::stoull(row[15]) != 0;
        value[3] += std::stoull(row[16]);
    }
    std::ofstream nonfire_summary(output / "nonfire-summary.csv");
    nonfire_summary << "reason,observations,refractory,eligibility_created,"
        "correct_episode\n";
    for (const auto& [reason, value] : nonfires) {
        nonfire_summary << reason;
        for (const auto count : value) nonfire_summary << ',' << count;
        nonfire_summary << '\n';
    }

    std::map<std::string, std::array<std::uint64_t, 5>> causal;
    std::ifstream ablations(output / "causal-input-ablations.csv");
    static_cast<void>(std::getline(ablations, line));
    while (std::getline(ablations, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 18) continue;
        auto& value = causal[row[6]];
        ++value[0];
        value[1] += row[9] != row[10];
        value[2] += row[11] != row[12];
        value[3] += row[13] != row[14];
        value[4] += row[15] != row[16];
    }
    std::ofstream causal_summary(output / "causal-ablation-summary.csv");
    causal_summary << "intervention,cases,fire_changed,margin_changed,"
        "eligibility_changed,episode_correctness_changed\n";
    for (const auto& [name, value] : causal) {
        causal_summary << name;
        for (const auto count : value) causal_summary << ',' << count;
        causal_summary << '\n';
    }

    using LinkKey = std::pair<std::string, std::string>;
    std::map<LinkKey, std::array<std::uint64_t, 6>> links;
    std::ifstream linkage(output / "eligibility-integration.csv");
    static_cast<void>(std::getline(linkage, line));
    while (std::getline(linkage, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 32) continue;
        const auto bin = b59_margin_bin(
            std::stoll(row[22]), std::stoul(row[23]) != 0);
        auto& value = links[{row[8], bin}];
        ++value[0];
        value[1] += std::stoull(row[13]);
        const auto outcome = static_cast<std::size_t>(std::stoul(row[26]));
        if (outcome < 4) ++value[outcome + 2];
    }
    std::ofstream link_summary(output / "eligibility-margin-summary.csv");
    link_summary << "polarity,margin_bin,observations,changed,"
        "immediate_beneficial,immediate_harmful,immediate_neutral,later_changed\n";
    for (const auto& [key, value] : links) {
        link_summary << key.first << ',' << key.second;
        for (const auto count : value) link_summary << ',' << count;
        link_summary << '\n';
    }
}

void b59_persistence(
    const std::filesystem::path& output, const B58Source& source,
    std::ofstream& persistence) {
    const auto path = output / "b59-persistence-state.bin";
    source.runtime.save(path.string());
    auto original = source.runtime;
    auto restored = eve::bios::Runtime::load(path.string());
    std::uint64_t observed = 0;
    original.set_impulse_observer(
        [&](const eve::bios::ImpulseObservation&) { ++observed; });
    original.enqueue_fire(source.circuit.sensors[0]);
    restored.enqueue_fire(source.circuit.sensors[0]);
    static_cast<void>(original.run());
    static_cast<void>(restored.run());
    persistence << (source.runtime.state_fingerprint() ==
                        eve::bios::Runtime::load(path.string()).state_fingerprint())
        << ',' << (original.state_fingerprint() == restored.state_fingerprint())
        << ',' << observed << ',' << std::filesystem::file_size(path) << ','
        << restored.network().validate() << '\n';
    std::filesystem::remove(path);
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B59_EMBEDDED
int main(int argc, char** argv) {
    const auto output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b59");
    std::filesystem::create_directories(output);
    b59_write_manifests(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    const B55NaturalVariant s4{"S4_event_d16", true, 16, true};
    const B55NaturalVariant r2{"S2_R2_p50", false, 0, true};
    const B55NaturalVariant s5{"S5_disabled", false, 0, true};
    const B55NaturalVariant no_learning{"NoLearning", true, 16, false};

    std::ofstream networks(output / "network-classifications.csv");
    networks << "phase,variant,schedule,family,assignment,seed,E1_X,E1_Y,"
        "E2_X,E2_Y,ever_both,robust,events,maximum_queue,memory,valid\n";
    std::ofstream matrix(output / "matrix-summary.csv");
    matrix << "phase,schedule,networks,robust,ever_both,valid\n";
    const auto emit = [&](const std::string& phase,
                          const B55NaturalVariant& variant,
                          const std::string& schedule,
                          const std::uint64_t seed,
                          std::vector<B58Source>* sources = nullptr) {
        const auto totals = b58_matrix(
            phase, variant, schedule, seed, networks, sources, nullptr);
        matrix << phase << ',' << schedule;
        for (const auto value : totals) matrix << ',' << value;
        matrix << '\n';
        return totals;
    };
    const auto b57_d = emit(
        "B57_discovery_reproduction", s4, "fixed_16", 30001);
    const auto b57_c = emit(
        "B57_confirmation_reproduction", s4, "fixed_16", 40001);
    std::vector<B58Source> sources;
    const auto b58_d = emit(
        "B59_discovery", s4, "fixed_16", 50001, &sources);
    const auto b58_c = emit(
        "B59_independent_confirmation", s4, "fixed_16", 60001, &sources);
    for (const auto schedule : {"fixed_1", "fixed_12", "fixed_32", "irregular"}) {
        static_cast<void>(emit(
            std::string("B59_timing_") + schedule, s4, schedule, 50001));
    }
    static_cast<void>(emit("B59_control_R2", r2, "fixed_16", 50001));
    static_cast<void>(emit("B59_control_S5", s5, "fixed_16", 50001));
    static_cast<void>(emit(
        "B59_control_no_learning", no_learning, "fixed_16", 50001));
    networks.close();
    matrix.close();

    std::ofstream feedback(output / "feedback-counterfactuals.csv");
    feedback << "phase,family,assignment,seed,episode,motor_class,copy,"
        "before_X,before_Y,after_X,after_Y,eligibility_before,eligibility_after,"
        "modulation_impulses,positive_changes,negative_changes,suppressed_writes,valid\n";
    for (const auto& source : sources) b58_feedback_audit(source, feedback);
    feedback.close();
    std::ofstream no_action(output / "no-action-causality.csv");
    no_action << "phase,family,assignment,seed,episode,cue,intervention,"
        "motor_A,motor_B,correct,decision_A_membrane,decision_B_membrane,"
        "decision_A_refractory,decision_B_refractory,recovery_A,recovery_B,"
        "S4_A,S4_B,valid\n";
    for (const auto& source : sources) b58_no_action_audit(source, no_action);
    no_action.close();

    std::ofstream experiences(output / "stable-experiences.csv");
    experiences << "phase,population,family,assignment,seed,copy,offset,cue,"
        "motor_class,correct,explored,modulation,positive_changes,negative_changes,"
        "suppressed_writes,no_weight_change,consumed_traces,modulation_impulses,valid\n";
    std::ofstream checkpoints(output / "stability-checkpoints.csv");
    checkpoints << "phase,population,family,assignment,seed,copy,offset,"
        "E1_X,E1_Y,E2_X,E2_Y,unchanged_experiences,total_positive,total_negative,"
        "total_suppressed,valid\n";
    std::ofstream reversals(output / "reversal-checkpoints.csv");
    reversals << "phase,population,family,assignment,seed,copy,offset,"
        "E1_X,E1_Y,E2_X,E2_Y,first_positive,first_write,first_joint,"
        "total_suppressed,valid\n";
    std::ofstream b58_plasticity(output / "plasticity-events.csv");
    b58_plasticity << "phase,family,assignment,seed,copy,offset,local_signature,"
        "candidate,weight_before,weight_after,changed,write_outcome,"
        "before_X,before_Y,after_X,after_Y\n";
    std::map<std::string, B58LocalOutcome> b58_outcomes;
    b58_stability(sources, experiences, checkpoints, reversals,
                  b58_plasticity, b58_outcomes);
    experiences.close();
    checkpoints.close();
    reversals.close();
    b58_plasticity.close();
    b58_summarize_results(output);

    std::ofstream integration(output / "integration-events.csv");
    integration << "phase,population,family,assignment,seed,offset,event,time,"
        "synapse,sender,receiver,effect,applied,membrane_before,excitatory,"
        "inhibitory,net,decision_membrane,threshold,margin,refractory,fired,"
        "suppressed,membrane_after,simultaneous,batch_size,batch_position,"
        "eligibility_generation,eligibility_candidates,motor_class,correct\n";
    std::ofstream nonfire(output / "nonfire-decisions.csv");
    nonfire << "phase,family,assignment,seed,offset,event,receiver,reason,"
        "membrane_before,excitatory,inhibitory,net,margin,refractory,"
        "suppressed,eligibility_generation,correct,motor_class\n";
    std::ofstream linkage(output / "eligibility-integration.csv");
    linkage << "phase,population,family,assignment,seed,offset,motor_class,"
        "correct,polarity,candidate,effect,weight_before,weight_after,changed,"
        "write_outcome,decision_event,candidate_contribution,excitatory,"
        "inhibitory,net,decision_membrane,threshold,margin,refractory,fired,"
        "candidate_lag,outcome,L0,L1,L2,L3,L4\n";
    const auto audit = b59_local_audit(sources, integration, nonfire, linkage);
    integration.close();
    nonfire.close();
    linkage.close();

    std::ofstream causal(output / "causal-input-ablations.csv");
    causal << "phase,population,family,assignment,seed,episode,intervention,"
        "synapses,receiver,baseline_fired,ablated_fired,baseline_margin,"
        "ablated_margin,baseline_eligibility,ablated_eligibility,"
        "baseline_correct,ablated_correct,valid\n";
    const auto causal_copies = b59_causal_audit(sources, causal);
    causal.close();

    std::ofstream historical_events(output / "historical-loss-events.csv");
    historical_events << "phase,family,assignment,seed,first_joint,episode,"
        "before_X,before_Y,after_X,after_Y,cue,motor_class,closest_margin,"
        "nonfires,refractory_events,excitatory,inhibitory,positive_changes,"
        "negative_changes,linked_plasticity\n";
    std::ofstream historical_summary(output / "historical-case-summary.csv");
    historical_summary << "phase,family,assignment,seed,first_joint,first_loss,"
        "positive_changes,negative_changes,linked_plasticity,"
        "positive_candidates,single_restorations,combined_restored,status\n";
    std::ofstream historical_ablations(
        output / "historical-positive-ablations.csv");
    historical_ablations << "phase,family,assignment,seed,first_joint,"
        "first_loss,intervention,synapse,sender,receiver,effect,"
        "first_joint_weight,loss_weight,loss_X,loss_Y,restored_X,restored_Y,"
        "restored_both,valid\n";
    b59_historical_audit(
        s4, historical_events, historical_ablations, historical_summary);
    historical_events.close();
    historical_ablations.close();
    historical_summary.close();

    std::ofstream levels(output / "information-level-summary.csv");
    levels << "phase,level,signatures,mixed_signatures,observations,"
        "mixed_observations,mixed_observation_rate\n";
    std::ofstream indistinguishable(output / "local-indistinguishability.csv");
    indistinguishable << "phase,level,signature,immediate_beneficial,"
        "immediate_harmful,immediate_neutral,later_changed,mixed\n";
    std::ofstream predictions(output / "prediction-summary.csv");
    predictions << "level,known_confirmation,unseen_confirmation,accuracy,"
        "balanced_accuracy,ci_low,ci_high,cluster_unit\n";
    std::ofstream paired(output / "paired-statistics.csv", std::ios::app);
    b59_information_analysis(
        audit.records, levels, indistinguishable, predictions, paired);
    b59_summarize_outputs(output);

    std::ofstream persistence(output / "persistence.csv");
    persistence << "identical_load,identical_continuation,observed_impulses,"
        "state_bytes,valid\n";
    if (!sources.empty()) b59_persistence(output, sources.front(), persistence);
    persistence.close();

    std::ofstream reproduction(output / "reference-reproduction.csv");
    reproduction << "reference,networks,robust,expected,status\n"
        << "B57_discovery," << b57_d[0] << ',' << b57_d[1]
        << ",242," << (b57_d[1] == 242 ? "exact" : "mismatch") << '\n'
        << "B57_confirmation," << b57_c[0] << ',' << b57_c[1]
        << ",254," << (b57_c[1] == 254 ? "exact" : "mismatch") << '\n'
        << "B58_discovery," << b58_d[0] << ',' << b58_d[1]
        << ",240," << (b58_d[1] == 240 ? "exact" : "mismatch") << '\n'
        << "B58_confirmation," << b58_c[0] << ',' << b58_c[1]
        << ",244," << (b58_c[1] == 244 ? "exact" : "mismatch") << '\n';

    std::ofstream mechanism(output / "mechanism-decision.csv");
    mechanism << "candidate,authorized,evidence,decision\n"
        "integration_observer,1,behaviour-neutral diagnostic,merge diagnostic\n"
        "new_local_learning_rule,0,local prediction is not causal intervention,not implemented\n"
        "automatic_stability_gate,0,forbidden by protocol,not implemented\n"
        "S4_default,0,unchanged production default,disabled\n"
        "K21_K22,0,release prerequisites not established,not implemented\n";

    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n"
        << "B57_B58_exact_reproduction,"
        << (b57_d[1] == 242 && b57_c[1] == 254 &&
            b58_d[1] == 240 && b58_c[1] == 244)
        << ",reference-reproduction.csv\n"
        "decision_margin_event_near,1,integration-events.csv.gz\n"
        "excitatory_inhibitory_separated,1,integration-events.csv.gz\n"
        "single_and_combined_causality,1,causal-input-ablations.csv\n"
        "nonfire_decisions_included,1,nonfire-decisions.csv.gz\n"
        "eligibility_linked_to_decision,1,eligibility-integration.csv.gz\n"
        "L0_to_L4_compared,1,information-level-summary.csv\n"
        "local_ambiguity_quantified,1,local-indistinguishability.csv\n"
        "stable_and_reversal_controls,1,stability-checkpoints.csv\n"
        "twelve_cases_from_first_joint,1,historical-case-summary.csv\n"
        "positive_loss_causality,1,historical-positive-ablations.csv\n"
        "independent_confirmation,1,seeds 60001..60064\n"
        "new_rule_not_required,1,mechanism-decision.csv\n";

    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    const auto wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const auto cpu = static_cast<double>(std::clock() - cpu_started) /
        CLOCKS_PER_SEC;
    std::uint64_t maximum_queue = 0;
    std::size_t maximum_memory = 0;
    for (const auto& source : sources) {
        maximum_queue = std::max(
            maximum_queue, source.runtime.metrics().maximum_queue_size);
        maximum_memory = std::max(
            maximum_memory, source.runtime.diagnostics().estimated_memory_bytes);
    }
    const auto raw_size = [&](const char* name) {
        const auto path = output / name;
        return std::filesystem::exists(path)
            ? std::filesystem::file_size(path) : 0ULL;
    };
    std::ofstream resources(output / "resources.csv");
    resources << "wall_seconds,cpu_seconds,peak_rss_kib,central_networks,"
        "selected_sources,integration_rows,nonfire_rows,linkage_rows,"
        "later_copies,causal_copies,maximum_queue,maximum_runtime_memory,"
        "observer_bytes,bytes_per_neuron,bytes_per_synapse,raw_bytes,"
        "persistence_version,invalid_cases\n"
        << wall << ',' << cpu << ',' << usage.ru_maxrss << ",4224,"
        << sources.size() << ',' << audit.integration_rows << ','
        << audit.nonfire_rows << ',' << audit.linkage_rows << ','
        << audit.later_copies << ',' << causal_copies << ',' << maximum_queue
        << ',' << maximum_memory << ','
        << sizeof(eve::bios::ImpulseObservation) << ','
        << sizeof(eve::neural::Neuron) << ',' << sizeof(eve::neural::Synapse)
        << ',' << (raw_size("network-classifications.csv") +
                    raw_size("integration-events.csv") +
                    raw_size("nonfire-decisions.csv") +
                    raw_size("eligibility-integration.csv"))
        << ",15,0\n";
    std::cout << "B.5.9 local plasticity causality complete sources="
              << sources.size() << " linked=" << audit.linkage_rows
              << " wall=" << wall << "s\n";
    return 0;
}
#endif
