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
    std::uint64_t maximum_events_per_run{1'000'000};
    bool learning_enabled{true};
    bool growth_enabled{true};
    ResetMode reset_mode{ResetMode::to_zero};
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
    std::uint64_t growth_attempts{};
    std::uint64_t grown_synapses{};
    std::uint64_t processed_sensor_bytes{};
    std::uint64_t maximum_queue_size{};
    std::uint64_t event_trace_hash{14695981039346656037ULL};

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
               growth_attempts == other.growth_attempts &&
               grown_synapses == other.grown_synapses &&
               processed_sensor_bytes == other.processed_sensor_bytes &&
               maximum_queue_size == other.maximum_queue_size &&
               event_trace_hash == other.event_trace_hash;
    }
};

}  // namespace eve
