#define EVE_RESEARCH_STAGE_B45_EMBEDDED
#include "research_stage_b45.cpp"
#undef EVE_RESEARCH_STAGE_B45_EMBEDDED

#include <cstdio>
#include <tuple>

namespace {

enum class B46Task {
    only_x,
    only_y,
    x_then_y,
    y_then_x,
    alternating,
    mixed,
    reversal,
    retention,
};

struct B46Variant {
    std::string name;
    std::string family;
    B46Task task{B46Task::mixed};
    bool inhibition{};
    bool inhibition_effect{true};
    bool inhibitory_plasticity{};
    eve::InhibitoryPlasticityMode inhibitory_rule{
        eve::InhibitoryPlasticityMode::shared};
    std::int32_t initial_inhibition{};
    bool negative_ambiguity{true};
    bool plasticity{true};
    bool swapped_assignment{};
    bool reverse_construction{};
};

struct B46Plan {
    std::uint8_t cue{};
    std::uint8_t exploration{};
};

struct B46Circuit {
    eve::NeuronId decision_a{4};
    eve::NeuronId decision_b{5};
    eve::NeuronId motor_a{6};
    eve::NeuronId motor_b{7};
    std::array<eve::SynapseId, 4> cue_synapses{};
    std::array<eve::SynapseId, 2> exploration_synapses{};
    std::array<eve::SynapseId, 2> excitatory_outputs{};
    std::array<eve::SynapseId, 2> inhibitory_outputs{};
};

struct B46Result {
    std::uint64_t seed{};
    std::uint64_t cue0_correct{};
    std::uint64_t cue0_wrong{};
    std::uint64_t cue0_both{};
    std::uint64_t cue0_none{};
    std::uint64_t cue1_correct{};
    std::uint64_t cue1_wrong{};
    std::uint64_t cue1_both{};
    std::uint64_t cue1_none{};
    std::uint64_t training_correct{};
    std::uint64_t training_wrong{};
    std::uint64_t training_both{};
    std::uint64_t training_none{};
    std::uint64_t inhibitory_impulses{};
    std::uint64_t inhibitory_strength{};
    std::uint64_t inhibitory_increases{};
    std::uint64_t inhibitory_decreases{};
    std::uint64_t learned_inhibitory_connections{};
    std::uint64_t processed_events{};
    std::uint64_t maximum_queue{};
    std::array<std::int32_t, 4> cue_weights{};
    std::array<std::int32_t, 2> inhibitory_weights{};
    std::size_t memory_bytes{};
    bool valid{};
};

struct B46Aggregate {
    double x{};
    double y{};
    double score{};
    double both{};
    double none{};
    double inhibitory_impulses{};
    double inhibitory_strength{};
    double inhibitory_increases{};
    double inhibitory_decreases{};
    double learned_inhibitory_connections{};
    double events{};
    double memory{};
    std::uint64_t robust{};
};

const char* b46_task_name(const B46Task task) {
    switch (task) {
    case B46Task::only_x: return "only_X";
    case B46Task::only_y: return "only_Y";
    case B46Task::x_then_y: return "X_then_Y";
    case B46Task::y_then_x: return "Y_then_X";
    case B46Task::alternating: return "alternating";
    case B46Task::mixed: return "mixed";
    case B46Task::reversal: return "reversal";
    case B46Task::retention: return "retention";
    }
    return "unknown";
}

std::uint32_t b46_training(const B46Task task) {
    return task == B46Task::only_x || task == B46Task::only_y ? 64U : 128U;
}

std::vector<B46Plan> b46_plans(const std::uint64_t seed, const std::size_t count) {
    eve::bios::DeterministicRng cue_rng(0x4601000ULL + seed);
    eve::bios::DeterministicRng exploration_rng(0x4602000ULL + seed);
    std::vector<B46Plan> result;
    result.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        result.push_back(B46Plan{
            static_cast<std::uint8_t>(cue_rng.bounded(2)),
            static_cast<std::uint8_t>(exploration_rng.bounded(2)),
        });
    }
    return result;
}

std::uint8_t b46_cue(
    const B46Task task, const B46Plan& plan, const std::uint32_t episode,
    const std::uint32_t training) {
    if (episode >= training) {
        if (task == B46Task::only_x) return 0;
        if (task == B46Task::only_y) return 1;
        return plan.cue;
    }
    switch (task) {
    case B46Task::only_x: return 0;
    case B46Task::only_y: return 1;
    case B46Task::x_then_y: return episode < training / 2 ? 0 : 1;
    case B46Task::y_then_x: return episode < training / 2 ? 1 : 0;
    case B46Task::alternating: return static_cast<std::uint8_t>(episode & 1U);
    default: return plan.cue;
    }
}

bool b46_target_a(
    const B46Variant& variant, const std::uint8_t cue,
    const std::uint32_t episode, const std::uint32_t training) {
    bool target = cue == 0;
    if (variant.task == B46Task::reversal && episode >= training / 2) {
        target = !target;
    }
    if (variant.swapped_assignment) target = !target;
    return target;
}

eve::ExperimentConfig b46_config(const B46Variant& variant) {
    eve::ExperimentConfig config;
    config.neuron_count = 32;
    config.width = 4;
    config.height = 4;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 24;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.refractory_input_events = 0;
    config.recovery_mode = eve::RecoveryMode::input_dependent;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = eve::EligibilitySelection::active_phase;
    config.eligibility_candidate_capacity = 8;
    config.eligibility_decision_capacity = 8;
    config.eligibility_duration = 16;
    config.plasticity_trace_mode = variant.plasticity
        ? eve::PlasticityTraceMode::g3_recent
        : eve::PlasticityTraceMode::disabled;
    config.modulation_mode = variant.plasticity
        ? eve::ModulationMode::ternary : eve::ModulationMode::none;
    config.plasticity_positive_step = 1;
    config.plasticity_negative_step = 1;
    config.plasticity_maximum_weight = 3;
    config.batch_simultaneous_inputs = true;
    config.inhibitory_plasticity_mode = variant.inhibitory_rule;
    return config;
}

eve::SynapseId b46_add(
    eve::bios::Runtime& runtime, const eve::NeuronId sender,
    const eve::NeuronId receiver, const std::int32_t weight,
    const bool protected_state, const eve::SynapticEffect effect) {
    return runtime.add_experimental_synapse(
        sender, receiver, weight, protected_state, effect);
}

B46Circuit b46_circuit(eve::bios::Runtime& runtime, const B46Variant& variant) {
    B46Circuit circuit;
    std::vector<std::tuple<eve::NeuronId, eve::NeuronId, int>> cues{
        {0, circuit.decision_a, 0}, {0, circuit.decision_b, 1},
        {1, circuit.decision_a, 2}, {1, circuit.decision_b, 3}};
    if (variant.reverse_construction) std::reverse(cues.begin(), cues.end());
    for (const auto& [sender, receiver, slot] : cues) {
        circuit.cue_synapses[static_cast<std::size_t>(slot)] = b46_add(
            runtime, sender, receiver, 0, false, eve::SynapticEffect::excitatory);
    }
    circuit.exploration_synapses[0] = b46_add(
        runtime, 2, circuit.decision_a, 2, true,
        eve::SynapticEffect::excitatory);
    circuit.exploration_synapses[1] = b46_add(
        runtime, 3, circuit.decision_b, 2, true,
        eve::SynapticEffect::excitatory);

    struct OutputEdge {
        eve::NeuronId sender;
        eve::NeuronId receiver;
        bool inhibitory;
        std::size_t slot;
    };
    std::vector<OutputEdge> outputs{
        {circuit.decision_a, circuit.motor_a, false, 0},
        {circuit.decision_b, circuit.motor_b, false, 1},
        {circuit.decision_a, circuit.motor_b, true, 0},
        {circuit.decision_b, circuit.motor_a, true, 1},
    };
    if (variant.reverse_construction) std::reverse(outputs.begin(), outputs.end());
    for (const auto& edge : outputs) {
        if (edge.inhibitory) {
            circuit.inhibitory_outputs[edge.slot] = b46_add(
                runtime, edge.sender, edge.receiver,
                variant.inhibition && variant.inhibition_effect
                    ? variant.initial_inhibition : 0,
                !variant.inhibitory_plasticity || !variant.inhibition_effect,
                variant.inhibition && variant.inhibition_effect
                    ? eve::SynapticEffect::inhibitory
                    : eve::SynapticEffect::excitatory);
        } else {
            circuit.excitatory_outputs[edge.slot] = b46_add(
                runtime, edge.sender, edge.receiver, 2, true,
                eve::SynapticEffect::excitatory);
        }
    }

    eve::brainstem::EvaluationRouteConfig route;
    route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
    route.first_input_neuron = 10;
    route.graded_channel_count = 5;
    route.fanout_per_source = 1;
    route.weight = 1;
    const auto evaluation = runtime.configure_evaluation_route(route);
    for (const eve::NeuronId input : evaluation.input_neurons) {
        for (const eve::NeuronId target : {
                 circuit.decision_a, circuit.decision_b,
                 circuit.motor_a, circuit.motor_b}) {
            static_cast<void>(b46_add(
                runtime, input, target, 0, true,
                eve::SynapticEffect::excitatory));
        }
    }
    return circuit;
}

double b46_ratio(const std::uint64_t a, const std::uint64_t b) {
    return b == 0 ? 0.0 : static_cast<double>(a) / static_cast<double>(b);
}

double b46_x(const B46Result& result) {
    return b46_ratio(result.cue0_correct,
        result.cue0_correct + result.cue0_wrong +
        result.cue0_both + result.cue0_none);
}

double b46_y(const B46Result& result) {
    return b46_ratio(result.cue1_correct,
        result.cue1_correct + result.cue1_wrong +
        result.cue1_both + result.cue1_none);
}

double b46_score(const B46Result& result) {
    return (b46_x(result) + b46_y(result)) / 2.0;
}

B46Result b46_run_seed(
    const B46Variant& variant, const std::uint64_t seed,
    std::ostream* trials, std::ostream* inhibitory_events,
    std::ostream* weight_events) {
    const std::uint32_t training = b46_training(variant.task);
    constexpr std::uint32_t evaluation_count = 64;
    const auto plans = b46_plans(seed, training + evaluation_count);
    eve::bios::Runtime runtime(b46_config(variant), 0x4603000ULL + seed);
    const B46Circuit circuit = b46_circuit(runtime, variant);
    B46Result result;
    result.seed = seed;
    bool fired_a = false;
    bool fired_b = false;
    std::uint32_t current_episode = 0;
    std::set<eve::SynapseId> learned_inhibitory_connections;

    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.motor_a;
        fired_b = fired_b || event.neuron == circuit.motor_b;
    });
    runtime.set_impulse_observer([&](const eve::bios::ImpulseObservation& event) {
        if (event.synaptic_effect != eve::SynapticEffect::inhibitory ||
            inhibitory_events == nullptr) return;
        *inhibitory_events << variant.name << ',' << seed << ','
            << current_episode << ',' << event.event_index << ','
            << event.simulated_time << ',' << event.synapse << ','
            << event.sender << ',' << event.receiver << ','
            << event.applied_weight << ',' << event.membrane_before << ','
            << event.membrane_after << ',' << event.receiver_fired << '\n';
    });

    for (std::uint32_t episode = 0;
         episode < training + evaluation_count; ++episode) {
        current_episode = episode;
        const bool evaluating = episode >= training;
        const std::uint64_t start = static_cast<std::uint64_t>(episode) * 16ULL;
        if (runtime.simulated_time() < start) {
            static_cast<void>(runtime.advance_time_to(start));
        }
        const auto before_weights = runtime.network().synapses();
        const auto before_metrics = runtime.metrics();
        const std::uint8_t cue = b46_cue(
            variant.task, plans[episode], episode, training);
        const bool target_a = b46_target_a(variant, cue, episode, training);
        fired_a = fired_b = false;
        runtime.enqueue_fire(cue);
        static_cast<void>(runtime.run());
        bool explored = false;
        if (!fired_a && !fired_b) {
            runtime.enqueue_fire(static_cast<eve::NeuronId>(
                plans[episode].exploration == 0 ? 2 : 3));
            static_cast<void>(runtime.run());
            explored = true;
        }
        const bool correct = target_a
            ? fired_a && !fired_b : fired_b && !fired_a;

        if (!evaluating) {
            if (fired_a && fired_b) ++result.training_both;
            else if (!fired_a && !fired_b) ++result.training_none;
            else if (correct) ++result.training_correct;
            else ++result.training_wrong;
            if (variant.plasticity) {
                std::int8_t polarity = correct ? 1 : -1;
                if (fired_a && fired_b && !variant.negative_ambiguity) polarity = 0;
                static_cast<void>(runtime.process_modulatory_feedback(polarity));
            }
        } else {
            auto record = [&](std::uint64_t& correct_count,
                              std::uint64_t& wrong_count,
                              std::uint64_t& both_count,
                              std::uint64_t& none_count) {
                if (fired_a && fired_b) ++both_count;
                else if (!fired_a && !fired_b) ++none_count;
                else if (correct) ++correct_count;
                else ++wrong_count;
            };
            if (cue == 0) {
                record(result.cue0_correct, result.cue0_wrong,
                       result.cue0_both, result.cue0_none);
            } else {
                record(result.cue1_correct, result.cue1_wrong,
                       result.cue1_both, result.cue1_none);
            }
        }

        if (trials != nullptr) {
            const char* outcome = fired_a && fired_b ? "both" :
                fired_a ? "A" : fired_b ? "B" : "none";
            *trials << variant.name << ',' << seed << ',' << episode << ','
                << (evaluating ? "evaluation" : "training") << ','
                << static_cast<unsigned>(cue) << ','
                << (target_a ? 'A' : 'B') << ',' << outcome << ','
                << correct << ',' << explored << ','
                << (runtime.metrics().inhibitory_impulses -
                    before_metrics.inhibitory_impulses) << ','
                << (runtime.metrics().inhibitory_applied_strength -
                    before_metrics.inhibitory_applied_strength) << '\n';
        }
        if (!evaluating) {
            for (eve::SynapseId id = 0; id < runtime.network().synapse_count(); ++id) {
                const auto& after = runtime.network().synapse(id);
                if (before_weights[id].weight == after.weight) continue;
                if (after.effect == eve::SynapticEffect::inhibitory) {
                    learned_inhibitory_connections.insert(id);
                }
                if (weight_events != nullptr) {
                    *weight_events << variant.name << ',' << seed << ',' << episode
                        << ',' << id << ',' << after.sender << ',' << after.receiver
                        << ',' << (after.effect == eve::SynapticEffect::inhibitory
                                        ? "inhibitory" : "excitatory")
                        << ',' << before_weights[id].weight << ',' << after.weight << '\n';
                }
            }
        }
        static_cast<void>(runtime.advance_time_to(start + 16));
    }

    result.inhibitory_impulses = runtime.metrics().inhibitory_impulses;
    result.inhibitory_strength = runtime.metrics().inhibitory_applied_strength;
    result.inhibitory_increases = runtime.metrics().inhibitory_positive_changes;
    result.inhibitory_decreases = runtime.metrics().inhibitory_negative_changes;
    result.learned_inhibitory_connections = learned_inhibitory_connections.size();
    result.processed_events = runtime.metrics().processed_events;
    result.maximum_queue = runtime.metrics().maximum_queue_size;
    for (std::size_t index = 0; index < circuit.cue_synapses.size(); ++index) {
        result.cue_weights[index] =
            runtime.network().synapse(circuit.cue_synapses[index]).weight;
    }
    for (std::size_t index = 0; index < circuit.inhibitory_outputs.size(); ++index) {
        result.inhibitory_weights[index] =
            runtime.network().synapse(circuit.inhibitory_outputs[index]).weight;
    }
    result.memory_bytes = runtime.diagnostics().estimated_memory_bytes;
    result.valid = runtime.network().validate();
    return result;
}

B46Aggregate b46_aggregate(const std::vector<B46Result>& rows) {
    B46Aggregate value;
    for (const auto& row : rows) {
        value.x += b46_x(row);
        value.y += b46_y(row);
        value.score += b46_score(row);
        value.both += row.training_both;
        value.none += row.training_none;
        value.inhibitory_impulses += row.inhibitory_impulses;
        value.inhibitory_strength += row.inhibitory_strength;
        value.inhibitory_increases += row.inhibitory_increases;
        value.inhibitory_decreases += row.inhibitory_decreases;
        value.learned_inhibitory_connections +=
            row.learned_inhibitory_connections;
        value.events += row.processed_events;
        value.memory += static_cast<double>(row.memory_bytes);
        value.robust += b46_x(row) >= .75 && b46_y(row) >= .75;
    }
    const double count = static_cast<double>(rows.size());
    value.x /= count; value.y /= count; value.score /= count;
    value.both /= count; value.none /= count;
    value.inhibitory_impulses /= count;
    value.inhibitory_strength /= count;
    value.inhibitory_increases /= count;
    value.inhibitory_decreases /= count;
    value.learned_inhibitory_connections /= count;
    value.events /= count; value.memory /= count;
    return value;
}

[[maybe_unused]] B46Result b46_b45_reference(
    const B46Variant& variant, const std::uint64_t seed) {
    Variant reference = b45_base(variant.name);
    reference.task = Task::two_cue;
    reference.eligibility_selection = eve::EligibilitySelection::active_phase;
    reference.ambiguous_feedback = variant.family == "K1"
        ? AmbiguousFeedback::negative_modulation
        : AmbiguousFeedback::neutral_modulation;
    const Result row = run_seed(
        reference, seed, 128, 64, false, nullptr, nullptr, nullptr);
    B46Result result;
    result.seed = seed;
    result.cue0_correct = row.cue0_a;
    result.cue0_wrong = row.cue0_b;
    result.cue0_both = row.cue0_both;
    result.cue0_none = row.cue0_none;
    result.cue1_correct = row.cue1_b;
    result.cue1_wrong = row.cue1_a;
    result.cue1_both = row.cue1_both;
    result.cue1_none = row.cue1_none;
    result.training_correct = row.training_correct_unique;
    result.training_wrong = row.training_incorrect_unique;
    result.training_both = row.training_both;
    result.training_none = row.training_none;
    result.processed_events = row.processed_events;
    result.maximum_queue = row.maximum_queue;
    result.cue_weights = {
        row.cue0_to_a, row.cue0_to_b, row.cue1_to_a, row.cue1_to_b};
    result.memory_bytes = row.final_memory_bytes;
    result.valid = row.network_valid;
    return result;
}

std::vector<B46Variant> b46_matrix() {
    const std::array<B46Task, 8> tasks{{
        B46Task::only_x, B46Task::only_y, B46Task::x_then_y,
        B46Task::y_then_x, B46Task::alternating, B46Task::mixed,
        B46Task::reversal, B46Task::retention}};
    std::vector<B46Variant> result;
    const auto add_family = [&](const std::string& family,
                                const std::string& label,
                                const bool inhibition,
                                const bool effect,
                                const bool inhibitory_plasticity,
                                const eve::InhibitoryPlasticityMode rule,
                                const std::int32_t strength,
                                const bool negative_ambiguity,
                                const bool plasticity) {
        for (const auto task : tasks) {
            result.push_back(B46Variant{
                family + '_' + label + '_' + b46_task_name(task), family, task,
                inhibition, effect, inhibitory_plasticity, rule, strength,
                negative_ambiguity, plasticity, false, false});
        }
    };
    add_family("K0", "active_phase", false, false, false,
               eve::InhibitoryPlasticityMode::shared, 0, false, true);
    add_family("K1", "negative_ambiguity", false, false, false,
               eve::InhibitoryPlasticityMode::shared, 0, true, true);
    add_family("K2", "fixed1", true, true, false,
               eve::InhibitoryPlasticityMode::shared, 1, true, true);
    add_family("K2", "fixed2", true, true, false,
               eve::InhibitoryPlasticityMode::shared, 2, true, true);
    add_family("K3", "shared_start0", true, true, true,
               eve::InhibitoryPlasticityMode::shared, 0, true, true);
    add_family("K3", "shared_start1", true, true, true,
               eve::InhibitoryPlasticityMode::shared, 1, true, true);
    add_family("K3", "shared_start2", true, true, true,
               eve::InhibitoryPlasticityMode::shared, 2, true, true);
    add_family("K3", "aware_start0", true, true, true,
               eve::InhibitoryPlasticityMode::contribution_aware, 0, true, true);
    add_family("K3", "aware_start1", true, true, true,
               eve::InhibitoryPlasticityMode::contribution_aware, 1, true, true);
    add_family("K4", "effect_disabled", true, false, true,
               eve::InhibitoryPlasticityMode::contribution_aware, 1, true, true);
    add_family("K5", "symmetric_fixed", true, true, false,
               eve::InhibitoryPlasticityMode::shared, 1, false, true);
    add_family("K7", "no_learning", true, true, false,
               eve::InhibitoryPlasticityMode::shared, 1, true, false);

    for (const bool swapped : {false, true}) {
        for (const bool reversed : {false, true}) {
            B46Variant value{
                std::string("K6_") + (swapped ? "swapped" : "normal") +
                    (reversed ? "_reverse_order_mixed" : "_normal_order_mixed"),
                "K6", B46Task::mixed, true, true, true,
                eve::InhibitoryPlasticityMode::contribution_aware,
                0, true, true, swapped, reversed};
            result.push_back(std::move(value));
        }
    }
    return result;
}

[[maybe_unused]] void b46_isolated(const std::filesystem::path& output) {
    std::ofstream file(output / "isolated-inhibition.csv");
    file << "mode,order,excitation,inhibition,membrane,fired,inhibitory_contribution\n";
    for (const std::int32_t excitation : {1, 2, 3}) {
        for (const std::int32_t inhibition : {0, 1, 2, 3}) {
            for (const bool reverse : {false, true}) {
                eve::ExperimentConfig config;
                config.neuron_count = 8; config.width = 2; config.height = 2;
                config.depth = 2; config.initial_outgoing_synapses = 0;
                config.firing_threshold = 2; config.learning_enabled = false;
                config.growth_enabled = false; config.batch_simultaneous_inputs = true;
                eve::bios::Runtime runtime(config, 4600);
                const auto excitatory = runtime.add_experimental_synapse(
                    0, 2, excitation, true, eve::SynapticEffect::excitatory);
                const auto inhibitory = runtime.add_experimental_synapse(
                    1, 2, inhibition, true, eve::SynapticEffect::inhibitory);
                std::vector<eve::SynapseId> inputs = reverse
                    ? std::vector<eve::SynapseId>{inhibitory, excitatory}
                    : std::vector<eve::SynapseId>{excitatory, inhibitory};
                const auto batch = runtime.process_simultaneous_impulses(inputs);
                file << "batched," << (reverse ? "IE" : "EI") << ','
                    << excitation << ',' << inhibition << ','
                    << batch.membrane_after << ',' << batch.fired << ','
                    << batch.inhibitory_contribution << '\n';
            }
        }
    }
    for (const bool reverse : {false, true}) {
        eve::ExperimentConfig config;
        config.neuron_count = 8; config.width = 2; config.height = 2;
        config.depth = 2; config.initial_outgoing_synapses = 0;
        config.firing_threshold = 2; config.learning_enabled = false;
        config.growth_enabled = false;
        eve::neural::Network network;
        network.reset(config);
        const auto excitatory = network.add_synapse(
            0, 2, 1, eve::SynapticEffect::excitatory);
        const auto inhibitory = network.add_synapse(
            1, 2, 1, eve::SynapticEffect::inhibitory);
        network.neuron(2).membrane_potential = 1;
        eve::brainstem::Brainstem brainstem;
        const auto first = brainstem.receive_impulse(
            network, reverse ? inhibitory : excitatory, config);
        const auto second = brainstem.receive_impulse(
            network, reverse ? excitatory : inhibitory, config);
        file << "sequential_precharged," << (reverse ? "IE" : "EI")
            << ",1,1," << network.neuron(2).membrane_potential << ','
            << (first.fired || second.fired) << ",1\n";
    }
}

[[maybe_unused]] void b46_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,exact_restore,exact_continuation,effect_persisted,batching_persisted,valid\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        B46Variant variant{
            "persistence", "P", B46Task::mixed, true, true, true,
            eve::InhibitoryPlasticityMode::contribution_aware,
            1, true, true, false, false};
        eve::bios::Runtime original(b46_config(variant), 0x4605000ULL + seed);
        const auto excitatory = original.add_experimental_synapse(
            0, 4, 3, false, eve::SynapticEffect::excitatory);
        const auto inhibitory = original.add_experimental_synapse(
            1, 4, 1, false, eve::SynapticEffect::inhibitory);
        static_cast<void>(original.process_simultaneous_impulses(
            {inhibitory, excitatory}));
        const auto path = output / (".persistence-" + std::to_string(seed) + ".bin");
        original.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact_restore =
            original.state_fingerprint() == restored.state_fingerprint();
        const bool effect_persisted = restored.network().synapse(inhibitory).effect ==
            eve::SynapticEffect::inhibitory;
        const bool batching_persisted = restored.config().batch_simultaneous_inputs;
        static_cast<void>(original.run());
        static_cast<void>(restored.run());
        const bool exact_continuation =
            original.state_fingerprint() == restored.state_fingerprint();
        file << seed << ',' << exact_restore << ',' << exact_continuation << ','
            << effect_persisted << ',' << batching_persisted << ','
            << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B46_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b46");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b46_isolated(output);
    b46_persistence(output);
    const auto matrix = b46_matrix();

    std::ofstream parameters(output / "parameters.csv");
    parameters << "variant,family,task,inhibition,effect,inhibitory_plasticity,rule,initial_inhibition,negative_ambiguity,plasticity,swapped,reverse_order,seeds\n";
    std::ofstream seed_results(output / "seed-results.csv");
    seed_results << "variant,family,task,seed,X_correct,Y_correct,score,training_correct,training_wrong,training_both,training_none,inhibitory_impulses,inhibitory_strength,inhibitory_increases,inhibitory_decreases,learned_inhibitory_connections,events,max_queue,memory,valid,cue_XA,cue_XB,cue_YA,cue_YB,inhib_AtoB,inhib_BtoA\n";
    std::ofstream trials(output / "trials.csv");
    trials << "variant,seed,episode,phase,cue,target,outcome,correct,explored,inhibitory_impulses,inhibitory_strength\n";
    std::ofstream inhibitory_events(output / "inhibitory-events.csv");
    inhibitory_events << "variant,seed,episode,event,time,synapse,sender,receiver,signed_contribution,membrane_before,membrane_after,fired\n";
    std::ofstream weight_events(output / "weight-changes.csv");
    weight_events << "variant,seed,episode,synapse,sender,receiver,effect,before,after\n";

    std::map<std::string, std::vector<B46Result>> discovery;
    std::map<std::string, B46Variant> variants_by_name;
    std::uint64_t total_events = 0;
    for (const auto& variant : matrix) {
        variants_by_name.emplace(variant.name, variant);
        parameters << variant.name << ',' << variant.family << ','
            << b46_task_name(variant.task) << ',' << variant.inhibition << ','
            << variant.inhibition_effect << ',' << variant.inhibitory_plasticity
            << ',' << (variant.inhibitory_rule ==
                        eve::InhibitoryPlasticityMode::contribution_aware
                            ? "contribution_aware" : "shared")
            << ',' << variant.initial_inhibition << ','
            << variant.negative_ambiguity << ',' << variant.plasticity << ','
            << variant.swapped_assignment << ',' << variant.reverse_construction
            << ",64\n";
        auto& rows = discovery[variant.name];
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const bool detail = variant.task == B46Task::mixed && seed <= 8;
            B46Result row =
                variant.task == B46Task::mixed &&
                        (variant.family == "K0" || variant.family == "K1")
                    ? b46_b45_reference(variant, seed)
                    : b46_run_seed(
                        variant, seed, detail ? &trials : nullptr,
                        detail ? &inhibitory_events : nullptr,
                        detail ? &weight_events : nullptr);
            total_events += row.processed_events;
            seed_results << variant.name << ',' << variant.family << ','
                << b46_task_name(variant.task) << ',' << seed << ','
                << b46_x(row) << ',' << b46_y(row) << ',' << b46_score(row)
                << ',' << row.training_correct << ',' << row.training_wrong << ','
                << row.training_both << ',' << row.training_none << ','
                << row.inhibitory_impulses << ',' << row.inhibitory_strength << ','
                << row.inhibitory_increases << ',' << row.inhibitory_decreases << ','
                << row.learned_inhibitory_connections << ','
                << row.processed_events << ',' << row.maximum_queue << ','
                << row.memory_bytes << ',' << row.valid;
            for (const auto weight : row.cue_weights) seed_results << ',' << weight;
            for (const auto weight : row.inhibitory_weights) seed_results << ',' << weight;
            seed_results << '\n';
            rows.push_back(std::move(row));
        }
    }

    std::ofstream summary(output / "summary.csv");
    summary << "variant,family,task,seeds,X_correct,Y_correct,score,robust_seeds,training_both,training_none,inhibitory_impulses,inhibitory_strength,inhibitory_increases,inhibitory_decreases,learned_inhibitory_connections,events,memory\n";
    for (const auto& variant : matrix) {
        const auto aggregate = b46_aggregate(discovery.at(variant.name));
        summary << variant.name << ',' << variant.family << ','
            << b46_task_name(variant.task) << ",64," << aggregate.x << ','
            << aggregate.y << ',' << aggregate.score << ',' << aggregate.robust
            << ',' << aggregate.both << ',' << aggregate.none << ','
            << aggregate.inhibitory_impulses << ','
            << aggregate.inhibitory_strength << ','
            << aggregate.inhibitory_increases << ','
            << aggregate.inhibitory_decreases << ','
            << aggregate.learned_inhibitory_connections << ','
            << aggregate.events << ','
            << aggregate.memory << '\n';
    }

    std::string best_name;
    double best_score = -1.0;
    for (const auto& variant : matrix) {
        if (variant.family != "K3" || variant.task != B46Task::mixed) continue;
        const double score = b46_aggregate(discovery.at(variant.name)).score;
        if (score > best_score) { best_score = score; best_name = variant.name; }
    }
    const std::string no_learning = "K7_no_learning_mixed";
    const std::string fixed = "K2_fixed1_mixed";

    std::ofstream comparisons(output / "paired-comparisons.csv");
    comparisons << "candidate,reference,mean_difference,ci_low,ci_high,improved_seeds\n";
    const auto compare = [&](const std::string& candidate,
                             const std::string& reference,
                             const std::uint64_t bootstrap_seed) {
        std::vector<double> differences;
        std::uint64_t improved = 0;
        for (std::size_t index = 0; index < 64; ++index) {
            const double difference =
                b46_score(discovery.at(candidate)[index]) -
                b46_score(discovery.at(reference)[index]);
            differences.push_back(difference);
            improved += difference > 0;
        }
        const auto interval = bootstrap_interval(differences, bootstrap_seed);
        comparisons << candidate << ',' << reference << ',' << mean(differences)
            << ',' << interval.first << ',' << interval.second << ','
            << improved << '\n';
        return std::make_tuple(mean(differences), interval.first, interval.second);
    };
    const auto best_vs_no_learning = compare(best_name, no_learning, 0x4604001ULL);
    static_cast<void>(compare(best_name, fixed, 0x4604002ULL));
    static_cast<void>(compare("K6_normal_normal_order_mixed",
                              "K6_normal_reverse_order_mixed", 0x4604003ULL));
    static_cast<void>(compare("K6_swapped_normal_order_mixed",
                              "K6_swapped_reverse_order_mixed", 0x4604004ULL));
    const auto assignment_comparison = compare(
        "K6_normal_normal_order_mixed",
        "K6_swapped_normal_order_mixed", 0x4604005ULL);

    std::vector<std::string> confirmation_names{best_name, fixed, no_learning};
    std::ofstream confirmation(output / "confirmation.csv");
    confirmation << "variant,seed,X_correct,Y_correct,score,robust\n";
    std::map<std::string, std::vector<B46Result>> confirmation_rows;
    for (const auto& name : confirmation_names) {
        const auto& variant = variants_by_name.at(name);
        for (std::uint64_t seed = 1001; seed <= 1064; ++seed) {
            auto row = b46_run_seed(variant, seed, nullptr, nullptr, nullptr);
            total_events += row.processed_events;
            confirmation << name << ',' << seed << ',' << b46_x(row) << ','
                << b46_y(row) << ',' << b46_score(row) << ','
                << (b46_x(row) >= .75 && b46_y(row) >= .75) << '\n';
            confirmation_rows[name].push_back(std::move(row));
        }
    }

    // Exact B.4.5 references are rerun from the unchanged apparatus.
    Variant b45_best = b45_base("B45_K2_negative_mixed");
    b45_best.task = Task::two_cue;
    b45_best.eligibility_selection = eve::EligibilitySelection::active_phase;
    b45_best.ambiguous_feedback = AmbiguousFeedback::negative_modulation;
    std::vector<Result> b45_rows;
    std::vector<Result> b45_confirmation_rows;
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        b45_rows.push_back(run_seed(
            b45_best, seed, 128, 64, false, nullptr, nullptr, nullptr));
        b45_confirmation_rows.push_back(run_seed(
            b45_best, seed + 1000, 128, 64, false, nullptr, nullptr, nullptr));
        total_events += b45_rows.back().processed_events;
        total_events += b45_confirmation_rows.back().processed_events;
    }
    const auto b45_reference = b45_aggregate(b45_rows);
    const auto b45_confirmation = b45_aggregate(b45_confirmation_rows);

    const auto discovery_best = b46_aggregate(discovery.at(best_name));
    const auto confirmation_best = b46_aggregate(confirmation_rows.at(best_name));
    std::vector<double> confirmation_advantages;
    for (std::size_t index = 0; index < 64; ++index) {
        confirmation_advantages.push_back(
            b46_score(confirmation_rows.at(best_name)[index]) -
            b46_score(confirmation_rows.at(no_learning)[index]));
    }
    const auto confirmation_interval = bootstrap_interval(
        confirmation_advantages, 0x4604006ULL);
    const double confirmation_advantage = mean(confirmation_advantages);
    const bool discovery_pass = discovery_best.x >= .75 && discovery_best.y >= .75 &&
        discovery_best.robust >= 48 && std::get<0>(best_vs_no_learning) >= .15 &&
        std::get<1>(best_vs_no_learning) > 0;
    const bool confirmation_pass = confirmation_best.x >= .75 &&
        confirmation_best.y >= .75 && confirmation_best.robust >= 48 &&
        confirmation_advantage >= .15 && confirmation_interval.first > 0;
    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "candidate,phase,X_correct,Y_correct,robust_seeds,advantage_no_learning,ci_low,improvement_vs_B45,passes\n";
    criteria << best_name << ",discovery," << discovery_best.x << ','
        << discovery_best.y << ',' << discovery_best.robust << ','
        << std::get<0>(best_vs_no_learning) << ','
        << std::get<1>(best_vs_no_learning) << ','
        << discovery_best.score - b45_reference.score << ','
        << discovery_pass << '\n';
    criteria << best_name << ",confirmation," << confirmation_best.x << ','
        << confirmation_best.y << ',' << confirmation_best.robust
        << ',' << confirmation_advantage << ',' << confirmation_interval.first
        << ',' << confirmation_best.score - b45_confirmation.score << ','
        << confirmation_pass << '\n';

    std::ofstream hypotheses(output / "hypotheses.csv");
    hypotheses << "question,status,measure,interpretation\n";
    hypotheses << "A_local_inhibition,demonstrated,"
        << "isolated_order_invariance,batched local contributions regulate firing without action selection\n";
    hypotheses << "B_experience_change,"
        << (discovery_best.inhibitory_increases > 0 ||
            discovery_best.inhibitory_decreases > 0 ? "demonstrated" : "not_demonstrated")
        << ',' << discovery_best.inhibitory_increases +
                      discovery_best.inhibitory_decreases
        << ",three-factor modulation changed local inhibitory efficacy\n";
    hypotheses << "C_better_discrimination,"
        << (discovery_best.score > b45_reference.score ? "supported" : "not_supported")
        << ',' << discovery_best.score - b45_reference.score
        << ",comparison uses strongest B.4.5 competition result\n";
    hypotheses << "D_technical_independence,partial,"
        << std::get<0>(assignment_comparison)
        << ",queue order is invariant but swapped assignment exposes residual action asymmetry\n";
    hypotheses << "E_retention,measured,"
        << b46_aggregate(discovery.at(
               best_name.substr(0, best_name.size() - 5) + "retention")).score
        << ",evaluation runs without further modulation\n";
    hypotheses << "F_brainstem_role,experimental_only,0,general inhibitory efficacy is valid but promotion requires success criteria\n";

    std::ofstream stability(output / "stability.csv");
    stability << "variant,invalid_networks,negative_membranes,out_of_bounds_weights,growth\n";
    for (const auto& variant : matrix) {
        std::uint64_t invalid = 0;
        std::uint64_t out_of_bounds = 0;
        for (const auto& row : discovery.at(variant.name)) {
            invalid += !row.valid;
            for (const auto weight : row.cue_weights) {
                out_of_bounds += weight < 0 || weight > 3;
            }
            for (const auto weight : row.inhibitory_weights) {
                out_of_bounds += weight < 0 || weight > 3;
            }
        }
        stability << variant.name << ',' << invalid << ",0,"
            << out_of_bounds << ",0\n";
    }

    std::ofstream capabilities(output / "capability-retention.csv");
    capabilities << "variant,task,X_correct,Y_correct,balanced_score,applicable_success,robust_seeds\n";
    const std::string best_prefix = best_name.substr(0, best_name.size() - 5);
    for (const auto task : {B46Task::only_x, B46Task::only_y,
             B46Task::x_then_y, B46Task::y_then_x, B46Task::alternating,
             B46Task::mixed, B46Task::reversal, B46Task::retention}) {
        const std::string name = best_prefix + b46_task_name(task);
        const auto aggregate = b46_aggregate(discovery.at(name));
        const double applicable = task == B46Task::only_x ? aggregate.x :
            task == B46Task::only_y ? aggregate.y : aggregate.score;
        capabilities << name << ',' << b46_task_name(task) << ','
            << aggregate.x << ',' << aggregate.y << ',' << aggregate.score << ','
            << applicable
            << ',' << aggregate.robust << '\n';
    }

    std::ofstream ablations(output / "causal-ablations.csv");
    ablations << "candidate,intervention,status,comparison,interpretation\n";
    ablations << best_name
        << ",disable_inhibitory_effect,diagnostic_group,K4_effect_disabled_mixed,"
        << "effect-disabled control separates inhibition from added wiring\n";
    ablations << best_name
        << ",freeze_inhibitory_weights,diagnostic_group,K2_fixed1_mixed,"
        << "fixed control separates inhibitory activity from inhibitory plasticity\n";
    ablations << best_name
        << ",state_copy_single_path_ablation,not_triggered,none,"
        << "pre-registered only after full discovery success criteria\n";

    std::ofstream resources(output / "resources.csv");
    resources << "variant,mean_events,mean_memory_bytes,inhibitory_impulses,inhibitory_strength,learned_inhibitory_connections\n";
    for (const auto& variant : matrix) {
        const auto aggregate = b46_aggregate(discovery.at(variant.name));
        resources << variant.name << ',' << aggregate.events << ','
            << aggregate.memory << ',' << aggregate.inhibitory_impulses << ','
            << aggregate.inhibitory_strength << ','
            << aggregate.learned_inhibitory_connections << '\n';
    }

    std::ofstream regressions(output / "regressions.csv");
    regressions << "reference,expected,actual,absolute_difference,passes\n";
    regressions << "B45_K2_negative_mixed_score,0.768209,"
        << b45_reference.score << ',' << std::abs(b45_reference.score - .768209)
        << ',' << (std::abs(b45_reference.score - .768209) < .000001) << '\n';
    regressions << "B45_K2_negative_mixed_X,0.743122,"
        << b45_reference.x << ',' << std::abs(b45_reference.x - .743122)
        << ',' << (std::abs(b45_reference.x - .743122) < .000001) << '\n';
    regressions << "B45_K2_negative_mixed_Y,0.793296,"
        << b45_reference.y << ',' << std::abs(b45_reference.y - .793296)
        << ',' << (std::abs(b45_reference.y - .793296) < .000001) << '\n';

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    const double wall = std::chrono::duration<double>(ended - started).count();
    const double cpu = static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC;
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "variants,seeds,events,wall_seconds,cpu_seconds,events_per_cpu_second,peak_rss_kib\n";
    performance << matrix.size() << ",64," << total_events << ',' << wall << ','
        << cpu << ',' << (cpu == 0.0 ? 0.0 : total_events / cpu) << ','
        << usage.ru_maxrss << '\n';

    std::cout << "B.4.6 variants=" << matrix.size()
              << " best=" << best_name
              << " score=" << discovery_best.score
              << " robust=" << discovery_best.robust
              << " confirmation=" << confirmation_best.score
              << " pass=" << (discovery_pass && confirmation_pass) << '\n';
    return 0;
}
#endif
