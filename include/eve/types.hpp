#pragma once

#include <cstdint>
#include <limits>

namespace eve {

using NeuronId = std::uint32_t;
using SynapseId = std::uint32_t;

constexpr std::uint32_t kInvalidIndex = std::numeric_limits<std::uint32_t>::max();

struct Coordinate {
    std::uint32_t x{};
    std::uint32_t y{};
    std::uint32_t z{};

    bool operator==(const Coordinate& other) const noexcept {
        return x == other.x && y == other.y && z == other.z;
    }
};

enum class ResetMode : std::uint8_t {
    to_zero = 0,
    subtract_threshold = 1,
};

enum class RecoveryMode : std::uint8_t {
    input_dependent = 0,
    independent = 1,
    independent_with_membrane_relaxation = 2,
};

enum class EligibilitySelection : std::uint8_t {
    none = 0,
    all_recent = 1,
    positive_weight = 2,
    // B.4.3: stimulus-agnostic inputs active in the current local fire window.
    // Zero-weight inputs deliberately remain eligible.
    active_phase = 3,
};

enum class EligibilityRepresentation : std::uint8_t {
    repeated = 0,
    unique = 1,
};

enum class EligibilityHistory : std::uint8_t {
    overwrite = 0,
    retain = 1,
    merge_unique = 2,
};

enum class ContextTraceMode : std::uint8_t {
    none = 0,
    single = 1,
    multiple = 2,
};

enum class ContextBindingMode : std::uint8_t {
    none = 0,
    at_fire = 1,
    temporal_overlap = 2,
};

// Research stage B.4: deliberately small, local three-factor plasticity.
enum class PlasticityTraceMode : std::uint8_t {
    disabled = 0,
    g3_recent = 1,
    pre_post_fire = 2,
    membrane_participation = 3,
};

enum class TraceDecayMode : std::uint8_t {
    immediate = 0,
    constant = 1,
    stepped = 2,
    local_activity = 3,
};

enum class ModulationMode : std::uint8_t {
    none = 0,
    positive = 1,
    negative = 2,
    ternary = 3,
};

// B.4.6: the sign of a neural contribution is an explicit, stable synaptic
// property.  Weight remains a non-negative efficacy magnitude and negative
// learning modulation remains a separate third-factor signal.
enum class SynapticEffect : std::uint8_t {
    excitatory = 0,
    inhibitory = 1,
};

enum class InhibitoryPlasticityMode : std::uint8_t {
    shared = 0,
    contribution_aware = 1,
};

struct ExperimentConfig {
    std::uint32_t neuron_count{256};
    std::uint32_t width{8};
    std::uint32_t height{8};
    std::uint32_t depth{4};
    std::uint32_t initial_outgoing_synapses{4};
    std::int64_t firing_threshold{3};
    std::uint32_t synaptic_learning_threshold{3};
    std::uint32_t activity_phase_limit{8};
    std::int32_t initial_synapse_weight{0};
    std::int32_t maximum_synapse_weight{std::numeric_limits<std::int32_t>::max()};
    std::uint64_t input_weight_budget{std::numeric_limits<std::uint64_t>::max()};
    std::uint32_t refractory_input_events{};
    RecoveryMode recovery_mode{RecoveryMode::input_dependent};
    std::uint64_t recovery_delay{16};
    std::uint32_t membrane_relaxation_percent{};
    std::uint32_t local_event_memory_capacity{};
    EligibilitySelection eligibility_selection{EligibilitySelection::none};
    EligibilityRepresentation eligibility_representation{
        EligibilityRepresentation::unique};
    EligibilityHistory eligibility_history{EligibilityHistory::retain};
    std::uint32_t eligibility_candidate_capacity{};
    std::uint32_t eligibility_decision_capacity{};
    std::uint64_t eligibility_duration{};
    ContextTraceMode context_trace_mode{ContextTraceMode::none};
    ContextBindingMode context_binding_mode{ContextBindingMode::none};
    std::uint32_t context_trace_capacity{};
    std::uint64_t context_trace_duration{};
    PlasticityTraceMode plasticity_trace_mode{PlasticityTraceMode::disabled};
    TraceDecayMode plasticity_decay_mode{TraceDecayMode::constant};
    ModulationMode modulation_mode{ModulationMode::none};
    std::int32_t plasticity_positive_step{1};
    std::int32_t plasticity_negative_step{1};
    std::int32_t plasticity_maximum_weight{3};
    bool consume_trace_on_modulation{true};
    std::uint64_t maximum_events_per_run{1'000'000};
    bool learning_enabled{true};
    bool growth_enabled{true};
    ResetMode reset_mode{ResetMode::to_zero};
    bool batch_simultaneous_inputs{};
    InhibitoryPlasticityMode inhibitory_plasticity_mode{
        InhibitoryPlasticityMode::shared};
    // B.5.5 research-only local lifetime for non-zero subthreshold membrane
    // state. Disabled by default and never scans neurons globally.
    bool subthreshold_relaxation_enabled{};
    std::uint64_t subthreshold_relaxation_delay{};
};

struct Metrics {
    std::uint64_t processed_events{};
    std::uint64_t transmitted_impulses{};
    std::uint64_t fire_events{};
    std::uint64_t completed_activity_phases{};
    std::uint64_t synaptic_strengthenings{};
    std::uint64_t rejected_strengthenings_at_weight_limit{};
    std::uint64_t synaptic_weakenings{};
    std::uint64_t refractory_suppressed_firings{};
    std::uint64_t local_event_memory_records{};
    std::uint64_t local_event_memory_overwrites{};
    std::uint64_t scheduled_recovery_events{};
    std::uint64_t processed_recovery_events{};
    std::uint64_t stale_recovery_events{};
    std::uint64_t processed_boundary_events{};
    std::uint64_t eligibility_decisions{};
    std::uint64_t eligibility_candidates{};
    std::uint64_t eligibility_positive_candidates{};
    std::uint64_t eligibility_zero_weight_candidates{};
    std::uint64_t eligibility_overwritten_decisions{};
    std::uint64_t scheduled_eligibility_expiries{};
    std::uint64_t processed_eligibility_expiries{};
    std::uint64_t stale_eligibility_expiries{};
    std::uint64_t feedback_fire_events{};
    std::uint64_t feedback_impulses{};
    std::uint64_t feedback_receipts_with_eligibility{};
    std::uint64_t feedback_receipts_without_eligibility{};
    std::uint64_t feedback_receipts_with_multiple_eligibilities{};
    std::uint64_t feedback_created_eligibilities{};
    std::uint64_t context_traces_created{};
    std::uint64_t context_traces_overwritten{};
    std::uint64_t processed_context_expiries{};
    std::uint64_t stale_context_expiries{};
    std::uint64_t eligibility_context_bindings{};
    std::uint64_t eligibility_context_ambiguities{};
    std::uint64_t feedback_context_unique_matches{};
    std::uint64_t feedback_context_ambiguous_matches{};
    std::uint64_t feedback_context_missing_matches{};
    std::uint64_t feedback_context_expired_matches{};
    std::uint64_t feedback_origin_context_matches{};
    std::uint64_t modulation_fire_events{};
    std::uint64_t modulation_impulses{};
    std::uint64_t modulation_receipts_with_trace{};
    std::uint64_t modulation_receipts_without_trace{};
    std::uint64_t plasticity_positive_changes{};
    std::uint64_t plasticity_negative_changes{};
    std::uint64_t plasticity_zero_to_one_changes{};
    std::uint64_t plasticity_rejected_at_weight_limit{};
    std::uint64_t plasticity_rejected_by_budget{};
    std::uint64_t plasticity_consumed_traces{};
    std::uint64_t plasticity_feedback_origin_traces_skipped{};
    std::uint64_t growth_attempts{};
    std::uint64_t grown_synapses{};
    std::uint64_t processed_sensor_bytes{};
    std::uint64_t maximum_queue_size{};
    std::uint64_t event_trace_hash{14695981039346656037ULL};
    std::uint64_t inhibitory_impulses{};
    std::uint64_t inhibitory_applied_strength{};
    std::uint64_t simultaneous_input_batches{};
    std::uint64_t inhibitory_positive_changes{};
    std::uint64_t inhibitory_negative_changes{};
    std::uint64_t scheduled_subthreshold_relaxations{};
    std::uint64_t processed_subthreshold_relaxations{};
    std::uint64_t stale_subthreshold_relaxations{};

    bool operator==(const Metrics& other) const noexcept {
        return processed_events == other.processed_events &&
               transmitted_impulses == other.transmitted_impulses &&
               fire_events == other.fire_events &&
               completed_activity_phases == other.completed_activity_phases &&
               synaptic_strengthenings == other.synaptic_strengthenings &&
               rejected_strengthenings_at_weight_limit ==
                   other.rejected_strengthenings_at_weight_limit &&
               synaptic_weakenings == other.synaptic_weakenings &&
               refractory_suppressed_firings == other.refractory_suppressed_firings &&
               local_event_memory_records == other.local_event_memory_records &&
               local_event_memory_overwrites == other.local_event_memory_overwrites &&
               scheduled_recovery_events == other.scheduled_recovery_events &&
               processed_recovery_events == other.processed_recovery_events &&
               stale_recovery_events == other.stale_recovery_events &&
               processed_boundary_events == other.processed_boundary_events &&
               eligibility_decisions == other.eligibility_decisions &&
               eligibility_candidates == other.eligibility_candidates &&
               eligibility_positive_candidates == other.eligibility_positive_candidates &&
               eligibility_zero_weight_candidates ==
                   other.eligibility_zero_weight_candidates &&
               eligibility_overwritten_decisions ==
                   other.eligibility_overwritten_decisions &&
               scheduled_eligibility_expiries ==
                   other.scheduled_eligibility_expiries &&
               processed_eligibility_expiries ==
                   other.processed_eligibility_expiries &&
               stale_eligibility_expiries == other.stale_eligibility_expiries &&
               feedback_fire_events == other.feedback_fire_events &&
               feedback_impulses == other.feedback_impulses &&
               feedback_receipts_with_eligibility ==
                   other.feedback_receipts_with_eligibility &&
               feedback_receipts_without_eligibility ==
                   other.feedback_receipts_without_eligibility &&
               feedback_receipts_with_multiple_eligibilities ==
                   other.feedback_receipts_with_multiple_eligibilities &&
               feedback_created_eligibilities ==
                   other.feedback_created_eligibilities &&
               context_traces_created == other.context_traces_created &&
               context_traces_overwritten == other.context_traces_overwritten &&
               processed_context_expiries == other.processed_context_expiries &&
               stale_context_expiries == other.stale_context_expiries &&
               eligibility_context_bindings == other.eligibility_context_bindings &&
               eligibility_context_ambiguities ==
                   other.eligibility_context_ambiguities &&
               feedback_context_unique_matches ==
                   other.feedback_context_unique_matches &&
               feedback_context_ambiguous_matches ==
                   other.feedback_context_ambiguous_matches &&
               feedback_context_missing_matches ==
                   other.feedback_context_missing_matches &&
               feedback_context_expired_matches ==
                   other.feedback_context_expired_matches &&
               feedback_origin_context_matches ==
                   other.feedback_origin_context_matches &&
               modulation_fire_events == other.modulation_fire_events &&
               modulation_impulses == other.modulation_impulses &&
               modulation_receipts_with_trace == other.modulation_receipts_with_trace &&
               modulation_receipts_without_trace == other.modulation_receipts_without_trace &&
               plasticity_positive_changes == other.plasticity_positive_changes &&
               plasticity_negative_changes == other.plasticity_negative_changes &&
               plasticity_zero_to_one_changes == other.plasticity_zero_to_one_changes &&
               plasticity_rejected_at_weight_limit ==
                   other.plasticity_rejected_at_weight_limit &&
               plasticity_rejected_by_budget == other.plasticity_rejected_by_budget &&
               plasticity_consumed_traces == other.plasticity_consumed_traces &&
               plasticity_feedback_origin_traces_skipped ==
                   other.plasticity_feedback_origin_traces_skipped &&
               growth_attempts == other.growth_attempts &&
               grown_synapses == other.grown_synapses &&
               processed_sensor_bytes == other.processed_sensor_bytes &&
               maximum_queue_size == other.maximum_queue_size &&
               event_trace_hash == other.event_trace_hash &&
               inhibitory_impulses == other.inhibitory_impulses &&
               inhibitory_applied_strength == other.inhibitory_applied_strength &&
               simultaneous_input_batches == other.simultaneous_input_batches &&
               inhibitory_positive_changes == other.inhibitory_positive_changes &&
               inhibitory_negative_changes == other.inhibitory_negative_changes &&
               scheduled_subthreshold_relaxations ==
                   other.scheduled_subthreshold_relaxations &&
               processed_subthreshold_relaxations ==
                   other.processed_subthreshold_relaxations &&
               stale_subthreshold_relaxations ==
                   other.stale_subthreshold_relaxations;
    }
};

}  // namespace eve
