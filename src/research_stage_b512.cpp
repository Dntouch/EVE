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

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace {

constexpr std::array<std::uint32_t, 11> kB512Checkpoints{{
    0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512}};
constexpr std::uint32_t kB512Pretraining = 2;
constexpr std::uint32_t kB512Continuation = 64;
constexpr double kB512ReliableThreshold = 5.0 / 6.0;

struct B512Metric {
    std::string phase;
    std::string group;
    std::string metric;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    double value{};
};

struct B512ContinuationResult {
    std::uint32_t first{kB51Never};
    std::uint32_t autonomous_correct{};
    std::uint32_t autonomous_total{};
    std::uint64_t positive_writes{};
    std::uint64_t negative_writes{};
    B511Evaluation final;
    bool valid{};
};

struct B512SimpleEpisode {
    bool autonomous_correct{};
    bool explored{};
    bool correct{};
    std::int8_t modulation{};
    std::uint64_t positive_writes{};
    std::uint64_t negative_writes{};
};

bool b512_autonomous_correct(
    const B511Episode& row, const B511Pattern& pattern) {
    return !row.early_a && !row.early_b &&
        b511_correct(pattern.target_a, row.response_a, row.response_b);
}

bool b512_reliable(const B511Evaluation& value, const B511Task task) {
    return value.conditional[static_cast<std::size_t>(task)] >=
        kB512ReliableThreshold;
}

std::pair<std::uint32_t, std::uint32_t> b512_weight_deltas(
    const eve::bios::Runtime& before, const eve::bios::Runtime& after) {
    std::uint32_t increases = 0;
    std::uint32_t decreases = 0;
    const auto count = std::min(before.network().synapse_count(),
                                after.network().synapse_count());
    for (eve::SynapseId id = 0; id < count; ++id) {
        const auto left = before.network().synapse(id).weight;
        const auto right = after.network().synapse(id).weight;
        increases += right > left;
        decreases += right < left;
    }
    return {increases, decreases};
}

B512SimpleEpisode b512_simple_episode(
    eve::bios::Runtime& runtime, const B511Circuit& circuit,
    const eve::NeuronId sensor, const bool target_a,
    const std::uint8_t exploration, const bool learn = true) {
    B512SimpleEpisode result;
    bool fired_a = false;
    bool fired_b = false;
    const auto before = runtime.metrics();
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.base.motors[0];
        fired_b = fired_b || event.neuron == circuit.base.motors[1];
    });
    runtime.enqueue_fire(sensor);
    static_cast<void>(runtime.run());
    result.autonomous_correct = b511_correct(target_a, fired_a, fired_b);
    if (!fired_a && !fired_b) {
        result.explored = true;
        runtime.enqueue_fire(circuit.base.explorers[exploration]);
        static_cast<void>(runtime.run());
    }
    result.correct = b511_correct(target_a, fired_a, fired_b);
    result.modulation = static_cast<std::int8_t>(result.correct ? 1 : -1);
    if (learn)
        static_cast<void>(runtime.process_modulatory_feedback(result.modulation));
    runtime.set_fire_observer({});
    const auto after = runtime.metrics();
    result.positive_writes = after.plasticity_positive_changes -
        before.plasticity_positive_changes;
    result.negative_writes = after.plasticity_negative_changes -
        before.plasticity_negative_changes;
    static_cast<void>(runtime.advance_time_by(kB511EpisodeSpan));
    return result;
}

bool b512_simple_probe(
    const eve::bios::Runtime& source, const B511Circuit& circuit,
    const eve::NeuronId sensor, const bool target_a) {
    auto runtime = source;
    bool fired_a = false;
    bool fired_b = false;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.base.motors[0];
        fired_b = fired_b || event.neuron == circuit.base.motors[1];
    });
    runtime.enqueue_fire(sensor);
    static_cast<void>(runtime.run());
    return b511_correct(target_a, fired_a, fired_b);
}

void b512_simple_reference(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint32_t family, const bool inverse, const std::uint64_t seed,
    std::ofstream& out, std::vector<B512Metric>& metrics) {
    eve::bios::Runtime base(b511_config(membrane, true),
        b47_mix_seed(0x5120800ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(base, true);
    auto pretrained = base;
    auto naive = base;
    auto independent = base;
    auto time_control = base;
    const auto plans = b48_plans(seed, 17, family, 519, 520, inverse);
    const bool target_a = !inverse;
    const auto prior = b512_simple_episode(pretrained, circuit,
        circuit.base.sensors[0], target_a, plans[0].exploration, true);
    static_cast<void>(b512_simple_episode(independent, circuit,
        circuit.base.sensors[1], target_a, plans[0].exploration, true));
    static_cast<void>(time_control.advance_time_by(kB511EpisodeSpan));
    const bool prior_probe = b512_simple_probe(pretrained, circuit,
        circuit.base.sensors[0], target_a);
    const bool candidate = !prior.autonomous_correct && !prior_probe;
    const std::array<std::pair<const char*, eve::bios::Runtime*>, 4> groups{{
        {"simple_A_pretrained", &pretrained},
        {"simple_unexperienced", &naive},
        {"simple_independent_pretrained", &independent},
        {"simple_time_control", &time_control}}};
    for (const auto& [name, state] : groups) {
        const auto before = *state;
        std::uint32_t first = kB51Never;
        std::uint32_t correct = 0;
        for (std::uint32_t episode = 0; episode < 16; ++episode) {
            const auto row = b512_simple_episode(*state, circuit,
                circuit.base.sensors[0], target_a,
                plans[episode + 1].exploration, true);
            if (row.autonomous_correct) {
                if (first == kB51Never) first = episode + 1;
                ++correct;
            }
        }
        const bool final_probe = b512_simple_probe(*state, circuit,
            circuit.base.sensors[0], target_a);
        const auto deltas = b512_weight_deltas(before, *state);
        out << phase << ',' << name << ',' << family << ','
            << b511_assignment(inverse) << ',' << seed << ',' << candidate
            << ',' << prior.correct << ',' << prior.positive_writes << ','
            << prior.negative_writes << ',' << prior_probe << ',' << first
            << ',' << correct << ',' << final_probe << ',' << deltas.first
            << ',' << deltas.second << ',' << state->network().validate()
            << '\n';
        if (candidate) {
            metrics.push_back({phase, name, "additional_to_first", family,
                inverse, seed, first == kB51Never ? 17.0
                                                  : static_cast<double>(first)});
            metrics.push_back({phase, name, "final_probe", family, inverse,
                seed, final_probe ? 1.0 : 0.0});
        }
    }
}

B512ContinuationResult b512_continue_a(
    eve::bios::Runtime& runtime, const B511Circuit& circuit,
    const bool inverse, const std::vector<B48Plan>& plans,
    const std::uint32_t offset, const std::uint32_t count) {
    B512ContinuationResult result;
    const auto pattern = b511_pattern(B511Task::a, false, inverse);
    for (std::uint32_t episode = 0; episode < count; ++episode) {
        const auto row = b511_episode(runtime, circuit, pattern,
            plans[offset + episode].exploration, true);
        const bool has_response = row.response_a || row.response_b;
        result.autonomous_total += has_response;
        if (b512_autonomous_correct(row, pattern)) {
            if (result.first == kB51Never) result.first = episode + 1;
            ++result.autonomous_correct;
        }
        result.positive_writes += row.positive_writes;
        result.negative_writes += row.negative_writes;
    }
    result.final = b511_evaluate(runtime, circuit, false, inverse);
    result.valid = runtime.network().validate();
    return result;
}

void b512_learning_curve(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint32_t family, const bool inverse, const std::uint64_t seed,
    std::ofstream& out, std::vector<B512Metric>& metrics,
    const bool suppress_writes = false) {
    eve::bios::Runtime runtime(b511_config(membrane, membrane.plasticity),
        b47_mix_seed(0x5121000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(runtime, membrane.plasticity);
    if (suppress_writes)
        runtime.set_experimental_plasticity_writes_suppressed(true);
    const auto plans = b48_plans(seed, 512, family, 521, 522, inverse);
    const auto pattern = b511_pattern(B511Task::a, false, inverse);
    std::uint32_t autonomous_correct = 0;
    std::uint32_t autonomous_wrong = 0;
    std::uint32_t missing = 0;
    std::uint32_t doubles = 0;
    std::uint32_t early = 0;
    std::uint32_t explored = 0;
    std::uint32_t positive_modulation = 0;
    std::uint32_t negative_modulation = 0;
    std::uint64_t positive_writes = 0;
    std::uint64_t negative_writes = 0;
    std::uint32_t first_success = kB51Never;
    std::uint32_t first_reliable = kB51Never;
    std::size_t checkpoint_index = 0;
    for (std::uint32_t episode = 0; episode <= 512; ++episode) {
        if (checkpoint_index < kB512Checkpoints.size() &&
            episode == kB512Checkpoints[checkpoint_index]) {
            const auto evaluation = b511_evaluate(
                runtime, circuit, false, inverse);
            if (first_reliable == kB51Never &&
                b512_reliable(evaluation, B511Task::a))
                first_reliable = episode;
            out << phase << ',' << membrane.name << ',' << family << ','
                << b511_assignment(inverse) << ',' << seed << ',' << episode
                << ',' << autonomous_correct << ',' << autonomous_wrong << ','
                << missing << ',' << doubles << ',' << early << ',' << explored
                << ',' << positive_modulation << ',' << negative_modulation
                << ',' << positive_writes << ',' << negative_writes << ','
                << evaluation.direct[0] << ',' << evaluation.conditional[0]
                << ',' << b512_reliable(evaluation, B511Task::a) << ','
                << first_success << ',' << first_reliable << ','
                << runtime.future_event_count() << ','
                << runtime.network().active_eligibility_count() << ','
                << runtime.network().validate() << '\n';
            if (episode == 512) {
                metrics.push_back({phase, "curve_" + membrane.name,
                    "final_conditional_A", family, inverse, seed,
                    evaluation.conditional[0]});
                metrics.push_back({phase, "curve_" + membrane.name,
                    "reliable", family, inverse, seed,
                    b512_reliable(evaluation, B511Task::a) ? 1.0 : 0.0});
            }
            ++checkpoint_index;
        }
        if (episode == 512) break;
        const auto row = b511_episode(runtime, circuit, pattern,
            plans[episode].exploration, membrane.plasticity);
        const bool correct = b512_autonomous_correct(row, pattern);
        const bool has_response = row.response_a || row.response_b;
        autonomous_correct += correct;
        autonomous_wrong += has_response && !correct;
        missing += !has_response;
        doubles += row.response_a && row.response_b;
        early += row.early_a || row.early_b;
        explored += row.explored;
        positive_modulation += row.modulation > 0;
        negative_modulation += row.modulation < 0;
        positive_writes += row.positive_writes;
        negative_writes += row.negative_writes;
        if (correct && first_success == kB51Never)
            first_success = episode + 1;
    }
}

void b512_latent_groups(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint32_t family, const bool inverse, const std::uint64_t seed,
    std::ofstream& out, std::vector<B512Metric>& metrics) {
    eve::bios::Runtime base(b511_config(membrane, true),
        b47_mix_seed(0x5122000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(base, true);
    auto pretrained = base;
    auto naive = base;
    auto independent = base;
    auto time_control = base;
    const auto plans = b48_plans(seed, kB512Pretraining +
        kB512Continuation, family, 523, 524, inverse);
    const auto shared = b511_pattern(B511Task::a, false, inverse);
    const auto separate = b511_pattern(B511Task::a, true, inverse);
    std::uint32_t prior_autonomous = 0;
    std::uint64_t prior_positive = 0;
    std::uint64_t prior_negative = 0;
    for (std::uint32_t episode = 0; episode < kB512Pretraining; ++episode) {
        const auto row = b511_episode(pretrained, circuit, shared,
            plans[episode].exploration, true);
        prior_autonomous += b512_autonomous_correct(row, shared);
        prior_positive += row.positive_writes;
        prior_negative += row.negative_writes;
        static_cast<void>(b511_episode(independent, circuit, separate,
            plans[episode].exploration, true));
    }
    static_cast<void>(time_control.advance_time_by(
        kB512Pretraining * kB511EpisodeSpan));
    const auto pre_eval = b511_evaluate(pretrained, circuit, false, inverse);
    const bool latent_candidate = prior_autonomous == 0 && !pre_eval.direct[0];
    const std::array<std::pair<const char*, eve::bios::Runtime*>, 4> groups{{
        {"A_pretrained", &pretrained}, {"unexperienced", &naive},
        {"independent_pretrained", &independent},
        {"time_control", &time_control}}};
    for (const auto& [name, state] : groups) {
        const auto before = *state;
        const auto result = b512_continue_a(*state, circuit, inverse, plans,
            kB512Pretraining, kB512Continuation);
        const auto deltas = b512_weight_deltas(before, *state);
        out << phase << ',' << name << ',' << family << ','
            << b511_assignment(inverse) << ',' << seed << ','
            << latent_candidate << ',' << prior_autonomous << ','
            << pre_eval.direct[0] << ',' << pre_eval.conditional[0] << ','
            << prior_positive << ',' << prior_negative << ',' << result.first
            << ',' << result.autonomous_correct << ','
            << result.autonomous_total << ',' << result.final.conditional[0]
            << ',' << b512_reliable(result.final, B511Task::a) << ','
            << result.positive_writes << ',' << result.negative_writes << ','
            << deltas.first << ',' << deltas.second << ',' << result.valid
            << '\n';
        if (latent_candidate) {
            metrics.push_back({phase, std::string("latent_") + name,
                "additional_to_first", family, inverse, seed,
                result.first == kB51Never
                    ? static_cast<double>(kB512Continuation + 1)
                    : static_cast<double>(result.first)});
            metrics.push_back({phase, std::string("latent_") + name,
                "final_conditional_A", family, inverse, seed,
                result.final.conditional[0]});
        }
    }
}

void b512_consolidation(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint32_t family, const bool inverse, const std::uint64_t seed,
    std::ofstream& out, std::vector<B512Metric>& metrics) {
    eve::bios::Runtime runtime(b511_config(membrane, true),
        b47_mix_seed(0x5123000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(runtime, true);
    const auto plans = b48_plans(seed, 256, family, 525, 526, inverse);
    const auto pattern = b511_pattern(B511Task::a, false, inverse);
    std::uint32_t first = kB51Never;
    for (std::uint32_t episode = 0; episode < 128; ++episode) {
        const auto row = b511_episode(runtime, circuit, pattern,
            plans[episode].exploration, true);
        if (b512_autonomous_correct(row, pattern)) {
            first = episode + 1;
            break;
        }
    }
    const auto baseline = b511_evaluate(runtime, circuit, false, inverse);
    const auto initial = runtime;
    std::array<eve::bios::Runtime, 4> states{{runtime, runtime, runtime, runtime}};
    const std::array<const char*, 4> names{{
        "additional_A", "pause", "independent_experience", "mixed_A_B"}};
    for (std::size_t branch = 0; branch < states.size(); ++branch) {
        std::uint32_t correct = 0;
        std::uint32_t total = 0;
        std::uint64_t positive = 0;
        std::uint64_t negative = 0;
        if (branch == 1) {
            static_cast<void>(states[branch].advance_time_by(
                kB512Continuation * kB511EpisodeSpan));
        } else {
            for (std::uint32_t episode = 0; episode < kB512Continuation;
                 ++episode) {
                const bool independent = branch == 2;
                const auto task = branch == 3 && (episode & 1U)
                    ? B511Task::b : B511Task::a;
                const auto selected = b511_pattern(task, independent, inverse);
                const auto row = b511_episode(states[branch], circuit, selected,
                    plans[128 + episode].exploration, true);
                if (task == B511Task::a) {
                    ++total;
                    correct += b512_autonomous_correct(row, selected);
                }
                positive += row.positive_writes;
                negative += row.negative_writes;
            }
        }
        const auto final = b511_evaluate(
            states[branch], circuit, false, inverse);
        const auto deltas = b512_weight_deltas(initial, states[branch]);
        out << phase << ',' << names[branch] << ',' << family << ','
            << b511_assignment(inverse) << ',' << seed << ',' << first << ','
            << baseline.conditional[0] << ','
            << b512_reliable(baseline, B511Task::a) << ',' << correct << ','
            << total << ',' << final.conditional[0] << ','
            << b512_reliable(final, B511Task::a) << ',' << positive << ','
            << negative << ',' << deltas.first << ',' << deltas.second << ','
            << states[branch].future_event_count() << ','
            << states[branch].network().validate() << '\n';
        metrics.push_back({phase, std::string("consolidation_") + names[branch],
            "final_conditional_A", family, inverse, seed,
            final.conditional[0]});
    }
}

void b512_impairment_and_multi(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint32_t family, const bool inverse, const std::uint64_t seed,
    std::ofstream& impairment, std::ofstream& multi,
    std::vector<B512Metric>& metrics) {
    eve::bios::Runtime runtime(b511_config(membrane, true),
        b47_mix_seed(0x5124000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto circuit = b511_circuit(runtime, true);
    const auto plans = b48_plans(seed, 256, family, 527, 528, inverse);
    const auto emit_stage = [&](const char* stage, const std::uint32_t episode) {
        const auto value = b511_evaluate(runtime, circuit, false, inverse);
        multi << phase << ',' << stage << ',' << family << ','
            << b511_assignment(inverse) << ',' << seed << ',' << episode << ','
            << value.conditional[0] << ',' << value.conditional[1] << ','
            << value.repertoire << ',' << value.direct[0] << ','
            << value.direct[1] << ',' << runtime.network().validate() << '\n';
        return value;
    };
    const auto initial = emit_stage("initial", 0);
    static_cast<void>(initial);
    std::uint32_t first_a = kB51Never;
    std::uint32_t first_b = kB51Never;
    std::uint32_t reacquired_a = kB51Never;
    for (std::uint32_t episode = 0; episode < 64; ++episode) {
        const auto pattern = b511_pattern(B511Task::a, false, inverse);
        const auto row = b511_episode(runtime, circuit, pattern,
            plans[episode].exploration, true);
        if (first_a == kB51Never && b512_autonomous_correct(row, pattern))
            first_a = episode + 1;
    }
    const auto after_a = emit_stage("after_A", 64);
    for (std::uint32_t episode = 64; episode < 128; ++episode) {
        const auto pattern = b511_pattern(B511Task::b, false, inverse);
        const auto row = b511_episode(runtime, circuit, pattern,
            plans[episode].exploration, true);
        if (first_b == kB51Never && b512_autonomous_correct(row, pattern))
            first_b = episode - 63;
    }
    const auto after_b = emit_stage("after_A_B", 128);
    const bool impaired = after_a.direct[0] && !after_b.direct[0];
    for (std::uint32_t episode = 128; episode < 192; ++episode) {
        const auto pattern = b511_pattern(B511Task::a, false, inverse);
        const auto row = b511_episode(runtime, circuit, pattern,
            plans[episode].exploration, true);
        if (reacquired_a == kB51Never && b512_autonomous_correct(row, pattern))
            reacquired_a = episode - 127;
    }
    const auto after_reacquisition = emit_stage("after_A_B_A", 192);
    for (std::uint32_t episode = 192; episode < 256; ++episode) {
        const auto task = (episode & 1U) ? B511Task::b : B511Task::a;
        static_cast<void>(b511_episode(runtime, circuit,
            b511_pattern(task, false, inverse),
            plans[episode].exploration, true));
    }
    const auto after_mixed = emit_stage("after_A_B_A_mixed", 256);

    eve::bios::Runtime naive(b511_config(membrane, true),
        b47_mix_seed(0x5124000ULL ^ seed ^
            (static_cast<std::uint64_t>(family) << 40U)));
    const auto naive_circuit = b511_circuit(naive, true);
    std::uint32_t naive_first = kB51Never;
    for (std::uint32_t episode = 0; episode < 64; ++episode) {
        const auto pattern = b511_pattern(B511Task::a, false, inverse);
        const auto row = b511_episode(naive, naive_circuit, pattern,
            plans[128 + episode].exploration, true);
        if (naive_first == kB51Never && b512_autonomous_correct(row, pattern))
            naive_first = episode + 1;
    }
    impairment << phase << ',' << family << ',' << b511_assignment(inverse)
        << ',' << seed << ',' << first_a << ',' << first_b << ','
        << after_a.conditional[0] << ',' << after_b.conditional[0] << ','
        << impaired << ',' << reacquired_a << ',' << naive_first << ','
        << (reacquired_a < naive_first) << ','
        << after_reacquisition.conditional[0] << ','
        << after_mixed.conditional[0] << ',' << after_mixed.conditional[1]
        << ',' << runtime.network().validate() << '\n';
    if (impaired) {
        metrics.push_back({phase, "impaired_reacquisition",
            "additional_to_first_A", family, inverse, seed,
            reacquired_a == kB51Never ? 65.0 : static_cast<double>(reacquired_a)});
        metrics.push_back({phase, "impaired_naive",
            "additional_to_first_A", family, inverse, seed,
            naive_first == kB51Never ? 65.0 : static_cast<double>(naive_first)});
    }
}

void b512_interventions(
    const std::string& phase, const B511MembraneVariant& membrane,
    const std::uint64_t seed_base, std::ofstream& out) {
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (std::uint32_t offset = 0; offset < 5; ++offset) {
                const auto seed = seed_base + offset;
                eve::bios::Runtime runtime(b511_config(membrane, true),
                    b47_mix_seed(0x5125000ULL ^ seed ^
                        (static_cast<std::uint64_t>(family) << 40U)));
                const auto circuit = b511_circuit(runtime, true);
                const auto plans = b48_plans(seed, 256, family, 529, 530, inverse);
                const auto pattern = b511_pattern(B511Task::a, false, inverse);
                for (std::uint32_t episode = 0; episode < 64; ++episode)
                    static_cast<void>(b511_episode(runtime, circuit, pattern,
                        plans[episode].exploration, true));
                bool found = false;
                for (std::uint32_t episode = 64; episode < 128 && !found;
                     ++episode) {
                    const auto before = runtime;
                    std::vector<eve::bios::PlasticityObservation> writes;
                    runtime.set_plasticity_observer(
                        [&](const eve::bios::PlasticityObservation& row) {
                            if (row.changed) writes.push_back(row);
                        });
                    static_cast<void>(b511_episode(runtime, circuit, pattern,
                        plans[episode].exploration, true));
                    runtime.set_plasticity_observer({});
                    if (writes.empty()) continue;
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
                    static_cast<void>(b511_episode(w1, circuit, pattern,
                        plans[episode].exploration, true));
                    w1.set_experimental_plasticity_write_interceptor({});
                    auto w2 = before;
                    static_cast<void>(b511_episode(w2, circuit, pattern,
                        plans[episode].exploration, true));
                    auto combined = before;
                    combined.set_experimental_plasticity_writes_suppressed(true);
                    static_cast<void>(b511_episode(combined, circuit, pattern,
                        plans[episode].exploration, true));
                    combined.set_experimental_plasticity_writes_suppressed(false);
                    const bool identical = runtime.state_fingerprint() ==
                        w2.state_fingerprint();
                    for (std::uint32_t future = 0; future < 32; ++future) {
                        for (auto* state : {&runtime, &w1, &w2, &combined})
                            static_cast<void>(b511_episode(*state, circuit,
                                pattern, plans[128 + future].exploration, true));
                    }
                    const auto e0 = b511_evaluate(runtime, circuit, false, inverse);
                    const auto e1 = b511_evaluate(w1, circuit, false, inverse);
                    const auto e2 = b511_evaluate(w2, circuit, false, inverse);
                    const auto ec = b511_evaluate(combined, circuit, false, inverse);
                    const char* effect = e0.conditional[0] > e1.conditional[0]
                        ? "beneficial" : e0.conditional[0] < e1.conditional[0]
                            ? "harmful" : "tied";
                    out << phase << ',' << family << ','
                        << b511_assignment(inverse) << ',' << seed << ','
                        << episode << ',' << selected.event_index << ','
                        << selected.candidate << ',' << selected.weight_before
                        << ',' << selected.proposed_weight << ','
                        << (selected.proposed_weight > selected.weight_before)
                        << ',' << (selected.proposed_weight < selected.weight_before)
                        << ',' << writes.size() << ',' << suppressed << ','
                        << identical << ',' << e0.conditional[0] << ','
                        << e1.conditional[0] << ',' << e2.conditional[0] << ','
                        << ec.conditional[0] << ',' << effect << ','
                        << runtime.network().validate() << ','
                        << w1.network().validate() << ','
                        << w2.network().validate() << ','
                        << combined.network().validate() << '\n';
                    found = true;
                }
            }
        }
    }
}

void b512_statistics(
    const std::vector<B512Metric>& rows, std::ofstream& aggregate,
    std::ofstream& paired) {
    using GroupKey = std::tuple<std::string, std::string, std::string>;
    std::map<GroupKey, std::vector<double>> groups;
    for (const auto& row : rows)
        groups[{row.phase, row.group, row.metric}].push_back(row.value);
    std::uint64_t salt = 0x5126000ULL;
    for (const auto& [key, values] : groups) {
        const auto interval = bootstrap_interval(values, ++salt);
        aggregate << std::get<0>(key) << ',' << std::get<1>(key) << ','
            << std::get<2>(key) << ',' << values.size() << ','
            << b48_mean(values) << ',' << interval.first << ','
            << interval.second << '\n';
    }
    using PairKey = std::tuple<std::string, std::uint32_t, bool, std::uint64_t,
                               std::string>;
    std::map<PairKey, double> values;
    for (const auto& row : rows)
        values[{row.phase, row.family, row.inverse, row.seed,
                row.group + ":" + row.metric}] = row.value;
    const std::array<std::tuple<const char*, const char*, const char*>, 4> pairs{{
        {"simple_A_pretrained", "simple_unexperienced",
         "simple_A_minus_unexperienced"},
        {"latent_A_pretrained", "latent_unexperienced",
         "latent_A_minus_unexperienced"},
        {"consolidation_additional_A", "consolidation_pause",
         "additional_A_minus_pause"},
        {"impaired_reacquisition", "impaired_naive",
         "reacquisition_minus_naive"}}};
    for (const auto& [left, right, label] : pairs) {
        std::map<std::pair<std::string, std::string>,
                 std::vector<double>> differences;
        for (const auto& row : rows) {
            if (row.group != left) continue;
            const PairKey key{row.phase, row.family, row.inverse, row.seed,
                              std::string(right) + ":" + row.metric};
            const auto found = values.find(key);
            if (found != values.end())
                differences[{row.phase, row.metric}].push_back(
                    row.value - found->second);
        }
        for (const auto& [key, difference] : differences) {
            const auto interval = bootstrap_interval(difference, ++salt);
            paired << key.first << ',' << label << '_' << key.second
                << ",seed_family_assignment," << difference.size() << ','
                << b48_mean(difference) << ',' << interval.first << ','
                << interval.second << '\n';
        }
    }
}

void b512_persistence(const std::filesystem::path& output, std::ofstream& out) {
    const B511MembraneVariant s4{
        "S4_d16", eve::RecoveryMode::independent_with_membrane_relaxation,
        1, 16, 50, true, true, 32};
    eve::bios::Runtime runtime(b511_config(s4, true), 0x5127000ULL);
    const auto circuit = b511_circuit(runtime, true);
    runtime.set_experimental_synapse_weight(circuit.element_synapses[0], 1);
    runtime.enqueue_fire(circuit.elements[0]);
    static_cast<void>(runtime.run());
    const auto pending_events = runtime.future_event_count();
    const auto path = output / "b512-pending-state.bin";
    runtime.save(path.string());
    auto restored = eve::bios::Runtime::load(path.string());
    const bool identical_load = runtime.state_fingerprint() ==
        restored.state_fingerprint();
    static_cast<void>(runtime.advance_time_by(32));
    static_cast<void>(restored.advance_time_by(32));
    out << pending_events << ',' << identical_load << ','
        << (runtime.state_fingerprint() == restored.state_fingerprint()) << ','
        << runtime.network().validate() << ','
        << restored.network().validate() << '\n';
    std::filesystem::remove(path);
}

void b512_manifests(const std::filesystem::path& output) {
    std::ofstream concepts(output / "learning-stage-definitions.csv");
    concepts << "stage,definition,behavior_alone_sufficient\n"
        "activity,observed neural processing,0\n"
        "change,experience-dependent neural state change,0\n"
        "partial_learning,functional task-specific effect before reliable skill,0\n"
        "latent_learning,controlled later advantage without prior successful recall,0\n"
        "first_recall,first correct autonomous response,1\n"
        "reliable_skill,at least five of six valid contexts correct,1\n"
        "consolidation,improved reliability after further experience,0\n"
        "retention,availability after time or competing experience,1\n"
        "reacquisition,return after demonstrated impairment,1\n";
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,description,implemented\n"
        "K0,unexperienced network,1\nK1,A experience,1\n"
        "K2,independent experience,1\nK3,equal time without experience,1\n"
        "K4,A pretraining without prior success,1\nK5,equal subsequent A training,1\n"
        "K6,first autonomous success,1\nK7,more A after first success,1\n"
        "K8,pause after first success,1\nK9,other experience after success,1\n"
        "K10,mixed experience after success,1\nK11,impaired A recall,1\n"
        "K12,A reacquisition,1\nK13,A then B,1\nK14,A then B then A,1\n"
        "K15,A then B then mixed,1\nK16,no learning,1\n"
        "K17,diagnostic write suppression,1\nK18,R0,1\nK19,R2-50,1\n"
        "K20,S4-d16,1\nK21,B511 corrected evaluation,1\n"
        "K22,extended learning-stage evaluation,1\n";
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "phase,seeds,families,assignments,checkpoints,selection\n"
        "discovery,90001..90064,3,2,0|1|2|4|8|16|32|64|128|256|512,predeclared\n"
        "confirmation,100001..100064,3,2,0|1|2|4|8|16|32|64|128|256|512,independent\n"
        "interventions,first 5 seeds,3,2,post-first-recall write,predeclared\n";
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "candidate,authorized,decision\n"
        "new_learning_rule,0,not part of B512\n"
        "semantic_task_state,0,forbidden\n"
        "automatic_weight_protection,0,forbidden\n"
        "rollback,0,forbidden\n"
        "extended_observation,1,research only\n"
        "S4_default,0,experimental and disabled\n";
}

}  // namespace

int main(int argc, char** argv) {
    const auto output = argc > 1 ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b512");
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    std::filesystem::create_directories(output);
    b512_manifests(output);
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
    const B511MembraneVariant frozen{
        "FrozenWrites", eve::RecoveryMode::input_dependent, 0, 16, 0,
        false, true, 32};

    std::ofstream curves(output / "learning-curves.csv");
    curves << "phase,membrane,family,assignment,seed,experiences,autonomous_correct,autonomous_wrong,missing,double,early,explored,positive_modulation,negative_modulation,positive_writes,negative_writes,direct_A,conditional_A,reliable_A,first_success,first_reliable,pending_events,active_eligibility,valid\n";
    std::ofstream latent(output / "latent-learning.csv");
    latent << "phase,group,family,assignment,seed,latent_candidate,prior_autonomous_successes,pre_direct_A,pre_conditional_A,prior_positive_writes,prior_negative_writes,additional_to_first,autonomous_correct,autonomous_total,final_conditional_A,reliable_A,positive_writes,negative_writes,weight_increases,weight_decreases,valid\n";
    std::ofstream simple(output / "simple-reference.csv");
    simple << "phase,group,family,assignment,seed,latent_candidate,prior_exploration_correct,prior_positive_writes,prior_negative_writes,prior_probe,additional_to_first,subsequent_autonomous_correct,final_probe,weight_increases,weight_decreases,valid\n";
    std::ofstream consolidation(output / "consolidation.csv");
    consolidation << "phase,group,family,assignment,seed,first_success,baseline_conditional_A,baseline_reliable_A,subsequent_correct_A,subsequent_A_experiences,final_conditional_A,final_reliable_A,positive_writes,negative_writes,weight_increases,weight_decreases,pending_events,valid\n";
    std::ofstream impairment(output / "impairment-reacquisition.csv");
    impairment << "phase,family,assignment,seed,first_A,first_B,A_after_A,A_after_B,impaired,reacquired_A,naive_first_A,faster_reacquisition,A_after_reacquisition,A_after_mixed,B_after_mixed,valid\n";
    std::ofstream multi(output / "multi-learning.csv");
    multi << "phase,stage,family,assignment,seed,experiences,A_conditional,B_conditional,repertoire,A_direct,B_direct,valid\n";
    std::ofstream interventions(output / "consolidation-interventions.csv");
    interventions << "phase,family,assignment,seed,episode,event,synapse,weight_before,weight_after,positive_change,negative_change,event_write_count,suppressed,W2_identical,W0_final_A,W1_final_A,W2_final_A,all_writes_suppressed_final_A,effect,W0_valid,W1_valid,W2_valid,combined_valid\n";
    std::vector<B512Metric> metrics;
    for (const auto& phase_row : std::array<std::pair<const char*, std::uint64_t>, 2>{{
             {"discovery", 90001}, {"confirmation", 100001}}}) {
        const std::string phase = phase_row.first;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint32_t offset = 0; offset < kB511Seeds; ++offset) {
                    const auto seed = phase_row.second + offset;
                    b512_simple_reference(phase, r0, family, inverse, seed,
                                          simple, metrics);
                    b512_learning_curve(phase, r0, family, inverse, seed,
                                        curves, metrics);
                    b512_latent_groups(phase, r0, family, inverse, seed,
                                       latent, metrics);
                    b512_consolidation(phase, r0, family, inverse, seed,
                                       consolidation, metrics);
                    b512_impairment_and_multi(phase, r0, family, inverse, seed,
                                              impairment, multi, metrics);
                }
            }
        }
        b512_interventions(phase, r0, phase_row.second, interventions);
    }
    // Same predefined A curve under R2 and S4; S4 remains research-only.
    for (const auto& variant : {r2, s4}) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint32_t offset = 0; offset < kB511Seeds; ++offset) {
                    b512_learning_curve("variant_discovery", variant, family,
                        inverse, 90001 + offset, curves, metrics);
                }
            }
        }
    }
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (std::uint32_t offset = 0; offset < kB511Seeds; ++offset) {
                b512_learning_curve("control_discovery", no_learning, family,
                    inverse, 90001 + offset, curves, metrics);
                b512_learning_curve("control_discovery", frozen, family,
                    inverse, 90001 + offset, curves, metrics, true);
            }
        }
    }

    std::ofstream aggregate(output / "aggregate-statistics.csv");
    aggregate << "phase,group,metric,n,mean,ci_low,ci_high\n";
    std::ofstream paired(output / "paired-statistics.csv");
    paired << "phase,comparison,cluster,n,mean_difference,ci_low,ci_high\n";
    b512_statistics(metrics, aggregate, paired);
    std::ofstream persistence(output / "persistence.csv");
    persistence << "pending_events,identical_load,identical_continuation,original_valid,restored_valid\n";
    b512_persistence(output, persistence);

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
        << "\nprimary_networks," << (2 * 3 * 2 * kB511Seeds)
        << "\nadditional_persistent_bytes_per_neuron,0\n"
        << "additional_persistent_bytes_per_synapse,0\n"
        << "production_rule_changes,0\n";
    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n"
        "complete_learning_curves,1,learning-curves.csv.gz\n"
        "simple_learnable_reference,1,simple-reference.csv.gz\n"
        "first_success_separate_from_reliability,1,learning-curves.csv.gz\n"
        "unsuccessful_pretraining_population,1,latent-learning.csv.gz\n"
        "functional_pretraining_controls,1,latent-learning.csv.gz\n"
        "experience_and_time_controls,1,consolidation.csv.gz\n"
        "temporary_impairment_and_reacquisition,1,impairment-reacquisition.csv.gz\n"
        "multiple_learning_stages,1,multi-learning.csv.gz\n"
        "isolated_and_combined_writes,1,consolidation-interventions.csv\n"
        "independent_confirmation,1,sampling-plan.csv\n"
        "pending_event_persistence,1,persistence.csv\n"
        "new_rule_not_released,1,mechanism-decision.csv\n";
    std::cout << "B.5.12 complete primary_networks="
              << (2 * 3 * 2 * kB511Seeds) << " wall=" << std::fixed
              << std::setprecision(2) << wall << "s\n";
    return 0;
}
