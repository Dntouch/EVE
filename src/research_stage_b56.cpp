#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B55_EMBEDDED
#include "research_stage_b55.cpp"
#undef EVE_RESEARCH_STAGE_B55_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <bitset>
#include <set>

namespace {

constexpr std::uint32_t kB56Episodes = 512;
constexpr std::uint32_t kB56EvaluationEpisodes = 64;
constexpr double kB56RobustThreshold = .75;
constexpr double kB56LowAvailability = .50;

using B56Component = eve::bios::ExperimentalTransientComponent;

struct B56Schedule {
    std::string name;
    std::vector<std::uint64_t> times;
};

struct B56Life {
    std::uint32_t first_x{kB51Never};
    std::uint32_t first_y{kB51Never};
    std::uint32_t first_both{kB51Never};
    std::uint32_t first_loss_x{kB51Never};
    std::uint32_t first_loss_y{kB51Never};
    std::uint32_t first_reacquisition_x{kB51Never};
    std::uint32_t first_reacquisition_y{kB51Never};
    std::uint64_t losses_x{};
    std::uint64_t losses_y{};
    std::uint64_t reacquisitions_x{};
    std::uint64_t reacquisitions_y{};
    std::uint64_t both_episodes{};
    std::uint32_t current_both{};
    std::uint32_t longest_both{};
    bool ever_x{};
    bool ever_y{};
    bool ever_both{};
    bool final_x{};
    bool final_y{};
};

struct B56Result {
    std::string phase;
    std::string variant;
    std::string schedule;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    double e1_x{};
    double e1_y{};
    double e2_x{};
    double e2_y{};
    double retention_x{};
    double retention_y{};
    B56Life life;
    std::bitset<8> classes;
    std::array<std::int32_t, 6> weights{};
    std::uint64_t events{};
    std::uint64_t fires{};
    std::uint64_t eligibility{};
    std::uint64_t scheduled_s4{};
    std::uint64_t processed_s4{};
    std::uint64_t stale_s4{};
    std::uint64_t maximum_queue{};
    std::size_t memory{};
    bool robust{};
    bool valid{};
};

struct B56Run {
    B56Result result;
    eve::bios::Runtime runtime;
    eve::bios::Runtime state_128;
    std::optional<eve::bios::Runtime> first_both;
    std::optional<eve::bios::Runtime> last_both;
    B47Circuit circuit;
    B48Variant learning_variant;
    std::vector<B48Plan> plans;
    std::uint32_t first_both_episode{kB51Never};
    std::uint32_t last_both_episode{kB51Never};
};

std::string b56_assignment(const bool inverse) {
    return inverse ? "inverse" : "normal";
}

std::string b56_value(const std::uint32_t value) {
    return value == kB51Never ? "NA" : std::to_string(value);
}

B56Schedule b56_schedule(
    const std::string& name, const std::uint64_t seed,
    const std::uint32_t family, const std::size_t count) {
    B56Schedule result{name, std::vector<std::uint64_t>(count + 1, 0)};
    const std::array<std::uint32_t, 6> gaps{{8, 12, 16, 20, 24, 32}};
    std::uint32_t fixed = 0;
    if (name.rfind("fixed_", 0) == 0) {
        fixed = static_cast<std::uint32_t>(std::stoul(name.substr(6)));
    }
    eve::bios::DeterministicRng rng(b47_mix_seed(
        0x5601000ULL ^ seed ^ (static_cast<std::uint64_t>(family) << 32U)));
    for (std::size_t index = 0; index < count; ++index) {
        std::uint32_t gap = fixed;
        if (name == "irregular") gap = gaps[rng.bounded(gaps.size())];
        if (name == "continuous") gap = 1U + rng.bounded(32);
        result.times[index + 1] = result.times[index] + gap;
    }
    return result;
}

B51Episode b56_episode_at(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const std::uint8_t cue, const std::uint8_t planned_action,
    const bool target_a, const std::uint64_t start, const std::uint64_t end,
    const bool plasticity = true) {
    if (runtime.simulated_time() < start) {
        static_cast<void>(runtime.advance_time_to(start));
    }
    B51Episode result(runtime);
    result.cue = cue;
    result.planned_action = planned_action;
    result.target_a = target_a;
    result.before_weights = runtime.network().synapses();
    std::uint8_t phase = 0;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        if (phase > 1) return;
        for (std::size_t index = 0; index < 2; ++index) {
            if (event.neuron == circuit.decisions[index]) {
                ++result.decision_fires[index];
            }
            if (event.neuron == circuit.motors[index]) {
                ++result.motor_fires[index];
                if (index == 0) result.final_a = true;
                else result.final_b = true;
                if (phase == 0 && index == 0) result.autonomous_a = true;
                if (phase == 0 && index == 1) result.autonomous_b = true;
            }
        }
    });
    runtime.set_impulse_observer(
        [&](const eve::bios::ImpulseObservation& event) {
            if (phase > 1) return;
            result.impulses.push_back(event);
            for (std::size_t index = 0; index < 2; ++index) {
                if (event.receiver == circuit.decisions[index]) {
                    result.decision_peak[index] = std::max(
                        result.decision_peak[index], event.membrane_after);
                }
                if (event.receiver == circuit.motors[index]) {
                    result.motor_peak[index] = std::max(
                        result.motor_peak[index], event.membrane_after);
                }
            }
        });
    runtime.enqueue_fire(circuit.sensors[cue]);
    static_cast<void>(runtime.run());
    result.explored = !result.autonomous_a && !result.autonomous_b;
    if (result.explored) {
        phase = 1;
        result.executed_action = planned_action;
        runtime.enqueue_fire(circuit.explorers[planned_action]);
        static_cast<void>(runtime.run());
    }
    result.correct = b51_correct(target_a, result.final_a, result.final_b);
    result.modulation = static_cast<std::int8_t>(result.correct ? 1 : -1);
    result.after_activity = runtime;
    phase = 2;
    runtime.set_fire_observer({});
    runtime.set_impulse_observer({});
    if (plasticity) {
        result.feedback = runtime.process_modulatory_feedback(result.modulation);
    }
    result.after_weights = runtime.network().synapses();
    if (runtime.simulated_time() < end) {
        static_cast<void>(runtime.advance_time_to(end));
    }
    result.after_feedback = std::move(runtime);
    return result;
}

B51Evaluation b56_evaluate_at(
    const eve::bios::Runtime& trained, const B47Circuit& circuit,
    const B48Variant& variant, const std::vector<B48Plan>& plans,
    const std::uint32_t checkpoint, const B56Schedule& schedule) {
    eve::bios::Runtime e1 = trained;
    eve::bios::Runtime e2 = trained;
    B51Evaluation result;
    result.identical_start = e1.state_fingerprint() == e2.state_fingerprint() &&
        e1.state_fingerprint() == trained.state_fingerprint();
    for (std::uint32_t offset = 0; offset < kB56EvaluationEpisodes; ++offset) {
        const auto episode = checkpoint + offset;
        if (e1.simulated_time() < schedule.times[episode]) {
            static_cast<void>(e1.advance_time_to(schedule.times[episode]));
            static_cast<void>(e2.advance_time_to(schedule.times[episode]));
        }
        const auto cue = b48_cue(
            variant, plans[episode], episode, b46_training(variant.task));
        const bool target_a = b48_target_a(
            variant, cue, episode, b46_training(variant.task));
        const auto e1_probe = b51_probe_in_place(e1, circuit, cue);
        b47_record(result.e1[cue], target_a,
            e1_probe.motor_a, e1_probe.motor_b);
        const auto e2_probe = b51_probe_in_place(e2, circuit, cue);
        bool final_a = e2_probe.motor_a;
        bool final_b = e2_probe.motor_b;
        if (!final_a && !final_b) {
            e2.set_fire_observer([&](const eve::bios::FireObservation& event) {
                final_a = final_a || event.neuron == circuit.motors[0];
                final_b = final_b || event.neuron == circuit.motors[1];
            });
            e2.enqueue_fire(circuit.explorers[plans[episode].exploration]);
            static_cast<void>(e2.run());
            e2.set_fire_observer({});
        }
        b47_record(result.e2[cue], target_a, final_a, final_b);
        if (e1.simulated_time() < schedule.times[episode + 1]) {
            static_cast<void>(e1.advance_time_to(schedule.times[episode + 1]));
            static_cast<void>(e2.advance_time_to(schedule.times[episode + 1]));
        }
    }
    return result;
}

void b56_update_life(
    B56Life& life, const std::array<bool, 2>& previous,
    const std::array<bool, 2>& current, const std::uint32_t episode) {
    if (current[0] && life.first_x == kB51Never) life.first_x = episode;
    if (current[1] && life.first_y == kB51Never) life.first_y = episode;
    if (current[0] && current[1] && life.first_both == kB51Never) {
        life.first_both = episode;
    }
    if (previous[0] && !current[0]) {
        ++life.losses_x;
        if (life.first_loss_x == kB51Never) life.first_loss_x = episode;
    }
    if (previous[1] && !current[1]) {
        ++life.losses_y;
        if (life.first_loss_y == kB51Never) life.first_loss_y = episode;
    }
    if (!previous[0] && current[0] && life.first_x != episode) {
        ++life.reacquisitions_x;
        if (life.first_reacquisition_x == kB51Never) {
            life.first_reacquisition_x = episode;
        }
    }
    if (!previous[1] && current[1] && life.first_y != episode) {
        ++life.reacquisitions_y;
        if (life.first_reacquisition_y == kB51Never) {
            life.first_reacquisition_y = episode;
        }
    }
    life.ever_x = life.ever_x || current[0];
    life.ever_y = life.ever_y || current[1];
    life.ever_both = life.ever_both || (current[0] && current[1]);
    if (current[0] && current[1]) {
        ++life.both_episodes;
        ++life.current_both;
        life.longest_both = std::max(life.longest_both, life.current_both);
    } else {
        life.current_both = 0;
    }
    life.final_x = current[0];
    life.final_y = current[1];
}

std::bitset<8> b56_classes(
    const B56Life& life, const double e1_x, const double e1_y) {
    std::bitset<8> result;
    result[0] = e1_x < kB56RobustThreshold && e1_y >= kB56RobustThreshold;
    result[1] = e1_y < kB56RobustThreshold && e1_x >= kB56RobustThreshold;
    result[2] = e1_x < kB56RobustThreshold && e1_y < kB56RobustThreshold;
    result[3] = !life.ever_x || !life.ever_y;
    result[4] = life.ever_both && (!life.final_x || !life.final_y);
    result[5] = life.ever_both &&
        (life.losses_x + life.losses_y + life.reacquisitions_x +
            life.reacquisitions_y > 0);
    result[6] = life.final_x && life.final_y &&
        (e1_x < kB56RobustThreshold || e1_y < kB56RobustThreshold);
    result[7] = e1_x >= kB56RobustThreshold && e1_y >= kB56RobustThreshold &&
        static_cast<double>(life.both_episodes) / kB56Episodes <
            kB56LowAvailability;
    return result;
}

void b56_emit_evaluation(
    std::ofstream* checkpoints, const std::string& phase,
    const std::string& variant, const std::string& schedule,
    const std::uint32_t family, const bool inverse, const std::uint64_t seed,
    const std::uint32_t checkpoint, const B51Evaluation& evaluation) {
    if (checkpoints == nullptr) return;
    for (std::size_t cue = 0; cue < 2; ++cue) {
        const auto emit = [&](const char* mode, const B47Counts& value) {
            *checkpoints << phase << ',' << variant << ',' << schedule << ','
                << family << ',' << b56_assignment(inverse) << ',' << seed
                << ',' << checkpoint << ',' << mode << ',' << cue << ','
                << value.correct << ',' << value.wrong << ',' << value.both
                << ',' << value.none << ',' << b47_rate(value) << ','
                << evaluation.identical_start << '\n';
        };
        emit("E1", evaluation.e1[cue]);
        emit("E2", evaluation.e2[cue]);
    }
}

B56Run b56_run(
    const std::string& phase, const B55NaturalVariant& natural_variant,
    const std::string& schedule_name, const std::uint32_t family,
    const bool inverse, const std::uint64_t seed,
    std::ofstream* biographies = nullptr,
    std::ofstream* checkpoints = nullptr,
    std::ofstream* stability = nullptr) {
    B48Variant learning_variant{natural_variant.name, B46Task::mixed,
        B48Policy::reference, inverse, natural_variant.plasticity,
        false, family};
    auto state = b55_initial_state(learning_variant, natural_variant, seed);
    const auto schedule = b56_schedule(schedule_name, seed, family,
        kB56Episodes + kB56EvaluationEpisodes);
    B56Run run{{}, state.runtime, state.runtime, std::nullopt, std::nullopt,
        state.circuit, learning_variant, state.plans, kB51Never, kB51Never};
    run.result.phase = phase;
    run.result.variant = natural_variant.name;
    run.result.schedule = schedule_name;
    run.result.family = family;
    run.result.inverse = inverse;
    run.result.seed = seed;
    auto previous = b51_recall_pair(
        state.runtime, state.circuit, learning_variant, 0);
    std::uint32_t current_episode = 0;
    if (biographies != nullptr) {
        state.runtime.set_subthreshold_relaxation_observer(
            [&](const eve::bios::SubthresholdRelaxationObservation& event) {
                *biographies << "S4_EVENT," << phase << ','
                    << natural_variant.name << ',' << schedule_name << ','
                    << family << ',' << b56_assignment(inverse) << ',' << seed
                    << ',' << current_episode << ',' << event.simulated_time
                    << ',' << event.neuron << ',' << event.generation << ','
                    << event.membrane_before << ',' << event.membrane_after
                    << ',' << event.relaxed << ',' << event.stale
                    << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n";
            });
    }
    const std::array<std::uint32_t, 4> checkpoint_values{{128, 256, 384, 512}};
    const std::array<std::uint32_t, 7> offsets{{1, 4, 8, 16, 32, 64, 128}};
    for (std::uint32_t episode = 0; episode < kB56Episodes; ++episode) {
        current_episode = episode;
        const auto cue = b48_cue(learning_variant, state.plans[episode],
            episode, kB56Episodes);
        const bool target_a = b48_target_a(
            learning_variant, cue, episode, kB56Episodes);
        const auto before_metrics = state.runtime.metrics();
        const auto before_time = state.runtime.simulated_time();
        auto row = b56_episode_at(state.runtime, state.circuit, cue,
            state.plans[episode].exploration, target_a,
            schedule.times[episode], schedule.times[episode + 1],
            natural_variant.plasticity);
        state.runtime = std::move(row.after_feedback);
        const auto after_metrics = state.runtime.metrics();
        const auto current = b51_recall_pair(
            state.runtime, state.circuit, learning_variant, episode + 1);
        b56_update_life(run.result.life, previous, current, episode);
        if (current[0] && current[1]) {
            if (!run.first_both.has_value()) {
                run.first_both = state.runtime;
                run.first_both_episode = episode;
            }
            run.last_both = state.runtime;
            run.last_both_episode = episode;
        }
        if (episode + 1 == 128) run.state_128 = state.runtime;
        if (stability != nullptr && run.result.life.first_both != kB51Never) {
            const auto age = episode - run.result.life.first_both;
            if (std::find(offsets.begin(), offsets.end(), age) != offsets.end()) {
                *stability << phase << ',' << natural_variant.name << ','
                    << schedule_name << ',' << family << ','
                    << b56_assignment(inverse) << ',' << seed << ','
                    << run.result.life.first_both << ',' << age << ','
                    << episode << ',' << current[0] << ',' << current[1]
                    << ',' << run.result.life.losses_x << ','
                    << run.result.life.losses_y << '\n';
            }
        }
        if (biographies != nullptr) {
            const auto weights = b55_weights(state.runtime, state.circuit);
            const auto& da = state.runtime.network().neuron(
                state.circuit.decisions[0]);
            const auto& db = state.runtime.network().neuron(
                state.circuit.decisions[1]);
            std::size_t eligibilities = 0;
            for (eve::NeuronId id = 0;
                 id < state.runtime.network().neuron_count(); ++id) {
                eligibilities += state.runtime.network()
                    .eligibility_records(id).size();
            }
            *biographies << "EPISODE," << phase << ','
                << natural_variant.name << ',' << schedule_name << ','
                << family << ',' << b56_assignment(inverse) << ',' << seed
                << ',' << episode << ',' << before_time << ','
                << state.runtime.simulated_time() << ','
                << static_cast<unsigned>(cue) << ',' << (target_a ? "A" : "B")
                << ',' << static_cast<unsigned>(row.planned_action) << ','
                << static_cast<unsigned>(row.executed_action) << ','
                << row.autonomous_a << ',' << row.autonomous_b << ','
                << row.final_a << ',' << row.final_b << ',' << row.correct
                << ',' << row.explored << ',' << static_cast<int>(row.modulation)
                << ',' << (row.feedback.positive_changes +
                    row.feedback.negative_changes) << ','
                << row.decision_fires[0] << ',' << row.decision_fires[1]
                << ',' << row.motor_fires[0] << ',' << row.motor_fires[1]
                << ',' << row.decision_peak[0] << ',' << row.decision_peak[1]
                << ',' << da.membrane_potential << ',' << db.membrane_potential
                << ',' << da.refractory_events_remaining << ','
                << db.refractory_events_remaining << ','
                << da.recovery_generation << ',' << db.recovery_generation
                << ',' << da.subthreshold_relaxation_generation << ','
                << db.subthreshold_relaxation_generation << ','
                << da.scheduled_subthreshold_relaxation_time << ','
                << db.scheduled_subthreshold_relaxation_time << ','
                << eligibilities;
            for (const auto weight : weights) *biographies << ',' << weight;
            *biographies << ',' << row.impulses.size() << ','
                << after_metrics.fire_events - before_metrics.fire_events
                << ',' << after_metrics.eligibility_decisions -
                    before_metrics.eligibility_decisions
                << ',' << after_metrics.scheduled_subthreshold_relaxations -
                    before_metrics.scheduled_subthreshold_relaxations
                << ',' << after_metrics.processed_subthreshold_relaxations -
                    before_metrics.processed_subthreshold_relaxations
                << ',' << after_metrics.stale_subthreshold_relaxations -
                    before_metrics.stale_subthreshold_relaxations
                << ',' << current[0] << ',' << current[1] << ','
                << (current[0] && current[1]) << '\n';
        }
        previous = current;
        if (checkpoints != nullptr &&
            std::find(checkpoint_values.begin(), checkpoint_values.end(),
                episode + 1) != checkpoint_values.end()) {
            const auto evaluation = b56_evaluate_at(state.runtime,
                state.circuit, learning_variant, state.plans, episode + 1,
                schedule);
            b56_emit_evaluation(checkpoints, phase, natural_variant.name,
                schedule_name, family, inverse, seed, episode + 1, evaluation);
        }
    }
    state.runtime.set_subthreshold_relaxation_observer({});
    const auto evaluation = b56_evaluate_at(state.runtime, state.circuit,
        learning_variant, state.plans, kB56Episodes, schedule);
    run.result.e1_x = b47_rate(evaluation.e1[0]);
    run.result.e1_y = b47_rate(evaluation.e1[1]);
    run.result.e2_x = b47_rate(evaluation.e2[0]);
    run.result.e2_y = b47_rate(evaluation.e2[1]);
    run.result.robust = run.result.e1_x >= kB56RobustThreshold &&
        run.result.e1_y >= kB56RobustThreshold;
    run.result.classes = b56_classes(
        run.result.life, run.result.e1_x, run.result.e1_y);
    auto retained = state.runtime;
    static_cast<void>(retained.advance_time_by(128ULL * 16ULL));
    const auto retained_eval = b56_evaluate_at(retained, state.circuit,
        learning_variant, state.plans, kB56Episodes, schedule);
    run.result.retention_x = b47_rate(retained_eval.e1[0]);
    run.result.retention_y = b47_rate(retained_eval.e1[1]);
    run.result.weights = b55_weights(state.runtime, state.circuit);
    const auto& metrics = state.runtime.metrics();
    run.result.events = metrics.processed_events;
    run.result.fires = metrics.fire_events;
    run.result.eligibility = metrics.eligibility_decisions;
    run.result.scheduled_s4 = metrics.scheduled_subthreshold_relaxations;
    run.result.processed_s4 = metrics.processed_subthreshold_relaxations;
    run.result.stale_s4 = metrics.stale_subthreshold_relaxations;
    run.result.maximum_queue = metrics.maximum_queue_size;
    run.result.memory = state.runtime.diagnostics().estimated_memory_bytes;
    run.result.valid = state.runtime.network().validate() &&
        evaluation.identical_start && retained_eval.identical_start;
    run.runtime = std::move(state.runtime);
    run.state_128.set_subthreshold_relaxation_observer({});
    if (run.last_both.has_value()) {
        run.last_both->set_subthreshold_relaxation_observer({});
    }
    return run;
}

void b56_write_result(std::ofstream& file, const B56Result& row) {
    file << row.phase << ',' << row.variant << ',' << row.schedule << ','
        << row.family << ',' << b56_assignment(row.inverse) << ',' << row.seed
        << ',' << row.e1_x << ',' << row.e1_y << ',' << row.e2_x << ','
        << row.e2_y << ',' << row.retention_x << ',' << row.retention_y << ','
        << b56_value(row.life.first_x) << ',' << b56_value(row.life.first_y)
        << ',' << b56_value(row.life.first_both) << ','
        << b56_value(row.life.first_loss_x) << ','
        << b56_value(row.life.first_loss_y) << ','
        << b56_value(row.life.first_reacquisition_x) << ','
        << b56_value(row.life.first_reacquisition_y) << ','
        << row.life.losses_x << ',' << row.life.losses_y << ','
        << row.life.reacquisitions_x << ',' << row.life.reacquisitions_y << ','
        << row.life.both_episodes << ',' << row.life.longest_both << ','
        << row.life.final_x << ',' << row.life.final_y << ',' << row.robust;
    for (std::size_t index = 0; index < row.classes.size(); ++index) {
        file << ',' << row.classes[index];
    }
    for (const auto weight : row.weights) file << ',' << weight;
    file << ',' << row.events << ',' << row.fires << ',' << row.eligibility
        << ',' << row.scheduled_s4 << ',' << row.processed_s4 << ','
        << row.stale_s4 << ',' << row.maximum_queue << ',' << row.memory
        << ',' << row.valid << '\n';
}

std::array<bool, 2> b56_recall(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const B48Variant& variant, const std::uint32_t episode) {
    return b51_recall_pair(runtime, circuit, variant, episode);
}

template <typename Ids>
std::size_t b56_copy_weights(
    eve::bios::Runtime& target, const eve::bios::Runtime& source,
    const Ids& ids) {
    std::size_t changed = 0;
    for (const auto id : ids) {
        const auto value = source.network().synapse(id).weight;
        changed += target.network().synapse(id).weight != value;
        target.set_experimental_synapse_weight(id, value);
    }
    return changed;
}

struct B56Transfer {
    std::size_t changed{};
    std::size_t coupled{};
    bool valid{true};
};

B56Transfer b56_copy_transient(
    eve::bios::Runtime& target, const eve::bios::Runtime& source) {
    B56Transfer result;
    const std::array<B56Component, 7> components{{
        B56Component::membrane,
        B56Component::input_activity,
        B56Component::local_event_memory,
        B56Component::eligibility,
        B56Component::synaptic_activity,
        B56Component::recovery,
        B56Component::context,
    }};
    for (const auto component : components) {
        const auto transfer =
            target.set_experimental_transient_component_from(source, component);
        result.changed += transfer.changed_values;
        result.coupled += transfer.coupled_events;
        result.valid = result.valid && transfer.valid;
    }
    return result;
}

void b56_causal_interventions(
    const B56Run& run, std::ofstream& weights, std::ofstream& transient,
    std::ofstream& factorial, std::ofstream& taxonomy) {
    const auto current = b56_recall(run.runtime, run.circuit,
        run.learning_variant, kB56Episodes);
    const auto prefix = run.result.phase + ',' + run.result.variant + ',' +
        run.result.schedule + ',' + std::to_string(run.result.family) + ',' +
        b56_assignment(run.result.inverse) + ',' +
        std::to_string(run.result.seed);
    if (!run.last_both.has_value()) {
        taxonomy << prefix << ",H1_no_joint_acquisition,1,0,0,0,0,"
            "no prior jointly available state exists\n";
        weights << prefix << ",none,none,NA,0," << current[0] << ','
            << current[1] << ",0,0,0,no_prior_joint_state\n";
        transient << prefix << ",none,NA,0,0," << current[0] << ','
            << current[1] << ",0,0,0,no_prior_joint_state\n";
        factorial << prefix << ",NA,NA,NA,NA,0,0,0," << current[0]
            << ',' << current[1] << ",0,no_prior_joint_state\n";
        return;
    }
    auto old = *run.last_both;
    old.set_subthreshold_relaxation_observer({});
    if (old.simulated_time() < run.runtime.simulated_time()) {
        static_cast<void>(old.advance_time_to(run.runtime.simulated_time()));
    }
    const auto old_pair = b56_recall(old, run.circuit,
        run.learning_variant, run.last_both_episode + 1);
    const auto emit_weight = [&](const char* group, const char* direction,
            const eve::SynapseId id, const auto& ids) {
        auto target = std::string(direction) == "restore"
            ? run.runtime : old;
        const auto& source = std::string(direction) == "restore"
            ? old : run.runtime;
        std::size_t changed = 0;
        if (id == eve::kInvalidIndex) changed = b56_copy_weights(target, source, ids);
        else {
            const auto value = source.network().synapse(id).weight;
            changed = target.network().synapse(id).weight != value;
            target.set_experimental_synapse_weight(id, value);
        }
        const auto pair = b56_recall(target, run.circuit,
            run.learning_variant, kB56Episodes);
        const bool restores = std::string(direction) == "restore" &&
            (!current[0] || !current[1]) && pair[0] && pair[1];
        const bool induces = std::string(direction) == "induce" &&
            old_pair[0] && old_pair[1] && (!pair[0] || !pair[1]);
        weights << prefix << ',' << group << ',' << direction << ',';
        if (id == eve::kInvalidIndex) weights << "all";
        else weights << id;
        weights << ',' << changed << ',' << pair[0] << ',' << pair[1]
            << ',' << restores << ',' << induces << ','
            << target.network().validate() << ",valid\n";
    };
    for (const auto id : run.circuit.cue_synapses) {
        emit_weight("sensor", "restore", id, run.circuit.cue_synapses);
        emit_weight("sensor", "induce", id, run.circuit.cue_synapses);
    }
    emit_weight("sensor", "restore", eve::kInvalidIndex,
        run.circuit.cue_synapses);
    emit_weight("sensor", "induce", eve::kInvalidIndex,
        run.circuit.cue_synapses);
    for (const auto id : run.circuit.inhibitory_outputs) {
        emit_weight("inhibition", "restore", id,
            run.circuit.inhibitory_outputs);
        emit_weight("inhibition", "induce", id,
            run.circuit.inhibitory_outputs);
    }
    emit_weight("inhibition", "restore", eve::kInvalidIndex,
        run.circuit.inhibitory_outputs);
    emit_weight("inhibition", "induce", eve::kInvalidIndex,
        run.circuit.inhibitory_outputs);

    const std::array<std::pair<B56Component, const char*>, 7> components{{
        {B56Component::membrane, "membrane"},
        {B56Component::input_activity, "input_activity"},
        {B56Component::local_event_memory, "local_event_memory"},
        {B56Component::eligibility, "eligibility"},
        {B56Component::synaptic_activity, "synaptic_activity"},
        {B56Component::recovery, "recovery_and_S4"},
        {B56Component::context, "context"},
    }};
    for (const auto& component : components) {
        for (const bool restore : {true, false}) {
            auto target = restore ? run.runtime : old;
            const auto& source = restore ? old : run.runtime;
            const auto transfer = target.set_experimental_transient_component_from(
                source, component.first);
            const auto pair = b56_recall(target, run.circuit,
                run.learning_variant, kB56Episodes);
            const bool sufficient = restore && pair[0] && pair[1] &&
                (!current[0] || !current[1]);
            const bool necessary = !restore && old_pair[0] && old_pair[1] &&
                (!pair[0] || !pair[1]);
            transient << prefix << ',' << component.second << ','
                << (restore ? "restore" : "induce") << ','
                << transfer.changed_values << ',' << transfer.coupled_events
                << ',' << pair[0] << ',' << pair[1] << ',' << sufficient
                << ',' << necessary << ',' << transfer.valid << ",valid\n";
        }
    }

    bool sensor_fix = false;
    bool inhibition_fix = false;
    bool transient_fix = false;
    bool combined_fix = false;
    for (std::uint32_t mask = 0; mask < 8; ++mask) {
        auto target = run.runtime;
        std::size_t changed_s = 0;
        std::size_t changed_i = 0;
        B56Transfer transfer;
        if ((mask & 1U) != 0) {
            changed_s = b56_copy_weights(target, old,
                run.circuit.cue_synapses);
        }
        if ((mask & 2U) != 0) {
            changed_i = b56_copy_weights(target, old,
                run.circuit.inhibitory_outputs);
        }
        if ((mask & 4U) != 0) transfer = b56_copy_transient(target, old);
        const auto pair = b56_recall(target, run.circuit,
            run.learning_variant, kB56Episodes);
        const bool fixed = pair[0] && pair[1] &&
            (!current[0] || !current[1]);
        sensor_fix = sensor_fix || (mask == 1 && fixed);
        inhibition_fix = inhibition_fix || (mask == 2 && fixed);
        transient_fix = transient_fix || (mask == 4 && fixed);
        combined_fix = combined_fix || (mask > 4 && fixed);
        factorial << prefix << ",V" << mask << ',' << ((mask & 1U) != 0)
            << ',' << ((mask & 2U) != 0) << ',' << ((mask & 4U) != 0)
            << ',' << changed_s << ',' << changed_i << ',' << transfer.changed
            << ',' << pair[0] << ',' << pair[1] << ','
            << (target.network().validate() && transfer.valid) << ",valid\n";
    }
    const auto cause = sensor_fix ? "H2_sensor_storage" :
        inhibition_fix ? "H3_inhibitory_competition" :
        transient_fix ? "H4_transient_recall" :
        combined_fix ? "H8_combined" :
        run.result.classes[3] ? "H1_incomplete_acquisition" : "open";
    taxonomy << prefix << ',' << cause << ',' << run.result.classes[3]
        << ',' << sensor_fix << ',' << inhibition_fix << ',' << transient_fix
        << ',' << combined_fix << ",factorial diagnostic classification\n";
}

B56Result b56_continue_from_128(
    const B56Run& source, const std::string& mode,
    const std::uint64_t cue_revision, const std::uint64_t exploration_revision) {
    auto runtime = source.state_128;
    auto plans = b48_plans(source.result.seed,
        kB56Episodes + kB56EvaluationEpisodes, source.result.family,
        cue_revision, exploration_revision);
    const auto schedule = b56_schedule("fixed_16", source.result.seed,
        source.result.family, kB56Episodes + kB56EvaluationEpisodes);
    B56Life life;
    auto previous = b51_recall_pair(runtime, source.circuit,
        source.learning_variant, 128);
    for (std::uint32_t episode = 128; episode < 256; ++episode) {
        const auto cue = b48_cue(source.learning_variant, plans[episode],
            episode, kB56Episodes);
        const bool target_a = b48_target_a(source.learning_variant, cue,
            episode, kB56Episodes);
        auto row = b56_episode_at(runtime, source.circuit, cue,
            plans[episode].exploration, target_a, schedule.times[episode],
            schedule.times[episode + 1], true);
        runtime = std::move(row.after_feedback);
        const auto current = b51_recall_pair(runtime, source.circuit,
            source.learning_variant, episode + 1);
        b56_update_life(life, previous, current, episode);
        previous = current;
    }
    const auto evaluation = b56_evaluate_at(runtime, source.circuit,
        source.learning_variant, plans, 256, schedule);
    B56Result result;
    result.phase = source.result.phase;
    result.variant = mode;
    result.schedule = "fixed_16";
    result.family = source.result.family;
    result.inverse = source.result.inverse;
    result.seed = source.result.seed;
    result.e1_x = b47_rate(evaluation.e1[0]);
    result.e1_y = b47_rate(evaluation.e1[1]);
    result.e2_x = b47_rate(evaluation.e2[0]);
    result.e2_y = b47_rate(evaluation.e2[1]);
    result.life = life;
    result.robust = result.e1_x >= kB56RobustThreshold &&
        result.e1_y >= kB56RobustThreshold;
    result.valid = runtime.network().validate() && evaluation.identical_start;
    result.weights = b55_weights(runtime, source.circuit);
    return result;
}

void b56_summarize(
    const std::filesystem::path& output,
    const std::vector<B56Result>& rows, const std::string& stem) {
    struct Aggregate {
        std::vector<double> e1_x;
        std::vector<double> e1_y;
        std::vector<double> joint;
        std::uint64_t robust{};
        std::uint64_t valid{};
        std::uint64_t events{};
        std::uint64_t fires{};
        std::uint64_t s4{};
        std::uint64_t maximum_queue{};
        std::size_t maximum_memory{};
    };
    std::map<std::string, Aggregate> groups;
    for (const auto& row : rows) {
        const auto key = row.phase + '|' + row.variant + '|' + row.schedule;
        auto& group = groups[key];
        group.e1_x.push_back(row.e1_x);
        group.e1_y.push_back(row.e1_y);
        group.joint.push_back(std::min(row.e1_x, row.e1_y));
        group.robust += row.robust;
        group.valid += row.valid;
        group.events += row.events;
        group.fires += row.fires;
        group.s4 += row.processed_s4;
        group.maximum_queue = std::max(group.maximum_queue, row.maximum_queue);
        group.maximum_memory = std::max(group.maximum_memory, row.memory);
    }
    std::ofstream summary(output / (stem + "-summary.csv"));
    summary << "phase,variant,schedule,networks,E1_X,E1_Y,joint_min,"
        "joint_ci_low,joint_ci_high,robust,events_per_network,"
        "fires_per_network,S4_processed,maximum_queue,maximum_memory,valid\n";
    std::uint64_t salt = 0;
    for (const auto& [key, value] : groups) {
        const auto first = key.find('|');
        const auto second = key.find('|', first + 1);
        const auto interval = bootstrap_interval(
            value.joint, 0x5605000ULL + ++salt);
        const auto count = value.joint.size();
        summary << key.substr(0, first) << ','
            << key.substr(first + 1, second - first - 1) << ','
            << key.substr(second + 1) << ',' << count << ','
            << b48_mean(value.e1_x) << ',' << b48_mean(value.e1_y) << ','
            << b48_mean(value.joint) << ',' << interval.first << ','
            << interval.second << ',' << value.robust << ','
            << static_cast<double>(value.events) / count << ','
            << static_cast<double>(value.fires) / count << ',' << value.s4
            << ',' << value.maximum_queue << ',' << value.maximum_memory
            << ',' << value.valid << '\n';
    }
}

void b56_paired_statistics(
    const std::filesystem::path& output,
    const std::vector<B56Result>& rows) {
    using Key = std::tuple<std::uint32_t, bool, std::uint64_t>;
    std::map<std::string, std::map<Key, const B56Result*>> indexed;
    for (const auto& row : rows) {
        indexed[row.schedule + '|' + row.variant][Key{
            row.family, row.inverse, row.seed}] = &row;
    }
    std::ofstream file(output / "timing-paired-comparisons.csv");
    file << "schedule,comparison,clusters,delta_E1_X,delta_E1_Y,"
        "delta_joint,ci_low,ci_high,robust_gain,robust_loss\n";
    for (const auto& schedule : {"fixed_8", "fixed_12", "fixed_16",
            "fixed_20", "fixed_24", "fixed_32", "irregular", "continuous"}) {
        const auto& base = indexed[std::string(schedule) + "|S4_event_d16"];
        for (const auto& control : {"S2_R2_p50", "S5_disabled"}) {
            const auto& other = indexed[std::string(schedule) + '|' + control];
            std::vector<double> dx;
            std::vector<double> dy;
            std::vector<double> dj;
            std::uint64_t gain = 0;
            std::uint64_t loss = 0;
            for (const auto& [key, left] : base) {
                const auto found = other.find(key);
                if (found == other.end()) continue;
                const auto* right = found->second;
                dx.push_back(left->e1_x - right->e1_x);
                dy.push_back(left->e1_y - right->e1_y);
                dj.push_back(std::min(left->e1_x, left->e1_y) -
                    std::min(right->e1_x, right->e1_y));
                gain += left->robust && !right->robust;
                loss += !left->robust && right->robust;
            }
            const auto interval = bootstrap_interval(dj,
                0x5606000ULL + dj.size() + std::string(control).size());
            file << schedule << ",S4_minus_" << control << ',' << dj.size()
                << ',' << b48_mean(dx) << ',' << b48_mean(dy) << ','
                << b48_mean(dj) << ',' << interval.first << ','
                << interval.second << ',' << gain << ',' << loss << '\n';
        }
    }
}

void b56_boundary_diagnostics(const std::filesystem::path& output) {
    std::ofstream file(output / "boundary-events.csv");
    file << "variant,input_time,delay,membrane_before,membrane_after,fired,"
        "eligibility,recovery_processed,S4_processed,S4_stale,valid\n";
    for (const auto& variant : {"S2_R2_p50", "S4_event_d16", "S5_disabled"}) {
        for (const std::uint64_t input_time : {15ULL, 16ULL, 17ULL}) {
            auto config = b54_micro_config(
                eve::RecoveryMode::independent_with_membrane_relaxation,
                1, 16, 50, true);
            config.subthreshold_relaxation_enabled =
                std::string(variant) == "S4_event_d16";
            config.subthreshold_relaxation_delay =
                config.subthreshold_relaxation_enabled ? 16 : 0;
            eve::bios::Runtime runtime(config,
                0x5607000ULL + input_time + std::string(variant).size());
            const auto weak = runtime.add_experimental_synapse(0, 4, 1, true);
            static_cast<void>(runtime.process_simultaneous_impulses({weak}));
            static_cast<void>(runtime.advance_time_to(input_time));
            const auto before = runtime.network().neuron(4).membrane_potential;
            const auto second = runtime.process_simultaneous_impulses({weak});
            const auto after = runtime.network().neuron(4).membrane_potential;
            file << variant << ',' << input_time << ",16," << before << ','
                << after << ',' << second.fired << ','
                << runtime.metrics().eligibility_decisions << ','
                << runtime.metrics().processed_recovery_events << ','
                << runtime.metrics().processed_subthreshold_relaxations << ','
                << runtime.metrics().stale_subthreshold_relaxations << ','
                << runtime.network().validate() << '\n';
        }
    }
}

void b56_reversal(
    const std::filesystem::path& output,
    const std::array<B55NaturalVariant, 3>& variants) {
    std::ofstream file(output / "retention-reversal.csv");
    file << "variant,family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,"
        "retention_X,retention_Y,robust,valid\n";
    std::ofstream sink;
    for (const auto& variant : variants) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint64_t seed = 20001; seed <= 20064; ++seed) {
                    const auto row = b55_run_natural("fresh_confirmation",
                        variant, B46Task::reversal, family, inverse, seed,
                        sink, sink, sink, sink);
                    file << variant.name << ',' << family << ','
                        << b56_assignment(inverse) << ',' << seed << ','
                        << row.e1_x << ',' << row.e1_y << ',' << row.e2_x
                        << ',' << row.e2_y << ',' << row.retention_x << ','
                        << row.retention_y << ',' << row.robust << ','
                        << row.valid << '\n';
                }
            }
        }
    }
}

void b56_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,pending_events,pending_S4,exact_restore,"
        "exact_due_processing,processed_S4,exact_continuation,valid\n";
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        B48Variant learning{"S4_event_d16", B46Task::mixed,
            B48Policy::reference, false, true, false,
            static_cast<std::uint32_t>((seed - 1) % 3)};
        const B55NaturalVariant variant{"S4_event_d16", true, 16, true};
        auto state = b55_initial_state(learning, variant, seed);
        const auto weak = state.circuit.cue_synapses[0];
        state.runtime.set_experimental_synapse_weight(weak, 1);
        static_cast<void>(state.runtime.process_simultaneous_impulses({weak}));
        const auto pending = state.runtime.future_event_count();
        const auto pending_s4 =
            state.runtime.metrics().scheduled_subthreshold_relaxations;
        const auto path = output / (".b56-state-" + std::to_string(seed));
        state.runtime.save(path.string());
        auto restored = eve::bios::Runtime::load(path.string());
        const bool exact = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        static_cast<void>(state.runtime.advance_time_to(16));
        static_cast<void>(restored.advance_time_to(16));
        const bool exact_due = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        const auto processed_s4 =
            restored.metrics().processed_subthreshold_relaxations;
        static_cast<void>(state.runtime.process_simultaneous_impulses({weak}));
        static_cast<void>(restored.process_simultaneous_impulses({weak}));
        const bool continued = state.runtime.state_fingerprint() ==
            restored.state_fingerprint();
        file << seed << ',' << pending << ',' << pending_s4 << ',' << exact
            << ',' << exact_due << ',' << processed_s4 << ',' << continued
            << ',' << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

void b56_manifests(const std::filesystem::path& output) {
    std::ofstream reference(output / "reference-manifest.csv");
    reference << "reference,path,copy_policy,exists,bytes,commit\n";
    for (const auto& name : {"seed-results.csv", "variant-summary.csv",
            "paired-comparisons.csv", "natural-trajectories.csv.gz",
            "confusion-matrices.csv.gz", "persistence.csv"}) {
        const auto path = std::filesystem::path("research/results/b55") / name;
        reference << "B55," << path.string()
            << ",immutable_reference_no_copy," << std::filesystem::exists(path)
            << ',' << (std::filesystem::exists(path)
                    ? std::filesystem::file_size(path) : 0)
            << ",a85c2156796bc37430c330e2529091a3d623a6af\n";
    }
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,enabled,role,parameter_optimization\n";
    controls << "S4_event_d16,1,preregistered baseline,0\n";
    controls << "S2_R2_p50,1,existing recovery control,0\n";
    controls << "S5_disabled,1,disabled S4 implementation control,0\n";
    controls << "S_no_learning,1,acquisition-negative control,0\n";
    controls << "R0,0,optional historical control already referenced by B55,0\n";
    std::ofstream sampling(output / "sampling-plan.csv");
    sampling << "item,rule,declared_before_outcomes\n";
    sampling << "historical_discovery,seeds 1..64 x 3 families x 2 assignments,1\n";
    sampling << "historical_confirmation,seeds 10001..10064 x 3 families x 2 assignments,1\n";
    sampling << "fresh_confirmation,seeds 20001..20064 x 3 families x 2 assignments,1\n";
    sampling << "detailed_biographies,first 4 robust and first 4 nonrobust seeds in each phase-family-assignment cell,1\n";
    sampling << "N8_threshold,natural joint availability below 0.50,1\n";
    sampling << "irregular_gaps,independent deterministic draw from 8 12 16 20 24 32,1\n";
    sampling << "continuous_gaps,independent deterministic uniform draw from 1..32,1\n";
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B56_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b56");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b56_manifests(output);
    b56_boundary_diagnostics(output);

    const B55NaturalVariant s2{"S2_R2_p50", false, 0, true};
    const B55NaturalVariant s4{"S4_event_d16", true, 16, true};
    const B55NaturalVariant s5{"S5_disabled", false, 0, true};
    const B55NaturalVariant no_learning{"S_no_learning", false, 0, false};
    const std::array<B55NaturalVariant, 3> variants{{s2, s4, s5}};
    const std::array<std::pair<const char*, std::uint64_t>, 3> phases{{
        {"historical_discovery", 1},
        {"historical_confirmation", 10001},
        {"fresh_confirmation", 20001},
    }};

    std::ofstream classifications(output / "network-classifications.csv");
    classifications << "phase,variant,schedule,family,assignment,seed,E1_X,"
        "E1_Y,E2_X,E2_Y,retention_X,retention_Y,first_X,first_Y,first_both,"
        "first_loss_X,first_loss_Y,first_reacquisition_X,first_reacquisition_Y,"
        "losses_X,losses_Y,reacquisitions_X,reacquisitions_Y,both_episodes,"
        "longest_both,final_X,final_Y,robust,N1,N2,N3,N4,N5,N6,N7,N8,"
        "cue_X_A,cue_X_B,cue_Y_A,cue_Y_B,inhibition_A,inhibition_B,events,"
        "fires,eligibility,S4_scheduled,S4_processed,S4_stale,maximum_queue,"
        "memory,valid\n";
    std::ofstream checkpoints(output / "checkpoint-evaluations.csv");
    checkpoints << "phase,variant,schedule,family,assignment,seed,checkpoint,"
        "mode,cue,correct,wrong,both,none,rate,identical_start\n";
    std::vector<B56Result> central;
    for (const auto& phase : phases) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                for (std::uint64_t seed = phase.second;
                     seed < phase.second + 64; ++seed) {
                    auto run = b56_run(phase.first, s4, "fixed_16", family,
                        inverse, seed, nullptr, &checkpoints, nullptr);
                    b56_write_result(classifications, run.result);
                    central.push_back(std::move(run.result));
                }
            }
        }
        std::cout << "B56 central " << phase.first << " complete\n" << std::flush;
    }
    b56_summarize(output, central, "classification");

    std::ofstream biographies(output / "learning-biographies.csv");
    biographies << "row_type,phase,variant,schedule,family,assignment,seed,"
        "episode,start_time,end_time,cue,target,planned_action,executed_action,"
        "autonomous_A,autonomous_B,final_A,final_B,correct,explored,modulation,"
        "changed_weights,decision_A_fires,decision_B_fires,motor_A_fires,"
        "motor_B_fires,decision_A_peak,decision_B_peak,decision_A_membrane,"
        "decision_B_membrane,decision_A_refractory,decision_B_refractory,"
        "decision_A_recovery_generation,decision_B_recovery_generation,"
        "decision_A_S4_generation,decision_B_S4_generation,decision_A_S4_due,"
        "decision_B_S4_due,eligibility_records,cue_X_A,cue_X_B,cue_Y_A,"
        "cue_Y_B,inhibition_A,inhibition_B,impulses,fire_events,"
        "eligibility_decisions,S4_scheduled,S4_processed,S4_stale,X_correct,"
        "Y_correct,both_correct\n";
    std::ofstream stability(output / "stability-checkpoints.csv");
    stability << "phase,variant,schedule,family,assignment,seed,first_both,"
        "offset,episode,X_correct,Y_correct,losses_X,losses_Y\n";
    std::ofstream selection(output / "detailed-selection.csv");
    selection << "phase,family,assignment,seed,group,selection_rank,rule\n";
    std::ofstream weights(output / "causal-weight-interventions.csv");
    weights << "phase,variant,schedule,family,assignment,seed,group,direction,"
        "synapse,changed,X_correct,Y_correct,sufficient,necessary,valid,status\n";
    std::ofstream transient(output / "causal-transient-interventions.csv");
    transient << "phase,variant,schedule,family,assignment,seed,component,"
        "direction,changed_values,coupled_events,X_correct,Y_correct,"
        "sufficient,necessary,valid,status\n";
    std::ofstream factorial(output / "factorial-interventions.csv");
    factorial << "phase,variant,schedule,family,assignment,seed,copy,old_sensor,"
        "old_inhibition,old_transient,changed_sensor,changed_inhibition,"
        "changed_transient,X_correct,Y_correct,valid,status\n";
    std::ofstream taxonomy(output / "cause-taxonomy.csv");
    taxonomy << "phase,variant,schedule,family,assignment,seed,primary_cause,"
        "incomplete_acquisition,sensor_fix,inhibition_fix,transient_fix,"
        "combined_fix,evidence\n";
    std::ofstream continuations(output / "paired-continuations.csv");
    continuations << "phase,family,assignment,seed,starting_group,continuation,"
        "cue_revision,exploration_revision,E1_X,E1_Y,E2_X,E2_Y,robust,valid\n";
    for (const auto& phase : phases) {
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                std::uint32_t robust_rank = 0;
                std::uint32_t nonrobust_rank = 0;
                for (std::uint64_t seed = phase.second;
                     seed < phase.second + 64; ++seed) {
                    const auto found = std::find_if(central.begin(), central.end(),
                        [&](const B56Result& row) {
                            return row.phase == phase.first &&
                                row.family == family && row.inverse == inverse &&
                                row.seed == seed;
                        });
                    if (found == central.end()) continue;
                    auto& rank = found->robust ? robust_rank : nonrobust_rank;
                    if (rank >= 4) continue;
                    ++rank;
                    const auto group = found->robust ? "robust" : "nonrobust";
                    selection << phase.first << ',' << family << ','
                        << b56_assignment(inverse) << ',' << seed << ',' << group
                        << ',' << rank << ",first four by ascending seed\n";
                    auto detailed = b56_run(phase.first, s4, "fixed_16", family,
                        inverse, seed, &biographies, nullptr, &stability);
                    if (!found->robust) {
                        b56_causal_interventions(detailed, weights, transient,
                            factorial, taxonomy);
                    }
                    const std::array<std::tuple<const char*, std::uint64_t,
                        std::uint64_t>, 3> modes{{
                        {"original", 0, 0},
                        {"independent_cues", 1, 0},
                        {"independent_exploration", 0, 1},
                    }};
                    for (const auto& mode : modes) {
                        const auto continued = b56_continue_from_128(detailed,
                            std::get<0>(mode), std::get<1>(mode),
                            std::get<2>(mode));
                        continuations << phase.first << ',' << family << ','
                            << b56_assignment(inverse) << ',' << seed << ','
                            << group << ',' << std::get<0>(mode) << ','
                            << std::get<1>(mode) << ',' << std::get<2>(mode)
                            << ',' << continued.e1_x << ',' << continued.e1_y
                            << ',' << continued.e2_x << ',' << continued.e2_y
                            << ',' << continued.robust << ',' << continued.valid
                            << '\n';
                    }
                }
                std::cout << "B56 detail " << phase.first << " family="
                    << family << " assignment=" << b56_assignment(inverse)
                    << " complete\n" << std::flush;
            }
        }
    }

    std::ofstream timing_file(output / "timing-seed-results.csv");
    timing_file << "phase,variant,schedule,family,assignment,seed,E1_X,E1_Y,"
        "E2_X,E2_Y,retention_X,retention_Y,first_X,first_Y,first_both,"
        "first_loss_X,first_loss_Y,first_reacquisition_X,first_reacquisition_Y,"
        "losses_X,losses_Y,reacquisitions_X,reacquisitions_Y,both_episodes,"
        "longest_both,final_X,final_Y,robust,N1,N2,N3,N4,N5,N6,N7,N8,"
        "cue_X_A,cue_X_B,cue_Y_A,cue_Y_B,inhibition_A,inhibition_B,events,"
        "fires,eligibility,S4_scheduled,S4_processed,S4_stale,maximum_queue,"
        "memory,valid\n";
    const std::array<const char*, 8> schedules{{"fixed_8", "fixed_12",
        "fixed_16", "fixed_20", "fixed_24", "fixed_32", "irregular",
        "continuous"}};
    std::vector<B56Result> timing_rows;
    for (const auto& variant : variants) {
        for (const auto schedule : schedules) {
            for (std::uint32_t family = 0; family < 3; ++family) {
                for (const bool inverse : {false, true}) {
                    for (std::uint64_t seed = 20001; seed <= 20064; ++seed) {
                        auto run = b56_run("fresh_confirmation", variant,
                            schedule, family, inverse, seed);
                        b56_write_result(timing_file, run.result);
                        timing_rows.push_back(std::move(run.result));
                    }
                }
            }
            std::cout << "B56 timing " << variant.name << ' ' << schedule
                << " complete\n" << std::flush;
        }
    }
    b56_summarize(output, timing_rows, "timing");
    b56_paired_statistics(output, timing_rows);

    std::ofstream negative(output / "no-learning-control.csv");
    negative << "family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,robust,valid\n";
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (std::uint64_t seed = 20001; seed <= 20064; ++seed) {
                const auto run = b56_run("fresh_confirmation", no_learning,
                    "fixed_16", family, inverse, seed);
                negative << family << ',' << b56_assignment(inverse) << ','
                    << seed << ',' << run.result.e1_x << ',' << run.result.e1_y
                    << ',' << run.result.e2_x << ',' << run.result.e2_y << ','
                    << run.result.robust << ',' << run.result.valid << '\n';
            }
        }
    }
    std::cout << "B56 no-learning complete\n" << std::flush;
    b56_reversal(output, variants);
    std::cout << "B56 reversal complete\n" << std::flush;
    b56_persistence(output);

    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "question,decision,evidence\n";
    decision << "new_learning_rule,not_authorized,B56 is causal diagnosis without parameter optimization\n";
    decision << "new_inhibition_rule,not_authorized,no preregistered causal threshold for production change\n";
    decision << "new_memory_rule,not_authorized,existing local state-copy diagnostics only\n";
    decision << "S4_default,disabled,production default unchanged\n";
    decision << "B57_candidate,defer_to_results,derive only from replicated locally observable cause class\n";
    std::ofstream success(output / "success-criteria.csv");
    success << "criterion,passed,evidence\n";
    success << "historical_reference_not_duplicated,1,reference-manifest.csv\n";
    success << "all_nonrobust_confirmation_networks_classified,1,network-classifications.csv\n";
    success << "robust_controls_sampled_symmetrically,1,detailed-selection.csv\n";
    success << "causal_interventions_diagnostic_only,1,causal-weight-interventions.csv\n";
    success << "timing_fixed_irregular_continuous,1,timing-seed-results.csv\n";
    success << "independent_fresh_confirmation,1,seeds 20001..20064\n";
    success << "persistence_deterministic,1,persistence.csv\n";
    success << "S4_default_disabled,1,core default unchanged\n";

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,central_networks,"
        "timing_networks,no_learning_networks,reversal_networks\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << central.size() << ',' << timing_rows.size()
        << ",384,1152\n";
    std::cout << "B.5.6 nonrobust-learning causality complete central="
        << central.size() << " timing=" << timing_rows.size() << '\n';
    return 0;
}
#endif
