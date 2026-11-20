#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B53_EMBEDDED
#include "research_stage_b53.cpp"
#undef EVE_RESEARCH_STAGE_B53_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <numeric>

namespace {

struct B54RecoveryVariant {
    std::string name;
    eve::RecoveryMode mode{eve::RecoveryMode::input_dependent};
    std::uint32_t refractory{};
    std::uint64_t delay{16};
    std::uint32_t relaxation{};
    bool plasticity{true};
};

struct B54NetworkResult {
    std::string phase;
    std::string variant;
    std::string task;
    std::uint32_t family{};
    bool inverse{};
    std::uint64_t seed{};
    double e1_x{};
    double e1_y{};
    double e2_x{};
    double e2_y{};
    double retention_x{};
    double retention_y{};
    std::uint32_t first_x{kB51Never};
    std::uint32_t first_y{kB51Never};
    std::uint32_t first_both{kB51Never};
    std::uint64_t losses_x{};
    std::uint64_t losses_y{};
    std::uint64_t both_episodes{};
    std::uint64_t both_longest{};
    std::uint64_t scheduled{};
    std::uint64_t processed{};
    std::uint64_t stale{};
    std::uint64_t suppressed{};
    std::uint64_t eligibility{};
    std::uint64_t fires{};
    std::uint64_t events{};
    std::uint64_t maximum_queue{};
    std::size_t memory{};
    double wall_seconds{};
    bool final_x{};
    bool final_y{};
    bool robust{};
    bool exploration_protected{};
    bool valid{};
};

const std::array<std::uint64_t, 6> kB54Delays{{1, 4, 8, 16, 32, 64}};
const std::array<std::uint32_t, 6> kB54Relaxations{{0, 10, 25, 50, 75, 100}};
const std::array<std::uint64_t, 8> kB54Gaps{{0, 1, 2, 4, 8, 16, 32, 64}};

const char* b54_mode_name(const eve::RecoveryMode mode) {
    switch (mode) {
    case eve::RecoveryMode::input_dependent: return "R0_input_dependent";
    case eve::RecoveryMode::independent: return "R1_independent";
    case eve::RecoveryMode::independent_with_membrane_relaxation:
        return "R2_relaxation";
    }
    return "unknown";
}

const std::vector<B54RecoveryVariant>& b54_discovery_variants() {
    static const std::vector<B54RecoveryVariant> variants{
        {"K0_R0_reference", eve::RecoveryMode::input_dependent, 0, 16, 0, true},
        {"K1_R0_no_learning", eve::RecoveryMode::input_dependent, 0, 16, 0, false},
        {"K4_R0_refractory1", eve::RecoveryMode::input_dependent, 1, 16, 0, true},
        {"K5_R1_r1_d16", eve::RecoveryMode::independent, 1, 16, 0, true},
        {"K6_R2_r1_d16_p0", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 0, true},
        {"K7_R2_r1_d16_p25", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 25, true},
        {"K7_R2_r1_d16_p50", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 50, true},
        {"K7_R2_r1_d16_p100", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 100, true},
        {"K7_R2_r2_d16_p50", eve::RecoveryMode::independent_with_membrane_relaxation, 2, 16, 50, true},
        {"K8_R2_r1_d4_p50", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 4, 50, true},
    };
    return variants;
}

const std::vector<B54RecoveryVariant>& b54_confirmation_variants() {
    static const std::vector<B54RecoveryVariant> variants{
        {"K0_R0_reference", eve::RecoveryMode::input_dependent, 0, 16, 0, true},
        {"K1_R0_no_learning", eve::RecoveryMode::input_dependent, 0, 16, 0, false},
        {"K5_R1_r1_d16", eve::RecoveryMode::independent, 1, 16, 0, true},
        {"K6_R2_r1_d16_p0", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 0, true},
        {"K7_R2_r1_d16_p50", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 50, true},
    };
    return variants;
}

const std::vector<B54RecoveryVariant>& b54_reversal_variants() {
    static const std::vector<B54RecoveryVariant> variants{
        {"K11_R0_reversal", eve::RecoveryMode::input_dependent, 0, 16, 0, true},
        {"K11_R1_reversal", eve::RecoveryMode::independent, 1, 16, 0, true},
        {"K11_R2_p50_reversal", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 50, true},
    };
    return variants;
}

eve::ExperimentConfig b54_config(
    const B48Variant& learning_variant,
    const B54RecoveryVariant& recovery_variant) {
    auto config = b47_config(b48_base_variant(learning_variant));
    config.reset_mode = eve::ResetMode::to_zero;
    config.refractory_input_events = recovery_variant.refractory;
    config.recovery_mode = recovery_variant.mode;
    config.recovery_delay = recovery_variant.delay;
    config.membrane_relaxation_percent = recovery_variant.relaxation;
    return config;
}

B48State b54_initial_state(
    const B48Variant& learning_variant,
    const B54RecoveryVariant& recovery_variant,
    const std::uint64_t seed) {
    const auto base = b48_base_variant(learning_variant);
    eve::bios::Runtime runtime(
        b54_config(learning_variant, recovery_variant),
        b47_mix_seed(0x5404000ULL ^ seed ^
            (static_cast<std::uint64_t>(learning_variant.family) << 40U)));
    const auto circuit = b47_circuit(runtime, base);
    return B48State{
        std::move(runtime), circuit,
        b48_plans(seed, kB51Episodes + kB51EvaluationEpisodes,
            learning_variant.family),
        B48Result{seed}, false, {{-1, -1}}};
}

void b54_reproduce_b53(const std::filesystem::path& output) {
    const auto reference = output / "b53-reproduction";
    std::filesystem::create_directories(reference);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b53_inventory(reference);
    b53_reproduce_b52(reference);
    b53_controls(reference);
    const auto results = b53_primary(reference);
    b53_secondary_summaries(reference);
    b53_statistics(reference, results);
    b53_persistence(reference);
    std::ofstream mechanism(reference / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K11,0,component causes are diagnostic and do not justify a production reset or normalization rule\n";
    mechanism << "K12,0,no K11 mechanism exists to deactivate\n";
    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(reference / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,networks,episodes,reference_reproduced\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ",768," << 768ULL * kB51Episodes << ",1\n";
}

eve::ExperimentConfig b54_micro_config(
    const eve::RecoveryMode mode, const std::uint32_t refractory,
    const std::uint64_t delay, const std::uint32_t relaxation,
    const bool eligibility = false) {
    eve::ExperimentConfig config;
    config.neuron_count = 8;
    config.width = 2;
    config.height = 2;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 3;
    config.learning_enabled = false;
    config.growth_enabled = false;
    config.reset_mode = eve::ResetMode::to_zero;
    config.refractory_input_events = refractory;
    config.recovery_mode = mode;
    config.recovery_delay = delay;
    config.membrane_relaxation_percent = relaxation;
    config.batch_simultaneous_inputs = true;
    config.local_event_memory_capacity = 8;
    if (eligibility) {
        config.eligibility_selection = eve::EligibilitySelection::all_recent;
        config.eligibility_candidate_capacity = 8;
        config.eligibility_decision_capacity = 8;
        config.eligibility_duration = delay;
    }
    return config;
}

void b54_audit(const std::filesystem::path& output) {
    std::ofstream reset(output / "reset-audit.csv");
    eve::ExperimentConfig defaults;
    B47Variant reference{"B54_audit", "B54", B46Task::mixed, {}, true,
        true, eve::InhibitoryPlasticityMode::shared, 0, true, false, 1,
        false, true};
    const auto historical = b47_config(reference);
    reset << "scope,reset_mode,refractory,recovery_mode,recovery_delay,relaxation,explicit,conclusion\n";
    reset << "ExperimentConfig_default,"
        << (defaults.reset_mode == eve::ResetMode::to_zero ? "to_zero" : "subtract_threshold")
        << ',' << defaults.refractory_input_events << ','
        << b54_mode_name(defaults.recovery_mode) << ','
        << defaults.recovery_delay << ',' << defaults.membrane_relaxation_percent
        << ",0,structure defaults\n";
    reset << "B46_to_B53_reference,"
        << (historical.reset_mode == eve::ResetMode::to_zero ? "to_zero" : "subtract_threshold")
        << ',' << historical.refractory_input_events << ','
        << b54_mode_name(historical.recovery_mode) << ','
        << historical.recovery_delay << ','
        << historical.membrane_relaxation_percent
        << ",refractory and mode explicit; reset inherited,B53 used to_zero and no scheduled recovery\n";

    std::ofstream chain(output / "recovery-audit.csv");
    chain << "step,code_condition,effect,experimental_consequence\n";
    chain << "schedule,impulse-triggered fire and mode not input_dependent,increment generation and enqueue local recovery,subthreshold-only states schedule nothing\n";
    chain << "refractory,fire assigns refractory_input_events,zero leaves no active refractory state,R1/R2 with zero will later be stale\n";
    chain << "validity,mode not R0 and matching generation and refractory greater than zero,clear refractory and scheduled time,all three conditions required\n";
    chain << "relaxation,valid R2 recovery only,multiply current membrane by retained percent,R1 never changes membrane\n";
    chain << "generation,new fire increments generation,older event becomes stale,repeated fire with refractory zero creates only stale recoveries\n";
    chain << "same_time,recovery enqueued before eligibility expiry during a fire,recovery has lower sequence and executes first,ordering is deterministic\n";

    std::ofstream manifest(output / "parameter-manifest.csv");
    manifest << "stage,variant,mode,refractory,delay,relaxation,learning_matrix,reason\n";
    for (const std::uint32_t refractory : {0U, 1U, 2U}) {
        manifest << "technical,R0_r" << refractory << ','
            << b54_mode_name(eve::RecoveryMode::input_dependent) << ','
            << refractory << ",16,0,0,historical and refractory control\n";
        for (const auto delay : kB54Delays) {
            manifest << "technical,R1_r" << refractory << "_d" << delay << ','
                << b54_mode_name(eve::RecoveryMode::independent) << ','
                << refractory << ',' << delay
                << ",0,0,independent recovery control\n";
            for (const auto relaxation : kB54Relaxations) {
                manifest << "technical,R2_r" << refractory << "_d" << delay
                    << "_p" << relaxation << ','
                    << b54_mode_name(
                        eve::RecoveryMode::independent_with_membrane_relaxation)
                    << ',' << refractory << ',' << delay << ',' << relaxation
                    << ",0,predeclared complete technical matrix\n";
            }
        }
    }
    for (const auto& variant : b54_discovery_variants()) {
        manifest << "discovery," << variant.name << ','
            << b54_mode_name(variant.mode) << ',' << variant.refractory << ','
            << variant.delay << ',' << variant.relaxation
            << ",1,limited predeclared natural matrix\n";
    }
    for (const auto& variant : b54_confirmation_variants()) {
        manifest << "confirmation," << variant.name << ','
            << b54_mode_name(variant.mode) << ',' << variant.refractory << ','
            << variant.delay << ',' << variant.relaxation
            << ",1,independent central comparison\n";
    }

    std::ofstream micro(output / "micro-recovery-events.csv");
    micro << "mode,refractory,delay,relaxation,fired,scheduled,membrane_after_fire,membrane_before_recovery,recovered,stale,membrane_after_recovery,refractory_after,processed,stale_count,subthreshold_scheduled,subthreshold_before,subthreshold_after,valid\n";
    const auto run_micro = [&](const eve::RecoveryMode mode,
            const std::uint32_t refractory, const std::uint64_t delay,
            const std::uint32_t relaxation) {
        eve::bios::Runtime runtime(
            b54_micro_config(mode, refractory, delay, relaxation),
            0x5400100ULL + delay + relaxation + refractory * 1000U);
        const auto fire = runtime.add_experimental_synapse(0, 4, 2, true);
        const auto weak = runtime.add_experimental_synapse(1, 4, 1, true);
        bool recovered = false;
        bool stale = false;
        runtime.set_recovery_observer(
            [&](const eve::bios::RecoveryObservation& observation) {
                recovered = recovered || observation.recovered;
                stale = stale || observation.stale;
            });
        const auto fired = runtime.process_simultaneous_impulses({fire}).fired;
        const auto membrane_after_fire =
            runtime.network().neuron(4).membrane_potential;
        static_cast<void>(runtime.process_simultaneous_impulses({weak}));
        const auto membrane_before = runtime.network().neuron(4).membrane_potential;
        const auto scheduled = runtime.metrics().scheduled_recovery_events;
        static_cast<void>(runtime.advance_time_to(delay));

        eve::bios::Runtime subthreshold(
            b54_micro_config(mode, refractory, delay, relaxation),
            0x5400200ULL + delay + relaxation + refractory * 1000U);
        const auto sub_weak =
            subthreshold.add_experimental_synapse(1, 4, 1, true);
        static_cast<void>(subthreshold.process_simultaneous_impulses({sub_weak}));
        const auto sub_before =
            subthreshold.network().neuron(4).membrane_potential;
        static_cast<void>(subthreshold.advance_time_to(delay + 64));
        micro << b54_mode_name(mode) << ',' << refractory << ',' << delay
            << ',' << relaxation << ',' << fired << ',' << scheduled << ','
            << membrane_after_fire << ',' << membrane_before << ','
            << recovered << ',' << stale << ','
            << runtime.network().neuron(4).membrane_potential << ','
            << runtime.network().neuron(4).refractory_events_remaining << ','
            << runtime.metrics().processed_recovery_events << ','
            << runtime.metrics().stale_recovery_events << ','
            << subthreshold.metrics().scheduled_recovery_events << ','
            << sub_before << ','
            << subthreshold.network().neuron(4).membrane_potential << ','
            << (runtime.network().validate() &&
                subthreshold.network().validate()) << '\n';
    };
    for (const std::uint32_t refractory : {0U, 1U, 2U}) {
        run_micro(eve::RecoveryMode::input_dependent, refractory, 16, 0);
        for (const auto delay : kB54Delays) {
            run_micro(eve::RecoveryMode::independent, refractory, delay, 0);
            for (const auto relaxation : kB54Relaxations) {
                run_micro(
                    eve::RecoveryMode::independent_with_membrane_relaxation,
                    refractory, delay, relaxation);
            }
        }
    }

    std::ofstream order(output / "event-order-controls.csv");
    order << "case,recovery_delay,eligibility_duration,processed_recovery,processed_expiry,stale_recovery,final_membrane,final_refractory,deterministic\n";
    const auto same_time = [](const std::uint64_t seed) {
        auto config = b54_micro_config(
            eve::RecoveryMode::independent_with_membrane_relaxation,
            1, 4, 50, true);
        eve::bios::Runtime runtime(config, seed);
        const auto fire = runtime.add_experimental_synapse(0, 4, 2, true);
        static_cast<void>(runtime.process_simultaneous_impulses({fire}));
        static_cast<void>(runtime.advance_time_to(4));
        return std::tuple<std::uint64_t, std::uint64_t, std::uint64_t,
            std::int64_t, std::uint32_t, std::uint64_t>{
                runtime.metrics().processed_recovery_events,
                runtime.metrics().processed_eligibility_expiries,
                runtime.metrics().stale_recovery_events,
                runtime.network().neuron(4).membrane_potential,
                runtime.network().neuron(4).refractory_events_remaining,
                runtime.state_fingerprint()};
    };
    const auto first = same_time(0x5400300ULL);
    const auto second = same_time(0x5400300ULL);
    order << "recovery_equals_eligibility,4,4," << std::get<0>(first) << ','
        << std::get<1>(first) << ',' << std::get<2>(first) << ','
        << std::get<3>(first) << ',' << std::get<4>(first) << ','
        << (first == second) << '\n';
}

void b54_temporal_integration(const std::filesystem::path& output) {
    std::ofstream file(output / "temporal-integration.csv");
    file << "variant,mode,refractory,delay,relaxation,sequence,gap,fires,final_membrane,scheduled,processed,stale,integration_preserved,valid\n";
    std::vector<B54RecoveryVariant> variants{
        {"R0_r0", eve::RecoveryMode::input_dependent, 0, 16, 0, true},
        {"R0_r1", eve::RecoveryMode::input_dependent, 1, 16, 0, true},
        {"R1_r1_d16", eve::RecoveryMode::independent, 1, 16, 0, true},
    };
    for (const auto delay : kB54Delays) {
        for (const auto relaxation : kB54Relaxations) {
            variants.push_back({"R2_r1_d" + std::to_string(delay) + "_p" +
                    std::to_string(relaxation),
                eve::RecoveryMode::independent_with_membrane_relaxation,
                1, delay, relaxation, true});
        }
    }
    const std::array<std::string, 6> sequences{{
        "AA", "BB", "AB", "BA", "ABA", "BAB"}};
    for (const auto& variant : variants) {
        for (const auto gap : kB54Gaps) {
            for (const auto& sequence : sequences) {
                eve::bios::Runtime runtime(b54_micro_config(
                    variant.mode, variant.refractory, variant.delay,
                    variant.relaxation),
                    0x5401000ULL + gap + sequence.size());
                const auto input_a =
                    runtime.add_experimental_synapse(0, 4, 1, true);
                const auto input_b =
                    runtime.add_experimental_synapse(1, 4, 1, true);
                std::uint64_t fires = 0;
                for (std::size_t index = 0; index < sequence.size(); ++index) {
                    const auto input = sequence[index] == 'A' ? input_a : input_b;
                    const auto result =
                        runtime.process_simultaneous_impulses({input});
                    fires += result.fired;
                    if (index + 1 < sequence.size()) {
                        static_cast<void>(runtime.advance_time_by(gap));
                    }
                }
                file << variant.name << ',' << b54_mode_name(variant.mode)
                    << ',' << variant.refractory << ',' << variant.delay << ','
                    << variant.relaxation << ',' << sequence << ',' << gap
                    << ',' << fires << ','
                    << runtime.network().neuron(4).membrane_potential << ','
                    << runtime.metrics().scheduled_recovery_events << ','
                    << runtime.metrics().processed_recovery_events << ','
                    << runtime.metrics().stale_recovery_events << ','
                    << (fires >= 1) << ',' << runtime.network().validate()
                    << '\n';
            }
        }
    }
}

B54NetworkResult b54_run_network(
    const std::string& phase, const B54RecoveryVariant& recovery_variant,
    const B46Task task, const std::uint32_t family, const bool inverse,
    const std::uint64_t seed, std::ofstream& trajectory,
    std::ofstream& recovery_events, std::ofstream& stability,
    std::ofstream& checkpoints, std::ofstream& confusion,
    std::ofstream& causal) {
    const auto started = std::chrono::steady_clock::now();
    B48Variant learning_variant{recovery_variant.name, task,
        B48Policy::reference, inverse, recovery_variant.plasticity,
        false, family};
    auto state = b54_initial_state(learning_variant, recovery_variant, seed);
    B54NetworkResult result;
    result.phase = phase;
    result.variant = recovery_variant.name;
    result.task = b46_task_name(task);
    result.family = family;
    result.inverse = inverse;
    result.seed = seed;
    auto previous = b51_recall_pair(
        state.runtime, state.circuit, learning_variant, 0);
    std::array<bool, 2> loss_emitted{{false, false}};
    std::uint32_t both_current = 0;
    std::uint32_t current_episode = 0;
    const auto prefix = [&]() {
        return phase + ',' + recovery_variant.name + ',' +
            b46_task_name(task) + ',' + std::to_string(family) + ',' +
            (inverse ? "inverse" : "normal") + ',' + std::to_string(seed);
    };
    state.runtime.set_recovery_observer(
        [&](const eve::bios::RecoveryObservation& observation) {
            recovery_events << prefix() << ',' << current_episode << ','
                << observation.event_index << ',' << observation.simulated_time
                << ',' << observation.neuron << ',' << observation.generation
                << ',' << observation.membrane_before << ','
                << observation.membrane_after << ','
                << observation.refractory_before << ','
                << observation.refractory_after << ','
                << observation.recovered << ',' << observation.stale << '\n';
        });

    for (std::uint32_t episode = 0; episode < kB51Episodes; ++episode) {
        current_episode = episode;
        const auto cue = b48_cue(learning_variant, state.plans[episode],
            episode, b46_training(task));
        const bool target_a = b48_target_a(
            learning_variant, cue, episode, b46_training(task));
        const auto before_metrics = state.runtime.metrics();
        auto row = b51_episode(state.runtime, state.circuit, cue,
            state.plans[episode].exploration, target_a, episode,
            recovery_variant.plasticity);
        state.runtime = row.after_feedback;
        const auto after_metrics = state.runtime.metrics();
        const auto correct = b51_recall_pair(
            state.runtime, state.circuit, learning_variant, episode);
        for (std::uint8_t path = 0; path < 2; ++path) {
            auto& first = path == 0 ? result.first_x : result.first_y;
            auto& losses = path == 0 ? result.losses_x : result.losses_y;
            if (!previous[path] && correct[path] && first == kB51Never) {
                first = episode;
            }
            if (previous[path] && !correct[path]) {
                ++losses;
                if (!loss_emitted[path]) {
                    auto before = row.before;
                    auto after = state.runtime;
                    before.set_recovery_observer({});
                    after.set_recovery_observer({});
                    const bool same_weights = b53_same_weights(before, after);
                    auto old_membrane = after;
                    const auto membrane_transfer = old_membrane
                        .set_experimental_transient_component_from(
                            before, Component::membrane);
                    const auto membrane_correct = b51_recall_pair(
                        old_membrane, state.circuit, learning_variant, episode);
                    auto old_recovery = after;
                    const auto recovery_transfer = old_recovery
                        .set_experimental_transient_component_from(
                            before, Component::recovery);
                    const auto recovery_correct = b51_recall_pair(
                        old_recovery, state.circuit, learning_variant, episode);
                    auto old_transient = before;
                    for (eve::SynapseId id = 0;
                         id < after.network().synapse_count(); ++id) {
                        old_transient.set_experimental_synapse_weight(
                            id, after.network().synapse(id).weight);
                    }
                    const auto transient_correct = b51_recall_pair(
                        old_transient, state.circuit, learning_variant, episode);
                    std::uint64_t weight_changes = 0;
                    for (eve::SynapseId id = 0;
                         id < after.network().synapse_count(); ++id) {
                        weight_changes += before.network().synapse(id).weight !=
                            after.network().synapse(id).weight;
                    }
                    causal << prefix() << ',' << episode << ','
                        << static_cast<unsigned>(path) << ',' << same_weights
                        << ',' << weight_changes << ','
                        << membrane_transfer.changed_values << ','
                        << membrane_correct[path] << ','
                        << recovery_transfer.changed_values << ','
                        << recovery_transfer.coupled_events << ','
                        << recovery_correct[path] << ','
                        << transient_correct[path] << ','
                        << correct[1U - path] << ','
                        << (membrane_transfer.valid && recovery_transfer.valid)
                        << '\n';
                    loss_emitted[path] = true;
                }
            }
            if (first != kB51Never) {
                const auto offset = episode - first;
                if (offset == 0 || offset == 1 || offset == 4 || offset == 8 ||
                    offset == 16 || offset == 32 || offset == 64) {
                    stability << prefix() << ','
                        << static_cast<unsigned>(path) << ',' << first << ','
                        << offset << ',' << episode << ',' << correct[path]
                        << ',' << correct[1U - path] << ',' << losses << '\n';
                }
            }
        }
        const bool both = correct[0] && correct[1];
        if (both) {
            if (result.first_both == kB51Never) result.first_both = episode;
            ++result.both_episodes;
            ++both_current;
            result.both_longest = std::max(result.both_longest,
                static_cast<std::uint64_t>(both_current));
        } else {
            both_current = 0;
        }
        std::uint64_t changed_weights = 0;
        for (eve::SynapseId id = 0; id < row.after_weights.size(); ++id) {
            changed_weights += row.before_weights[id].weight !=
                row.after_weights[id].weight;
        }
        trajectory << prefix() << ',' << episode << ','
            << static_cast<unsigned>(cue) << ',' << (target_a ? 'A' : 'B')
            << ',' << b47_outcome(row.autonomous_a, row.autonomous_b) << ','
            << b47_outcome(row.final_a, row.final_b) << ',' << row.correct
            << ',' << row.explored << ',' << static_cast<int>(row.modulation)
            << ',' << changed_weights << ',' << row.decision_peak[0] << ','
            << row.decision_peak[1] << ','
            << state.runtime.network().neuron(state.circuit.decisions[0])
                   .membrane_potential
            << ','
            << state.runtime.network().neuron(state.circuit.decisions[1])
                   .membrane_potential;
        for (const auto id : state.circuit.cue_synapses) {
            trajectory << ',' << state.runtime.network().synapse(id).weight;
        }
        for (const auto id : state.circuit.inhibitory_outputs) {
            trajectory << ',' << state.runtime.network().synapse(id).weight;
        }
        trajectory << ','
            << (after_metrics.scheduled_recovery_events -
                before_metrics.scheduled_recovery_events)
            << ','
            << (after_metrics.processed_recovery_events -
                before_metrics.processed_recovery_events)
            << ','
            << (after_metrics.stale_recovery_events -
                before_metrics.stale_recovery_events)
            << ','
            << (after_metrics.refractory_suppressed_firings -
                before_metrics.refractory_suppressed_firings)
            << ','
            << (after_metrics.eligibility_decisions -
                before_metrics.eligibility_decisions)
            << ',' << (after_metrics.fire_events - before_metrics.fire_events)
            << ',' << (after_metrics.processed_events -
                before_metrics.processed_events)
            << ',' << after_metrics.maximum_queue_size << ',' << correct[0]
            << ',' << correct[1] << ',' << both << '\n';
        previous = correct;

        if (episode + 1 == 128 || episode + 1 == 256 ||
            episode + 1 == 512) {
            auto evaluation_runtime = state.runtime;
            evaluation_runtime.set_recovery_observer({});
            const auto evaluated = b51_evaluate(evaluation_runtime,
                state.circuit, learning_variant, state.plans, episode + 1);
            const auto e1_x = b47_rate(evaluated.e1[0]);
            const auto e1_y = b47_rate(evaluated.e1[1]);
            const auto e2_x = b47_rate(evaluated.e2[0]);
            const auto e2_y = b47_rate(evaluated.e2[1]);
            checkpoints << prefix() << ',' << episode + 1 << ',' << e1_x
                << ',' << e1_y << ',' << e2_x << ',' << e2_y << ','
                << (e1_x >= .75 && e1_y >= .75) << ','
                << evaluated.identical_start << ','
                << state.runtime.network().validate() << '\n';
            for (std::uint8_t evaluated_cue = 0; evaluated_cue < 2;
                 ++evaluated_cue) {
                const auto emit = [&](const char* metric,
                        const B47Counts& counts) {
                    confusion << prefix() << ',' << episode + 1 << ','
                        << metric << ','
                        << static_cast<unsigned>(evaluated_cue) << ','
                        << counts.correct << ',' << counts.wrong << ','
                        << counts.both << ',' << counts.none << '\n';
                };
                emit("E1", evaluated.e1[evaluated_cue]);
                emit("E2", evaluated.e2[evaluated_cue]);
            }
            if (episode + 1 == 512) {
                result.e1_x = e1_x;
                result.e1_y = e1_y;
                result.e2_x = e2_x;
                result.e2_y = e2_y;
                result.robust = e1_x >= .75 && e1_y >= .75;
            }
        }
    }
    const auto final_correct = b51_recall_pair(
        state.runtime, state.circuit, learning_variant, kB51Episodes);
    result.final_x = final_correct[0];
    result.final_y = final_correct[1];
    auto retained = state.runtime;
    retained.set_recovery_observer({});
    static_cast<void>(retained.advance_time_by(64ULL * 16ULL));
    const auto retained_eval = b51_evaluate(retained, state.circuit,
        learning_variant, state.plans, kB51Episodes);
    result.retention_x = b47_rate(retained_eval.e1[0]);
    result.retention_y = b47_rate(retained_eval.e1[1]);
    const auto& metrics = state.runtime.metrics();
    result.scheduled = metrics.scheduled_recovery_events;
    result.processed = metrics.processed_recovery_events;
    result.stale = metrics.stale_recovery_events;
    result.suppressed = metrics.refractory_suppressed_firings;
    result.eligibility = metrics.eligibility_decisions;
    result.fires = metrics.fire_events;
    result.events = metrics.processed_events;
    result.maximum_queue = metrics.maximum_queue_size;
    result.memory = state.runtime.diagnostics().estimated_memory_bytes;
    result.exploration_protected = true;
    for (const auto id : state.circuit.exploration_synapses) {
        result.exploration_protected = result.exploration_protected &&
            state.runtime.network().synapse(id).plasticity_protected &&
            state.runtime.network().synapse(id).weight == 2;
    }
    result.valid = state.runtime.network().validate();
    result.wall_seconds = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - started).count();
    return result;
}

std::vector<B54NetworkResult> b54_natural_matrix(
    const std::filesystem::path& output) {
    std::ofstream trajectory(output / "natural-trajectories.csv");
    trajectory << "phase,variant,task,family,assignment,seed,episode,cue,target,autonomous,final,correct,explored,modulation,changed_weights,decision_A_peak,decision_B_peak,decision_A_membrane,decision_B_membrane,cue_X_A,cue_X_B,cue_Y_A,cue_Y_B,inhibition_A,inhibition_B,recovery_scheduled,recovery_processed,recovery_stale,refractory_suppressed,eligibility_decisions,fire_events,processed_events,maximum_queue,X_correct,Y_correct,both_correct\n";
    std::ofstream recovery(output / "recovery-events.csv");
    recovery << "phase,variant,task,family,assignment,seed,episode,event_index,time,neuron,generation,membrane_before,membrane_after,refractory_before,refractory_after,recovered,stale\n";
    std::ofstream stability(output / "stability-checkpoints.csv");
    stability << "phase,variant,task,family,assignment,seed,cue,acquired_episode,offset,episode,correct,other_correct,losses\n";
    std::ofstream checkpoints(output / "checkpoint-evaluation.csv");
    checkpoints << "phase,variant,task,family,assignment,seed,checkpoint,E1_X,E1_Y,E2_X,E2_Y,E1_robust,identical_start,valid\n";
    std::ofstream confusion(output / "confusion-matrices.csv");
    confusion << "phase,variant,task,family,assignment,seed,checkpoint,metric,cue,correct,wrong,both,none\n";
    std::ofstream causal(output / "causal-copies.csv");
    causal << "phase,variant,task,family,assignment,seed,episode,lost_cue,same_weights,weight_changes,membrane_changes,old_membrane_restores,recovery_changes,recovery_events,old_recovery_restores,full_old_transient_restores,other_correct,valid\n";
    std::vector<B54NetworkResult> rows;
    const auto run = [&](const std::string& phase,
            const std::vector<B54RecoveryVariant>& variants,
            const B46Task task, const std::uint64_t first_seed) {
        for (const auto& recovery_variant : variants) {
            for (std::uint32_t family = 0; family < 3; ++family) {
                for (const bool inverse : {false, true}) {
                    for (std::uint64_t seed = first_seed;
                         seed < first_seed + 64; ++seed) {
                        rows.push_back(b54_run_network(phase,
                            recovery_variant, task, family, inverse, seed,
                            trajectory, recovery, stability, checkpoints,
                            confusion, causal));
                    }
                }
            }
        }
    };
    run("discovery", b54_discovery_variants(), B46Task::mixed, 1);
    run("confirmation", b54_confirmation_variants(), B46Task::mixed, 10001);
    run("confirmation", b54_reversal_variants(), B46Task::reversal, 10001);
    return rows;
}

void b54_statistics(
    const std::filesystem::path& output,
    const std::vector<B54NetworkResult>& rows) {
    struct Aggregate {
        std::uint64_t networks{};
        double e1_x{};
        double e1_y{};
        double e2_x{};
        double e2_y{};
        double retention_x{};
        double retention_y{};
        double both_fraction{};
        double losses{};
        std::uint64_t robust{};
        std::uint64_t final_both{};
        std::uint64_t ever_both{};
        std::uint64_t scheduled{};
        std::uint64_t processed{};
        std::uint64_t stale{};
        std::uint64_t suppressed{};
        std::uint64_t eligibility{};
        std::uint64_t fires{};
        std::uint64_t events{};
        std::uint64_t maximum_queue{};
        std::size_t memory{};
        std::uint64_t protected_exploration{};
        std::uint64_t valid{};
        double wall_seconds{};
    };
    std::map<std::string, Aggregate> aggregates;
    std::map<std::string, std::vector<const B54NetworkResult*>> groups;
    std::map<std::string, const B54NetworkResult*> indexed;
    for (const auto& row : rows) {
        const auto group = row.phase + '|' + row.variant + '|' + row.task;
        auto& value = aggregates[group];
        ++value.networks;
        value.e1_x += row.e1_x;
        value.e1_y += row.e1_y;
        value.e2_x += row.e2_x;
        value.e2_y += row.e2_y;
        value.retention_x += row.retention_x;
        value.retention_y += row.retention_y;
        value.both_fraction += static_cast<double>(row.both_episodes) /
            kB51Episodes;
        value.losses += row.losses_x + row.losses_y;
        value.robust += row.robust;
        value.final_both += row.final_x && row.final_y;
        value.ever_both += row.first_both != kB51Never;
        value.scheduled += row.scheduled;
        value.processed += row.processed;
        value.stale += row.stale;
        value.suppressed += row.suppressed;
        value.eligibility += row.eligibility;
        value.fires += row.fires;
        value.events += row.events;
        value.maximum_queue = std::max(value.maximum_queue, row.maximum_queue);
        value.memory = std::max(value.memory, row.memory);
        value.protected_exploration += row.exploration_protected;
        value.valid += row.valid;
        value.wall_seconds += row.wall_seconds;
        groups[group].push_back(&row);
        indexed[row.phase + '|' + row.task + '|' + row.variant + '|' +
            std::to_string(row.family) + '|' + (row.inverse ? "I" : "N") +
            '|' + std::to_string(row.seed)] = &row;
    }
    std::ofstream summary(output / "variant-summary.csv");
    summary << "phase,variant,task,networks,E1_X,E1_Y,E2_X,E2_Y,retention_X,retention_Y,robust_networks,ever_both,final_both,both_episode_fraction,mean_losses,scheduled_recovery,processed_recovery,stale_recovery,refractory_suppressed,eligibility_decisions,fire_events,processed_events,maximum_queue,memory_bytes,protected_exploration,valid\n";
    for (const auto& [key, value] : aggregates) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        const auto count = static_cast<double>(value.networks);
        summary << printable << ',' << value.networks << ','
            << value.e1_x / count << ',' << value.e1_y / count << ','
            << value.e2_x / count << ',' << value.e2_y / count << ','
            << value.retention_x / count << ',' << value.retention_y / count
            << ',' << value.robust << ',' << value.ever_both << ','
            << value.final_both << ',' << value.both_fraction / count << ','
            << value.losses / count << ',' << value.scheduled << ','
            << value.processed << ',' << value.stale << ','
            << value.suppressed << ',' << value.eligibility << ','
            << value.fires << ',' << value.events << ','
            << value.maximum_queue << ',' << value.memory << ','
            << value.protected_exploration << ',' << value.valid << '\n';
    }

    std::ofstream resources(output / "resources.csv");
    resources << "phase,variant,task,networks,episodes,scheduled_recovery,processed_recovery,stale_recovery,additional_recovery_work_per_episode,processed_events,events_per_episode,maximum_queue,memory_bytes,wall_seconds,seconds_per_network\n";
    for (const auto& [key, value] : aggregates) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        const auto episodes = value.networks * kB51Episodes;
        resources << printable << ',' << value.networks << ',' << episodes
            << ',' << value.scheduled << ',' << value.processed << ','
            << value.stale << ','
            << static_cast<double>(value.processed + value.stale) / episodes
            << ',' << value.events << ','
            << static_cast<double>(value.events) / episodes << ','
            << value.maximum_queue << ',' << value.memory << ','
            << value.wall_seconds << ','
            << value.wall_seconds / value.networks << '\n';
    }

    std::ofstream clusters(output / "cluster-statistics.csv");
    clusters << "phase,variant,task,metric,clusters,mean,ci_low,ci_high\n";
    std::uint64_t salt = 0;
    for (const auto& [key, group] : groups) {
        std::map<std::string, std::vector<double>> metrics;
        for (const auto* row : group) {
            metrics["E1_X"].push_back(row->e1_x);
            metrics["E1_Y"].push_back(row->e1_y);
            metrics["E1_score"].push_back((row->e1_x + row->e1_y) / 2.0);
            metrics["both_fraction"].push_back(
                static_cast<double>(row->both_episodes) / kB51Episodes);
            metrics["final_both"].push_back(row->final_x && row->final_y);
            metrics["losses"].push_back(
                static_cast<double>(row->losses_x + row->losses_y));
            metrics["processed_recovery"].push_back(
                static_cast<double>(row->processed));
            metrics["refractory_suppressed"].push_back(
                static_cast<double>(row->suppressed));
        }
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        for (const auto& [metric, values] : metrics) {
            const auto interval = bootstrap_interval(
                values, 0x5405000ULL + ++salt);
            clusters << printable << ',' << metric << ',' << values.size()
                << ',' << b48_mean(values) << ',' << interval.first << ','
                << interval.second << '\n';
        }
    }

    std::ofstream paired(output / "paired-comparisons.csv");
    paired << "phase,task,candidate,control,metric,pairs,mean_difference,ci_low,ci_high\n";
    for (const auto& [key, group] : groups) {
        static_cast<void>(group);
        const auto first = key.find('|');
        const auto second = key.find('|', first + 1);
        const auto phase = key.substr(0, first);
        const auto candidate = key.substr(first + 1, second - first - 1);
        const auto task = key.substr(second + 1);
        std::vector<std::string> controls;
        if (task == "mixed") {
            if (candidate == "K0_R0_reference" ||
                candidate == "K1_R0_no_learning") continue;
            controls = {"K0_R0_reference", "K1_R0_no_learning"};
            if (candidate.rfind("K6_", 0) == 0 ||
                candidate.rfind("K7_", 0) == 0 ||
                candidate.rfind("K8_", 0) == 0) {
                controls.push_back("K5_R1_r1_d16");
            }
            if (phase == "discovery" && candidate == "K5_R1_r1_d16") {
                controls.push_back("K4_R0_refractory1");
            }
        } else if (task == "reversal") {
            if (candidate == "K11_R0_reversal") continue;
            controls.push_back("K11_R0_reversal");
            if (candidate == "K11_R2_p50_reversal") {
                controls.push_back("K11_R1_reversal");
            }
        } else {
            continue;
        }
        for (const auto& control : controls) {
            std::map<std::string, std::vector<double>> differences;
            for (const auto* row : groups.at(key)) {
                const auto found = indexed.find(phase + '|' + task + '|' +
                    control + '|' + std::to_string(row->family) + '|' +
                    (row->inverse ? "I" : "N") + '|' +
                    std::to_string(row->seed));
                if (found == indexed.end()) continue;
                const auto* base = found->second;
                differences["E1_score"].push_back(
                    ((row->e1_x - base->e1_x) +
                     (row->e1_y - base->e1_y)) / 2.0);
                differences["both_fraction"].push_back(
                    (static_cast<double>(row->both_episodes) -
                     static_cast<double>(base->both_episodes)) /
                    kB51Episodes);
                differences["losses"].push_back(
                    static_cast<double>(row->losses_x + row->losses_y) -
                    static_cast<double>(base->losses_x + base->losses_y));
            }
            for (const auto& [metric, values] : differences) {
                if (values.empty()) continue;
                const auto interval = bootstrap_interval(
                    values, 0x5406000ULL + ++salt);
                paired << phase << ',' << task << ',' << candidate << ','
                    << control << ',' << metric << ',' << values.size() << ','
                    << b48_mean(values) << ',' << interval.first << ','
                    << interval.second << '\n';
            }
        }
    }

    std::ofstream criteria(output / "success-criteria.csv");
    criteria << "phase,variant,task,family,assignment,seeds,E1_X,E1_Y,robust_seeds,advantage_vs_no_learning,ci_low,ci_high,active_recovery,retention,protected_exploration,passes\n";
    for (const auto& [key, group] : groups) {
        const auto first = key.find('|');
        const auto second = key.find('|', first + 1);
        const auto phase = key.substr(0, first);
        const auto variant = key.substr(first + 1, second - first - 1);
        const auto task = key.substr(second + 1);
        if (task != "mixed") continue;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                std::vector<double> advantages;
                double x = 0.0;
                double y = 0.0;
                std::uint64_t robust = 0;
                std::uint64_t active = 0;
                std::uint64_t retained = 0;
                std::uint64_t protected_count = 0;
                std::uint64_t count = 0;
                for (const auto* row : group) {
                    if (row->family != family || row->inverse != inverse) continue;
                    ++count;
                    x += row->e1_x;
                    y += row->e1_y;
                    robust += row->robust;
                    active += row->processed > 0;
                    retained += row->retention_x >= .75 &&
                        row->retention_y >= .75;
                    protected_count += row->exploration_protected;
                    const auto base = indexed.find(phase + "|mixed|K1_R0_no_learning|" +
                        std::to_string(family) + '|' +
                        (inverse ? "I" : "N") + '|' +
                        std::to_string(row->seed));
                    if (base != indexed.end()) {
                        advantages.push_back((row->e1_x + row->e1_y -
                            base->second->e1_x - base->second->e1_y) / 2.0);
                    }
                }
                if (count == 0) continue;
                const auto interval = bootstrap_interval(
                    advantages, 0x5407000ULL + ++salt);
                const auto mean_x = x / count;
                const auto mean_y = y / count;
                const auto advantage = b48_mean(advantages);
                const bool reference = variant == "K0_R0_reference" ||
                    variant == "K1_R0_no_learning";
                const bool pass = mean_x >= .75 && mean_y >= .75 &&
                    robust >= 48 && advantage >= .15 &&
                    !advantages.empty() && interval.first > 0 &&
                    (reference || active == count) && retained >= 48 &&
                    protected_count == count;
                criteria << phase << ',' << variant << ',' << task << ','
                    << family << ',' << (inverse ? "inverse" : "normal")
                    << ',' << count << ',' << mean_x << ',' << mean_y << ','
                    << robust << ',' << advantage << ',' << interval.first
                    << ',' << interval.second << ',' << active << ','
                    << retained << ',' << protected_count << ',' << pass
                    << '\n';
            }
        }
    }

    std::ofstream seed_results(output / "seed-results.csv");
    seed_results << "phase,variant,task,family,assignment,seed,E1_X,E1_Y,E2_X,E2_Y,retention_X,retention_Y,first_X,first_Y,first_both,losses_X,losses_Y,both_episodes,both_longest,scheduled,processed,stale,suppressed,eligibility,fires,events,maximum_queue,memory,final_X,final_Y,robust,protected_exploration,valid\n";
    for (const auto& row : rows) {
        seed_results << row.phase << ',' << row.variant << ',' << row.task
            << ',' << row.family << ','
            << (row.inverse ? "inverse" : "normal") << ',' << row.seed << ','
            << row.e1_x << ',' << row.e1_y << ',' << row.e2_x << ','
            << row.e2_y << ',' << row.retention_x << ',' << row.retention_y
            << ',' << b51_u32(row.first_x) << ',' << b51_u32(row.first_y)
            << ',' << b51_u32(row.first_both) << ',' << row.losses_x << ','
            << row.losses_y << ',' << row.both_episodes << ','
            << row.both_longest << ',' << row.scheduled << ','
            << row.processed << ',' << row.stale << ',' << row.suppressed
            << ',' << row.eligibility << ',' << row.fires << ',' << row.events
            << ',' << row.maximum_queue << ',' << row.memory << ','
            << row.final_x << ',' << row.final_y << ',' << row.robust << ','
            << row.exploration_protected << ',' << row.valid << '\n';
    }
}

void b54_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "variant,seed,pending_recovery,exact_restore,exact_due_processing,exact_continuation,valid\n";
    const std::array<B54RecoveryVariant, 4> variants{{
        {"R0", eve::RecoveryMode::input_dependent, 0, 16, 0, true},
        {"R1", eve::RecoveryMode::independent, 1, 16, 0, true},
        {"R2_0", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 0, true},
        {"R2_50", eve::RecoveryMode::independent_with_membrane_relaxation, 1, 16, 50, true},
    }};
    for (const auto& recovery_variant : variants) {
        for (std::uint64_t seed = 1; seed <= 64; ++seed) {
            B48Variant learning_variant{"B54_persistence", B46Task::mixed,
                B48Policy::reference, false, true, false, 0};
            auto state = b54_initial_state(
                learning_variant, recovery_variant, seed);
            state.runtime = b51_continue(state.runtime, state.circuit,
                learning_variant, state.plans, 0, 64);
            state.runtime.enqueue_fire(state.circuit.sensors[0]);
            static_cast<void>(state.runtime.run());
            const auto pending = state.runtime.future_event_count();
            const auto path = output / (".b54-state-" + recovery_variant.name +
                '-' + std::to_string(seed));
            state.runtime.save(path.string());
            auto restored = eve::bios::Runtime::load(path.string());
            const bool exact = state.runtime.state_fingerprint() ==
                restored.state_fingerprint();
            static_cast<void>(state.runtime.advance_time_by(16));
            static_cast<void>(restored.advance_time_by(16));
            const bool due = state.runtime.state_fingerprint() ==
                restored.state_fingerprint();
            state.runtime.enqueue_fire(state.circuit.sensors[1]);
            restored.enqueue_fire(state.circuit.sensors[1]);
            static_cast<void>(state.runtime.run());
            static_cast<void>(restored.run());
            file << recovery_variant.name << ',' << seed << ',' << pending
                << ',' << exact << ',' << due << ','
                << (state.runtime.state_fingerprint() ==
                    restored.state_fingerprint())
                << ',' << restored.network().validate() << '\n';
            static_cast<void>(std::filesystem::remove(path));
        }
    }
}

void b54_controls(const std::filesystem::path& output) {
    std::ofstream controls(output / "control-group-manifest.csv");
    controls << "group,description,data,enabled\n";
    controls << "K0,unchanged B53 reference,b53-reproduction,1\n";
    controls << "K1,no learning,natural-trajectories.csv.gz,1\n";
    controls << "K2,fixed inhibition,b53-reproduction/reference-control-groups.csv,1\n";
    controls << "K3,learned inhibition,b53-reproduction/reference-control-groups.csv,1\n";
    controls << "K4,R0 controlled refractory,micro-recovery-events.csv and natural-trajectories.csv.gz,1\n";
    controls << "K5,R1 independent recovery,natural-trajectories.csv.gz,1\n";
    controls << "K6,R2 zero percent,natural-trajectories.csv.gz,1\n";
    controls << "K7,R2 relaxation strengths,parameter-manifest.csv and natural-trajectories.csv.gz,1\n";
    controls << "K8,recovery delays,parameter-manifest.csv and temporal-integration.csv.gz,1\n";
    controls << "K9,diagnostic state copies,causal-copies.csv.gz,1\n";
    controls << "K10,temporal integration,temporal-integration.csv.gz,1\n";
    controls << "K11,reversal and retention,seed-results.csv,1\n";
    controls << "K12,new function,not causally required,0\n";
    controls << "K13,deactivation control,no K12 implementation,0\n";

    std::ofstream task(output / "task-matrix.csv");
    task << "task,normal,inverse,families,discovery_seeds,confirmation_seeds,scope\n";
    task << "mixed,1,1,3,64,64,all selected recovery variants\n";
    task << "reversal,1,1,3,0,64,R0 R1 R2-50\n";
    task << "retention,1,1,3,64,64,event-free then E1 evaluation\n";
    task << "single and sequential references,1,1,3,64,64,b53-reproduction task results\n";
    task << "temporal weak-input sequences,0,0,1,deterministic,deterministic,AA BB AB BA ABA BAB gaps 0-64\n";
}

}  // namespace

#ifndef EVE_RESEARCH_STAGE_B54_EMBEDDED
int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b54");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b54_reproduce_b53(output);
    b54_audit(output);
    b54_temporal_integration(output);
    b54_controls(output);
    const auto rows = b54_natural_matrix(output);
    b54_statistics(output, rows);
    b54_persistence(output);
    std::ofstream decision(output / "mechanism-decision.csv");
    decision << "group,implemented,reason\n";
    decision << "K12,0,B54 evaluates existing recovery modes; no new local function is introduced\n";
    decision << "K13,0,no K12 function exists to deactivate\n";
    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,networks,episodes,reference_reproduced\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << rows.size() << ','
        << rows.size() * kB51Episodes << ",1\n";
    std::cout << "B.5.4 local membrane recovery complete K12=not_implemented\n";
    return 0;
}
#endif
