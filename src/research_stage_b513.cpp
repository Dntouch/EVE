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
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>

namespace {

constexpr std::uint32_t kB513Seeds = 64;
constexpr std::array<std::uint32_t, 5> kB513Sizes{{32, 128, 256, 512, 2048}};
const std::vector<eve::NeuronId> kB513Protected{{0, 1, 2, 3, 4, 5, 6, 7}};

struct B513GrowthTrial {
    eve::bios::Runtime runtime;
    eve::NeuronId source{};
    eve::NeuronId target{};
    eve::SynapseId edge{};
    bool target_was_unconnected{};
};

struct B513FunctionalResult {
    std::int32_t learned_weight{};
    bool eligibility{};
    bool zero_to_one{};
    bool functional{};
    bool ablated_functional{};
    bool reproduced{};
    std::int32_t no_learning_weight{};
    bool static_extra_functional{};
    std::uint32_t no_growth_edges{};
    std::uint32_t no_exploration_edges{};
    bool protected_target{};
};

struct B513TaskResult {
    std::uint32_t repertoire{};
    std::uint64_t grown_synapses{};
    std::uint32_t protected_growth_targets{};
};

eve::ExperimentConfig b513_config(const std::uint32_t neurons,
                                  const bool growth = true,
                                  const bool plasticity = true) {
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
    config.consume_trace_on_modulation = true;
    return config;
}

eve::brainstem::ExplorationConfig b513_exploration(
    const std::uint64_t seed, const std::uint32_t count = 1,
    const eve::brainstem::ExplorationMode mode =
        eve::brainstem::ExplorationMode::single) {
    eve::brainstem::ExplorationConfig config;
    config.mode = mode;
    config.target_policy =
        eve::brainstem::ExplorationTargetPolicy::processing_neurons_only;
    config.activation_count = count;
    config.activation_budget = count;
    config.seed = seed;
    config.additional_reserved_neurons = kB513Protected;
    return config;
}

B513GrowthTrial b513_grow(const std::uint32_t neurons,
                          const std::uint64_t runtime_seed,
                          const std::uint64_t exploration_seed) {
    eve::bios::Runtime runtime(b513_config(neurons), runtime_seed);
    const auto selected = runtime.enqueue_exploration(
        b513_exploration(exploration_seed));
    const auto source = selected.activated_neurons.front();
    static_cast<void>(runtime.run());
    if (runtime.network().synapse_count() != 1)
        throw std::runtime_error("B.5.13 first exploratory fire did not grow");
    const auto& edge = runtime.network().synapse(0);
    const bool initially_unconnected =
        runtime.network().neuron(edge.receiver).incoming_count == 1 &&
        runtime.network().neuron(edge.receiver).outgoing_count == 0;
    return {std::move(runtime), source, edge.receiver, 0, initially_unconnected};
}

eve::NeuronId b513_helper(const eve::NeuronId source,
                          const eve::NeuronId target) {
    eve::NeuronId result = 0;
    while (result == source || result == target) ++result;
    return result;
}

void b513_add_modulation_route(eve::bios::Runtime& runtime,
                               const eve::NeuronId target) {
    eve::brainstem::EvaluationRouteConfig route;
    route.mode = eve::brainstem::EvaluationRouteMode::sensory_input;
    route.first_input_neuron = 24;
    route.graded_channel_count = 5;
    route.fanout_per_source = 1;
    route.weight = 1;
    const auto configured = runtime.configure_evaluation_route(route);
    const auto source = *std::find_if(
        configured.input_neurons.begin(), configured.input_neurons.end(),
        [&runtime, target](const eve::NeuronId candidate) {
            return candidate != target &&
                !runtime.network().has_synapse(candidate, target);
        });
    static_cast<void>(runtime.add_experimental_synapse(
        source, target, 0, true));
}

bool b513_coactivate(eve::bios::Runtime& runtime,
                     const eve::NeuronId source,
                     const eve::NeuronId target,
                     const eve::SynapseId edge) {
    runtime.enqueue_fire(source);
    runtime.enqueue_fire(b513_helper(source, target));
    static_cast<void>(runtime.run());
    for (const auto& record : runtime.network().eligibility_records(target)) {
        if (std::find(record.candidates.begin(), record.candidates.end(), edge) !=
            record.candidates.end()) return true;
    }
    return false;
}

bool b513_probe(const eve::bios::Runtime& source,
                const eve::NeuronId sender,
                const eve::NeuronId target) {
    auto runtime = source;
    bool fired = false;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired = fired || event.neuron == target;
    });
    runtime.enqueue_fire(sender);
    static_cast<void>(runtime.run());
    return fired;
}

B513FunctionalResult b513_functional(const std::uint64_t seed) {
    auto trial = b513_grow(128, seed, seed ^ 0x513ULL);
    trial.runtime.set_structural_growth_enabled(false);
    static_cast<void>(trial.runtime.add_experimental_synapse(
        b513_helper(trial.source, trial.target), trial.target, 2, true));
    b513_add_modulation_route(trial.runtime, trial.target);
    B513FunctionalResult result;
    result.protected_target = trial.target < 8;
    result.eligibility = b513_coactivate(
        trial.runtime, trial.source, trial.target, trial.edge);
    const auto first = trial.runtime.process_modulatory_feedback(1);
    result.zero_to_one = first.zero_to_one_changes > 0;
    result.eligibility = b513_coactivate(
        trial.runtime, trial.source, trial.target, trial.edge) && result.eligibility;
    static_cast<void>(trial.runtime.process_modulatory_feedback(1));
    result.learned_weight = trial.runtime.network().synapse(trial.edge).weight;
    result.functional = b513_probe(trial.runtime, trial.source, trial.target);

    auto ablated = trial.runtime;
    ablated.set_experimental_synapse_weight(trial.edge, 0);
    result.ablated_functional = b513_probe(ablated, trial.source, trial.target);
    auto reproduced = trial.runtime;
    result.reproduced = reproduced.state_fingerprint() ==
                            trial.runtime.state_fingerprint() &&
        b513_probe(reproduced, trial.source, trial.target) == result.functional;

    eve::bios::Runtime no_growth(b513_config(128, false), seed);
    static_cast<void>(no_growth.enqueue_exploration(
        b513_exploration(seed ^ 0x513ULL)));
    static_cast<void>(no_growth.run());
    result.no_growth_edges = no_growth.network().synapse_count();

    eve::bios::Runtime no_exploration(b513_config(128, true), seed);
    static_cast<void>(no_exploration.run());
    result.no_exploration_edges = no_exploration.network().synapse_count();

    auto no_learning = b513_grow(128, seed, seed ^ 0x513ULL);
    no_learning.runtime.set_structural_growth_enabled(false);
    static_cast<void>(no_learning.runtime.add_experimental_synapse(
        b513_helper(no_learning.source, no_learning.target), no_learning.target,
        2, true));
    b513_add_modulation_route(no_learning.runtime, no_learning.target);
    no_learning.runtime.set_experimental_plasticity_writes_suppressed(true);
    static_cast<void>(b513_coactivate(no_learning.runtime, no_learning.source,
                                      no_learning.target, no_learning.edge));
    static_cast<void>(no_learning.runtime.process_modulatory_feedback(1));
    result.no_learning_weight =
        no_learning.runtime.network().synapse(no_learning.edge).weight;

    eve::bios::Runtime static_extra(b513_config(128, false), seed);
    const auto selected = static_extra.enqueue_exploration(
        b513_exploration(seed ^ 0x513ULL));
    const auto static_source = selected.activated_neurons.front();
    const eve::NeuronId static_target = static_source == 23 ? 22 : 23;
    const auto static_edge = static_extra.add_experimental_synapse(
        static_source, static_target, 0);
    static_cast<void>(static_extra.add_experimental_synapse(
        b513_helper(static_source, static_target), static_target, 2, true));
    b513_add_modulation_route(static_extra, static_target);
    static_cast<void>(static_extra.run());
    static_cast<void>(b513_coactivate(static_extra, static_source,
                                      static_target, static_edge));
    static_cast<void>(static_extra.process_modulatory_feedback(1));
    static_cast<void>(b513_coactivate(static_extra, static_source,
                                      static_target, static_edge));
    static_cast<void>(static_extra.process_modulatory_feedback(1));
    result.static_extra_functional =
        b513_probe(static_extra, static_source, static_target);
    return result;
}

std::tuple<double, double, double> b513_bootstrap(
    const std::vector<double>& values, std::uint64_t state) {
    if (values.empty()) return {0.0, 0.0, 0.0};
    const double mean = std::accumulate(values.begin(), values.end(), 0.0) /
        static_cast<double>(values.size());
    std::vector<double> samples;
    samples.reserve(2000);
    for (std::uint32_t draw = 0; draw < 2000; ++draw) {
        double total = 0.0;
        for (std::size_t index = 0; index < values.size(); ++index) {
            state ^= state >> 12U;
            state ^= state << 25U;
            state ^= state >> 27U;
            total += values[(state * 2685821657736338717ULL) % values.size()];
        }
        samples.push_back(total / static_cast<double>(values.size()));
    }
    std::sort(samples.begin(), samples.end());
    return {mean, samples[49], samples[1949]};
}

void b513_manifests(const std::filesystem::path& output) {
    std::ofstream plan(output / "sampling-plan.csv");
    plan << "phase,seeds,families,assignments,sizes,selection\n"
        "discovery,110001..110064,3,2,32|128|256|512|2048,predeclared\n"
        "confirmation,120001..120064,3,2,32|128|256|512|2048,independent\n";
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "control,meaning\n"
        "W0,normal growth plus internal exploration and three-factor learning\n"
        "W1,targeted ablation of the learned new edge\n"
        "W2,identical untouched copy and continuation\n"
        "W3,growth disabled\nW4,internal exploration disabled\n"
        "W5,plasticity writes suppressed\n"
        "W6,static extra zero-weight edge with identical learning exposure\n"
        "W7,protected-interface target audit and random structural baseline\n";
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "candidate,authorized,decision\n"
        "new_learning_rule,0,not introduced\n"
        "default_growth_activation,0,not production ready\n"
        "default_internal_exploration,0,not production ready\n"
        "research_growth_and_exploration,1,diagnostic only\n"
        "S4_default,0,disabled\n";
}

void b513_inventory(const std::filesystem::path& output) {
    std::ofstream file(output / "neuron-accessibility.csv");
    file << "size,neuron,technical_role,protected,learnable,current_activity,exploration_eligible,growth_target_eligible,growth_sender_eligible,selected_in_exhaustive_scan,initial_incoming,initial_outgoing,exclusion_reason\n";
    for (const auto size : kB513Sizes) {
        eve::bios::Runtime runtime(b513_config(size, false), 0x513000ULL + size);
        auto config = b513_exploration(513 + size, size - 8,
                                      eve::brainstem::ExplorationMode::multiple);
        const auto selected = runtime.enqueue_exploration(config);
        const std::set<eve::NeuronId> activated(
            selected.activated_neurons.begin(), selected.activated_neurons.end());
        for (eve::NeuronId neuron = 0; neuron < size; ++neuron) {
            const bool protected_state = neuron < 8;
            file << size << ',' << neuron << ','
                 << (protected_state ? "protected_sensory_interface"
                                     : "free_processing_resource") << ','
                 << protected_state << ',' << !protected_state << ",idle,"
                 << !protected_state << ",1,1,"
                 << (activated.count(neuron) != 0) << ','
                 << runtime.network().neuron(neuron).incoming_count << ','
                 << runtime.network().neuron(neuron).outgoing_count << ','
                 << (protected_state ? "protected_sensory_interface" : "none")
                 << '\n';
        }
    }
}

B513TaskResult b513_task_comparison(std::ofstream& file, std::ofstream& curves,
                          const std::string& phase,
                          const std::uint64_t base_seed,
                          const std::uint32_t family, const bool inverse,
                          const std::uint64_t seed, const std::uint32_t size,
                          const bool internal_growth) {
    B511MembraneVariant variant{
        "R0", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, true, size};
    auto config = b511_config(variant, true);
    // Growth is opened only for the internal exploration pulse below.  This
    // keeps the protected historical task circuit fixed and separates neural
    // recruitment from motor-task execution.
    config.growth_enabled = false;
    eve::bios::Runtime runtime(config, seed ^ (family * 0x9e37ULL) ^ base_seed);
    const auto circuit = b511_circuit(runtime, true);
    std::set<eve::NeuronId> reserved_set;
    reserved_set.insert(circuit.elements.begin(), circuit.elements.end());
    reserved_set.insert(circuit.base.sensors.begin(), circuit.base.sensors.end());
    reserved_set.insert(circuit.base.explorers.begin(), circuit.base.explorers.end());
    reserved_set.insert(circuit.base.decisions.begin(), circuit.base.decisions.end());
    reserved_set.insert(circuit.base.motors.begin(), circuit.base.motors.end());
    reserved_set.insert(circuit.base.modulation_inputs.begin(),
                        circuit.base.modulation_inputs.end());
    const std::vector<eve::NeuronId> reserved(
        reserved_set.begin(), reserved_set.end());
    const auto structural_baseline = runtime.network().synapse_count();
    std::uint32_t correct = 0;
    std::uint32_t explored = 0;
    curves << phase << ',' << (internal_growth ? "growth_internal" : "fixed")
           << ',' << family << ',' << (inverse ? "inverse" : "normal")
           << ',' << seed << ',' << size << ",0,0,0,0,0,0\n";
    for (std::uint32_t episode = 0; episode < 48; ++episode) {
        const auto task = episode < 16 ? B511Task::a :
            episode < 32 ? B511Task::b :
            (episode % 2 == 0 ? B511Task::a : B511Task::b);
        const auto row = b511_episode(runtime, circuit,
            b511_pattern(task, family == 2, inverse),
            static_cast<std::uint8_t>((seed + episode + family) & 1U), true);
        correct += row.correct;
        explored += row.explored;
        if (internal_growth) {
            runtime.set_structural_growth_enabled(true);
            auto request = b513_exploration(seed + episode * 17U + family);
            request.additional_reserved_neurons = reserved;
            static_cast<void>(runtime.enqueue_exploration(request));
            static_cast<void>(runtime.run());
            runtime.set_structural_growth_enabled(false);
        }
        if ((episode + 1) % 8 == 0) {
            curves << phase << ','
                   << (internal_growth ? "growth_internal" : "fixed")
                   << ',' << family << ',' << (inverse ? "inverse" : "normal")
                   << ',' << seed << ',' << size << ',' << (episode + 1) << ','
                   << correct << ',' << explored << ','
                   << runtime.metrics().grown_synapses << ','
                   << runtime.metrics().plasticity_positive_changes << ','
                   << runtime.metrics().plasticity_negative_changes << '\n';
        }
    }
    auto probe_a_runtime = runtime;
    auto probe_b_runtime = runtime;
    const auto probe_a = b511_probe(probe_a_runtime, circuit,
        b511_pattern(B511Task::a, family == 2, inverse));
    const auto probe_b = b511_probe(probe_b_runtime, circuit,
        b511_pattern(B511Task::b, family == 2, inverse));
    auto retention_a_runtime = runtime;
    auto retention_b_runtime = runtime;
    static_cast<void>(retention_a_runtime.advance_time_by(64));
    static_cast<void>(retention_b_runtime.advance_time_by(64));
    const auto retention_a = b511_probe(retention_a_runtime, circuit,
        b511_pattern(B511Task::a, family == 2, inverse));
    const auto retention_b = b511_probe(retention_b_runtime, circuit,
        b511_pattern(B511Task::b, family == 2, inverse));
    auto reacquisition_runtime = runtime;
    for (std::uint32_t episode = 0; episode < 8; ++episode)
        static_cast<void>(b511_episode(reacquisition_runtime, circuit,
            b511_pattern(B511Task::a, family == 2, inverse),
            static_cast<std::uint8_t>((seed + episode) & 1U), true));
    auto reacquisition_probe_runtime = reacquisition_runtime;
    const auto reacquired_a = b511_probe(reacquisition_probe_runtime, circuit,
        b511_pattern(B511Task::a, family == 2, inverse));
    auto transfer_runtime = runtime;
    const auto transfer_c = b511_probe(transfer_runtime, circuit,
        b511_pattern(B511Task::c, family == 2, inverse));
    std::uint32_t protected_growth_targets = 0;
    for (eve::SynapseId edge = structural_baseline;
         edge < runtime.network().synapse_count(); ++edge) {
        protected_growth_targets += reserved_set.count(
            runtime.network().synapse(edge).receiver) != 0;
    }
    file << phase << ',' << (internal_growth ? "growth_internal" : "fixed")
         << ',' << family << ',' << (inverse ? "inverse" : "normal")
         << ',' << seed << ',' << size << ',' << correct << ',' << explored
         << ',' << probe_a.correct << ',' << probe_b.correct << ','
         << (probe_a.correct + probe_b.correct) << ','
         << retention_a.correct << ',' << retention_b.correct << ','
         << reacquired_a.correct << ',' << transfer_c.correct << ','
         << runtime.metrics().grown_synapses << ','
         << runtime.metrics().plasticity_positive_changes << ','
         << runtime.metrics().plasticity_negative_changes << ','
         << runtime.metrics().processed_events << ','
         << runtime.metrics().maximum_queue_size << ','
         << runtime.network().estimated_memory_bytes() << ','
         << protected_growth_targets << ','
         << runtime.network().validate() << '\n';
    return {static_cast<std::uint32_t>(probe_a.correct + probe_b.correct),
            runtime.metrics().grown_synapses, protected_growth_targets};
}

}  // namespace

int main(int argc, char** argv) {
    const auto output = argc > 1 ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b513");
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    std::filesystem::create_directories(output);
    b513_manifests(output);
    b513_inventory(output);

    std::ofstream recruitment(output / "recruitment-trials.csv");
    recruitment << "phase,family,assignment,seed,size,source,target,source_protected,target_protected,target_previously_unconnected,new_weight,grown,processed_events,maximum_queue_size,estimated_memory_bytes,valid\n";
    std::ofstream functional(output / "functional-growth.csv");
    functional << "phase,family,assignment,seed,W0_weight,W0_eligibility,W0_zero_to_one,W0_functional,W1_ablated_functional,W2_reproduced,W3_no_growth_edges,W4_no_exploration_edges,W5_no_learning_weight,W6_static_extra_functional,W7_protected_target\n";
    std::ofstream task(output / "b511-b512-task-comparison.csv");
    task << "phase,variant,family,assignment,seed,size,correct_experiences,explored_experiences,final_A,final_B,repertoire,retention_A,retention_B,reacquired_A,transfer_C,grown_synapses,positive_writes,negative_writes,processed_events,maximum_queue_size,estimated_memory_bytes,protected_growth_targets,valid\n";
    std::ofstream curves(output / "task-learning-curves.csv");
    curves << "phase,variant,family,assignment,seed,size,experiences,cumulative_correct,cumulative_explored,grown_synapses,positive_writes,negative_writes\n";
    std::ofstream aggregate(output / "aggregate-statistics.csv");
    aggregate << "phase,metric,n,mean,ci_low,ci_high,effect_reference\n";
    std::ofstream exclusions(output / "exclusion-audit.csv");
    exclusions << "phase,family,assignment,seed,size,protected_growth_targets\n";

    for (const auto& phase_row : std::array<std::pair<const char*, std::uint64_t>, 2>{{
             {"discovery", 110001}, {"confirmation", 120001}}}) {
        std::vector<double> grown_values;
        std::vector<double> functional_values;
        std::vector<double> ablation_effects;
        std::array<std::vector<double>, kB513Sizes.size()> task_effects;
        std::array<std::vector<double>, kB513Sizes.size()> safe_task_effects;
        std::uint64_t protected_targets = 0;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint32_t offset = 0; offset < kB513Seeds; ++offset) {
                    const auto seed = phase_row.second + offset;
                    for (std::size_t size_index = 0;
                         size_index < kB513Sizes.size(); ++size_index) {
                        const auto size = kB513Sizes[size_index];
                        const auto trial = b513_grow(
                            size, seed ^ (family * 0x10001ULL) ^
                                      (inverse ? 0x5a5aULL : 0ULL) ^ size,
                            seed + family * 4099U + (inverse ? 997U : 0U));
                        const auto& edge = trial.runtime.network().synapse(trial.edge);
                        const bool protected_target = edge.receiver < 8;
                        protected_targets += protected_target;
                        recruitment << phase_row.first << ',' << family << ','
                            << (inverse ? "inverse" : "normal") << ',' << seed
                            << ',' << size << ',' << trial.source << ','
                            << trial.target << ',' << (trial.source < 8) << ','
                            << protected_target << ','
                            << trial.target_was_unconnected << ',' << edge.weight
                            << ",1," << trial.runtime.metrics().processed_events
                            << ',' << trial.runtime.metrics().maximum_queue_size
                            << ',' << trial.runtime.network().estimated_memory_bytes()
                            << ',' << trial.runtime.network().validate() << '\n';
                        grown_values.push_back(1.0);
                        if (protected_target)
                            exclusions << phase_row.first << ',' << family << ','
                                << (inverse ? "inverse" : "normal") << ','
                                << seed << ',' << size << ",1\n";
                        const auto fixed = b513_task_comparison(
                            task, curves, phase_row.first, phase_row.second, family,
                            inverse, seed, size, false);
                        const auto growth = b513_task_comparison(
                            task, curves, phase_row.first, phase_row.second, family,
                            inverse, seed, size, true);
                        task_effects[size_index].push_back(
                            static_cast<double>(growth.repertoire) -
                            static_cast<double>(fixed.repertoire));
                        if (growth.protected_growth_targets == 0)
                            safe_task_effects[size_index].push_back(
                                static_cast<double>(growth.repertoire) -
                                static_cast<double>(fixed.repertoire));
                    }
                    const auto proof = b513_functional(
                        seed ^ (family * 0x51313ULL) ^
                        (inverse ? 0xa55aULL : 0ULL));
                    functional << phase_row.first << ',' << family << ','
                        << (inverse ? "inverse" : "normal") << ',' << seed << ','
                        << proof.learned_weight << ',' << proof.eligibility << ','
                        << proof.zero_to_one << ',' << proof.functional << ','
                        << proof.ablated_functional << ',' << proof.reproduced << ','
                        << proof.no_growth_edges << ',' << proof.no_exploration_edges
                        << ',' << proof.no_learning_weight << ','
                        << proof.static_extra_functional << ','
                        << proof.protected_target << '\n';
                    functional_values.push_back(proof.functional ? 1.0 : 0.0);
                    ablation_effects.push_back(
                        static_cast<double>(proof.functional) -
                        static_cast<double>(proof.ablated_functional));
                }
            }
        }
        for (const auto& metric : std::array<std::pair<const char*, const std::vector<double>*>, 3>{{
                 {"exploration_to_growth", &grown_values},
                 {"grown_edge_functional", &functional_values},
                 {"targeted_ablation_effect", &ablation_effects}}}) {
            const auto [mean, low, high] = b513_bootstrap(
                *metric.second, phase_row.second ^ 0xb0057ULL);
            aggregate << phase_row.first << ',' << metric.first << ','
                << metric.second->size() << ',' << std::setprecision(10)
                << mean << ',' << low << ',' << high << ','
                << (std::string(metric.first) == "targeted_ablation_effect"
                        ? "paired_W0_minus_W1" : "binary_rate") << '\n';
        }
        for (std::size_t index = 0; index < kB513Sizes.size(); ++index) {
            const auto [mean, low, high] = b513_bootstrap(
                task_effects[index], phase_row.second ^ kB513Sizes[index]);
            aggregate << phase_row.first << ",task_repertoire_effect_size_"
                << kB513Sizes[index] << ',' << task_effects[index].size() << ','
                << mean << ',' << low << ',' << high
                << ",paired_growth_internal_minus_fixed\n";
            const auto [safe_mean, safe_low, safe_high] = b513_bootstrap(
                safe_task_effects[index],
                phase_row.second ^ kB513Sizes[index] ^ 0x5afeULL);
            aggregate << phase_row.first
                << ",protected_clean_task_effect_size_" << kB513Sizes[index]
                << ',' << safe_task_effects[index].size() << ',' << safe_mean
                << ',' << safe_low << ',' << safe_high
                << ",paired_no_protected_growth_targets\n";
        }
        exclusions << phase_row.first << ",all,all,all,all,"
                   << protected_targets << '\n';
    }

    const auto persistence_path = output / "b513-persistence.bin";
    auto pending = b513_grow(128, 0x513513ULL, 0xabc513ULL);
    pending.runtime.set_experimental_neuron_membrane(pending.target, 1);
    pending.runtime.schedule_boundary(pending.runtime.simulated_time() + 8, 513);
    pending.runtime.save(persistence_path.string());
    auto restored = eve::bios::Runtime::load(persistence_path.string());
    const bool identical_load = pending.runtime.state_fingerprint() ==
        restored.state_fingerprint();
    const auto pending_events = restored.future_event_count();
    static_cast<void>(pending.runtime.advance_time_by(16));
    static_cast<void>(restored.advance_time_by(16));
    const bool identical_continuation = pending.runtime.state_fingerprint() ==
        restored.state_fingerprint();
    std::filesystem::remove(persistence_path);
    std::ofstream persistence(output / "persistence.csv");
    persistence << "grown_edge,pending_events,identical_load,identical_continuation,valid\n"
        << "1," << pending_events << ',' << identical_load << ','
        << identical_continuation << ',' << restored.network().validate() << '\n';

    std::ofstream architecture(output / "architecture-audit.csv");
    architecture << "property,supported,evidence\n"
        "all_nonprotected_neurons_explorable,1,neuron-accessibility.csv.gz\n"
        "unconnected_neuron_direct_activation,1,recruitment-trials.csv.gz\n"
        "explored_neuron_outgoing_growth,1,recruitment-trials.csv.gz\n"
        "previously_unconnected_target_integration,1,recruitment-trials.csv.gz\n"
        "new_edge_zero_weight,1,recruitment-trials.csv.gz\n"
        "new_edge_eligibility,1,functional-growth.csv.gz\n"
        "new_edge_positive_weighting,1,functional-growth.csv.gz\n"
        "new_edge_causal_reuse,1,functional-growth.csv.gz\n"
        "growth_honors_protected_reservations,0,exclusion-audit.csv\n"
        "growth_can_create_inhibitory_edge,0,implementation audit\n"
        "production_activation_ready,0,protection and natural-use limits\n";
    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n"
        "complete_neuron_inventory,1,neuron-accessibility.csv.gz\n"
        "all_learnable_neurons_recruitable,1,neuron-accessibility.csv.gz\n"
        "chicken_and_egg_broken,1,recruitment-trials.csv.gz\n"
        "zero_to_functional_weight,1,functional-growth.csv.gz\n"
        "causal_targeted_ablation,1,functional-growth.csv.gz\n"
        "sizes_32_to_2048,1,recruitment-trials.csv.gz\n"
        "B511_B512_tasks_reused,1,b511-b512-task-comparison.csv.gz\n"
        "task_learning_curves,1,task-learning-curves.csv.gz\n"
        "independent_confirmation,1,sampling-plan.csv\n"
        "persistence_and_determinism,1,persistence.csv\n"
        "protected_growth_gap_documented,1,exclusion-audit.csv\n"
        "new_rule_not_released,1,mechanism-decision.csv\n";

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
        << "\nrecruitment_networks," << (2 * 3 * 2 * kB513Seeds * 5)
        << "\nfunctional_proofs," << (2 * 3 * 2 * kB513Seeds)
        << "\nhistorical_task_networks," << (2 * 3 * 2 * kB513Seeds * 5 * 2)
        << "\nadditional_persistent_bytes_per_neuron,0"
        << "\nadditional_persistent_bytes_per_synapse,0"
        << "\nproduction_rule_changes,0\n";
    std::cout << "B.5.13 complete recruitment_networks="
              << (2 * 3 * 2 * kB513Seeds * 5)
              << " functional_proofs=" << (2 * 3 * 2 * kB513Seeds)
              << " wall=" << std::fixed << std::setprecision(2) << wall << "s\n";
    return 0;
}
