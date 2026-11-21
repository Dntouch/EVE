#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B59_EMBEDDED
#include "research_stage_b59.cpp"
#undef EVE_RESEARCH_STAGE_B59_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <chrono>
#include <map>
#include <set>
#include <sys/resource.h>

namespace {

constexpr std::array<std::uint32_t, 7> kB510Checkpoints{{1, 4, 8, 16, 32, 64, 128}};

struct B510Target {
    eve::SynapseId synapse{};
    std::uint64_t generation{};
    std::int32_t before{};
    std::int32_t after{};
};

struct B510Run {
    B58Step step;
    std::uint32_t suppressed{};
};

struct B510Info {
    std::string phase;
    std::uint64_t seed{};
    std::array<std::string, 6> signatures;
    std::string outcome;
};

bool b510_matches(const eve::bios::PlasticityObservation& row,
                  const B510Target& target) {
    return row.candidate == target.synapse &&
           row.eligibility_generation == target.generation &&
           row.weight_before == target.before &&
           row.proposed_weight == target.after;
}

B510Run b510_step(eve::bios::Runtime& runtime, const B58Source& source,
                  const std::uint32_t episode, const bool reversed,
                  const std::vector<B510Target>& targets,
                  const bool global_gate = false) {
    std::uint32_t suppressed = 0;
    runtime.set_experimental_plasticity_write_interceptor(
        [&](const eve::bios::PlasticityObservation& row) {
            for (const auto& target : targets) {
                if (b510_matches(row, target)) {
                    ++suppressed;
                    return true;
                }
            }
            return false;
        });
    const auto& plan = source.plans[episode];
    auto step = b58_step(runtime, source.circuit, plan.cue, plan.exploration,
        b58_target_a(source.inverse, plan.cue, reversed), 16,
        B58Feedback::natural,
        global_gate ? B58Gate::suppressed : B58Gate::active);
    runtime.set_experimental_plasticity_write_interceptor({});
    return {std::move(step), suppressed};
}

B510Run b510_feedback(eve::bios::Runtime& runtime, B51Episode episode,
                      const std::uint64_t end_time,
                      const std::vector<B510Target>& targets,
                      const bool global_gate = false) {
    std::uint32_t suppressed = 0;
    std::vector<eve::bios::PlasticityObservation> observations;
    runtime.set_plasticity_observer(
        [&](const auto& row) { observations.push_back(row); });
    runtime.set_experimental_plasticity_write_interceptor(
        [&](const eve::bios::PlasticityObservation& row) {
            for (const auto& target : targets) {
                if (b510_matches(row, target)) {
                    ++suppressed;
                    return true;
                }
            }
            return false;
        });
    const auto before = runtime.metrics();
    runtime.set_experimental_plasticity_writes_suppressed(global_gate);
    episode.feedback = runtime.process_modulatory_feedback(episode.modulation);
    runtime.set_plasticity_observer({});
    runtime.set_experimental_plasticity_write_interceptor({});
    runtime.set_experimental_plasticity_writes_suppressed(false);
    if (runtime.simulated_time() < end_time) {
        static_cast<void>(runtime.advance_time_to(end_time));
    }
    const auto after = runtime.metrics();
    return {{std::move(episode), std::move(observations),
        after.plasticity_positive_changes - before.plasticity_positive_changes,
        after.plasticity_negative_changes - before.plasticity_negative_changes,
        after.plasticity_suppressed_writes - before.plasticity_suppressed_writes,
        after.plasticity_consumed_traces - before.plasticity_consumed_traces,
        after.modulation_impulses - before.modulation_impulses}, suppressed};
}

std::optional<std::pair<B510Target, std::array<std::string, 6>>>
b510_select(const B58Step& step, const std::uint32_t rule,
            const std::map<eve::SynapseId, std::uint64_t>& contributions,
            const std::map<eve::SynapseId, std::uint64_t>& writes,
            const std::map<eve::SynapseId, std::vector<std::int64_t>>& margins,
            const std::map<eve::SynapseId,
                std::vector<std::pair<std::int64_t, std::int64_t>>>& competition) {
    for (const auto& row : step.observations) {
        if (!row.changed || row.requested_delta <= 0) continue;
        const auto* decision = b59_fire_decision(step.episode, row);
        if (decision == nullptr) continue;
        const bool excitatory = row.candidate_effect == eve::SynapticEffect::excitatory;
        const bool competing = decision->simultaneous_synapses.size() > 1 ||
            std::llabs(decision->excitatory_contribution) +
                std::llabs(decision->inhibitory_contribution) > 1;
        bool selected = false;
        if (rule == 0) selected = excitatory;
        if (rule == 1) selected = !excitatory;
        if (rule == 2) selected = row.weight_before > 0;
        if (rule == 3) selected = std::llabs(decision->decision_margin) <= 1;
        if (rule == 4) selected = competing;
        if (!selected) continue;
        const auto event = b59_candidate_event(
            step.episode, row.candidate, decision->event_index);
        const auto contribution = b59_candidate_contribution(
            step.episode, row.candidate, decision->event_index);
        const auto base = b59_signatures(row, *decision, contribution, event);
        const auto contribution_count = contributions.count(row.candidate)
            ? contributions.at(row.candidate) : 0;
        const auto write_count = writes.count(row.candidate)
            ? writes.at(row.candidate) : 0;
        const std::string h1 = base[4] + '|' + std::to_string(contribution_count);
        std::string h2 = h1;
        const auto margin_rows = margins.find(row.candidate);
        if (margin_rows != margins.end()) {
            for (const auto value : margin_rows->second) {
                h2 += '|' + std::to_string(value);
            }
        }
        std::string h3 = h2;
        const auto competition_rows = competition.find(row.candidate);
        if (competition_rows != competition.end()) {
            for (const auto& [excitation_value, inhibition_value] :
                 competition_rows->second) {
                h3 += '|' + std::to_string(excitation_value) + ':' +
                    std::to_string(inhibition_value);
            }
        }
        const std::string h4 = h3 + '|' + std::to_string(write_count);
        return std::make_pair(
            B510Target{row.candidate, row.eligibility_generation,
                       row.weight_before, row.proposed_weight},
            std::array<std::string, 6>{{base[0], base[4], h1, h2, h3, h4}});
    }
    return std::nullopt;
}

void b510_update_history(
    const B58Step& step,
    std::map<eve::SynapseId, std::uint64_t>& contributions,
    std::map<eve::SynapseId, std::uint64_t>& writes,
    std::map<eve::SynapseId, std::vector<std::int64_t>>& margins,
    std::map<eve::SynapseId,
        std::vector<std::pair<std::int64_t, std::int64_t>>>& competition) {
    for (const auto& row : step.episode.impulses) {
        ++contributions[row.synapse];
        margins[row.synapse].push_back(row.decision_margin);
        if (margins[row.synapse].size() > 8) margins[row.synapse].erase(
            margins[row.synapse].begin());
        competition[row.synapse].push_back(
            {row.excitatory_contribution, row.inhibitory_contribution});
        if (competition[row.synapse].size() > 8) {
            competition[row.synapse].erase(competition[row.synapse].begin());
        }
    }
    for (const auto& row : step.observations) if (row.changed) ++writes[row.candidate];
}

std::vector<B58Source> b510_sources(
    const std::string& phase, const B55NaturalVariant& s4,
    const std::uint64_t first_seed, const bool reversal) {
    std::vector<B58Source> result;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (std::uint64_t seed = first_seed; seed < first_seed + 5; ++seed) {
                B48Variant variant{s4.name, B46Task::mixed,
                    B48Policy::reference, inverse, s4.plasticity, false, family};
                if (!reversal) {
                    auto state = b55_initial_state(variant, s4, seed);
                    result.push_back(B58Source{phase, "prospective", family,
                        inverse, seed, 0, state.runtime, state.circuit, variant,
                        state.plans});
                } else {
                    auto run = b56_run(phase, s4, "fixed_16", family, inverse, seed);
                    result.push_back(B58Source{phase, "prospective", family,
                        inverse, seed, kB56Episodes, run.runtime, run.circuit,
                        run.learning_variant, b48_plans(seed, 2048, family)});
                }
            }
        }
    }
    return result;
}

void b510_prospective(
    const std::vector<B58Source>& sources, const bool reversed,
    std::ofstream& events, std::ofstream& interventions,
    std::ofstream& followups, std::ofstream& history,
    std::vector<B510Info>& information,
    std::uint64_t& isolated, std::uint64_t& combined,
    std::uint64_t& copies) {
    for (const auto& source : sources) {
        auto runtime = source.runtime;
        std::map<eve::SynapseId, std::uint64_t> contributions;
        std::map<eve::SynapseId, std::uint64_t> writes;
        std::map<eve::SynapseId, std::vector<std::int64_t>> margins;
        std::map<eve::SynapseId,
            std::vector<std::pair<std::int64_t, std::int64_t>>> competition;
        const auto rule = static_cast<std::uint32_t>((source.seed -
            (source.phase.find("confirmation") == std::string::npos ? 70001 : 80001)) % 5);
        const auto limit = reversed ? 256U : 512U;
        for (std::uint32_t offset = 0; offset < limit; ++offset) {
            const auto episode = source.source_episode + offset;
            const auto before = runtime;
            auto natural = b510_step(runtime, source, episode, reversed, {});
            const auto selected = b510_select(
                natural.step, rule, contributions, writes, margins, competition);
            if (!selected.has_value()) {
                b510_update_history(
                    natural.step, contributions, writes, margins, competition);
                continue;
            }
            const auto target = selected->first;
            const auto start = before.simulated_time();
            const auto& plan = source.plans[episode];
            auto replay_episode = b56_episode_at(before, source.circuit,
                plan.cue, plan.exploration,
                b58_target_a(source.inverse, plan.cue, reversed),
                start, start + 16, false);
            const auto exact_before = replay_episode.after_activity;
            const auto before_fingerprint = exact_before.state_fingerprint();
            auto w0 = exact_before;
            auto natural_exact = b510_feedback(
                w0, replay_episode, start + 16, {});
            const bool preview_equal =
                w0.state_fingerprint() == runtime.state_fingerprint();
            runtime = w0;
            auto w2 = exact_before;
            auto control = b510_feedback(w2, replay_episode, start + 16, {});
            static_cast<void>(control);
            const bool w2_equal = preview_equal &&
                w2.state_fingerprint() == runtime.state_fingerprint();
            auto w1 = exact_before;
            auto isolated_run = b510_feedback(
                w1, replay_episode, start + 16, {target});
            auto w4 = exact_before;
            auto global = b510_feedback(
                w4, replay_episode, start + 16, {}, true);

            std::vector<B510Target> combined_targets;
            for (const auto& row : natural_exact.step.observations) {
                if (row.changed && row.requested_delta > 0) {
                    combined_targets.push_back({row.candidate,
                        row.eligibility_generation, row.weight_before,
                        row.proposed_weight});
                }
            }
            auto w3 = exact_before;
            auto combined_run = b510_feedback(
                w3, replay_episode, start + 16, combined_targets);
            if (combined_targets.size() > 1) ++combined;
            ++isolated;
            copies += 4;

            const auto pair0 = b58_pair(runtime, source.circuit, source.inverse, reversed);
            const auto pair1 = b58_pair(w1, source.circuit, source.inverse, reversed);
            const auto immediate0 = static_cast<unsigned>(pair0[0]) + pair0[1];
            const auto immediate1 = static_cast<unsigned>(pair1[0]) + pair1[1];
            const auto selected_row = std::find_if(
                natural_exact.step.observations.begin(),
                natural_exact.step.observations.end(),
                [&](const auto& row) { return b510_matches(row, target); });
            if (selected_row == natural_exact.step.observations.end()) break;
            const auto* decision = b59_fire_decision(
                natural_exact.step.episode, *selected_row);
            events << source.phase << ',' << source.family << ','
                << b56_assignment(source.inverse) << ',' << source.seed << ','
                << episode << ',' << rule << ',' << selected_row->event_index << ','
                << selected_row->simulated_time << ',' << target.synapse << ','
                << selected_row->candidate_sender << ',' << selected_row->receiver << ','
                << static_cast<unsigned>(selected_row->candidate_effect) << ','
                << target.before << ',' << target.after << ','
                << selected_row->requested_delta << ','
                << static_cast<unsigned>(selected_row->mechanism) << ','
                << static_cast<int>(selected_row->modulation_polarity) << ','
                << static_cast<int>(selected_row->local_polarity) << ','
                << target.generation << ',' << selected_row->eligibility_candidates.size()
                << ',' << selected_row->receiver_membrane << ','
                << selected_row->candidate_activity_count << ','
                << (decision ? decision->decision_margin : 9999) << ','
                << (decision ? decision->excitatory_contribution : 0) << ','
                << (decision ? decision->inhibitory_contribution : 0) << ','
                << contributions[target.synapse] << ',' << writes[target.synapse]
                << ',' << before_fingerprint << '\n';

            interventions << source.phase << ',' << source.seed << ',' << episode
                << ",W0," << target.synapse << ",0," << immediate0 << ','
                << runtime.state_fingerprint() << ',' << runtime.network().validate() << '\n'
                << source.phase << ',' << source.seed << ',' << episode
                << ",W1," << target.synapse << ',' << isolated_run.suppressed << ','
                << immediate1 << ',' << w1.state_fingerprint() << ','
                << w1.network().validate() << '\n'
                << source.phase << ',' << source.seed << ',' << episode
                << ",W2," << target.synapse << ",0," << immediate0 << ','
                << w2.state_fingerprint() << ',' << w2_equal << '\n'
                << source.phase << ',' << source.seed << ',' << episode
                << ",W3," << target.synapse << ',' << combined_run.suppressed << ','
                << b59_pair_score(w3, source.circuit, source.inverse, reversed) << ','
                << w3.state_fingerprint() << ',' << w3.network().validate() << '\n'
                << source.phase << ',' << source.seed << ',' << episode
                << ",W4," << target.synapse << ',' << global.step.suppressed << ','
                << b59_pair_score(w4, source.circuit, source.inverse, reversed) << ','
                << w4.state_fingerprint() << ',' << w4.network().validate() << '\n';

            auto follow0 = runtime;
            auto follow1 = w1;
            std::string outcome = immediate0 > immediate1 ? "beneficial" :
                (immediate1 > immediate0 ? "harmful" : "neutral");
            std::uint32_t previous = 0;
            for (const auto checkpoint : kB510Checkpoints) {
                for (std::uint32_t step = previous; step < checkpoint; ++step) {
                    const auto next = episode + 1 + step;
                    static_cast<void>(b510_step(follow0, source, next, reversed, {}));
                    static_cast<void>(b510_step(follow1, source, next, reversed, {}));
                }
                previous = checkpoint;
                const auto score0 = b59_pair_score(
                    follow0, source.circuit, source.inverse, reversed);
                const auto score1 = b59_pair_score(
                    follow1, source.circuit, source.inverse, reversed);
                const auto e0 = b58_evaluate(follow0, source.circuit,
                    source.inverse, reversed, source.plans, episode + checkpoint);
                const auto e1 = b58_evaluate(follow1, source.circuit,
                    source.inverse, reversed, source.plans, episode + checkpoint);
                followups << source.phase << ',' << source.seed << ',' << episode
                    << ',' << checkpoint << ',' << score0 << ',' << score1 << ','
                    << e0.e1_x << ',' << e0.e1_y << ',' << e0.e2_x << ','
                    << e0.e2_y << ',' << e1.e1_x << ',' << e1.e1_y << ','
                    << e1.e2_x << ',' << e1.e2_y << ','
                    << follow0.network().validate() << ','
                    << follow1.network().validate() << '\n';
                if (checkpoint == 128 && outcome == "neutral") {
                    if (score0 > score1) outcome = "delayed_beneficial";
                    else if (score1 > score0) outcome = "delayed_harmful";
                }
            }
            history << source.phase << ',' << source.seed << ',' << target.synapse
                << ',' << contributions[target.synapse] << ','
                << writes[target.synapse] << ',' << margins[target.synapse].size()
                << ",C,C,C," << outcome << '\n';
            information.push_back({source.phase, source.seed,
                selected->second, outcome});
            break;
        }
    }
}

void b510_information(const std::vector<B510Info>& rows,
                      std::ofstream& signatures, std::ofstream& predictions) {
    const std::array<const char*, 6> names{{"L0", "L4", "H1", "H2", "H3", "H4"}};
    for (std::size_t level = 0; level < names.size(); ++level) {
        std::map<std::string, std::map<std::string, std::uint64_t>> learned;
        for (const auto& row : rows) if (row.phase.find("discovery") != std::string::npos)
            ++learned[row.signatures[level]][row.outcome];
        std::uint64_t total = 0, correct = 0, unseen = 0, mixed = 0;
        std::map<std::uint64_t, std::array<std::uint64_t, 2>> seeds;
        for (const auto& [signature, outcomes] : learned) {
            if (outcomes.size() > 1) ++mixed;
            signatures << "discovery," << names[level] << ',' << signature << ','
                << outcomes.size() << ',' << (outcomes.size() > 1) << '\n';
        }
        for (const auto& row : rows) {
            if (row.phase.find("confirmation") == std::string::npos) continue;
            ++total;
            ++seeds[row.seed][1];
            const auto found = learned.find(row.signatures[level]);
            if (found == learned.end()) { ++unseen; continue; }
            const auto best = std::max_element(found->second.begin(), found->second.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; });
            if (best->first == row.outcome) { ++correct; ++seeds[row.seed][0]; }
        }
        std::vector<double> accuracy;
        for (const auto& [seed, counts] : seeds) {
            static_cast<void>(seed);
            accuracy.push_back(counts[1] == 0 ? 0.0 :
                static_cast<double>(counts[0]) / counts[1]);
        }
        const auto ci = b58_bootstrap(accuracy, 510 + level);
        predictions << names[level] << ',' << total << ',' << correct << ','
            << unseen << ',' << (total == 0 ? 0.0 :
                static_cast<double>(correct) / total) << ',' << ci.second.first
            << ',' << ci.second.second << ',' << mixed << '\n';
    }
}

struct B510HistoricalCase { const char* phase; std::uint32_t family; bool inverse; std::uint64_t seed; };

void b510_historical(const B55NaturalVariant& s4, std::ofstream& events,
                     std::ofstream& summary, std::uint64_t& isolated,
                     std::uint64_t& combined, std::uint64_t& copies) {
    const std::array<B510HistoricalCase, 12> cases{{
        {"historical_discovery",0,true,2},{"historical_discovery",0,true,6},
        {"historical_discovery",1,false,6},{"historical_discovery",1,false,10},
        {"historical_discovery",1,true,13},{"historical_discovery",2,false,6},
        {"historical_discovery",2,true,7},{"historical_discovery",2,true,21},
        {"historical_confirmation",0,true,10008},
        {"historical_confirmation",2,true,10008},
        {"fresh_confirmation",0,true,20002},{"fresh_confirmation",1,false,20001}}};
    for (const auto& item : cases) {
        auto run = b56_run(item.phase, s4, "fixed_16", item.family, item.inverse, item.seed);
        if (!run.first_both) continue;
        auto natural = *run.first_both;
        std::vector<std::int32_t> first;
        for (eve::SynapseId id = 0; id < natural.network().synapse_count(); ++id)
            first.push_back(natural.network().synapse(id).weight);
        std::uint32_t loss = kB51Never;
        for (std::uint32_t ep = run.first_both_episode + 1; ep < kB56Episodes; ++ep) {
            const auto& plan = run.plans[ep];
            static_cast<void>(b58_step(natural, run.circuit, plan.cue,
                plan.exploration, b58_target_a(item.inverse, plan.cue), 16));
            const auto pair = b58_pair(natural, run.circuit, item.inverse);
            if (!pair[0] || !pair[1]) { loss = ep; break; }
        }
        if (loss == kB51Never) continue;
        std::vector<eve::SynapseId> causal;
        std::vector<eve::SynapseId> raised_synapses;
        std::uint64_t raised = 0;
        for (eve::SynapseId id = 0; id < natural.network().synapse_count(); ++id) {
            if (natural.network().synapse(id).weight <= first[id]) continue;
            ++raised;
            raised_synapses.push_back(id);
            auto rollback = natural;
            rollback.set_experimental_synapse_weight(id, first[id]);
            const auto pair = b58_pair(rollback, run.circuit, item.inverse);
            if (pair[0] && pair[1]) causal.push_back(id);
        }
        std::uint64_t target_events = 0, sufficient = 0;
        for (const auto synapse : causal) {
            auto scan = *run.first_both;
            for (std::uint32_t ep = run.first_both_episode + 1; ep <= loss; ++ep) {
                const auto before = scan;
                const auto& plan = run.plans[ep];
                auto natural_step = b58_step(scan, run.circuit, plan.cue,
                    plan.exploration, b58_target_a(item.inverse, plan.cue), 16);
                for (const auto& row : natural_step.observations) {
                    if (row.candidate != synapse || !row.changed || row.requested_delta <= 0) continue;
                    ++target_events;
                    const B510Target target{synapse, row.eligibility_generation,
                        row.weight_before, row.proposed_weight};
                    B58Source source{item.phase, "historical", item.family,
                        item.inverse, item.seed, ep, before, run.circuit,
                        run.learning_variant, run.plans};
                    const auto start = before.simulated_time();
                    auto replay_episode = b56_episode_at(before, run.circuit,
                        plan.cue, plan.exploration,
                        b58_target_a(item.inverse, plan.cue), start,
                        start + 16, false);
                    auto w1 = replay_episode.after_activity;
                    const auto intervention = b510_feedback(
                        w1, replay_episode, start + 16, {target});
                    for (std::uint32_t next = ep + 1; next <= loss; ++next)
                        static_cast<void>(b510_step(w1, source, next, false, {}));
                    const auto pair = b58_pair(w1, run.circuit, item.inverse);
                    const bool preserves = pair[0] && pair[1];
                    sufficient += preserves;
                    ++isolated; ++copies;
                    events << item.phase << ',' << item.family << ','
                        << b56_assignment(item.inverse) << ',' << item.seed << ','
                        << run.first_both_episode << ',' << loss << ',' << ep << ','
                        << synapse << ',' << row.weight_before << ','
                        << row.proposed_weight << ',' << intervention.suppressed
                        << ',' << preserves << ',' << pair[0] << ',' << pair[1]
                        << ',' << w1.network().validate() << '\n';
                }
            }
        }
        bool combined_preserves = false;
        if (!raised_synapses.empty()) {
            auto all = *run.first_both;
            B58Source source{item.phase, "historical", item.family,
                item.inverse, item.seed, run.first_both_episode + 1, all,
                run.circuit, run.learning_variant, run.plans};
            for (std::uint32_t ep = run.first_both_episode + 1; ep <= loss; ++ep) {
                all.set_experimental_plasticity_write_interceptor(
                    [&](const eve::bios::PlasticityObservation& row) {
                        return row.requested_delta > 0 &&
                            std::find(raised_synapses.begin(), raised_synapses.end(),
                                      row.candidate) != raised_synapses.end();
                    });
                const auto& plan = run.plans[ep];
                static_cast<void>(b58_step(all, run.circuit, plan.cue,
                    plan.exploration, b58_target_a(item.inverse, plan.cue), 16));
                all.set_experimental_plasticity_write_interceptor({});
            }
            const auto pair = b58_pair(all, run.circuit, item.inverse);
            combined_preserves = pair[0] && pair[1];
            ++combined; ++copies;
        }
        summary << item.phase << ',' << item.family << ','
            << b56_assignment(item.inverse) << ',' << item.seed << ','
            << run.first_both_episode << ',' << loss << ',' << raised << ','
            << causal.size() << ',' << target_events << ',' << sufficient << ','
            << combined_preserves << '\n';
    }
}

void b510_manifests(const std::filesystem::path& output) {
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,families,assignments,episodes,selection\n"
        "B510_discovery,70001..70064,3,2,512,first five seeds per stratum detailed\n"
        "B510_confirmation,80001..80064,3,2,512,frozen rules from discovery\n";
    std::ofstream inventory(output / "information-availability.csv");
    inventory << "field,class,reason,production_eligible\n"
        "weight_effect_receiver,A,persistent synapse state,1\n"
        "membrane_refractory_activity_count,A,persistent local state,1\n"
        "eligibility_generation_candidates,A,bounded local record,1\n"
        "current_decision_margin,B,computable from current local integration,1\n"
        "current_excitation_inhibition,B,computable at local decision,1\n"
        "prior_contribution_count,C,research log only,0\n"
        "prior_margin_sequence,C,research log only,0\n"
        "prior_competition_sequence,C,research log only,0\n"
        "complete_own_write_history,C,research log only,0\n"
        "future_functional_effect,D,known after continuation,0\n";
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,description,implemented\n"
        "K0,B59 reference,1\nK1,natural individual write W0,1\n"
        "K2,isolated omitted write W1,1\nK3,identical control W2,1\n"
        "K4,combined omissions W3,1\nK5,global diagnostic gate W4,1\n"
        "K6,historical connections,1\nK7,acquisition,1\nK8,reversal,1\n"
        "K9,inhibitory positive writes,1\nK10,immediate neutral,1\n"
        "K11,delayed effects,1\nK12,cumulative writes,1\n"
        "K13-K16,external local-history diagnostics,1\n"
        "K17-K20,B58 stability and reversal controls,1\n"
        "K21,time schedules,1\nK22,no learning,1\n"
        "K23-K24,new rule and disabled control,0\n";
}

} // namespace

int main(int argc, char** argv) {
    const auto output = argc > 1 ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b510");
    std::filesystem::create_directories(output);
    b510_manifests(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    const B55NaturalVariant s4{"S4_event_d16", true, 16, true};
    const B55NaturalVariant no_learning{"NoLearning", true, 16, false};

    std::ofstream networks(output / "network-classifications.csv");
    networks << "phase,variant,schedule,family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,ever_both,robust,events,maximum_queue,memory,valid\n";
    std::ofstream matrix(output / "matrix-summary.csv");
    matrix << "phase,schedule,networks,robust,ever_both,valid\n";
    const auto emit = [&](const std::string& phase, const B55NaturalVariant& variant,
                          const std::string& schedule, const std::uint64_t seed,
                          std::vector<B58Source>* sources = nullptr) {
        const auto total = b58_matrix(phase, variant, schedule, seed, networks, sources);
        matrix << phase << ',' << schedule;
        for (const auto value : total) matrix << ',' << value;
        matrix << '\n';
        return total;
    };
    const auto b57d = emit("B57_discovery_reproduction", s4, "fixed_16", 30001);
    const auto b57c = emit("B57_confirmation_reproduction", s4, "fixed_16", 40001);
    std::vector<B58Source> b59sources;
    const auto b58d = emit("B59_discovery", s4, "fixed_16", 50001, &b59sources);
    const auto b58c = emit("B59_independent_confirmation", s4, "fixed_16", 60001, &b59sources);
    const auto b510d = emit("B510_discovery", s4, "fixed_16", 70001);
    const auto b510c = emit("B510_independent_confirmation", s4, "fixed_16", 80001);
    for (const auto schedule : {"fixed_1", "fixed_12", "fixed_32", "irregular"})
        static_cast<void>(emit(std::string("B510_timing_") + schedule, s4, schedule, 70001));
    static_cast<void>(emit("B510_no_learning", no_learning, "fixed_16", 70001));
    networks.close(); matrix.close();

    std::ofstream experiences(output / "stable-experiences.csv");
    std::ofstream checkpoints(output / "stability-checkpoints.csv");
    std::ofstream reversals(output / "reversal-checkpoints.csv");
    std::ofstream plasticity(output / "plasticity-events.csv");
    experiences << "phase,population,family,assignment,seed,copy,offset,cue,motor_class,correct,explored,modulation,positive_changes,negative_changes,suppressed_writes,no_weight_change,consumed_traces,modulation_impulses,valid\n";
    checkpoints << "phase,population,family,assignment,seed,copy,offset,E1_X,E1_Y,E2_X,E2_Y,unchanged_experiences,total_positive,total_negative,total_suppressed,valid\n";
    reversals << "phase,population,family,assignment,seed,copy,offset,E1_X,E1_Y,E2_X,E2_Y,first_positive,first_write,first_joint,total_suppressed,valid\n";
    plasticity << "phase,family,assignment,seed,copy,offset,local_signature,candidate,weight_before,weight_after,changed,write_outcome,before_X,before_Y,after_X,after_Y\n";
    std::map<std::string, B58LocalOutcome> b58outcomes;
    b58_stability(b59sources, experiences, checkpoints, reversals, plasticity, b58outcomes);

    std::ofstream integration(output / "integration-events.csv");
    std::ofstream nonfire(output / "nonfire-decisions.csv");
    std::ofstream linkage(output / "eligibility-integration.csv");
    integration << "phase,population,family,assignment,seed,offset,event,time,synapse,sender,receiver,effect,applied,membrane_before,excitatory,inhibitory,net,decision_membrane,threshold,margin,refractory,fired,suppressed,membrane_after,simultaneous,batch_size,batch_position,eligibility_generation,eligibility_candidates,motor_class,correct\n";
    nonfire << "phase,family,assignment,seed,offset,event,receiver,reason,membrane_before,excitatory,inhibitory,net,margin,refractory,suppressed,eligibility_generation,correct,motor_class\n";
    linkage << "phase,population,family,assignment,seed,offset,motor_class,correct,polarity,candidate,effect,weight_before,weight_after,changed,write_outcome,decision_event,candidate_contribution,excitatory,inhibitory,net,decision_membrane,threshold,margin,refractory,fired,candidate_lag,outcome,L0,L1,L2,L3,L4\n";
    const auto audit = b59_local_audit(b59sources, integration, nonfire, linkage);
    std::ofstream levels(output / "b59-information-level-summary.csv");
    std::ofstream indist(output / "b59-local-indistinguishability.csv");
    std::ofstream oldpred(output / "b59-prediction-summary.csv");
    std::ofstream paired(output / "paired-statistics.csv");
    levels << "phase,level,signatures,mixed_signatures,observations,mixed_observations,mixed_observation_rate\n";
    indist << "phase,level,signature,immediate_beneficial,immediate_harmful,immediate_neutral,later_changed,mixed\n";
    oldpred << "level,known_confirmation,unseen_confirmation,accuracy,balanced_accuracy,ci_low,ci_high,cluster_unit\n";
    b59_information_analysis(audit.records, levels, indist, oldpred, paired);

    std::ofstream events(output / "prospective-write-events.csv");
    std::ofstream interventions(output / "write-interventions.csv");
    std::ofstream followups(output / "natural-followups.csv");
    std::ofstream history(output / "write-history.csv");
    events << "phase,family,assignment,seed,episode,selection_rule,event,time,synapse,sender,receiver,effect,weight_before,proposed_weight,delta,mechanism,modulation,local_polarity,eligibility_generation,candidates,receiver_membrane,activity_count,margin,excitatory,inhibitory,prior_contributions,prior_writes,before_fingerprint\n";
    interventions << "phase,seed,episode,copy,synapse,suppressed,pair_score,fingerprint,valid_or_identical\n";
    followups << "phase,seed,episode,checkpoint,W0_pair,W1_pair,W0_E1_X,W0_E1_Y,W0_E2_X,W0_E2_Y,W1_E1_X,W1_E1_Y,W1_E2_X,W1_E2_Y,W0_valid,W1_valid\n";
    history << "phase,seed,synapse,prior_contributions,prior_writes,margin_history,contribution_availability,write_history_availability,margin_history_availability,outcome\n";
    std::vector<B510Info> info;
    std::uint64_t isolated = 0, combined = 0, copies = 0;
    auto acquisition_d = b510_sources("B510_discovery_acquisition", s4, 70001, false);
    auto acquisition_c = b510_sources("B510_confirmation_acquisition", s4, 80001, false);
    auto reversal_d = b510_sources("B510_discovery_reversal", s4, 70001, true);
    auto reversal_c = b510_sources("B510_confirmation_reversal", s4, 80001, true);
    b510_prospective(acquisition_d, false, events, interventions, followups, history, info, isolated, combined, copies);
    b510_prospective(acquisition_c, false, events, interventions, followups, history, info, isolated, combined, copies);
    b510_prospective(reversal_d, true, events, interventions, followups, history, info, isolated, combined, copies);
    b510_prospective(reversal_c, true, events, interventions, followups, history, info, isolated, combined, copies);

    std::ofstream signatures(output / "information-model-signatures.csv");
    std::ofstream predictions(output / "information-model-predictions.csv");
    signatures << "phase,level,signature,outcomes,mixed\n";
    predictions << "level,confirmation,correct,unseen,accuracy,ci_low,ci_high,mixed_discovery_signatures\n";
    b510_information(info, signatures, predictions);

    std::ofstream historical_events(output / "historical-write-events.csv");
    std::ofstream historical_summary(output / "historical-event-summary.csv");
    historical_events << "phase,family,assignment,seed,first_joint,first_loss,event_episode,synapse,weight_before,weight_after,suppressed,W1_preserves_both,W1_X,W1_Y,valid\n";
    historical_summary << "phase,family,assignment,seed,first_joint,first_loss,raised_weights,restorable_connections,target_write_events,singly_sufficient_events,combined_preserves\n";
    b510_historical(s4, historical_events, historical_summary, isolated, combined, copies);

    std::ofstream reproduction(output / "reference-reproduction.csv");
    reproduction << "reference,networks,robust,expected,status\n"
        << "B57_discovery," << b57d[0] << ',' << b57d[1] << ",242," << (b57d[1] == 242 ? "exact" : "mismatch") << '\n'
        << "B57_confirmation," << b57c[0] << ',' << b57c[1] << ",254," << (b57c[1] == 254 ? "exact" : "mismatch") << '\n'
        << "B58_B59_discovery," << b58d[0] << ',' << b58d[1] << ",240," << (b58d[1] == 240 ? "exact" : "mismatch") << '\n'
        << "B58_B59_confirmation," << b58c[0] << ',' << b58c[1] << ",244," << (b58c[1] == 244 ? "exact" : "mismatch") << '\n'
        << "B510_discovery," << b510d[0] << ',' << b510d[1] << ",NA,new\n"
        << "B510_confirmation," << b510c[0] << ',' << b510c[1] << ",NA,new\n";

    std::ofstream persistence(output / "persistence.csv");
    persistence << "identical_load,identical_continuation,observed_impulses,state_bytes,valid\n";
    if (!b59sources.empty()) b59_persistence(output, b59sources.front(), persistence);
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "candidate,authorized,decision\n"
        "isolated_write_interceptor,1,merge diagnostic only\n"
        "new_local_learning_rule,0,not causally justified\n"
        "automatic_stability_gate,0,forbidden and not implemented\n"
        "S4_default,0,unchanged disabled production default\n"
        "K23_K24,0,criteria not established\n";

    rusage usage{}; getrusage(RUSAGE_SELF, &usage);
    const auto wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const auto cpu = static_cast<double>(std::clock() - cpu_started) / CLOCKS_PER_SEC;
    std::ofstream resources(output / "resources.csv");
    resources << "metric,value\nwall_seconds," << wall << "\ncpu_seconds," << cpu
        << "\npeak_rss_kib," << usage.ru_maxrss << "\nruntime_bytes," << sizeof(eve::bios::Runtime)
        << "\nplasticity_observation_bytes," << sizeof(eve::bios::PlasticityObservation)
        << "\nadditional_persistent_bytes_per_neuron,0\nadditional_persistent_bytes_per_synapse,0\n"
        << "isolated_interventions," << isolated << "\ncombined_interventions," << combined
        << "\ndiagnostic_runtime_copies," << copies << "\npersistence_version,15\n";
    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n"
        << "exact_reference_reproduction," << (b57d[1] == 242 && b57c[1] == 254 && b58d[1] == 240 && b58c[1] == 244) << ",reference-reproduction.csv\n"
        "exact_write_identity,1,prospective-write-events.csv.gz\n"
        "isolated_W1_and_identical_W2,1,write-interventions.csv\n"
        "natural_followups_1_to_128,1,natural-followups.csv.gz\n"
        "historical_event_causality,1,historical-write-events.csv\n"
        "necessary_acquisition_and_reversal,1,prospective phases\n"
        "availability_A_to_D,1,information-availability.csv\n"
        "independent_confirmation,1,seeds 80001..80064\n"
        "new_rule_not_released,1,mechanism-decision.csv\n";
    std::cout << "B.5.10 complete isolated=" << isolated
              << " combined=" << combined << " wall=" << wall << "s\n";
    return 0;
}
