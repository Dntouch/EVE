#pragma once

#include "eve/brainstem/brainstem.hpp"
#include "eve/neural/network.hpp"
#include "eve/types.hpp"

#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace eve::bios {

class DeterministicRng {
public:
    explicit DeterministicRng(std::uint64_t seed = 1) noexcept;

    [[nodiscard]] std::uint64_t next() noexcept;
    [[nodiscard]] std::uint32_t bounded(std::uint32_t exclusive_upper_bound);
    [[nodiscard]] std::uint64_t state() const noexcept;
    void set_state(std::uint64_t state);

private:
    std::uint64_t state_{};
};

enum class EventKind : std::uint8_t {
    fire = 0,
    impulse = 1,
    recovery = 2,
    boundary = 3,
    eligibility_expiry = 4,
    feedback_fire = 5,
    feedback_impulse = 6,
    context_expiry = 7,
    modulation_fire = 8,
    modulation_impulse = 9,
    eligibility_decay = 10,
    subthreshold_relaxation = 11,
};

struct Event {
    EventKind kind{EventKind::fire};
    std::uint32_t target{};
    std::uint64_t scheduled_time{};
    std::uint64_t sequence{};
    std::uint64_t generation{};
    std::uint32_t context_signature{};
    bool context_feedback_origin{};
    std::int8_t modulation{};

    bool operator==(const Event& other) const noexcept {
        return kind == other.kind && target == other.target &&
               scheduled_time == other.scheduled_time && sequence == other.sequence &&
               generation == other.generation &&
               context_signature == other.context_signature &&
               context_feedback_origin == other.context_feedback_origin &&
               modulation == other.modulation;
    }
};

struct Diagnostics {
    std::uint32_t neuron_count{};
    std::uint32_t synapse_count{};
    std::size_t estimated_memory_bytes{};
    std::size_t queued_events{};
    std::size_t ready_events{};
    std::size_t future_events{};
    std::uint64_t simulated_time{};
    std::map<std::int32_t, std::uint64_t> weight_distribution;
    Metrics metrics;
    std::uint64_t state_fingerprint{};
};

// Research-only state factors used by controlled causal copies.  These
// transfers never run automatically and are not part of neural processing.
enum class ExperimentalTransientComponent : std::uint8_t {
    membrane = 0,
    input_activity = 1,
    local_event_memory = 2,
    eligibility = 3,
    synaptic_activity = 4,
    recovery = 5,
    time_and_queue = 6,
    context = 7,
    random_state = 8,
    diagnostic_counters = 9,
};

struct ExperimentalTransientTransfer {
    std::size_t changed_values{};
    std::size_t coupled_events{};
    bool valid{};
};

struct FireObservation {
    std::uint64_t event_index{};
    std::uint64_t simulated_time{};
    NeuronId neuron{};
    std::uint32_t outgoing_count{};
};

using FireObserver = std::function<void(const FireObservation&)>;

struct ImpulseObservation {
    std::uint64_t event_index{};
    std::uint64_t simulated_time{};
    SynapseId synapse{};
    NeuronId sender{};
    NeuronId receiver{};
    std::int32_t applied_weight{};
    SynapticEffect synaptic_effect{SynapticEffect::excitatory};
    std::int64_t membrane_before{};
    std::int64_t membrane_after{};
    std::uint32_t refractory_before{};
    std::uint32_t refractory_after{};
    bool receiver_fired{};
    bool firing_suppressed{};
    bool feedback_context{};
    std::uint32_t action_context_signature{};
    bool action_context_feedback_origin{};
    std::vector<SynapseId> local_event_snapshot;
    std::uint64_t eligibility_generation{};
    std::vector<SynapseId> eligibility_candidates;

    bool operator==(const ImpulseObservation& other) const noexcept {
        return event_index == other.event_index &&
               simulated_time == other.simulated_time && synapse == other.synapse &&
               sender == other.sender && receiver == other.receiver &&
               applied_weight == other.applied_weight &&
               synaptic_effect == other.synaptic_effect &&
               membrane_before == other.membrane_before &&
               membrane_after == other.membrane_after &&
               refractory_before == other.refractory_before &&
               refractory_after == other.refractory_after &&
               receiver_fired == other.receiver_fired &&
               firing_suppressed == other.firing_suppressed &&
               feedback_context == other.feedback_context &&
               action_context_signature == other.action_context_signature &&
               action_context_feedback_origin ==
                   other.action_context_feedback_origin &&
               local_event_snapshot == other.local_event_snapshot &&
               eligibility_generation == other.eligibility_generation &&
               eligibility_candidates == other.eligibility_candidates;
    }
};

using ImpulseObserver = std::function<void(const ImpulseObservation&)>;

struct RecoveryObservation {
    std::uint64_t event_index{};
    std::uint64_t simulated_time{};
    NeuronId neuron{};
    std::uint64_t generation{};
    std::int64_t membrane_before{};
    std::int64_t membrane_after{};
    std::uint32_t refractory_before{};
    std::uint32_t refractory_after{};
    bool recovered{};
    bool stale{};
};

struct EvaluationFeedbackResult {
    brainstem::EvaluationEncoding encoding;
    std::uint64_t processed_events{};
    std::uint64_t feedback_fires{};
    std::uint64_t feedback_impulses{};
    std::uint64_t receipts_with_eligibility{};
    std::uint64_t receipts_without_eligibility{};
    std::uint64_t receipts_with_multiple_eligibilities{};
    std::uint64_t created_eligibilities{};
    std::uint64_t unique_context_matches{};
    std::uint64_t ambiguous_context_matches{};
    std::uint64_t missing_context_matches{};
    std::uint64_t expired_context_matches{};
};

struct ModulatoryFeedbackResult {
    std::int8_t polarity{};
    std::uint64_t processed_events{};
    std::uint64_t neural_fires{};
    std::uint64_t neural_impulses{};
    std::uint64_t receipts_with_trace{};
    std::uint64_t receipts_without_trace{};
    std::uint64_t positive_changes{};
    std::uint64_t negative_changes{};
    std::uint64_t zero_to_one_changes{};
    std::uint64_t suppressed_writes{};
};

struct ActionContextEmission {
    brainstem::ActionContextSignal signal{brainstem::ActionContextSignal::start};
    std::uint8_t observed_motor_value{};
    std::uint32_t signature{};
    std::vector<NeuronId> active_inputs;
    std::uint64_t processed_events{};
    std::uint64_t created_traces{};
    std::uint64_t bound_eligibilities{};
};

using RecoveryObserver = std::function<void(const RecoveryObservation&)>;

struct SubthresholdRelaxationObservation {
    std::uint64_t event_index{};
    std::uint64_t simulated_time{};
    NeuronId neuron{};
    std::uint64_t generation{};
    std::int64_t membrane_before{};
    std::int64_t membrane_after{};
    bool relaxed{};
    bool stale{};
};

using SubthresholdRelaxationObserver =
    std::function<void(const SubthresholdRelaxationObservation&)>;

// Behaviour-neutral research observation emitted for every eligible candidate
// considered by a locally arriving modulatory impulse.  Observers are not
// persisted and cannot alter the neural state.
enum class PlasticityWriteOutcome : std::uint8_t {
    changed = 0,
    protected_synapse = 1,
    positive_weight_limit = 2,
    input_weight_budget = 3,
    negative_zero_floor = 4,
    write_suppressed = 5,
};

struct PlasticityObservation {
    std::uint64_t event_index{};
    std::uint64_t simulated_time{};
    SynapseId modulation_synapse{};
    NeuronId modulation_sender{};
    NeuronId receiver{};
    std::int8_t modulation_polarity{};
    std::int8_t local_polarity{};
    std::uint32_t active_eligibility_entries{};
    std::uint64_t eligibility_generation{};
    std::uint64_t eligibility_created_at{};
    std::uint64_t eligibility_expires_at{};
    std::uint8_t eligibility_strength{};
    std::vector<SynapseId> eligibility_candidates;
    SynapseId candidate{};
    NeuronId candidate_sender{};
    SynapticEffect candidate_effect{SynapticEffect::excitatory};
    std::int32_t weight_before{};
    std::int32_t weight_after{};
    bool plasticity_protected{};
    bool changed{};
    PlasticityWriteOutcome write_outcome{PlasticityWriteOutcome::changed};
    std::int64_t receiver_membrane{};
    std::uint32_t candidate_activity_count{};
};

using PlasticityObserver = std::function<void(const PlasticityObservation&)>;

struct MotorCausality {
    bool technical_abort{};
    bool completed{};
    bool completed_without_prior_data{};
    bool completed_after_data{};
    bool proven_circuit_path{};
    std::uint64_t first_data_event{};
    std::uint64_t completion_event{};
    std::uint64_t events_between_data_and_completion{};
    std::uint32_t observed_circuit_impulses{};
    std::uint32_t firing_circuit_impulses{};
    std::uint32_t suppressed_circuit_impulses{};
};

struct LocalMemoryCausality {
    bool accumulator_fired{};
    bool completion_fired{};
    bool accumulator_reconstructable{};
    bool completion_reconstructable{};
    bool full_path_reconstructable{};
    std::uint32_t required_accumulator_inputs{};
    std::uint32_t retained_accumulator_inputs{};
    std::uint32_t retained_completion_inputs{};
    std::uint32_t inactive_entries{};
    std::uint32_t unassigned_entries{};
};

class Runtime {
public:
    explicit Runtime(ExperimentConfig config = {}, std::uint64_t seed = 1);

    void enqueue_fire(NeuronId neuron);
    [[nodiscard]] brainstem::ImpulseBatchResult process_simultaneous_impulses(
        const std::vector<SynapseId>& synapses);
    [[nodiscard]] std::uint64_t run();
    [[nodiscard]] std::uint64_t advance_time_to(
        std::uint64_t target_time,
        std::uint64_t event_limit = 0);
    [[nodiscard]] std::uint64_t advance_time_by(
        std::uint64_t duration,
        std::uint64_t event_limit = 0);
    void schedule_boundary(std::uint64_t time, std::uint32_t marker = 0);
    [[nodiscard]] std::vector<brainstem::EncodedByte> process_utf8_bytes(
        const std::vector<std::uint8_t>& bytes);
    [[nodiscard]] std::vector<brainstem::EncodedByte> process_utf8_text(
        const std::string& utf8_text);

    void configure_motor_output(const brainstem::MotorOutputConfig& config);
    [[nodiscard]] brainstem::MotorResponse request_response(
        bool emit_response_stimulus = true,
        std::uint64_t safety_event_limit = 0);
    [[nodiscard]] bool motor_output_configured() const noexcept;
    [[nodiscard]] const brainstem::MotorAssignment& motor_assignment() const;
    [[nodiscard]] brainstem::InnateWiringResult configure_innate_wiring(
        const brainstem::InnateWiringConfig& config);
    [[nodiscard]] brainstem::MotorReachability motor_reachability() const;
    [[nodiscard]] brainstem::ExplorationResult enqueue_exploration(
        const brainstem::ExplorationConfig& config);
    [[nodiscard]] std::vector<NeuronId> exploration_reservations() const;
    [[nodiscard]] brainstem::MotorCircuitResult configure_motor_circuit(
        const brainstem::MotorCircuitConfig& config);
    [[nodiscard]] brainstem::FeedbackResult process_motor_feedback(
        const brainstem::MotorResponse& response,
        const brainstem::FeedbackConfig& config);
    [[nodiscard]] brainstem::EvaluationRouteResult configure_evaluation_route(
        const brainstem::EvaluationRouteConfig& config);
    [[nodiscard]] EvaluationFeedbackResult process_evaluation_feedback(
        std::uint8_t success_percent,
        brainstem::EvaluationSignalKind kind =
            brainstem::EvaluationSignalKind::graded,
        std::uint64_t scheduled_time = 0);
    [[nodiscard]] ModulatoryFeedbackResult process_modulatory_feedback(
        std::int8_t polarity,
        std::uint64_t scheduled_time = 0);
    [[nodiscard]] bool evaluation_route_configured() const noexcept;
    [[nodiscard]] brainstem::ActionContextRoute configure_action_context(
        const brainstem::ActionContextConfig& config);
    [[nodiscard]] ActionContextEmission process_action_context(
        brainstem::ActionContextSignal signal,
        std::uint8_t observed_motor_value = 0,
        std::uint64_t scheduled_time = 0,
        bool feedback_origin = false);
    [[nodiscard]] bool action_context_configured() const noexcept;

    void set_trace_stream(std::ostream* stream) noexcept;
    void set_fire_observer(FireObserver observer);
    void set_impulse_observer(ImpulseObserver observer);
    void set_recovery_observer(RecoveryObserver observer);
    void set_subthreshold_relaxation_observer(
        SubthresholdRelaxationObserver observer);
    void set_plasticity_observer(PlasticityObserver observer);
    [[nodiscard]] static MotorCausality analyze_motor_causality(
        const brainstem::MotorResponse& response,
        const brainstem::MotorCircuitResult& circuit,
        const std::vector<ImpulseObservation>& impulses);
    [[nodiscard]] LocalMemoryCausality analyze_local_motor_memory(
        const brainstem::MotorResponse& response,
        const brainstem::MotorCircuitResult& circuit,
        const std::vector<ImpulseObservation>& impulses) const;
    void set_experimental_synapse_weight(SynapseId synapse, std::int32_t weight);
    void set_experimental_synapse_effect(SynapseId synapse, SynapticEffect effect);
    void set_experimental_synapse_protection(SynapseId synapse, bool protected_state);
    void set_experimental_neuron_membrane(
        NeuronId neuron, std::int64_t membrane_potential);
    void set_experimental_eligibility_records(
        NeuronId neuron,
        const std::vector<neural::EligibilityRecord>& records);
    void set_experimental_plasticity_writes_suppressed(bool suppressed) noexcept;
    [[nodiscard]] ExperimentalTransientTransfer
    set_experimental_transient_component_from(
        const Runtime& source, ExperimentalTransientComponent component);
    void set_structural_growth_enabled(bool enabled);
    [[nodiscard]] SynapseId add_experimental_synapse(
        NeuronId sender, NeuronId receiver, std::int32_t weight,
        bool plasticity_protected = false,
        SynapticEffect effect = SynapticEffect::excitatory);

    [[nodiscard]] const ExperimentConfig& config() const noexcept;
    [[nodiscard]] const neural::Network& network() const noexcept;
    [[nodiscard]] const Metrics& metrics() const noexcept;
    [[nodiscard]] Diagnostics diagnostics() const;
    [[nodiscard]] std::uint64_t state_fingerprint() const noexcept;
    [[nodiscard]] std::uint64_t simulated_time() const noexcept;
    [[nodiscard]] std::size_t future_event_count() const noexcept;

    void save(const std::string& path) const;
    [[nodiscard]] static Runtime load(const std::string& path);

private:
    struct SkipInitializationTag {};
    Runtime(ExperimentConfig config, std::uint64_t seed, SkipInitializationTag);

    void validate_config() const;
    void process_fire(
        NeuronId neuron,
        bool feedback_context = false,
        std::uint32_t context_signature = 0,
        bool context_feedback_origin = false,
        std::int8_t modulation = 0);
    void process_impulse(
        SynapseId synapse,
        bool feedback_context = false,
        std::uint32_t context_signature = 0,
        bool context_feedback_origin = false,
        std::int8_t modulation = 0);
    void process_recovery(const Event& event);
    void process_subthreshold_relaxation(const Event& event);
    void update_subthreshold_relaxation(NeuronId neuron, bool fired);
    void process_eligibility_expiry(const Event& event);
    void process_eligibility_decay(const Event& event);
    void process_context_expiry(const Event& event);
    void process_event(const Event& event);
    void enqueue_event(
        EventKind kind,
        std::uint32_t target,
        std::uint64_t scheduled_time,
        std::uint64_t generation = 0,
        std::uint32_t context_signature = 0,
        bool context_feedback_origin = false,
        std::int8_t modulation = 0);
    void create_local_plasticity_trace(NeuronId neuron, SynapseId candidate,
                                       bool feedback_origin);
    void apply_local_modulation(
        NeuronId neuron, std::int8_t polarity, SynapseId modulation_synapse);
    [[nodiscard]] bool has_ready_event() const noexcept;
    [[nodiscard]] std::uint64_t process_ready_wave();
    [[nodiscard]] Event pop_next_event();
    void update_event_hash(const Event& event) noexcept;
    void update_queue_watermark() noexcept;
    void trace(const std::string& line);

    ExperimentConfig config_;
    neural::Network network_;
    brainstem::Brainstem brainstem_;
    DeterministicRng rng_;
    DeterministicRng growth_rng_;
    std::deque<Event> queue_;
    Metrics metrics_;
    std::ostream* trace_stream_{};
    FireObserver fire_observer_;
    ImpulseObserver impulse_observer_;
    RecoveryObserver recovery_observer_;
    SubthresholdRelaxationObserver subthreshold_relaxation_observer_;
    PlasticityObserver plasticity_observer_;
    brainstem::MotorOutput motor_output_;
    std::vector<NeuronId> innate_reservations_;
    std::vector<NeuronId> motor_circuit_reservations_;
    brainstem::EvaluationRouteResult evaluation_route_;
    bool evaluation_route_configured_{};
    brainstem::ActionContextRoute action_context_route_;
    bool action_context_configured_{};
    std::uint64_t simulated_time_{};
    std::uint64_t next_event_sequence_{};
};

}  // namespace eve::bios
