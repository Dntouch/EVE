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
    std::uint64_t growth_attempts{};
    std::uint64_t grown_synapses{};
    std::uint64_t processed_sensor_bytes{};
    std::uint64_t event_trace_hash{14695981039346656037ULL};

    bool operator==(const Metrics& other) const noexcept {
        return processed_events == other.processed_events &&
               transmitted_impulses == other.transmitted_impulses &&
               fire_events == other.fire_events &&
               completed_activity_phases == other.completed_activity_phases &&
               synaptic_strengthenings == other.synaptic_strengthenings &&
               growth_attempts == other.growth_attempts &&
               grown_synapses == other.grown_synapses &&
               processed_sensor_bytes == other.processed_sensor_bytes &&
               event_trace_hash == other.event_trace_hash;
    }
};

}  // namespace eve
