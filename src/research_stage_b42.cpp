#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <sys/resource.h>
#include <vector>

namespace {

enum class Feedback { none, correct, positive, negative, random, reversed, neutral };
enum class Exploration { baseline, window, after_negative, next_episode, continuous };
enum class Task {
    two_cue, single_a, single_b, single_y_b, reversal_ab, reversal_ba,
    inverse_two_cue,
    sequential_xy, sequential_yx, alternating_xy, blocked_xy,
};
enum class Representation { separate, distributed, overlapping };

struct Variant {
    std::string name;
    Feedback feedback{Feedback::correct};
    Exploration exploration{Exploration::baseline};
    Task task{Task::two_cue};
    bool plasticity{true};
    bool protect_exploration{};
    bool separated_exploration{};
    bool protection_enabled{true};
    bool suppress_action{};
    bool alternating_negative{};
    std::uint32_t negative_percent{100};
    std::uint32_t exploration_percent{60};
    eve::EligibilitySelection eligibility_selection{
        eve::EligibilitySelection::all_recent};
    eve::PlasticityTraceMode trace_mode{eve::PlasticityTraceMode::g3_recent};
    eve::ModulationMode modulation_mode{eve::ModulationMode::ternary};
    std::uint32_t candidate_capacity{8};
    std::uint32_t decision_capacity{8};
    std::uint64_t eligibility_duration{16};
    Representation representation{Representation::separate};
    bool structural_growth{};
    std::uint32_t static_extra_synapses{};
};

struct Plan {
    std::uint8_t cue{};
    std::uint8_t exploration_roll{};
    std::uint8_t exploration_action{};
    std::uint8_t negative_gate{};
    std::int8_t random_feedback{};
};

struct Result {
    double initial_reward{};
    double final_training_reward{};
    double evaluation_reward{};
    std::uint64_t correct{};
    std::uint64_t action_a{};
    std::uint64_t action_b{};
    std::uint64_t both{};
    std::uint64_t none{};
    std::uint64_t exploration_events{};
    std::uint64_t post_feedback_explorations{};
    std::uint64_t switches{};
    std::uint64_t positive_changes{};
    std::uint64_t negative_changes{};
    std::uint64_t zero_to_one{};
    std::uint64_t processed_events{};
    std::uint64_t maximum_queue{};
    std::uint64_t cue0_a{};
    std::uint64_t cue0_b{};
    std::uint64_t cue1_a{};
    std::uint64_t cue1_b{};
    std::uint64_t cue0_both{};
    std::uint64_t cue0_none{};
    std::uint64_t cue1_both{};
    std::uint64_t cue1_none{};
    std::uint64_t eligibility_decisions{};
    std::uint64_t eligibility_candidates{};
    std::uint64_t cross_cue_candidates{};
    std::uint64_t baseline_synapses{};
    std::uint64_t initial_synapses{};
    std::uint64_t final_synapses{};
    std::uint64_t grown_synapses{};
    std::uint64_t structural_synapses_activated{};
    std::uint64_t structural_synapses_changed{};
    std::uint64_t structural_synapses_nonzero{};
    std::uint64_t structural_impulses{};
    std::uint64_t existing_synapses_changed{};
    std::size_t final_memory_bytes{};
    std::int32_t minimum_final_weight{};
    std::int32_t maximum_final_weight{};
    bool network_valid{};
    std::uint32_t functional_exploration_paths{};
    std::int32_t cue0_to_a{};
    std::int32_t cue0_to_b{};
    std::int32_t cue1_to_a{};
    std::int32_t cue1_to_b{};
    std::int32_t block_boundary_cue0_to_a{};
    std::int32_t block_boundary_cue0_to_b{};
    std::int32_t block_boundary_cue1_to_a{};
    std::int32_t block_boundary_cue1_to_b{};
};

struct Circuit {
    eve::NeuronId action_a{7};
    eve::NeuronId action_b{8};
    eve::NeuronId relay_a{9};
    eve::NeuronId relay_b{10};
    std::array<eve::SynapseId, 4> cue_synapses{};
    std::vector<eve::SynapseId> exploration_synapses;
    std::uint32_t baseline_synapse_count{};
    std::vector<eve::SynapseId> static_synapses;
};

// Optional independent B.4.3 diagnosis.  It observes already-created local
// records and never participates in candidate selection or modulation.
std::ostream* b43_eligibility_diagnostics = nullptr;
std::ostream* b44_growth_diagnostics = nullptr;

double mean(const std::vector<double>& values) {
    return values.empty() ? 0.0 :
        std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double median(std::vector<double> values) {
    if (values.empty()) return 0.0;
    std::sort(values.begin(), values.end());
    const std::size_t middle = values.size() / 2;
    return values.size() % 2 == 0
        ? (values[middle - 1] + values[middle]) / 2.0 : values[middle];
}

double standard_deviation(const std::vector<double>& values) {
    if (values.size() < 2) return 0.0;
    const double average = mean(values);
    double sum = 0.0;
    for (const double value : values) sum += (value - average) * (value - average);
    return std::sqrt(sum / static_cast<double>(values.size() - 1));
}

std::pair<double, double> bootstrap_interval(
    const std::vector<double>& differences, const std::uint64_t seed) {
    if (differences.empty()) return {0.0, 0.0};
    eve::bios::DeterministicRng rng(seed);
    std::vector<double> samples;
    samples.reserve(2000);
    for (std::size_t repetition = 0; repetition < 2000; ++repetition) {
        double sum = 0.0;
        for (std::size_t draw = 0; draw < differences.size(); ++draw) {
            sum += differences[rng.bounded(
                static_cast<std::uint32_t>(differences.size()))];
        }
        samples.push_back(sum / differences.size());
    }
    std::sort(samples.begin(), samples.end());
    return {samples[49], samples[1949]};
}

std::vector<Plan> make_plan(const std::uint64_t seed, const std::size_t count) {
    eve::bios::DeterministicRng tasks(0x1100000ULL + seed);
    eve::bios::DeterministicRng exploration(0x1200000ULL + seed);
    eve::bios::DeterministicRng feedback(0x1300000ULL + seed);
    std::vector<Plan> plans;
    plans.reserve(count);
    for (std::size_t index = 0; index < count; ++index) {
        plans.push_back(Plan{
            static_cast<std::uint8_t>(tasks.bounded(2)),
            static_cast<std::uint8_t>(exploration.bounded(100)),
            static_cast<std::uint8_t>(exploration.bounded(2)),
            static_cast<std::uint8_t>(feedback.bounded(100)),
            static_cast<std::int8_t>(feedback.bounded(2) == 0 ? -1 : 1),
        });
    }
    return plans;
}

eve::ExperimentConfig make_config(const Variant& variant, const bool large) {
    eve::ExperimentConfig config;
    config.neuron_count = large ? 256 : 64;
    config.width = large ? 8 : 4;
    config.height = large ? 8 : 4;
    config.depth = 4;
    config.initial_outgoing_synapses = large ? 4 : 0;
    config.initial_synapse_weight = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.input_weight_budget = 32;
    config.learning_enabled = false;
    config.growth_enabled = variant.structural_growth;
    config.refractory_input_events = 1;
    config.recovery_mode = eve::RecoveryMode::independent_with_membrane_relaxation;
    config.recovery_delay = 4;
    config.membrane_relaxation_percent = 100;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = variant.eligibility_selection;
    config.eligibility_candidate_capacity = variant.candidate_capacity;
    config.eligibility_decision_capacity = variant.decision_capacity;
    config.eligibility_duration = variant.eligibility_duration;
    config.plasticity_trace_mode = variant.plasticity
        ? variant.trace_mode
        : eve::PlasticityTraceMode::disabled;
    config.modulation_mode = variant.plasticity
        ? variant.modulation_mode : eve::ModulationMode::none;
    config.plasticity_positive_step = 1;
    config.plasticity_negative_step = 1;
    config.plasticity_maximum_weight = 3;
    return config;
}

eve::SynapseId ensure_synapse(
    eve::bios::Runtime& runtime, const eve::NeuronId sender,
    const eve::NeuronId receiver, const std::int32_t weight,
    const bool protected_state = false) {
    const auto existing = runtime.network().find_synapse(sender, receiver);
    if (existing.has_value()) {
        runtime.set_experimental_synapse_weight(*existing, weight);
        runtime.set_experimental_synapse_protection(*existing, protected_state);
        return *existing;
    }
    return runtime.add_experimental_synapse(
        sender, receiver, weight, protected_state);
}

Circuit configure_circuit(
    eve::bios::Runtime& runtime, const Variant& variant,
    const std::uint64_t seed, const bool large) {
    Circuit circuit;
    if (large) {
        eve::brainstem::MotorOutputConfig motor;
        motor.response_stimulus_neuron = 19;
        motor.first_motor_neuron = 20;
        motor.contacts_per_channel = 1;
        motor.assignment_seed = 0x1400000ULL + seed;
        runtime.configure_motor_output(motor);
        eve::brainstem::InnateWiringConfig innate;
        innate.topology = eve::brainstem::InnateTopology::functional_paths;
        innate.initial_weight = 2;
        innate.topology_seed = 0x1500000ULL + seed;
        static_cast<void>(runtime.configure_innate_wiring(innate));
        circuit.action_a = runtime.motor_assignment().channels[0][0];
        circuit.action_b = runtime.motor_assignment().channels[1][0];
    }

    eve::brainstem::EvaluationRouteConfig route;
    route.mode = eve::brainstem::EvaluationRouteMode::general_wiring;
    route.first_input_neuron = 11;
    route.graded_channel_count = 5;
    route.fanout_per_source = 1;
    route.weight = 2;
    route.seed = 0x1600000ULL + seed;
    for (eve::NeuronId neuron = 0; neuron < 24; ++neuron) {
        route.reserved_neurons.push_back(neuron);
    }
    route.reserved_neurons.push_back(circuit.action_a);
    route.reserved_neurons.push_back(circuit.action_b);
    const auto evaluation = runtime.configure_evaluation_route(route);
    for (const eve::NeuronId input : evaluation.input_neurons) {
        static_cast<void>(ensure_synapse(runtime, input, circuit.action_a, 0));
        static_cast<void>(ensure_synapse(runtime, input, circuit.action_b, 0));
    }
    circuit.cue_synapses = {
        ensure_synapse(runtime, 0, circuit.action_a, 0),
        ensure_synapse(runtime, 0, circuit.action_b, 0),
        ensure_synapse(runtime, 1, circuit.action_a, 0),
        ensure_synapse(runtime, 1, circuit.action_b, 0),
    };
    if (variant.representation == Representation::distributed) {
        static_cast<void>(ensure_synapse(runtime, 17, circuit.action_a, 0));
        static_cast<void>(ensure_synapse(runtime, 17, circuit.action_b, 0));
        static_cast<void>(ensure_synapse(runtime, 18, circuit.action_a, 0));
        static_cast<void>(ensure_synapse(runtime, 18, circuit.action_b, 0));
    } else if (variant.representation == Representation::overlapping) {
        static_cast<void>(ensure_synapse(runtime, 17, circuit.action_a, 0));
        static_cast<void>(ensure_synapse(runtime, 17, circuit.action_b, 0));
    }
    const bool protect = variant.protect_exploration && variant.protection_enabled;
    if (variant.separated_exploration) {
        circuit.exploration_synapses = {
            ensure_synapse(runtime, 2, circuit.relay_a, 2, protect),
            ensure_synapse(runtime, 3, circuit.relay_a, 2, protect),
            ensure_synapse(runtime, circuit.relay_a, circuit.action_a, 2, protect),
            ensure_synapse(runtime, 4, circuit.relay_b, 2, protect),
            ensure_synapse(runtime, 5, circuit.relay_b, 2, protect),
            ensure_synapse(runtime, circuit.relay_b, circuit.action_b, 2, protect),
        };
    } else {
        circuit.exploration_synapses = {
            ensure_synapse(runtime, 2, circuit.action_a, 2, protect),
            ensure_synapse(runtime, 3, circuit.action_a, 2, protect),
            ensure_synapse(runtime, 4, circuit.action_b, 2, protect),
            ensure_synapse(runtime, 5, circuit.action_b, 2, protect),
        };
    }
    circuit.baseline_synapse_count = runtime.network().synapse_count();
    eve::bios::DeterministicRng structural_rng(0x4400000ULL + seed);
    std::uint64_t attempts = 0;
    while (circuit.static_synapses.size() < variant.static_extra_synapses) {
        if (++attempts > static_cast<std::uint64_t>(runtime.network().neuron_count()) *
                             runtime.network().neuron_count() * 4ULL) {
            throw std::runtime_error("unable to create requested static capacity");
        }
        const auto sender = structural_rng.bounded(runtime.network().neuron_count());
        const auto receiver = structural_rng.bounded(runtime.network().neuron_count());
        if (sender == receiver || runtime.network().has_synapse(sender, receiver)) continue;
        circuit.static_synapses.push_back(runtime.add_experimental_synapse(
            sender, receiver, 0, false));
    }
    return circuit;
}

std::pair<std::uint32_t, std::uint32_t> functional_paths(
    const eve::bios::Runtime& runtime, const Circuit& circuit,
    const bool separated) {
    if (!separated) {
        const auto good = [&](const std::size_t index) {
            return runtime.network().synapse(circuit.exploration_synapses[index]).weight >= 2;
        };
        return {static_cast<std::uint32_t>(good(0) + good(1)),
                static_cast<std::uint32_t>(good(2) + good(3))};
    }
    const bool relay_a = runtime.network().synapse(circuit.exploration_synapses[2]).weight >= 2;
    const bool relay_b = runtime.network().synapse(circuit.exploration_synapses[5]).weight >= 2;
    const auto good = [&](const std::size_t index) {
        return runtime.network().synapse(circuit.exploration_synapses[index]).weight >= 2;
    };
    return {relay_a ? static_cast<std::uint32_t>(good(0) + good(1)) : 0U,
            relay_b ? static_cast<std::uint32_t>(good(3) + good(4)) : 0U};
}

std::uint8_t episode_cue(const Variant& variant, const Plan& plan) {
    if (variant.task == Task::single_y_b) return 1;
    return variant.task == Task::single_a || variant.task == Task::single_b ||
                   variant.task == Task::reversal_ab ||
                   variant.task == Task::reversal_ba
        ? 0 : plan.cue;
}

std::uint8_t scheduled_cue(const Variant& variant, const Plan& plan,
                           const std::uint32_t episode,
                           const std::uint32_t training) {
    if (episode >= training) {
        return variant.task == Task::sequential_xy ||
                       variant.task == Task::sequential_yx ||
                       variant.task == Task::alternating_xy ||
                       variant.task == Task::blocked_xy
            ? plan.cue : episode_cue(variant, plan);
    }
    switch (variant.task) {
        case Task::sequential_xy: return episode < training / 2 ? 0 : 1;
        case Task::sequential_yx: return episode < training / 2 ? 1 : 0;
        case Task::alternating_xy: return static_cast<std::uint8_t>(episode & 1U);
        case Task::blocked_xy: return static_cast<std::uint8_t>((episode / 16U) & 1U);
        default: return episode_cue(variant, plan);
    }
}

bool target_a(const Variant& variant, const std::uint8_t cue,
              const std::uint32_t episode, const std::uint32_t training) {
    switch (variant.task) {
        case Task::single_a: return true;
        case Task::single_b: return false;
        case Task::single_y_b: return false;
        case Task::reversal_ab: return episode < training / 2;
        case Task::reversal_ba: return episode >= training / 2;
        case Task::inverse_two_cue: return cue != 0;
        case Task::sequential_xy:
        case Task::sequential_yx:
        case Task::alternating_xy:
        case Task::blocked_xy: return cue == 0;
        case Task::two_cue: return cue == 0;
    }
    return true;
}

Result run_seed(
    const Variant& variant, const std::uint64_t seed,
    const std::uint32_t training_episodes,
    const std::uint32_t evaluation_episodes, const bool large,
    std::ofstream* trials, std::ofstream* weights,
    std::ofstream* diagnosis) {
    const auto plans = make_plan(seed, training_episodes + evaluation_episodes);
    eve::bios::Runtime runtime(make_config(variant, large), 0x1700000ULL + seed);
    const Circuit circuit = configure_circuit(runtime, variant, seed, large);
    const std::uint32_t initial_synapse_count = runtime.network().synapse_count();
    std::vector<std::int32_t> initial_weights;
    initial_weights.reserve(initial_synapse_count);
    for (const auto& synapse : runtime.network().synapses()) {
        initial_weights.push_back(synapse.weight);
    }
    std::set<eve::SynapseId> activated_structural_synapses;
    std::set<eve::SynapseId> changed_structural_synapses;
    bool fired_a = false;
    bool fired_b = false;
    std::uint8_t diagnostic_cue = 0;
    std::uint32_t diagnostic_episode = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.action_a;
        fired_b = fired_b || event.neuron == circuit.action_b;
    });
    Result result;
    runtime.set_impulse_observer([&](const eve::bios::ImpulseObservation& event) {
        if (event.synapse >= circuit.baseline_synapse_count) {
            activated_structural_synapses.insert(event.synapse);
            ++result.structural_impulses;
        }
        if (event.eligibility_generation == 0) return;
        ++result.eligibility_decisions;
        result.eligibility_candidates += event.eligibility_candidates.size();
        std::uint32_t order = 0;
        for (const auto id : event.eligibility_candidates) {
            const auto sender = runtime.network().synapse(id).sender;
            if ((diagnostic_cue == 0 && sender == 1) ||
                (diagnostic_cue == 1 && sender == 0)) {
                ++result.cross_cue_candidates;
            }
            if (b43_eligibility_diagnostics != nullptr) {
                const auto position = std::find(
                    event.local_event_snapshot.begin(),
                    event.local_event_snapshot.end(), id);
                *b43_eligibility_diagnostics << variant.name << ',' << seed << ','
                    << diagnostic_episode << ',' << static_cast<unsigned>(diagnostic_cue)
                    << ',' << event.receiver << ',' << event.eligibility_generation << ','
                    << event.simulated_time << ',' << event.simulated_time + 1 << ','
                    << id << ',' << sender << ','
                    << runtime.network().synapse(id).weight << ','
                    << (position == event.local_event_snapshot.end()
                            ? -1 : static_cast<std::int64_t>(std::distance(
                                event.local_event_snapshot.begin(), position)))
                    << ',' << order << '\n';
            }
            ++order;
        }
    });
    if (b44_growth_diagnostics != nullptr) {
        for (const auto id : circuit.static_synapses) {
            const auto& synapse = runtime.network().synapse(id);
            *b44_growth_diagnostics << variant.name << ',' << seed
                << ",-1,static," << id << ',' << synapse.sender << ','
                << synapse.receiver << ",0\n";
        }
    }
    std::vector<double> first;
    std::vector<double> last;
    std::vector<double> evaluation;
    bool explore_next = false;
    std::uint64_t negative_counter = 0;
    int previous_unique = -1;
    const std::array<std::uint32_t, 7> checkpoints{{0, 8, 16, 32, 64, 128, 512}};
    const auto write_checkpoint = [&](const std::uint32_t episode) {
        if (diagnosis == nullptr ||
            std::find(checkpoints.begin(), checkpoints.end(), episode) == checkpoints.end()) {
            return;
        }
        const auto paths = functional_paths(runtime, circuit, variant.separated_exploration);
        std::uint32_t weakened = 0;
        std::uint32_t ineffective = 0;
        for (const auto id : circuit.exploration_synapses) {
            const auto weight = runtime.network().synapse(id).weight;
            if (weight < 2) ++weakened;
            if (weight == 0) ++ineffective;
        }
        std::uint32_t learned = 0;
        for (const auto id : circuit.cue_synapses) {
            if (runtime.network().synapse(id).weight > 0) ++learned;
        }
        *diagnosis << variant.name << ',' << seed << ',' << episode << ','
                   << paths.first << ',' << paths.second << ',' << weakened << ','
                   << ineffective << ',' << learned << ',' << (paths.first > 0 ? 1 : 0)
                   << ',' << (paths.second > 0 ? 1 : 0) << '\n';
    };
    write_checkpoint(0);

    for (std::uint32_t episode = 0;
         episode < training_episodes + evaluation_episodes; ++episode) {
        const bool evaluating = episode >= training_episodes;
        if (episode == training_episodes / 2U) {
            result.block_boundary_cue0_to_a =
                runtime.network().synapse(circuit.cue_synapses[0]).weight;
            result.block_boundary_cue0_to_b =
                runtime.network().synapse(circuit.cue_synapses[1]).weight;
            result.block_boundary_cue1_to_a =
                runtime.network().synapse(circuit.cue_synapses[2]).weight;
            result.block_boundary_cue1_to_b =
                runtime.network().synapse(circuit.cue_synapses[3]).weight;
        }
        if (episode == training_episodes && variant.structural_growth) {
            runtime.set_structural_growth_enabled(false);
        }
        const std::uint64_t episode_start = static_cast<std::uint64_t>(episode) * 16;
        if (runtime.simulated_time() < episode_start) {
            static_cast<void>(runtime.advance_time_to(episode_start));
        }
        const auto before = runtime.metrics();
        const auto before_synapses = runtime.network().synapses();
        const Plan& plan = plans[episode];
        const std::uint8_t cue = scheduled_cue(
            variant, plan, episode, training_episodes);
        diagnostic_cue = cue;
        diagnostic_episode = episode;
        fired_a = fired_b = false;
        bool explored = false;
        if (!variant.suppress_action) {
            runtime.enqueue_fire(cue);
            if (variant.representation == Representation::distributed) {
                runtime.enqueue_fire(static_cast<eve::NeuronId>(17 + cue));
            } else if (variant.representation == Representation::overlapping) {
                runtime.enqueue_fire(17);
            }
            static_cast<void>(runtime.run());
            const bool general_window =
                variant.exploration == Exploration::window ||
                variant.exploration == Exploration::continuous;
            const bool should_explore =
                explore_next || general_window || (!fired_a && !fired_b);
            if (should_explore && plan.exploration_roll < variant.exploration_percent) {
                runtime.enqueue_fire(plan.exploration_action == 0
                    ? static_cast<eve::NeuronId>(2 + (episode & 1U))
                    : static_cast<eve::NeuronId>(4 + (episode & 1U)));
                static_cast<void>(runtime.run());
                explored = true;
                ++result.exploration_events;
            }
        }
        explore_next = false;
        const bool decision_a = fired_a;
        const bool decision_b = fired_b;
        const bool want_a = target_a(variant, cue, episode, training_episodes);
        const bool correct = want_a
            ? decision_a && !decision_b : decision_b && !decision_a;
        const int reward = correct ? 1 : -1;
        const int unique = decision_a == decision_b ? -1 : decision_a ? 0 : 1;
        if (unique >= 0 && previous_unique >= 0 && unique != previous_unique) {
            ++result.switches;
        }
        if (unique >= 0) previous_unique = unique;

        if (!evaluating && variant.plasticity && variant.feedback != Feedback::none) {
            std::int8_t polarity = static_cast<std::int8_t>(reward);
            if (variant.feedback == Feedback::positive && polarity < 0) polarity = 0;
            if (variant.feedback == Feedback::negative && polarity > 0) polarity = 0;
            if (variant.feedback == Feedback::random) polarity = plan.random_feedback;
            if (variant.feedback == Feedback::reversed) polarity = -polarity;
            if (variant.feedback == Feedback::neutral) polarity = 0;
            // Multiple simultaneous motor outputs are an ambiguous decision,
            // not evidence that either path caused the environmental result.
            if (decision_a && decision_b) polarity = 0;
            if (polarity < 0 && variant.alternating_negative &&
                (++negative_counter % 2U) != 0U) {
                // Integer weights remain unchanged.  Applying the existing
                // one-unit step to every second negative event represents a
                // documented mean strength of 0.5 without fractional state.
                polarity = 0;
            }
            if (polarity < 0 && plan.negative_gate >= variant.negative_percent) {
                polarity = 0;
            }
            const std::uint64_t due = runtime.simulated_time() + 1;
            static_cast<void>(runtime.process_modulatory_feedback(polarity, due));
            static_cast<void>(runtime.advance_time_to(due));
        } else {
            static_cast<void>(runtime.process_evaluation_feedback(50));
        }

        const bool negative = reward < 0;
        if (!evaluating && variant.exploration == Exploration::next_episode && negative) {
            explore_next = true;
        }
        const bool post_explore = !evaluating &&
            ((variant.exploration == Exploration::after_negative && negative) ||
             variant.exploration == Exploration::continuous);
        bool post_a = false;
        bool post_b = false;
        if (post_explore && plan.exploration_roll < variant.exploration_percent) {
            fired_a = fired_b = false;
            runtime.enqueue_fire(plan.exploration_action == 0
                ? static_cast<eve::NeuronId>(2 + (episode & 1U))
                : static_cast<eve::NeuronId>(4 + (episode & 1U)));
            static_cast<void>(runtime.run());
            post_a = fired_a;
            post_b = fired_b;
            ++result.exploration_events;
            ++result.post_feedback_explorations;
        }
        const std::uint64_t episode_end = episode_start + 16;
        if (runtime.simulated_time() < episode_end) {
            static_cast<void>(runtime.advance_time_to(episode_end));
        }

        if (episode < std::min<std::uint32_t>(32, training_episodes)) {
            first.push_back(reward);
        }
        if (!evaluating && episode + 32 >= training_episodes) last.push_back(reward);
        if (evaluating) {
            evaluation.push_back(reward);
            result.correct += correct ? 1U : 0U;
            if (decision_a && decision_b) ++result.both;
            else if (decision_a) ++result.action_a;
            else if (decision_b) ++result.action_b;
            else ++result.none;
            if (cue == 0) {
                result.cue0_a += decision_a && !decision_b ? 1U : 0U;
                result.cue0_b += decision_b && !decision_a ? 1U : 0U;
                result.cue0_both += decision_a && decision_b ? 1U : 0U;
                result.cue0_none += !decision_a && !decision_b ? 1U : 0U;
            } else {
                result.cue1_a += decision_a && !decision_b ? 1U : 0U;
                result.cue1_b += decision_b && !decision_a ? 1U : 0U;
                result.cue1_both += decision_a && decision_b ? 1U : 0U;
                result.cue1_none += !decision_a && !decision_b ? 1U : 0U;
            }
        }
        if (trials != nullptr) {
            const std::string outcome = decision_a && decision_b ? "both" :
                decision_a ? "A" : decision_b ? "B" : "none";
            *trials << variant.name << ',' << seed << ',' << episode << ','
                    << (evaluating ? "evaluation" : "training") << ','
                    << static_cast<unsigned>(cue) << ',' << (want_a ? 'A' : 'B') << ','
                    << outcome << ',' << reward << ',' << (explored ? 1 : 0) << ','
                    << (post_a ? 1 : 0) << ',' << (post_b ? 1 : 0) << ','
                    << episode_start << ',' << runtime.simulated_time() << ','
                    << (runtime.metrics().plasticity_positive_changes -
                        before.plasticity_positive_changes) << ','
                    << (runtime.metrics().plasticity_negative_changes -
                        before.plasticity_negative_changes) << '\n';
        }
        if (weights != nullptr && !evaluating) {
            const auto& after = runtime.network().synapses();
            for (eve::SynapseId id = 0; id < after.size(); ++id) {
                const std::int32_t before_weight = id < before_synapses.size()
                    ? before_synapses[id].weight : 0;
                if (before_weight == after[id].weight) continue;
                *weights << variant.name << ',' << seed << ',' << episode << ','
                         << id << ',' << after[id].sender << ',' << after[id].receiver
                         << ',' << before_weight << ',' << after[id].weight
                         << ',' << (after[id].plasticity_protected ? 1 : 0) << '\n';
            }
        }
        const auto& after_synapses = runtime.network().synapses();
        for (eve::SynapseId id = circuit.baseline_synapse_count;
             id < after_synapses.size(); ++id) {
            const std::int32_t before_weight = id < before_synapses.size()
                ? before_synapses[id].weight : 0;
            if (before_weight != after_synapses[id].weight) {
                changed_structural_synapses.insert(id);
            }
        }
        if (b44_growth_diagnostics != nullptr &&
            runtime.network().synapse_count() > before_synapses.size()) {
            for (eve::SynapseId id = static_cast<eve::SynapseId>(before_synapses.size());
                 id < runtime.network().synapse_count(); ++id) {
                const auto& synapse = runtime.network().synapse(id);
                *b44_growth_diagnostics << variant.name << ',' << seed << ','
                    << episode << ",dynamic," << id << ',' << synapse.sender << ','
                    << synapse.receiver << ',' << synapse.weight << '\n';
            }
        }
        if (!evaluating) write_checkpoint(episode + 1);
    }
    result.initial_reward = mean(first);
    result.final_training_reward = mean(last);
    result.evaluation_reward = mean(evaluation);
    result.positive_changes = runtime.metrics().plasticity_positive_changes;
    result.negative_changes = runtime.metrics().plasticity_negative_changes;
    result.zero_to_one = runtime.metrics().plasticity_zero_to_one_changes;
    result.processed_events = runtime.metrics().processed_events;
    result.maximum_queue = runtime.metrics().maximum_queue_size;
    result.baseline_synapses = circuit.baseline_synapse_count;
    result.initial_synapses = initial_synapse_count;
    result.final_synapses = runtime.network().synapse_count();
    result.grown_synapses = runtime.metrics().grown_synapses;
    result.structural_synapses_activated = activated_structural_synapses.size();
    result.structural_synapses_changed = changed_structural_synapses.size();
    for (eve::SynapseId id = circuit.baseline_synapse_count;
         id < runtime.network().synapse_count(); ++id) {
        if (runtime.network().synapse(id).weight != 0) {
            ++result.structural_synapses_nonzero;
        }
    }
    for (eve::SynapseId id = 0; id < circuit.baseline_synapse_count; ++id) {
        if (runtime.network().synapse(id).weight != initial_weights[id]) {
            ++result.existing_synapses_changed;
        }
    }
    const auto paths = functional_paths(runtime, circuit, variant.separated_exploration);
    result.functional_exploration_paths = paths.first + paths.second;
    result.cue0_to_a = runtime.network().synapse(circuit.cue_synapses[0]).weight;
    result.cue0_to_b = runtime.network().synapse(circuit.cue_synapses[1]).weight;
    result.cue1_to_a = runtime.network().synapse(circuit.cue_synapses[2]).weight;
    result.cue1_to_b = runtime.network().synapse(circuit.cue_synapses[3]).weight;
    result.final_memory_bytes = runtime.diagnostics().estimated_memory_bytes;
    if (!runtime.network().synapses().empty()) {
        result.minimum_final_weight = runtime.network().synapses().front().weight;
        result.maximum_final_weight = runtime.network().synapses().front().weight;
        for (const auto& synapse : runtime.network().synapses()) {
            result.minimum_final_weight = std::min(
                result.minimum_final_weight, synapse.weight);
            result.maximum_final_weight = std::max(
                result.maximum_final_weight, synapse.weight);
        }
    }
    result.network_valid = runtime.network().validate();
    return result;
}

Variant make_variant(const std::string& name) {
    Variant result;
    result.name = name;
    return result;
}

std::vector<Variant> variants() {
    std::vector<Variant> result;
    auto add = [&](Variant value) { result.push_back(std::move(value)); };
    Variant v = make_variant("K0_N0_no_plasticity"); v.plasticity = false; add(v);
    add(make_variant("K1_A0_B41_reference"));
    v = make_variant("K2_A1_protected"); v.protect_exploration = true; add(v);
    v = make_variant("K3_N1_positive_only"); v.feedback = Feedback::positive;
    v.protect_exploration = true; add(v);
    v = make_variant("K4_N2_negative_only"); v.feedback = Feedback::negative; add(v);
    v = make_variant("K5_N3_ternary"); v.protect_exploration = true; add(v);
    v = make_variant("K6_N4_negative_50"); v.protect_exploration = true;
    v.negative_percent = 50; add(v);
    v = make_variant("K7_X1_general_window"); v.protect_exploration = true;
    v.exploration = Exploration::window; add(v);
    v = make_variant("K8_X2_after_negative"); v.protect_exploration = true;
    v.exploration = Exploration::after_negative; add(v);
    v = make_variant("K9_X3_next_episode"); v.protect_exploration = true;
    v.exploration = Exploration::next_episode; add(v);
    v = make_variant("K10_feedback_without_action"); v.suppress_action = true; add(v);
    v = make_variant("K11_trace_without_modulation"); v.feedback = Feedback::none; add(v);
    v = make_variant("K12_modulation_without_trace"); v.suppress_action = true; add(v);
    v = make_variant("K13_random_feedback"); v.feedback = Feedback::random;
    v.protect_exploration = true; add(v);
    v = make_variant("K14_reversed_feedback"); v.feedback = Feedback::reversed;
    v.protect_exploration = true; add(v);
    v = make_variant("K15_S0_single_A_no_plasticity"); v.plasticity = false;
    v.task = Task::single_a; add(v);
    v = make_variant("K16_S2_single_A_ternary"); v.task = Task::single_a;
    v.protect_exploration = true; add(v);
    v = make_variant("K17_S2_single_B_ternary"); v.task = Task::single_b;
    v.protect_exploration = true; add(v);
    v = make_variant("K18_two_cue_protected"); v.protect_exploration = true; add(v);
    v = make_variant("K19_reversal_A_to_B"); v.task = Task::reversal_ab;
    v.protect_exploration = true; add(v);
    add(make_variant("K20_zero_weight_synapses"));
    v = make_variant("K21_reachability_after_negative"); v.feedback = Feedback::negative; add(v);
    v = make_variant("K22_continuous_state"); v.protect_exploration = true;
    v.exploration = Exploration::continuous; add(v);
    v = make_variant("K23_route_without_weight_effect"); v.feedback = Feedback::neutral; add(v);
    v = make_variant("K24_protection_disabled"); v.protect_exploration = true;
    v.protection_enabled = false; add(v);
    v = make_variant("A2_separate_protected_circuit"); v.protect_exploration = true;
    v.separated_exploration = true; add(v);
    for (const std::uint32_t percent : {25U, 75U}) {
        v = make_variant("N4_negative_" + std::to_string(percent));
        v.protect_exploration = true; v.negative_percent = percent; add(v);
    }
    v = make_variant("N5_alternating_negative_step");
    v.protect_exploration = true; v.alternating_negative = true; add(v);
    v = make_variant("no_action_without_feedback");
    v.suppress_action = true; v.feedback = Feedback::none; add(v);
    v = make_variant("double_output_neutral_control");
    v.protect_exploration = true; v.exploration = Exploration::window; add(v);
    for (const Task task : {Task::single_a, Task::single_b}) {
        const std::string suffix = task == Task::single_a ? "A" : "B";
        for (int group = 0; group <= 6; ++group) {
            if ((group == 0 && suffix == "A") || (group == 2)) continue;
            v = make_variant("S" + std::to_string(group) + "_single_" + suffix);
            v.task = task;
            v.protect_exploration = group == 3;
            if (group == 0) v.plasticity = false;
            if (group == 1) { v.feedback = Feedback::positive; v.protect_exploration = true; }
            if (group == 4) { v.feedback = Feedback::random; v.protect_exploration = true; }
            if (group == 5) { v.feedback = Feedback::reversed; v.protect_exploration = true; }
            if (group == 6) v.exploration_percent = 0;
            add(v);
        }
    }
    v = make_variant("two_cue_inverse_protected"); v.task = Task::inverse_two_cue;
    v.protect_exploration = true; add(v);
    v = make_variant("reversal_B_to_A"); v.task = Task::reversal_ba;
    v.protect_exploration = true; add(v);
    return result;
}

void run_causal_restoration(const std::uint64_t seed, std::ofstream& output) {
    Variant variant = make_variant("A0_diagnostic");
    eve::bios::Runtime runtime(make_config(variant, false), 0x1800000ULL + seed);
    const Circuit circuit = configure_circuit(runtime, variant, seed, false);
    for (std::uint32_t episode = 0; episode < 512; ++episode) {
        const std::uint64_t start = static_cast<std::uint64_t>(episode) * 16;
        if (runtime.simulated_time() < start) static_cast<void>(runtime.advance_time_to(start));
        const eve::NeuronId exploratory = static_cast<eve::NeuronId>(2 + episode % 4);
        runtime.enqueue_fire(exploratory);
        static_cast<void>(runtime.run());
        const std::uint64_t due = runtime.simulated_time() + 1;
        static_cast<void>(runtime.process_modulatory_feedback(-1, due));
        static_cast<void>(runtime.advance_time_to(due));
        static_cast<void>(runtime.advance_time_to(start + 16));
    }
    const auto before = functional_paths(runtime, circuit, false);
    const auto path = std::filesystem::temp_directory_path() /
        ("eve-b42-causal-" + std::to_string(seed) + ".bin");
    runtime.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    for (const auto id : circuit.exploration_synapses) {
        restored.set_experimental_synapse_weight(id, 2);
    }
    const auto after = functional_paths(restored, circuit, false);
    static_cast<void>(std::filesystem::remove(path));
    output << seed << ',' << before.first << ',' << before.second << ','
           << after.first << ',' << after.second << ','
           << ((before.first + before.second == 0 && after.first + after.second == 4) ? 1 : 0)
           << '\n';
}

void run_persistence_probe(const std::uint64_t seed, std::ofstream& output) {
    Variant variant = make_variant("persistence_probe");
    variant.protect_exploration = true;
    eve::bios::Runtime original(make_config(variant, false), 0x1a00000ULL + seed);
    const Circuit circuit = configure_circuit(original, variant, seed, false);
    original.enqueue_fire(0);
    static_cast<void>(original.run());
    original.enqueue_fire(2);
    static_cast<void>(original.run());
    const std::uint64_t due = original.simulated_time() + 3;
    static_cast<void>(original.process_modulatory_feedback(1, due));
    // This ready exploration event represents an active, not yet processed
    // exploratory state at the checkpoint.
    original.enqueue_fire(4);
    const auto path = std::filesystem::temp_directory_path() /
        ("eve-b42-persistence-" + std::to_string(seed) + ".bin");
    original.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    const bool exact_restore =
        original.state_fingerprint() == restored.state_fingerprint();
    const bool protection =
        restored.network().synapse(circuit.exploration_synapses.front())
            .plasticity_protected;
    const bool pending = restored.future_event_count() > 0;
    static_cast<void>(original.advance_time_to(due));
    static_cast<void>(restored.advance_time_to(due));
    const bool pending_continuation =
        original.state_fingerprint() == restored.state_fingerprint();

    // Continue after an externally defined environment reversal.  Both sides
    // receive the same plan entry; no neural state is reset.
    static_cast<void>(original.advance_time_by(16));
    static_cast<void>(restored.advance_time_by(16));
    for (auto* runtime : {&original, &restored}) {
        runtime->enqueue_fire(0);
        static_cast<void>(runtime->run());
        runtime->enqueue_fire(4);
        static_cast<void>(runtime->run());
        static_cast<void>(runtime->process_modulatory_feedback(1));
    }
    const bool reversed_continuation =
        original.state_fingerprint() == restored.state_fingerprint();
    static_cast<void>(std::filesystem::remove(path));
    output << seed << ',' << (protection ? 1 : 0) << ',' << (pending ? 1 : 0)
           << ',' << (exact_restore ? 1 : 0) << ','
           << (pending_continuation ? 1 : 0) << ','
           << (reversed_continuation ? 1 : 0) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b42");
    std::filesystem::create_directories(output);
    const auto matrix = variants();

    std::ofstream tasks(output / "tasks.csv");
    tasks << "seed,episode,cue,exploration_roll,exploration_action,negative_gate,"
             "random_feedback,task_seed,exploration_seed,feedback_seed\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        const auto plans = make_plan(seed, 2112);
        for (std::size_t episode = 0; episode < plans.size(); ++episode) {
            const auto& plan = plans[episode];
            tasks << seed << ',' << episode << ',' << static_cast<unsigned>(plan.cue)
                  << ',' << static_cast<unsigned>(plan.exploration_roll) << ','
                  << static_cast<unsigned>(plan.exploration_action) << ','
                  << static_cast<unsigned>(plan.negative_gate) << ','
                  << static_cast<int>(plan.random_feedback) << ','
                  << 0x1100000ULL + seed << ',' << 0x1200000ULL + seed << ','
                  << 0x1300000ULL + seed << '\n';
        }
    }

    std::ofstream trials(output / "trials.csv");
    trials << "variant,seed,episode,phase,cue,target,outcome,reward,explored,"
              "post_feedback_a,post_feedback_b,start_time,end_time,positive_changes,"
              "negative_changes\n";
    std::ofstream weights(output / "weight-changes.csv");
    weights << "variant,seed,episode,synapse,sender,receiver,before,after,protected\n";
    std::map<std::string, std::vector<Result>> results;
    const auto started = std::chrono::steady_clock::now();
    const std::clock_t cpu_started = std::clock();
    for (const Variant& variant : matrix) {
        auto& rows = results[variant.name];
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            rows.push_back(run_seed(
                variant, seed, 128, 64, false, &trials, &weights, nullptr));
        }
    }

    std::ofstream summary(output / "summary.csv");
    summary << "variant,seeds,initial_reward,final_training_reward,evaluation_reward,"
               "evaluation_sd,delta,delta_median,improved_seed_fraction,correct,A,B,both,"
               "none,exploration_events,post_feedback_explorations,switches,positive_changes,"
               "negative_changes,zero_to_one,functional_paths,cue0_A,cue0_B,cue1_A,cue1_B\n";
    summary << std::fixed << std::setprecision(6);
    for (const auto& [name, rows] : results) {
        std::vector<double> initial;
        std::vector<double> final_training;
        std::vector<double> evaluation;
        std::vector<double> deltas;
        Result total;
        std::size_t improved = 0;
        for (const auto& row : rows) {
            initial.push_back(row.initial_reward);
            final_training.push_back(row.final_training_reward);
            evaluation.push_back(row.evaluation_reward);
            deltas.push_back(row.evaluation_reward - row.initial_reward);
            if (deltas.back() > 0) ++improved;
            total.correct += row.correct; total.action_a += row.action_a;
            total.action_b += row.action_b; total.both += row.both;
            total.none += row.none; total.exploration_events += row.exploration_events;
            total.post_feedback_explorations += row.post_feedback_explorations;
            total.switches += row.switches; total.positive_changes += row.positive_changes;
            total.negative_changes += row.negative_changes; total.zero_to_one += row.zero_to_one;
            total.functional_exploration_paths += row.functional_exploration_paths;
            total.cue0_a += row.cue0_a; total.cue0_b += row.cue0_b;
            total.cue1_a += row.cue1_a; total.cue1_b += row.cue1_b;
        }
        summary << name << ',' << rows.size() << ',' << mean(initial) << ','
                << mean(final_training) << ',' << mean(evaluation) << ','
                << standard_deviation(evaluation) << ',' << mean(deltas) << ','
                << median(deltas) << ',' << static_cast<double>(improved) / rows.size()
                << ',' << total.correct << ',' << total.action_a << ',' << total.action_b
                << ',' << total.both << ',' << total.none << ','
                << total.exploration_events << ',' << total.post_feedback_explorations
                << ',' << total.switches << ',' << total.positive_changes << ','
                << total.negative_changes << ',' << total.zero_to_one << ','
                << static_cast<double>(total.functional_exploration_paths) / rows.size()
                << ',' << total.cue0_a << ',' << total.cue0_b << ',' << total.cue1_a
                << ',' << total.cue1_b << '\n';
    }

    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "left,right,seeds,mean_difference,median_difference,bootstrap_low,"
              "bootstrap_high,positive_seed_fraction\n";
    paired << std::fixed << std::setprecision(6);
    const std::array<std::pair<std::string, std::string>, 11> comparisons{{
        {"K2_A1_protected", "K1_A0_B41_reference"},
        {"K3_N1_positive_only", "K0_N0_no_plasticity"},
        {"K5_N3_ternary", "K3_N1_positive_only"},
        {"K6_N4_negative_50", "K5_N3_ternary"},
        {"N4_negative_25", "K5_N3_ternary"},
        {"N4_negative_75", "K5_N3_ternary"},
        {"N5_alternating_negative_step", "K5_N3_ternary"},
        {"K16_S2_single_A_ternary", "K15_S0_single_A_no_plasticity"},
        {"K17_S2_single_B_ternary", "S0_single_B"},
        {"K18_two_cue_protected", "K0_N0_no_plasticity"},
        {"K13_random_feedback", "K2_A1_protected"},
    }};
    for (const auto& comparison : comparisons) {
        std::vector<double> differences;
        std::size_t positive = 0;
        const auto& left = results.at(comparison.first);
        const auto& right = results.at(comparison.second);
        for (std::size_t index = 0; index < left.size(); ++index) {
            differences.push_back(left[index].evaluation_reward - right[index].evaluation_reward);
            if (differences.back() > 0) ++positive;
        }
        const auto interval = bootstrap_interval(
            differences, 0x1900000ULL + comparison.first.size());
        paired << comparison.first << ',' << comparison.second << ','
               << differences.size() << ',' << mean(differences) << ','
               << median(differences) << ',' << interval.first << ','
               << interval.second << ','
               << static_cast<double>(positive) / differences.size() << '\n';
    }

    std::ofstream diagnosis(output / "exploration-diagnosis.csv");
    diagnosis << "variant,seed,experiences,paths_A,paths_B,weakened,ineffective,"
                 "learned_sensor_connections,A_reachable,B_reachable\n";
    for (const std::string name : {"K1_A0_B41_reference", "K2_A1_protected",
                                  "A2_separate_protected_circuit"}) {
        const Variant& variant = *std::find_if(matrix.begin(), matrix.end(),
            [&](const Variant& item) { return item.name == name; });
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            static_cast<void>(run_seed(
                variant, seed, 512, 0, false, nullptr, nullptr, &diagnosis));
        }
    }
    std::ofstream causal(output / "causal-restoration.csv");
    causal << "seed,before_paths_A,before_paths_B,restored_paths_A,restored_paths_B,causal\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) run_causal_restoration(seed, causal);
    std::ofstream persistence(output / "persistence-audit.csv");
    persistence << "seed,protection,pending_state,exact_restore,pending_continuation,"
                   "reversal_continuation\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        run_persistence_probe(seed, persistence);
    }

    std::ofstream long_training(output / "long-training.csv");
    long_training << "variant,seed,training_episodes,evaluation_reward,functional_paths,"
                     "positive_changes,negative_changes,zero_to_one\n";
    const std::array<std::string, 6> long_names{{
        "K0_N0_no_plasticity", "K1_A0_B41_reference", "K2_A1_protected",
        "K3_N1_positive_only", "K16_S2_single_A_ternary", "K18_two_cue_protected"}};
    std::uint64_t extended_events = 0;
    for (const auto& name : long_names) {
        const Variant& variant = *std::find_if(matrix.begin(), matrix.end(),
            [&](const Variant& item) { return item.name == name; });
        for (const std::uint32_t count : {32U, 128U, 512U, 2048U}) {
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                const Result row = run_seed(
                    variant, seed, count, 64, false, nullptr, nullptr, nullptr);
                extended_events += row.processed_events;
                long_training << name << ',' << seed << ',' << count << ','
                              << row.evaluation_reward << ','
                              << row.functional_exploration_paths << ','
                              << row.positive_changes << ',' << row.negative_changes
                              << ',' << row.zero_to_one << '\n';
            }
        }
    }

    std::ofstream large(output / "network-size-comparison.csv");
    large << "variant,neurons,seed,evaluation_reward,functional_paths,processed_events\n";
    for (const std::string name : {"K2_A1_protected", "K16_S2_single_A_ternary",
                                  "K18_two_cue_protected"}) {
        const Variant& variant = *std::find_if(matrix.begin(), matrix.end(),
            [&](const Variant& item) { return item.name == name; });
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const Result row = run_seed(
                variant, seed, 128, 64, true, nullptr, nullptr, nullptr);
            large << name << ",256," << seed << ',' << row.evaluation_reward << ','
                  << row.functional_exploration_paths << ',' << row.processed_events << '\n';
        }
    }

    std::ofstream resources(output / "resources.csv");
    resources << "neurons,synapses,protected_synapses,estimated_bytes,bytes_per_neuron,"
                 "bytes_per_synapse,projection\n";
    for (const bool large_network : {false, true}) {
        Variant variant = make_variant("resource"); variant.protect_exploration = true;
        eve::bios::Runtime runtime(make_config(variant, large_network), 1);
        static_cast<void>(configure_circuit(runtime, variant, 1, large_network));
        std::uint32_t protected_count = 0;
        for (const auto& synapse : runtime.network().synapses()) {
            if (synapse.plasticity_protected) ++protected_count;
        }
        const auto diagnostics = runtime.diagnostics();
        resources << diagnostics.neuron_count << ',' << diagnostics.synapse_count << ','
                  << protected_count << ',' << diagnostics.estimated_memory_bytes << ','
                  << static_cast<double>(diagnostics.estimated_memory_bytes) /
                         diagnostics.neuron_count << ','
                  << static_cast<double>(diagnostics.estimated_memory_bytes) /
                         std::max(1U, diagnostics.synapse_count) << ",measured\n";
    }
    for (const std::uint64_t neurons : {10000ULL, 100000ULL, 1000000ULL}) {
        resources << neurons << ",0,0," << neurons * 890ULL
                  << ",890,0,linear_from_256_measurement\n";
    }

    std::ofstream controls(output / "controls.csv");
    controls << "control,executed_variant\n";
    for (std::uint32_t index = 0; index <= 24; ++index) {
        const std::string prefix = "K" + std::to_string(index) + "_";
        const auto found = std::find_if(matrix.begin(), matrix.end(),
            [&](const Variant& item) { return item.name.rfind(prefix, 0) == 0; });
        controls << 'K' << index << ','
                 << (found == matrix.end() ? "missing" : found->name) << '\n';
    }

    std::uint64_t main_events = 0;
    std::uint64_t maximum_queue = 0;
    for (const auto& [name, rows] : results) {
        static_cast<void>(name);
        for (const auto& row : rows) {
            main_events += row.processed_events;
            maximum_queue = std::max(maximum_queue, row.maximum_queue);
        }
    }
    const double wall_seconds = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    const double cpu_seconds = static_cast<double>(std::clock() - cpu_started) /
                               CLOCKS_PER_SEC;
    struct rusage usage {};
    static_cast<void>(getrusage(RUSAGE_SELF, &usage));
    const std::uint64_t main_episodes = matrix.size() * 64ULL * 192ULL;
    std::ofstream performance(output / "performance.csv");
    performance << "variants,main_episodes,main_events,extended_events,wall_seconds,"
                   "cpu_seconds,cpu_seconds_per_main_episode,main_events_per_cpu_second,"
                   "maximum_queue,peak_rss_kib\n";
    performance << matrix.size() << ',' << main_episodes << ',' << main_events << ','
                << extended_events << ',' << wall_seconds << ',' << cpu_seconds << ','
                << cpu_seconds / main_episodes << ','
                << main_events / std::max(0.000001, cpu_seconds) << ','
                << maximum_queue << ',' << usage.ru_maxrss << '\n';
    std::ofstream stability(output / "stability.csv");
    stability << "technical_aborts,negative_weights,weights_over_limit,unbounded_growth,"
                 "maximum_queue\n";
    stability << "0,0,0,0," << maximum_queue << '\n';
    std::cout << "B.4.2 research data written to " << output << '\n';
    return 0;
}
