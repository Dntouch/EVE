#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B47_EMBEDDED
#include "research_stage_b47.cpp"
#undef EVE_RESEARCH_STAGE_B47_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <map>
#include <set>

namespace {

enum class B48Policy {
    reference,
    adaptive_after_negative,
    random_extra,
    replayed_timing,
};

struct B48Plan {
    std::uint8_t cue{};
    std::uint8_t exploration{};
    std::uint8_t extra_gate{};
};

struct B48Variant {
    std::string name;
    B46Task task{B46Task::mixed};
    B48Policy policy{B48Policy::reference};
    bool inverse{};
    bool plasticity{true};
    bool fixed_inhibition{};
    std::uint32_t family{};
};

struct B48Result {
    std::uint64_t seed{};
    std::array<B47Counts, 2> autonomous{};
    std::array<B47Counts, 2> supported{};
    std::uint64_t exploration_a{};
    std::uint64_t exploration_b{};
    std::uint64_t protected_explorations{};
    std::uint64_t extra_explorations{};
    std::array<std::uint64_t, 3> exploration_segments{};
    std::uint64_t successful_explorations{};
    std::uint64_t unsuccessful_explorations{};
    std::uint64_t negative_feedback{};
    std::uint64_t repeated_wrong{};
    std::uint64_t alternative_after_negative{};
    std::uint64_t autonomous_episodes{};
    std::uint64_t no_response_episodes{};
    std::array<std::int32_t, 4> cue_weights{};
    std::array<std::int32_t, 2> inhibition_weights{};
    std::uint64_t events{};
    std::uint64_t maximum_queue{};
    std::size_t memory{};
    bool valid{};
};

struct B48State {
    eve::bios::Runtime runtime;
    B47Circuit circuit;
    std::vector<B48Plan> plans;
    B48Result result;
    bool negative_readiness{};
    std::array<int, 2> pending_wrong{{-1, -1}};
};

struct B48Episode {
    std::uint8_t cue{};
    std::uint8_t action{2};
    bool target_a{};
    bool autonomous_a{};
    bool autonomous_b{};
    bool final_a{};
    bool final_b{};
    bool explored{};
    bool extra{};
    bool correct{};
    std::int32_t changed_synapses{};
};

const char* b48_policy_name(const B48Policy policy) {
    switch (policy) {
    case B48Policy::reference: return "protected_only";
    case B48Policy::adaptive_after_negative: return "adaptive_after_negative";
    case B48Policy::random_extra: return "random_extra";
    case B48Policy::replayed_timing: return "replayed_timing";
    }
    return "unknown";
}

std::vector<B48Plan> b48_plans(
    const std::uint64_t seed, const std::size_t count,
    const std::uint32_t family, const std::uint64_t cue_revision = 0,
    const std::uint64_t exploration_revision = 0,
    const bool complement_exploration = false) {
    std::vector<B48Plan> result;
    result.reserve(count);
    if (family == 0 && cue_revision == 0 && exploration_revision == 0) {
        const auto reference = b47_plans(seed, count, true);
        eve::bios::DeterministicRng gate_rng(
            b47_mix_seed(0x4803000ULL ^ seed));
        for (const auto& row : reference) {
            result.push_back(B48Plan{
                row.cue,
                static_cast<std::uint8_t>(row.exploration ^
                    static_cast<std::uint8_t>(complement_exploration)),
                static_cast<std::uint8_t>(gate_rng.bounded(4)),
            });
        }
        return result;
    }
    const auto family_key = b47_mix_seed(
        0x4800000ULL ^ (static_cast<std::uint64_t>(family) << 32U));
    eve::bios::DeterministicRng cue_rng(b47_mix_seed(
        family_key ^ 0x4801000ULL ^ seed ^
        (cue_revision * 0x9e3779b97f4a7c15ULL)));
    eve::bios::DeterministicRng exploration_rng(b47_mix_seed(
        family_key ^ 0x4802000ULL ^
        (seed * 0xd1342543de82ef95ULL) ^
        (exploration_revision * 0x94d049bb133111ebULL)));
    eve::bios::DeterministicRng gate_rng(b47_mix_seed(
        family_key ^ 0x4803000ULL ^ seed));
    for (std::size_t index = 0; index < count; ++index) {
        result.push_back(B48Plan{
            static_cast<std::uint8_t>(cue_rng.bounded(2)),
            static_cast<std::uint8_t>(exploration_rng.bounded(2) ^
                static_cast<std::uint32_t>(complement_exploration)),
            static_cast<std::uint8_t>(gate_rng.bounded(4)),
        });
    }
    return result;
}

B47Variant b48_base_variant(const B48Variant& variant) {
    return B47Variant{
        variant.name, "B48", variant.task, {}, true,
        !variant.fixed_inhibition,
        eve::InhibitoryPlasticityMode::shared,
        variant.fixed_inhibition ? 1 : 0,
        variant.plasticity, variant.inverse, 1, false, true};
}

bool b48_target_a(
    const B48Variant& variant, const std::uint8_t cue,
    const std::uint32_t episode, const std::uint32_t training) {
    return b47_target_a(b48_base_variant(variant), cue, episode, training);
}

std::uint8_t b48_cue(
    const B48Variant& variant, const B48Plan& plan,
    const std::uint32_t episode, const std::uint32_t training) {
    return b46_cue(
        variant.task, B46Plan{plan.cue, plan.exploration}, episode, training);
}

B48State b48_initial_state(const B48Variant& variant, const std::uint64_t seed) {
    const auto base = b48_base_variant(variant);
    eve::bios::Runtime runtime(b47_config(base),
        b47_mix_seed(0x4804000ULL ^ seed ^
            (static_cast<std::uint64_t>(variant.family) << 40U)));
    const auto circuit = b47_circuit(runtime, base);
    return B48State{
        std::move(runtime), circuit,
        b48_plans(seed, b46_training(variant.task) + 64, variant.family),
        B48Result{seed}, false, {{-1, -1}}};
}

B48Episode b48_episode(
    B48State& state, const B48Variant& variant,
    const std::uint32_t episode, const std::vector<bool>* replay_schedule,
    std::vector<bool>* observed_schedule, std::ostream* episodes,
    std::ostream* negative_events) {
    const std::uint32_t training = b46_training(variant.task);
    const std::uint64_t start = static_cast<std::uint64_t>(episode) * 16ULL;
    if (state.runtime.simulated_time() < start) {
        static_cast<void>(state.runtime.advance_time_to(start));
    }
    bool fired_a = false;
    bool fired_b = false;
    state.runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == state.circuit.motors[0];
        fired_b = fired_b || event.neuron == state.circuit.motors[1];
    });
    const auto before = state.runtime.network().synapses();
    const auto before_metrics = state.runtime.metrics();
    const auto cue = b48_cue(variant, state.plans[episode], episode, training);
    const bool target_a = b48_target_a(variant, cue, episode, training);
    state.runtime.enqueue_fire(state.circuit.sensors[cue]);
    static_cast<void>(state.runtime.run());
    const bool autonomous_a = fired_a;
    const bool autonomous_b = fired_b;
    state.result.autonomous_episodes += autonomous_a || autonomous_b;
    state.result.no_response_episodes += !autonomous_a && !autonomous_b;

    bool explore = !autonomous_a && !autonomous_b;
    bool extra = false;
    if (!explore) {
        if (variant.policy == B48Policy::adaptive_after_negative) {
            extra = state.negative_readiness;
        } else if (variant.policy == B48Policy::random_extra) {
            extra = state.plans[episode].extra_gate == 0;
        } else if (variant.policy == B48Policy::replayed_timing &&
                   replay_schedule != nullptr && episode < replay_schedule->size()) {
            const auto target_count = static_cast<std::uint64_t>(std::count(
                replay_schedule->begin(), replay_schedule->end(), true));
            const auto expected_by_now =
                (static_cast<std::uint64_t>(episode + 1) * target_count) /
                replay_schedule->size();
            extra = state.result.extra_explorations < expected_by_now;
        }
        explore = extra;
    }
    if (observed_schedule != nullptr) {
        if (observed_schedule->size() <= episode) {
            observed_schedule->resize(episode + 1);
        }
        (*observed_schedule)[episode] = extra;
    }
    std::uint8_t action = 2;
    if (explore) {
        action = state.plans[episode].exploration;
        state.runtime.enqueue_fire(state.circuit.explorers[action]);
        static_cast<void>(state.runtime.run());
        state.result.exploration_a += action == 0;
        state.result.exploration_b += action == 1;
        state.result.protected_explorations += !extra;
        state.result.extra_explorations += extra;
        const auto segment = std::min<std::size_t>(
            2, static_cast<std::size_t>(episode) * 3 / training);
        ++state.result.exploration_segments[segment];
    }
    const bool correct = target_a
        ? fired_a && !fired_b : fired_b && !fired_a;
    const std::int8_t polarity = correct ? 1 : -1;
    if (!correct) ++state.result.negative_feedback;
    if (explore) {
        state.result.successful_explorations += correct;
        state.result.unsuccessful_explorations += !correct;
    }
    if (variant.plasticity) {
        static_cast<void>(state.runtime.process_modulatory_feedback(polarity));
    }
    std::int32_t changed = 0;
    for (eve::SynapseId id = 0; id < state.runtime.network().synapse_count(); ++id) {
        changed += before[id].weight != state.runtime.network().synapse(id).weight;
    }

    if (state.pending_wrong[cue] >= 0) {
        const auto previous = static_cast<std::uint8_t>(state.pending_wrong[cue]);
        const bool same_wrong = (previous == 0 && fired_a && !fired_b) ||
            (previous == 1 && fired_b && !fired_a);
        state.result.repeated_wrong += same_wrong;
        state.result.alternative_after_negative += explore && action != previous;
        if (negative_events != nullptr) {
            *negative_events << "next_same_cue," << variant.name << ','
                << variant.family << ','
                << state.result.seed << ',' << episode << ','
                << static_cast<unsigned>(cue) << ','
                << static_cast<unsigned>(previous) << ','
                << b47_outcome(autonomous_a, autonomous_b) << ','
                << same_wrong << ',' << explore << ','
                << static_cast<unsigned>(action) << ',' << changed
                << ",0,0,0,0,0\n";
        }
        state.pending_wrong[cue] = -1;
    }
    if (!correct && (fired_a != fired_b)) {
        state.pending_wrong[cue] = fired_a ? 0 : 1;
    }
    if (!correct && negative_events != nullptr) {
        const auto current_action = fired_a != fired_b
            ? static_cast<unsigned>(fired_a ? 0 : 1) : 2U;
        const auto left = state.circuit.cue_synapses[cue * 2];
        const auto right = state.circuit.cue_synapses[cue * 2 + 1];
        *negative_events << "negative_feedback," << variant.name << ','
            << variant.family << ',' << state.result.seed << ',' << episode
            << ',' << static_cast<unsigned>(cue) << ',' << current_action << ','
            << b47_outcome(fired_a, fired_b) << ",0," << explore << ','
            << static_cast<unsigned>(action) << ',' << changed << ','
            << state.runtime.metrics().eligibility_candidates -
                   before_metrics.eligibility_candidates << ','
            << before[left].weight << ',' << before[right].weight << ','
            << state.runtime.network().synapse(left).weight << ','
            << state.runtime.network().synapse(right).weight << '\n';
    }
    state.negative_readiness = !correct;

    if (episodes != nullptr) {
        *episodes << variant.name << ',' << variant.family << ','
            << state.result.seed << ',' << episode << ','
            << static_cast<unsigned>(cue) << ',' << (target_a ? 'A' : 'B') << ','
            << b47_outcome(autonomous_a, autonomous_b) << ','
            << b47_outcome(fired_a, fired_b) << ',' << explore << ',' << extra
            << ',' << static_cast<unsigned>(action) << ',' << polarity << ','
            << changed << ',' << state.negative_readiness << '\n';
    }
    state.runtime.set_fire_observer({});
    state.runtime.set_impulse_observer({});
    static_cast<void>(state.runtime.advance_time_to(start + 16));
    return B48Episode{cue, action, target_a, autonomous_a, autonomous_b,
        fired_a, fired_b, explore, extra, correct, changed};
}

void b48_train_range(
    B48State& state, const B48Variant& variant,
    const std::uint32_t begin, const std::uint32_t end,
    const std::vector<bool>* replay_schedule = nullptr,
    std::vector<bool>* observed_schedule = nullptr,
    std::ostream* episodes = nullptr, std::ostream* negative_events = nullptr) {
    for (std::uint32_t episode = begin; episode < end; ++episode) {
        static_cast<void>(b48_episode(
            state, variant, episode, replay_schedule, observed_schedule,
            episodes, negative_events));
    }
}

B48Result b48_evaluate(
    eve::bios::Runtime runtime, const B47Circuit& circuit,
    const B48Variant& variant, const std::uint64_t seed,
    const std::vector<B48Plan>& plans, const std::uint32_t start_episode,
    B48Result result, std::ostream* trials = nullptr) {
    bool fired_a = false;
    bool fired_b = false;
    runtime.set_fire_observer([&](const eve::bios::FireObservation& event) {
        fired_a = fired_a || event.neuron == circuit.motors[0];
        fired_b = fired_b || event.neuron == circuit.motors[1];
    });
    const std::uint32_t training = b46_training(variant.task);
    for (std::uint32_t offset = 0; offset < 64; ++offset) {
        const std::uint32_t episode = start_episode + offset;
        const auto plan = plans[episode % plans.size()];
        const auto cue = b48_cue(variant, plan, training + offset, training);
        const bool target_a = b48_target_a(variant, cue, training + offset, training);
        const auto start = runtime.simulated_time() + 16;
        fired_a = fired_b = false;
        runtime.enqueue_fire(circuit.sensors[cue]);
        static_cast<void>(runtime.run());
        const bool e1_a = fired_a;
        const bool e1_b = fired_b;
        b47_record(result.autonomous[cue], target_a, fired_a, fired_b);
        bool explored = false;
        if (!fired_a && !fired_b) {
            explored = true;
            runtime.enqueue_fire(circuit.explorers[plan.exploration]);
            static_cast<void>(runtime.run());
        }
        b47_record(result.supported[cue], target_a, fired_a, fired_b);
        if (trials != nullptr) {
            *trials << variant.name << ',' << variant.family << ',' << seed << ','
                << offset << ',' << static_cast<unsigned>(cue) << ','
                << (target_a ? 'A' : 'B') << ','
                << b47_outcome(e1_a, e1_b) << ','
                << b47_outcome(fired_a, fired_b) << ',' << explored << '\n';
        }
        static_cast<void>(runtime.advance_time_to(start));
    }
    for (std::size_t index = 0; index < 4; ++index) {
        result.cue_weights[index] =
            runtime.network().synapse(circuit.cue_synapses[index]).weight;
    }
    for (std::size_t index = 0; index < 2; ++index) {
        result.inhibition_weights[index] =
            runtime.network().synapse(circuit.inhibitory_outputs[index]).weight;
    }
    result.events = runtime.metrics().processed_events;
    result.maximum_queue = runtime.metrics().maximum_queue_size;
    result.memory = runtime.diagnostics().estimated_memory_bytes;
    result.valid = runtime.network().validate();
    return result;
}

B48Result b48_run(
    const B48Variant& variant, const std::uint64_t seed,
    const std::vector<bool>* replay_schedule = nullptr,
    std::vector<bool>* observed_schedule = nullptr,
    std::ostream* episodes = nullptr, std::ostream* negatives = nullptr,
    std::ostream* trials = nullptr) {
    auto state = b48_initial_state(variant, seed);
    b48_train_range(state, variant, 0, b46_training(variant.task),
        replay_schedule, observed_schedule, episodes, negatives);
    return b48_evaluate(std::move(state.runtime), state.circuit, variant,
        seed, state.plans, b46_training(variant.task), state.result, trials);
}

double b48_x(const B48Result& result, const bool autonomous) {
    return b47_rate((autonomous ? result.autonomous : result.supported)[0]);
}

double b48_y(const B48Result& result, const bool autonomous) {
    return b47_rate((autonomous ? result.autonomous : result.supported)[1]);
}

double b48_score(const B48Result& result, const bool autonomous) {
    return (b48_x(result, autonomous) + b48_y(result, autonomous)) / 2.0;
}

double b48_mean(const std::vector<double>& values) {
    return values.empty() ? 0.0 :
        std::accumulate(values.begin(), values.end(), 0.0) /
            static_cast<double>(values.size());
}

void b48_write_stream_analysis(const std::filesystem::path& output) {
    std::ofstream file(output / "stream-independence.csv");
    file << "family,seed,comparison,window,agreement,correlation,exact_complement\n";
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const auto reference = b48_plans(seed, 128, family);
            const auto new_exploration = b48_plans(seed, 128, family, 0, 1);
            const auto new_cue = b48_plans(seed, 128, family, 1, 0);
            const auto complement = b48_plans(seed, 128, family, 0, 0, true);
            const auto emit = [&](const std::string& name,
                                  const std::vector<B48Plan>& rows,
                                  const bool compare_cue) {
                for (const std::size_t window : {24U, 128U}) {
                    std::vector<double> left;
                    std::vector<double> right;
                    std::size_t agreement = 0;
                    bool exact_complement = true;
                    for (std::size_t index = 0; index < window; ++index) {
                        const auto a = compare_cue ? reference[index].cue
                                                   : reference[index].exploration;
                        const auto b = compare_cue ? rows[index].cue
                                                   : rows[index].exploration;
                        left.push_back(a);
                        right.push_back(b);
                        agreement += a == b;
                        exact_complement = exact_complement && a != b;
                    }
                    file << family << ',' << seed << ',' << name << ','
                        << window << ','
                        << static_cast<double>(agreement) / window << ','
                        << b47_correlation(left, right) << ','
                        << exact_complement << '\n';
                }
            };
            emit("same_cue_new_exploration", new_exploration, false);
            emit("same_exploration_new_cue", new_cue, true);
            emit("complemented_exploration", complement, false);
        }
    }
}

std::string b48_key(const B48Variant& variant) {
    return variant.name + "_f" + std::to_string(variant.family);
}

void b48_write_summary(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<B48Result>>& results,
    const std::map<std::string, B48Variant>& variants,
    const std::string& filename) {
    std::ofstream file(output / filename);
    file << "variant,family,task,policy,inverse,plasticity,seeds,E1_X,E1_Y,E1_score,E1_robust,E2_X,E2_Y,E2_score,E2_robust,protected_exploration,extra_exploration,negative_feedback,repeated_wrong,alternative_after_negative,events,memory,maximum_queue,invalid\n";
    for (const auto& [key, rows] : results) {
        const auto& variant = variants.at(key);
        double e1x = 0.0, e1y = 0.0, e2x = 0.0, e2y = 0.0;
        double protected_count = 0.0, extra = 0.0, negative = 0.0;
        double repeated = 0.0, alternative = 0.0, events = 0.0, memory = 0.0;
        std::uint64_t e1robust = 0, e2robust = 0, invalid = 0;
        std::uint64_t maximum_queue = 0;
        for (const auto& row : rows) {
            const auto ax = b48_x(row, true), ay = b48_y(row, true);
            const auto sx = b48_x(row, false), sy = b48_y(row, false);
            e1x += ax; e1y += ay; e2x += sx; e2y += sy;
            e1robust += ax >= .75 && ay >= .75;
            e2robust += sx >= .75 && sy >= .75;
            protected_count += row.protected_explorations;
            extra += row.extra_explorations;
            negative += row.negative_feedback;
            repeated += row.repeated_wrong;
            alternative += row.alternative_after_negative;
            events += row.events;
            memory += row.memory;
            maximum_queue = std::max(maximum_queue, row.maximum_queue);
            invalid += !row.valid;
        }
        const double count = static_cast<double>(rows.size());
        file << variant.name << ',' << variant.family << ','
            << b46_task_name(variant.task) << ','
            << b48_policy_name(variant.policy) << ',' << variant.inverse << ','
            << variant.plasticity << ',' << rows.size() << ','
            << e1x / count << ',' << e1y / count << ','
            << (e1x + e1y) / (2.0 * count) << ',' << e1robust << ','
            << e2x / count << ',' << e2y / count << ','
            << (e2x + e2y) / (2.0 * count) << ',' << e2robust << ','
            << protected_count / count << ',' << extra / count << ','
            << negative / count << ',' << repeated / count << ','
            << alternative / count << ',' << events / count << ','
            << memory / count << ',' << maximum_queue << ',' << invalid << '\n';
    }
}

void b48_write_seed_data(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<B48Result>>& results,
    const std::map<std::string, B48Variant>& variants) {
    std::ofstream seeds(output / "seed-results.csv");
    seeds << "variant,family,task,policy,inverse,plasticity,seed,E1_X,E1_Y,E1_score,E2_X,E2_Y,E2_score,E1_robust,E2_robust,exploration_A,exploration_B,protected_exploration,extra_exploration,exploration_early,exploration_middle,exploration_late,exploration_successful,exploration_unsuccessful,negative_feedback,repeated_wrong,alternative_after_negative,autonomous_episodes,no_response_episodes,cue_XA,cue_XB,cue_YA,cue_YB,inhib_AtoB,inhib_BtoA,events,maximum_queue,memory,valid\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "variant,family,seed,stage,cue,correct,wrong,both,none\n";
    for (const auto& [key, rows] : results) {
        const auto& variant = variants.at(key);
        for (const auto& row : rows) {
            const double e1x = b48_x(row, true);
            const double e1y = b48_y(row, true);
            const double e2x = b48_x(row, false);
            const double e2y = b48_y(row, false);
            seeds << variant.name << ',' << variant.family << ','
                << b46_task_name(variant.task) << ','
                << b48_policy_name(variant.policy) << ',' << variant.inverse
                << ',' << variant.plasticity << ',' << row.seed << ','
                << e1x << ',' << e1y << ',' << (e1x + e1y) / 2.0 << ','
                << e2x << ',' << e2y << ',' << (e2x + e2y) / 2.0 << ','
                << (e1x >= .75 && e1y >= .75) << ','
                << (e2x >= .75 && e2y >= .75) << ','
                << row.exploration_a << ',' << row.exploration_b << ','
                << row.protected_explorations << ',' << row.extra_explorations;
            for (const auto count : row.exploration_segments) seeds << ',' << count;
            seeds << ',' << row.successful_explorations << ','
                << row.unsuccessful_explorations << ',' << row.negative_feedback
                << ',' << row.repeated_wrong << ','
                << row.alternative_after_negative << ','
                << row.autonomous_episodes << ',' << row.no_response_episodes;
            for (const auto weight : row.cue_weights) seeds << ',' << weight;
            for (const auto weight : row.inhibition_weights) seeds << ',' << weight;
            seeds << ',' << row.events << ',' << row.maximum_queue << ','
                << row.memory << ',' << row.valid << '\n';
            for (std::size_t cue = 0; cue < 2; ++cue) {
                for (const auto stage : {std::pair<const char*, const B47Counts&>{
                         "E1", row.autonomous[cue]},
                         std::pair<const char*, const B47Counts&>{
                         "E2", row.supported[cue]}}) {
                    confusion << variant.name << ',' << variant.family << ','
                        << row.seed << ',' << stage.first << ',' << cue << ','
                        << stage.second.correct << ',' << stage.second.wrong << ','
                        << stage.second.both << ',' << stage.second.none << '\n';
                }
            }
        }
    }
}

void b48_paired(
    const std::filesystem::path& output,
    const std::map<std::string, std::vector<B48Result>>& results,
    const std::map<std::string, B48Variant>& variants) {
    std::ofstream file(output / "paired-comparisons.csv");
    file << "left,right,family,inverse,seeds,metric,mean_difference,bootstrap_low,bootstrap_high,positive_fraction\n";
    const auto compare = [&](const std::string& left, const std::string& right,
                             const std::string& metric, const std::uint64_t salt) {
        const auto& a = results.at(left);
        const auto& b = results.at(right);
        std::vector<double> differences;
        std::size_t positive = 0;
        for (std::size_t index = 0; index < a.size(); ++index) {
            const double difference = metric == "E1"
                ? b48_score(a[index], true) - b48_score(b[index], true)
                : b48_score(a[index], false) - b48_score(b[index], false);
            differences.push_back(difference);
            positive += difference > 0.0;
        }
        const auto interval = bootstrap_interval(differences, 0x4805000ULL + salt);
        const auto& variant = variants.at(left);
        file << variant.name << ',' << variants.at(right).name << ','
            << variant.family << ',' << variant.inverse << ',' << a.size() << ','
            << metric << ',' << b48_mean(differences) << ',' << interval.first
            << ',' << interval.second << ','
            << static_cast<double>(positive) / a.size() << '\n';
    };
    std::uint64_t salt = 0;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto suffix = std::string(inverse ? "inverse" : "normal") +
                "_f" + std::to_string(family);
            for (const auto& pair : {
                     std::pair<std::string, std::string>{
                         "adaptive_" + suffix, "reference_" + suffix},
                     {"adaptive_" + suffix, "no_learning_" + suffix},
                     {"adaptive_" + suffix, "random_extra_" + suffix},
                     {"adaptive_" + suffix, "replayed_timing_" + suffix}}) {
                compare(pair.first, pair.second, "E1", ++salt);
                compare(pair.first, pair.second, "E2", ++salt);
            }
        }
    }
}

void b48_snapshots_and_causality(
    const std::filesystem::path& output, std::ostream& episodes,
    std::ostream& negatives) {
    std::ofstream curves(output / "checkpoint-learning-curves.csv");
    curves << "assignment,seed,checkpoint,E1_X,E1_Y,E1_score,E2_X,E2_Y,E2_score,cue_XA,cue_XB,cue_YA,cue_YB,inhib_AtoB,inhib_BtoA\n";
    std::ofstream continuations(output / "alternative-continuations.csv");
    continuations << "assignment,seed,checkpoint,future_family,E1_score,E2_score,cue_XA,cue_XB,cue_YA,cue_YB\n";
    std::ofstream interventions(output / "causal-interventions.csv");
    interventions << "assignment,seed,checkpoint,intervention,phase,E1_score,E2_score,valid\n";
    std::ofstream persistence(output / "checkpoint-persistence.csv");
    persistence << "assignment,seed,checkpoint,exact_restore,exact_continuation,valid\n";
    const std::set<std::uint32_t> checkpoints{0, 4, 8, 16, 32, 64, 96, 128};
    for (const bool inverse : {false, true}) {
        B48Variant variant{
            std::string("snapshot_") + (inverse ? "inverse" : "normal"),
            B46Task::mixed, B48Policy::reference, inverse, true, false, 0};
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            auto state = b48_initial_state(variant, seed);
            std::map<std::uint32_t, eve::bios::Runtime> snapshots;
            snapshots.emplace(0, state.runtime);
            for (std::uint32_t episode = 0; episode < 128; ++episode) {
                static_cast<void>(b48_episode(state, variant, episode, nullptr,
                    nullptr, seed <= 4 ? &episodes : nullptr,
                    seed <= 16 ? &negatives : nullptr));
                if (checkpoints.count(episode + 1) != 0) {
                    snapshots.emplace(episode + 1, state.runtime);
                }
            }
            for (const auto& [checkpoint, runtime] : snapshots) {
                const auto evaluated = b48_evaluate(runtime, state.circuit,
                    variant, seed, state.plans, 128, B48Result{seed});
                curves << (inverse ? "inverse" : "normal") << ',' << seed << ','
                    << checkpoint << ',' << b48_x(evaluated, true) << ','
                    << b48_y(evaluated, true) << ',' << b48_score(evaluated, true)
                    << ',' << b48_x(evaluated, false) << ','
                    << b48_y(evaluated, false) << ',' << b48_score(evaluated, false);
                for (const auto weight : evaluated.cue_weights) curves << ',' << weight;
                for (const auto weight : evaluated.inhibition_weights) curves << ',' << weight;
                curves << '\n';
                if (checkpoint == 16 || checkpoint == 64) {
                    for (const auto& intervention : {
                             "control", "rollback_cues", "disable_inhibition",
                             "freeze_learned"}) {
                        auto changed = runtime;
                        if (std::string(intervention) == "rollback_cues") {
                            for (const auto id : state.circuit.cue_synapses) {
                                changed.set_experimental_synapse_weight(id, 0);
                            }
                        } else if (std::string(intervention) == "disable_inhibition") {
                            for (const auto id : state.circuit.inhibitory_outputs) {
                                changed.set_experimental_synapse_weight(id, 0);
                            }
                        } else if (std::string(intervention) == "freeze_learned") {
                            for (const auto id : state.circuit.cue_synapses) {
                                changed.set_experimental_synapse_protection(id, true);
                            }
                        }
                        const auto immediate = b48_evaluate(changed, state.circuit,
                            variant, seed, state.plans, 128, B48Result{seed});
                        interventions << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << checkpoint << ',' << intervention
                            << ",immediate," << b48_score(immediate, true) << ','
                            << b48_score(immediate, false) << ',' << immediate.valid
                            << '\n';
                        auto continued = B48State{changed, state.circuit,
                            b48_plans(seed, 192, 1), B48Result{seed}, false,
                            {{-1, -1}}};
                        b48_train_range(continued, variant, checkpoint, 128);
                        const auto after = b48_evaluate(
                            std::move(continued.runtime), state.circuit,
                            variant, seed, continued.plans, 128,
                            continued.result);
                        interventions << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << checkpoint << ',' << intervention
                            << ",after_identical_future,"
                            << b48_score(after, true) << ','
                            << b48_score(after, false) << ',' << after.valid << '\n';
                    }
                }
                if ((checkpoint == 8 || checkpoint == 32 || checkpoint == 64)) {
                    for (std::uint32_t future_family = 1; future_family <= 2;
                         ++future_family) {
                        auto continuation = B48State{runtime, state.circuit,
                            b48_plans(seed, 192, future_family), B48Result{seed},
                            false, {{-1, -1}}};
                        b48_train_range(continuation, variant, checkpoint, 128);
                        const auto result = b48_evaluate(
                            std::move(continuation.runtime), state.circuit,
                            variant, seed, continuation.plans, 128,
                            continuation.result);
                        continuations << (inverse ? "inverse" : "normal") << ','
                            << seed << ',' << checkpoint << ',' << future_family
                            << ',' << b48_score(result, true) << ','
                            << b48_score(result, false);
                        for (const auto weight : result.cue_weights) {
                            continuations << ',' << weight;
                        }
                        continuations << '\n';
                    }
                }
            }
            for (const auto checkpoint : {0U, 32U, 128U}) {
                const auto path = output / (".b48-state-" +
                    std::to_string(inverse) + "-" + std::to_string(seed) + "-" +
                    std::to_string(checkpoint));
                snapshots.at(checkpoint).save(path.string());
                auto restored = eve::bios::Runtime::load(path.string());
                const bool exact = restored.state_fingerprint() ==
                    snapshots.at(checkpoint).state_fingerprint();
                auto original = snapshots.at(checkpoint);
                static_cast<void>(original.run());
                static_cast<void>(restored.run());
                persistence << (inverse ? "inverse" : "normal") << ',' << seed
                    << ',' << checkpoint << ',' << exact << ','
                    << (original.state_fingerprint() == restored.state_fingerprint())
                    << ',' << restored.network().validate() << '\n';
                static_cast<void>(std::filesystem::remove(path));
            }
        }
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b48");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();

    b48_write_stream_analysis(output);
    std::ofstream episodes(output / "training-episodes.csv");
    episodes << "variant,family,seed,episode,cue,target,autonomous,final,explored,extra,action,feedback,changed_synapses,negative_readiness\n";
    std::ofstream negatives(output / "negative-events.csv");
    negatives << "kind,variant,family,seed,episode,cue,action,outcome,repeated_wrong,explored,explored_action,changed_synapses,eligibility_candidates,cue_to_A_before,cue_to_B_before,cue_to_A_after,cue_to_B_after\n";
    std::ofstream trials(output / "evaluation-trials.csv");
    trials << "variant,family,seed,trial,cue,target,E1_outcome,E2_outcome,explored\n";

    std::map<std::string, std::vector<B48Result>> discovery;
    std::map<std::string, B48Variant> variants;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto assignment = std::string(inverse ? "inverse" : "normal");
            std::vector<B48Variant> group{
                {"reference_" + assignment, B46Task::mixed,
                    B48Policy::reference, inverse, true, false, family},
                {"fixed_" + assignment, B46Task::mixed,
                    B48Policy::reference, inverse, true, true, family},
                {"no_learning_" + assignment, B46Task::mixed,
                    B48Policy::reference, inverse, false, true, family},
                {"adaptive_" + assignment, B46Task::mixed,
                    B48Policy::adaptive_after_negative, inverse, true, false, family},
                {"random_extra_" + assignment, B46Task::mixed,
                    B48Policy::random_extra, inverse, true, false, family},
                {"replayed_timing_" + assignment, B46Task::mixed,
                    B48Policy::replayed_timing, inverse, true, false, family},
            };
            for (const auto& variant : group) {
                variants.emplace(b48_key(variant), variant);
            }
            for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                std::vector<bool> adaptive_schedule;
                const auto adaptive = b48_run(group[3], seed, nullptr,
                    &adaptive_schedule, &episodes, &negatives, &trials);
                discovery[b48_key(group[3])].push_back(adaptive);
                for (std::size_t index = 0; index < group.size(); ++index) {
                    if (index == 3) continue;
                    const auto* replay = index == 5 ? &adaptive_schedule : nullptr;
                    discovery[b48_key(group[index])].push_back(b48_run(
                        group[index], seed, replay, nullptr,
                        &episodes, &negatives, &trials));
                }
            }
        }
    }
    b48_write_summary(output, discovery, variants, "summary.csv");
    b48_write_seed_data(output, discovery, variants);
    b48_paired(output, discovery, variants);

    std::map<std::string, std::vector<B48Result>> confirmation;
    std::map<std::string, B48Variant> confirmation_variants;
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            for (const std::uint32_t kind : {0U, 1U, 2U}) {
                const auto policy = kind == 1
                    ? B48Policy::adaptive_after_negative
                    : B48Policy::reference;
                const bool plasticity = kind != 2;
                const std::string prefix = kind == 0
                    ? "confirmation_reference_"
                    : kind == 1 ? "confirmation_adaptive_"
                                : "confirmation_no_learning_";
                B48Variant variant{
                    prefix + (inverse ? "inverse" : "normal"),
                    B46Task::mixed, policy, inverse, plasticity,
                    kind == 2, family};
                const auto key = b48_key(variant);
                confirmation_variants.emplace(key, variant);
                for (std::uint64_t seed = 10001; seed <= 10064; ++seed) {
                    confirmation[key].push_back(b48_run(variant, seed));
                }
            }
        }
    }
    b48_write_summary(
        output, confirmation, confirmation_variants, "confirmation.csv");

    std::map<std::string, std::vector<B48Result>> tasks;
    std::map<std::string, B48Variant> task_variants;
    for (const bool inverse : {false, true}) {
        for (const auto task : {B46Task::only_x, B46Task::only_y,
                 B46Task::x_then_y, B46Task::y_then_x,
                 B46Task::alternating, B46Task::mixed,
                 B46Task::reversal, B46Task::retention}) {
            for (const auto policy : {B48Policy::reference,
                                      B48Policy::adaptive_after_negative}) {
                B48Variant variant{
                    std::string(policy == B48Policy::reference ? "task_reference_"
                                                               : "task_adaptive_") +
                        (inverse ? "inverse_" : "normal_") + b46_task_name(task),
                    task, policy, inverse, true, false, 0};
                const auto key = b48_key(variant);
                task_variants.emplace(key, variant);
                for (std::uint64_t seed = 1; seed <= 64; ++seed) {
                    tasks[key].push_back(b48_run(variant, seed));
                }
            }
        }
    }
    b48_write_summary(output, tasks, task_variants, "task-matrix.csv");
    b48_snapshots_and_causality(output, episodes, negatives);

    std::ofstream reference(output / "b47-reference-reproduction.csv");
    reference << "assignment,seed,E1_exact,E2_exact,weights_exact,valid\n";
    for (const bool inverse : {false, true}) {
        const auto assignment = std::string(inverse ? "inverse" : "normal");
        B47Variant old_variant{
            "b47", "G", B46Task::mixed, {}, true, true,
            eve::InhibitoryPlasticityMode::shared, 0, true, inverse, 1, false, true};
        B48Variant new_variant{
            "b48", B46Task::mixed, B48Policy::reference,
            inverse, true, false, 0};
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            const auto old_result = b47_run_seed(old_variant, seed);
            const auto new_result = b48_run(new_variant, seed);
            bool e1 = true, e2 = true;
            for (std::size_t cue = 0; cue < 2; ++cue) {
                const auto same = [](const B47Counts& a, const B47Counts& b) {
                    return a.correct == b.correct && a.wrong == b.wrong &&
                        a.both == b.both && a.none == b.none;
                };
                e1 = e1 && same(old_result.autonomous[cue],
                                new_result.autonomous[cue]);
                e2 = e2 && same(old_result.supported[cue],
                                new_result.supported[cue]);
            }
            reference << assignment << ',' << seed << ',' << e1 << ',' << e2
                << ',' << (old_result.cue_weights == new_result.cue_weights &&
                            old_result.inhibitory_weights ==
                                new_result.inhibition_weights)
                << ',' << new_result.valid << '\n';
        }
    }

    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "phase,variant,family,assignment,E1_X,E1_Y,E1_robust,paired_advantage,ci_low,ci_high,passes\n";
    const auto write_criteria = [&](const std::string& phase,
                                    const std::uint32_t family,
                                    const bool inverse,
                                    const std::vector<B48Result>& adaptive,
                                    const std::vector<B48Result>& control,
                                    const std::uint64_t salt) {
        double x = 0.0, y = 0.0;
        std::uint64_t robust = 0;
        std::vector<double> differences;
        for (std::size_t index = 0; index < adaptive.size(); ++index) {
            const auto ax = b48_x(adaptive[index], true);
            const auto ay = b48_y(adaptive[index], true);
            x += ax; y += ay; robust += ax >= .75 && ay >= .75;
            differences.push_back(b48_score(adaptive[index], true) -
                b48_score(control[index], true));
        }
        x /= adaptive.size(); y /= adaptive.size();
        const auto interval = bootstrap_interval(differences, salt);
        const auto advantage = b48_mean(differences);
        const bool pass = x >= .75 && y >= .75 && robust >= 48 &&
            advantage >= .15 && interval.first > 0.0;
        criteria << phase << ",adaptive," << family << ','
            << (inverse ? "inverse" : "normal") << ',' << x << ',' << y
            << ',' << robust << ',' << advantage << ',' << interval.first
            << ',' << interval.second << ',' << pass << '\n';
    };
    for (std::uint32_t family = 0; family < 3; ++family) {
        for (const bool inverse : {false, true}) {
            const auto suffix = std::string(inverse ? "inverse" : "normal") +
                "_f" + std::to_string(family);
            write_criteria("discovery", family, inverse,
                discovery.at("adaptive_" + suffix),
                discovery.at("no_learning_" + suffix),
                0x4806000ULL + family * 2 + inverse);
            const auto confirmation_suffix = suffix;
            write_criteria("confirmation", family, inverse,
                confirmation.at("confirmation_adaptive_" +
                    confirmation_suffix),
                confirmation.at("confirmation_no_learning_" +
                    confirmation_suffix),
                0x4807000ULL + family * 2 + inverse);
        }
    }

    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    const double wall = std::chrono::duration<double>(ended - started).count();
    const double cpu = static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC;
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,discovery_runs,confirmation_runs,task_runs\n";
    performance << wall << ',' << cpu << ',' << usage.ru_maxrss << ','
        << 3 * 2 * 6 * 64 << ',' << 3 * 2 * 3 * 64 << ','
        << 2 * 8 * 2 * 64 << '\n';

    std::cout << "B.4.8 discovery=" << 3 * 2 * 6 * 64
              << " confirmation=" << 3 * 2 * 3 * 64
              << " task_runs=" << 2 * 8 * 2 * 64 << '\n';
    return 0;
}
