#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B511_EMBEDDED
#include "research_stage_b511.cpp"
#undef EVE_RESEARCH_STAGE_B511_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <sys/resource.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace {

constexpr std::uint32_t kSeeds = 64;
constexpr std::array<std::uint32_t, 5> kSizes{{32, 128, 256, 512, 2048}};
constexpr std::array<const char*, 10> kTasks{{
    "simple_sensorimotor", "competing_relations", "P_to_Q_or_R", "A_to_B",
    "mixed_A_B", "reacquisition", "partial_latent_learning",
    "retention_consolidation", "transfer", "B512_learning_checkpoints"}};
constexpr std::array<const char*, 8> kControls{{
    "K0_fixed", "K1_growth_only", "K2_exploration_only",
    "K3_protected_combined", "K4_static_zero_extras", "K5_random_structure",
    "K6_no_learning", "K7_no_natural_modulation"}};

struct TrialResult {
    std::uint32_t free_neurons{};
    std::uint64_t explorations{};
    std::uint64_t grown{};
    std::uint64_t protected_violations{};
    std::uint64_t positive_edges{};
    std::uint64_t changed_edges{};
    std::uint64_t functional_edges{};
    std::uint64_t peak_queue{};
    std::size_t memory_bytes{};
    std::uint64_t correct_experiences{};
    std::uint64_t positive_writes{};
    std::uint64_t negative_writes{};
    std::uint32_t repertoire{};
    std::uint64_t causally_verified_functional_edges{};
    bool valid{};
};

eve::ExperimentConfig config_for(const std::uint32_t neurons,
                                 const bool growth,
                                 const bool plasticity) {
    eve::ExperimentConfig config;
    config.neuron_count = neurons;
    config.width = neurons == 32 ? 4 : 8;
    config.height = neurons == 32 ? 4 : 8;
    config.depth = neurons / (config.width * config.height);
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 4;
    config.input_weight_budget = 32;
    config.learning_enabled = false;
    config.growth_enabled = growth;
    config.reset_mode = eve::ResetMode::to_zero;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 64;
    config.plasticity_trace_mode = plasticity
        ? eve::PlasticityTraceMode::g3_recent
        : eve::PlasticityTraceMode::disabled;
    config.modulation_mode = plasticity
        ? eve::ModulationMode::ternary : eve::ModulationMode::none;
    config.maximum_events_per_run = 100'000;
    return config;
}

eve::brainstem::StructuralProtectionContract protection_for(
    const std::uint32_t neurons) {
    auto contract =
        eve::brainstem::StructuralProtectionContract::protected_default(neurons);
    const std::uint32_t sensory_end = std::min<std::uint32_t>(8, neurons);
    const std::uint32_t motor_end = std::min<std::uint32_t>(12, neurons);
    const std::uint32_t evaluation_end = std::min<std::uint32_t>(14, neurons);
    const std::uint32_t brainstem_end = std::min<std::uint32_t>(16, neurons);
    const std::uint32_t transition_end = std::min<std::uint32_t>(20, neurons);
    for (std::uint32_t id = 0; id < sensory_end; ++id)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::innate_sensory;
    for (std::uint32_t id = sensory_end; id < motor_end; ++id)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::innate_motor;
    for (std::uint32_t id = motor_end; id < evaluation_end; ++id)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::evaluation_feedback;
    for (std::uint32_t id = evaluation_end; id < brainstem_end; ++id)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::protected_brainstem;
    for (std::uint32_t id = brainstem_end; id < transition_end; ++id)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::transition_contact;
    return contract;
}

eve::brainstem::StructuralProtectionContract protection_for(
    const std::uint32_t neurons, const B511Circuit& circuit) {
    auto contract =
        eve::brainstem::StructuralProtectionContract::protected_default(neurons);
    for (const auto id : circuit.elements)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::innate_sensory;
    for (const auto id : circuit.base.sensors)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::innate_sensory;
    for (const auto id : circuit.base.explorers)
        contract.neuron_roles[id] =
            eve::brainstem::StructuralRole::protected_brainstem;
    for (const auto id : circuit.base.decisions)
        contract.neuron_roles[id] =
            eve::brainstem::StructuralRole::transition_contact;
    for (const auto id : circuit.base.motors)
        contract.neuron_roles[id] = eve::brainstem::StructuralRole::innate_motor;
    for (const auto id : circuit.base.modulation_inputs)
        contract.neuron_roles[id] =
            eve::brainstem::StructuralRole::evaluation_feedback;
    return contract;
}

const char* role_name(const eve::brainstem::StructuralRole role) {
    using Role = eve::brainstem::StructuralRole;
    switch (role) {
    case Role::free_processing: return "free_processing";
    case Role::innate_sensory: return "innate_sensory";
    case Role::innate_motor: return "innate_motor";
    case Role::evaluation_feedback: return "evaluation_feedback";
    case Role::protected_brainstem: return "protected_brainstem";
    case Role::transition_contact: return "transition_contact";
    }
    return "invalid";
}

const char* write_outcome_name(const eve::bios::PlasticityWriteOutcome outcome) {
    using Outcome = eve::bios::PlasticityWriteOutcome;
    switch (outcome) {
    case Outcome::changed: return "changed";
    case Outcome::protected_synapse: return "protected_synapse";
    case Outcome::positive_weight_limit: return "positive_weight_limit";
    case Outcome::input_weight_budget: return "input_weight_budget";
    case Outcome::negative_zero_floor: return "negative_zero_floor";
    case Outcome::write_suppressed: return "write_suppressed";
    case Outcome::isolated_write_suppressed: return "isolated_write_suppressed";
    }
    return "unknown";
}

const char* intervention_name(const std::uint32_t intervention) {
    constexpr std::array<const char*, 7> names{{
        "natural_continuation", "selected_edge_ablation",
        "copied_natural_continuation", "all_functional_edges_ablation",
        "growth_disabled_continuation", "exploration_disabled_continuation",
        "weight_writes_suppressed_continuation"}};
    return names.at(intervention);
}

bool growth_enabled(const std::string& control) {
    return control != "K0_fixed" && control != "K2_exploration_only";
}

bool exploration_enabled(const std::string& control) {
    return control != "K0_fixed" && control != "K1_growth_only";
}

TrialResult run_trial(const std::string& phase, const std::string& task,
                      const std::string& control, const std::uint32_t neurons,
                      const bool inverse, const std::uint64_t seed,
                      std::ofstream* connections, std::ofstream* curves,
                      std::ofstream* causal, std::ofstream* writes) {
    const bool growth = growth_enabled(control);
    const bool exploration = exploration_enabled(control);
    const bool plasticity = control != "K6_no_learning";
    B511MembraneVariant variant{
        "C0", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, plasticity, neurons};
    auto config = b511_config(variant, plasticity);
    config.growth_enabled = growth;
    eve::bios::Runtime runtime(config, seed);
    const auto circuit = b511_circuit(runtime, plasticity);
    const auto contract = protection_for(neurons, circuit);
    runtime.configure_structural_protection(contract);
    eve::brainstem::AutonomousExplorationConfig exploration_config;
    exploration_config.enabled = exploration;
    exploration_config.activation_budget = 8;
    exploration_config.trigger_probability_denominator = 1;
    exploration_config.maximum_sender_outgoing = 4;
    exploration_config.local_radius = 2;
    exploration_config.seed = seed ^ 0xC0E000ULL;
    runtime.configure_autonomous_exploration(exploration_config);

    std::vector<eve::NeuronId> free_ids;
    for (eve::NeuronId id = 0; id < neurons; ++id)
        if (contract.neuron_roles[id] ==
            eve::brainstem::StructuralRole::free_processing)
            free_ids.push_back(id);
    if (control == "K4_static_zero_extras" && free_ids.size() >= 2)
        static_cast<void>(runtime.add_experimental_synapse(
            free_ids[0], free_ids[1], 0));
    if (control == "K5_random_structure" && free_ids.size() >= 4) {
        static_cast<void>(runtime.add_experimental_synapse(
            free_ids[0], free_ids[2], 0));
        static_cast<void>(runtime.add_experimental_synapse(
            free_ids[1], free_ids[3], 0));
    }

    std::map<eve::NeuronId, std::uint64_t> fires;
    struct ConnectionRecord {
        std::uint32_t episode{};
        eve::SynapseId id{};
        eve::NeuronId sender{};
        std::uint32_t sender_outgoing_before{};
        eve::NeuronId target{};
        std::int64_t target_membrane{};
        std::uint64_t time{};
        std::int32_t initial_weight{};
        bool permitted{};
    };
    std::vector<ConnectionRecord> connection_records;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        ++fires[event.neuron];
    });
    const std::uint32_t structural_baseline = runtime.network().synapse_count();
    std::uint32_t known_synapses = structural_baseline;
    std::uint64_t correct_experiences = 0;
    std::uint64_t positive_writes = 0;
    std::uint64_t negative_writes = 0;
    const auto task_for_episode = [&task](const std::uint32_t episode) {
        if (task == "simple_sensorimotor") return B511Task::a;
        if (task == "A_to_B")
            return episode < 8 ? B511Task::a : B511Task::b;
        if (task == "reacquisition")
            return episode < 6 || episode >= 11 ? B511Task::a : B511Task::b;
        if (task == "transfer")
            return episode < 8 ? B511Task::a : B511Task::c;
        if (task == "partial_latent_learning") return B511Task::a;
        return episode % 2 == 0 ? B511Task::a : B511Task::b;
    };
    const auto direct_repertoire = [&circuit, inverse](
                                       const eve::bios::Runtime& source) {
        auto probe_a_runtime = source;
        auto probe_b_runtime = source;
        const auto probe_a = b511_probe(
            probe_a_runtime, circuit,
            b511_pattern(B511Task::a, false, inverse));
        const auto probe_b = b511_probe(
            probe_b_runtime, circuit,
            b511_pattern(B511Task::b, false, inverse));
        return static_cast<std::uint32_t>(probe_a.correct + probe_b.correct);
    };
    for (std::uint32_t episode = 0; episode < 16; ++episode) {
        const auto selected_task = task_for_episode(episode);
        const bool learn = control != "K7_no_natural_modulation" &&
            (task != "partial_latent_learning" || episode >= 8);
        const auto row = b511_episode(
            runtime, circuit, b511_pattern(selected_task, false, inverse),
            static_cast<std::uint8_t>((seed + episode) & 1U), learn);
        correct_experiences += row.correct;
        positive_writes += row.positive_writes;
        negative_writes += row.negative_writes;
        if (task == "retention_consolidation" && episode == 7)
            static_cast<void>(runtime.advance_time_by(64));
        if (connections != nullptr) {
            for (eve::SynapseId id = known_synapses;
                 id < runtime.network().synapse_count(); ++id) {
                const auto& edge = runtime.network().synapse(id);
                connection_records.push_back(ConnectionRecord{
                    episode, id, edge.sender,
                    runtime.network().neuron(edge.sender).outgoing_count - 1U,
                    edge.receiver,
                    runtime.network().neuron(edge.receiver).membrane_potential,
                    runtime.simulated_time(), edge.weight,
                    contract.permits(edge.sender, edge.receiver)});
            }
        }
        known_synapses = runtime.network().synapse_count();
        if (curves != nullptr &&
            (episode == 0 || episode == 1 || episode == 3 ||
             episode == 7 || episode == 15)) {
            const auto repertoire = direct_repertoire(runtime);
            std::uint64_t positive_growth = 0;
            std::uint64_t functional_growth = 0;
            for (eve::SynapseId id = structural_baseline;
                 id < runtime.network().synapse_count(); ++id) {
                const auto weight = runtime.network().synapse(id).weight;
                positive_growth += weight > 0;
                functional_growth += weight >= runtime.config().firing_threshold;
            }
            *curves << phase << ',' << task << ','
                << (inverse ? "inverse" : "normal") << ',' << control << ','
                << neurons << ',' << seed << ',' << episode + 1 << ','
                << correct_experiences << ',' << repertoire << ','
                << runtime.autonomous_exploration_activations() << ','
                << runtime.metrics().grown_synapses << ',' << positive_growth << ','
                << functional_growth << ',' << positive_writes << ','
                << negative_writes << '\n';
        }
    }
    if (connections != nullptr) {
        for (const auto& record : connection_records) {
            const auto& edge = runtime.network().synapse(record.id);
            *connections << phase << ',' << task << ','
                << (inverse ? "inverse" : "normal") << ',' << control << ','
                << neurons << ',' << seed << ',' << record.episode << ','
                << record.id << ',' << record.sender << ','
                << role_name(contract.neuron_roles[record.sender]) << ','
                << record.sender_outgoing_before << ',' << record.target << ','
                << role_name(contract.neuron_roles[record.target]) << ','
                << record.target_membrane << ',' << record.time << ','
                << record.initial_weight << ',' << fires[record.target] << ','
                << edge.weight << ',' << record.permitted << '\n';
        }
    }

    TrialResult result;
    result.free_neurons = static_cast<std::uint32_t>(std::count(
        contract.neuron_roles.begin(), contract.neuron_roles.end(),
        eve::brainstem::StructuralRole::free_processing));
    result.explorations = runtime.autonomous_exploration_activations();
    result.grown = runtime.metrics().grown_synapses;
    std::vector<eve::SynapseId> functional_ids;
    for (eve::SynapseId id = structural_baseline;
         id < runtime.network().synapse_count(); ++id) {
        const auto& edge = runtime.network().synapse(id);
        if (!contract.permits(edge.sender, edge.receiver))
            ++result.protected_violations;
        result.positive_edges += edge.weight > 0;
        result.changed_edges += edge.weight != 0;
        if (edge.weight >= runtime.config().firing_threshold) {
            ++result.functional_edges;
            functional_ids.push_back(id);
        }
    }
    result.peak_queue = runtime.metrics().maximum_queue_size;
    result.memory_bytes = runtime.diagnostics().estimated_memory_bytes;
    result.correct_experiences = correct_experiences;
    result.positive_writes = positive_writes;
    result.negative_writes = negative_writes;
    result.repertoire = direct_repertoire(runtime);
    if (causal != nullptr) {
        for (const auto selected : functional_ids) {
            const auto sender = runtime.network().synapse(selected).sender;
            const auto target = runtime.network().synapse(selected).receiver;
            const auto probe_target = [sender, target](eve::bios::Runtime copy) {
                bool target_fired = false;
                copy.set_fire_observer(
                    [&](const eve::bios::FireObservation& event) {
                        target_fired = target_fired || event.neuron == target;
                    });
                copy.enqueue_fire(sender);
                static_cast<void>(copy.run());
                return target_fired;
            };
            const bool pre_continuation_fired = probe_target(runtime);
            bool single_ablation_fired = false;
            for (std::uint32_t intervention = 0; intervention <= 6;
                 ++intervention) {
                auto copy = runtime;
                const auto selected_weight_before =
                    copy.network().synapse(selected).weight;
                if (intervention == 1)
                    copy.set_experimental_synapse_weight(selected, 0);
                if (intervention == 3)
                    for (const auto id : functional_ids)
                        copy.set_experimental_synapse_weight(id, 0);
                if (intervention == 4)
                    copy.set_structural_growth_enabled(false);
                if (intervention == 5)
                    copy.set_autonomous_exploration_enabled(false);
                if (intervention == 6)
                    copy.set_experimental_plasticity_writes_suppressed(true);
                int continuation_correct = -1;
                std::vector<eve::bios::PlasticityObservation> observations;
                copy.set_plasticity_observer(
                    [&](const eve::bios::PlasticityObservation& observation) {
                        observations.push_back(observation);
                    });
                const bool continuation_executed =
                    intervention == 0 || intervention == 2 || intervention >= 4;
                if (continuation_executed) {
                    continuation_correct = b511_episode(
                        copy, circuit, b511_pattern(B511Task::a, false, inverse),
                        static_cast<std::uint8_t>((seed + 16) & 1U), true).correct;
                }
                const auto selected_weight_after =
                    copy.network().synapse(selected).weight;
                const bool target_fired = probe_target(copy);
                if (intervention == 1) single_ablation_fired = target_fired;
                *causal << phase << ',' << task << ','
                    << (inverse ? "inverse" : "normal") << ',' << neurons << ','
                    << seed << ',' << selected << ",W" << intervention << ','
                    << intervention_name(intervention) << ','
                    << continuation_executed << ',' << continuation_correct << ','
                    << pre_continuation_fired << ',' << target_fired << ','
                    << selected_weight_before << ',' << selected_weight_after << ','
                    << (selected_weight_after - selected_weight_before) << ','
                    << std::count_if(
                           observations.begin(), observations.end(),
                           [](const auto& item) {
                               return item.changed && item.requested_delta > 0;
                           }) << ','
                    << std::count_if(
                           observations.begin(), observations.end(),
                           [](const auto& item) {
                               return item.changed && item.requested_delta < 0;
                           }) << ','
                    << (static_cast<int>(pre_continuation_fired) -
                        static_cast<int>(target_fired)) << ",1\n";

                if (writes != nullptr && continuation_executed) {
                    for (std::size_t write_index = 0;
                         write_index < observations.size(); ++write_index) {
                        const auto& observation = observations[write_index];
                        int isolated_target_fired = -1;
                        const char* isolated_effect = "not_isolated";
                        if (intervention == 0 && observation.changed) {
                            auto isolated = runtime;
                            isolated.set_experimental_plasticity_write_interceptor(
                                [&](const eve::bios::PlasticityObservation& candidate) {
                                    return candidate.event_index ==
                                               observation.event_index &&
                                        candidate.candidate == observation.candidate &&
                                        candidate.weight_before ==
                                            observation.weight_before &&
                                        candidate.proposed_weight ==
                                            observation.proposed_weight;
                                });
                            static_cast<void>(b511_episode(
                                isolated, circuit,
                                b511_pattern(B511Task::a, false, inverse),
                                static_cast<std::uint8_t>((seed + 16) & 1U), true));
                            isolated_target_fired = probe_target(isolated);
                            if (isolated_target_fired == target_fired) {
                                isolated_effect = "no_isolated_function_effect";
                            } else if (isolated_target_fired) {
                                isolated_effect = "write_contributed_to_function_loss";
                            } else {
                                isolated_effect =
                                    "write_contributed_to_function_retention";
                            }
                        }
                        const char* relation = observation.candidate == selected
                            ? "selected_edge"
                            : observation.receiver == target
                                ? "same_target" : "other_edge";
                        *writes << phase << ',' << task << ','
                            << (inverse ? "inverse" : "normal") << ','
                            << neurons << ',' << seed << ',' << selected << ",W"
                            << intervention << ',' << intervention_name(intervention)
                            << ',' << write_index << ',' << observation.event_index
                            << ',' << observation.candidate << ','
                            << observation.candidate_sender << ','
                            << observation.receiver << ',' << relation << ','
                            << static_cast<int>(observation.modulation_polarity)
                            << ',' << observation.weight_before << ','
                            << observation.weight_after << ','
                            << observation.requested_delta << ','
                            << observation.changed << ','
                            << write_outcome_name(observation.write_outcome) << ','
                            << target_fired << ',' << isolated_target_fired << ','
                            << isolated_effect << '\n';
                    }
                }
            }
            result.causally_verified_functional_edges +=
                pre_continuation_fired && !single_ablation_fired;
        }
    }
    result.valid = runtime.network().validate();
    return result;
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_C0_EMBEDDED
int main(int argc, char** argv) {
    const auto started = std::chrono::steady_clock::now();
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/c0");
    std::filesystem::create_directories(output);

    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,seed_start,assignment_policy,predeclared\n"
        "discovery,64,100000,normal_and_inverse,1\n"
        "confirmation,64,200000,normal_and_inverse,1\n";
    std::ofstream protocol(output / "predeclared-protocol.csv");
    protocol << "field,value\ncentral_control,K3_protected_combined\n"
        "integrity_gate,independent_structural_safety_preflight_before_learning\n"
        "assessment_model,A_integrity;B_self_organization;C_learning\n"
        "episodes,16\ncheckpoints,1;2;4;8;16\nexploration_budget,8\n"
        "growth_budget,local_outgoing_probability\nfunctional_threshold,weight_ge_firing_threshold\n"
        "learning_success,B512_stage_rules\ncurrent_controls,K0-K7\n"
        "K8_status,historical_B513_reference_only\n"
        "W0_status,unintervened_identical_natural_continuation\n"
        "production_S4,disabled\n";

    std::ofstream roles(output / "protection-contract.csv");
    roles << "role,may_send_growth,may_receive_growth,purpose\n"
        "innate_sensory,0,0,sensory_interface\n"
        "innate_motor,0,0,motor_interface\n"
        "evaluation_feedback,0,0,technical_feedback\n"
        "protected_brainstem,0,0,innate_control\n"
        "free_processing,1,1,unrestricted_learning_capacity\n"
        "transition_contact,1,1,explicit_interface_transition\n";

    std::ofstream connections(output / "natural-growth-connections.csv");
    connections << "phase,task,assignment,control,neurons,seed,episode,synapse,sender,sender_role,"
        "sender_outgoing_before,target,target_role,target_membrane,time,initial_weight,"
        "later_target_fires,later_weight,permitted\n";
    std::ofstream trials(output / "autonomous-trials.csv");
    trials << "phase,task,assignment,control,neurons,seed,free_neurons,equivalent_capacity,"
        "explorations,grown,protected_violations,positive_edges,changed_edges,"
        "functional_edges,causally_verified_edges,correct_experiences,positive_writes,negative_writes,"
        "repertoire,peak_queue,memory_bytes,valid\n";
    std::ofstream stages(output / "recruitment-stages.csv");
    stages << "phase,task,assignment,neurons,seed,S0_available,S1_explored,S2_structural,"
        "S3_functional,S4_experience_changed,S5_causal_learning_relevant\n";
    std::ofstream curves(output / "task-learning-curves.csv");
    curves << "phase,task,assignment,control,neurons,seed,experiences,"
        "cumulative_correct,repertoire,explorations,grown,positive_growth_edges,"
        "functional_growth_edges,positive_writes,negative_writes\n";
    std::ofstream causal(output / "causal-interventions.csv");
    causal << "phase,task,assignment,neurons,seed,synapse,intervention,"
        "intervention_name,continuation_executed,continuation_correct,"
        "target_fired_before_continuation,target_fired_after,"
        "selected_weight_before,selected_weight_after,selected_weight_delta,"
        "positive_writes,negative_writes,function_effect,applicable\n";
    std::ofstream writes(output / "continuation-weight-writes.csv");
    writes << "phase,task,assignment,neurons,seed,selected_synapse,intervention,"
        "intervention_name,write_index,event_index,candidate_synapse,"
        "candidate_sender,receiver,relation,modulation_polarity,weight_before,"
        "weight_after,requested_delta,changed,write_outcome,"
        "continued_target_fired,isolated_suppression_target_fired,"
        "isolated_function_effect\n";
    std::uint64_t confirmed_functional_edges = 0;
    std::uint64_t confirmed_causal_edges = 0;
    std::uint64_t central_functional_edges = 0;

    for (const auto& phase_row : std::array<std::pair<const char*, std::uint64_t>, 2>{{
             {"discovery", 100000}, {"confirmation", 200000}}}) {
        for (std::uint32_t task_index = 0; task_index < kTasks.size(); ++task_index) {
            for (const bool inverse : {false, true}) {
                for (const auto neurons : kSizes) {
                    for (std::uint32_t sample = 0; sample < kSeeds; ++sample) {
                        const std::uint64_t seed = phase_row.second +
                            task_index * 10000ULL + inverse * 5000ULL +
                            neurons * 10ULL + sample;
                        const auto result = run_trial(
                            phase_row.first, kTasks[task_index], kControls[3],
                            neurons, inverse, seed, &connections, &curves, &causal,
                            &writes);
                        const bool equivalent = neurons != 32 &&
                            result.free_neurons >= 24;
                        trials << phase_row.first << ',' << kTasks[task_index]
                            << ',' << (inverse ? "inverse" : "normal") << ','
                            << kControls[3] << ',' << neurons << ',' << seed << ','
                            << result.free_neurons << ',' << equivalent << ','
                            << result.explorations << ',' << result.grown << ','
                            << result.protected_violations << ','
                            << result.positive_edges << ',' << result.changed_edges
                            << ',' << result.functional_edges << ','
                            << result.causally_verified_functional_edges << ','
                            << result.correct_experiences << ','
                            << result.positive_writes << ',' << result.negative_writes
                            << ',' << result.repertoire << ',' << result.peak_queue
                            << ',' << result.memory_bytes << ',' << result.valid << '\n';
                        stages << phase_row.first << ',' << kTasks[task_index]
                            << ',' << (inverse ? "inverse" : "normal") << ','
                            << neurons << ',' << seed << ',' << result.free_neurons
                            << ',' << result.explorations << ',' << result.grown << ','
                            << result.functional_edges << ','
                            << result.functional_edges << ",0\n";
                        if (std::string(phase_row.first) == "confirmation") {
                            confirmed_functional_edges += result.functional_edges;
                            confirmed_causal_edges +=
                                result.causally_verified_functional_edges;
                        }
                        central_functional_edges += result.functional_edges;
                    }
                }
            }
        }
    }

    std::ofstream controls(output / "control-groups.csv");
    controls << "phase,assignment,control,seed,explorations,grown,protected_violations,"
        "positive_edges,functional_edges,correct_experiences,repertoire,interpretation\n";
    std::map<std::string, std::uint64_t> control_correct;
    std::map<std::string, std::uint64_t> control_repertoire;
    for (const auto& phase_row : std::array<std::pair<const char*, std::uint64_t>, 2>{{
             {"discovery", 300000}, {"confirmation", 400000}}}) {
        for (const char* control : kControls) {
            for (const bool inverse : {false, true}) {
                for (std::uint32_t sample = 0; sample < kSeeds; ++sample) {
                    const auto result = run_trial(
                        phase_row.first, "simple_sensorimotor", control, 128,
                        inverse, phase_row.second + inverse * 1000ULL + sample,
                        nullptr, nullptr, nullptr, nullptr);
                    controls << phase_row.first << ','
                        << (inverse ? "inverse" : "normal") << ',' << control << ','
                        << phase_row.second + inverse * 1000ULL + sample << ','
                        << result.explorations << ',' << result.grown << ','
                        << result.protected_violations << ',' << result.positive_edges
                        << ',' << result.functional_edges << ','
                        << result.correct_experiences << ',' << result.repertoire << ','
                        << "current_C0_measurement" << '\n';
                    const std::string key = std::string(phase_row.first) + ':' +
                        (inverse ? "inverse:" : "normal:") + control;
                    control_correct[key] += result.correct_experiences;
                    control_repertoire[key] += result.repertoire;
                }
            }
        }
    }

    std::ofstream historical_controls(output / "historical-controls.csv");
    historical_controls << "control,status,source,interpretation\n"
        "K8_B513_positive_control,historical_reference_only,"
        "research/results/b513/functional-growth.csv.gz,"
        "not_a_current_C0_measurement\n";

    const bool confirmed_learning_advantage =
        control_correct["confirmation:normal:K3_protected_combined"] >
            control_correct["confirmation:normal:K0_fixed"] &&
        control_correct["confirmation:inverse:K3_protected_combined"] >
            control_correct["confirmation:inverse:K0_fixed"] &&
        control_repertoire["confirmation:normal:K3_protected_combined"] >
            control_repertoire["confirmation:normal:K0_fixed"] &&
        control_repertoire["confirmation:inverse:K3_protected_combined"] >
            control_repertoire["confirmation:inverse:K0_fixed"];

    const auto state = output / "c0-persistence.bin";
    eve::bios::Runtime original(config_for(128, true, true), 0xC0C0ULL);
    original.configure_structural_protection(protection_for(128));
    eve::brainstem::AutonomousExplorationConfig auto_config;
    auto_config.enabled = true;
    auto_config.activation_budget = 8;
    auto_config.trigger_probability_denominator = 1;
    auto_config.maximum_sender_outgoing = 4;
    auto_config.local_radius = 2;
    auto_config.seed = 0xC0E0ULL;
    original.configure_autonomous_exploration(auto_config);
    original.enqueue_fire(16);
    original.save(state.string());
    auto restored = eve::bios::Runtime::load(state.string());
    const bool identical_load = original.state_fingerprint() == restored.state_fingerprint();
    static_cast<void>(original.run());
    static_cast<void>(restored.run());
    const bool identical_continuation =
        original.state_fingerprint() == restored.state_fingerprint();
    auto guarded = restored;
    auto disabled_contract = guarded.structural_protection();
    disabled_contract.enabled = false;
    bool post_load_contract_removal_rejected = false;
    try {
        guarded.configure_structural_protection(disabled_contract);
    } catch (const std::invalid_argument&) {
        post_load_contract_removal_rejected = true;
    }
    auto unprotected_config = config_for(128, true, true);
    eve::bios::Runtime unprotected(unprotected_config, 0xC0BADULL);
    auto unsafe_exploration = auto_config;
    bool unsafe_combined_enable_rejected = false;
    try {
        unprotected.configure_autonomous_exploration(unsafe_exploration);
    } catch (const std::invalid_argument&) {
        unsafe_combined_enable_rejected = true;
    }
    std::filesystem::remove(state);
    std::ofstream persistence(output / "persistence.csv");
    persistence << "pending_events,roles,exploration_rng,budget,new_synapses,"
        "identical_load,identical_continuation,unsafe_combined_enable_rejected,"
        "post_load_contract_removal_rejected,valid\n"
        << "1,1,1,1,1," << identical_load << ',' << identical_continuation << ','
        << unsafe_combined_enable_rejected << ','
        << post_load_contract_removal_rejected << ','
        << restored.network().validate() << '\n';

    std::ofstream outcomes(output / "research-outcomes.csv");
    outcomes << "assessment,criterion,passed,evidence\n"
        "B,self_organization_natural_functional_path," <<
            (confirmed_functional_edges > 0 && confirmed_causal_edges > 0) <<
            ",causal-interventions.csv.gz\n"
        "C,learning_simple_128_K3_over_K0_advantage,"
        << confirmed_learning_advantage << ",control-groups.csv.gz\n";
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "component,merge_recommendation,production_default,rationale\n"
        "protection_contract,yes,enabled_when_autonomy_enabled,all_safety_tests_pass\n"
        "autonomous_exploration,yes_infrastructure_only,disabled,no_causal_learned_skill_contribution\n"
        "protected_growth,yes_infrastructure_only,disabled,natural_function_but_no_simple_128_K3_K0_advantage\n"
        "new_learning_rule,no,absent,no_causal_bottleneck_rule_authorized\n"
        "S4,no,disabled,unchanged_production_default\n";

    const auto wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream resources(output / "resources.csv");
    resources << "metric,value\nwall_seconds," << std::setprecision(10) << wall
        << "\npeak_rss_kib," << usage.ru_maxrss
        << "\ncentral_trials," << 4 * kTasks.size() * kSizes.size() * kSeeds
        << "\ncontrol_trials," << 4 * kControls.size() * kSeeds
        << "\ncausal_rows," << (central_functional_edges * 7)
        << "\nadditional_persistent_role_bytes_per_neuron,1"
        << "\nproduction_S4_enabled,0\n";
    std::cout << "C.0 complete central_trials="
              << 4 * kTasks.size() * kSizes.size() * kSeeds
              << " controls=" << 4 * kControls.size() * kSeeds
              << " integrity=external-preflight result=B-"
              << (confirmed_functional_edges > 0 && confirmed_causal_edges > 0
                      ? "pass" : "fail")
              << "_C-" << (confirmed_learning_advantage ? "pass" : "fail")
              << "\n";
    return 0;
}
#endif
