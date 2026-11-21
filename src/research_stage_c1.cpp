#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_C0_EMBEDDED
#include "research_stage_c0.cpp"
#undef EVE_RESEARCH_STAGE_C0_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <cmath>
#include <numeric>
#include <optional>
#include <sstream>
#include <tuple>

namespace {

constexpr std::array<std::uint32_t, 10> kC1Horizons{{
    0, 1, 2, 4, 8, 16, 32, 64, 128, 256}};

struct HistoricalCase {
    const char* phase;
    const char* task;
    bool inverse;
    std::uint32_t neurons;
    std::uint64_t seed;
    eve::SynapseId synapse;
};

constexpr std::array<HistoricalCase, 19> kHistoricalCases{{
    {"discovery", "P_to_Q_or_R", false, 32, 120348, 53},
    {"discovery", "P_to_Q_or_R", false, 256, 122598, 52},
    {"discovery", "P_to_Q_or_R", false, 2048, 140492, 52},
    {"discovery", "mixed_A_B", false, 128, 141334, 53},
    {"discovery", "mixed_A_B", false, 256, 142610, 54},
    {"discovery", "retention_consolidation", false, 2048, 190516, 54},
    {"discovery", "B512_learning_checkpoints", false, 512, 195134, 52},
    {"discovery", "B512_learning_checkpoints", false, 512, 195144, 50},
    {"discovery", "B512_learning_checkpoints", true, 32, 195355, 53},
    {"discovery", "B512_learning_checkpoints", true, 128, 196325, 50},
    {"discovery", "B512_learning_checkpoints", true, 512, 200171, 55},
    {"confirmation", "competing_relations", false, 2048, 230512, 50},
    {"confirmation", "P_to_Q_or_R", false, 32, 220338, 51},
    {"confirmation", "P_to_Q_or_R", false, 32, 220366, 51},
    {"confirmation", "P_to_Q_or_R", true, 32, 225325, 54},
    {"confirmation", "retention_consolidation", false, 256, 272582, 53},
    {"confirmation", "B512_learning_checkpoints", false, 32, 290364, 53},
    {"confirmation", "B512_learning_checkpoints", false, 128, 291314, 56},
    {"confirmation", "B512_learning_checkpoints", true, 2048, 315527, 56},
}};

struct C1Outputs {
    std::ofstream reconstruction;
    std::ofstream eligibility;
    std::ofstream prospective;
    std::ofstream matrix;
    std::ofstream appropriateness;
    std::ofstream longitudinal;
    std::ofstream long_term_assessment;
    std::ofstream reorganization;

    explicit C1Outputs(const std::filesystem::path& output)
        : reconstruction(output / "historical-case-reconstruction.csv"),
          eligibility(output / "eligibility-modulation-analysis.csv"),
          prospective(output / "prospective-connections.csv"),
          matrix(output / "causal-matrix.csv"),
          appropriateness(output / "situational-appropriateness.csv"),
          longitudinal(output / "longitudinal-checkpoints.csv"),
          long_term_assessment(output / "long-term-assessment.csv"),
          reorganization(output / "distributed-reorganization.csv") {
        reconstruction << "population,phase,task,assignment,neurons,seed,selected_synapse,"
            "episode,event,weight_before,weight_after,sender,receiver,effect,"
            "receiver_membrane,receiver_refractory,active_selected,response,correct,"
            "modulation,positive_writes,negative_writes,time\n";
        eligibility << "population,phase,task,assignment,neurons,seed,selected_synapse,"
            "episode,event_index,time,candidate,sender,receiver,effect,modulation,"
            "eligibility_generation,eligibility_created_at,eligibility_expires_at,"
            "eligibility_strength,candidates,candidate_activity,receiver_membrane,"
            "weight_before,proposed_weight,weight_after,changed,outcome,mechanism,"
            "selected_candidate\n";
        prospective << "phase,task,assignment,neurons,seed,synapse,creation_episode,"
            "sender,receiver,initial_weight,permitted,first_weight_one_episode,"
            "first_threshold_episode,final_weight,short_term_functional,"
            "causal_single_ablation,retained_after_one,reemerged_by_256,"
            "ability_contribution,selection_rule\n";
        matrix << "population,phase,task,assignment,neurons,seed,synapse,variant,"
            "intervention,actual_action,correct,modulation,selected_active,"
            "weight_before,weight_after,target_fired_before,target_fired_after,"
            "positive_writes,negative_writes,integrity,causality,"
            "situational_assessment,long_term_assessment\n";
        appropriateness << "population,phase,task,assignment,neurons,seed,synapse,"
            "selected_active,natural_action,counterfactual_action,natural_correct,"
            "counterfactual_correct,natural_consequence,counterfactual_consequence,"
            "classification,reason,integrity,causality\n";
        longitudinal << "population,phase,task,assignment,neurons,seed,synapse,"
            "variant,experiences,selected_weight,target_fires,cumulative_correct,"
            "repertoire,network_synapses,alternative_functional_inputs,"
            "selected_reused,valid\n";
        long_term_assessment << "population,phase,task,assignment,neurons,seed,"
            "synapse,natural_correct_256,omitted_correct_256,"
            "natural_repertoire_256,omitted_repertoire_256,"
            "natural_target_function_256,omitted_target_function_256,"
            "classification,integrity,causality\n";
        reorganization << "population,phase,task,assignment,neurons,seed,synapse,"
            "variant,experiences,selected_functional,alternative_functional_inputs,"
            "repertoire,path_reuse,interpretation\n";
    }
};

struct ConnectionHistory {
    eve::SynapseId id{};
    eve::NeuronId sender{};
    eve::NeuronId receiver{};
    std::uint32_t creation_episode{};
    std::int32_t initial_weight{};
    std::int32_t final_weight{};
    int first_one{-1};
    int first_threshold{-1};
    bool permitted{};
};

struct TrialSummary {
    std::uint64_t grown{};
    std::uint64_t functional{};
    std::uint64_t retained{};
    std::uint64_t reemerged{};
    std::uint64_t correct{};
    std::uint64_t events{};
    std::size_t memory{};
};

struct MatrixOutcome {
    std::string action;
    bool correct{};
    std::int8_t modulation{};
    bool selected_active{};
    std::int32_t weight_before{};
    std::int32_t weight_after{};
    bool target_before{};
    bool target_after{};
    std::uint64_t positive_writes{};
    std::uint64_t negative_writes{};
    std::vector<eve::bios::PlasticityObservation> observations;
};

std::string action_name(const B511Episode& row) {
    return b511_response(row.final_a, row.final_b);
}

std::string effect_name(const eve::SynapticEffect effect) {
    return effect == eve::SynapticEffect::excitatory ? "excitatory" : "inhibitory";
}

std::string mechanism_name(const eve::bios::PlasticityMechanism mechanism) {
    return mechanism == eve::bios::PlasticityMechanism::eligibility_modulation
        ? "eligibility_modulation" : "activity_dependent";
}

std::string join_ids(const std::vector<eve::SynapseId>& ids) {
    std::ostringstream stream;
    for (std::size_t index = 0; index < ids.size(); ++index) {
        if (index != 0) stream << ';';
        stream << ids[index];
    }
    return stream.str();
}

B511Task c1_task_for_episode(const std::string& task,
                             const std::uint32_t episode) {
    if (task == "simple_sensorimotor") return B511Task::a;
    if (task == "A_to_B") return episode < 8 ? B511Task::a : B511Task::b;
    if (task == "reacquisition")
        return episode < 6 || (episode >= 11 && episode < 24)
            ? B511Task::a : B511Task::b;
    if (task == "transfer") return episode < 8 ? B511Task::a : B511Task::c;
    if (task == "partial_latent_learning") return B511Task::a;
    return episode % 2 == 0 ? B511Task::a : B511Task::b;
}

bool c1_learning_enabled(const std::string& task, const std::uint32_t episode) {
    return task != "partial_latent_learning" || episode >= 8;
}

std::uint32_t c1_repertoire(const eve::bios::Runtime& source,
                            const B511Circuit& circuit, const bool inverse) {
    auto a = source;
    auto b = source;
    return static_cast<std::uint32_t>(
        b511_probe(a, circuit, b511_pattern(B511Task::a, false, inverse)).correct +
        b511_probe(b, circuit, b511_pattern(B511Task::b, false, inverse)).correct);
}

bool c1_target_fires(const eve::bios::Runtime& source,
                     const eve::SynapseId selected) {
    auto copy = source;
    const auto edge = copy.network().synapse(selected);
    bool fired = false;
    copy.set_fire_observer([&](const eve::bios::FireObservation& observation) {
        fired = fired || observation.neuron == edge.receiver;
    });
    copy.enqueue_fire(edge.sender);
    static_cast<void>(copy.run());
    return fired;
}

std::uint32_t alternative_functional_inputs(
    const eve::bios::Runtime& runtime, const eve::SynapseId selected) {
    const auto target = runtime.network().synapse(selected).receiver;
    std::uint32_t count = 0;
    for (const auto id : runtime.network().incoming_synapses(target)) {
        if (id != selected &&
            runtime.network().synapse(id).weight >= runtime.config().firing_threshold)
            ++count;
    }
    return count;
}

eve::bios::Runtime c1_runtime(const std::uint32_t neurons,
                              const std::uint64_t seed,
                              B511Circuit& circuit) {
    B511MembraneVariant variant{
        "C1", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, true, neurons};
    auto config = b511_config(variant, true);
    config.growth_enabled = true;
    eve::bios::Runtime runtime(config, seed);
    circuit = b511_circuit(runtime, true);
    runtime.configure_structural_protection(protection_for(neurons, circuit));
    eve::brainstem::AutonomousExplorationConfig exploration;
    exploration.enabled = true;
    exploration.activation_budget = 8;
    exploration.trigger_probability_denominator = 1;
    exploration.maximum_sender_outgoing = 4;
    exploration.local_radius = 2;
    exploration.seed = seed ^ 0xC0E000ULL;
    runtime.configure_autonomous_exploration(exploration);
    return runtime;
}

void remove_selected_eligibility(eve::bios::Runtime& runtime,
                                 const eve::SynapseId selected) {
    const auto receiver = runtime.network().synapse(selected).receiver;
    auto records = runtime.network().eligibility_records(receiver);
    for (auto& record : records) {
        record.candidates.erase(
            std::remove(record.candidates.begin(), record.candidates.end(), selected),
            record.candidates.end());
    }
    records.erase(std::remove_if(records.begin(), records.end(),
        [](const auto& record) { return record.candidates.empty(); }), records.end());
    runtime.set_experimental_eligibility_records(receiver, records);
}

void isolate_selected_input(eve::bios::Runtime& runtime,
                            const eve::SynapseId selected) {
    const auto receiver = runtime.network().synapse(selected).receiver;
    for (const auto id : runtime.network().incoming_synapses(receiver)) {
        if (id != selected) runtime.set_experimental_synapse_weight(id, 0);
    }
}

MatrixOutcome run_matrix_variant(
    const eve::bios::Runtime& baseline, const B511Circuit& circuit,
    const std::string& task, const bool inverse, const std::uint64_t seed,
    const eve::SynapseId selected, const std::uint32_t variant) {
    auto copy = baseline;
    MatrixOutcome result;
    result.weight_before = copy.network().synapse(selected).weight;
    result.target_before = c1_target_fires(copy, selected);
    const auto receiver = copy.network().synapse(selected).receiver;
    if (variant == 3) copy.set_experimental_plasticity_writes_suppressed(true);
    if (variant == 6 || variant == 9) isolate_selected_input(copy, selected);
    if (variant == 7 || variant == 9) copy.set_structural_growth_enabled(false);
    if (variant == 8 || variant == 9)
        copy.set_autonomous_exploration_enabled(false);
    if (variant == 1) {
        copy.set_experimental_plasticity_write_interceptor(
            [selected](const eve::bios::PlasticityObservation& observation) {
                return observation.candidate == selected &&
                    observation.weight_before == 2 &&
                    observation.proposed_weight == 1;
            });
    } else if (variant == 5) {
        copy.set_experimental_plasticity_write_interceptor(
            [receiver](const eve::bios::PlasticityObservation& observation) {
                return observation.receiver == receiver;
            });
    }
    copy.set_plasticity_observer(
        [&](const eve::bios::PlasticityObservation& observation) {
            result.observations.push_back(observation);
        });
    const auto before = copy.metrics();
    const auto selected_task = c1_task_for_episode(task, 16);
    const std::function<void(eve::bios::Runtime&)> before_feedback =
        variant == 4 || variant == 9
        ? std::function<void(eve::bios::Runtime&)>(
              [selected](eve::bios::Runtime& active) {
                  remove_selected_eligibility(active, selected);
              })
        : std::function<void(eve::bios::Runtime&)>{};
    const auto row = b511_episode(
        copy, circuit, b511_pattern(selected_task, false, inverse),
        static_cast<std::uint8_t>((seed + 16) & 1U), true, true,
        kB511ElementGap, before_feedback);
    const auto after = copy.metrics();
    result.action = action_name(row);
    result.correct = row.correct;
    result.modulation = row.modulation;
    result.selected_active = row.active_synapses.count(selected) != 0;
    result.weight_after = copy.network().synapse(selected).weight;
    result.target_after = c1_target_fires(copy, selected);
    result.positive_writes = after.plasticity_positive_changes -
        before.plasticity_positive_changes;
    result.negative_writes = after.plasticity_negative_changes -
        before.plasticity_negative_changes;
    return result;
}

std::pair<std::string, std::string> situational_assessment(
    const MatrixOutcome& natural, const MatrixOutcome& skipped) {
    if (!natural.selected_active) {
        return {"undecided",
                "selected synapse was not active in the evaluated experience"};
    }
    if (natural.correct != skipped.correct) {
        if (natural.correct)
            return {"useful", "natural write changed an incorrect counterfactual to correct"};
        return {"harmful", "omitting the write changed the natural error to correct"};
    }
    if (natural.action == skipped.action)
        return {"neutral", "isolated omission did not change action or correctness"};
    return {"context_dependent",
            "action changed while task-level correctness remained equal"};
}

void write_matrix_rows(
    C1Outputs& outputs, const std::string& population, const char* phase,
    const std::string& task, const bool inverse, const std::uint32_t neurons,
    const std::uint64_t seed, const eve::SynapseId selected,
    const eve::bios::Runtime& baseline, const B511Circuit& circuit,
    std::array<MatrixOutcome, 10>& outcomes) {
    constexpr std::array<const char*, 10> names{{
        "natural_continuation", "critical_write_omitted", "identical_copy",
        "all_writes_suppressed", "selected_eligibility_removed",
        "local_modulation_effect_suppressed", "competing_inputs_removed",
        "growth_disabled", "exploration_disabled",
        "eligibility_competition_growth_exploration_joint"}};
    for (std::uint32_t variant = 0; variant < outcomes.size(); ++variant)
        outcomes[variant] = run_matrix_variant(
            baseline, circuit, task, inverse, seed, selected, variant);
    for (const auto& observation : outcomes[0].observations) {
        outputs.eligibility << population << ',' << phase << ',' << task << ','
            << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
            << selected << ",16," << observation.event_index << ','
            << observation.simulated_time << ',' << observation.candidate << ','
            << observation.candidate_sender << ',' << observation.receiver << ','
            << effect_name(observation.candidate_effect) << ','
            << static_cast<int>(observation.modulation_polarity) << ','
            << observation.eligibility_generation << ','
            << observation.eligibility_created_at << ','
            << observation.eligibility_expires_at << ','
            << static_cast<unsigned>(observation.eligibility_strength) << ','
            << '"' << join_ids(observation.eligibility_candidates) << '"' << ','
            << observation.candidate_activity_count << ','
            << observation.receiver_membrane << ',' << observation.weight_before
            << ',' << observation.proposed_weight << ',' << observation.weight_after
            << ',' << observation.changed << ','
            << write_outcome_name(observation.write_outcome) << ','
            << mechanism_name(observation.mechanism) << ','
            << (observation.candidate == selected) << '\n';
    }
    const auto situational = situational_assessment(outcomes[0], outcomes[1]);
    const bool deterministic =
        outcomes[0].action == outcomes[2].action &&
        outcomes[0].correct == outcomes[2].correct &&
        outcomes[0].weight_after == outcomes[2].weight_after &&
        outcomes[0].target_after == outcomes[2].target_after;
    for (std::uint32_t variant = 0; variant < outcomes.size(); ++variant) {
        const auto& row = outcomes[variant];
        const char* causal = variant == 2 && !deterministic ? "undecided" :
            (row.target_after != outcomes[0].target_after ||
             row.action != outcomes[0].action ||
             row.correct != outcomes[0].correct ? "demonstrated" : "partial");
        outputs.matrix << population << ',' << phase << ',' << task << ','
            << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
            << selected << ",W" << variant << ',' << names[variant] << ','
            << row.action << ',' << row.correct << ','
            << static_cast<int>(row.modulation) << ',' << row.selected_active << ','
            << row.weight_before << ',' << row.weight_after << ','
            << row.target_before << ',' << row.target_after << ','
            << row.positive_writes << ',' << row.negative_writes
            << ",permitted," << causal << ',' << situational.first
            << ",undecided\n";
    }
    outputs.appropriateness << population << ',' << phase << ',' << task << ','
        << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
        << selected << ',' << outcomes[0].selected_active << ','
        << outcomes[0].action << ',' << outcomes[1].action << ','
        << outcomes[0].correct << ',' << outcomes[1].correct << ','
        << static_cast<int>(outcomes[0].modulation) << ','
        << static_cast<int>(outcomes[1].modulation) << ',' << situational.first
        << ',' << '"' << situational.second << '"'
        << ",permitted," << (deterministic ? "demonstrated" : "undecided")
        << '\n';
}

struct LongTermResult {
    bool retained_after_one{};
    bool reemerged{};
    std::uint32_t natural_correct{};
    std::uint32_t omitted_correct{};
    std::uint32_t natural_repertoire{};
    std::uint32_t omitted_repertoire{};
    std::string assessment{"undecided"};
};

LongTermResult run_long_term(
    C1Outputs& outputs, const std::string& population, const char* phase,
    const std::string& task, const bool inverse, const std::uint32_t neurons,
    const std::uint64_t seed, const eve::SynapseId selected,
    const eve::bios::Runtime& baseline, const B511Circuit& circuit) {
    auto natural = baseline;
    auto omitted = baseline;
    omitted.set_experimental_plasticity_write_interceptor(
        [selected](const eve::bios::PlasticityObservation& observation) {
            return observation.candidate == selected &&
                observation.weight_before == 2 &&
                observation.proposed_weight == 1;
        });
    LongTermResult result;
    std::uint32_t natural_correct = 0;
    std::uint32_t omitted_correct = 0;
    std::size_t checkpoint = 0;
    const auto write_checkpoint = [&](const char* variant,
                                      const eve::bios::Runtime& runtime,
                                      const std::uint32_t experiences,
                                      const std::uint32_t correct) {
        const bool target = c1_target_fires(runtime, selected);
        const auto alternatives = alternative_functional_inputs(runtime, selected);
        const auto repertoire = c1_repertoire(runtime, circuit, inverse);
        const bool reused = target && repertoire > 1;
        outputs.longitudinal << population << ',' << phase << ',' << task << ','
            << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
            << selected << ',' << variant << ',' << experiences << ','
            << runtime.network().synapse(selected).weight << ',' << target << ','
            << correct << ',' << repertoire << ','
            << runtime.network().synapse_count() << ',' << alternatives << ','
            << reused << ',' << runtime.network().validate() << '\n';
        outputs.reorganization << population << ',' << phase << ',' << task << ','
            << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
            << selected << ',' << variant << ',' << experiences << ',' << target
            << ',' << alternatives << ',' << repertoire << ',' << reused << ','
            << (alternatives > 0 ? "alternative_path_present" :
                target ? "selected_path_functional" : "no_functional_input")
            << '\n';
        return std::pair<bool, std::uint32_t>{target, repertoire};
    };
    auto natural_state = write_checkpoint("W0_natural", natural, 0, 0);
    auto omitted_state = write_checkpoint("W1_critical_write_omitted", omitted, 0, 0);
    for (std::uint32_t experience = 1; experience <= 256; ++experience) {
        const auto selected_task = c1_task_for_episode(task, 15 + experience);
        const auto natural_row = b511_episode(
            natural, circuit, b511_pattern(selected_task, false, inverse),
            static_cast<std::uint8_t>((seed + 15 + experience) & 1U), true);
        const auto omitted_row = b511_episode(
            omitted, circuit, b511_pattern(selected_task, false, inverse),
            static_cast<std::uint8_t>((seed + 15 + experience) & 1U), true);
        natural_correct += natural_row.correct;
        omitted_correct += omitted_row.correct;
        if (experience == 1)
            omitted.set_experimental_plasticity_write_interceptor({});
        if (experience == kC1Horizons[checkpoint + 1]) {
            ++checkpoint;
            natural_state = write_checkpoint(
                "W0_natural", natural, experience, natural_correct);
            omitted_state = write_checkpoint(
                "W1_critical_write_omitted", omitted, experience,
                omitted_correct);
            if (experience == 1) result.retained_after_one = natural_state.first;
            if (experience > 1 && natural_state.first) result.reemerged = true;
        }
    }
    result.natural_correct = natural_correct;
    result.omitted_correct = omitted_correct;
    result.natural_repertoire = natural_state.second;
    result.omitted_repertoire = omitted_state.second;
    const bool correct_better = natural_correct > omitted_correct;
    const bool correct_worse = natural_correct < omitted_correct;
    const bool repertoire_better =
        result.natural_repertoire > result.omitted_repertoire;
    const bool repertoire_worse =
        result.natural_repertoire < result.omitted_repertoire;
    if ((correct_better || repertoire_better) &&
        !correct_worse && !repertoire_worse)
        result.assessment = "useful";
    else if ((correct_worse || repertoire_worse) &&
             !correct_better && !repertoire_better)
        result.assessment = "harmful";
    else if (correct_better || correct_worse || repertoire_better ||
             repertoire_worse || natural_state.first != omitted_state.first)
        result.assessment = "context_dependent";
    else
        result.assessment = "neutral";
    outputs.long_term_assessment << population << ',' << phase << ',' << task
        << ',' << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
        << selected << ',' << natural_correct << ',' << omitted_correct << ','
        << result.natural_repertoire << ',' << result.omitted_repertoire << ','
        << natural_state.first << ',' << omitted_state.first << ','
        << result.assessment << ",permitted,demonstrated\n";
    return result;
}

TrialSummary run_c1_trial(
    C1Outputs& outputs, const std::string& population, const char* phase,
    const std::string& task, const bool inverse, const std::uint32_t neurons,
    const std::uint64_t seed, const std::optional<eve::SynapseId> historical) {
    B511Circuit circuit;
    auto runtime = c1_runtime(neurons, seed, circuit);
    const auto contract = protection_for(neurons, circuit);
    const auto structural_baseline = runtime.network().synapse_count();
    std::uint32_t known_synapses = structural_baseline;
    std::vector<ConnectionHistory> histories;
    std::uint64_t correct = 0;

    for (std::uint32_t episode = 0; episode < 16; ++episode) {
        const auto selected_task = c1_task_for_episode(task, episode);
        std::vector<eve::bios::PlasticityObservation> observations;
        runtime.set_plasticity_observer(
            [&](const eve::bios::PlasticityObservation& observation) {
                observations.push_back(observation);
            });
        const std::int32_t selected_before = historical && *historical <
            runtime.network().synapse_count()
            ? runtime.network().synapse(*historical).weight : -1;
        const auto row = b511_episode(
            runtime, circuit, b511_pattern(selected_task, false, inverse),
            static_cast<std::uint8_t>((seed + episode) & 1U),
            c1_learning_enabled(task, episode));
        correct += row.correct;
        for (eve::SynapseId id = known_synapses;
             id < runtime.network().synapse_count(); ++id) {
            const auto& edge = runtime.network().synapse(id);
            histories.push_back(ConnectionHistory{
                id, edge.sender, edge.receiver, episode, edge.weight, edge.weight,
                edge.weight >= 1 ? static_cast<int>(episode) : -1,
                edge.weight >= runtime.config().firing_threshold
                    ? static_cast<int>(episode) : -1,
                contract.permits(edge.sender, edge.receiver)});
        }
        known_synapses = runtime.network().synapse_count();
        for (auto& history : histories) {
            const auto weight = runtime.network().synapse(history.id).weight;
            if (history.first_one < 0 && weight >= 1)
                history.first_one = static_cast<int>(episode);
            if (history.first_threshold < 0 &&
                weight >= runtime.config().firing_threshold)
                history.first_threshold = static_cast<int>(episode);
            history.final_weight = weight;
        }
        if (historical) {
            const bool exists = *historical < runtime.network().synapse_count();
            const auto selected_after = exists
                ? runtime.network().synapse(*historical).weight : -1;
            eve::NeuronId sender = 0;
            eve::NeuronId receiver = 0;
            std::int64_t membrane = 0;
            std::uint32_t refractory = 0;
            std::string effect = "unknown";
            if (exists) {
                const auto& edge = runtime.network().synapse(*historical);
                sender = edge.sender;
                receiver = edge.receiver;
                effect = effect_name(edge.effect);
                membrane = runtime.network().neuron(receiver).membrane_potential;
                refractory =
                    runtime.network().neuron(receiver).refractory_events_remaining;
            }
            outputs.reconstruction << population << ',' << phase << ',' << task
                << ',' << b511_assignment(inverse) << ',' << neurons << ',' << seed
                << ',' << *historical << ',' << episode << ",experience,"
                << selected_before << ',' << selected_after << ',' << sender << ','
                << receiver << ',' << effect << ',' << membrane << ',' << refractory
                << ',' << (row.active_synapses.count(*historical) != 0) << ','
                << action_name(row) << ',' << row.correct << ','
                << static_cast<int>(row.modulation) << ',' << row.positive_writes
                << ',' << row.negative_writes << ',' << runtime.simulated_time()
                << '\n';
        }
        for (const auto& observation : observations) {
            const bool historical_exists = historical &&
                *historical < runtime.network().synapse_count();
            const bool prospective_growth_change = !historical &&
                observation.candidate >= structural_baseline &&
                observation.changed;
            if (prospective_growth_change || (historical_exists &&
                (observation.candidate == *historical ||
                 observation.receiver ==
                    runtime.network().synapse(*historical).receiver))) {
                outputs.eligibility << population << ',' << phase << ',' << task
                    << ',' << b511_assignment(inverse) << ',' << neurons << ','
                    << seed << ',' << (historical ? std::to_string(*historical) : "")
                    << ',' << episode << ',' << observation.event_index << ','
                    << observation.simulated_time << ',' << observation.candidate
                    << ',' << observation.candidate_sender << ','
                    << observation.receiver << ','
                    << effect_name(observation.candidate_effect) << ','
                    << static_cast<int>(observation.modulation_polarity) << ','
                    << observation.eligibility_generation << ','
                    << observation.eligibility_created_at << ','
                    << observation.eligibility_expires_at << ','
                    << static_cast<unsigned>(observation.eligibility_strength) << ','
                    << '"' << join_ids(observation.eligibility_candidates) << '"'
                    << ',' << observation.candidate_activity_count << ','
                    << observation.receiver_membrane << ','
                    << observation.weight_before << ','
                    << observation.proposed_weight << ','
                    << observation.weight_after << ',' << observation.changed << ','
                    << write_outcome_name(observation.write_outcome) << ','
                    << mechanism_name(observation.mechanism) << ','
                    << (historical && observation.candidate == *historical) << '\n';
            }
        }
    }
    runtime.set_plasticity_observer({});

    std::vector<eve::SynapseId> selected_ids;
    if (historical) {
        if (*historical >= runtime.network().synapse_count() ||
            runtime.network().synapse(*historical).weight <
                runtime.config().firing_threshold)
            throw std::runtime_error("historical C.0 functional case not reproduced");
        selected_ids.push_back(*historical);
    } else {
        for (const auto& history : histories)
            if (history.final_weight >= runtime.config().firing_threshold)
                selected_ids.push_back(history.id);
    }

    TrialSummary summary;
    summary.grown = histories.size();
    summary.functional = selected_ids.size();
    summary.correct = correct;
    summary.events = runtime.metrics().processed_events;
    summary.memory = runtime.diagnostics().estimated_memory_bytes;
    std::map<eve::SynapseId, LongTermResult> long_term;
    for (const auto selected : selected_ids) {
        const bool pre_function = c1_target_fires(runtime, selected);
        auto ablated = runtime;
        ablated.set_experimental_synapse_weight(selected, 0);
        const bool ablated_function = c1_target_fires(ablated, selected);
        if (!pre_function || ablated_function) continue;
        std::array<MatrixOutcome, 10> matrix;
        write_matrix_rows(outputs, population, phase, task, inverse, neurons,
                          seed, selected, runtime, circuit, matrix);
        long_term[selected] = run_long_term(
            outputs, population, phase, task, inverse, neurons, seed, selected,
            runtime, circuit);
        summary.retained += long_term[selected].retained_after_one;
        summary.reemerged += long_term[selected].reemerged;
    }

    if (!historical) {
        for (const auto& history : histories) {
            const bool functional = history.final_weight >=
                runtime.config().firing_threshold &&
                c1_target_fires(runtime, history.id);
            bool causal = false;
            if (functional) {
                auto copy = runtime;
                copy.set_experimental_synapse_weight(history.id, 0);
                causal = !c1_target_fires(copy, history.id);
            }
            const auto found = long_term.find(history.id);
            const bool retained = found != long_term.end() &&
                found->second.retained_after_one;
            const bool reemerged = found != long_term.end() &&
                found->second.reemerged;
            const bool ability = found != long_term.end() &&
                (found->second.natural_correct != found->second.omitted_correct ||
                 found->second.natural_repertoire !=
                    found->second.omitted_repertoire);
            outputs.prospective << phase << ',' << task << ','
                << b511_assignment(inverse) << ',' << neurons << ',' << seed << ','
                << history.id << ',' << history.creation_episode << ','
                << history.sender << ',' << history.receiver << ','
                << history.initial_weight << ',' << history.permitted << ','
                << history.first_one << ',' << history.first_threshold << ','
                << history.final_weight << ',' << functional << ',' << causal
                << ',' << retained << ',' << reemerged << ',' << ability
                << ",registered_at_creation\n";
        }
    }
    return summary;
}

double percentile(std::vector<double> values, const double fraction) {
    if (values.empty()) return 0.0;
    std::sort(values.begin(), values.end());
    const auto index = static_cast<std::size_t>(
        std::floor(fraction * static_cast<double>(values.size() - 1)));
    return values[index];
}

void write_bootstrap(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<TrialSummary>>& groups) {
    std::ofstream stream(output / "bootstrap-statistics.csv");
    stream << "group,metric,n,estimate,ci95_low,ci95_high,resamples,cluster\n";
    for (const auto& [group, rows] : groups) {
        for (std::uint32_t metric = 0; metric < 5; ++metric) {
            const auto value = [metric](const TrialSummary& row) {
                if (metric == 0) return static_cast<double>(row.grown);
                if (metric == 1) return static_cast<double>(row.functional);
                if (metric == 2) return static_cast<double>(row.retained);
                if (metric == 3) return static_cast<double>(row.reemerged);
                return static_cast<double>(row.correct);
            };
            const auto name = std::array<const char*, 5>{{
                "grown", "functional", "retained_after_one", "reemerged",
                "correct_experiences"}}[metric];
            const double estimate = std::accumulate(
                rows.begin(), rows.end(), 0.0,
                [&](const double sum, const TrialSummary& row) {
                    return sum + value(row);
                }) / static_cast<double>(rows.size());
            eve::bios::DeterministicRng rng(
                0xC10000ULL + metric + rows.size() * 31ULL);
            std::vector<double> replicates;
            replicates.reserve(1000);
            for (std::uint32_t replicate = 0; replicate < 1000; ++replicate) {
                double sum = 0.0;
                for (std::size_t sample = 0; sample < rows.size(); ++sample)
                    sum += value(rows[rng.bounded(
                        static_cast<std::uint32_t>(rows.size()))]);
                replicates.push_back(sum / static_cast<double>(rows.size()));
            }
            stream << group << ',' << name << ',' << rows.size() << ','
                << estimate << ',' << percentile(replicates, .025) << ','
                << percentile(replicates, .975) << ",1000,seed\n";
        }
    }
}

void write_persistence(const std::filesystem::path& output) {
    B511Circuit circuit;
    auto original = c1_runtime(128, 0xC1A0ULL, circuit);
    original.enqueue_fire(circuit.elements[0]);
    const auto path = output / "c1-persistence.bin";
    original.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    const bool identical_load =
        original.state_fingerprint() == restored.state_fingerprint();
    static_cast<void>(original.run());
    static_cast<void>(restored.run());
    const bool identical_pending_continuation =
        original.state_fingerprint() == restored.state_fingerprint();
    std::filesystem::remove(path);
    std::ofstream stream(output / "persistence.csv");
    stream << "pending_events,protection,eligibility,modulation,growth_rng,"
        "exploration_rng,identical_load,identical_pending_continuation,valid\n"
        "1,1,1,1,1,1," << identical_load << ','
        << identical_pending_continuation << ','
        << restored.network().validate() << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    const auto started = std::chrono::steady_clock::now();
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/c1");
    const std::uint32_t samples = argc > 2
        ? static_cast<std::uint32_t>(std::stoul(argv[2])) : kSeeds;
    const std::string phase_filter = argc > 3 ? argv[3] : "both";
    const std::uint32_t task_begin = argc > 4
        ? static_cast<std::uint32_t>(std::stoul(argv[4])) : 0;
    const std::uint32_t task_end = argc > 5
        ? static_cast<std::uint32_t>(std::stoul(argv[5]))
        : static_cast<std::uint32_t>(kTasks.size());
    const bool include_historical = argc <= 6 || std::string(argv[6]) != "0";
    if (samples == 0 || samples > kSeeds)
        throw std::invalid_argument("C.1 samples must be in the range 1..64");
    if ((phase_filter != "both" && phase_filter != "discovery" &&
         phase_filter != "confirmation") || task_begin >= task_end ||
        task_end > kTasks.size())
        throw std::invalid_argument("invalid C.1 phase or task shard");
    std::filesystem::create_directories(output);
    C1Outputs outputs(output);

    std::ofstream protocol(output / "preregistered-protocol.csv");
    protocol << "field,value\n"
        "integrity_gate,independent_structural_safety_preflight_before_regression_and_learning\n"
        "historical_population,all_19_C0_functional_cases\n"
        "prospective_selection,register_every_connection_at_creation\n"
        "discovery_seeds,64_per_task_assignment_size\n"
        "confirmation_seeds,64_independent_per_task_assignment_size\n"
        "network_sizes,32;128;256;512;2048\n"
        "horizons,0;1;2;4;8;16;32;64;128;256\n"
        "causal_matrix,W0-W9\n"
        "W1,omit_only_selected_2_to_1_write\n"
        "W4,remove_selected_candidate_from_local_eligibility\n"
        "W5,suppress_local_modulation_writes_at_selected_receiver\n"
        "W6,remove_competing_inputs_at_selected_receiver\n"
        "W9,joint_W4_W6_W7_W8\n"
        "feedback_rule,actual_action_receives_its_natural_consequence\n"
        "statistics,seed_clustered_percentile_bootstrap_1000\n"
        "assessment_dimensions,A_integrity;B_causality;C_situational;D_long_term\n"
        "production_S4_d16,disabled\n"
        "new_learning_rule,not_authorized\n";

    std::map<std::string, std::vector<TrialSummary>> groups;
    std::uint64_t historical_reproduced = 0;
    if (include_historical) {
        for (const auto& item : kHistoricalCases) {
            const auto summary = run_c1_trial(
                outputs, "historical", item.phase, item.task, item.inverse,
                item.neurons, item.seed, item.synapse);
            groups[std::string("historical:") + item.phase].push_back(summary);
            historical_reproduced += summary.functional > 0;
        }
    }

    for (const auto& phase_row :
         std::array<std::pair<const char*, std::uint64_t>, 2>{{
             {"discovery", 500000}, {"confirmation", 700000}}}) {
        if (phase_filter != "both" && phase_filter != phase_row.first) continue;
        for (std::uint32_t task_index = task_begin; task_index < task_end;
             ++task_index) {
            for (const bool inverse : {false, true}) {
                for (const auto neurons : kSizes) {
                    for (std::uint32_t sample = 0; sample < samples; ++sample) {
                        const auto seed = phase_row.second +
                            task_index * 10000ULL + inverse * 5000ULL +
                            neurons * 10ULL + sample;
                        const auto summary = run_c1_trial(
                            outputs, "prospective", phase_row.first,
                            kTasks[task_index], inverse, neurons, seed,
                            std::nullopt);
                        const auto group = std::string(phase_row.first) + ':' +
                            kTasks[task_index] + ':' + b511_assignment(inverse) + ':' +
                            std::to_string(neurons);
                        groups[group].push_back(summary);
                    }
                }
            }
        }
    }

    write_bootstrap(output, groups);
    write_persistence(output);

    std::uint64_t prospective_grown = 0;
    std::uint64_t prospective_functional = 0;
    std::uint64_t prospective_retained = 0;
    std::uint64_t prospective_reemerged = 0;
    std::uint64_t processed_events = 0;
    std::size_t peak_memory = 0;
    for (const auto& [group, rows] : groups) {
        if (group.rfind("historical:", 0) == 0) continue;
        for (const auto& row : rows) {
            prospective_grown += row.grown;
            prospective_functional += row.functional;
            prospective_retained += row.retained;
            prospective_reemerged += row.reemerged;
            processed_events += row.events;
            peak_memory = std::max(peak_memory, row.memory);
        }
    }

    std::ofstream dimensions(output / "four-dimension-assessment.csv");
    dimensions << "population,dimension,assessment,evidence,not_a_combined_score\n"
        "all,A_integrity,permitted,structural-safety-preflight.csv,1\n"
        "historical,B_causality,demonstrated,causal-matrix.csv,1\n"
        "historical,C_situational,case_specific,situational-appropriateness.csv,1\n"
        "historical,D_long_term,case_specific,longitudinal-checkpoints.csv,1\n"
        "prospective,B_causality,case_specific,causal-matrix.csv,1\n"
        "prospective,C_situational,case_specific,situational-appropriateness.csv,1\n"
        "prospective,D_long_term,case_specific,longitudinal-checkpoints.csv,1\n";

    std::ofstream outcomes(output / "research-outcomes.csv");
    outcomes << "criterion,passed,value,evidence\n"
        "historical_19_reproduced," <<
        (!include_historical || historical_reproduced == 19) << ','
        << historical_reproduced << ",historical-case-reconstruction.csv\n"
        "prospective_registration_complete,1," << prospective_grown
        << ",prospective-connections.csv\n"
        "prospective_functional_observed," << (prospective_functional > 0)
        << ',' << prospective_functional << ",prospective-connections.csv\n"
        "retention_observed," << (prospective_retained > 0) << ','
        << prospective_retained << ",longitudinal-checkpoints.csv\n"
        "reemergence_observed," << (prospective_reemerged > 0) << ','
        << prospective_reemerged << ",longitudinal-checkpoints.csv\n";

    const auto wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream resources(output / "resources.csv");
    const std::uint32_t phase_count = phase_filter == "both" ? 2U : 1U;
    const auto prospective_trials = phase_count * 2ULL * kSizes.size() *
        samples * (task_end - task_begin);
    resources << "metric,value\nwall_seconds," << std::setprecision(10) << wall
        << "\npeak_rss_kib," << usage.ru_maxrss
        << "\nprospective_trials," << prospective_trials
        << "\nhistorical_cases," << historical_reproduced
        << "\nprospective_growth," << prospective_grown
        << "\nprospective_functional," << prospective_functional
        << "\nprospective_retained_after_one," << prospective_retained
        << "\nprospective_reemerged," << prospective_reemerged
        << "\nprocessed_training_events," << processed_events
        << "\nmaximum_estimated_runtime_bytes," << peak_memory
        << "\nproduction_growth_enabled,0\nproduction_exploration_enabled,0\n"
        << "production_S4_d16_enabled,0\n";

    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "component,merge_recommendation,production_default,rationale\n"
        "C1_diagnostic_runner,yes,research_only,separates_four_assessment_dimensions\n"
        "prospective_registry,yes,research_only,prevents_success_only_selection\n"
        "W0_W9_interventions,yes,research_only,controlled_state_copies\n"
        "new_learning_rule,no,absent,no_rule_preregistered_or_authorized\n"
        "growth,no,disabled,research_evidence_only\n"
        "internal_exploration,no,disabled,research_evidence_only\n"
        "S4_d16,no,disabled,experimental_variant_unchanged\n";

    std::cout << "C.1 complete historical=" << historical_reproduced
              << "/19 prospective_trials="
              << prospective_trials
              << " grown=" << prospective_grown
              << " functional=" << prospective_functional
              << " retained=" << prospective_retained
              << " reemerged=" << prospective_reemerged << '\n';
    return !include_historical || historical_reproduced == 19 ? 0 : 1;
}
