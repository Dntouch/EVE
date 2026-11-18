#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr std::array<std::uint64_t, 64> kSeeds{{
    20261118, 7, 42, 99, 2026, 314159, 1, 2, 3, 5, 11, 13, 17, 19, 23, 29,
    31, 37, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
    107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
    181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
    263, 269, 271, 277,
}};
constexpr std::array<std::uint64_t, 6> kEstablished{{20261118, 7, 42, 99, 2026, 314159}};
constexpr std::uint64_t kMotorSeed = 0x4233312d4d4f544fULL;
constexpr std::uint64_t kTopologySalt = 0x4233312d544f504fULL;
constexpr std::uint64_t kCircuitSalt = 0x4233312d43495243ULL;
constexpr std::uint64_t kExploreSalt = 0x4233312d4558504cULL;
constexpr std::uint64_t kFeedbackSalt = 0x4233312d46454544ULL;
constexpr std::uint64_t kContextSalt = 0x4233312d434f4e54ULL;

struct Setup {
    eve::brainstem::MotorCircuitResult circuit;
    eve::brainstem::EvaluationRouteResult feedback;
    eve::brainstem::ActionContextRoute context;
};

struct Episode {
    std::uint64_t technical_id{};
    std::string stimulus;
    std::uint8_t motor_value{};
    bool completed{};
    std::set<std::uint32_t> signatures;
};

struct Assignment {
    std::string quality{"unassigned"};
    std::set<std::uint32_t> matched_signatures;
    std::uint64_t local_unique{};
    std::uint64_t local_ambiguous{};
    std::uint64_t local_missing{};
    std::uint64_t local_expired{};
    std::uint64_t feedback_origin{};
};

const char* context_name(const eve::brainstem::ActionContextMode mode) {
    switch (mode) {
    case eve::brainstem::ActionContextMode::none: return "C0";
    case eve::brainstem::ActionContextMode::boundary: return "C1";
    case eve::brainstem::ActionContextMode::motor_copy: return "C2";
    case eve::brainstem::ActionContextMode::general_circuit: return "C3";
    case eve::brainstem::ActionContextMode::combined: return "C4";
    }
    return "C?";
}

const char* trace_name(const eve::ContextTraceMode mode) {
    switch (mode) {
    case eve::ContextTraceMode::none: return "P0";
    case eve::ContextTraceMode::single: return "P1";
    case eve::ContextTraceMode::multiple: return "P2";
    }
    return "P?";
}

const char* binding_name(const eve::ContextBindingMode mode) {
    switch (mode) {
    case eve::ContextBindingMode::none: return "B0";
    case eve::ContextBindingMode::at_fire: return "B1";
    case eve::ContextBindingMode::temporal_overlap: return "B2";
    }
    return "B?";
}

eve::ExperimentConfig experiment_config(
    const eve::ContextTraceMode trace_mode,
    const eve::ContextBindingMode binding_mode,
    const eve::EligibilitySelection selection = eve::EligibilitySelection::positive_weight,
    const eve::RecoveryMode recovery =
        eve::RecoveryMode::independent_with_membrane_relaxation) {
    eve::ExperimentConfig config;
    config.initial_synapse_weight = 0;
    config.maximum_synapse_weight = 3;
    config.firing_threshold = 3;
    config.refractory_input_events = 16;
    config.recovery_mode = recovery;
    config.recovery_delay = 16;
    config.membrane_relaxation_percent =
        recovery == eve::RecoveryMode::independent_with_membrane_relaxation ? 100 : 0;
    config.local_event_memory_capacity = 8;
    config.eligibility_selection = selection;
    config.eligibility_candidate_capacity = selection == eve::EligibilitySelection::none ? 0 : 2;
    config.eligibility_decision_capacity = selection == eve::EligibilitySelection::none ? 0 : 2;
    config.eligibility_duration = selection == eve::EligibilitySelection::none ? 0 : 64;
    config.context_trace_mode = trace_mode;
    config.context_binding_mode = trace_mode == eve::ContextTraceMode::none
                                      ? eve::ContextBindingMode::none : binding_mode;
    config.context_trace_capacity = trace_mode == eve::ContextTraceMode::single ? 1U :
                                    trace_mode == eve::ContextTraceMode::multiple ? 2U : 0U;
    config.context_trace_duration = trace_mode == eve::ContextTraceMode::none ? 0 : 64;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.maximum_events_per_run = 200'000;
    return config;
}

std::vector<eve::NeuronId> reserved_neurons(
    const eve::bios::Runtime& runtime,
    const eve::brainstem::MotorCircuitResult& circuit,
    const eve::brainstem::EvaluationRouteResult& feedback) {
    std::vector<eve::NeuronId> reserved;
    for (eve::NeuronId neuron = 0; neuron < 14; ++neuron) reserved.push_back(neuron);
    for (const auto& channel : runtime.motor_assignment().channels) {
        reserved.insert(reserved.end(), channel.begin(), channel.end());
    }
    reserved.push_back(runtime.motor_assignment().completion_contact);
    reserved.insert(reserved.end(), circuit.relay_neurons.begin(), circuit.relay_neurons.end());
    reserved.insert(reserved.end(), feedback.input_neurons.begin(), feedback.input_neurons.end());
    reserved.insert(reserved.end(), feedback.relay_neurons.begin(), feedback.relay_neurons.end());
    reserved.insert(reserved.end(), feedback.target_neurons.begin(), feedback.target_neurons.end());
    std::sort(reserved.begin(), reserved.end());
    reserved.erase(std::unique(reserved.begin(), reserved.end()), reserved.end());
    return reserved;
}

eve::NeuronId find_context_inputs(
    const std::uint32_t neuron_count, const std::vector<eve::NeuronId>& reserved) {
    std::vector<bool> used(neuron_count, false);
    for (const auto neuron : reserved) if (neuron < neuron_count) used[neuron] = true;
    for (eve::NeuronId first = 14; first + 10 <= neuron_count; ++first) {
        bool free = true;
        for (eve::NeuronId offset = 0; offset < 10; ++offset) free = free && !used[first + offset];
        if (free) return first;
    }
    throw std::runtime_error("no disjoint input range for neuronal action context");
}

Setup configure(
    eve::bios::Runtime& runtime,
    const std::uint64_t seed,
    const eve::brainstem::ActionContextMode context_mode,
    const eve::brainstem::EvaluationRouteMode feedback_mode =
        eve::brainstem::EvaluationRouteMode::relay_wiring) {
    eve::brainstem::MotorOutputConfig motor;
    motor.assignment_seed = kMotorSeed;
    motor.response_stimulus_neuron = 8;
    motor.first_motor_neuron = 42;
    runtime.configure_motor_output(motor);
    eve::brainstem::InnateWiringConfig innate;
    innate.topology = eve::brainstem::InnateTopology::functional_paths;
    innate.initial_weight = 3;
    innate.topology_seed = seed ^ kTopologySalt;
    static_cast<void>(runtime.configure_innate_wiring(innate));
    eve::brainstem::MotorCircuitConfig motor_circuit;
    motor_circuit.mode = eve::brainstem::MotorCircuitMode::activity_dependent;
    motor_circuit.motor_weight = 1;
    motor_circuit.completion_weight = 3;
    motor_circuit.relay_count = 1;
    motor_circuit.seed = seed ^ kCircuitSalt;
    Setup result;
    result.circuit = runtime.configure_motor_circuit(motor_circuit);

    eve::brainstem::EvaluationRouteConfig feedback;
    feedback.mode = feedback_mode;
    feedback.first_input_neuron = 9;
    feedback.graded_channel_count = 5;
    feedback.relay_count = 4;
    feedback.fanout_per_source = 4;
    feedback.weight = 3;
    feedback.seed = seed ^ kFeedbackSalt;
    feedback.reserved_neurons = runtime.exploration_reservations();
    result.feedback = runtime.configure_evaluation_route(feedback);

    eve::brainstem::ActionContextConfig context;
    context.mode = context_mode;
    context.relay_count = 4;
    context.fanout_per_source = 4;
    context.weight = 3;
    context.seed = seed ^ kContextSalt;
    context.reserved_neurons = reserved_neurons(runtime, result.circuit, result.feedback);
    context.first_input_neuron = find_context_inputs(
        runtime.network().neuron_count(), context.reserved_neurons);
    result.context = runtime.configure_action_context(context);
    return result;
}

eve::brainstem::ExplorationConfig exploration(
    const std::uint64_t seed, const std::uint64_t technical_id) {
    eve::brainstem::ExplorationConfig config;
    config.mode = eve::brainstem::ExplorationMode::distributed;
    config.target_policy = eve::brainstem::ExplorationTargetPolicy::processing_neurons_only;
    config.activation_count = 16;
    config.activation_budget = 16;
    config.seed = (seed ^ kExploreSalt) + technical_id;
    config.center = {4, 4, 2};
    config.radius = 2;
    return config;
}

void remember_signature(Episode& episode, const eve::bios::ActionContextEmission& emission) {
    if (emission.signature != 0) episode.signatures.insert(emission.signature);
}

Episode run_episode(
    eve::bios::Runtime& runtime,
    const std::uint64_t seed,
    const std::uint64_t technical_id,
    const std::string& stimulus,
    const std::uint64_t context_delay = 0,
    const bool feedback_origin_context = false) {
    Episode episode;
    episode.technical_id = technical_id;
    episode.stimulus = stimulus;
    const std::uint64_t due = context_delay == 0 ? 0 : runtime.simulated_time() + context_delay;
    remember_signature(episode, runtime.process_action_context(
        eve::brainstem::ActionContextSignal::start, 0, due, feedback_origin_context));
    static_cast<void>(runtime.process_utf8_text(stimulus));
    static_cast<void>(runtime.enqueue_exploration(exploration(seed, technical_id)));
    const auto response = runtime.request_response(true, 200'000);
    static_cast<void>(runtime.run());
    episode.motor_value = response.value;
    episode.completed = response.valid();
    remember_signature(episode, runtime.process_action_context(
        eve::brainstem::ActionContextSignal::motor_copy, response.value, due,
        feedback_origin_context));
    remember_signature(episode, runtime.process_action_context(
        eve::brainstem::ActionContextSignal::end, 0, due, feedback_origin_context));
    return episode;
}

Assignment assign_feedback(
    eve::bios::Runtime& runtime,
    const Episode& intended,
    const std::vector<Episode>& possible,
    const std::uint8_t score,
    const std::uint64_t delay = 0,
    const bool emit = true) {
    Assignment result;
    if (!emit) return result;
    std::set<std::uint32_t> observed;
    runtime.set_impulse_observer([&](const eve::bios::ImpulseObservation& item) {
        if (!item.feedback_context) return;
        const auto traces = runtime.network().context_traces(item.receiver);
        for (const auto& eligibility : runtime.network().eligibility_records(item.receiver)) {
            if (eligibility.feedback_origin || eligibility.context_signature == 0) continue;
            const bool active = std::any_of(traces.begin(), traces.end(), [&](const auto& trace) {
                return trace.signature == eligibility.context_signature;
            });
            if (active) observed.insert(eligibility.context_signature);
        }
    });
    const auto before = runtime.metrics();
    const std::uint64_t due = delay == 0 ? 0 : runtime.simulated_time() + delay;
    static_cast<void>(runtime.process_evaluation_feedback(
        score, eve::brainstem::EvaluationSignalKind::graded, due));
    if (delay != 0) static_cast<void>(runtime.advance_time_to(due));
    runtime.set_impulse_observer({});
    result.matched_signatures = observed;
    result.local_unique = runtime.metrics().feedback_context_unique_matches -
                          before.feedback_context_unique_matches;
    result.local_ambiguous = runtime.metrics().feedback_context_ambiguous_matches -
                             before.feedback_context_ambiguous_matches;
    result.local_missing = runtime.metrics().feedback_context_missing_matches -
                           before.feedback_context_missing_matches;
    result.local_expired = runtime.metrics().feedback_context_expired_matches -
                           before.feedback_context_expired_matches;
    result.feedback_origin = runtime.metrics().feedback_origin_context_matches -
                             before.feedback_origin_context_matches;
    std::set<std::uint64_t> candidates;
    for (const auto& episode : possible) {
        for (const auto signature : observed) {
            if (episode.signatures.count(signature) != 0) candidates.insert(episode.technical_id);
        }
    }
    if (candidates.empty()) result.quality = "unassigned";
    else if (candidates.size() > 1) result.quality = "ambiguous";
    else if (*candidates.begin() == intended.technical_id) result.quality = "correct";
    else result.quality = "false";
    return result;
}

std::uint8_t score_for(const Episode& episode, const std::uint8_t target) {
    const auto evaluation = eve::brainstem::Brainstem::evaluate_binary(
        episode.motor_value, target);
    return static_cast<std::uint8_t>(
        std::min(100U, ((evaluation.success_percent + 12U) / 25U) * 25U));
}

void write_parameters(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "parameter,value\nbase_branch,codex/research-stage-b3-neural-feedback-routing\n"
           "neurons,256\nregular_synapses,1024\ninnate,V2:3\nmotor,M3:1:3\n"
           "threshold,3\nmemory,G3:8\nrecovery,R2F:16\n"
           "eligibility,L2:2:2:64\ncontext,C0|C1|C2|C3|C4\n"
           "traces,P0|P1:1:64|P2:2:64\nbinding,B0|B1|B2\n"
           "feedback,F2|F3\nlearning,disabled\ngrowth,disabled\nseeds,64\n"
           "technical_action_id,external_truth_only\n";
}

void run_context_matrix(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,context,trace,binding,completed,motor,signatures,traces,bindings,ambiguities,"
           "route_synapses,active_context,active_eligibility,correct,false,ambiguous,unassigned,"
           "local_unique,local_ambiguous,local_missing,local_expired,weights_changed\n";
    for (const auto seed : kSeeds) {
        for (const auto mode : {eve::brainstem::ActionContextMode::none,
                                eve::brainstem::ActionContextMode::boundary,
                                eve::brainstem::ActionContextMode::motor_copy,
                                eve::brainstem::ActionContextMode::general_circuit,
                                eve::brainstem::ActionContextMode::combined}) {
            const std::array<std::pair<eve::ContextTraceMode, eve::ContextBindingMode>, 4> variants{{
                {mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextTraceMode::none : eve::ContextTraceMode::single,
                 mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextBindingMode::none : eve::ContextBindingMode::none},
                {mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextTraceMode::none : eve::ContextTraceMode::single,
                 mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextBindingMode::none : eve::ContextBindingMode::at_fire},
                {mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextTraceMode::none : eve::ContextTraceMode::multiple,
                 mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextBindingMode::none : eve::ContextBindingMode::at_fire},
                {mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextTraceMode::none : eve::ContextTraceMode::multiple,
                 mode == eve::brainstem::ActionContextMode::none
                     ? eve::ContextBindingMode::none : eve::ContextBindingMode::temporal_overlap},
            }};
            std::set<std::pair<int, int>> seen;
            for (const auto [trace_mode, binding_mode] : variants) {
                if (!seen.insert({static_cast<int>(trace_mode), static_cast<int>(binding_mode)}).second) continue;
                eve::bios::Runtime runtime(experiment_config(trace_mode, binding_mode), seed);
                const auto setup = configure(runtime, seed, mode);
                const auto weights = runtime.network().weight_distribution();
                const auto before = runtime.metrics();
                const auto episode = run_episode(runtime, seed, 1, "AB");
                const auto assignment = assign_feedback(
                    runtime, episode, {episode}, score_for(episode, 0x41));
                out << seed << ',' << context_name(mode) << ',' << trace_name(trace_mode) << ','
                    << binding_name(binding_mode) << ',' << (episode.completed ? 1 : 0) << ','
                    << unsigned(episode.motor_value) << ',' << episode.signatures.size() << ','
                    << runtime.metrics().context_traces_created - before.context_traces_created << ','
                    << runtime.metrics().eligibility_context_bindings - before.eligibility_context_bindings << ','
                    << runtime.metrics().eligibility_context_ambiguities - before.eligibility_context_ambiguities << ','
                    << setup.context.route_synapses.size() << ','
                    << runtime.network().active_context_trace_count() << ','
                    << runtime.network().active_eligibility_count() << ','
                    << (assignment.quality == "correct") << ','
                    << (assignment.quality == "false") << ','
                    << (assignment.quality == "ambiguous") << ','
                    << (assignment.quality == "unassigned") << ','
                    << assignment.local_unique << ',' << assignment.local_ambiguous << ','
                    << assignment.local_missing << ',' << assignment.local_expired << ','
                    << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
            }
        }
    }
}

struct SequenceCase { const char* name; const char* first; const char* second; };
constexpr std::array<SequenceCase, 6> kSequences{{
    {"AB-AB", "AB", "AB"}, {"AB-BA", "AB", "BA"}, {"AB-AA", "AB", "AA"},
    {"AA-AA", "AA", "AA"}, {"BB-BB", "BB", "BB"}, {"alternating", "AB", "BA"},
}};

void run_sequences(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,sequence,feedback_case,first_motor,second_motor,first_signatures,second_signatures,"
           "intended,quality,matched_signatures,local_unique,local_ambiguous,local_missing,"
           "local_expired,feedback_origin,weights_changed\n";
    for (const auto seed : kSeeds) {
        for (const auto& sequence : kSequences) {
            for (int feedback_case = 0; feedback_case <= 6; ++feedback_case) {
                eve::bios::Runtime runtime(experiment_config(
                    eve::ContextTraceMode::multiple, eve::ContextBindingMode::at_fire), seed);
                static_cast<void>(configure(runtime, seed,
                    eve::brainstem::ActionContextMode::combined));
                const auto weights = runtime.network().weight_distribution();
                const auto first = run_episode(runtime, seed, 1, sequence.first);
                const auto second = run_episode(runtime, seed, 2, sequence.second);
                const std::vector<Episode> possible{first, second};
                const Episode& intended = feedback_case == 3 ? second : first;
                const bool emit = feedback_case != 4;
                const std::uint64_t delay = feedback_case == 0 ? 0 :
                                            feedback_case == 1 ? 16 :
                                            feedback_case == 2 ? 8 :
                                            feedback_case == 3 ? 4 : 0;
                std::uint8_t score = score_for(intended, feedback_case == 6 ? 0x42 : 0x41);
                if (feedback_case == 5) score = 50;
                if (feedback_case == 6) score = intended.technical_id == 1 ? 25 : 75;
                const auto assignment = assign_feedback(
                    runtime, intended, possible, score, delay, emit);
                out << seed << ',' << sequence.name << ",R" << feedback_case << ','
                    << unsigned(first.motor_value) << ',' << unsigned(second.motor_value) << ','
                    << first.signatures.size() << ',' << second.signatures.size() << ','
                    << intended.technical_id << ',' << assignment.quality << ','
                    << assignment.matched_signatures.size() << ',' << assignment.local_unique << ','
                    << assignment.local_ambiguous << ',' << assignment.local_missing << ','
                    << assignment.local_expired << ',' << assignment.feedback_origin << ','
                    << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
            }
        }
    }
}

void run_controls(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,control,subvariant,actions,context,trace,binding,feedback,quality,traces,bindings,"
           "feedback_origin,active_context,active_eligibility,weights_changed\n";
    for (const auto seed : kSeeds) {
        for (int control = 0; control <= 15; ++control) {
            const bool all_off = control == 15;
            const auto trace_mode = (control == 0 || all_off)
                ? eve::ContextTraceMode::none : eve::ContextTraceMode::multiple;
            const auto binding = (control == 0 || control == 1 || all_off)
                ? eve::ContextBindingMode::none : eve::ContextBindingMode::at_fire;
            const auto selection = all_off ? eve::EligibilitySelection::none :
                control == 10 && seed % 2 == 0 ? eve::EligibilitySelection::all_recent :
                eve::EligibilitySelection::positive_weight;
            const auto recovery = control == 14 && seed % 2 == 0
                ? eve::RecoveryMode::independent
                : eve::RecoveryMode::independent_with_membrane_relaxation;
            eve::bios::Runtime runtime(experiment_config(
                trace_mode, binding, selection, recovery), seed);
            const auto context_mode = (control == 0 || all_off)
                ? eve::brainstem::ActionContextMode::none :
                control == 12 ? eve::brainstem::ActionContextMode::motor_copy :
                eve::brainstem::ActionContextMode::combined;
            const auto feedback_mode = (control == 2 || all_off)
                ? eve::brainstem::EvaluationRouteMode::none
                : eve::brainstem::EvaluationRouteMode::relay_wiring;
            const auto setup = configure(runtime, seed, context_mode, feedback_mode);
            if (control == 8 && !setup.circuit.circuit_synapses.empty()) {
                runtime.set_experimental_synapse_weight(setup.circuit.circuit_synapses.front(), 0);
            }
            const auto weights = runtime.network().weight_distribution();
            const auto before = runtime.metrics();
            std::vector<Episode> episodes;
            if (control != 3) {
                episodes.push_back(run_episode(runtime, seed, 1, "AB",
                    control == 13 ? 16 : 0, control == 11));
            }
            if (control == 5 || control == 6 || control == 7 || control == 9) {
                episodes.push_back(run_episode(runtime, seed, 2,
                    control == 6 ? "AB" : "BA"));
            }
            if (control == 4) static_cast<void>(runtime.advance_time_by(64));
            Episode placeholder;
            placeholder.technical_id = 99;
            const Episode& intended = episodes.empty() ? placeholder :
                control == 5 && episodes.size() > 1 ? episodes[1] : episodes[0];
            const auto assignment = assign_feedback(runtime, intended, episodes,
                control == 6 || control == 7 ? 50 : 75, 0, feedback_mode !=
                    eve::brainstem::EvaluationRouteMode::none);
            out << seed << ",K" << control << ','
                << (control == 10 ? (selection == eve::EligibilitySelection::all_recent ? "L1" : "L2") :
                    control == 14 ? (recovery == eve::RecoveryMode::independent ? "R1" : "R2F") : "base")
                << ',' << episodes.size() << ',' << context_name(context_mode) << ','
                << trace_name(trace_mode) << ',' << binding_name(binding) << ','
                << (feedback_mode == eve::brainstem::EvaluationRouteMode::none ? "off" : "F3") << ','
                << assignment.quality << ','
                << runtime.metrics().context_traces_created - before.context_traces_created << ','
                << runtime.metrics().eligibility_context_bindings - before.eligibility_context_bindings << ','
                << assignment.feedback_origin << ','
                << runtime.network().active_context_trace_count() << ','
                << runtime.network().active_eligibility_count() << ','
                << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
        }
    }
}

void run_continuous(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,repetitions,phase,stimulus,motor,signatures,quality,active_context,"
           "active_eligibility,overwrites,stale_expiry,pending,weights_changed\n";
    for (const auto seed : kEstablished) {
        for (const std::uint32_t repetitions : {2U, 8U, 32U, 128U}) {
            eve::bios::Runtime runtime(experiment_config(
                eve::ContextTraceMode::multiple, eve::ContextBindingMode::at_fire), seed);
            static_cast<void>(configure(runtime, seed,
                eve::brainstem::ActionContextMode::combined));
            const auto weights = runtime.network().weight_distribution();
            for (std::uint32_t phase = 1; phase <= repetitions; ++phase) {
                const std::string stimulus = phase % 2 == 0 ? "BA" : "AB";
                const auto before = runtime.metrics();
                const auto episode = run_episode(runtime, seed, phase, stimulus);
                const auto assignment = assign_feedback(
                    runtime, episode, {episode}, score_for(episode, 0x41), phase % 4 == 0 ? 8 : 0);
                static_cast<void>(runtime.advance_time_by(8));
                out << seed << ',' << repetitions << ',' << phase << ',' << stimulus << ','
                    << unsigned(episode.motor_value) << ',' << episode.signatures.size() << ','
                    << assignment.quality << ',' << runtime.network().active_context_trace_count() << ','
                    << runtime.network().active_eligibility_count() << ','
                    << runtime.metrics().context_traces_overwritten - before.context_traces_overwritten << ','
                    << runtime.metrics().stale_context_expiries - before.stale_context_expiries << ','
                    << runtime.future_event_count() << ','
                    << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
            }
        }
    }
}

void run_persistence(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "seed,active_traces,pending,pending_equal,continuation_equal,processed_expiry,"
           "stale_expiry,weights_changed\n";
    for (const auto seed : kSeeds) {
        eve::bios::Runtime runtime(experiment_config(
            eve::ContextTraceMode::multiple, eve::ContextBindingMode::at_fire), seed);
        static_cast<void>(configure(runtime, seed,
            eve::brainstem::ActionContextMode::combined));
        static_cast<void>(run_episode(runtime, seed, 1, "AB"));
        const auto weights = runtime.network().weight_distribution();
        const auto temp = std::filesystem::temp_directory_path() /
            ("eve-b31-" + std::to_string(seed) + ".bin");
        runtime.save(temp.string());
        auto restored = eve::bios::Runtime::load(temp.string());
        const bool pending_equal = runtime.state_fingerprint() == restored.state_fingerprint();
        static_cast<void>(runtime.advance_time_by(64));
        static_cast<void>(restored.advance_time_by(64));
        out << seed << ',' << runtime.network().active_context_trace_count() << ','
            << restored.future_event_count() << ',' << (pending_equal ? 1 : 0) << ','
            << (runtime.state_fingerprint() == restored.state_fingerprint() ? 1 : 0) << ','
            << runtime.metrics().processed_context_expiries << ','
            << runtime.metrics().stale_context_expiries << ','
            << (runtime.network().weight_distribution() == weights ? 0 : 1) << '\n';
        static_cast<void>(std::filesystem::remove(temp));
    }
}

void run_performance(const std::filesystem::path& path) {
    std::ofstream out(path);
    out << "neurons,measured,trace_capacity,context,route_synapses,events,maximum_queue,"
           "memory_bytes,seconds,events_per_second,estimated_context_bytes\n";
    for (const auto mode : {eve::brainstem::ActionContextMode::none,
                            eve::brainstem::ActionContextMode::boundary,
                            eve::brainstem::ActionContextMode::motor_copy,
                            eve::brainstem::ActionContextMode::general_circuit,
                            eve::brainstem::ActionContextMode::combined}) {
        const auto trace_mode = mode == eve::brainstem::ActionContextMode::none
            ? eve::ContextTraceMode::none : eve::ContextTraceMode::multiple;
        eve::bios::Runtime runtime(experiment_config(
            trace_mode, trace_mode == eve::ContextTraceMode::none
                            ? eve::ContextBindingMode::none : eve::ContextBindingMode::at_fire), 8802);
        const auto setup = configure(runtime, 8802, mode);
        const auto before = runtime.metrics().processed_events;
        const auto start = std::chrono::steady_clock::now();
        static_cast<void>(run_episode(runtime, 8802, 1, "AB"));
        const double seconds = std::chrono::duration<double>(
            std::chrono::steady_clock::now() - start).count();
        const auto events = runtime.metrics().processed_events - before;
        out << "256,1," << runtime.network().context_trace_capacity() << ','
            << context_name(mode) << ',' << setup.context.route_synapses.size() << ','
            << events << ',' << runtime.metrics().maximum_queue_size << ','
            << runtime.diagnostics().estimated_memory_bytes << ',' << std::setprecision(9)
            << seconds << ',' << (seconds == 0 ? 0 : events / seconds) << ','
            << runtime.network().neuron_count() * runtime.network().context_trace_capacity() *
                   sizeof(eve::neural::ContextTrace) << '\n';
    }
    for (const std::uint64_t neurons : {10000ULL, 100000ULL, 1000000ULL}) {
        out << neurons << ",0,2,C4,0,0,0,0,0,0,"
            << neurons * 2 * sizeof(eve::neural::ContextTrace) << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const std::filesystem::path directory = argc > 1 ? argv[1] : "research/results";
        std::filesystem::create_directories(directory);
        write_parameters(directory / "research-stage-b31-parameters.csv");
        run_context_matrix(directory / "research-stage-b31-contexts.csv");
        run_sequences(directory / "research-stage-b31-sequences.csv");
        run_controls(directory / "research-stage-b31-controls.csv");
        run_continuous(directory / "research-stage-b31-continuous.csv");
        run_persistence(directory / "research-stage-b31-persistence.csv");
        run_performance(directory / "research-stage-b31-performance.csv");
        std::cout << "wrote B.3.1 research data to " << directory << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "research stage B.3.1 failed: " << exception.what() << '\n';
        return 1;
    }
}
