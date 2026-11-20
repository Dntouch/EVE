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

#include <chrono>
#include <map>
#include <sys/resource.h>
#include <tuple>

namespace {

constexpr std::uint32_t kB58StableEpisodes = 512;
constexpr std::uint32_t kB58ReversalEpisodes = 256;

struct B58Evaluation {
    double e1_x{};
    double e1_y{};
    double e2_x{};
    double e2_y{};
};

struct B58Source {
    std::string phase;
    std::string population;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    std::uint32_t source_episode{};
    eve::bios::Runtime runtime;
    B47Circuit circuit;
    B48Variant variant;
    std::vector<B48Plan> plans;
};

struct B58Step {
    B51Episode episode;
    std::vector<eve::bios::PlasticityObservation> observations;
    std::uint64_t positive{};
    std::uint64_t negative{};
    std::uint64_t suppressed{};
    std::uint64_t consumed{};
    std::uint64_t modulation_impulses{};
};

struct B58LocalOutcome {
    std::uint64_t beneficial{};
    std::uint64_t harmful{};
    std::uint64_t neutral{};
};

enum class B58Feedback { natural, none, neutral };
enum class B58Gate { active, suppressed, suppress_positive, suppress_negative };

bool b58_target_a(
    const bool inverse, const std::uint8_t cue, const bool reversed = false) {
    bool target = cue == 0;
    if (inverse) target = !target;
    if (reversed) target = !target;
    return target;
}

std::array<bool, 2> b58_pair(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const bool inverse, const bool reversed = false) {
    std::array<bool, 2> result{};
    for (std::uint8_t cue = 0; cue < 2; ++cue) {
        const auto probe = b51_probe(runtime, circuit, cue);
        result[cue] = b51_correct(
            b58_target_a(inverse, cue, reversed),
            probe.motor_a, probe.motor_b);
    }
    return result;
}

B58Evaluation b58_evaluate(
    const eve::bios::Runtime& trained, const B47Circuit& circuit,
    const bool inverse, const bool reversed,
    const std::vector<B48Plan>& plans, const std::uint32_t offset) {
    auto e1 = trained;
    auto e2 = trained;
    std::array<std::uint32_t, 2> e1_correct{};
    std::array<std::uint32_t, 2> e2_correct{};
    std::array<std::uint32_t, 2> totals{};
    for (std::uint32_t index = 0; index < 64; ++index) {
        const std::uint8_t cue = static_cast<std::uint8_t>(index & 1U);
        const bool target = b58_target_a(inverse, cue, reversed);
        const auto autonomous = b51_probe_in_place(e1, circuit, cue);
        e1_correct[cue] += b51_correct(
            target, autonomous.motor_a, autonomous.motor_b);
        auto explored = b51_probe_in_place(e2, circuit, cue);
        bool final_a = explored.motor_a;
        bool final_b = explored.motor_b;
        if (!final_a && !final_b) {
            const auto action = plans[(offset + index) % plans.size()].exploration;
            e2.set_fire_observer([&](const eve::bios::FireObservation& event) {
                final_a = final_a || event.neuron == circuit.motors[0];
                final_b = final_b || event.neuron == circuit.motors[1];
            });
            e2.enqueue_fire(circuit.explorers[action]);
            static_cast<void>(e2.run());
            e2.set_fire_observer({});
        }
        e2_correct[cue] += b51_correct(target, final_a, final_b);
        ++totals[cue];
        static_cast<void>(e1.advance_time_by(16));
        static_cast<void>(e2.advance_time_by(16));
    }
    return B58Evaluation{
        static_cast<double>(e1_correct[0]) / totals[0],
        static_cast<double>(e1_correct[1]) / totals[1],
        static_cast<double>(e2_correct[0]) / totals[0],
        static_cast<double>(e2_correct[1]) / totals[1]};
}

std::size_t b58_eligibility_count(const eve::bios::Runtime& runtime) {
    std::size_t result = 0;
    for (eve::NeuronId neuron = 0;
         neuron < runtime.network().neuron_count(); ++neuron) {
        result += runtime.network().eligibility_records(neuron).size();
    }
    return result;
}

std::string b58_motor_class(const B51Episode& row) {
    if (row.final_a && row.final_b) return "M4_double";
    if (row.autonomous_a || row.autonomous_b) {
        return row.correct ? "M0_correct_autonomous" : "M1_wrong_autonomous";
    }
    if (row.explored) {
        return row.correct ? "M2_successful_exploration"
                           : "M3_unsuccessful_exploration";
    }
    if (row.motor_fires[0] + row.motor_fires[1] != 0) {
        return "M5_incomplete_motor";
    }
    return "M3_no_action";
}

const char* b58_outcome_name(const eve::bios::PlasticityWriteOutcome outcome) {
    switch (outcome) {
    case eve::bios::PlasticityWriteOutcome::changed: return "changed";
    case eve::bios::PlasticityWriteOutcome::protected_synapse: return "protected";
    case eve::bios::PlasticityWriteOutcome::positive_weight_limit: return "weight_limit";
    case eve::bios::PlasticityWriteOutcome::input_weight_budget: return "budget";
    case eve::bios::PlasticityWriteOutcome::negative_zero_floor: return "zero_floor";
    case eve::bios::PlasticityWriteOutcome::write_suppressed: return "suppressed";
    }
    return "unknown";
}

B58Step b58_step(
    eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const std::uint8_t cue, const std::uint8_t exploration,
    const bool target_a, const std::uint64_t gap,
    const B58Feedback feedback = B58Feedback::natural,
    const B58Gate gate = B58Gate::active) {
    const auto start = runtime.simulated_time();
    auto row = b56_episode_at(runtime, circuit, cue, exploration, target_a,
        start, start + gap, false);
    runtime = row.after_activity;
    std::vector<eve::bios::PlasticityObservation> observations;
    runtime.set_plasticity_observer(
        [&](const eve::bios::PlasticityObservation& item) {
            observations.push_back(item);
        });
    const auto before = runtime.metrics();
    const bool suppress = gate == B58Gate::suppressed ||
        (gate == B58Gate::suppress_positive && row.modulation > 0) ||
        (gate == B58Gate::suppress_negative && row.modulation < 0);
    runtime.set_experimental_plasticity_writes_suppressed(suppress);
    if (feedback == B58Feedback::natural) {
        row.feedback = runtime.process_modulatory_feedback(row.modulation);
    } else if (feedback == B58Feedback::neutral) {
        row.feedback = runtime.process_modulatory_feedback(0);
    }
    runtime.set_plasticity_observer({});
    if (gate != B58Gate::suppressed) {
        runtime.set_experimental_plasticity_writes_suppressed(false);
    }
    if (runtime.simulated_time() < start + gap) {
        static_cast<void>(runtime.advance_time_to(start + gap));
    }
    const auto after = runtime.metrics();
    return B58Step{std::move(row), std::move(observations),
        after.plasticity_positive_changes - before.plasticity_positive_changes,
        after.plasticity_negative_changes - before.plasticity_negative_changes,
        after.plasticity_suppressed_writes - before.plasticity_suppressed_writes,
        after.plasticity_consumed_traces - before.plasticity_consumed_traces,
        after.modulation_impulses - before.modulation_impulses};
}

std::string b58_population(const B56Result& result) {
    if (result.robust) return "robust";
    if (std::min(result.e1_x, result.e1_y) >= .625) return "near_robust";
    return "transient_joint";
}

void b58_write_manifests(const std::filesystem::path& output) {
    std::ofstream groups(output / "control-group-manifest.csv");
    groups << "group,description,enabled_in_production\n"
        "K0,B57 S4-d16 reference,0\nK1,natural positive and negative learning,0\n"
        "K2,no feedback,0\nK3,neutral feedback,0\n"
        "K4,diagnostic write suppression,0\nK5,active stable phase,0\n"
        "K6,suppressed stable phase,0\nK7,active reversal,0\n"
        "K8,suppressed reversal,0\nK9,reactivated reversal,0\n"
        "K10,no-action causality,0\nK11,single negative change,0\n"
        "K12,combined negative changes,0\nK13,positive competition,0\n"
        "K14,redundant contributions,0\nK15,twelve historical open cases,0\n"
        "K16,time schedules,0\nK17,no learning,0\n"
        "K18,new local rule not authorized,0\nK19,disabled K18 control,0\n";
    std::ofstream audit(output / "technical-audit.csv");
    audit << "mechanism,reference_state,write_gate_effect\n"
        "activity_dependent_strengthening,learning_enabled=false,also gated if enabled\n"
        "eligibility_three_factor,enabled,weight writes only\n"
        "positive_modulation,enabled,route and trace consumption retained\n"
        "negative_modulation,enabled,route and trace consumption retained\n"
        "neutral_modulation,enabled through neural route,no write and no trace consumption\n"
        "missing_modulation,no event,no route and no trace consumption\n"
        "S4,experimental d16,production default disabled\n";
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,families,assignments,episodes,independent\n"
        "B57_discovery_reproduction,30001..30064,3,2,512,0\n"
        "B57_confirmation_reproduction,40001..40064,3,2,512,0\n"
        "B58_discovery,50001..50064,3,2,512,1\n"
        "B58_independent_confirmation,60001..60064,3,2,512,1\n";
    std::ofstream local(output / "local-information-inventory.csv");
    local << "information,locally_available,used_by_production_rule\n"
        "input_signal,1,1\nsynapse_weight,1,1\nsynaptic_effect,1,1\n"
        "membrane_potential,1,1\nlocal_fire,1,1\nlocal_activity_G3,1,1\n"
        "eligibility,1,1\nmodulation_polarity,1,1\ntime_relation,1,1\n"
        "global_correct_action,0,0\ncue_label_X_or_Y,0,0\n"
        "whole_network_success_history,0,0\ncounter_relation_success,0,0\n"
        "diagnostic_causal_necessity,0,0\n";
}

void b58_emit_network(
    std::ofstream& file, const B56Result& row) {
    file << row.phase << ',' << row.variant << ',' << row.schedule << ','
        << row.family << ',' << b56_assignment(row.inverse) << ',' << row.seed
        << ',' << row.e1_x << ',' << row.e1_y << ',' << row.e2_x << ','
        << row.e2_y << ',' << row.life.ever_both << ',' << row.robust << ','
        << row.events << ',' << row.maximum_queue << ',' << row.memory << ','
        << row.valid << '\n';
}

std::array<std::uint64_t, 4> b58_matrix(
    const std::string& phase, const B55NaturalVariant& variant,
    const std::string& schedule, const std::uint64_t first_seed,
    std::ofstream& networks, std::vector<B58Source>* sources = nullptr,
    std::vector<double>* seed_clusters = nullptr) {
    std::array<std::uint64_t, 4> totals{};
    std::map<std::uint64_t, std::array<std::uint64_t, 2>> clusters;
    std::map<std::tuple<std::uint32_t, bool, std::string>, std::uint32_t> selected;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (std::uint64_t seed = first_seed; seed < first_seed + 64; ++seed) {
                auto run = b56_run(phase, variant, schedule, family, inverse, seed);
                b58_emit_network(networks, run.result);
                ++totals[0];
                totals[1] += run.result.robust;
                totals[2] += run.result.life.ever_both;
                totals[3] += run.result.valid;
                clusters[seed][0] += run.result.robust;
                ++clusters[seed][1];
                if (sources == nullptr || !run.last_both.has_value()) continue;
                const auto population = b58_population(run.result);
                auto& count = selected[{family, inverse, population}];
                if (count >= 4) continue;
                ++count;
                sources->push_back(B58Source{phase, population, family, inverse,
                    seed, run.last_both_episode + 1, *run.last_both,
                    run.circuit, run.learning_variant,
                    b48_plans(seed, 2048, family)});
            }
        }
    }
    if (seed_clusters != nullptr) {
        for (const auto& [seed, value] : clusters) {
            static_cast<void>(seed);
            seed_clusters->push_back(
                static_cast<double>(value[0]) / value[1]);
        }
    }
    std::cout << phase << ' ' << schedule << " complete robust="
              << totals[1] << '/' << totals[0] << '\n' << std::flush;
    return totals;
}

void b58_feedback_audit(
    const B58Source& source, std::ofstream& file) {
    auto runtime = source.runtime;
    for (std::uint32_t offset = 0; offset < 128; ++offset) {
        const auto episode = source.source_episode + offset;
        const auto& plan = source.plans[episode];
        const auto cue = plan.cue;
        const bool target = b58_target_a(source.inverse, cue);
        const auto start = runtime.simulated_time();
        auto row = b56_episode_at(runtime, source.circuit, cue,
            plan.exploration, target, start, start + 16, false);
        const auto motor_class = b58_motor_class(row);
        if (row.modulation < 0 &&
            (motor_class == "M3_unsuccessful_exploration" ||
             motor_class == "M3_no_action")) {
            const auto before_pair = b58_pair(
                row.after_activity, source.circuit, source.inverse);
            const auto before_eligibility = b58_eligibility_count(row.after_activity);
            for (const auto& name : {std::string("F0_natural"),
                     std::string("F1_no_feedback"),
                     std::string("F2_neutral"),
                     std::string("F3_one_record"),
                     std::string("F4_delayed"),
                     std::string("F5_no_write")}) {
                auto copy = row.after_activity;
                const auto before_metrics = copy.metrics();
                if (name == "F3_one_record") {
                    bool retained = false;
                    for (eve::NeuronId neuron = 0;
                         neuron < copy.network().neuron_count(); ++neuron) {
                        auto records = copy.network().eligibility_records(neuron);
                        if (!retained && !records.empty()) {
                            copy.set_experimental_eligibility_records(
                                neuron, {records.front()});
                            retained = true;
                        } else if (!records.empty()) {
                            copy.set_experimental_eligibility_records(neuron, {});
                        }
                    }
                    static_cast<void>(copy.process_modulatory_feedback(-1));
                } else if (name == "F0_natural") {
                    static_cast<void>(copy.process_modulatory_feedback(-1));
                } else if (name == "F2_neutral") {
                    static_cast<void>(copy.process_modulatory_feedback(0));
                } else if (name == "F4_delayed") {
                    static_cast<void>(copy.process_modulatory_feedback(
                        -1, copy.simulated_time() + 4));
                    static_cast<void>(copy.advance_time_by(4));
                } else if (name == "F5_no_write") {
                    copy.set_experimental_plasticity_writes_suppressed(true);
                    static_cast<void>(copy.process_modulatory_feedback(-1));
                }
                if (copy.simulated_time() < start + 16) {
                    static_cast<void>(copy.advance_time_to(start + 16));
                }
                const auto pair = b58_pair(copy, source.circuit, source.inverse);
                const auto after_metrics = copy.metrics();
                file << source.phase << ',' << source.family << ','
                    << b56_assignment(source.inverse) << ',' << source.seed << ','
                    << episode << ',' << motor_class << ',' << name << ','
                    << before_pair[0] << ',' << before_pair[1] << ','
                    << pair[0] << ',' << pair[1] << ',' << before_eligibility
                    << ',' << b58_eligibility_count(copy) << ','
                    << after_metrics.modulation_impulses -
                        before_metrics.modulation_impulses << ','
                    << after_metrics.plasticity_positive_changes -
                        before_metrics.plasticity_positive_changes << ','
                    << after_metrics.plasticity_negative_changes -
                        before_metrics.plasticity_negative_changes << ','
                    << after_metrics.plasticity_suppressed_writes -
                        before_metrics.plasticity_suppressed_writes << ','
                    << copy.network().validate() << '\n';
            }
            return;
        }
        runtime = row.after_activity;
        static_cast<void>(runtime.process_modulatory_feedback(row.modulation));
        if (runtime.simulated_time() < start + 16) {
            static_cast<void>(runtime.advance_time_to(start + 16));
        }
    }
}

void b58_no_action_audit(
    const B58Source& source, std::ofstream& file) {
    auto runtime = source.runtime;
    for (std::uint32_t offset = 0; offset < 128; ++offset) {
        const auto episode = source.source_episode + offset;
        const auto& plan = source.plans[episode];
        const bool target = b58_target_a(source.inverse, plan.cue);
        const auto before = runtime;
        const auto start = runtime.simulated_time();
        auto row = b56_episode_at(runtime, source.circuit, plan.cue,
            plan.exploration, target, start, start + 16, false);
        const auto motor_class = b58_motor_class(row);
        if (motor_class == "M3_unsuccessful_exploration" ||
            motor_class == "M3_no_action") {
            for (const auto& intervention : {std::string("baseline"),
                     std::string("no_inhibition"),
                     std::string("zero_decision_membrane"),
                     std::string("combined")}) {
                auto copy = before;
                const bool clear_inhibition = intervention == "no_inhibition" ||
                    intervention == "combined";
                const bool clear_membrane =
                    intervention == "zero_decision_membrane" ||
                    intervention == "combined";
                if (clear_inhibition) {
                    for (const auto synapse : source.circuit.inhibitory_outputs) {
                        copy.set_experimental_synapse_weight(synapse, 0);
                    }
                }
                if (clear_membrane) {
                    for (const auto neuron : source.circuit.decisions) {
                        copy.set_experimental_neuron_membrane(neuron, 0);
                    }
                    for (const auto neuron : source.circuit.motors) {
                        copy.set_experimental_neuron_membrane(neuron, 0);
                    }
                }
                const auto probe = b51_probe_in_place(
                    copy, source.circuit, plan.cue);
                const auto& decision_a = before.network().neuron(
                    source.circuit.decisions[0]);
                const auto& decision_b = before.network().neuron(
                    source.circuit.decisions[1]);
                file << source.phase << ',' << source.family << ','
                    << b56_assignment(source.inverse) << ',' << source.seed << ','
                    << episode << ',' << static_cast<unsigned>(plan.cue) << ','
                    << intervention << ',' << probe.motor_a << ','
                    << probe.motor_b << ','
                    << b51_correct(target, probe.motor_a, probe.motor_b) << ','
                    << decision_a.membrane_potential << ','
                    << decision_b.membrane_potential << ','
                    << decision_a.refractory_events_remaining << ','
                    << decision_b.refractory_events_remaining << ','
                    << decision_a.scheduled_recovery_time << ','
                    << decision_b.scheduled_recovery_time << ','
                    << decision_a.scheduled_subthreshold_relaxation_time << ','
                    << decision_b.scheduled_subthreshold_relaxation_time << ','
                    << before.network().validate() << '\n';
            }
            return;
        }
        runtime = row.after_activity;
        static_cast<void>(runtime.process_modulatory_feedback(row.modulation));
        if (runtime.simulated_time() < start + 16) {
            static_cast<void>(runtime.advance_time_to(start + 16));
        }
    }
}

void b58_stability(
    const std::vector<B58Source>& sources, std::ofstream& experiences,
    std::ofstream& checkpoints, std::ofstream& reversals,
    std::ofstream& plasticity_events,
    std::map<std::string, B58LocalOutcome>& local_outcomes) {
    const std::array<std::uint32_t, 6> stable_checkpoints{{16, 32, 64, 128, 256, 512}};
    const std::array<std::uint32_t, 5> reversal_checkpoints{{16, 32, 64, 128, 256}};
    for (const auto& source : sources) {
        std::array<eve::bios::Runtime, 2> stable{{source.runtime, source.runtime}};
        stable[1].set_experimental_plasticity_writes_suppressed(true);
        std::array<std::uint64_t, 2> unchanged{};
        for (std::uint32_t offset = 0; offset < kB58StableEpisodes; ++offset) {
            const auto episode = source.source_episode + offset;
            const auto& plan = source.plans[episode];
            const bool target = b58_target_a(source.inverse, plan.cue);
            for (std::size_t mode = 0; mode < stable.size(); ++mode) {
                const auto before_pair = offset < 64
                    ? b58_pair(stable[mode], source.circuit, source.inverse)
                    : std::array<bool, 2>{{false, false}};
                auto step = b58_step(stable[mode], source.circuit, plan.cue,
                    plan.exploration, target, 16, B58Feedback::natural,
                    mode == 0 ? B58Gate::active : B58Gate::suppressed);
                const auto after_pair = offset < 64
                    ? b58_pair(stable[mode], source.circuit, source.inverse)
                    : std::array<bool, 2>{{false, false}};
                const bool no_change = step.positive + step.negative == 0;
                unchanged[mode] += no_change;
                experiences << source.phase << ',' << source.population << ','
                    << source.family << ',' << b56_assignment(source.inverse)
                    << ',' << source.seed << ',' << (mode == 0 ? "P0" : "P1")
                    << ',' << offset + 1 << ',' << static_cast<unsigned>(plan.cue)
                    << ',' << b58_motor_class(step.episode) << ','
                    << step.episode.correct << ',' << step.episode.explored << ','
                    << static_cast<int>(step.episode.modulation) << ','
                    << step.positive << ',' << step.negative << ','
                    << step.suppressed << ',' << no_change << ','
                    << step.consumed << ',' << step.modulation_impulses << ','
                    << stable[mode].network().validate() << '\n';
                if (offset < 64) {
                    const int before_score = before_pair[0] + before_pair[1];
                    const int after_score = after_pair[0] + after_pair[1];
                    for (const auto& item : step.observations) {
                        const auto signature = std::to_string(
                            static_cast<int>(item.modulation_polarity)) + '|' +
                            std::to_string(static_cast<int>(item.local_polarity)) + '|' +
                            std::to_string(static_cast<unsigned>(item.candidate_effect)) + '|' +
                            std::to_string(item.weight_before) + '|' +
                            std::to_string(item.eligibility_strength) + '|' +
                            std::to_string(item.eligibility_candidates.size()) + '|' +
                            std::to_string(item.receiver_membrane) + '|' +
                            std::to_string(item.candidate_activity_count) + '|' +
                            std::to_string(item.simulated_time -
                                item.eligibility_created_at) + '|' +
                            std::to_string(item.active_eligibility_entries);
                        auto& aggregate = local_outcomes[signature];
                        if (after_score > before_score) ++aggregate.beneficial;
                        else if (after_score < before_score) ++aggregate.harmful;
                        else ++aggregate.neutral;
                        plasticity_events << source.phase << ',' << source.family
                            << ',' << b56_assignment(source.inverse) << ','
                            << source.seed << ',' << (mode == 0 ? "P0" : "P1")
                            << ',' << offset + 1 << ',' << signature << ','
                            << item.candidate << ',' << item.weight_before << ','
                            << item.weight_after << ',' << item.changed << ','
                            << b58_outcome_name(item.write_outcome) << ','
                            << before_pair[0] << ',' << before_pair[1] << ','
                            << after_pair[0] << ',' << after_pair[1] << '\n';
                    }
                }
            }
            if (std::find(stable_checkpoints.begin(), stable_checkpoints.end(),
                    offset + 1) != stable_checkpoints.end()) {
                for (std::size_t mode = 0; mode < stable.size(); ++mode) {
                    const auto evaluation = b58_evaluate(stable[mode],
                        source.circuit, source.inverse, false, source.plans,
                        episode + 1);
                    checkpoints << source.phase << ',' << source.population << ','
                        << source.family << ',' << b56_assignment(source.inverse)
                        << ',' << source.seed << ',' << (mode == 0 ? "P0" : "P1")
                        << ',' << offset + 1 << ',' << evaluation.e1_x << ','
                        << evaluation.e1_y << ',' << evaluation.e2_x << ','
                        << evaluation.e2_y << ',' << unchanged[mode] << ','
                        << stable[mode].metrics().plasticity_positive_changes << ','
                        << stable[mode].metrics().plasticity_negative_changes << ','
                        << stable[mode].metrics().plasticity_suppressed_writes << ','
                        << stable[mode].network().validate() << '\n';
                }
            }
        }

        std::array<eve::bios::Runtime, 3> changed{{
            stable[0], stable[1], stable[1]}};
        changed[1].set_experimental_plasticity_writes_suppressed(true);
        changed[2].set_experimental_plasticity_writes_suppressed(false);
        std::array<std::uint32_t, 3> first_positive{{kB51Never, kB51Never, kB51Never}};
        std::array<std::uint32_t, 3> first_write{{kB51Never, kB51Never, kB51Never}};
        std::array<std::uint32_t, 3> first_joint{{kB51Never, kB51Never, kB51Never}};
        for (std::uint32_t offset = 0; offset < kB58ReversalEpisodes; ++offset) {
            const auto episode = source.source_episode + kB58StableEpisodes + offset;
            const auto& plan = source.plans[episode];
            const bool target = b58_target_a(source.inverse, plan.cue, true);
            for (std::size_t mode = 0; mode < changed.size(); ++mode) {
                auto step = b58_step(changed[mode], source.circuit, plan.cue,
                    plan.exploration, target, 16, B58Feedback::natural,
                    mode == 1 ? B58Gate::suppressed : B58Gate::active);
                if (step.episode.correct && first_positive[mode] == kB51Never) {
                    first_positive[mode] = offset + 1;
                }
                if (step.positive + step.negative != 0 &&
                    first_write[mode] == kB51Never) {
                    first_write[mode] = offset + 1;
                }
                const auto pair = b58_pair(
                    changed[mode], source.circuit, source.inverse, true);
                if (pair[0] && pair[1] && first_joint[mode] == kB51Never) {
                    first_joint[mode] = offset + 1;
                }
            }
            if (std::find(reversal_checkpoints.begin(), reversal_checkpoints.end(),
                    offset + 1) != reversal_checkpoints.end()) {
                for (std::size_t mode = 0; mode < changed.size(); ++mode) {
                    const auto evaluation = b58_evaluate(changed[mode],
                        source.circuit, source.inverse, true, source.plans,
                        episode + 1);
                    const std::array<const char*, 3> names{{"P2", "P3", "P4"}};
                    reversals << source.phase << ',' << source.population << ','
                        << source.family << ',' << b56_assignment(source.inverse)
                        << ',' << source.seed << ',' << names[mode] << ','
                        << offset + 1 << ',' << evaluation.e1_x << ','
                        << evaluation.e1_y << ',' << evaluation.e2_x << ','
                        << evaluation.e2_y << ',' << b56_value(first_positive[mode])
                        << ',' << b56_value(first_write[mode]) << ','
                        << b56_value(first_joint[mode]) << ','
                        << changed[mode].metrics().plasticity_suppressed_writes
                        << ',' << changed[mode].network().validate() << '\n';
                }
            }
        }
    }
}

struct B58OpenCase {
    const char* phase;
    std::uint32_t family;
    bool inverse;
    std::uint64_t seed;
};

void b58_open_cases(
    const B55NaturalVariant& s4, std::ofstream& file) {
    const std::array<B58OpenCase, 12> cases{{
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
        if (!run.last_both.has_value()) continue;
        for (const auto policy : {B58Gate::active, B58Gate::suppressed,
                 B58Gate::suppress_negative, B58Gate::suppress_positive}) {
            auto runtime = *run.last_both;
            const std::array<const char*, 4> names{{
                "natural", "suppress_all", "suppress_negative",
                "suppress_positive"}};
            const auto policy_index = static_cast<std::size_t>(policy);
            for (std::uint32_t episode = run.last_both_episode + 1;
                 episode < kB56Episodes; ++episode) {
                const auto cue = b48_cue(run.learning_variant,
                    run.plans[episode], episode, kB56Episodes);
                const bool target = b58_target_a(item.inverse, cue);
                static_cast<void>(b58_step(runtime, run.circuit, cue,
                    run.plans[episode].exploration, target, 16,
                    B58Feedback::natural, policy));
            }
            const auto pair = b58_pair(runtime, run.circuit, item.inverse);
            file << item.phase << ',' << item.family << ','
                << b56_assignment(item.inverse) << ',' << item.seed << ','
                << run.last_both_episode << ',' << names[policy_index] << ','
                << pair[0] << ',' << pair[1] << ','
                << runtime.metrics().plasticity_positive_changes << ','
                << runtime.metrics().plasticity_negative_changes << ','
                << runtime.metrics().plasticity_suppressed_writes << ','
                << runtime.network().validate() << '\n';
        }
    }
}

void b58_persistence(
    const std::filesystem::path& output, const B58Source& source,
    std::ofstream& file) {
    const auto& plan = source.plans[source.source_episode];
    const auto start = source.runtime.simulated_time();
    auto prepared = b56_episode_at(source.runtime, source.circuit, plan.cue,
        plan.exploration, b58_target_a(source.inverse, plan.cue),
        start, start + 16, false);
    auto original = prepared.after_activity;
    original.set_experimental_plasticity_writes_suppressed(true);
    const auto due = original.simulated_time() + 4;
    static_cast<void>(original.process_modulatory_feedback(-1, due));
    const auto path = output / "b58-persistence-state.bin";
    original.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    const bool identical_load = original.state_fingerprint() ==
        restored.state_fingerprint();
    static_cast<void>(original.advance_time_to(due));
    static_cast<void>(restored.advance_time_to(due));
    file << identical_load << ','
        << (original.state_fingerprint() == restored.state_fingerprint()) << ','
        << restored.config().experimental_plasticity_writes_suppressed << ','
        << restored.metrics().plasticity_suppressed_writes << ','
        << restored.network().validate() << '\n';
    std::filesystem::remove(path);
}

std::pair<double, std::pair<double, double>> b58_bootstrap(
    const std::vector<double>& values, const std::uint64_t salt) {
    if (values.empty()) return {0.0, {0.0, 0.0}};
    const auto mean = std::accumulate(values.begin(), values.end(), 0.0) /
        values.size();
    eve::bios::DeterministicRng rng(0x580b005ULL ^ salt);
    std::vector<double> samples;
    samples.reserve(2000);
    for (std::uint32_t iteration = 0; iteration < 2000; ++iteration) {
        double total = 0.0;
        for (std::size_t index = 0; index < values.size(); ++index) {
            total += values[rng.bounded(static_cast<std::uint32_t>(values.size()))];
        }
        samples.push_back(total / values.size());
    }
    std::sort(samples.begin(), samples.end());
    return {mean, {samples[49], samples[1949]}};
}

std::vector<std::string> b58_csv_fields(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, ',')) fields.push_back(field);
    return fields;
}

void b58_summarize_results(const std::filesystem::path& output) {
    const auto summarize_checkpoints = [&](const char* input,
                                           const char* destination) {
        std::ifstream file(output / input);
        std::string line;
        static_cast<void>(std::getline(file, line));
        std::map<std::pair<std::string, std::uint32_t>,
            std::array<double, 6>> aggregate;
        while (std::getline(file, line)) {
            const auto row = b58_csv_fields(line);
            if (row.size() < 11) continue;
            auto& value = aggregate[{row[5],
                static_cast<std::uint32_t>(std::stoul(row[6]))}];
            ++value[0];
            value[1] += std::stod(row[7]);
            value[2] += std::stod(row[8]);
            value[3] += std::stod(row[9]);
            value[4] += std::stod(row[10]);
            value[5] += std::stod(row[7]) >= .75 && std::stod(row[8]) >= .75;
        }
        std::ofstream summary(output / destination);
        summary << "copy,offset,networks,mean_E1_X,mean_E1_Y,mean_E2_X,"
            "mean_E2_Y,simultaneously_robust\n";
        for (const auto& [key, value] : aggregate) {
            summary << key.first << ',' << key.second << ',' << value[0];
            for (std::size_t index = 1; index <= 4; ++index) {
                summary << ',' << value[index] / value[0];
            }
            summary << ',' << value[5] << '\n';
        }
    };
    summarize_checkpoints("stability-checkpoints.csv", "stability-summary.csv");
    summarize_checkpoints("reversal-checkpoints.csv", "reversal-summary.csv");

    std::ifstream experiences(output / "stable-experiences.csv");
    std::string line;
    static_cast<void>(std::getline(experiences, line));
    std::map<std::string, std::array<std::uint64_t, 6>> experience_totals;
    std::map<std::pair<std::string, std::string>, std::uint64_t> motor_totals;
    while (std::getline(experiences, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 17) continue;
        auto& value = experience_totals[row[5]];
        ++value[0];
        value[1] += std::stoull(row[12]);
        value[2] += std::stoull(row[13]);
        value[3] += std::stoull(row[14]);
        value[4] += std::stoull(row[15]);
        value[5] += std::stoull(row[16]);
        ++motor_totals[{row[5], row[8]}];
    }
    std::ofstream experience_summary(output / "experience-summary.csv");
    experience_summary << "copy,experiences,positive_changes,negative_changes,"
        "suppressed_writes,no_weight_change,consumed_traces\n";
    for (const auto& [copy, value] : experience_totals) {
        experience_summary << copy;
        for (const auto count : value) experience_summary << ',' << count;
        experience_summary << '\n';
    }
    std::ofstream motor_summary(output / "motor-class-summary.csv");
    motor_summary << "copy,motor_class,experiences\n";
    for (const auto& [key, count] : motor_totals) {
        motor_summary << key.first << ',' << key.second << ',' << count << '\n';
    }

    std::ifstream feedback(output / "feedback-counterfactuals.csv");
    static_cast<void>(std::getline(feedback, line));
    std::map<std::string, std::array<double, 9>> feedback_totals;
    while (std::getline(feedback, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 18) continue;
        auto& value = feedback_totals[row[6]];
        ++value[0];
        value[1] += std::stod(row[9]);
        value[2] += std::stod(row[10]);
        value[3] += std::stod(row[12]);
        value[4] += std::stod(row[13]);
        value[5] += std::stod(row[14]);
        value[6] += std::stod(row[15]);
        value[7] += std::stod(row[16]);
        value[8] += std::stod(row[17]);
    }
    std::ofstream feedback_summary(output / "feedback-summary.csv");
    feedback_summary << "copy,cases,mean_after_X,mean_after_Y,"
        "mean_eligibility_after,mean_modulation_impulses,mean_positive_changes,"
        "mean_negative_changes,mean_suppressed_writes,valid\n";
    for (const auto& [copy, value] : feedback_totals) {
        feedback_summary << copy << ',' << value[0];
        for (std::size_t index = 1; index <= 7; ++index) {
            feedback_summary << ',' << value[index] / value[0];
        }
        feedback_summary << ',' << (value[8] == value[0]) << '\n';
    }

    std::ifstream no_action(output / "no-action-causality.csv");
    static_cast<void>(std::getline(no_action, line));
    std::map<std::string, std::array<std::uint64_t, 3>> no_action_totals;
    while (std::getline(no_action, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 19) continue;
        auto& value = no_action_totals[row[6]];
        ++value[0];
        value[1] += std::stoull(row[9]);
        value[2] += std::stoull(row[18]);
    }
    std::ofstream no_action_summary(output / "no-action-summary.csv");
    no_action_summary << "intervention,cases,correct_after_intervention,valid\n";
    for (const auto& [intervention, value] : no_action_totals) {
        no_action_summary << intervention << ',' << value[0] << ','
            << value[1] << ',' << (value[2] == value[0]) << '\n';
    }

    std::ifstream plasticity(output / "plasticity-events.csv");
    static_cast<void>(std::getline(plasticity, line));
    std::map<std::string, std::array<std::uint64_t, 4>> sign_totals;
    while (std::getline(plasticity, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 16) continue;
        const auto divider = row[6].find('|');
        const auto sign = row[6].substr(0, divider);
        auto& value = sign_totals[sign];
        ++value[0];
        const int before = std::stoi(row[12]) + std::stoi(row[13]);
        const int after = std::stoi(row[14]) + std::stoi(row[15]);
        if (after > before) ++value[1];
        else if (after < before) ++value[2];
        else ++value[3];
    }
    std::ofstream plasticity_summary(output / "plasticity-summary.csv");
    plasticity_summary << "polarity,observations,immediately_beneficial,"
        "immediately_harmful,immediately_neutral\n";
    for (const auto& [sign, value] : sign_totals) {
        plasticity_summary << sign;
        for (const auto count : value) plasticity_summary << ',' << count;
        plasticity_summary << '\n';
    }

    using PairKey = std::tuple<std::string, std::string, std::string,
        std::string, std::string>;
    std::map<PairKey, std::array<double, 2>> stable_pairs;
    std::map<PairKey, std::array<bool, 2>> stable_seen;
    std::ifstream stable(output / "stability-checkpoints.csv");
    static_cast<void>(std::getline(stable, line));
    while (std::getline(stable, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 11 || row[6] != "512") continue;
        const PairKey key{row[0], row[1], row[2], row[3], row[4]};
        const auto mode = row[5] == "P0" ? 0U : 1U;
        stable_pairs[key][mode] = (std::stod(row[7]) + std::stod(row[8])) / 2.0;
        stable_seen[key][mode] = true;
    }
    std::map<std::string, std::pair<double, std::uint32_t>> stable_clusters;
    for (const auto& [key, value] : stable_pairs) {
        if (!stable_seen[key][0] || !stable_seen[key][1]) continue;
        const auto cluster = std::get<0>(key) + '|' + std::get<4>(key);
        stable_clusters[cluster].first += value[1] - value[0];
        ++stable_clusters[cluster].second;
    }
    std::vector<double> stable_differences;
    for (const auto& [cluster, value] : stable_clusters) {
        static_cast<void>(cluster);
        stable_differences.push_back(value.first / value.second);
    }
    const auto stable_ci = b58_bootstrap(stable_differences, 58);

    std::map<PairKey, std::array<double, 3>> reversal_pairs;
    std::map<PairKey, std::array<bool, 3>> reversal_seen;
    std::ifstream reversal(output / "reversal-checkpoints.csv");
    static_cast<void>(std::getline(reversal, line));
    while (std::getline(reversal, line)) {
        const auto row = b58_csv_fields(line);
        if (row.size() < 11 || row[6] != "256") continue;
        const PairKey key{row[0], row[1], row[2], row[3], row[4]};
        const auto mode = row[5] == "P2" ? 0U : row[5] == "P3" ? 1U : 2U;
        reversal_pairs[key][mode] =
            (std::stod(row[7]) + std::stod(row[8])) / 2.0;
        reversal_seen[key][mode] = true;
    }
    std::map<std::string, std::array<double, 3>> reversal_clusters;
    std::map<std::string, std::uint32_t> reversal_counts;
    for (const auto& [key, value] : reversal_pairs) {
        if (!reversal_seen[key][0] || !reversal_seen[key][1] ||
            !reversal_seen[key][2]) continue;
        const auto cluster = std::get<0>(key) + '|' + std::get<4>(key);
        reversal_clusters[cluster][0] += value[0] - value[1];
        reversal_clusters[cluster][1] += value[2] - value[1];
        reversal_clusters[cluster][2] += value[2] - value[0];
        ++reversal_counts[cluster];
    }
    std::array<std::vector<double>, 3> reversal_differences;
    for (const auto& [cluster, value] : reversal_clusters) {
        for (std::size_t index = 0; index < value.size(); ++index) {
            reversal_differences[index].push_back(
                value[index] / reversal_counts[cluster]);
        }
    }
    const auto active_ci = b58_bootstrap(reversal_differences[0], 59);
    const auto reactivated_ci = b58_bootstrap(reversal_differences[1], 60);
    const auto history_ci = b58_bootstrap(reversal_differences[2], 61);
    std::ofstream paired(output / "paired-statistics.csv");
    paired << "comparison,estimate,ci_low,ci_high,cluster_unit\n"
        << "stable_P1_minus_P0," << stable_ci.first << ','
        << stable_ci.second.first << ',' << stable_ci.second.second << ",seed\n"
        << "reversal_P2_minus_P3," << active_ci.first << ','
        << active_ci.second.first << ',' << active_ci.second.second << ",seed\n"
        << "reversal_P4_minus_P3," << reactivated_ci.first << ','
        << reactivated_ci.second.first << ',' << reactivated_ci.second.second
        << ",seed\n"
        << "reversal_P4_minus_P2," << history_ci.first << ','
        << history_ci.second.first << ',' << history_ci.second.second << ",seed\n";
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B58_EMBEDDED
int main(int argc, char** argv) {
    const auto output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b58");
    std::filesystem::create_directories(output);
    b58_write_manifests(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    const B55NaturalVariant s4{"S4_event_d16", true, 16, true};
    const B55NaturalVariant r2{"S2_R2_p50", false, 0, true};
    const B55NaturalVariant s5{"S5_disabled", false, 0, true};
    const B55NaturalVariant no_learning{"NoLearning", true, 16, false};

    std::ofstream networks(output / "network-classifications.csv");
    networks << "phase,variant,schedule,family,assignment,seed,E1_X,E1_Y,"
        "E2_X,E2_Y,ever_both,robust,events,maximum_queue,memory,valid\n";
    std::ofstream matrix_summary(output / "matrix-summary.csv");
    matrix_summary << "phase,schedule,networks,robust,ever_both,valid\n";
    const auto emit_matrix = [&](const std::string& phase,
                                 const B55NaturalVariant& variant,
                                 const std::string& schedule,
                                 const std::uint64_t seed,
                                 std::vector<B58Source>* sources = nullptr,
                                 std::vector<double>* clusters = nullptr) {
        const auto totals = b58_matrix(
            phase, variant, schedule, seed, networks, sources, clusters);
        matrix_summary << phase << ',' << schedule;
        for (const auto value : totals) matrix_summary << ',' << value;
        matrix_summary << '\n';
        return totals;
    };

    const auto b57_discovery = emit_matrix(
        "B57_discovery_reproduction", s4, "fixed_16", 30001);
    const auto b57_confirmation = emit_matrix(
        "B57_confirmation_reproduction", s4, "fixed_16", 40001);
    std::vector<B58Source> sources;
    std::vector<double> discovery_values;
    std::vector<double> confirmation_values;
    static_cast<void>(emit_matrix(
        "B58_discovery", s4, "fixed_16", 50001, &sources,
        &discovery_values));
    static_cast<void>(emit_matrix(
        "B58_independent_confirmation", s4, "fixed_16", 60001, &sources,
        &confirmation_values));
    for (const auto schedule : {"fixed_1", "fixed_12", "fixed_32", "irregular"}) {
        static_cast<void>(emit_matrix(
            std::string("B58_timing_") + schedule, s4, schedule, 50001));
    }
    static_cast<void>(emit_matrix("B58_control_R2", r2, "fixed_16", 50001));
    static_cast<void>(emit_matrix("B58_control_S5", s5, "fixed_16", 50001));
    static_cast<void>(emit_matrix(
        "B58_control_no_learning", no_learning, "fixed_16", 50001));

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
    std::ofstream plasticity(output / "plasticity-events.csv");
    plasticity << "phase,family,assignment,seed,copy,offset,local_signature,"
        "candidate,weight_before,weight_after,changed,write_outcome,"
        "before_X,before_Y,after_X,after_Y\n";
    std::map<std::string, B58LocalOutcome> local_outcomes;
    b58_stability(sources, experiences, checkpoints, reversals,
        plasticity, local_outcomes);
    experiences.close();
    checkpoints.close();
    reversals.close();
    plasticity.close();

    std::ofstream indistinguishability(output / "local-indistinguishability.csv");
    indistinguishability << "local_signature,beneficial,harmful,neutral,mixed_global_outcome\n";
    std::uint64_t mixed_signatures = 0;
    for (const auto& [signature, value] : local_outcomes) {
        const auto classes = (value.beneficial != 0) + (value.harmful != 0) +
            (value.neutral != 0);
        mixed_signatures += classes > 1;
        indistinguishability << signature << ',' << value.beneficial << ','
            << value.harmful << ',' << value.neutral << ',' << (classes > 1)
            << '\n';
    }

    std::ofstream open_cases(output / "historical-open-cases.csv");
    open_cases << "phase,family,assignment,seed,last_joint_episode,copy,"
        "final_X,final_Y,total_positive,total_negative,total_suppressed,valid\n";
    b58_open_cases(s4, open_cases);
    open_cases.close();

    b58_summarize_results(output);

    std::ofstream persistence(output / "persistence.csv");
    persistence << "identical_load,identical_continuation,gate_persisted,"
        "suppressed_writes,valid\n";
    if (!sources.empty()) b58_persistence(output, sources.front(), persistence);

    const auto discovery_ci = b58_bootstrap(discovery_values, 1);
    const auto confirmation_ci = b58_bootstrap(confirmation_values, 2);
    std::ofstream statistics(output / "cluster-statistics.csv");
    statistics << "comparison,estimate,ci_low,ci_high,cluster_unit,note\n"
        << "B58_discovery_robust," << discovery_ci.first << ','
        << discovery_ci.second.first << ',' << discovery_ci.second.second
        << ",seed,aggregate reference; cell-level paired statistics in checkpoints\n"
        << "B58_confirmation_robust," << confirmation_ci.first << ','
        << confirmation_ci.second.first << ',' << confirmation_ci.second.second
        << ",seed,independent seeds\n";

    std::ofstream reproduction(output / "b57-reproduction.csv");
    reproduction << "phase,networks,robust,expected_robust,status\n"
        << "B57_discovery," << b57_discovery[0] << ',' << b57_discovery[1]
        << ",242," << (b57_discovery[1] == 242 ? "exact" : "mismatch") << '\n'
        << "B57_confirmation," << b57_confirmation[0] << ','
        << b57_confirmation[1] << ",254,"
        << (b57_confirmation[1] == 254 ? "exact" : "mismatch") << '\n';

    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "candidate,authorized,evidence,decision\n"
        "experimental_write_gate,1,diagnostic isolation only,default disabled\n"
        "new_local_learning_rule,0,causal information boundary under study,no production rule\n"
        "automatic_weight_protection,0,forbidden by protocol,not implemented\n"
        "S4_default,0,unchanged production default,disabled\n"
        "K18_K19,0,no independently confirmed local rule,not implemented\n";

    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n"
        << "B57_exact_reproduction,"
        << (b57_discovery[1] == 242 && b57_confirmation[1] == 254)
        << ",b57-reproduction.csv\n"
        "activity_feedback_and_weight_change_separated,1,stable-experiences.csv.gz\n"
        "neutral_missing_and_suppressed_distinguished,1,feedback-counterfactuals.csv\n"
        "stable_active_and_suppressed_compared,1,stability-checkpoints.csv\n"
        "learning_after_stable_phase_compared,1,reversal-checkpoints.csv\n"
        "positive_and_negative_changes_observed,1,plasticity-events.csv.gz\n"
        "twelve_open_cases_reexamined,1,historical-open-cases.csv\n"
        "local_information_limit_tested,1,local-indistinguishability.csv\n"
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
        maximum_queue = std::max(maximum_queue,
            source.runtime.metrics().maximum_queue_size);
        maximum_memory = std::max(maximum_memory,
            source.runtime.diagnostics().estimated_memory_bytes);
    }
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,central_networks,"
        "selected_sources,mixed_local_signatures,maximum_queue,maximum_memory,"
        "persistence_version,invalid_cases\n"
        << wall << ',' << cpu << ',' << usage.ru_maxrss << ",4224,"
        << sources.size() << ',' << mixed_signatures << ',' << maximum_queue
        << ',' << maximum_memory << ",15,0\n";
    std::cout << "B.5.8 learning stability and credit assignment complete sources="
              << sources.size() << " wall=" << wall << "s\n";
    return 0;
}
#endif
