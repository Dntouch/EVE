#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B510_EMBEDDED
#include "research_stage_b510.cpp"
#undef EVE_RESEARCH_STAGE_B510_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "eve/research/b511_episode_evaluation.hpp"

#include <sys/resource.h>

#include <array>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace {

constexpr std::uint32_t kB511Seeds = 64;
constexpr std::uint32_t kB511PhaseEpisodes = 64;
constexpr std::uint32_t kB511EpisodeSpan = 16;
constexpr std::uint32_t kB511ElementGap = 4;
constexpr std::uint32_t kB511Contexts = 6;

enum class B511Task : std::uint8_t { a = 0, b = 1, c = 2 };

struct B511MembraneVariant {
    std::string name;
    eve::RecoveryMode recovery{eve::RecoveryMode::input_dependent};
    std::uint32_t refractory{};
    std::uint64_t recovery_delay{16};
    std::uint32_t relaxation_percent{};
    bool s4{};
    bool plasticity{true};
    std::uint32_t neuron_count{32};
};

struct B511Circuit {
    B47Circuit base;
    // External names only: P,Q,R,S,T,U,V.  No task identity is stored here.
    std::array<eve::NeuronId, 7> elements{};
    std::array<eve::SynapseId, 14> element_synapses{};
};

struct B511Pattern {
    std::uint8_t first{};
    std::uint8_t second{};
    bool target_a{};
};

struct B511Episode {
    bool early_a{};
    bool early_b{};
    bool response_a{};
    bool response_b{};
    bool exploration_a{};
    bool exploration_b{};
    bool final_a{};
    bool final_b{};
    bool explored{};
    bool correct{};
    std::uint8_t exploration{};
    std::int8_t modulation{};
    std::uint64_t positive_writes{};
    std::uint64_t negative_writes{};
    std::set<eve::NeuronId> active_neurons;
    std::set<eve::SynapseId> active_synapses;
};

struct B511Probe {
    bool early_a{};
    bool early_b{};
    bool response_a{};
    bool response_b{};
    bool correct{};
    std::set<eve::NeuronId> active_neurons;
    std::set<eve::SynapseId> active_synapses;
};

struct B511Evaluation {
    std::array<std::array<B511Probe, kB511Contexts>, 3> probes;
    std::array<double, 3> conditional{};
    std::array<bool, 3> direct{};
    std::uint32_t repertoire{};
};

struct B511Run {
    std::string phase;
    std::string condition;
    std::string membrane;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    eve::bios::Runtime runtime;
    B511Circuit circuit;
    B511Evaluation before;
    B511Evaluation after_first_block;
    B511Evaluation final;
    std::set<eve::NeuronId> training_neurons;
    std::set<eve::SynapseId> training_synapses;
    std::uint32_t first_a{kB51Never};
    std::uint32_t first_b{kB51Never};
    std::uint32_t reacquired_a{kB51Never};
    std::uint64_t experiences{};
    std::uint64_t positive_writes{};
    std::uint64_t negative_writes{};
    std::uint64_t changed_weights{};
    std::uint64_t start_events{};
    std::uint64_t end_events{};
    std::size_t initial_synapses{};
    bool valid{};

    B511Run(std::string phase_value, std::string condition_value,
            std::string membrane_value, const std::uint32_t family_value,
            const bool inverse_value, const std::uint64_t seed_value,
            eve::bios::Runtime runtime_value, const B511Circuit& circuit_value)
        : phase(std::move(phase_value)),
          condition(std::move(condition_value)),
          membrane(std::move(membrane_value)),
          family(family_value), inverse(inverse_value), seed(seed_value),
          runtime(std::move(runtime_value)), circuit(circuit_value) {}
};

struct B511Summary {
    std::string phase;
    std::string condition;
    std::string membrane;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    double a{};
    double b{};
    double c{};
    double temporal{};
    double retention{};
    double reacquisition{};
    double transfer{};
    double reuse{};
    double efficiency{};
    std::uint32_t repertoire{};
    bool valid{};
};

std::string b511_task_name(const B511Task task) {
    if (task == B511Task::a) return "A";
    if (task == B511Task::b) return "B";
    return "C";
}

std::string b511_assignment(const bool inverse) {
    return inverse ? "inverse" : "normal";
}

std::string b511_response(const bool a, const bool b) {
    if (a && b) return "double";
    if (a) return "motor_A";
    if (b) return "motor_B";
    return "missing";
}

bool b511_correct(const bool target_a, const bool a, const bool b) {
    return target_a ? a && !b : b && !a;
}

eve::ExperimentConfig b511_config(
    const B511MembraneVariant& membrane, const bool plasticity) {
    B47Variant base;
    base.name = "B511";
    base.family = "B511";
    base.task = B46Task::mixed;
    base.inhibition_effect = true;
    base.inhibitory_plasticity = true;
    base.inhibitory_rule = eve::InhibitoryPlasticityMode::shared;
    base.plasticity = plasticity;
    auto config = b47_config(base);
    config.neuron_count = membrane.neuron_count;
    config.width = membrane.neuron_count == 32 ? 4 : 8;
    config.height = 4;
    config.depth = membrane.neuron_count / (config.width * config.height);
    config.reset_mode = eve::ResetMode::to_zero;
    config.refractory_input_events = membrane.refractory;
    config.recovery_mode = membrane.recovery;
    config.recovery_delay = membrane.recovery_delay;
    config.membrane_relaxation_percent = membrane.relaxation_percent;
    config.subthreshold_relaxation_enabled = membrane.s4;
    config.subthreshold_relaxation_delay = membrane.s4 ? 16 : 0;
    if (!plasticity) {
        config.plasticity_trace_mode = eve::PlasticityTraceMode::disabled;
        config.modulation_mode = eve::ModulationMode::none;
    }
    return config;
}

B511Circuit b511_circuit(eve::bios::Runtime& runtime, const bool plasticity) {
    B47Variant base;
    base.name = "B511";
    base.family = "B511";
    base.task = B46Task::mixed;
    base.inhibition_effect = true;
    base.inhibitory_plasticity = true;
    base.inhibitory_rule = eve::InhibitoryPlasticityMode::shared;
    base.plasticity = plasticity;
    B511Circuit result;
    result.base = b47_circuit(runtime, base);
    result.elements = {result.base.sensors[0], result.base.sensors[1],
                       8, 9, 15, 16, 18};
    result.element_synapses[0] = result.base.cue_synapses[0];
    result.element_synapses[1] = result.base.cue_synapses[1];
    result.element_synapses[2] = result.base.cue_synapses[2];
    result.element_synapses[3] = result.base.cue_synapses[3];
    for (std::size_t element = 2; element < result.elements.size(); ++element) {
        for (std::size_t action = 0; action < 2; ++action) {
            result.element_synapses[element * 2 + action] =
                runtime.add_experimental_synapse(
                    result.elements[element], result.base.decisions[action],
                    0, false, eve::SynapticEffect::excitatory);
        }
    }
    return result;
}

B511Pattern b511_pattern(
    const B511Task task, const bool independent, const bool inverse) {
    B511Pattern result;
    if (!independent) {
        if (task == B511Task::a) result = {0, 1, true};       // P -> Q
        else if (task == B511Task::b) result = {0, 2, false}; // P -> R
        else result = {1, 2, true};                           // Q -> R
    } else {
        if (task == B511Task::a) result = {3, 4, true};       // S -> T
        else if (task == B511Task::b) result = {5, 6, false}; // U -> V
        else result = {4, 6, true};                           // control transfer
    }
    result.target_a = result.target_a != inverse;
    return result;
}

B511Episode b511_episode(
    eve::bios::Runtime& runtime, const B511Circuit& circuit,
    const B511Pattern pattern, const std::uint8_t exploration,
    const bool learn, const bool allow_exploration = true,
    const std::uint32_t gap = kB511ElementGap) {
    B511Episode result;
    result.exploration = exploration;
    const auto before = runtime.metrics();
    std::uint8_t phase = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        result.active_neurons.insert(event.neuron);
        if (event.neuron == circuit.base.motors[0]) {
            if (phase == 0) result.early_a = true;
            else if (phase == 1) result.response_a = true;
            else result.exploration_a = true;
            result.final_a = true;
        }
        if (event.neuron == circuit.base.motors[1]) {
            if (phase == 0) result.early_b = true;
            else if (phase == 1) result.response_b = true;
            else result.exploration_b = true;
            result.final_b = true;
        }
    });
    runtime.set_impulse_observer([&](const eve::bios::ImpulseObservation& event) {
        result.active_synapses.insert(event.synapse);
    });
    const auto first_time = runtime.simulated_time();
    runtime.enqueue_fire(circuit.elements[pattern.first]);
    static_cast<void>(runtime.run());
    static_cast<void>(runtime.advance_time_to(first_time + gap));
    phase = 1;
    runtime.enqueue_fire(circuit.elements[pattern.second]);
    static_cast<void>(runtime.run());
    result.explored = allow_exploration &&
        !result.response_a && !result.response_b;
    if (result.explored) {
        phase = 2;
        runtime.enqueue_fire(circuit.base.explorers[exploration]);
        static_cast<void>(runtime.run());
    }
    const eve::research::B511MotorActivity activity{
        result.early_a, result.early_b, result.response_a, result.response_b,
        result.explored, result.exploration_a, result.exploration_b};
    result.correct = eve::research::b511_episode_success(
        pattern.target_a, activity);
    result.modulation = eve::research::b511_episode_modulation(
        pattern.target_a, activity);
    if (learn) {
        static_cast<void>(runtime.process_modulatory_feedback(result.modulation));
    }
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    const auto after = runtime.metrics();
    result.positive_writes = after.plasticity_positive_changes -
        before.plasticity_positive_changes;
    result.negative_writes = after.plasticity_negative_changes -
        before.plasticity_negative_changes;
    const auto end = first_time + kB511EpisodeSpan;
    if (runtime.simulated_time() < end)
        static_cast<void>(runtime.advance_time_to(end));
    return result;
}

B511Probe b511_probe(
    eve::bios::Runtime& runtime, const B511Circuit& circuit,
    const B511Pattern pattern, const std::uint32_t gap = kB511ElementGap) {
    const auto row = b511_episode(
        runtime, circuit, pattern, 0, false, false, gap);
    return B511Probe{row.early_a, row.early_b, row.response_a,
                     row.response_b,
                     !row.early_a && !row.early_b &&
                         b511_correct(pattern.target_a,
                                      row.response_a, row.response_b),
                     row.active_neurons, row.active_synapses};
}

const std::array<const char*, kB511Contexts> kB511ContextNames{{
    "direct", "after_A", "after_B", "short_idle",
    "long_idle", "after_mixed"}};

B511Probe b511_context_probe(
    const eve::bios::Runtime& source, const B511Circuit& circuit,
    const B511Task task, const std::uint32_t context,
    const bool independent, const bool inverse) {
    auto runtime = source;
    const auto target = b511_pattern(task, independent, inverse);
    if (context == 1)
        static_cast<void>(b511_probe(runtime, circuit,
            b511_pattern(B511Task::a, independent, inverse)));
    else if (context == 2)
        static_cast<void>(b511_probe(runtime, circuit,
            b511_pattern(B511Task::b, independent, inverse)));
    else if (context == 3)
        static_cast<void>(runtime.advance_time_by(4));
    else if (context == 4)
        static_cast<void>(runtime.advance_time_by(64));
    else if (context == 5) {
        static_cast<void>(b511_probe(runtime, circuit,
            b511_pattern(B511Task::a, independent, inverse)));
        static_cast<void>(b511_probe(runtime, circuit,
            b511_pattern(B511Task::b, independent, inverse)));
    }
    return b511_probe(runtime, circuit, target);
}

B511Evaluation b511_evaluate(
    const eve::bios::Runtime& runtime, const B511Circuit& circuit,
    const bool independent, const bool inverse) {
    B511Evaluation result;
    for (std::uint32_t task = 0; task < 3; ++task) {
        std::uint32_t correct = 0;
        for (std::uint32_t context = 0; context < kB511Contexts; ++context) {
            result.probes[task][context] = b511_context_probe(
                runtime, circuit, static_cast<B511Task>(task), context,
                independent, inverse);
            correct += result.probes[task][context].correct;
        }
        result.conditional[task] =
            static_cast<double>(correct) / kB511Contexts;
        result.direct[task] = result.probes[task][0].correct;
        if (task < 2)
            result.repertoire += result.conditional[task] >= .5;
    }
    return result;
}

std::vector<B511Task> b511_plan(
    const std::string& condition, const std::uint64_t seed,
    const std::uint32_t family) {
    std::vector<B511Task> plan;
    const auto append = [&](const B511Task task, const std::uint32_t count) {
        plan.insert(plan.end(), count, task);
    };
    if (condition == "A_only") append(B511Task::a, 192);
    else if (condition == "B_only") append(B511Task::b, 192);
    else if (condition == "A_B" || condition == "independent_A_B") {
        append(B511Task::a, 96); append(B511Task::b, 96);
    } else if (condition == "B_A") {
        append(B511Task::b, 96); append(B511Task::a, 96);
    } else if (condition == "A_B_A") {
        append(B511Task::a, 64); append(B511Task::b, 64);
        append(B511Task::a, 64);
    } else if (condition == "A_B_mixed" ||
               condition == "diagnostic_frozen" ||
               condition == "reactivated_plasticity") {
        append(B511Task::a, 64); append(B511Task::b, 64);
        for (std::uint32_t i = 0; i < 64; ++i)
            plan.push_back((i & 1U) ? B511Task::b : B511Task::a);
    } else if (condition == "alternating") {
        for (std::uint32_t i = 0; i < 192; ++i)
            plan.push_back((i & 1U) ? B511Task::b : B511Task::a);
    } else if (condition == "block_8") {
        for (std::uint32_t i = 0; i < 192; ++i)
            plan.push_back(((i / 8U) & 1U) ? B511Task::b : B511Task::a);
    } else {
        // Exactly 96 A and 96 B, then deterministically shuffle.
        append(B511Task::a, 96); append(B511Task::b, 96);
        eve::bios::DeterministicRng rng(b47_mix_seed(
            0x5111000ULL ^ seed ^ (static_cast<std::uint64_t>(family) << 32U)));
        for (std::size_t i = plan.size(); i > 1; --i)
            std::swap(plan[i - 1], plan[rng.bounded(i)]);
    }
    return plan;
}

B511Run b511_train(
    const std::string& phase, const std::string& condition,
    const B511MembraneVariant& membrane, const std::uint32_t family,
    const bool inverse, const std::uint64_t seed) {
    const bool independent = condition == "independent_A_B";
    const bool plasticity = membrane.plasticity;
    eve::bios::Runtime runtime(
        b511_config(membrane, plasticity),
        b47_mix_seed(0x5112000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(runtime, plasticity);
    B511Run result{phase, condition, membrane.name, family, inverse, seed,
                   std::move(runtime), circuit};
    if (condition == "diagnostic_frozen" ||
        condition == "reactivated_plasticity") {
        result.runtime.set_experimental_plasticity_writes_suppressed(true);
    }
    result.initial_synapses = result.runtime.network().synapse_count();
    result.start_events = result.runtime.metrics().processed_events;
    result.before = b511_evaluate(
        result.runtime, result.circuit, independent, inverse);
    const auto plan = b511_plan(condition, seed, family);
    const auto explorations = b48_plans(
        seed, plan.size(), family, 511, 512, inverse);
    bool seen_b = false;
    bool captured_first_block = false;
    for (std::uint32_t episode = 0; episode < plan.size(); ++episode) {
        if (condition == "reactivated_plasticity" && episode == 128)
            result.runtime.set_experimental_plasticity_writes_suppressed(false);
        if (!captured_first_block && episode > 0 && plan[episode] != plan[0]) {
            result.after_first_block = b511_evaluate(
                result.runtime, result.circuit, independent, inverse);
            captured_first_block = true;
        }
        const auto task = plan[episode];
        seen_b = seen_b || task == B511Task::b;
        const auto row = b511_episode(
            result.runtime, result.circuit,
            b511_pattern(task, independent, inverse),
            explorations[episode].exploration, plasticity);
        result.training_neurons.insert(
            row.active_neurons.begin(), row.active_neurons.end());
        result.training_synapses.insert(
            row.active_synapses.begin(), row.active_synapses.end());
        result.positive_writes += row.positive_writes;
        result.negative_writes += row.negative_writes;
        const bool autonomous_correct = b511_correct(
            b511_pattern(task, independent, inverse).target_a,
            row.response_a, row.response_b);
        if (task == B511Task::a && autonomous_correct) {
            if (result.first_a == kB51Never) result.first_a = episode;
            if (seen_b && result.reacquired_a == kB51Never)
                result.reacquired_a = episode;
        }
        if (task == B511Task::b && autonomous_correct &&
            result.first_b == kB51Never) result.first_b = episode;
        ++result.experiences;
    }
    if (!captured_first_block)
        result.after_first_block = b511_evaluate(
            result.runtime, result.circuit, independent, inverse);
    result.final = b511_evaluate(
        result.runtime, result.circuit, independent, inverse);
    for (const auto& synapse : result.runtime.network().synapses())
        result.changed_weights += synapse.weight != 0 && !synapse.plasticity_protected;
    result.end_events = result.runtime.metrics().processed_events;
    result.valid = result.runtime.network().validate() &&
        result.runtime.network().synapse_count() == result.initial_synapses;
    return result;
}

double b511_reacquisition_value(const B511Run& run) {
    if (run.condition != "A_B_A" || run.reacquired_a == kB51Never)
        return -1.0;
    return static_cast<double>(run.reacquired_a - 128U + 1U);
}

std::size_t b511_overlap(
    const std::set<eve::SynapseId>& left,
    const std::set<eve::SynapseId>& right) {
    std::vector<eve::SynapseId> intersection;
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                          std::back_inserter(intersection));
    return intersection.size();
}

void b511_emit_run(
    const B511Run& run, std::ofstream& networks, std::ofstream& matrix,
    std::ofstream& dimensions, std::ofstream& components,
    std::vector<B511Summary>& summaries) {
    const bool independent = run.condition == "independent_A_B";
    const auto& a = run.final.probes[0][0];
    const auto& b = run.final.probes[1][0];
    const auto& c = run.final.probes[2][0];
    const auto overlap_ab = b511_overlap(a.active_synapses, b.active_synapses);
    const auto overlap_ac = b511_overlap(a.active_synapses, c.active_synapses);
    const auto overlap_bc = b511_overlap(b.active_synapses, c.active_synapses);
    const double temporal = static_cast<double>(
        (!a.early_a && !a.early_b) + (!b.early_a && !b.early_b) +
        (!c.early_a && !c.early_b)) / 3.0;
    const double retention = run.final.conditional[0];
    const double efficiency = run.runtime.network().synapse_count() == 0 ? 0.0 :
        static_cast<double>(run.final.repertoire) /
            static_cast<double>(run.runtime.network().synapse_count());
    networks << run.phase << ',' << run.condition << ',' << run.membrane << ','
        << run.family << ',' << b511_assignment(run.inverse) << ',' << run.seed
        << ',' << run.final.conditional[0] << ',' << run.final.conditional[1]
        << ',' << run.final.conditional[2] << ',' << run.final.repertoire << ','
        << run.first_a << ',' << run.first_b << ',' << run.reacquired_a << ','
        << run.positive_writes << ',' << run.negative_writes << ','
        << run.changed_weights << ',' << run.training_neurons.size() << ','
        << run.training_synapses.size() << ','
        << run.runtime.config().neuron_count << ','
        << run.runtime.network().synapse_count() << ','
        << run.end_events - run.start_events << ','
        << run.runtime.metrics().maximum_queue_size << ','
        << run.runtime.network().estimated_memory_bytes() << ',' << run.valid
        << '\n';
    for (std::uint32_t task = 0; task < 3; ++task) {
        const auto pattern = b511_pattern(
            static_cast<B511Task>(task), independent, run.inverse);
        for (std::uint32_t context = 0; context < kB511Contexts; ++context) {
            const auto& probe = run.final.probes[task][context];
            matrix << run.phase << ',' << run.condition << ',' << run.membrane
                << ',' << run.family << ',' << b511_assignment(run.inverse)
                << ',' << run.seed << ','
                << b511_task_name(static_cast<B511Task>(task)) << ','
                << kB511ContextNames[context] << ','
                << (pattern.target_a ? "motor_A" : "motor_B") << ','
                << b511_response(probe.response_a, probe.response_b) << ','
                << probe.correct << ',' << probe.early_a << ',' << probe.early_b
                << '\n';
        }
    }
    const double reuse = static_cast<double>(overlap_ab);
    dimensions << run.phase << ',' << run.condition << ',' << run.membrane
        << ',' << run.family << ',' << b511_assignment(run.inverse) << ','
        << run.seed << ','
        << (static_cast<double>(run.final.direct[0] + run.final.direct[1]) / 2.0)
        << ',' << ((run.final.conditional[0] + run.final.conditional[1]) / 2.0)
        << ',' << temporal << ',' << run.final.repertoire << ','
        << static_cast<int>(run.final.repertoire) -
               static_cast<int>(run.before.repertoire)
        << ',' << retention << ',' << b511_reacquisition_value(run)
        << ",-1," << reuse << ',' << efficiency << '\n';
    components << run.phase << ',' << run.condition << ',' << run.membrane
        << ',' << run.family << ',' << b511_assignment(run.inverse) << ','
        << run.seed << ',' << a.active_neurons.size() << ','
        << b.active_neurons.size() << ',' << c.active_neurons.size() << ','
        << a.active_synapses.size() << ',' << b.active_synapses.size() << ','
        << c.active_synapses.size() << ',' << overlap_ab << ',' << overlap_ac
        << ',' << overlap_bc << '\n';
    summaries.push_back(B511Summary{
        run.phase, run.condition, run.membrane, run.family, run.inverse,
        run.seed, run.final.conditional[0], run.final.conditional[1],
        run.final.conditional[2], temporal, retention,
        b511_reacquisition_value(run), -1.0, reuse, efficiency,
        run.final.repertoire, run.valid});
}

void b511_ablation(
    const B511Run& run, std::ofstream& out) {
    const auto& a = run.final.probes[0][0].active_synapses;
    const auto& b = run.final.probes[1][0].active_synapses;
    std::vector<eve::SynapseId> candidates;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(candidates));
    std::vector<eve::SynapseId> functional;
    for (const auto id : candidates) {
        const auto& synapse = run.runtime.network().synapse(id);
        if (synapse.weight == 0) continue;
        const bool bottleneck = synapse.plasticity_protected;
        auto copy = run.runtime;
        copy.set_experimental_synapse_weight(id, 0);
        const auto pa = b511_probe(copy, run.circuit,
            b511_pattern(B511Task::a, false, run.inverse));
        const auto pb = b511_probe(copy, run.circuit,
            b511_pattern(B511Task::b, false, run.inverse));
        const bool causal_a = run.final.direct[0] && !pa.correct;
        const bool causal_b = run.final.direct[1] && !pb.correct;
        if (causal_a || causal_b) functional.push_back(id);
        out << run.phase << ',' << run.family << ','
            << b511_assignment(run.inverse) << ',' << run.seed
            << ",single," << id << ',' << synapse.sender << ','
            << synapse.receiver << ',' << synapse.weight << ',' << bottleneck
            << ',' << causal_a << ',' << causal_b << ','
            << (causal_a && causal_b) << ',' << copy.network().validate()
            << '\n';
    }
    auto combined = run.runtime;
    for (const auto id : functional)
        combined.set_experimental_synapse_weight(id, 0);
    const auto pa = b511_probe(combined, run.circuit,
        b511_pattern(B511Task::a, false, run.inverse));
    const auto pb = b511_probe(combined, run.circuit,
        b511_pattern(B511Task::b, false, run.inverse));
    out << run.phase << ',' << run.family << ','
        << b511_assignment(run.inverse) << ',' << run.seed
        << ",combined,-1,-1,-1," << functional.size()
        << ",0," << (run.final.direct[0] && !pa.correct) << ','
        << (run.final.direct[1] && !pb.correct) << ','
        << (run.final.direct[0] && run.final.direct[1] &&
            !pa.correct && !pb.correct) << ','
        << combined.network().validate() << '\n';
}

void b511_population(
    const std::string& phase, const std::vector<std::string>& conditions,
    const B511MembraneVariant& membrane, const std::uint64_t seed_base,
    std::ofstream& networks, std::ofstream& matrix,
    std::ofstream& dimensions, std::ofstream& components,
    std::ofstream& ablations, std::vector<B511Summary>& summaries,
    std::optional<B511Run>& persistence_sample) {
    for (const auto& condition : conditions) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint32_t offset = 0; offset < kB511Seeds; ++offset) {
                    auto run = b511_train(phase, condition, membrane, family,
                                          inverse, seed_base + offset);
                    if (!persistence_sample && condition == "A_B_mixed")
                        persistence_sample = run;
                    if (condition == "A_B_mixed" && offset < 2)
                        b511_ablation(run, ablations);
                    b511_emit_run(run, networks, matrix, dimensions,
                                  components, summaries);
                }
            }
        }
    }
}

struct B511Transfer {
    std::string phase;
    std::string pretraining;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    std::uint32_t first_c{kB51Never};
    double final_c{};
    std::uint64_t writes{};
    std::size_t shared_synapses{};
    bool valid{};
};

B511Transfer b511_transfer(
    const std::string& phase, const std::string& pretraining,
    const B511MembraneVariant& membrane, const std::uint32_t family,
    const bool inverse, const std::uint64_t seed) {
    eve::bios::Runtime runtime(b511_config(membrane, true),
        b47_mix_seed(0x5113000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(runtime, true);
    std::vector<B511Task> prefix;
    if (pretraining == "after_A") prefix.assign(128, B511Task::a);
    else if (pretraining == "after_B") prefix.assign(128, B511Task::b);
    else if (pretraining == "after_A_B") {
        prefix.assign(64, B511Task::a);
        prefix.insert(prefix.end(), 64, B511Task::b);
    } else if (pretraining == "independent_control") {
        for (std::uint32_t i = 0; i < 128; ++i)
            prefix.push_back((i & 1U) ? B511Task::b : B511Task::a);
    } else {
        static_cast<void>(runtime.advance_time_by(128 * kB511EpisodeSpan));
    }
    const bool independent = pretraining == "independent_control";
    const auto plans = b48_plans(seed, prefix.size() + 64, family, 513, 514);
    std::set<eve::SynapseId> prefix_synapses;
    std::set<eve::SynapseId> c_synapses;
    std::uint64_t writes = 0;
    for (std::uint32_t index = 0; index < prefix.size(); ++index) {
        const auto row = b511_episode(runtime, circuit,
            b511_pattern(prefix[index], independent, inverse),
            plans[index].exploration, true);
        prefix_synapses.insert(row.active_synapses.begin(), row.active_synapses.end());
        writes += row.positive_writes + row.negative_writes;
    }
    B511Transfer result{phase, pretraining, family, inverse, seed};
    for (std::uint32_t index = 0; index < 64; ++index) {
        const auto row = b511_episode(runtime, circuit,
            b511_pattern(B511Task::c, false, inverse),
            plans[prefix.size() + index].exploration, true);
        c_synapses.insert(row.active_synapses.begin(), row.active_synapses.end());
        writes += row.positive_writes + row.negative_writes;
        if (result.first_c == kB51Never && b511_correct(
                b511_pattern(B511Task::c, false, inverse).target_a,
                row.response_a, row.response_b)) result.first_c = index;
    }
    result.final_c = b511_evaluate(runtime, circuit, false, inverse).conditional[2];
    result.writes = writes;
    result.shared_synapses = b511_overlap(prefix_synapses, c_synapses);
    result.valid = runtime.network().validate();
    return result;
}

void b511_transfer_population(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint64_t seed_base, std::ofstream& out,
    std::vector<B511Summary>& summaries) {
    const std::array<std::string, 5> groups{{
        "no_pretraining", "after_A", "after_B", "after_A_B",
        "independent_control"}};
    for (const auto& group : groups) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint32_t offset = 0; offset < kB511Seeds; ++offset) {
                    const auto row = b511_transfer(
                        phase, group, membrane, family, inverse,
                        seed_base + offset);
                    out << row.phase << ',' << row.pretraining << ','
                        << row.family << ',' << b511_assignment(row.inverse)
                        << ',' << row.seed << ',' << row.first_c << ','
                        << row.final_c << ',' << row.writes << ','
                        << row.shared_synapses << ',' << row.valid << '\n';
                    summaries.push_back(B511Summary{
                        row.phase, "transfer_" + row.pretraining,
                        membrane.name, row.family, row.inverse, row.seed,
                        0, 0, row.final_c, 0, 0, -1,
                        row.first_c == kB51Never ? -1.0 :
                            static_cast<double>(row.first_c),
                        static_cast<double>(row.shared_synapses), 0, 0,
                        row.valid});
                }
            }
        }
    }
}

void b511_reorganization_population(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint64_t seed_base, std::ofstream& out) {
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (std::uint32_t offset = 0; offset < 5; ++offset) {
                const auto seed = seed_base + offset;
                eve::bios::Runtime runtime(b511_config(membrane, true),
                    b47_mix_seed(0x5114000ULL ^ seed ^
                        (static_cast<std::uint64_t>(family) << 40U)));
                const auto circuit = b511_circuit(runtime, true);
                const auto plans = b48_plans(seed, 256, family, 515, 516);
                for (std::uint32_t episode = 0; episode < 64; ++episode) {
                    static_cast<void>(b511_episode(
                        runtime, circuit,
                        b511_pattern(B511Task::a, false, inverse),
                        plans[episode].exploration, true));
                }
                bool found = false;
                for (std::uint32_t episode = 64; episode < 128 && !found;
                     ++episode) {
                    const auto before = runtime;
                    const auto before_a = b511_context_probe(
                        before, circuit, B511Task::a, 0, false, inverse).correct;
                    std::vector<eve::bios::PlasticityObservation> writes;
                    runtime.set_plasticity_observer(
                        [&](const eve::bios::PlasticityObservation& row) {
                            if (row.changed) writes.push_back(row);
                        });
                    static_cast<void>(b511_episode(
                        runtime, circuit,
                        b511_pattern(B511Task::b, false, inverse),
                        plans[episode].exploration, true));
                    runtime.set_plasticity_observer({});
                    const auto after_a = b511_context_probe(
                        runtime, circuit, B511Task::a, 0, false, inverse).correct;
                    if (writes.empty() || before_a == after_a) continue;

                    const auto selected = writes.front();
                    auto w1 = before;
                    bool suppressed = false;
                    w1.set_experimental_plasticity_write_interceptor(
                        [&](const eve::bios::PlasticityObservation& row) {
                            const bool match = !suppressed &&
                                row.event_index == selected.event_index &&
                                row.candidate == selected.candidate &&
                                row.weight_before == selected.weight_before &&
                                row.proposed_weight == selected.proposed_weight;
                            suppressed = suppressed || match;
                            return match;
                        });
                    static_cast<void>(b511_episode(
                        w1, circuit,
                        b511_pattern(B511Task::b, false, inverse),
                        plans[episode].exploration, true));
                    w1.set_experimental_plasticity_write_interceptor({});
                    auto w2 = before;
                    static_cast<void>(b511_episode(
                        w2, circuit,
                        b511_pattern(B511Task::b, false, inverse),
                        plans[episode].exploration, true));
                    const bool identical_control =
                        runtime.state_fingerprint() == w2.state_fingerprint();
                    const auto isolated_a = b511_context_probe(
                        w1, circuit, B511Task::a, 0, false, inverse).correct;
                    const auto immediate_b = b511_context_probe(
                        runtime, circuit, B511Task::b, 0, false, inverse).correct;
                    const auto isolated_b = b511_context_probe(
                        w1, circuit, B511Task::b, 0, false, inverse).correct;
                    for (std::uint32_t future = 0; future < 64; ++future) {
                        const auto task = (future & 1U)
                            ? B511Task::b : B511Task::a;
                        const auto plan_index = 128 + future;
                        for (auto* state : {&runtime, &w1, &w2}) {
                            static_cast<void>(b511_episode(
                                *state, circuit,
                                b511_pattern(task, false, inverse),
                                plans[plan_index].exploration, true));
                        }
                    }
                    const auto final0 = b511_evaluate(
                        runtime, circuit, false, inverse);
                    const auto final1 = b511_evaluate(w1, circuit, false, inverse);
                    const auto final2 = b511_evaluate(w2, circuit, false, inverse);
                    const bool positive_weight_change =
                        selected.proposed_weight > selected.weight_before;
                    const bool negative_weight_change =
                        selected.proposed_weight < selected.weight_before;
                    const bool initial_impairment = before_a && !after_a;
                    const bool w1_prevented_impairment =
                        initial_impairment && isolated_a;
                    const bool coincident_impairment =
                        initial_impairment && !isolated_a;
                    const bool productive =
                        final0.repertoire > final1.repertoire;
                    const bool harmful =
                        final0.repertoire < final1.repertoire;
                    const bool tied =
                        final0.repertoire == final1.repertoire;
                    const char* reorganization_class = productive
                        ? "productive" : harmful ? "harmful" : "tied";
                    out << phase << ',' << family << ','
                        << b511_assignment(inverse) << ',' << seed << ','
                        << episode << ',' << selected.event_index << ','
                        << selected.candidate << ',' << selected.weight_before
                        << ',' << selected.proposed_weight << ',' << suppressed
                        << ',' << identical_control << ',' << before_a << ','
                        << after_a << ',' << isolated_a << ',' << immediate_b
                        << ',' << isolated_b << ',' << final0.repertoire << ','
                        << final1.repertoire << ',' << final2.repertoire << ','
                        << positive_weight_change << ','
                        << negative_weight_change << ','
                        << initial_impairment << ','
                        << w1_prevented_impairment << ','
                        << coincident_impairment << ','
                        << reorganization_class << ','
                        << productive << ',' << harmful << ',' << tied << ','
                        << runtime.network().validate() << ','
                        << w1.network().validate() << ','
                        << w2.network().validate() << '\n';
                    found = true;
                }
                if (!found) {
                    out << phase << ',' << family << ','
                        << b511_assignment(inverse) << ',' << seed
                        << ",-1,-1,-1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,"
                           "0,0,0,not_found,0,0,0,1,1,1\n";
                }
            }
        }
    }
}

void b511_statistics(
    const std::vector<B511Summary>& rows, std::ofstream& aggregate,
    std::ofstream& paired) {
    using Key = std::tuple<std::string, std::string, std::string>;
    std::map<Key, std::vector<const B511Summary*>> groups;
    for (const auto& row : rows)
        groups[{row.phase, row.condition, row.membrane}].push_back(&row);
    std::uint64_t salt = 0x5115000ULL;
    for (const auto& [key, group] : groups) {
        std::array<std::pair<const char*, std::vector<double>>, 10> metrics{{
            {"V1_A_current", {}}, {"V2_B_conditional", {}},
            {"V3_temporal", {}}, {"V4_repertoire", {}},
            {"V6_retention", {}}, {"V7_reacquisition", {}},
            {"V8_transfer", {}}, {"V9_active_overlap", {}},
            {"V10_efficiency", {}}, {"valid", {}}}};
        for (const auto* row : group) {
            const std::array<double, 10> values{{
                row->a, row->b, row->temporal,
                static_cast<double>(row->repertoire), row->retention,
                row->reacquisition, row->transfer, row->reuse,
                row->efficiency, row->valid ? 1.0 : 0.0}};
            for (std::size_t i = 0; i < metrics.size(); ++i)
                if (values[i] >= 0.0) metrics[i].second.push_back(values[i]);
        }
        for (const auto& metric : metrics) {
            if (metric.second.empty()) continue;
            const auto interval = bootstrap_interval(metric.second, ++salt);
            aggregate << std::get<0>(key) << ',' << std::get<1>(key) << ','
                << std::get<2>(key) << ',' << metric.first << ','
                << metric.second.size() << ',' << b48_mean(metric.second)
                << ',' << interval.first << ',' << interval.second << '\n';
        }
    }
    using PairKey = std::tuple<std::string, std::uint32_t, bool, std::uint64_t>;
    std::map<PairKey, double> shared;
    std::map<PairKey, double> separate;
    for (const auto& row : rows) {
        const PairKey key{row.phase, row.family, row.inverse, row.seed};
        if (row.condition == "A_B") shared[key] = (row.a + row.b) / 2.0;
        if (row.condition == "independent_A_B")
            separate[key] = (row.a + row.b) / 2.0;
    }
    std::map<std::string, std::vector<double>> differences;
    for (const auto& [key, value] : shared) {
        const auto found = separate.find(key);
        if (found != separate.end())
            differences[std::get<0>(key)].push_back(value - found->second);
    }
    for (const auto& [phase, values] : differences) {
        const auto interval = bootstrap_interval(values, ++salt);
        paired << phase << ",A_B_minus_independent_A_B,seed_family_assignment,"
            << values.size() << ',' << b48_mean(values) << ','
            << interval.first << ',' << interval.second << '\n';
    }
}

void b511_persistence(
    const std::filesystem::path& output, std::ofstream& out) {
    const B511MembraneVariant s4{
        "S4_d16", eve::RecoveryMode::independent_with_membrane_relaxation,
        1, 16, 50, true, true, 32};
    eve::bios::Runtime runtime(b511_config(s4, true), 0x5116000ULL);
    const auto circuit = b511_circuit(runtime, true);
    runtime.set_experimental_synapse_weight(circuit.element_synapses[0], 1);
    runtime.enqueue_fire(circuit.elements[0]);
    static_cast<void>(runtime.run());
    const auto pending = runtime.future_event_count();
    const auto path = output / "pending-s4-state.bin";
    runtime.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    const bool identical_load = runtime.state_fingerprint() ==
        restored.state_fingerprint();
    static_cast<void>(runtime.advance_time_by(32));
    static_cast<void>(restored.advance_time_by(32));
    const bool identical_continuation = runtime.state_fingerprint() ==
        restored.state_fingerprint();
    out << pending << ',' << identical_load << ',' << identical_continuation
        << ',' << runtime.network().validate() << ','
        << restored.network().validate() << '\n';
    std::filesystem::remove(path);
}

void b511_historical_reproduction(const std::filesystem::path& output) {
    const B55NaturalVariant s4{"S4_event_d16", true, 16, true};
    std::ofstream networks(output / "historical-network-reproduction.csv");
    networks << "phase,variant,schedule,family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,ever_both,robust,events,maximum_queue,memory,valid\n";
    const auto b57d = b58_matrix(
        "B57_discovery_reproduction", s4, "fixed_16", 30001, networks);
    const auto b57c = b58_matrix(
        "B57_confirmation_reproduction", s4, "fixed_16", 40001, networks);
    const auto b59d = b58_matrix(
        "B59_discovery_reproduction", s4, "fixed_16", 50001, networks);
    const auto b59c = b58_matrix(
        "B59_confirmation_reproduction", s4, "fixed_16", 60001, networks);

    std::ofstream events(output / "b510-reproduction-events.csv");
    std::ofstream interventions(output / "b510-reproduction-interventions.csv");
    std::ofstream followups(output / "b510-reproduction-followups.csv");
    std::ofstream history(output / "b510-reproduction-history.csv");
    events << "phase,family,assignment,seed,episode,selection_rule,event,time,synapse,sender,receiver,effect,weight_before,proposed_weight,delta,mechanism,modulation,local_polarity,eligibility_generation,candidates,receiver_membrane,activity_count,margin,excitatory,inhibitory,prior_contributions,prior_writes,before_fingerprint\n";
    interventions << "phase,seed,episode,copy,synapse,suppressed,pair_score,fingerprint,valid_or_identical\n";
    followups << "phase,seed,episode,checkpoint,W0_pair,W1_pair,W0_E1_X,W0_E1_Y,W0_E2_X,W0_E2_Y,W1_E1_X,W1_E1_Y,W1_E2_X,W1_E2_Y,W0_valid,W1_valid\n";
    history << "phase,seed,synapse,prior_contributions,prior_writes,margin_history,contribution_availability,write_history_availability,margin_history_availability,outcome\n";
    std::vector<B510Info> information;
    std::uint64_t isolated = 0;
    std::uint64_t combined = 0;
    std::uint64_t copies = 0;
    const auto acquisition_d = b510_sources(
        "B510_discovery_acquisition", s4, 70001, false);
    const auto acquisition_c = b510_sources(
        "B510_confirmation_acquisition", s4, 80001, false);
    const auto reversal_d = b510_sources(
        "B510_discovery_reversal", s4, 70001, true);
    const auto reversal_c = b510_sources(
        "B510_confirmation_reversal", s4, 80001, true);
    b510_prospective(acquisition_d, false, events, interventions, followups,
                     history, information, isolated, combined, copies);
    b510_prospective(acquisition_c, false, events, interventions, followups,
                     history, information, isolated, combined, copies);
    b510_prospective(reversal_d, true, events, interventions, followups,
                     history, information, isolated, combined, copies);
    b510_prospective(reversal_c, true, events, interventions, followups,
                     history, information, isolated, combined, copies);

    std::ofstream reproduction(output / "historical-reproduction.csv");
    reproduction << "reference,observed,expected,status,method\n"
        << "B57_discovery," << b57d[1] << ",242,"
        << (b57d[1] == 242 ? "exact" : "mismatch")
        << ",fresh deterministic rerun\n"
        << "B57_confirmation," << b57c[1] << ",254,"
        << (b57c[1] == 254 ? "exact" : "mismatch")
        << ",fresh deterministic rerun\n"
        << "B58_B59_discovery," << b59d[1] << ",240,"
        << (b59d[1] == 240 ? "exact" : "mismatch")
        << ",fresh deterministic rerun\n"
        << "B58_B59_confirmation," << b59c[1] << ",244,"
        << (b59c[1] == 244 ? "exact" : "mismatch")
        << ",fresh deterministic rerun\n"
        << "B510_prospective_events," << isolated << ",119,"
        << (isolated == 119 ? "exact" : "mismatch")
        << ",fresh W0-W4 intervention rerun\n";
}

void b511_manifests(const std::filesystem::path& output) {
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,families,assignments,primary_conditions,episodes,selection\n"
        "discovery,90001..90064,3,2,9,192,predeclared\n"
        "confirmation,100001..100064,3,2,9,192,frozen from discovery\n"
        "transfer_discovery,90001..90064,3,2,5,64 plus controlled prefix,predeclared\n"
        "transfer_confirmation,100001..100064,3,2,5,64 plus controlled prefix,independent\n";
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,description,implemented\n"
        "K0,historical X/Y reference,1\nK1,A only,1\nK2,B only,1\n"
        "K3,A then B,1\nK4,B then A,1\nK5,A then B then A,1\n"
        "K6,A then B then mixed,1\nK7,alternating,1\nK8,random mixed,1\n"
        "K9,independent elements,1\nK10,shared elements,1\nK11,C transfer,1\n"
        "K12,equal independent prefix,1\nK13,active common paths,1\n"
        "K14,single ablation,1\nK15,combined ablation,1\n"
        "K16,natural reorganization trajectory,1\n"
        "K17,isolated write omission through B510 interceptor,1\n"
        "K18,R0,1\nK19,R2-50,1\nK20,S4-d16,1\nK21,fixed topology,1\n"
        "K22,64-neuron capacity without target wiring,1\n"
        "K23,structural plasticity,0\nK24,no learning,1\n"
        "K25,diagnostic frozen writes,1\nK26,reactivated plasticity,1\n"
        "K27,historical E1/E2,1\nK28,context and sequence matrix,1\n";
    std::ofstream tasks(output / "task-definition.csv");
    tasks << "task,first_external_element,second_external_element,target,shared_control\n"
        "A,P,Q,motor_A,shared\nB,P,R,motor_B,shared\n"
        "C,Q,R,motor_A,shared transfer\nA_control,S,T,motor_A,independent\n"
        "B_control,U,V,motor_B,independent\n";
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "candidate,authorized,decision\n"
        "new_learning_rule,0,not part of B511\n"
        "automatic_representation_labels,0,forbidden\n"
        "isolated_write_interceptor,1,diagnostic only\n"
        "structural_plasticity,0,fixed-topology limit not yet causal justification\n"
        "S4_default,0,remains experimental and disabled by default\n";
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B511_EMBEDDED
int main(int argc, char** argv) {
    const auto output = argc > 1 ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b511");
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    std::filesystem::create_directories(output);
    b511_manifests(output);
    b511_historical_reproduction(output);

    const B511MembraneVariant r0{
        "R0", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, true, 32};
    const B511MembraneVariant r2{
        "R2_p50", eve::RecoveryMode::independent_with_membrane_relaxation,
        1, 16, 50, false, true, 32};
    const B511MembraneVariant s4{
        "S4_d16", eve::RecoveryMode::independent_with_membrane_relaxation,
        1, 16, 50, true, true, 32};
    const B511MembraneVariant no_learning{
        "NoLearning", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, false, 32};
    const B511MembraneVariant expanded{
        "R0_capacity64", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, true, 64};

    std::ofstream networks(output / "network-results.csv");
    std::ofstream matrix(output / "response-matrix.csv");
    std::ofstream dimensions(output / "v1-v10-dimensions.csv");
    std::ofstream components(output / "component-overlap.csv");
    std::ofstream ablations(output / "causal-ablations.csv");
    std::ofstream transfer(output / "transfer-c.csv");
    std::ofstream reorganization(output / "productive-reorganization.csv");
    networks << "phase,condition,membrane,family,assignment,seed,A_conditional,B_conditional,C_conditional,repertoire,first_A,first_B,reacquired_A,positive_writes,negative_writes,changed_weights,active_neurons,active_synapses,neurons,synapses,events,max_queue,memory_bytes,valid\n";
    matrix << "phase,condition,membrane,family,assignment,seed,task,context,expected,actual,correct,early_A,early_B\n";
    dimensions << "phase,condition,membrane,family,assignment,seed,V1_current,V2_conditional,V3_temporal,V4_repertoire,V5_progress,V6_retention,V7_reacquisition,V8_transfer,V9_active_overlap,V10_efficiency\n";
    components << "phase,condition,membrane,family,assignment,seed,A_neurons,B_neurons,C_neurons,A_synapses,B_synapses,C_synapses,AB_overlap,AC_overlap,BC_overlap\n";
    ablations << "phase,family,assignment,seed,ablation,synapse,sender,receiver,weight_or_count,general_bottleneck,causal_A,causal_B,causal_multiple,valid\n";
    transfer << "phase,pretraining,family,assignment,seed,first_C,final_C,writes,shared_synapses,valid\n";
    reorganization << "phase,family,assignment,seed,episode,event,synapse,weight_before,weight_after,suppressed,W2_identical,A_before,A_after_natural,A_after_omission,B_after_natural,B_after_omission,W0_final_repertoire,W1_final_repertoire,W2_final_repertoire,positive_weight_change,negative_weight_change,initial_A_impairment,W1_prevented_A_impairment,coincident_A_impairment,reorganization_class,productive,harmful,tied,W0_valid,W1_valid,W2_valid\n";

    const std::vector<std::string> conditions{
        "A_only", "B_only", "A_B", "B_A", "A_B_A", "A_B_mixed",
        "alternating", "random", "block_8", "independent_A_B"};
    std::vector<B511Summary> summaries;
    std::optional<B511Run> persistence_sample;
    b511_population("discovery", conditions, r0, 90001, networks, matrix,
                    dimensions, components, ablations, summaries,
                    persistence_sample);
    b511_population("confirmation", conditions, r0, 100001, networks, matrix,
                    dimensions, components, ablations, summaries,
                    persistence_sample);
    const std::vector<std::string> variant_condition{"A_B_mixed"};
    for (const auto& variant : {r2, s4}) {
        b511_population("discovery", variant_condition, variant, 90001,
                        networks, matrix, dimensions, components, ablations,
                        summaries, persistence_sample);
        b511_population("confirmation", variant_condition, variant, 100001,
                        networks, matrix, dimensions, components, ablations,
                        summaries, persistence_sample);
    }
    b511_population("capacity_discovery", variant_condition, expanded, 90001,
                    networks, matrix, dimensions, components, ablations,
                    summaries, persistence_sample);
    const std::vector<std::string> gate_conditions{
        "diagnostic_frozen", "reactivated_plasticity"};
    b511_population("write_gate_discovery", gate_conditions, r0, 90001,
                    networks, matrix, dimensions, components, ablations,
                    summaries, persistence_sample);
    b511_population("no_learning_discovery", variant_condition, no_learning,
                    90001, networks, matrix, dimensions, components, ablations,
                    summaries, persistence_sample);
    b511_transfer_population("transfer_discovery", r0, 90001, transfer,
                             summaries);
    b511_transfer_population("transfer_confirmation", r0, 100001, transfer,
                             summaries);
    b511_reorganization_population(
        "reorganization_discovery", r0, 90001, reorganization);
    b511_reorganization_population(
        "reorganization_confirmation", r0, 100001, reorganization);

    std::ofstream aggregate(output / "aggregate-statistics.csv");
    std::ofstream paired(output / "paired-statistics.csv");
    aggregate << "phase,condition,membrane,metric,n,mean,ci_low,ci_high\n";
    paired << "phase,comparison,cluster,n,mean_difference,ci_low,ci_high\n";
    b511_statistics(summaries, aggregate, paired);

    std::ofstream persistence(output / "persistence.csv");
    persistence << "pending_events,identical_load,identical_continuation,original_valid,restored_valid\n";
    b511_persistence(output, persistence);

    const auto wall = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const auto cpu = static_cast<double>(std::clock() - cpu_started) /
        CLOCKS_PER_SEC;
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream resources(output / "resources.csv");
    resources << "metric,value\nwall_seconds," << wall
        << "\ncpu_seconds," << cpu << "\npeak_rss_kib," << usage.ru_maxrss
        << "\nruntime_bytes," << sizeof(eve::bios::Runtime)
        << "\nnetworks," << summaries.size()
        << "\nadditional_persistent_bytes_per_neuron,0\n"
        << "additional_persistent_bytes_per_synapse,0\n"
        << "production_rule_changes,0\n";
    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n"
        "shared_element_environment,1,task-definition.csv\n"
        "sequential_and_mixed_learning,1,network-results.csv.gz\n"
        "separate_V1_to_V10,1,v1-v10-dimensions.csv.gz\n"
        "complete_response_matrices,1,response-matrix.csv.gz\n"
        "causal_reuse_checks,1,causal-ablations.csv\n"
        "isolated_reorganization_writes,1,productive-reorganization.csv\n"
        "R0_R2_S4_same_tasks,1,aggregate-statistics.csv\n"
        "independent_confirmation,1,sampling-plan.csv\n"
        "pending_event_persistence,1,persistence.csv\n"
        "new_rule_not_released,1,mechanism-decision.csv\n";
    std::cout << "B.5.11 complete networks=" << summaries.size()
              << " wall=" << std::fixed << std::setprecision(2) << wall
              << "s\n";
    return 0;
}
#endif
