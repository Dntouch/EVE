#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define EVE_RESEARCH_STAGE_B52_EMBEDDED
#include "research_stage_b52.cpp"
#undef EVE_RESEARCH_STAGE_B52_EMBEDDED
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <tuple>

namespace {

constexpr std::uint32_t kB53Episodes = 512;

using Component = eve::bios::ExperimentalTransientComponent;

struct B53Aggregate {
    std::uint64_t cases{};
    std::uint64_t changed{};
    std::uint64_t restored{};
    std::uint64_t induced{};
    std::uint64_t other_preserved{};
    std::uint64_t both_correct{};
    std::uint64_t valid{};
};

struct B53Results {
    std::map<std::string, B53Aggregate> components;
    std::map<std::string, B53Aggregate> combinations;
    std::map<std::string, B53Aggregate> sample_groups;
    std::map<std::string, std::map<std::string, B53Aggregate>> clusters;
    std::uint64_t losses{};
    std::uint64_t detailed{};
    std::uint64_t no_weight{};
    std::uint64_t full_restored{};
    std::uint64_t invalid_controls{};
};

const std::array<std::pair<Component, const char*>, 10> kB53Components{{
    {Component::membrane, "T1_membrane"},
    {Component::input_activity, "T2_input_activity"},
    {Component::local_event_memory, "T3_G3"},
    {Component::eligibility, "T4_eligibility"},
    {Component::synaptic_activity, "T5_synaptic_activity"},
    {Component::recovery, "T6_recovery"},
    {Component::time_and_queue, "T7_T8_time_queue"},
    {Component::context, "T9_context"},
    {Component::random_state, "T9_random_state"},
    {Component::diagnostic_counters, "T9_diagnostic_counters"},
}};

struct B53Combination {
    const char* name;
    std::vector<Component> components;
};

const std::array<B53Combination, 5> kB53Combinations{{
    {"C1_membrane_G3", {Component::membrane,
                           Component::local_event_memory}},
    {"C2_membrane_input", {Component::membrane,
                              Component::input_activity}},
    {"C3_membrane_activity", {Component::membrane,
                                 Component::synaptic_activity}},
    {"C4_G3_eligibility", {Component::local_event_memory,
                              Component::eligibility}},
    {"C5_membrane_G3_eligibility", {Component::membrane,
        Component::local_event_memory, Component::eligibility}},
}};

std::string b53_outcome(const B51Probe& probe) {
    return b47_outcome(probe.motor_a, probe.motor_b);
}

bool b53_same_weights(
    const eve::bios::Runtime& left, const eve::bios::Runtime& right) {
    if (left.network().synapse_count() != right.network().synapse_count()) {
        return false;
    }
    for (eve::SynapseId id = 0; id < left.network().synapse_count(); ++id) {
        if (left.network().synapse(id).weight !=
            right.network().synapse(id).weight) return false;
    }
    return true;
}

std::string b53_cause(
    const B51Episode& row, const B47Circuit& circuit) {
    bool sensor = false;
    bool inhibition = false;
    bool other = false;
    for (eve::SynapseId id = 0; id < row.after_weights.size(); ++id) {
        if (row.before_weights[id].weight == row.after_weights[id].weight) continue;
        if (b52_contains(circuit.cue_synapses, id)) sensor = true;
        else if (b52_contains(circuit.inhibitory_outputs, id)) inhibition = true;
        else other = true;
    }
    if (!sensor && !inhibition && !other) return "no_weight_change";
    if (sensor && inhibition) return "sensor_and_inhibition";
    if (sensor) return "sensor_change";
    if (inhibition) return "inhibitory_change";
    return "other_change";
}

std::string b53_cluster_key(
    const std::uint32_t family, const bool inverse, const std::uint64_t seed) {
    return std::to_string(family) + '-' + (inverse ? "I" : "N") + '-' +
        std::to_string(seed);
}

std::string b53_neuron_role(
    const B47Circuit& circuit, const eve::NeuronId neuron) {
    if (b52_contains(circuit.sensors, neuron)) return "sensor";
    if (b52_contains(circuit.explorers, neuron)) return "explorer";
    if (b52_contains(circuit.decisions, neuron)) return "decision";
    if (b52_contains(circuit.motors, neuron)) return "motor";
    if (b52_contains(circuit.modulation_inputs, neuron)) {
        return "modulation_input";
    }
    return "other";
}

void b53_inventory(const std::filesystem::path& output) {
    std::ofstream file(output / "state-inventory.csv");
    file << "id,component,written_by,read_by,direct_fire_effect,plasticity_effect,diagnostic_only,persisted,copied,isolatable,technical_coupling\n";
    file << "T1,membrane_potential,impulse integration and recovery,fire threshold comparison,1,0,0,1,1,1,none\n";
    file << "T2,input_event_count,each received impulse,activity phase completion,0,1,0,1,1,1,synaptic activity reset\n";
    file << "T3,local G3 event memory,received synaptic impulse,eligibility candidate creation,0,1,0,1,1,1,logical ring state\n";
    file << "T4,eligibility records and generations,neuron fire and local trace creation,modulatory feedback and expiry,0,1,0,1,1,1,expiry and decay queue events\n";
    file << "T5,synaptic activity_count,received impulse,learning threshold and active_phase selection,0,1,0,1,1,1,input phase completion\n";
    file << "T6,refractory and recovery state,fire and recovery events,fire suppression and recovery,1,0,0,1,1,1,recovery queue events\n";
    file << "T7,simulated time,event scheduling and explicit advance,ready-event selection and trace expiry,1,1,0,1,1,0,complete queue and all timed states\n";
    file << "T8,event queue and sequence,all scheduled neural events,event dispatcher,1,1,0,1,1,0,simulated time and generation-bearing states\n";
    file << "T9a,context traces,action-context events,eligibility binding,0,1,0,1,1,1,context expiry queue events\n";
    file << "T9b,random generator state,exploration growth and competition,random choice sites,0,1,0,1,1,1,no immediate E1 use\n";
    file << "T9c,metrics counters,event accounting,diagnostics fingerprint and persistence,0,0,1,1,1,1,none\n";
    file << "static,synapse weights and effects,learning or experiment,impulse integration,1,1,0,1,1,0,not a transient factor\n";
}

void b53_reproduce_b52(const std::filesystem::path& output) {
    const auto reference = output / "b52-reproduction";
    std::filesystem::create_directories(reference);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b52_reproduce_b51(reference);
    b52_task_controls(reference);
    b52_control_groups(reference);
    const auto rows = b52_primary(reference);
    b52_statistics(reference, rows);
    b52_persistence(reference);
    std::ofstream mechanism(reference / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K9,0,B52 separates synaptic and transient causes diagnostically; no single local production intervention is justified before the combined evidence is reviewed\n";
    mechanism << "K10,0,no K9 mechanism exists to deactivate\n";
    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(reference / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,networks,episodes,reference_reproduced\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ',' << rows.size() << ','
        << rows.size() * kB53Episodes << ",1\n";
}

void b53_controls(const std::filesystem::path& output) {
    for (const std::string name : {
             "checkpoint-evaluation.csv", "confusion-matrices.csv",
             "success-criteria.csv", "task-results.csv",
             "control-groups.csv"}) {
        std::filesystem::copy_file(output / "b52-reproduction" / name,
            output / ("reference-" + name),
            std::filesystem::copy_options::overwrite_existing);
    }
    std::ofstream file(output / "control-group-manifest.csv");
    file << "group,kind,implementation,data_file,enabled\n";
    file << "K0,reference,unchanged B52,b52-reproduction,1\n";
    file << "K1,reference,no learning,b52-reproduction/control-groups.csv,1\n";
    file << "K2,reference,fixed inhibition,b52-reproduction/control-groups.csv,1\n";
    file << "K3,reference,learned inhibition,b52-reproduction/control-groups.csv,1\n";
    file << "K4,reference,no additional plasticity,b52-reproduction/control-groups.csv,1\n";
    file << "K5,diagnostic,isolated membrane transfer,component-interventions.csv.gz,1\n";
    file << "K6,diagnostic,isolated G3 transfer,component-interventions.csv.gz,1\n";
    file << "K7,diagnostic,eligibility plus expiry bookkeeping,component-interventions.csv.gz,1\n";
    file << "K8,diagnostic,input and synaptic activity transfers,component-interventions.csv.gz,1\n";
    file << "K9,diagnostic,code-derived transient combinations,combination-interventions.csv.gz,1\n";
    file << "K10,diagnostic,probe sequence and time gaps,probe-sequences.csv.gz,1\n";
    file << "K11,research variant,not causally justified,mechanism-decision.csv,0\n";
    file << "K12,deactivation control,no K11 implementation,mechanism-decision.csv,0\n";
    std::ofstream tasks(output / "task-matrix.csv");
    tasks << "task,normal,inverse,reference_file,diagnostic_scope,production_change\n";
    for (const auto task : {"only_X", "only_Y", "X_then_Y", "Y_then_X",
             "alternating", "mixed", "reversal", "retention"}) {
        tasks << task << ",1,1,reference-task-results.csv,"
            << (std::string(task) == "mixed"
                    ? "K5-K10 real loss-state copies"
                    : "unchanged B52 regression")
            << ",0\n";
    }
}

std::array<bool, 2> b53_recall(
    const eve::bios::Runtime& runtime, const B47Circuit& circuit,
    const B48Variant& variant, const std::uint32_t episode) {
    return b51_recall_pair(runtime, circuit, variant, episode);
}

void b53_emit_probe_sequences(
    std::ofstream& file, const std::string& prefix,
    const eve::bios::Runtime& snapshot, const B47Circuit& circuit,
    const B48Variant& variant, const std::uint32_t episode) {
    const std::array<std::pair<const char*, std::vector<std::uint8_t>>, 6>
        sequences{{
            {"XX", {0, 0}}, {"YY", {1, 1}}, {"XY", {0, 1}},
            {"YX", {1, 0}}, {"XYX", {0, 1, 0}},
            {"YXY", {1, 0, 1}},
        }};
    for (const auto gap : {0ULL, 1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL}) {
        for (const auto& sequence : sequences) {
            auto continued = snapshot;
            for (std::size_t step = 0; step < sequence.second.size(); ++step) {
                const auto cue = sequence.second[step];
                if (step != 0 && gap != 0) {
                    static_cast<void>(continued.advance_time_by(gap));
                }
                const auto continued_before = continued;
                const auto continued_probe = b51_probe_in_place(
                    continued, circuit, cue);
                const auto continued_difference = b52_state_difference(
                    continued_before, continued);
                const bool target_a = b48_target_a(
                    variant, cue, episode, b46_training(variant.task));
                file << prefix << ",continued," << sequence.first << ','
                    << gap << ',' << step << ',' << static_cast<unsigned>(cue)
                    << ',' << b53_outcome(continued_probe) << ','
                    << b51_correct(target_a, continued_probe.motor_a,
                                   continued_probe.motor_b) << ','
                    << continued_difference.membrane << ','
                    << continued_difference.input_count << ','
                    << continued_difference.local_memory << ','
                    << continued_difference.eligibility << ','
                    << continued_difference.synapse_activity << ','
                    << continued_difference.time_delta << ','
                    << continued_difference.queue_delta << ','
                    << b53_same_weights(continued_before, continued) << ','
                    << continued.network().validate() << '\n';

                auto fresh = snapshot;
                if (gap != 0) static_cast<void>(fresh.advance_time_by(gap));
                const auto fresh_before = fresh;
                const auto fresh_probe = b51_probe_in_place(fresh, circuit, cue);
                const auto fresh_difference = b52_state_difference(
                    fresh_before, fresh);
                file << prefix << ",fresh_copy," << sequence.first << ','
                    << gap << ',' << step << ',' << static_cast<unsigned>(cue)
                    << ',' << b53_outcome(fresh_probe) << ','
                    << b51_correct(target_a, fresh_probe.motor_a,
                                   fresh_probe.motor_b) << ','
                    << fresh_difference.membrane << ','
                    << fresh_difference.input_count << ','
                    << fresh_difference.local_memory << ','
                    << fresh_difference.eligibility << ','
                    << fresh_difference.synapse_activity << ','
                    << fresh_difference.time_delta << ','
                    << fresh_difference.queue_delta << ','
                    << b53_same_weights(fresh_before, fresh) << ','
                    << fresh.network().validate() << '\n';
            }
        }
    }
}

B53Results b53_primary(const std::filesystem::path& output) {
    std::ofstream selection(output / "loss-selection.csv");
    selection << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,same_weights,first_or_second,baseline_A_correct,baseline_B_correct,identical_A,identical_B,full_T_restored,valid\n";
    std::ofstream components(output / "component-interventions.csv");
    components << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,direction,component,changed_values,coupled_events,isolatable,lost_correct,other_correct,lost_outcome,other_outcome,necessary,sufficient,valid\n";
    std::ofstream combinations(output / "combination-interventions.csv");
    combinations << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,direction,combination,changed_values,coupled_events,lost_correct,other_correct,necessary,sufficient,valid\n";
    std::ofstream weights(output / "weight-interactions.csv");
    weights << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,sensor_state,inhibition_state,membrane_state,lost_correct,other_correct,valid\n";
    std::ofstream probes(output / "probe-sequences.csv");
    probes << "phase,family,assignment,seed,source_episode,mode,sequence,gap,step,cue,outcome,correct,membrane_differences,input_differences,G3_differences,eligibility_differences,activity_differences,time_delta,queue_delta,same_weights,valid\n";
    std::ofstream neurons(output / "membrane-neuron-interventions.csv");
    neurons << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,direction,neuron,role,old_membrane,new_membrane,lost_correct,other_correct,necessary,sufficient,valid\n";
    std::ofstream minimal(output / "minimal-membrane-cases.csv");
    minimal << "phase,family,assignment,seed,episode,lost_cue,loss_index,cause,changed_neurons,full_restore,full_induce_loss,any_single_restore,any_single_induce,pair_required_restore,pair_required_induce,both_correct_after_restore,valid\n";
    B53Results results;

    for (const std::string phase : {"discovery", "confirmation"}) {
        const std::uint64_t first_seed = phase == "discovery" ? 1 : 10001;
        for (std::uint32_t family = 0; family < 3; ++family) {
            for (const bool inverse : {false, true}) {
                B48Variant variant{"B53_transient_recall", B46Task::mixed,
                    B48Policy::reference, inverse, true, false, family};
                for (std::uint64_t seed = first_seed;
                     seed < first_seed + 64; ++seed) {
                    auto state = b51_initial_state(variant, seed);
                    auto previous_correct = b53_recall(
                        state.runtime, state.circuit, variant, 0);
                    std::array<std::uint32_t, 2> loss_count{};
                    bool probe_sequences_emitted = false;
                    const auto cluster = b53_cluster_key(family, inverse, seed);
                    for (std::uint32_t episode = 0;
                         episode < kB53Episodes; ++episode) {
                        const auto cue = b48_cue(variant, state.plans[episode],
                            episode, b46_training(variant.task));
                        const bool target_a = b48_target_a(variant, cue,
                            episode, b46_training(variant.task));
                        auto row = b51_episode(state.runtime, state.circuit, cue,
                            state.plans[episode].exploration, target_a, episode);
                        state.runtime = row.after_feedback;
                        const auto after_correct = b53_recall(
                            state.runtime, state.circuit, variant, episode);
                        for (std::uint8_t path = 0; path < 2; ++path) {
                            if (!previous_correct[path] || after_correct[path]) continue;
                            ++results.losses;
                            const auto index = ++loss_count[path];
                            const auto cause = b53_cause(row, state.circuit);
                            const bool same_weights = cause == "no_weight_change";
                            const bool detailed = index <= 2 || same_weights;
                            if (!detailed) continue;
                            ++results.detailed;
                            results.no_weight += same_weights;
                            const auto prefix = phase + ',' +
                                std::to_string(family) + ',' +
                                (inverse ? "inverse" : "normal") + ',' +
                                std::to_string(seed) + ',' +
                                std::to_string(episode) + ',' +
                                std::to_string(path) + ',' +
                                std::to_string(index) + ',' + cause;
                            const bool lost_target_a = b48_target_a(variant, path,
                                episode, b46_training(variant.task));
                            const auto before_probe = b51_probe(
                                row.before, state.circuit, path);
                            const auto after_probe = b51_probe(
                                state.runtime, state.circuit, path);
                            auto a1 = row.before;
                            auto a2 = row.before;
                            auto b1 = state.runtime;
                            auto b2 = state.runtime;
                            const auto a1_probe = b51_probe_in_place(
                                a1, state.circuit, path);
                            const auto a2_probe = b51_probe_in_place(
                                a2, state.circuit, path);
                            const auto b1_probe = b51_probe_in_place(
                                b1, state.circuit, path);
                            const auto b2_probe = b51_probe_in_place(
                                b2, state.circuit, path);
                            auto full_t = b52_factor_state(row.before,
                                state.runtime, state.circuit, true, true, false);
                            const auto full_pair = b53_recall(
                                full_t, state.circuit, variant, episode);
                            results.full_restored += full_pair[path];
                            const bool controls_valid =
                                b51_correct(lost_target_a, before_probe.motor_a,
                                            before_probe.motor_b) &&
                                !b51_correct(lost_target_a, after_probe.motor_a,
                                             after_probe.motor_b) &&
                                a1_probe.motor_a == a2_probe.motor_a &&
                                a1_probe.motor_b == a2_probe.motor_b &&
                                b1_probe.motor_a == b2_probe.motor_a &&
                                b1_probe.motor_b == b2_probe.motor_b;
                            results.invalid_controls += !controls_valid;
                            selection << prefix << ',' << same_weights << ','
                                << (index <= 2 ? std::to_string(index) : "later")
                                << ','
                                << b51_correct(lost_target_a,
                                    before_probe.motor_a, before_probe.motor_b)
                                << ',' << b51_correct(lost_target_a,
                                    after_probe.motor_a, after_probe.motor_b)
                                << ',' << (a1.state_fingerprint() ==
                                           a2.state_fingerprint()) << ','
                                << (b1.state_fingerprint() ==
                                           b2.state_fingerprint()) << ','
                                << full_pair[path] << ',' << controls_valid
                                << '\n';
                            auto& sample = results.sample_groups[
                                phase + '|' + cause];
                            ++sample.cases;
                            sample.restored += full_pair[path];
                            sample.valid += controls_valid;

                            std::vector<eve::NeuronId> changed_membranes;
                            for (eve::NeuronId id = 0;
                                 id < row.before.network().neuron_count(); ++id) {
                                if (row.before.network().neuron(id)
                                        .membrane_potential !=
                                    state.runtime.network().neuron(id)
                                        .membrane_potential) {
                                    changed_membranes.push_back(id);
                                }
                            }
                            auto all_membrane_restore = state.runtime;
                            const auto all_restore_transfer = all_membrane_restore
                                .set_experimental_transient_component_from(
                                    row.before, Component::membrane);
                            const auto all_restore_pair = b53_recall(
                                all_membrane_restore, state.circuit, variant,
                                episode);
                            auto all_membrane_induce = row.before;
                            const auto all_induce_transfer = all_membrane_induce
                                .set_experimental_transient_component_from(
                                    state.runtime, Component::membrane);
                            const auto all_induce_pair = b53_recall(
                                all_membrane_induce, state.circuit, variant,
                                episode);
                            bool any_single_restore = false;
                            bool any_single_induce = false;
                            for (const auto neuron : changed_membranes) {
                                const auto old_value = row.before.network()
                                    .neuron(neuron).membrane_potential;
                                const auto new_value = state.runtime.network()
                                    .neuron(neuron).membrane_potential;
                                auto restore_one = state.runtime;
                                restore_one.set_experimental_neuron_membrane(
                                    neuron, old_value);
                                const auto restore_one_pair = b53_recall(
                                    restore_one, state.circuit, variant, episode);
                                any_single_restore = any_single_restore ||
                                    restore_one_pair[path];
                                neurons << prefix << ",restore," << neuron << ','
                                    << b53_neuron_role(state.circuit, neuron)
                                    << ',' << old_value << ',' << new_value << ','
                                    << restore_one_pair[path] << ','
                                    << restore_one_pair[1U - path] << ','
                                    << restore_one_pair[path] << ",0,"
                                    << restore_one.network().validate() << '\n';
                                auto induce_one = row.before;
                                induce_one.set_experimental_neuron_membrane(
                                    neuron, new_value);
                                const auto induce_one_pair = b53_recall(
                                    induce_one, state.circuit, variant, episode);
                                const bool induced_loss = !induce_one_pair[path];
                                any_single_induce = any_single_induce ||
                                    induced_loss;
                                neurons << prefix << ",induce," << neuron << ','
                                    << b53_neuron_role(state.circuit, neuron)
                                    << ',' << old_value << ',' << new_value << ','
                                    << induce_one_pair[path] << ','
                                    << induce_one_pair[1U - path] << ",0,"
                                    << induced_loss << ','
                                    << induce_one.network().validate() << '\n';
                            }
                            minimal << prefix << ',' << changed_membranes.size()
                                << ',' << all_restore_pair[path] << ','
                                << !all_induce_pair[path] << ','
                                << any_single_restore << ','
                                << any_single_induce << ','
                                << (all_restore_pair[path] &&
                                    !any_single_restore &&
                                    changed_membranes.size() > 1) << ','
                                << (!all_induce_pair[path] &&
                                    !any_single_induce &&
                                    changed_membranes.size() > 1) << ','
                                << (all_restore_pair[path] &&
                                    all_restore_pair[1U - path]) << ','
                                << (controls_valid && all_restore_transfer.valid &&
                                    all_induce_transfer.valid) << '\n';

                            for (const auto& component : kB53Components) {
                                const auto key = phase + '|' + component.second;
                                auto& aggregate = results.components[key];
                                auto& clustered = results.clusters[key][cluster];
                                ++aggregate.cases;
                                ++clustered.cases;
                                if (component.first == Component::time_and_queue) {
                                    components << prefix
                                        << ",not_isolatable," << component.second
                                        << ",-1,-1,0,0,0,NA,NA,0,0,0\n";
                                    continue;
                                }
                                auto restored = state.runtime;
                                const auto restore_transfer =
                                    restored.set_experimental_transient_component_from(
                                        row.before, component.first);
                                const auto restore_pair = b53_recall(restored,
                                    state.circuit, variant, episode);
                                const auto restore_lost = b51_probe(
                                    restored, state.circuit, path);
                                const auto restore_other = b51_probe(
                                    restored, state.circuit, 1U - path);
                                const bool valid_restore =
                                    restore_transfer.valid && controls_valid;
                                aggregate.changed +=
                                    restore_transfer.changed_values != 0;
                                aggregate.restored += restore_pair[path];
                                aggregate.other_preserved +=
                                    restore_pair[1U - path] ==
                                        after_correct[1U - path];
                                aggregate.both_correct += restore_pair[path] &&
                                    restore_pair[1U - path];
                                aggregate.valid += valid_restore;
                                clustered.changed +=
                                    restore_transfer.changed_values != 0;
                                clustered.restored += restore_pair[path];
                                clustered.valid += valid_restore;
                                components << prefix << ",restore,"
                                    << component.second << ','
                                    << restore_transfer.changed_values << ','
                                    << restore_transfer.coupled_events
                                    << ",1," << restore_pair[path] << ','
                                    << restore_pair[1U - path] << ','
                                    << b53_outcome(restore_lost) << ','
                                    << b53_outcome(restore_other) << ','
                                    << restore_pair[path] << ",0,"
                                    << valid_restore << '\n';

                                auto induced = row.before;
                                const auto induce_transfer =
                                    induced.set_experimental_transient_component_from(
                                        state.runtime, component.first);
                                const auto induce_pair = b53_recall(induced,
                                    state.circuit, variant, episode);
                                const auto induce_lost = b51_probe(
                                    induced, state.circuit, path);
                                const auto induce_other = b51_probe(
                                    induced, state.circuit, 1U - path);
                                const bool induced_loss = !induce_pair[path];
                                aggregate.induced += induced_loss;
                                clustered.induced += induced_loss;
                                components << prefix << ",induce,"
                                    << component.second << ','
                                    << induce_transfer.changed_values << ','
                                    << induce_transfer.coupled_events
                                    << ",1," << induce_pair[path] << ','
                                    << induce_pair[1U - path] << ','
                                    << b53_outcome(induce_lost) << ','
                                    << b53_outcome(induce_other) << ",0,"
                                    << induced_loss << ','
                                    << (induce_transfer.valid && controls_valid)
                                    << '\n';
                            }

                            for (const auto& combination : kB53Combinations) {
                                const auto key = phase + '|' + combination.name;
                                auto& aggregate = results.combinations[key];
                                ++aggregate.cases;
                                for (const bool restore_direction : {true, false}) {
                                    auto runtime = restore_direction
                                        ? state.runtime : row.before;
                                    const auto& source = restore_direction
                                        ? row.before : state.runtime;
                                    std::size_t changed = 0;
                                    std::size_t coupled = 0;
                                    bool valid = controls_valid;
                                    for (const auto component :
                                         combination.components) {
                                        const auto transfer = runtime
                                            .set_experimental_transient_component_from(
                                                source, component);
                                        changed += transfer.changed_values;
                                        coupled += transfer.coupled_events;
                                        valid = valid && transfer.valid;
                                    }
                                    const auto pair = b53_recall(runtime,
                                        state.circuit, variant, episode);
                                    const bool restored_answer = pair[path];
                                    const bool induced_loss = !pair[path];
                                    if (restore_direction) {
                                        aggregate.changed += changed != 0;
                                        aggregate.restored += restored_answer;
                                        aggregate.other_preserved +=
                                            pair[1U - path] ==
                                                after_correct[1U - path];
                                        aggregate.both_correct += pair[path] &&
                                            pair[1U - path];
                                        aggregate.valid += valid;
                                    } else {
                                        aggregate.induced += induced_loss;
                                    }
                                    combinations << prefix << ','
                                        << (restore_direction ? "restore" : "induce")
                                        << ',' << combination.name << ','
                                        << changed << ',' << coupled << ','
                                        << pair[path] << ',' << pair[1U - path]
                                        << ',' << (restore_direction &&
                                                    restored_answer) << ','
                                        << (!restore_direction && induced_loss)
                                        << ',' << valid << '\n';
                                }
                            }

                            for (const bool old_sensor : {false, true}) {
                                for (const bool old_inhibition : {false, true}) {
                                    auto runtime = state.runtime;
                                    for (const auto id : state.circuit.cue_synapses) {
                                        runtime.set_experimental_synapse_weight(id,
                                            (old_sensor ? row.before : state.runtime)
                                                .network().synapse(id).weight);
                                    }
                                    for (const auto id :
                                         state.circuit.inhibitory_outputs) {
                                        runtime.set_experimental_synapse_weight(id,
                                            (old_inhibition ? row.before : state.runtime)
                                                .network().synapse(id).weight);
                                    }
                                    const auto transfer = runtime
                                        .set_experimental_transient_component_from(
                                            row.before, Component::membrane);
                                    const auto pair = b53_recall(runtime,
                                        state.circuit, variant, episode);
                                    weights << prefix << ','
                                        << (old_sensor ? "old" : "new") << ','
                                        << (old_inhibition ? "old" : "new")
                                        << ",old," << pair[path] << ','
                                        << pair[1U - path] << ','
                                        << transfer.valid << '\n';
                                }
                            }
                            if (!probe_sequences_emitted) {
                                const auto probe_prefix = phase + ',' +
                                    std::to_string(family) + ',' +
                                    (inverse ? "inverse" : "normal") + ',' +
                                    std::to_string(seed) + ',' +
                                    std::to_string(episode);
                                b53_emit_probe_sequences(probes, probe_prefix,
                                    state.runtime, state.circuit, variant,
                                    episode);
                                probe_sequences_emitted = true;
                            }
                        }
                        previous_correct = after_correct;
                    }
                }
            }
        }
    }
    return results;
}

void b53_secondary_summaries(const std::filesystem::path& output) {
    struct Counts {
        std::uint64_t cases{};
        std::uint64_t correct{};
        std::uint64_t other{};
        std::uint64_t necessary{};
        std::uint64_t sufficient{};
        std::uint64_t induced{};
        std::uint64_t valid{};
        std::uint64_t membrane{};
        std::uint64_t input{};
        std::uint64_t g3{};
        std::uint64_t eligibility{};
        std::uint64_t activity{};
    };
    std::map<std::string, Counts> membrane_rows;
    {
        std::ifstream file(output / "component-interventions.csv");
        std::string line;
        static_cast<void>(std::getline(file, line));
        while (std::getline(file, line)) {
            const auto fields = b50_fields(line);
            if (fields.size() != 20 || fields[9] != "T1_membrane") continue;
            const auto loss_group = fields[6] == "1" ? "first" :
                fields[6] == "2" ? "second" : "later";
            auto& row = membrane_rows[fields[0] + '|' + fields[7] + '|' +
                loss_group + '|' + fields[8]];
            ++row.cases;
            row.correct += std::stoull(fields[13]);
            row.other += std::stoull(fields[14]);
            row.necessary += std::stoull(fields[17]);
            row.sufficient += std::stoull(fields[18]);
            row.valid += std::stoull(fields[19]);
        }
    }
    std::ofstream membrane(output / "membrane-cause-summary.csv");
    membrane << "phase,cause,loss_group,direction,cases,lost_correct,other_correct,necessary,sufficient,valid\n";
    for (const auto& [key, row] : membrane_rows) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        membrane << printable << ',' << row.cases << ',' << row.correct << ','
            << row.other << ',' << row.necessary << ',' << row.sufficient
            << ',' << row.valid << '\n';
    }

    std::map<std::string, Counts> neuron_rows;
    {
        std::ifstream file(output / "membrane-neuron-interventions.csv");
        std::string line;
        static_cast<void>(std::getline(file, line));
        while (std::getline(file, line)) {
            const auto fields = b50_fields(line);
            if (fields.size() != 18) continue;
            auto& row = neuron_rows[fields[0] + '|' + fields[10] + '|' +
                fields[8]];
            ++row.cases;
            row.correct += std::stoull(fields[13]);
            row.other += std::stoull(fields[14]);
            row.necessary += std::stoull(fields[15]);
            row.sufficient += std::stoull(fields[16]);
            row.valid += std::stoull(fields[17]);
        }
    }
    std::ofstream neuron_summary(output / "membrane-neuron-summary.csv");
    neuron_summary << "phase,role,direction,cases,lost_correct,other_correct,necessary,sufficient,valid\n";
    for (const auto& [key, row] : neuron_rows) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        neuron_summary << printable << ',' << row.cases << ',' << row.correct
            << ',' << row.other << ',' << row.necessary << ','
            << row.sufficient << ',' << row.valid << '\n';
    }

    std::map<std::string, Counts> minimal_rows;
    {
        std::ifstream file(output / "minimal-membrane-cases.csv");
        std::string line;
        static_cast<void>(std::getline(file, line));
        while (std::getline(file, line)) {
            const auto fields = b50_fields(line);
            if (fields.size() != 17) continue;
            auto& row = minimal_rows[fields[0] + '|' + fields[7] + '|' +
                fields[8]];
            ++row.cases;
            row.correct += std::stoull(fields[9]);
            row.sufficient += std::stoull(fields[10]);
            row.necessary += std::stoull(fields[13]);
            row.induced += std::stoull(fields[14]);
            row.other += std::stoull(fields[15]);
            row.valid += std::stoull(fields[16]);
        }
    }
    std::ofstream minimal_summary(output / "minimal-membrane-summary.csv");
    minimal_summary << "phase,cause,changed_neurons,cases,full_restore,full_induce_loss,pair_required_restore,pair_required_induce,both_correct_after_restore,valid\n";
    for (const auto& [key, row] : minimal_rows) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        minimal_summary << printable << ',' << row.cases << ',' << row.correct
            << ',' << row.sufficient << ',' << row.necessary << ','
            << row.induced << ',' << row.other << ',' << row.valid << '\n';
    }

    std::map<std::string, Counts> weight_rows;
    {
        std::ifstream file(output / "weight-interactions.csv");
        std::string line;
        static_cast<void>(std::getline(file, line));
        while (std::getline(file, line)) {
            const auto fields = b50_fields(line);
            if (fields.size() != 14) continue;
            auto& row = weight_rows[fields[8] + '|' + fields[9]];
            ++row.cases;
            row.correct += std::stoull(fields[11]);
            row.other += std::stoull(fields[12]);
            row.valid += std::stoull(fields[13]);
        }
    }
    std::ofstream weights(output / "weight-interaction-summary.csv");
    weights << "sensor_state,inhibition_state,cases,lost_correct,other_correct,valid\n";
    for (const auto& [key, row] : weight_rows) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        weights << printable << ',' << row.cases << ',' << row.correct << ','
            << row.other << ',' << row.valid << '\n';
    }

    std::map<std::string, Counts> probe_rows;
    {
        std::ifstream file(output / "probe-sequences.csv");
        std::string line;
        static_cast<void>(std::getline(file, line));
        while (std::getline(file, line)) {
            const auto fields = b50_fields(line);
            if (fields.size() != 21) continue;
            auto& row = probe_rows[fields[5] + '|' + fields[6] + '|' +
                fields[7] + '|' + fields[8]];
            ++row.cases;
            row.correct += std::stoull(fields[11]);
            row.membrane += std::stoull(fields[12]);
            row.input += std::stoull(fields[13]);
            row.g3 += std::stoull(fields[14]);
            row.eligibility += std::stoull(fields[15]);
            row.activity += std::stoull(fields[16]);
            row.valid += std::stoull(fields[20]);
        }
    }
    std::ofstream probes(output / "probe-summary.csv");
    probes << "mode,sequence,gap,step,cases,correct,membrane_differences,input_differences,G3_differences,eligibility_differences,activity_differences,valid\n";
    for (const auto& [key, row] : probe_rows) {
        std::string printable = key;
        std::replace(printable.begin(), printable.end(), '|', ',');
        probes << printable << ',' << row.cases << ',' << row.correct << ','
            << row.membrane << ',' << row.input << ',' << row.g3 << ','
            << row.eligibility << ',' << row.activity << ',' << row.valid
            << '\n';
    }
}

void b53_statistics(
    const std::filesystem::path& output, const B53Results& results) {
    std::ofstream summary(output / "component-summary.csv");
    summary << "phase,component,cases,changed,restored,induced_loss,other_relation_preserved,both_correct,valid\n";
    for (const auto& [key, value] : results.components) {
        const auto split = key.find('|');
        summary << key.substr(0, split) << ',' << key.substr(split + 1) << ','
            << value.cases << ',' << value.changed << ',' << value.restored
            << ',' << value.induced << ',' << value.other_preserved << ','
            << value.both_correct << ',' << value.valid << '\n';
    }
    std::ofstream combination(output / "combination-summary.csv");
    combination << "phase,combination,cases,changed,restored,induced_loss,other_relation_preserved,both_correct,valid\n";
    for (const auto& [key, value] : results.combinations) {
        const auto split = key.find('|');
        combination << key.substr(0, split) << ',' << key.substr(split + 1)
            << ',' << value.cases << ',' << value.changed << ','
            << value.restored << ',' << value.induced << ','
            << value.other_preserved << ',' << value.both_correct << ','
            << value.valid << '\n';
    }
    std::ofstream samples(output / "sample-group-summary.csv");
    samples << "phase,cause,cases,full_transient_restored,valid_controls\n";
    for (const auto& [key, value] : results.sample_groups) {
        const auto split = key.find('|');
        samples << key.substr(0, split) << ',' << key.substr(split + 1) << ','
            << value.cases << ',' << value.restored << ',' << value.valid
            << '\n';
    }
    std::ofstream clusters(output / "cluster-statistics.csv");
    clusters << "phase,component,metric,clusters,mean,ci_low,ci_high\n";
    std::uint64_t salt = 0;
    for (const auto& [key, networks] : results.clusters) {
        std::vector<double> restoration;
        std::vector<double> induction;
        for (const auto& [unused, value] : networks) {
            static_cast<void>(unused);
            restoration.push_back(value.cases == 0 ? 0.0 :
                static_cast<double>(value.restored) / value.cases);
            induction.push_back(value.cases == 0 ? 0.0 :
                static_cast<double>(value.induced) / value.cases);
        }
        const auto split = key.find('|');
        for (const auto& metric : {
                 std::pair<const char*, const std::vector<double>*>(
                     "restoration_fraction", &restoration),
                 {"induction_fraction", &induction}}) {
            const auto interval = bootstrap_interval(
                *metric.second, 0x5300000ULL + ++salt);
            clusters << key.substr(0, split) << ',' << key.substr(split + 1)
                << ',' << metric.first << ',' << metric.second->size() << ','
                << b48_mean(*metric.second) << ',' << interval.first << ','
                << interval.second << '\n';
        }
    }
    std::ofstream diagnostic(output / "diagnostic-summary.csv");
    diagnostic << "metric,value\n";
    diagnostic << "networks,768\n";
    diagnostic << "episodes," << 768ULL * kB53Episodes << '\n';
    diagnostic << "losses," << results.losses << '\n';
    diagnostic << "detailed_loss_copies," << results.detailed << '\n';
    diagnostic << "losses_without_weight_change," << results.no_weight << '\n';
    diagnostic << "full_transient_restored," << results.full_restored << '\n';
    diagnostic << "invalid_baseline_controls," << results.invalid_controls
        << '\n';
}

void b53_persistence(const std::filesystem::path& output) {
    std::ofstream file(output / "persistence.csv");
    file << "seed,component,changed_values,exact_restore,exact_recall,valid\n";
    B48Variant variant{"B53_persistence", B46Task::mixed,
        B48Policy::reference, false, true, false, 0};
    for (std::uint64_t seed = 1; seed <= 64; ++seed) {
        auto state = b51_initial_state(variant, seed);
        auto source = state.runtime;
        static_cast<void>(b51_probe_in_place(source, state.circuit, 0));
        auto diagnostic = source;
        const auto transfer = diagnostic
            .set_experimental_transient_component_from(
                state.runtime, Component::membrane);
        const auto before = b53_recall(
            diagnostic, state.circuit, variant, 0);
        const auto path = output / (".b53-state-" + std::to_string(seed));
        diagnostic.save(path.string());
        const auto restored = eve::bios::Runtime::load(path.string());
        const auto after = b53_recall(restored, state.circuit, variant, 0);
        file << seed << ",T1_membrane," << transfer.changed_values << ','
            << (diagnostic.state_fingerprint() == restored.state_fingerprint())
            << ',' << (before == after) << ','
            << restored.network().validate() << '\n';
        static_cast<void>(std::filesystem::remove(path));
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("research/results/b53");
    std::filesystem::create_directories(output);
    const auto started = std::chrono::steady_clock::now();
    const auto cpu_started = std::clock();
    b53_inventory(output);
    b53_reproduce_b52(output);
    b53_controls(output);
    const auto results = b53_primary(output);
    b53_secondary_summaries(output);
    b53_statistics(output, results);
    b53_persistence(output);
    std::ofstream mechanism(output / "mechanism-decision.csv");
    mechanism << "group,implemented,reason\n";
    mechanism << "K11,0,component causes are diagnostic and do not yet justify a production reset or normalization rule\n";
    mechanism << "K12,0,no K11 mechanism exists to deactivate\n";
    const auto ended = std::chrono::steady_clock::now();
    const auto cpu_ended = std::clock();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::ofstream performance(output / "performance.csv");
    performance << "wall_seconds,cpu_seconds,peak_rss_kib,networks,episodes,reference_reproduced\n";
    performance << std::chrono::duration<double>(ended - started).count() << ','
        << static_cast<double>(cpu_ended - cpu_started) / CLOCKS_PER_SEC << ','
        << usage.ru_maxrss << ",768," << 768ULL * kB53Episodes << ",1\n";
    std::cout << "B.5.3 transient recall causes complete K11=not_implemented\n";
    return 0;
}
