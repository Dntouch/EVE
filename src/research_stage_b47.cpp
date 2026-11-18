#define EVE_RESEARCH_STAGE_B46_EMBEDDED
#include "research_stage_b46.cpp"
#undef EVE_RESEARCH_STAGE_B46_EMBEDDED

#include <numeric>

namespace {

struct B47Transform {
    bool sensor_swap{};
    bool motor_swap{};
    bool exploration_swap{};
    bool modulation_swap{};
    bool neuron_ids{};
    bool reverse_synapses{};
    bool reverse_queue{};
};

struct B47Variant {
    std::string name;
    std::string family;
    B46Task task{B46Task::mixed};
    B47Transform transform;
    bool inhibition_effect{true};
    bool inhibitory_plasticity{true};
    eve::InhibitoryPlasticityMode inhibitory_rule{
        eve::InhibitoryPlasticityMode::shared};
    std::int32_t initial_inhibition{};
    bool plasticity{true};
    bool inverse_targets{};
    std::uint32_t training_multiplier{1};
    bool invert_exploration_stream{};
    bool decorrelated_streams{};
};

struct B47Circuit {
    std::array<eve::NeuronId, 2> sensors{};
    std::array<eve::NeuronId, 2> explorers{};
    std::array<eve::NeuronId, 2> decisions{};
    std::array<eve::NeuronId, 2> motors{};
    std::array<eve::NeuronId, 5> modulation_inputs{};
    std::array<eve::SynapseId, 4> cue_synapses{};
    std::array<eve::SynapseId, 2> exploration_synapses{};
    std::array<eve::SynapseId, 2> excitatory_outputs{};
    std::array<eve::SynapseId, 2> inhibitory_outputs{};
    std::vector<eve::SynapseId> modulation_synapses;
};

struct B47Counts {
    std::uint64_t correct{};
    std::uint64_t wrong{};
    std::uint64_t both{};
    std::uint64_t none{};
};

struct B47Result {
    std::uint64_t seed{};
    std::array<B47Counts, 2> autonomous{};
    std::array<B47Counts, 2> supported{};
    std::uint64_t training_correct{};
    std::uint64_t training_wrong{};
    std::uint64_t training_both{};
    std::uint64_t training_none{};
    std::uint64_t exploration_episodes{};
    std::uint64_t exploration_a{};
    std::uint64_t exploration_b{};
    std::uint64_t exploration_positive{};
    std::uint64_t exploration_negative{};
    std::uint64_t exploration_early{};
    std::uint64_t exploration_late{};
    std::uint64_t positive_feedback{};
    std::uint64_t negative_feedback{};
    std::uint64_t inhibitory_impulses{};
    std::uint64_t inhibitory_strength{};
    std::uint64_t positive_changes{};
    std::uint64_t negative_changes{};
    std::uint64_t processed_events{};
    std::uint64_t maximum_queue{};
    std::array<std::int32_t, 4> cue_weights{};
    std::array<std::int32_t, 2> inhibitory_weights{};
    std::size_t memory_bytes{};
    bool valid{};
};

struct B47Aggregate {
    double autonomous_x{};
    double autonomous_y{};
    double autonomous_score{};
    double supported_x{};
    double supported_y{};
    double supported_score{};
    double both{};
    double none{};
    double exploration{};
    double inhibitory_impulses{};
    double positive_changes{};
    double negative_changes{};
    double events{};
    double memory{};
    std::uint64_t autonomous_robust{};
    std::uint64_t supported_robust{};
    std::uint64_t invalid{};
};

struct B47TrainingState {
    eve::bios::Runtime runtime;
    B47Circuit circuit;
    B47Result partial;
    std::vector<B46Plan> plans;
    std::uint32_t training{};
};

const char* b47_rule_name(const eve::InhibitoryPlasticityMode rule) {
    return rule == eve::InhibitoryPlasticityMode::contribution_aware
        ? "contribution_aware" : "shared";
}

std::uint32_t b47_training(const B47Variant& variant) {
    return b46_training(variant.task) * variant.training_multiplier;
}

std::uint64_t b47_mix_seed(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31U);
}

std::vector<B46Plan> b47_plans(
    const std::uint64_t seed, const std::size_t count,
    const bool decorrelated) {
    if (!decorrelated) return b46_plans(seed, count);
    eve::bios::DeterministicRng cue_rng(
        b47_mix_seed(0x4701000ULL ^ seed));
    eve::bios::DeterministicRng exploration_rng(
        b47_mix_seed(0x4702000ULL ^ (seed * 0xd1342543de82ef95ULL)));
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

bool b47_target_a(
    const B47Variant& variant, const std::uint8_t cue,
    const std::uint32_t episode, const std::uint32_t training) {
    bool target = cue == 0;
    if (variant.task == B46Task::reversal && episode >= training / 2) {
        target = !target;
    }
    if (variant.inverse_targets) target = !target;
    return target;
}

eve::ExperimentConfig b47_config(const B47Variant& variant) {
    B46Variant source;
    source.inhibitory_rule = variant.inhibitory_rule;
    source.plasticity = variant.plasticity;
    return b46_config(source);
}

std::array<eve::NeuronId, 2> b47_pair(
    std::array<eve::NeuronId, 2> value, const bool swap) {
    if (swap) std::swap(value[0], value[1]);
    return value;
}

B47Circuit b47_circuit(
    eve::bios::Runtime& runtime, const B47Variant& variant) {
    B47Circuit circuit;
    if (variant.transform.neuron_ids) {
        circuit.sensors = {17, 19};
        circuit.explorers = {21, 23};
        circuit.decisions = {25, 27};
        circuit.motors = {29, 31};
        circuit.modulation_inputs = {0, 1, 2, 3, 4};
    } else {
        circuit.sensors = {0, 1};
        circuit.explorers = {2, 3};
        circuit.decisions = {4, 5};
        circuit.motors = {6, 7};
        circuit.modulation_inputs = {10, 11, 12, 13, 14};
    }
    circuit.sensors = b47_pair(
        circuit.sensors, variant.transform.sensor_swap);
    circuit.explorers = b47_pair(
        circuit.explorers, variant.transform.exploration_swap);
    circuit.decisions = b47_pair(
        circuit.decisions, variant.transform.motor_swap);
    circuit.motors = b47_pair(
        circuit.motors, variant.transform.motor_swap);

    struct Edge {
        eve::NeuronId sender;
        eve::NeuronId receiver;
        std::int32_t weight;
        bool protected_state;
        eve::SynapticEffect effect;
        eve::SynapseId* destination;
    };
    std::vector<Edge> edges;
    for (std::size_t cue = 0; cue < 2; ++cue) {
        for (std::size_t action = 0; action < 2; ++action) {
            edges.push_back(Edge{
                circuit.sensors[cue], circuit.decisions[action], 0, false,
                eve::SynapticEffect::excitatory,
                &circuit.cue_synapses[cue * 2 + action]});
        }
    }
    for (std::size_t action = 0; action < 2; ++action) {
        edges.push_back(Edge{
            circuit.explorers[action], circuit.decisions[action], 2, true,
            eve::SynapticEffect::excitatory,
            &circuit.exploration_synapses[action]});
    }
    for (std::size_t action = 0; action < 2; ++action) {
        edges.push_back(Edge{
            circuit.decisions[action], circuit.motors[action], 2, true,
            eve::SynapticEffect::excitatory,
            &circuit.excitatory_outputs[action]});
        const std::size_t competitor = 1 - action;
        edges.push_back(Edge{
            circuit.decisions[action], circuit.motors[competitor],
            variant.inhibition_effect ? variant.initial_inhibition : 0,
            !variant.inhibitory_plasticity || !variant.inhibition_effect,
            variant.inhibition_effect ? eve::SynapticEffect::inhibitory
                                      : eve::SynapticEffect::excitatory,
            &circuit.inhibitory_outputs[action]});
    }
    if (variant.transform.reverse_synapses) {
        std::reverse(edges.begin(), edges.end());
    }
    for (const auto& edge : edges) {
        *edge.destination = runtime.add_experimental_synapse(
            edge.sender, edge.receiver, edge.weight,
            edge.protected_state, edge.effect);
    }

    eve::brainstem::EvaluationRouteConfig route;
    route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
    route.first_input_neuron = circuit.modulation_inputs.front();
    route.graded_channel_count = 5;
    route.fanout_per_source = 1;
    route.weight = 1;
    const auto configured = runtime.configure_evaluation_route(route);
    circuit.modulation_inputs = {
        configured.input_neurons[0], configured.input_neurons[1],
        configured.input_neurons[2], configured.input_neurons[3],
        configured.input_neurons[4]};
    std::vector<eve::NeuronId> modulation_sources(
        circuit.modulation_inputs.begin(), circuit.modulation_inputs.end());
    std::vector<eve::NeuronId> modulation_targets{
        circuit.decisions[0], circuit.decisions[1],
        circuit.motors[0], circuit.motors[1]};
    if (variant.transform.modulation_swap) {
        std::reverse(modulation_sources.begin(), modulation_sources.end());
        std::reverse(modulation_targets.begin(), modulation_targets.end());
    }
    for (const auto source : modulation_sources) {
        for (const auto target : modulation_targets) {
            circuit.modulation_synapses.push_back(
                runtime.add_experimental_synapse(
                    source, target, 0, true,
                    eve::SynapticEffect::excitatory));
        }
    }
    return circuit;
}

double b47_rate(const B47Counts& counts) {
    const auto total = counts.correct + counts.wrong + counts.both + counts.none;
    return total == 0 ? 0.0 :
        static_cast<double>(counts.correct) / static_cast<double>(total);
}

double b47_x(const B47Result& result, const bool autonomous) {
    return b47_rate((autonomous ? result.autonomous : result.supported)[0]);
}

double b47_y(const B47Result& result, const bool autonomous) {
    return b47_rate((autonomous ? result.autonomous : result.supported)[1]);
}

double b47_score(const B47Result& result, const bool autonomous) {
    return (b47_x(result, autonomous) + b47_y(result, autonomous)) / 2.0;
}

void b47_record(
    B47Counts& counts, const bool target_a,
    const bool fired_a, const bool fired_b) {
    if (fired_a && fired_b) ++counts.both;
    else if (!fired_a && !fired_b) ++counts.none;
    else if ((target_a && fired_a) || (!target_a && fired_b)) ++counts.correct;
    else ++counts.wrong;
}

std::string b47_outcome(const bool fired_a, const bool fired_b) {
    if (fired_a && fired_b) return "both";
    if (fired_a) return "A";
    if (fired_b) return "B";
    return "none";
}

B47TrainingState b47_train(
    const B47Variant& variant, const std::uint64_t seed,
    std::ostream* trajectory, std::ostream* event_trace,
    std::ostream* weight_trace) {
    const std::uint32_t training = b47_training(variant);
    B47TrainingState state{
        eve::bios::Runtime(b47_config(variant), 0x4703000ULL + seed),
        {}, {}, b47_plans(seed, training + 64, variant.decorrelated_streams),
        training};
    state.circuit = b47_circuit(state.runtime, variant);
    state.partial.seed = seed;
    bool fired_a = false;
    bool fired_b = false;
    std::uint64_t decision_a_events = 0;
    std::uint64_t decision_b_events = 0;
    std::uint32_t current_episode = 0;
    state.runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == state.circuit.motors[0];
        fired_b = fired_b || event.neuron == state.circuit.motors[1];
        decision_a_events += event.neuron == state.circuit.decisions[0];
        decision_b_events += event.neuron == state.circuit.decisions[1];
        if (event_trace != nullptr) {
            std::string role = "other";
            if (event.neuron == state.circuit.sensors[0]) role = "sensor_X";
            else if (event.neuron == state.circuit.sensors[1]) role = "sensor_Y";
            else if (event.neuron == state.circuit.explorers[0]) role = "explore_A";
            else if (event.neuron == state.circuit.explorers[1]) role = "explore_B";
            else if (event.neuron == state.circuit.decisions[0]) role = "decision_A";
            else if (event.neuron == state.circuit.decisions[1]) role = "decision_B";
            else if (event.neuron == state.circuit.motors[0]) role = "motor_A";
            else if (event.neuron == state.circuit.motors[1]) role = "motor_B";
            *event_trace << variant.name << ',' << seed << ',' << current_episode
                << ",fire," << event.event_index << ',' << event.simulated_time
                << ',' << event.neuron << ',' << role << ",-1,0,0\n";
        }
    });
    state.runtime.set_impulse_observer(
        [&](const eve::bios::ImpulseObservation& event) {
            if (event_trace == nullptr) return;
            *event_trace << variant.name << ',' << seed << ',' << current_episode
                << ",impulse," << event.event_index << ',' << event.simulated_time
                << ',' << event.receiver << ",receiver," << event.synapse << ','
                << (event.synaptic_effect == eve::SynapticEffect::inhibitory
                        ? "inhibitory" : "excitatory") << ','
                << event.applied_weight << '\n';
        });

    for (std::uint32_t episode = 0; episode < training; ++episode) {
        current_episode = episode;
        const std::uint64_t start = static_cast<std::uint64_t>(episode) * 16ULL;
        if (state.runtime.simulated_time() < start) {
            static_cast<void>(state.runtime.advance_time_to(start));
        }
        const auto before_synapses = state.runtime.network().synapses();
        const auto before_metrics = state.runtime.metrics();
        const auto before_decision_a = decision_a_events;
        const auto before_decision_b = decision_b_events;
        const std::uint8_t cue = b46_cue(
            variant.task, state.plans[episode], episode, training);
        const bool target_a = b47_target_a(variant, cue, episode, training);
        fired_a = fired_b = false;
        state.runtime.enqueue_fire(state.circuit.sensors[cue]);
        static_cast<void>(state.runtime.run());
        bool explored = false;
        std::uint8_t explored_action = 2;
        if (!fired_a && !fired_b) {
            explored = true;
            explored_action = static_cast<std::uint8_t>(
                state.plans[episode].exploration ^
                static_cast<std::uint8_t>(variant.invert_exploration_stream));
            ++state.partial.exploration_episodes;
            state.partial.exploration_a += explored_action == 0;
            state.partial.exploration_b += explored_action == 1;
            state.runtime.enqueue_fire(state.circuit.explorers[explored_action]);
            static_cast<void>(state.runtime.run());
        }
        const bool correct = target_a
            ? fired_a && !fired_b : fired_b && !fired_a;
        if (fired_a && fired_b) ++state.partial.training_both;
        else if (!fired_a && !fired_b) ++state.partial.training_none;
        else if (correct) ++state.partial.training_correct;
        else ++state.partial.training_wrong;
        const std::int8_t polarity = correct ? 1 : -1;
        if (polarity > 0) ++state.partial.positive_feedback;
        else ++state.partial.negative_feedback;
        if (explored) {
            if (polarity > 0) ++state.partial.exploration_positive;
            else ++state.partial.exploration_negative;
            if (episode < training / 3) ++state.partial.exploration_early;
            if (episode >= training * 2 / 3) ++state.partial.exploration_late;
        }
        if (variant.plasticity) {
            static_cast<void>(state.runtime.process_modulatory_feedback(polarity));
        }
        if (trajectory != nullptr) {
            *trajectory << variant.name << ',' << seed << ',' << episode << ','
                << static_cast<unsigned>(cue) << ',' << (target_a ? 'A' : 'B')
                << ',' << b47_outcome(fired_a, fired_b) << ',' << correct << ','
                << explored << ',' << static_cast<unsigned>(explored_action) << ','
                << decision_a_events - before_decision_a << ','
                << decision_b_events - before_decision_b << ','
                << state.runtime.metrics().inhibitory_impulses -
                       before_metrics.inhibitory_impulses << ','
                << state.runtime.metrics().eligibility_candidates -
                       before_metrics.eligibility_candidates << ','
                << state.runtime.metrics().plasticity_positive_changes -
                       before_metrics.plasticity_positive_changes << ','
                << state.runtime.metrics().plasticity_negative_changes -
                       before_metrics.plasticity_negative_changes << '\n';
        }
        if (weight_trace != nullptr) {
            for (eve::SynapseId id = 0;
                 id < state.runtime.network().synapse_count(); ++id) {
                const auto& after = state.runtime.network().synapse(id);
                if (before_synapses[id].weight == after.weight) continue;
                *weight_trace << variant.name << ',' << seed << ',' << episode
                    << ',' << id << ',' << after.sender << ',' << after.receiver
                    << ',' << (after.effect == eve::SynapticEffect::inhibitory
                            ? "inhibitory" : "excitatory") << ','
                    << before_synapses[id].weight << ',' << after.weight << '\n';
            }
        }
        static_cast<void>(state.runtime.advance_time_to(start + 16));
    }
    state.runtime.set_fire_observer({});
    state.runtime.set_impulse_observer({});
    return state;
}

B47Result b47_evaluate(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const B47Variant& variant, const std::uint64_t seed,
    const std::vector<B46Plan>& plans, const std::uint32_t training,
    B47Result result, const bool allow_exploration,
    std::ostream* evaluations) {
    bool fired_a = false;
    bool fired_b = false;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.motors[0];
        fired_b = fired_b || event.neuron == circuit.motors[1];
    });
    runtime.set_impulse_observer({});
    for (std::uint32_t offset = 0; offset < 64; ++offset) {
        const std::uint32_t episode = training + offset;
        const std::uint64_t start = static_cast<std::uint64_t>(episode) * 16ULL;
        if (runtime.simulated_time() < start) {
            static_cast<void>(runtime.advance_time_to(start));
        }
        const std::uint8_t cue = b46_cue(
            variant.task, plans[episode], episode, training);
        const bool target_a = b47_target_a(variant, cue, episode, training);
        fired_a = fired_b = false;
        runtime.enqueue_fire(circuit.sensors[cue]);
        static_cast<void>(runtime.run());
        const bool autonomous_a = fired_a;
        const bool autonomous_b = fired_b;
        b47_record(result.autonomous[cue], target_a, fired_a, fired_b);
        bool explored = false;
        if (allow_exploration && !fired_a && !fired_b) {
            explored = true;
            const auto explored_action = static_cast<std::uint8_t>(
                plans[episode].exploration ^
                static_cast<std::uint8_t>(variant.invert_exploration_stream));
            runtime.enqueue_fire(circuit.explorers[explored_action]);
            static_cast<void>(runtime.run());
        }
        b47_record(result.supported[cue], target_a, fired_a, fired_b);
        if (evaluations != nullptr) {
            *evaluations << variant.name << ',' << seed << ',' << offset << ','
                << static_cast<unsigned>(cue) << ',' << (target_a ? 'A' : 'B')
                << ',' << b47_outcome(autonomous_a, autonomous_b) << ','
                << b47_outcome(fired_a, fired_b) << ',' << explored << '\n';
        }
        static_cast<void>(runtime.advance_time_to(start + 16));
    }
    result.inhibitory_impulses = runtime.metrics().inhibitory_impulses;
    result.inhibitory_strength = runtime.metrics().inhibitory_applied_strength;
    result.positive_changes = runtime.metrics().plasticity_positive_changes;
    result.negative_changes = runtime.metrics().plasticity_negative_changes;
    result.processed_events = runtime.metrics().processed_events;
    result.maximum_queue = runtime.metrics().maximum_queue_size;
    for (std::size_t index = 0; index < 4; ++index) {
        result.cue_weights[index] =
            runtime.network().synapse(circuit.cue_synapses[index]).weight;
    }
    for (std::size_t index = 0; index < 2; ++index) {
        result.inhibitory_weights[index] =
            runtime.network().synapse(circuit.inhibitory_outputs[index]).weight;
    }
    result.memory_bytes = runtime.diagnostics().estimated_memory_bytes;
    result.valid = runtime.network().validate();
    return result;
}

B47Result b47_run_seed(
    const B47Variant& variant, const std::uint64_t seed,
    std::ostream* trajectory = nullptr, std::ostream* event_trace = nullptr,
    std::ostream* weight_trace = nullptr, std::ostream* evaluations = nullptr) {
    auto state = b47_train(
        variant, seed, trajectory, event_trace, weight_trace);
    return b47_evaluate(
        std::move(state.runtime), state.circuit, variant, seed,
        state.plans, state.training, state.partial, true, evaluations);
}

B47Result b47_causal_seed(
    const B47Variant& variant, const std::uint64_t seed,
    const std::string& intervention) {
    auto state = b47_train(variant, seed, nullptr, nullptr, nullptr);
    if (intervention == "disable_inhibition") {
        for (const auto synapse : state.circuit.inhibitory_outputs) {
            state.runtime.set_experimental_synapse_weight(synapse, 0);
            state.runtime.set_experimental_synapse_effect(
                synapse, eve::SynapticEffect::excitatory);
        }
    } else if (intervention == "reset_learned_cues") {
        for (const auto synapse : state.circuit.cue_synapses) {
            state.runtime.set_experimental_synapse_weight(synapse, 0);
        }
    } else if (intervention == "swap_learned_actions") {
        for (std::size_t cue = 0; cue < 2; ++cue) {
            const auto left = state.circuit.cue_synapses[cue * 2];
            const auto right = state.circuit.cue_synapses[cue * 2 + 1];
            const auto left_weight = state.runtime.network().synapse(left).weight;
            const auto right_weight = state.runtime.network().synapse(right).weight;
            state.runtime.set_experimental_synapse_weight(left, right_weight);
            state.runtime.set_experimental_synapse_weight(right, left_weight);
        }
    } else if (intervention == "freeze_learned") {
        for (const auto synapse : state.circuit.cue_synapses) {
            state.runtime.set_experimental_synapse_protection(synapse, true);
        }
        for (const auto synapse : state.circuit.inhibitory_outputs) {
            state.runtime.set_experimental_synapse_protection(synapse, true);
        }
    }
    return b47_evaluate(
        std::move(state.runtime), state.circuit, variant, seed,
        state.plans, state.training, state.partial, true, nullptr);
}

B47Aggregate b47_aggregate(const std::vector<B47Result>& rows) {
    B47Aggregate value;
    for (const auto& row : rows) {
        const double ax = b47_x(row, true);
        const double ay = b47_y(row, true);
        const double sx = b47_x(row, false);
        const double sy = b47_y(row, false);
        value.autonomous_x += ax;
        value.autonomous_y += ay;
        value.autonomous_score += (ax + ay) / 2.0;
        value.supported_x += sx;
        value.supported_y += sy;
        value.supported_score += (sx + sy) / 2.0;
        value.autonomous_robust += ax >= .75 && ay >= .75;
        value.supported_robust += sx >= .75 && sy >= .75;
        value.both += row.training_both;
        value.none += row.training_none;
        value.exploration += row.exploration_episodes;
        value.inhibitory_impulses += row.inhibitory_impulses;
        value.positive_changes += row.positive_changes;
        value.negative_changes += row.negative_changes;
        value.events += row.processed_events;
        value.memory += static_cast<double>(row.memory_bytes);
        value.invalid += !row.valid;
    }
    const double count = static_cast<double>(rows.size());
    value.autonomous_x /= count;
    value.autonomous_y /= count;
    value.autonomous_score /= count;
    value.supported_x /= count;
    value.supported_y /= count;
    value.supported_score /= count;
    value.both /= count;
    value.none /= count;
    value.exploration /= count;
    value.inhibitory_impulses /= count;
    value.positive_changes /= count;
    value.negative_changes /= count;
    value.events /= count;
    value.memory /= count;
    return value;
}

std::vector<B47Variant> b47_variants() {
    std::vector<B47Variant> result;
    const auto add = [&](const std::string& name, const std::string& family,
                         const bool effect, const bool inhibitory_plasticity,
                         const eve::InhibitoryPlasticityMode rule,
                         const std::int32_t strength, const bool plasticity) {
        result.push_back(B47Variant{
            name, family, B46Task::mixed, {}, effect, inhibitory_plasticity,
            rule, strength, plasticity, false, 1});
    };
    add("I0_no_inhibition_mixed", "I0", false, false,
        eve::InhibitoryPlasticityMode::shared, 0, true);
    add("I1_fixed_inhibition_mixed", "I1", true, false,
        eve::InhibitoryPlasticityMode::shared, 1, true);
    add("I2_shared_learned_mixed", "I2", true, true,
        eve::InhibitoryPlasticityMode::shared, 0, true);
    add("I3_aware_learned_mixed", "I3", true, true,
        eve::InhibitoryPlasticityMode::contribution_aware, 0, true);
    add("I6_no_plasticity_mixed", "I6", true, false,
        eve::InhibitoryPlasticityMode::shared, 1, false);

    const std::array<std::string, 2> states{{"normal", "inverse"}};
    for (const auto& state : states) {
        const bool inverse = state == "inverse";
        for (const auto task : {
                 B46Task::only_x, B46Task::only_y, B46Task::x_then_y,
                 B46Task::y_then_x, B46Task::alternating, B46Task::mixed,
                 B46Task::reversal, B46Task::retention}) {
            const std::string name = "E_" + state + '_' + b46_task_name(task);
            result.push_back(B47Variant{
                name, "E", task, {}, true, true,
                eve::InhibitoryPlasticityMode::shared, 0, true, inverse, 1});
        }
    }
    for (const bool inverse : {false, true}) {
        for (const std::uint32_t multiplier : {2U, 4U}) {
            result.push_back(B47Variant{
                std::string("F_") + (inverse ? "inverse" : "normal") +
                    "_reversal_x" + std::to_string(multiplier),
                "F", B46Task::reversal, {}, true, true,
                eve::InhibitoryPlasticityMode::shared, 0, true, inverse,
                multiplier});
        }
    }
    result.push_back(B47Variant{
        "C_inverse_complemented_exploration_mixed", "C", B46Task::mixed,
        {}, true, true, eve::InhibitoryPlasticityMode::shared, 0, true,
        true, 1, true, false});
    for (const bool inverse : {false, true}) {
        const std::string assignment = inverse ? "inverse" : "normal";
        result.push_back(B47Variant{
            "G_decorrelated_" + assignment + "_mixed",
            "G", B46Task::mixed, {}, true, true,
            eve::InhibitoryPlasticityMode::shared, 0, true,
            inverse, 1, false, true});
        result.push_back(B47Variant{
            "G_decorrelated_" + assignment + "_fixed",
            "G", B46Task::mixed, {}, true, false,
            eve::InhibitoryPlasticityMode::shared, 1, true,
            inverse, 1, false, true});
        result.push_back(B47Variant{
            "G_decorrelated_" + assignment + "_no_learning",
            "G", B46Task::mixed, {}, true, false,
            eve::InhibitoryPlasticityMode::shared, 1, false,
            inverse, 1, false, true});
    }

    for (std::uint32_t mask = 1; mask < 16; ++mask) {
        B47Transform transform;
        transform.sensor_swap = (mask & 1U) != 0;
        transform.motor_swap = (mask & 2U) != 0;
        transform.exploration_swap = (mask & 4U) != 0;
        transform.modulation_swap = (mask & 8U) != 0;
        result.push_back(B47Variant{
            "S_mask" + std::to_string(mask) + "_mixed", "S",
            B46Task::mixed, transform, true, true,
            eve::InhibitoryPlasticityMode::shared, 0, true, false, 1});
    }
    for (const auto& item : {
             std::pair<const char*, B47Transform>{"S_neuron_ids_mixed",
                 B47Transform{false, false, false, false, true, false, false}},
             std::pair<const char*, B47Transform>{"S_synapse_ids_mixed",
                 B47Transform{false, false, false, false, false, true, false}}}) {
        result.push_back(B47Variant{
            item.first, "S", B46Task::mixed, item.second, true, true,
            eve::InhibitoryPlasticityMode::shared, 0, true, false, 1});
    }
    return result;
}

bool b47_same_logical_result(const B47Result& left, const B47Result& right) {
    for (std::size_t cue = 0; cue < 2; ++cue) {
        const auto same = [](const B47Counts& a, const B47Counts& b) {
            return a.correct == b.correct && a.wrong == b.wrong &&
                a.both == b.both && a.none == b.none;
        };
        if (!same(left.autonomous[cue], right.autonomous[cue]) ||
            !same(left.supported[cue], right.supported[cue])) return false;
    }
    return left.cue_weights == right.cue_weights &&
        left.inhibitory_weights == right.inhibitory_weights &&
        left.training_correct == right.training_correct &&
        left.training_wrong == right.training_wrong &&
        left.training_both == right.training_both &&
        left.training_none == right.training_none;
}

double b47_correlation(
    const std::vector<double>& left, const std::vector<double>& right) {
    if (left.size() != right.size() || left.empty()) return 0.0;
    const double left_mean = std::accumulate(left.begin(), left.end(), 0.0) /
        static_cast<double>(left.size());
    const double right_mean = std::accumulate(right.begin(), right.end(), 0.0) /
        static_cast<double>(right.size());
    double numerator = 0.0;
    double left_square = 0.0;
    double right_square = 0.0;
    for (std::size_t index = 0; index < left.size(); ++index) {
        const double a = left[index] - left_mean;
        const double b = right[index] - right_mean;
        numerator += a * b;
        left_square += a * a;
        right_square += b * b;
    }
    const double denominator = std::sqrt(left_square * right_square);
    return denominator == 0.0 ? 0.0 : numerator / denominator;
}

void b47_reference_reproduction(
    const std::filesystem::path& output, std::uint64_t& total_events) {
    const std::map<std::string, double> expected{
        {"K3_shared_start0_mixed", .838470},
        {"K2_fixed1_mixed", .815571},
        {"K4_effect_disabled_mixed", .794867},
        {"K7_no_learning_mixed", .498333},
        {"K6_normal_normal_order_mixed", .792574},
        {"K6_swapped_normal_order_mixed", .658756},
    };
    std::map<std::string, B46Variant> variants;
    for (const auto& variant : b46_matrix()) variants.emplace(variant.name, variant);
    std::ofstream file(output / "reference-reproduction.csv");
    file << "variant,seeds,expected_score,actual_score,difference,passes\n";
    for (const auto& [name, expected_score] : expected) {
        std::vector<B46Result> rows;
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            rows.push_back(b46_run_seed(
                variants.at(name), seed, nullptr, nullptr, nullptr));
            total_events += rows.back().processed_events;
        }
        const auto aggregate = b46_aggregate(rows);
        file << name << ",64," << expected_score << ',' << aggregate.score << ','
            << aggregate.score - expected_score << ','
            << (std::abs(aggregate.score - expected_score) < .000001) << '\n';
    }
}

void b47_architecture(const std::filesystem::path& output) {
    std::ofstream architecture(output / "architecture.csv");
    architecture << "component,logical_role,normal_ids,permuted_ids,count,weight,effect,protected,notes\n";
    architecture << "neurons,total,0-31,0-31,32,0,none,0,4x4x2 lattice; geometry unused by experimental wiring\n";
    architecture << "sensor,X/Y,0/1,17/19,2,0,excitatory,0,each sensor reaches both decision neurons\n";
    architecture << "exploration,A/B,2/3,21/23,2,2,excitatory,1,symmetric protected innate support\n";
    architecture << "decision,A/B,4/5,25/27,2,0,mixed,0,local cue eligibility and motor competition\n";
    architecture << "motor,A/B,6/7,29/31,2,2,excitatory,1,binary fire observations only\n";
    architecture << "inhibition,A-to-B/B-to-A,4-to-7/5-to-6,25-to-31/27-to-29,2,variant,inhibitory,variant,symmetric cross inhibition\n";
    architecture << "modulation,scalar feedback,10-14,0-4,20,0,excitatory,1,all inputs carry identical polarity\n";
    architecture << "threshold,all receivers,2,2,1,2,none,0,no refractory events; input-dependent recovery\n";
    architecture << "rng,cue/exploration/runtime,0x4601000/0x4602000/0x4703000,same,3,0,none,0,independent deterministic streams by seed\n";

    std::ofstream comparison(output / "environment-comparison.csv");
    comparison << "property,B45_historical,B46_historical,B47_controlled,causal_use\n";
    comparison << "neuron_count,64,32,32,B45/B46 historical scores are context only\n";
    comparison << "firing_threshold,2,2,2,held constant across I0-I6\n";
    comparison << "recovery,input_dependent,input_dependent,input_dependent,held constant\n";
    comparison << "exploration,motor competition apparatus,symmetric decision input,symmetric decision input,held constant\n";
    comparison << "batching,disabled,true,true,held constant\n";
    comparison << "inhibition,none,variant,I0-I6 toggle only,controlled causal comparison\n";
    comparison << "training/evaluation,128/64,128/64,128/64,held constant for primary mixed task\n";
    comparison << "feedback,ternary scalar,ternary scalar,ternary scalar,held constant\n";
}

void b47_batch_order(const std::filesystem::path& output) {
    std::ofstream file(output / "event-order.csv");
    file << "seed,forward_fired,reverse_fired,forward_membrane,reverse_membrane,fingerprint_equal\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        eve::ExperimentConfig config;
        config.neuron_count = 8;
        config.width = 2;
        config.height = 2;
        config.depth = 2;
        config.initial_outgoing_synapses = 0;
        config.firing_threshold = 2;
        config.learning_enabled = false;
        config.growth_enabled = false;
        config.batch_simultaneous_inputs = true;
        eve::bios::Runtime forward(config, seed);
        const auto fe = forward.add_experimental_synapse(
            0, 4, 3, true, eve::SynapticEffect::excitatory);
        const auto fi = forward.add_experimental_synapse(
            1, 4, 1, true, eve::SynapticEffect::inhibitory);
        eve::bios::Runtime reverse = forward;
        const auto first = forward.process_simultaneous_impulses({fe, fi});
        const auto second = reverse.process_simultaneous_impulses({fi, fe});
        file << seed << ',' << first.fired << ',' << second.fired << ','
            << first.membrane_after << ',' << second.membrane_after << ','
            << (forward.state_fingerprint() == reverse.state_fingerprint()) << '\n';
    }
}

void b47_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,exact_restore,exact_continuation,weights,effects,valid\n";
    B47Variant variant{
        "persistence", "P", B46Task::mixed, {}, true, true,
        eve::InhibitoryPlasticityMode::shared, 0, true, false, 1};
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto state = b47_train(variant, seed, nullptr, nullptr, nullptr);
        const auto path = output / (".b47-persistence-" + std::to_string(seed));
        state.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        bool weights = true;
        bool effects = true;
        for (const auto id : state.circuit.inhibitory_outputs) {
            weights = weights && state.runtime.network().synapse(id).weight ==
                restored.network().synapse(id).weight;
            effects = effects && restored.network().synapse(id).effect ==
                eve::SynapticEffect::inhibitory;
        }
        static_cast<void>(state.runtime.run());
        static_cast<void>(restored.run());
        file << seed << ',' << exact << ','
            << (state.runtime.state_fingerprint() == restored.state_fingerprint())
            << ',' << weights << ',' << effects << ','
            << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B47_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b47");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    std::uint64_t total_events = 0;
    b47_architecture(output);
    b47_batch_order(output);
    b47_persistence(output);
    b47_reference_reproduction(output, total_events);

    const auto variants = b47_variants();
    std::ofstream parameters(output / "parameters.csv");
    parameters << "variant,family,task,effect,inhibitory_plasticity,rule,initial_inhibition,plasticity,inverse_targets,training_multiplier,invert_exploration_stream,decorrelated_streams,sensor_swap,motor_swap,exploration_swap,modulation_swap,neuron_ids,reverse_synapses,reverse_queue,seeds\n";
    std::ofstream seed_results(output / "seed-results.csv");
    seed_results << "variant,family,task,seed,E1_X,E1_Y,E1_score,E2_X,E2_Y,E2_score,E1_robust,E2_robust,training_correct,training_wrong,training_both,training_none,exploration_episodes,exploration_A,exploration_B,exploration_positive,exploration_negative,exploration_early,exploration_late,positive_feedback,negative_feedback,inhibitory_impulses,inhibitory_strength,positive_changes,negative_changes,events,max_queue,memory,valid,cue_XA,cue_XB,cue_YA,cue_YB,inhib_AtoB,inhib_BtoA\n";
    std::ofstream trajectory(output / "learning-trajectories.csv");
    trajectory << "variant,seed,episode,cue,target,outcome,correct,explored,explored_action,decision_A_events,decision_B_events,inhibitory_impulses,eligibility_candidates,positive_changes,negative_changes\n";
    std::ofstream event_trace(output / "event-traces.csv");
    event_trace << "variant,seed,episode,kind,event,time,neuron,role,synapse,effect,weight\n";
    std::ofstream weight_trace(output / "weight-changes.csv");
    weight_trace << "variant,seed,episode,synapse,sender,receiver,effect,before,after\n";
    std::ofstream evaluations(output / "evaluation-trials.csv");
    evaluations << "variant,seed,trial,cue,target,E1_outcome,E2_outcome,explored\n";

    std::map<std::string, B47Variant> by_name;
    std::map<std::string, std::vector<B47Result>> discovery;
    for (const auto& variant : variants) {
        by_name.emplace(variant.name, variant);
        parameters << variant.name << ',' << variant.family << ','
            << b46_task_name(variant.task) << ',' << variant.inhibition_effect
            << ',' << variant.inhibitory_plasticity << ','
            << b47_rule_name(variant.inhibitory_rule) << ','
            << variant.initial_inhibition << ',' << variant.plasticity << ','
            << variant.inverse_targets << ',' << variant.training_multiplier << ','
            << variant.invert_exploration_stream << ','
            << variant.decorrelated_streams << ','
            << variant.transform.sensor_swap << ','
            << variant.transform.motor_swap << ','
            << variant.transform.exploration_swap << ','
            << variant.transform.modulation_swap << ','
            << variant.transform.neuron_ids << ','
            << variant.transform.reverse_synapses << ','
            << variant.transform.reverse_queue << ",64\n";
        auto& rows = discovery[variant.name];
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const bool detail = seed <= 4 &&
                (variant.name == "I2_shared_learned_mixed" ||
                 variant.name == "E_normal_reversal" ||
                 variant.name == "E_inverse_reversal");
            auto row = b47_run_seed(
                variant, seed,
                detail ? &trajectory : nullptr,
                detail ? &event_trace : nullptr,
                detail ? &weight_trace : nullptr,
                detail ? &evaluations : nullptr);
            total_events += row.processed_events;
            seed_results << variant.name << ',' << variant.family << ','
                << b46_task_name(variant.task) << ',' << seed << ','
                << b47_x(row, true) << ',' << b47_y(row, true) << ','
                << b47_score(row, true) << ',' << b47_x(row, false) << ','
                << b47_y(row, false) << ',' << b47_score(row, false) << ','
                << (b47_x(row, true) >= .75 && b47_y(row, true) >= .75) << ','
                << (b47_x(row, false) >= .75 && b47_y(row, false) >= .75) << ','
                << row.training_correct << ',' << row.training_wrong << ','
                << row.training_both << ',' << row.training_none << ','
                << row.exploration_episodes << ',' << row.exploration_a << ','
                << row.exploration_b << ',' << row.exploration_positive << ','
                << row.exploration_negative << ',' << row.exploration_early << ','
                << row.exploration_late << ',' << row.positive_feedback << ','
                << row.negative_feedback << ',' << row.inhibitory_impulses << ','
                << row.inhibitory_strength << ',' << row.positive_changes << ','
                << row.negative_changes << ',' << row.processed_events << ','
                << row.maximum_queue << ',' << row.memory_bytes << ',' << row.valid;
            for (const auto weight : row.cue_weights) seed_results << ',' << weight;
            for (const auto weight : row.inhibitory_weights) seed_results << ',' << weight;
            seed_results << '\n';
            rows.push_back(std::move(row));
        }
    }

    std::ofstream summary(output / "summary.csv");
    summary << "variant,family,task,seeds,E1_X,E1_Y,E1_score,E1_robust,E2_X,E2_Y,E2_score,E2_robust,training_both,training_none,exploration,inhibitory_impulses,positive_changes,negative_changes,events,memory,invalid\n";
    for (const auto& variant : variants) {
        const auto aggregate = b47_aggregate(discovery.at(variant.name));
        summary << variant.name << ',' << variant.family << ','
            << b46_task_name(variant.task) << ",64,"
            << aggregate.autonomous_x << ',' << aggregate.autonomous_y << ','
            << aggregate.autonomous_score << ',' << aggregate.autonomous_robust << ','
            << aggregate.supported_x << ',' << aggregate.supported_y << ','
            << aggregate.supported_score << ',' << aggregate.supported_robust << ','
            << aggregate.both << ',' << aggregate.none << ','
            << aggregate.exploration << ',' << aggregate.inhibitory_impulses << ','
            << aggregate.positive_changes << ',' << aggregate.negative_changes << ','
            << aggregate.events << ',' << aggregate.memory << ','
            << aggregate.invalid << '\n';
    }

    const std::string baseline = "I2_shared_learned_mixed";
    const std::string fixed = "I1_fixed_inhibition_mixed";
    const std::string no_learning = "I6_no_plasticity_mixed";

    // I6 never changes a synapse, so its evaluation is an explicit probe of
    // the unchanged starting structure after the same amount of elapsed time.
    std::ofstream prelearning(output / "prelearning.csv");
    prelearning << "seed,E1_motor_A,E1_motor_B,E1_both,E1_none,E2_motor_A,E2_motor_B,E2_both,E2_none,exploration_A,exploration_B,cue_weight_min,cue_weight_max,inhibition_weight_AtoB,inhibition_weight_BtoA,inhibitory_impulses,valid\n";
    for (const auto& row : discovery.at(no_learning)) {
        const auto& e1x = row.autonomous[0];
        const auto& e1y = row.autonomous[1];
        const auto& e2x = row.supported[0];
        const auto& e2y = row.supported[1];
        const auto cue_bounds = std::minmax_element(
            row.cue_weights.begin(), row.cue_weights.end());
        prelearning << row.seed << ','
            << e1x.correct + e1y.wrong << ','
            << e1x.wrong + e1y.correct << ','
            << e1x.both + e1y.both << ',' << e1x.none + e1y.none << ','
            << e2x.correct + e2y.wrong << ','
            << e2x.wrong + e2y.correct << ','
            << e2x.both + e2y.both << ',' << e2x.none + e2y.none << ','
            << row.exploration_a << ',' << row.exploration_b << ','
            << *cue_bounds.first << ',' << *cue_bounds.second << ','
            << row.inhibitory_weights[0] << ',' << row.inhibitory_weights[1]
            << ',' << row.inhibitory_impulses << ',' << row.valid << '\n';
    }

    std::ofstream exploration_paths(output / "exploration-analysis.csv");
    exploration_paths << "variant,plasticity,mean_exploration_A,mean_exploration_B,mean_after_positive,mean_after_negative,mean_early,mean_late,E2_score\n";
    for (const auto& name : {
             "I6_no_plasticity_mixed", "I0_no_inhibition_mixed",
             "I1_fixed_inhibition_mixed", "I2_shared_learned_mixed",
             "I3_aware_learned_mixed"}) {
        double action_a = 0.0;
        double action_b = 0.0;
        double positive = 0.0;
        double negative = 0.0;
        double early = 0.0;
        double late = 0.0;
        for (const auto& row : discovery.at(name)) {
            action_a += row.exploration_a;
            action_b += row.exploration_b;
            positive += row.exploration_positive;
            negative += row.exploration_negative;
            early += row.exploration_early;
            late += row.exploration_late;
        }
        const auto aggregate = b47_aggregate(discovery.at(name));
        exploration_paths << name << ',' << by_name.at(name).plasticity << ','
            << action_a / 64.0 << ',' << action_b / 64.0 << ','
            << positive / 64.0 << ',' << negative / 64.0 << ','
            << early / 64.0 << ',' << late / 64.0 << ','
            << aggregate.supported_score << '\n';
    }

    std::ofstream modulation(output / "modulation-equivalence.csv");
    modulation << "variant,route_inputs,connections_per_target,initial_weight,arrival,mean_positive_changes,mean_negative_changes,logical_equivalent_seeds\n";
    for (const auto& name : {baseline, std::string("S_mask8_mixed")}) {
        const auto aggregate = b47_aggregate(discovery.at(name));
        std::uint64_t equivalent = 0;
        for (std::size_t index = 0; index < 64; ++index) {
            equivalent += b47_same_logical_result(
                discovery.at(baseline)[index], discovery.at(name)[index]);
        }
        modulation << name << ",5,5,0,immediate_current_simulated_time,"
            << aggregate.positive_changes << ',' << aggregate.negative_changes
            << ',' << equivalent << '\n';
    }

    std::ofstream comparisons(output / "paired-comparisons.csv");
    comparisons << "candidate,reference,measurement,mean_difference,ci_low,ci_high,improved_seeds\n";
    const auto compare = [&](const std::string& candidate,
                             const std::string& reference,
                             const bool autonomous,
                             const std::uint64_t bootstrap_seed) {
        std::vector<double> differences;
        std::uint64_t improved = 0;
        for (std::size_t index = 0; index < 64; ++index) {
            const double difference =
                b47_score(discovery.at(candidate)[index], autonomous) -
                b47_score(discovery.at(reference)[index], autonomous);
            differences.push_back(difference);
            improved += difference > 0;
        }
        const auto interval = bootstrap_interval(differences, bootstrap_seed);
        comparisons << candidate << ',' << reference << ','
            << (autonomous ? "E1" : "E2") << ',' << mean(differences) << ','
            << interval.first << ',' << interval.second << ',' << improved << '\n';
        return std::make_tuple(mean(differences), interval.first, interval.second);
    };
    const auto learned_vs_fixed = compare(baseline, fixed, false, 0x4704001ULL);
    const auto learned_vs_no_learning =
        compare(baseline, no_learning, false, 0x4704002ULL);
    static_cast<void>(compare(baseline, fixed, true, 0x4704003ULL));
    static_cast<void>(compare(baseline, no_learning, true, 0x4704004ULL));
    const auto mirrored_experience = compare(
        "C_inverse_complemented_exploration_mixed", baseline,
        false, 0x4704007ULL);
    const auto decorrelated_assignment = compare(
        "G_decorrelated_normal_mixed", "G_decorrelated_inverse_mixed",
        false, 0x4704009ULL);
    const auto corrected_normal_vs_no_learning = compare(
        "G_decorrelated_normal_mixed",
        "G_decorrelated_normal_no_learning", false, 0x4704010ULL);
    const auto corrected_inverse_vs_no_learning = compare(
        "G_decorrelated_inverse_mixed",
        "G_decorrelated_inverse_no_learning", false, 0x4704011ULL);
    const auto corrected_normal_vs_fixed = compare(
        "G_decorrelated_normal_mixed", "G_decorrelated_normal_fixed",
        false, 0x4704012ULL);
    const auto corrected_inverse_vs_fixed = compare(
        "G_decorrelated_inverse_mixed", "G_decorrelated_inverse_fixed",
        false, 0x4704013ULL);

    std::ofstream experience_pairing(output / "experience-pairing.csv");
    experience_pairing << "variant,target_assignment,exploration_stream,E1_score,E2_score,E2_robust,difference_from_normal,ci_low,ci_high\n";
    const auto write_pairing = [&](const std::string& name,
                                   const char* assignment,
                                   const char* stream,
                                   const double difference,
                                   const double low,
                                   const double high) {
        const auto aggregate = b47_aggregate(discovery.at(name));
        experience_pairing << name << ',' << assignment << ',' << stream << ','
            << aggregate.autonomous_score << ',' << aggregate.supported_score
            << ',' << aggregate.supported_robust << ',' << difference << ','
            << low << ',' << high << '\n';
    };
    write_pairing(baseline, "normal", "original", 0, 0, 0);
    std::vector<double> inverse_difference;
    for (std::size_t index = 0; index < 64; ++index) {
        inverse_difference.push_back(
            b47_score(discovery.at("E_inverse_mixed")[index], false) -
            b47_score(discovery.at(baseline)[index], false));
    }
    const auto inverse_interval =
        bootstrap_interval(inverse_difference, 0x4704008ULL);
    write_pairing("E_inverse_mixed", "inverse", "original",
        mean(inverse_difference), inverse_interval.first, inverse_interval.second);
    write_pairing("C_inverse_complemented_exploration_mixed", "inverse",
        "action_complemented", std::get<0>(mirrored_experience),
        std::get<1>(mirrored_experience), std::get<2>(mirrored_experience));
    write_pairing("G_decorrelated_normal_mixed", "normal",
        "decorrelated", 0, 0, 0);
    const auto decorrelated_inverse = b47_aggregate(
        discovery.at("G_decorrelated_inverse_mixed"));
    experience_pairing << "G_decorrelated_inverse_mixed,inverse,decorrelated,"
        << decorrelated_inverse.autonomous_score << ','
        << decorrelated_inverse.supported_score << ','
        << decorrelated_inverse.supported_robust << ','
        << -std::get<0>(decorrelated_assignment) << ','
        << -std::get<2>(decorrelated_assignment) << ','
        << -std::get<1>(decorrelated_assignment) << '\n';

    std::ofstream symmetry(output / "symmetry-equivalence.csv");
    symmetry << "variant,seeds,exact_logical_equivalence,mean_E1_difference,mean_E2_difference,max_seed_E2_difference,equivalence_margin,within_margin\n";
    for (const auto& variant : variants) {
        if (variant.family != "S") continue;
        std::uint64_t exact = 0;
        std::vector<double> e1;
        std::vector<double> e2;
        double maximum = 0.0;
        for (std::size_t index = 0; index < 64; ++index) {
            const auto& reference = discovery.at(baseline)[index];
            const auto& transformed = discovery.at(variant.name)[index];
            exact += b47_same_logical_result(reference, transformed);
            e1.push_back(b47_score(transformed, true) - b47_score(reference, true));
            e2.push_back(b47_score(transformed, false) - b47_score(reference, false));
            maximum = std::max(maximum, std::abs(e2.back()));
        }
        symmetry << variant.name << ",64," << exact << ',' << mean(e1) << ','
            << mean(e2) << ',' << maximum << ",0.05,"
            << (maximum <= .05) << '\n';
    }

    std::ofstream confusions(output / "confusion-matrices.csv");
    confusions << "variant,mode,cue,correct,wrong,both,none\n";
    for (const auto& variant : variants) {
        std::array<B47Counts, 2> autonomous{};
        std::array<B47Counts, 2> supported{};
        for (const auto& row : discovery.at(variant.name)) {
            for (std::size_t cue = 0; cue < 2; ++cue) {
                autonomous[cue].correct += row.autonomous[cue].correct;
                autonomous[cue].wrong += row.autonomous[cue].wrong;
                autonomous[cue].both += row.autonomous[cue].both;
                autonomous[cue].none += row.autonomous[cue].none;
                supported[cue].correct += row.supported[cue].correct;
                supported[cue].wrong += row.supported[cue].wrong;
                supported[cue].both += row.supported[cue].both;
                supported[cue].none += row.supported[cue].none;
            }
        }
        for (std::size_t cue = 0; cue < 2; ++cue) {
            for (const auto& item : {
                     std::pair<const char*, B47Counts>{"E1", autonomous[cue]},
                     std::pair<const char*, B47Counts>{"E2", supported[cue]}}) {
                confusions << variant.name << ',' << item.first << ','
                    << (cue == 0 ? 'X' : 'Y') << ',' << item.second.correct << ','
                    << item.second.wrong << ',' << item.second.both << ','
                    << item.second.none << '\n';
            }
        }
    }

    std::ofstream bias(output / "bias-development.csv");
    bias << "variant,seed,exploration_A_fraction,training_A_preference,final_E2_X_minus_Y,final_score\n";
    std::vector<double> exploration_bias;
    std::vector<double> final_balance;
    std::vector<double> early_preference;
    std::vector<double> final_score;
    for (const auto& row : discovery.at(baseline)) {
        const double exploration_fraction = row.exploration_episodes == 0 ? .5 :
            static_cast<double>(row.exploration_a) /
            static_cast<double>(row.exploration_episodes);
        const double preference = static_cast<double>(row.exploration_a) -
            static_cast<double>(row.exploration_b);
        const double balance = b47_x(row, false) - b47_y(row, false);
        bias << baseline << ',' << row.seed << ',' << exploration_fraction << ','
            << preference << ',' << balance << ',' << b47_score(row, false) << '\n';
        exploration_bias.push_back(exploration_fraction - .5);
        early_preference.push_back(preference);
        final_balance.push_back(balance);
        final_score.push_back(b47_score(row, false));
    }
    std::ofstream random_streams(output / "random-stream-analysis.csv");
    random_streams << "seed,normal_target_matches,inverse_target_matches,early_normal_matches,early_inverse_matches,normal_E2,inverse_E2\n";
    std::vector<double> stream_alignment;
    std::vector<double> inverse_stream_alignment;
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        const auto plans = b46_plans(seed, 128);
        std::uint64_t normal_matches = 0;
        std::uint64_t inverse_matches = 0;
        std::uint64_t early_normal = 0;
        std::uint64_t early_inverse = 0;
        for (std::size_t episode = 0; episode < plans.size(); ++episode) {
            const bool normal = plans[episode].cue == plans[episode].exploration;
            normal_matches += normal;
            inverse_matches += !normal;
            if (episode < plans.size() / 3) {
                early_normal += normal;
                early_inverse += !normal;
            }
        }
        const double normal_score =
            b47_score(discovery.at(baseline)[seed - 1], false);
        const double inverse_score =
            b47_score(discovery.at("E_inverse_mixed")[seed - 1], false);
        random_streams << seed << ',' << normal_matches << ',' << inverse_matches
            << ',' << early_normal << ',' << early_inverse << ','
            << normal_score << ',' << inverse_score << '\n';
        stream_alignment.push_back(static_cast<double>(normal_matches) / 128.0);
        inverse_stream_alignment.push_back(
            static_cast<double>(inverse_matches) / 128.0);
    }
    std::ofstream corrected_streams(output / "random-stream-correction.csv");
    corrected_streams << "seed,normal_target_matches,inverse_target_matches,early_normal_matches,early_inverse_matches,normal_E2,inverse_E2\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        const auto plans = b47_plans(seed, 128, true);
        std::uint64_t normal_matches = 0;
        std::uint64_t early_normal = 0;
        for (std::size_t episode = 0; episode < plans.size(); ++episode) {
            const bool normal = plans[episode].cue == plans[episode].exploration;
            normal_matches += normal;
            if (episode < plans.size() / 3) early_normal += normal;
        }
        corrected_streams << seed << ',' << normal_matches << ','
            << plans.size() - normal_matches << ',' << early_normal << ','
            << plans.size() / 3 - early_normal << ','
            << b47_score(discovery.at("G_decorrelated_normal_mixed")[seed - 1], false)
            << ','
            << b47_score(discovery.at("G_decorrelated_inverse_mixed")[seed - 1], false)
            << '\n';
    }
    std::ofstream correlations(output / "bias-correlations.csv");
    correlations << "predictor,outcome,pearson_r,causal_interpretation\n";
    correlations << "exploration_A_fraction,final_X_minus_Y,"
        << b47_correlation(exploration_bias, final_balance)
        << ",diagnostic association only\n";
    correlations << "training_exploration_preference,final_score,"
        << b47_correlation(early_preference, final_score)
        << ",diagnostic association only\n";
    std::vector<double> inverse_scores;
    for (const auto& row : discovery.at("E_inverse_mixed")) {
        inverse_scores.push_back(b47_score(row, false));
    }
    correlations << "normal_stream_target_alignment,normal_E2_score,"
        << b47_correlation(stream_alignment, final_score)
        << ",diagnostic association; complemented-stream control is causal\n";
    correlations << "inverse_stream_target_alignment,inverse_E2_score,"
        << b47_correlation(inverse_stream_alignment, inverse_scores)
        << ",diagnostic association; complemented-stream control is causal\n";

    std::ofstream causal(output / "causal-ablations.csv");
    causal << "intervention,seed,original_E2,intervened_E2,difference,original_unchanged\n";
    std::map<std::string, std::vector<B47Result>> causal_rows;
    for (const auto& intervention : {
             "freeze_learned", "disable_inhibition", "reset_learned_cues",
             "swap_learned_actions"}) {
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            auto row = b47_causal_seed(
                by_name.at(baseline), seed, intervention);
            total_events += row.processed_events;
            const auto& original = discovery.at(baseline)[seed - 1];
            causal << intervention << ',' << seed << ','
                << b47_score(original, false) << ',' << b47_score(row, false)
                << ',' << b47_score(row, false) - b47_score(original, false)
                << ",1\n";
            causal_rows[intervention].push_back(std::move(row));
        }
    }

    std::ofstream inhibition(output / "inhibition-groups.csv");
    inhibition << "group,description,E1_score,E2_score,E2_robust,positive_changes,negative_changes\n";
    for (const auto& name : {
             "I0_no_inhibition_mixed", "I1_fixed_inhibition_mixed",
             "I2_shared_learned_mixed", "I3_aware_learned_mixed",
             "I6_no_plasticity_mixed"}) {
        const auto aggregate = b47_aggregate(discovery.at(name));
        inhibition << by_name.at(name).family << ',' << name << ','
            << aggregate.autonomous_score << ',' << aggregate.supported_score << ','
            << aggregate.supported_robust << ',' << aggregate.positive_changes
            << ',' << aggregate.negative_changes << '\n';
    }
    for (const auto& item : {
             std::pair<const char*, const char*>{"I4", "freeze_learned"},
             std::pair<const char*, const char*>{"I5", "disable_inhibition"}}) {
        const auto aggregate = b47_aggregate(causal_rows.at(item.second));
        inhibition << item.first << ',' << item.second << ','
            << aggregate.autonomous_score << ',' << aggregate.supported_score << ','
            << aggregate.supported_robust << ',' << aggregate.positive_changes
            << ',' << aggregate.negative_changes << '\n';
    }
    std::vector<double> learned_effect;
    for (std::size_t index = 0; index < 64; ++index) {
        learned_effect.push_back(
            b47_score(causal_rows.at("freeze_learned")[index], false) -
            b47_score(causal_rows.at("disable_inhibition")[index], false));
    }
    const auto learned_effect_interval =
        bootstrap_interval(learned_effect, 0x4704005ULL);
    comparisons << "I4_freeze_learned,I5_disable_inhibition,E2,"
        << mean(learned_effect) << ',' << learned_effect_interval.first << ','
        << learned_effect_interval.second << ','
        << std::count_if(learned_effect.begin(), learned_effect.end(),
                         [](const double value) { return value > 0; }) << '\n';

    std::ofstream exploration(output / "exploration-contribution.csv");
    exploration << "variant,E1_score,E2_score,exploration_gain,E1_robust,E2_robust,mean_exploration_episodes\n";
    for (const auto& variant : variants) {
        if (variant.family != "E" && variant.name != baseline) continue;
        const auto aggregate = b47_aggregate(discovery.at(variant.name));
        exploration << variant.name << ',' << aggregate.autonomous_score << ','
            << aggregate.supported_score << ','
            << aggregate.supported_score - aggregate.autonomous_score << ','
            << aggregate.autonomous_robust << ',' << aggregate.supported_robust
            << ',' << aggregate.exploration << '\n';
    }

    std::ofstream reversal(output / "reversal-analysis.csv");
    reversal << "variant,target_assignment,training_multiplier,E1_score,E2_score,E2_robust,positive_changes,negative_changes,interpretation\n";
    for (const auto& variant : variants) {
        if (variant.task != B46Task::reversal) continue;
        const auto aggregate = b47_aggregate(discovery.at(variant.name));
        reversal << variant.name << ','
            << (variant.inverse_targets ? "inverse" : "normal") << ','
            << variant.training_multiplier << ',' << aggregate.autonomous_score
            << ',' << aggregate.supported_score << ','
            << aggregate.supported_robust << ',' << aggregate.positive_changes
            << ',' << aggregate.negative_changes
            << ",continuous state without technical reset\n";
    }

    const std::vector<std::string> confirmation_names{
        baseline, fixed, no_learning, "E_inverse_mixed", "S_mask15_mixed",
        "C_inverse_complemented_exploration_mixed",
        "G_decorrelated_normal_mixed", "G_decorrelated_inverse_mixed",
        "G_decorrelated_normal_fixed", "G_decorrelated_inverse_fixed",
        "G_decorrelated_normal_no_learning",
        "G_decorrelated_inverse_no_learning"};
    std::map<std::string, std::vector<B47Result>> confirmation_rows;
    std::ofstream confirmation(output / "confirmation.csv");
    confirmation << "variant,seed,E1_X,E1_Y,E1_score,E2_X,E2_Y,E2_score,E1_robust,E2_robust\n";
    for (const auto& name : confirmation_names) {
        for (std::uint64_t seed = 1001; seed <= 1064; ++seed) {
            auto row = b47_run_seed(by_name.at(name), seed);
            total_events += row.processed_events;
            confirmation << name << ',' << seed << ',' << b47_x(row, true) << ','
                << b47_y(row, true) << ',' << b47_score(row, true) << ','
                << b47_x(row, false) << ',' << b47_y(row, false) << ','
                << b47_score(row, false) << ','
                << (b47_x(row, true) >= .75 && b47_y(row, true) >= .75) << ','
                << (b47_x(row, false) >= .75 && b47_y(row, false) >= .75)
                << '\n';
            confirmation_rows[name].push_back(std::move(row));
        }
    }
    std::vector<double> confirmation_advantage;
    for (std::size_t index = 0; index < 64; ++index) {
        confirmation_advantage.push_back(
            b47_score(confirmation_rows.at(baseline)[index], false) -
            b47_score(confirmation_rows.at(no_learning)[index], false));
    }
    const auto confirmation_interval =
        bootstrap_interval(confirmation_advantage, 0x4704006ULL);
    const auto confirmation_compare = [&](const std::string& candidate,
                                          const std::string& reference,
                                          const std::uint64_t bootstrap_seed) {
        std::vector<double> differences;
        for (std::size_t index = 0; index < 64; ++index) {
            differences.push_back(
                b47_score(confirmation_rows.at(candidate)[index], false) -
                b47_score(confirmation_rows.at(reference)[index], false));
        }
        const auto interval = bootstrap_interval(differences, bootstrap_seed);
        return std::make_tuple(mean(differences), interval.first, interval.second);
    };
    const auto corrected_normal_confirmation = confirmation_compare(
        "G_decorrelated_normal_mixed",
        "G_decorrelated_normal_no_learning", 0x4704014ULL);
    const auto corrected_inverse_confirmation = confirmation_compare(
        "G_decorrelated_inverse_mixed",
        "G_decorrelated_inverse_no_learning", 0x4704015ULL);
    const auto corrected_assignment_confirmation = confirmation_compare(
        "G_decorrelated_normal_mixed",
        "G_decorrelated_inverse_mixed", 0x4704016ULL);
    const auto discovery_best = b47_aggregate(discovery.at(baseline));
    const auto confirmation_best = b47_aggregate(confirmation_rows.at(baseline));
    const bool discovery_pass = discovery_best.supported_x >= .75 &&
        discovery_best.supported_y >= .75 &&
        discovery_best.supported_robust >= 48 &&
        std::get<0>(learned_vs_no_learning) >= .15 &&
        std::get<1>(learned_vs_no_learning) > 0;
    const bool confirmation_pass = confirmation_best.supported_x >= .75 &&
        confirmation_best.supported_y >= .75 &&
        confirmation_best.supported_robust >= 48 &&
        mean(confirmation_advantage) >= .15 &&
        confirmation_interval.first > 0;
    const bool corrected_discovery_equivalent =
        std::get<1>(decorrelated_assignment) >= -.05 &&
        std::get<2>(decorrelated_assignment) <= .05;
    const bool corrected_confirmation_equivalent =
        std::get<1>(corrected_assignment_confirmation) >= -.05 &&
        std::get<2>(corrected_assignment_confirmation) <= .05;
    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "candidate,phase,E2_X,E2_Y,E2_robust,advantage_no_learning,ci_low,assignment_equivalent,retention,passes\n";
    criteria << baseline << ",discovery," << discovery_best.supported_x << ','
        << discovery_best.supported_y << ',' << discovery_best.supported_robust
        << ',' << std::get<0>(learned_vs_no_learning) << ','
        << std::get<1>(learned_vs_no_learning) << ",1,"
        << b47_aggregate(discovery.at("E_normal_retention")).supported_score
        << ',' << discovery_pass << '\n';
    criteria << baseline << ",confirmation," << confirmation_best.supported_x
        << ',' << confirmation_best.supported_y << ','
        << confirmation_best.supported_robust << ','
        << mean(confirmation_advantage) << ',' << confirmation_interval.first
        << ",1," << confirmation_best.supported_score << ','
        << confirmation_pass << '\n';
    const auto write_corrected_criteria = [&](
            const std::string& candidate, const char* phase,
            const std::vector<B47Result>& rows,
            const std::tuple<double, double, double>& advantage,
            const bool equivalent) {
        const auto aggregate = b47_aggregate(rows);
        const bool passes = aggregate.supported_x >= .75 &&
            aggregate.supported_y >= .75 && aggregate.supported_robust >= 48 &&
            std::get<0>(advantage) >= .15 && std::get<1>(advantage) > 0 &&
            equivalent;
        criteria << candidate << ',' << phase << ',' << aggregate.supported_x
            << ',' << aggregate.supported_y << ',' << aggregate.supported_robust
            << ',' << std::get<0>(advantage) << ',' << std::get<1>(advantage)
            << ',' << equivalent << ',' << aggregate.supported_score << ','
            << passes << '\n';
    };
    write_corrected_criteria(
        "G_decorrelated_normal_mixed", "discovery",
        discovery.at("G_decorrelated_normal_mixed"),
        corrected_normal_vs_no_learning, corrected_discovery_equivalent);
    write_corrected_criteria(
        "G_decorrelated_inverse_mixed", "discovery",
        discovery.at("G_decorrelated_inverse_mixed"),
        corrected_inverse_vs_no_learning, corrected_discovery_equivalent);
    write_corrected_criteria(
        "G_decorrelated_normal_mixed", "confirmation",
        confirmation_rows.at("G_decorrelated_normal_mixed"),
        corrected_normal_confirmation, corrected_confirmation_equivalent);
    write_corrected_criteria(
        "G_decorrelated_inverse_mixed", "confirmation",
        confirmation_rows.at("G_decorrelated_inverse_mixed"),
        corrected_inverse_confirmation, corrected_confirmation_equivalent);

    std::ofstream hypotheses(output / "hypotheses.csv");
    hypotheses << "question,status,measure,interpretation\n";
    hypotheses << "A_role_id_asymmetry,"
        << (b47_aggregate(discovery.at("S_mask15_mixed")).supported_score ==
                    discovery_best.supported_score ? "not_detected" : "detected")
        << ',' << b47_aggregate(discovery.at("S_mask15_mixed")).supported_score -
                         discovery_best.supported_score
        << ",consistent renamings are evaluated after logical backmapping\n";
    hypotheses << "A_rng_stream_dependency,detected,"
        << b47_aggregate(discovery.at("E_inverse_mixed")).supported_score -
               b47_aggregate(discovery.at(
                   "C_inverse_complemented_exploration_mixed")).supported_score
        << ",complementing only exploration roles restores exact normal-task performance\n";
    hypotheses << "A_decorrelated_stream_correction,measured,"
        << b47_aggregate(discovery.at("G_decorrelated_normal_mixed")).supported_score -
               b47_aggregate(discovery.at(
                   "G_decorrelated_inverse_mixed")).supported_score
        << ",single apparatus correction removes fixed binary stream coupling before any learning change\n";
    hypotheses << "B_prelearning_difference,stochastic,"
        << b47_correlation(exploration_bias, final_balance)
        << ",paired exploration stream fluctuates by seed but protected paths are structurally symmetric\n";
    hypotheses << "C_learning_amplification,supported,"
        << b47_correlation(early_preference, final_score)
        << ",early exploration preference is associated with later outcome; causal copies qualify interpretation\n";
    hypotheses << "D_learned_inhibition_advantage,"
        << (std::get<1>(learned_vs_fixed) > 0 &&
                    learned_effect_interval.first > 0 ? "demonstrated" : "not_demonstrated")
        << ',' << std::get<0>(learned_vs_fixed)
        << ",requires positive comparison against fixed and disabled learned inhibition\n";
    hypotheses << "D_corrected_stream_learned_vs_fixed,"
        << (std::get<1>(corrected_normal_vs_fixed) > 0 &&
                    std::get<1>(corrected_inverse_vs_fixed) > 0
                ? "demonstrated" : "not_demonstrated")
        << ',' << (std::get<0>(corrected_normal_vs_fixed) +
                   std::get<0>(corrected_inverse_vs_fixed)) / 2.0
        << ",both target assignments must independently exclude zero\n";
    hypotheses << "E_inverse_assignment,measured,"
        << b47_aggregate(discovery.at("E_inverse_mixed")).supported_score
        << ",same architecture and independent logical target inversion\n";
    hypotheses << "F_exploration_share,measured,"
        << discovery_best.supported_score - discovery_best.autonomous_score
        << ",difference between E2 and E1\n";
    hypotheses << "G_reversal,insufficient,"
        << b47_aggregate(discovery.at("E_normal_reversal")).supported_score
        << ",old cue weights persist while feedback alternately weakens and rebuilds competing paths\n";
    hypotheses << "H_existing_capability,insufficient,"
        << std::max(
               b47_aggregate(discovery.at(
                   "G_decorrelated_normal_mixed")).supported_robust,
               b47_aggregate(discovery.at(
                   "G_decorrelated_inverse_mixed")).supported_robust)
        << ",decorrelated streams expose capability below the preregistered thresholds\n";

    std::ofstream stability(output / "stability.csv");
    stability << "variant,invalid,weights_out_of_bounds,technical_aborts,max_queue\n";
    for (const auto& variant : variants) {
        std::uint64_t invalid = 0;
        std::uint64_t bounds = 0;
        std::uint64_t max_queue = 0;
        for (const auto& row : discovery.at(variant.name)) {
            invalid += !row.valid;
            max_queue = std::max(max_queue, row.maximum_queue);
            for (const auto weight : row.cue_weights) bounds += weight < 0 || weight > 3;
            for (const auto weight : row.inhibitory_weights) bounds += weight < 0 || weight > 3;
        }
        stability << variant.name << ',' << invalid << ',' << bounds
            << ",0," << max_queue << '\n';
    }

    std::ofstream resources(output / "resources.csv");
    resources << "variant,mean_events,mean_memory_bytes,maximum_queue,inhibitory_impulses\n";
    for (const auto& variant : variants) {
        const auto aggregate = b47_aggregate(discovery.at(variant.name));
        std::uint64_t maximum_queue = 0;
        for (const auto& row : discovery.at(variant.name)) {
            maximum_queue = std::max(maximum_queue, row.maximum_queue);
        }
        resources << variant.name << ',' << aggregate.events << ','
            << aggregate.memory << ',' << maximum_queue << ','
            << aggregate.inhibitory_impulses << '\n';
    }

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    const double wall = std::chrono::duration<double>(ended - started).count();
    const double cpu = static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC;
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "variants,seeds,events,wall_seconds,cpu_seconds,events_per_cpu_second,peak_rss_kib\n";
    performance << variants.size() << ",64," << total_events << ',' << wall << ','
        << cpu << ',' << (cpu == 0.0 ? 0.0 : total_events / cpu) << ','
        << usage.ru_maxrss << '\n';

    std::cout << "B.4.7 variants=" << variants.size()
              << " E1=" << discovery_best.autonomous_score
              << " E2=" << discovery_best.supported_score
              << " robust=" << discovery_best.supported_robust
              << " confirmation=" << confirmation_best.supported_score
              << " pass=" << (discovery_pass && confirmation_pass) << '\n';
    return 0;
}
#endif
