#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace eve::bios {
namespace {

constexpr std::array<char, 8> kPersistenceMagic{{'E', 'V', 'E', '0', '0', '1', '\r', '\n'}};
constexpr std::uint32_t kPersistenceVersion = 15;
constexpr std::uint64_t kNonZeroRngState = 0x9e3779b97f4a7c15ULL;
constexpr std::uint64_t kGrowthSeedMask = 0xd1b54a32d192ed03ULL;
constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

void write_bytes(std::ostream& stream, const char* data, const std::size_t size) {
    stream.write(data, static_cast<std::streamsize>(size));
    if (!stream) {
        throw std::runtime_error("failed to write EVE state");
    }
}

void write_u8(std::ostream& stream, const std::uint8_t value) {
    const char byte = static_cast<char>(value);
    write_bytes(stream, &byte, 1);
}

void write_u32(std::ostream& stream, const std::uint32_t value) {
    std::array<char, 4> bytes{};
    for (unsigned index = 0; index < bytes.size(); ++index) {
        bytes[index] = static_cast<char>((value >> (index * 8U)) & 0xffU);
    }
    write_bytes(stream, bytes.data(), bytes.size());
}

void write_u64(std::ostream& stream, const std::uint64_t value) {
    std::array<char, 8> bytes{};
    for (unsigned index = 0; index < bytes.size(); ++index) {
        bytes[index] = static_cast<char>((value >> (index * 8U)) & 0xffU);
    }
    write_bytes(stream, bytes.data(), bytes.size());
}

void write_i32(std::ostream& stream, const std::int32_t value) {
    write_u32(stream, static_cast<std::uint32_t>(value));
}

void write_i64(std::ostream& stream, const std::int64_t value) {
    write_u64(stream, static_cast<std::uint64_t>(value));
}

void read_bytes(std::istream& stream, char* data, const std::size_t size) {
    stream.read(data, static_cast<std::streamsize>(size));
    if (!stream) {
        throw std::runtime_error("truncated or unreadable EVE state");
    }
}

std::uint8_t read_u8(std::istream& stream) {
    char byte{};
    read_bytes(stream, &byte, 1);
    return static_cast<std::uint8_t>(static_cast<unsigned char>(byte));
}

std::uint32_t read_u32(std::istream& stream) {
    std::array<char, 4> bytes{};
    read_bytes(stream, bytes.data(), bytes.size());
    std::uint32_t value = 0;
    for (unsigned index = 0; index < bytes.size(); ++index) {
        value |= static_cast<std::uint32_t>(static_cast<unsigned char>(bytes[index]))
                 << (index * 8U);
    }
    return value;
}

std::uint64_t read_u64(std::istream& stream) {
    std::array<char, 8> bytes{};
    read_bytes(stream, bytes.data(), bytes.size());
    std::uint64_t value = 0;
    for (unsigned index = 0; index < bytes.size(); ++index) {
        value |= static_cast<std::uint64_t>(static_cast<unsigned char>(bytes[index]))
                 << (index * 8U);
    }
    return value;
}

std::int32_t read_i32(std::istream& stream) {
    return static_cast<std::int32_t>(read_u32(stream));
}

std::int64_t read_i64(std::istream& stream) {
    return static_cast<std::int64_t>(read_u64(stream));
}

bool read_bool(std::istream& stream) {
    const std::uint8_t value = read_u8(stream);
    if (value > 1) {
        throw std::runtime_error("invalid boolean in EVE state");
    }
    return value != 0;
}

void write_config(std::ostream& stream, const ExperimentConfig& config) {
    write_u32(stream, config.neuron_count);
    write_u32(stream, config.width);
    write_u32(stream, config.height);
    write_u32(stream, config.depth);
    write_u32(stream, config.initial_outgoing_synapses);
    write_i64(stream, config.firing_threshold);
    write_u32(stream, config.synaptic_learning_threshold);
    write_u32(stream, config.activity_phase_limit);
    write_i32(stream, config.initial_synapse_weight);
    write_i32(stream, config.maximum_synapse_weight);
    write_u64(stream, config.input_weight_budget);
    write_u32(stream, config.refractory_input_events);
    write_u8(stream, static_cast<std::uint8_t>(config.recovery_mode));
    write_u64(stream, config.recovery_delay);
    write_u32(stream, config.membrane_relaxation_percent);
    write_u32(stream, config.local_event_memory_capacity);
    write_u8(stream, static_cast<std::uint8_t>(config.eligibility_selection));
    write_u8(stream, static_cast<std::uint8_t>(config.eligibility_representation));
    write_u8(stream, static_cast<std::uint8_t>(config.eligibility_history));
    write_u32(stream, config.eligibility_candidate_capacity);
    write_u32(stream, config.eligibility_decision_capacity);
    write_u64(stream, config.eligibility_duration);
    write_u8(stream, static_cast<std::uint8_t>(config.context_trace_mode));
    write_u8(stream, static_cast<std::uint8_t>(config.context_binding_mode));
    write_u32(stream, config.context_trace_capacity);
    write_u64(stream, config.context_trace_duration);
    write_u8(stream, static_cast<std::uint8_t>(config.plasticity_trace_mode));
    write_u8(stream, static_cast<std::uint8_t>(config.plasticity_decay_mode));
    write_u8(stream, static_cast<std::uint8_t>(config.modulation_mode));
    write_i32(stream, config.plasticity_positive_step);
    write_i32(stream, config.plasticity_negative_step);
    write_i32(stream, config.plasticity_maximum_weight);
    write_u8(stream, config.consume_trace_on_modulation ? 1 : 0);
    write_u64(stream, config.maximum_events_per_run);
    write_u8(stream, config.learning_enabled ? 1 : 0);
    write_u8(stream, config.growth_enabled ? 1 : 0);
    write_u8(stream, static_cast<std::uint8_t>(config.reset_mode));
    write_u8(stream, config.batch_simultaneous_inputs ? 1 : 0);
    write_u8(stream, static_cast<std::uint8_t>(config.inhibitory_plasticity_mode));
    write_u8(stream, config.subthreshold_relaxation_enabled ? 1 : 0);
    write_u64(stream, config.subthreshold_relaxation_delay);
    write_u8(stream, config.experimental_plasticity_writes_suppressed ? 1 : 0);
}

ExperimentConfig read_config(std::istream& stream, const std::uint32_t version) {
    ExperimentConfig config;
    config.neuron_count = read_u32(stream);
    config.width = read_u32(stream);
    config.height = read_u32(stream);
    config.depth = read_u32(stream);
    config.initial_outgoing_synapses = read_u32(stream);
    config.firing_threshold = read_i64(stream);
    config.synaptic_learning_threshold = read_u32(stream);
    config.activity_phase_limit = read_u32(stream);
    config.initial_synapse_weight = read_i32(stream);
    config.maximum_synapse_weight = read_i32(stream);
    if (version >= 2) {
        config.input_weight_budget = read_u64(stream);
        config.refractory_input_events = read_u32(stream);
    }
    if (version >= 5) {
        const std::uint8_t recovery_mode = read_u8(stream);
        if (recovery_mode > static_cast<std::uint8_t>(
                                RecoveryMode::independent_with_membrane_relaxation)) {
            throw std::runtime_error("invalid recovery mode in EVE state");
        }
        config.recovery_mode = static_cast<RecoveryMode>(recovery_mode);
        config.recovery_delay = read_u64(stream);
        config.membrane_relaxation_percent = read_u32(stream);
    }
    if (version >= 4) {
        config.local_event_memory_capacity = read_u32(stream);
    }
    if (version >= 6) {
        const std::uint8_t selection = read_u8(stream);
        const std::uint8_t representation = read_u8(stream);
        const std::uint8_t history = read_u8(stream);
        if (selection > static_cast<std::uint8_t>(EligibilitySelection::active_phase) ||
            representation > static_cast<std::uint8_t>(EligibilityRepresentation::unique) ||
            history > static_cast<std::uint8_t>(EligibilityHistory::merge_unique)) {
            throw std::runtime_error("invalid eligibility configuration in EVE state");
        }
        config.eligibility_selection = static_cast<EligibilitySelection>(selection);
        config.eligibility_representation =
            static_cast<EligibilityRepresentation>(representation);
        config.eligibility_history = static_cast<EligibilityHistory>(history);
        config.eligibility_candidate_capacity = read_u32(stream);
        config.eligibility_decision_capacity = read_u32(stream);
        config.eligibility_duration = read_u64(stream);
    }
    if (version >= 8) {
        const std::uint8_t trace_mode = read_u8(stream);
        const std::uint8_t binding_mode = read_u8(stream);
        if (trace_mode > static_cast<std::uint8_t>(ContextTraceMode::multiple) ||
            binding_mode >
                static_cast<std::uint8_t>(ContextBindingMode::temporal_overlap)) {
            throw std::runtime_error("invalid action context configuration in EVE state");
        }
        config.context_trace_mode = static_cast<ContextTraceMode>(trace_mode);
        config.context_binding_mode = static_cast<ContextBindingMode>(binding_mode);
        config.context_trace_capacity = read_u32(stream);
        config.context_trace_duration = read_u64(stream);
    }
    if (version >= 9) {
        const auto trace = read_u8(stream);
        const auto decay = read_u8(stream);
        const auto modulation = read_u8(stream);
        if (trace > static_cast<std::uint8_t>(PlasticityTraceMode::membrane_participation) ||
            decay > static_cast<std::uint8_t>(TraceDecayMode::local_activity) ||
            modulation > static_cast<std::uint8_t>(ModulationMode::ternary)) {
            throw std::runtime_error("invalid three-factor plasticity configuration");
        }
        config.plasticity_trace_mode = static_cast<PlasticityTraceMode>(trace);
        config.plasticity_decay_mode = static_cast<TraceDecayMode>(decay);
        config.modulation_mode = static_cast<ModulationMode>(modulation);
        config.plasticity_positive_step = read_i32(stream);
        config.plasticity_negative_step = read_i32(stream);
        config.plasticity_maximum_weight = read_i32(stream);
        config.consume_trace_on_modulation = read_bool(stream);
    }
    config.maximum_events_per_run = read_u64(stream);
    config.learning_enabled = read_bool(stream);
    config.growth_enabled = read_bool(stream);
    const std::uint8_t reset_mode = read_u8(stream);
    if (reset_mode > static_cast<std::uint8_t>(ResetMode::subtract_threshold)) {
        throw std::runtime_error("invalid reset mode in EVE state");
    }
    config.reset_mode = static_cast<ResetMode>(reset_mode);
    if (version >= 13) {
        config.batch_simultaneous_inputs = read_bool(stream);
        const auto mode = read_u8(stream);
        if (mode > static_cast<std::uint8_t>(
                       InhibitoryPlasticityMode::contribution_aware)) {
            throw std::runtime_error("invalid inhibitory plasticity mode");
        }
        config.inhibitory_plasticity_mode =
            static_cast<InhibitoryPlasticityMode>(mode);
    }
    if (version >= 14) {
        config.subthreshold_relaxation_enabled = read_bool(stream);
        config.subthreshold_relaxation_delay = read_u64(stream);
    }
    if (version >= 15) {
        config.experimental_plasticity_writes_suppressed = read_bool(stream);
    }
    return config;
}

void write_metrics(std::ostream& stream, const Metrics& metrics) {
    write_u64(stream, metrics.processed_events);
    write_u64(stream, metrics.transmitted_impulses);
    write_u64(stream, metrics.fire_events);
    write_u64(stream, metrics.completed_activity_phases);
    write_u64(stream, metrics.synaptic_strengthenings);
    write_u64(stream, metrics.rejected_strengthenings_at_weight_limit);
    write_u64(stream, metrics.synaptic_weakenings);
    write_u64(stream, metrics.refractory_suppressed_firings);
    write_u64(stream, metrics.local_event_memory_records);
    write_u64(stream, metrics.local_event_memory_overwrites);
    write_u64(stream, metrics.scheduled_recovery_events);
    write_u64(stream, metrics.processed_recovery_events);
    write_u64(stream, metrics.stale_recovery_events);
    write_u64(stream, metrics.processed_boundary_events);
    write_u64(stream, metrics.eligibility_decisions);
    write_u64(stream, metrics.eligibility_candidates);
    write_u64(stream, metrics.eligibility_positive_candidates);
    write_u64(stream, metrics.eligibility_zero_weight_candidates);
    write_u64(stream, metrics.eligibility_overwritten_decisions);
    write_u64(stream, metrics.scheduled_eligibility_expiries);
    write_u64(stream, metrics.processed_eligibility_expiries);
    write_u64(stream, metrics.stale_eligibility_expiries);
    write_u64(stream, metrics.feedback_fire_events);
    write_u64(stream, metrics.feedback_impulses);
    write_u64(stream, metrics.feedback_receipts_with_eligibility);
    write_u64(stream, metrics.feedback_receipts_without_eligibility);
    write_u64(stream, metrics.feedback_receipts_with_multiple_eligibilities);
    write_u64(stream, metrics.feedback_created_eligibilities);
    write_u64(stream, metrics.context_traces_created);
    write_u64(stream, metrics.context_traces_overwritten);
    write_u64(stream, metrics.processed_context_expiries);
    write_u64(stream, metrics.stale_context_expiries);
    write_u64(stream, metrics.eligibility_context_bindings);
    write_u64(stream, metrics.eligibility_context_ambiguities);
    write_u64(stream, metrics.feedback_context_unique_matches);
    write_u64(stream, metrics.feedback_context_ambiguous_matches);
    write_u64(stream, metrics.feedback_context_missing_matches);
    write_u64(stream, metrics.feedback_context_expired_matches);
    write_u64(stream, metrics.feedback_origin_context_matches);
    write_u64(stream, metrics.modulation_fire_events);
    write_u64(stream, metrics.modulation_impulses);
    write_u64(stream, metrics.modulation_receipts_with_trace);
    write_u64(stream, metrics.modulation_receipts_without_trace);
    write_u64(stream, metrics.plasticity_positive_changes);
    write_u64(stream, metrics.plasticity_negative_changes);
    write_u64(stream, metrics.plasticity_zero_to_one_changes);
    write_u64(stream, metrics.plasticity_rejected_at_weight_limit);
    write_u64(stream, metrics.plasticity_rejected_by_budget);
    write_u64(stream, metrics.plasticity_consumed_traces);
    write_u64(stream, metrics.plasticity_feedback_origin_traces_skipped);
    write_u64(stream, metrics.growth_attempts);
    write_u64(stream, metrics.grown_synapses);
    write_u64(stream, metrics.processed_sensor_bytes);
    write_u64(stream, metrics.maximum_queue_size);
    write_u64(stream, metrics.event_trace_hash);
    write_u64(stream, metrics.inhibitory_impulses);
    write_u64(stream, metrics.inhibitory_applied_strength);
    write_u64(stream, metrics.simultaneous_input_batches);
    write_u64(stream, metrics.inhibitory_positive_changes);
    write_u64(stream, metrics.inhibitory_negative_changes);
    write_u64(stream, metrics.scheduled_subthreshold_relaxations);
    write_u64(stream, metrics.processed_subthreshold_relaxations);
    write_u64(stream, metrics.stale_subthreshold_relaxations);
    write_u64(stream, metrics.plasticity_suppressed_writes);
}

Metrics read_metrics(std::istream& stream, const std::uint32_t version) {
    Metrics metrics;
    metrics.processed_events = read_u64(stream);
    metrics.transmitted_impulses = read_u64(stream);
    metrics.fire_events = read_u64(stream);
    metrics.completed_activity_phases = read_u64(stream);
    metrics.synaptic_strengthenings = read_u64(stream);
    if (version >= 3) {
        metrics.rejected_strengthenings_at_weight_limit = read_u64(stream);
    }
    if (version >= 2) {
        metrics.synaptic_weakenings = read_u64(stream);
        metrics.refractory_suppressed_firings = read_u64(stream);
    }
    if (version >= 4) {
        metrics.local_event_memory_records = read_u64(stream);
        metrics.local_event_memory_overwrites = read_u64(stream);
    }
    if (version >= 5) {
        metrics.scheduled_recovery_events = read_u64(stream);
        metrics.processed_recovery_events = read_u64(stream);
        metrics.stale_recovery_events = read_u64(stream);
        metrics.processed_boundary_events = read_u64(stream);
    }
    if (version >= 6) {
        metrics.eligibility_decisions = read_u64(stream);
        metrics.eligibility_candidates = read_u64(stream);
        metrics.eligibility_positive_candidates = read_u64(stream);
        metrics.eligibility_zero_weight_candidates = read_u64(stream);
        metrics.eligibility_overwritten_decisions = read_u64(stream);
        metrics.scheduled_eligibility_expiries = read_u64(stream);
        metrics.processed_eligibility_expiries = read_u64(stream);
        metrics.stale_eligibility_expiries = read_u64(stream);
    }
    if (version >= 7) {
        metrics.feedback_fire_events = read_u64(stream);
        metrics.feedback_impulses = read_u64(stream);
        metrics.feedback_receipts_with_eligibility = read_u64(stream);
        metrics.feedback_receipts_without_eligibility = read_u64(stream);
        metrics.feedback_receipts_with_multiple_eligibilities = read_u64(stream);
        metrics.feedback_created_eligibilities = read_u64(stream);
    }
    if (version >= 8) {
        metrics.context_traces_created = read_u64(stream);
        metrics.context_traces_overwritten = read_u64(stream);
        metrics.processed_context_expiries = read_u64(stream);
        metrics.stale_context_expiries = read_u64(stream);
        metrics.eligibility_context_bindings = read_u64(stream);
        metrics.eligibility_context_ambiguities = read_u64(stream);
        metrics.feedback_context_unique_matches = read_u64(stream);
        metrics.feedback_context_ambiguous_matches = read_u64(stream);
        metrics.feedback_context_missing_matches = read_u64(stream);
        metrics.feedback_context_expired_matches = read_u64(stream);
        metrics.feedback_origin_context_matches = read_u64(stream);
    }
    if (version >= 9) {
        metrics.modulation_fire_events = read_u64(stream);
        metrics.modulation_impulses = read_u64(stream);
        metrics.modulation_receipts_with_trace = read_u64(stream);
        metrics.modulation_receipts_without_trace = read_u64(stream);
        metrics.plasticity_positive_changes = read_u64(stream);
        metrics.plasticity_negative_changes = read_u64(stream);
        metrics.plasticity_zero_to_one_changes = read_u64(stream);
        metrics.plasticity_rejected_at_weight_limit = read_u64(stream);
        metrics.plasticity_rejected_by_budget = read_u64(stream);
        metrics.plasticity_consumed_traces = read_u64(stream);
        metrics.plasticity_feedback_origin_traces_skipped = read_u64(stream);
    }
    metrics.growth_attempts = read_u64(stream);
    metrics.grown_synapses = read_u64(stream);
    metrics.processed_sensor_bytes = read_u64(stream);
    if (version >= 2) {
        metrics.maximum_queue_size = read_u64(stream);
    }
    metrics.event_trace_hash = read_u64(stream);
    if (version >= 13) {
        metrics.inhibitory_impulses = read_u64(stream);
        metrics.inhibitory_applied_strength = read_u64(stream);
        metrics.simultaneous_input_batches = read_u64(stream);
        metrics.inhibitory_positive_changes = read_u64(stream);
        metrics.inhibitory_negative_changes = read_u64(stream);
    }
    if (version >= 14) {
        metrics.scheduled_subthreshold_relaxations = read_u64(stream);
        metrics.processed_subthreshold_relaxations = read_u64(stream);
        metrics.stale_subthreshold_relaxations = read_u64(stream);
    }
    if (version >= 15) {
        metrics.plasticity_suppressed_writes = read_u64(stream);
    }
    return metrics;
}

void mix_hash(std::uint64_t& hash, const std::uint64_t value) noexcept {
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash ^= (value >> shift) & 0xffU;
        hash *= kFnvPrime;
    }
}

}  // namespace

DeterministicRng::DeterministicRng(const std::uint64_t seed) noexcept
    : state_(seed == 0 ? kNonZeroRngState : seed) {}

std::uint64_t DeterministicRng::next() noexcept {
    std::uint64_t value = state_;
    value ^= value >> 12U;
    value ^= value << 25U;
    value ^= value >> 27U;
    state_ = value;
    return value * 2685821657736338717ULL;
}

std::uint32_t DeterministicRng::bounded(const std::uint32_t exclusive_upper_bound) {
    if (exclusive_upper_bound == 0) {
        throw std::invalid_argument("random bound must be greater than zero");
    }
    const std::uint64_t bound = exclusive_upper_bound;
    const std::uint64_t rejection_limit = (0ULL - bound) % bound;
    std::uint64_t value = 0;
    do {
        value = next();
    } while (value < rejection_limit);
    return static_cast<std::uint32_t>(value % bound);
}

std::uint64_t DeterministicRng::state() const noexcept {
    return state_;
}

void DeterministicRng::set_state(const std::uint64_t state) {
    if (state == 0) {
        throw std::invalid_argument("random state must not be zero");
    }
    state_ = state;
}

Runtime::Runtime(ExperimentConfig config, const std::uint64_t seed)
    : config_(std::move(config)), rng_(seed), growth_rng_(seed ^ kGrowthSeedMask) {
    validate_config();
    brainstem_.initialize_network(network_, config_, rng_);
}

Runtime::Runtime(
    ExperimentConfig config,
    const std::uint64_t seed,
    SkipInitializationTag)
    : config_(std::move(config)), rng_(seed), growth_rng_(seed ^ kGrowthSeedMask) {
    validate_config();
}

void Runtime::enqueue_fire(const NeuronId neuron) {
    static_cast<void>(network_.neuron(neuron));
    enqueue_event(EventKind::fire, neuron, simulated_time_);
}

brainstem::ImpulseBatchResult Runtime::process_simultaneous_impulses(
    const std::vector<SynapseId>& synapses) {
    if (!config_.batch_simultaneous_inputs) {
        throw std::logic_error("simultaneous input batching is disabled");
    }
    if (synapses.empty()) {
        throw std::invalid_argument("simultaneous input batch must not be empty");
    }
    std::vector<SynapseId> canonical = synapses;
    std::sort(canonical.begin(), canonical.end());
    const NeuronId receiver = network_.synapse(canonical.front()).receiver;
    for (const SynapseId id : canonical) {
        if (network_.synapse(id).receiver != receiver) {
            throw std::invalid_argument(
                "simultaneous input batch must have one local receiver");
        }
    }

    const auto refractory_before = network_.neuron(receiver).refractory_events_remaining;
    const auto membrane_before = network_.neuron(receiver).membrane_potential;
    for (const SynapseId id : canonical) {
        Event event{EventKind::impulse, id, simulated_time_, next_event_sequence_++};
        ++metrics_.processed_events;
        update_event_hash(event);
    }
    auto batch = brainstem_.receive_impulse_batch(
        network_, canonical, config_, &rng_);
    ++metrics_.simultaneous_input_batches;

    brainstem::EligibilityResult eligibility;
    if (batch.fired && config_.eligibility_selection != EligibilitySelection::none) {
        const std::uint64_t duration =
            config_.plasticity_trace_mode != PlasticityTraceMode::disabled &&
                    config_.plasticity_decay_mode == TraceDecayMode::immediate
                ? 0 : config_.eligibility_duration;
        if (duration > std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
            throw std::overflow_error("eligibility expiry time overflow");
        }
        eligibility = brainstem_.create_eligibility(
            network_, receiver, simulated_time_ + duration, config_, false);
        if (eligibility.created) {
            ++metrics_.eligibility_decisions;
            metrics_.eligibility_candidates += eligibility.candidates.size();
            metrics_.eligibility_positive_candidates += eligibility.positive_candidates;
            metrics_.eligibility_zero_weight_candidates +=
                eligibility.zero_weight_candidates;
            metrics_.eligibility_overwritten_decisions +=
                eligibility.overwritten_decisions;
        }
    }

    for (std::size_t index = 0; index < batch.impulses.size(); ++index) {
        const auto& result = batch.impulses[index];
        const SynapseId id = batch.synapses[index];
        const auto& item = network_.synapse(id);
        if (result.local_event_recorded) ++metrics_.local_event_memory_records;
        if (result.local_event_overwritten) ++metrics_.local_event_memory_overwrites;
        if (result.strengthened) ++metrics_.synaptic_strengthenings;
        if (result.strengthening_rejected_by_weight_limit) {
            ++metrics_.rejected_strengthenings_at_weight_limit;
        }
        if (result.weakened) ++metrics_.synaptic_weakenings;
        if (result.effect == SynapticEffect::inhibitory) {
            ++metrics_.inhibitory_impulses;
            metrics_.inhibitory_applied_strength +=
                static_cast<std::uint64_t>(
                    -static_cast<std::int64_t>(result.applied_weight));
        }
        if (impulse_observer_) {
            impulse_observer_(ImpulseObservation{
                metrics_.processed_events - batch.impulses.size() + index + 1,
                simulated_time_, id, item.sender, receiver,
                result.applied_weight, result.effect, membrane_before,
                batch.membrane_after, refractory_before,
                network_.neuron(receiver).refractory_events_remaining,
                batch.fired, batch.firing_suppressed, false, 0, false,
                network_.local_event_memory(receiver),
                result.fired ? eligibility.generation : 0,
                result.fired ? eligibility.candidates : std::vector<SynapseId>{},
                batch.synapses,
                [&batch] {
                    std::vector<std::int32_t> weights;
                    weights.reserve(batch.impulses.size());
                    for (const auto& impulse : batch.impulses) {
                        weights.push_back(impulse.applied_weight);
                    }
                    return weights;
                }(),
                batch.excitatory_contribution,
                batch.inhibitory_contribution,
                batch.excitatory_contribution - batch.inhibitory_contribution,
                batch.membrane_at_decision,
                batch.firing_threshold,
                batch.decision_margin,
                batch.refractory_before != 0,
                batch.synapses.size() > 1,
                static_cast<std::uint32_t>(index),
            });
        }
    }
    if (batch.firing_suppressed) ++metrics_.refractory_suppressed_firings;
    if (batch.phase_completed) ++metrics_.completed_activity_phases;
    if (batch.fired && config_.eligibility_selection ==
                           EligibilitySelection::active_phase) {
        network_.reset_incoming_activity(receiver);
    }
    if (batch.fired) {
        enqueue_event(EventKind::fire, receiver, simulated_time_);
        if (config_.recovery_mode != RecoveryMode::input_dependent) {
            const std::uint64_t due = simulated_time_ + config_.recovery_delay;
            network_.neuron(receiver).scheduled_recovery_time = due;
            enqueue_event(EventKind::recovery, receiver, due,
                          batch.recovery_generation);
            ++metrics_.scheduled_recovery_events;
        }
        if (eligibility.created) {
            enqueue_event(EventKind::eligibility_expiry, receiver,
                          eligibility.expires_at, eligibility.generation);
            ++metrics_.scheduled_eligibility_expiries;
        }
    }
    update_subthreshold_relaxation(receiver, batch.fired);
    return batch;
}

std::uint64_t Runtime::run() {
    std::uint64_t processed = 0;
    while (has_ready_event()) {
        if (processed >= config_.maximum_events_per_run) {
            throw std::runtime_error(
                "event processing limit reached with " + std::to_string(queue_.size()) +
                " events still queued");
        }
        if (config_.batch_simultaneous_inputs) {
            const auto wave_size = static_cast<std::uint64_t>(std::count_if(
                queue_.begin(), queue_.end(), [this](const Event& event) {
                    return event.scheduled_time == simulated_time_;
                }));
            if (wave_size > config_.maximum_events_per_run - processed) {
                throw std::runtime_error("event processing limit reached inside batch");
            }
            processed += process_ready_wave();
        } else {
            const Event event = pop_next_event();
            ++processed;
            process_event(event);
        }
    }
    return processed;
}

std::uint64_t Runtime::advance_time_to(
    const std::uint64_t target_time, const std::uint64_t event_limit) {
    if (target_time < simulated_time_) {
        throw std::invalid_argument("simulated time cannot move backwards");
    }
    const std::uint64_t limit =
        event_limit == 0 ? config_.maximum_events_per_run : event_limit;
    std::uint64_t processed = 0;
    while (!queue_.empty() && queue_.front().scheduled_time <= target_time) {
        if (processed >= limit) {
            throw std::runtime_error("time advance event limit reached");
        }
        simulated_time_ = queue_.front().scheduled_time;
        if (config_.batch_simultaneous_inputs) {
            const auto wave_size = static_cast<std::uint64_t>(std::count_if(
                queue_.begin(), queue_.end(), [this](const Event& event) {
                    return event.scheduled_time == simulated_time_;
                }));
            if (wave_size > limit - processed) {
                throw std::runtime_error("time advance event limit reached inside batch");
            }
            processed += process_ready_wave();
        } else {
            const Event event = pop_next_event();
            ++processed;
            process_event(event);
        }
    }
    simulated_time_ = target_time;
    return processed;
}

std::uint64_t Runtime::advance_time_by(
    const std::uint64_t duration, const std::uint64_t event_limit) {
    if (duration > std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
        throw std::overflow_error("simulated time overflow");
    }
    return advance_time_to(simulated_time_ + duration, event_limit);
}

void Runtime::schedule_boundary(const std::uint64_t time, const std::uint32_t marker) {
    if (time < simulated_time_) {
        throw std::invalid_argument("boundary cannot be scheduled in the past");
    }
    enqueue_event(EventKind::boundary, marker, time);
}

std::vector<brainstem::EncodedByte> Runtime::process_utf8_bytes(
    const std::vector<std::uint8_t>& bytes) {
    auto encoded = brainstem_.encode_utf8_bytes(bytes, network_.neuron_count());
    const std::uint64_t first_byte_index = metrics_.processed_sensor_bytes;
    for (brainstem::EncodedByte& item : encoded) {
        item.byte_index += first_byte_index;
    }
    for (const brainstem::EncodedByte& item : encoded) {
        std::ostringstream begin;
        begin << "sensor byte=" << item.byte_index << " value="
              << static_cast<unsigned>(item.value) << " begin";
        trace(begin.str());

        for (const NeuronId channel : item.fired_channels) {
            trace("sensor byte=" + std::to_string(item.byte_index) + " channel=" +
                  std::to_string(channel));
            enqueue_fire(channel);
        }
        static_cast<void>(run());
        ++metrics_.processed_sensor_bytes;
        trace("sensor byte=" + std::to_string(item.byte_index) + " end");
    }
    return encoded;
}

std::vector<brainstem::EncodedByte> Runtime::process_utf8_text(const std::string& utf8_text) {
    std::vector<std::uint8_t> bytes;
    bytes.reserve(utf8_text.size());
    for (const unsigned char byte : utf8_text) {
        bytes.push_back(byte);
    }
    return process_utf8_bytes(bytes);
}

void Runtime::configure_motor_output(const brainstem::MotorOutputConfig& config) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot reconfigure an active motor response");
    }
    motor_output_.configure(network_.neuron_count(), config);
    innate_reservations_.clear();
    motor_circuit_reservations_.clear();
}

brainstem::MotorResponse Runtime::request_response(
    const bool emit_response_stimulus,
    const std::uint64_t safety_event_limit) {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output is not configured");
    }
    const std::uint64_t limit =
        safety_event_limit == 0 ? config_.maximum_events_per_run : safety_event_limit;
    motor_output_.begin_response();
    if (emit_response_stimulus) {
        enqueue_fire(motor_output_.assignment().config.response_stimulus_neuron);
    }

    std::uint64_t processed = 0;
    while (has_ready_event() && motor_output_.active()) {
        if (processed >= limit) {
            motor_output_.abort();
            break;
        }
        if (config_.batch_simultaneous_inputs) {
            const auto wave_size = static_cast<std::uint64_t>(std::count_if(
                queue_.begin(), queue_.end(), [this](const Event& event) {
                    return event.scheduled_time == simulated_time_;
                }));
            if (wave_size > limit - processed) {
                motor_output_.abort();
                break;
            }
            processed += process_ready_wave();
        } else {
            const Event event = pop_next_event();
            ++processed;
            process_event(event);
        }
    }
    if (motor_output_.active()) {
        motor_output_.finish_without_completion();
    }
    brainstem::MotorResponse response = motor_output_.response();
    response.processed_events = processed;
    return response;
}

bool Runtime::motor_output_configured() const noexcept {
    return motor_output_.configured();
}

const brainstem::MotorAssignment& Runtime::motor_assignment() const {
    return motor_output_.assignment();
}

brainstem::InnateWiringResult Runtime::configure_innate_wiring(
    const brainstem::InnateWiringConfig& config) {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before innate wiring");
    }
    if (config.initial_weight < 0 ||
        config.initial_weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("innate weight is outside configured synaptic bounds");
    }
    brainstem::InnateWiringResult result = brainstem_.create_innate_wiring(
        network_, motor_output_.assignment(), config);
    for (const brainstem::InnatePath& path : result.paths) {
        innate_reservations_.push_back(path.first_relay);
        innate_reservations_.push_back(path.second_relay);
    }
    std::sort(innate_reservations_.begin(), innate_reservations_.end());
    innate_reservations_.erase(
        std::unique(innate_reservations_.begin(), innate_reservations_.end()),
        innate_reservations_.end());
    return result;
}

brainstem::MotorReachability Runtime::motor_reachability() const {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before reachability analysis");
    }
    return brainstem_.analyze_motor_reachability(
        network_, motor_output_.assignment());
}

brainstem::ExplorationResult Runtime::enqueue_exploration(
    const brainstem::ExplorationConfig& config) {
    brainstem::ExplorationResult result =
        brainstem_.select_exploration(network_, config, exploration_reservations());
    for (const NeuronId neuron : result.activated_neurons) {
        enqueue_fire(neuron);
    }
    return result;
}

std::vector<NeuronId> Runtime::exploration_reservations() const {
    std::vector<NeuronId> reserved;
    if (!motor_output_.configured()) {
        return reserved;
    }
    const brainstem::MotorAssignment& assignment = motor_output_.assignment();
    for (NeuronId neuron = 0; neuron < 8 && neuron < network_.neuron_count(); ++neuron) {
        reserved.push_back(neuron);
    }
    reserved.push_back(assignment.config.response_stimulus_neuron);
    for (NeuronId neuron = assignment.config.response_stimulus_neuron + 1U;
         neuron < assignment.config.first_motor_neuron;
         ++neuron) {
        reserved.push_back(neuron);
    }
    for (const auto& channel : assignment.channels) {
        reserved.insert(reserved.end(), channel.begin(), channel.end());
    }
    reserved.push_back(assignment.completion_contact);
    reserved.insert(reserved.end(), innate_reservations_.begin(), innate_reservations_.end());
    reserved.insert(reserved.end(), motor_circuit_reservations_.begin(),
                    motor_circuit_reservations_.end());
    std::sort(reserved.begin(), reserved.end());
    reserved.erase(std::unique(reserved.begin(), reserved.end()), reserved.end());
    return reserved;
}

brainstem::MotorCircuitResult Runtime::configure_motor_circuit(
    const brainstem::MotorCircuitConfig& config) {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before motor circuit");
    }
    if (config.motor_weight < 0 || config.completion_weight < 0 ||
        config.motor_weight > config_.maximum_synapse_weight ||
        config.completion_weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("motor circuit weight is outside synaptic bounds");
    }
    brainstem::MotorCircuitResult result = brainstem_.create_motor_circuit(
        network_, motor_output_.assignment(), config);
    motor_circuit_reservations_ = result.relay_neurons;
    return result;
}

brainstem::FeedbackResult Runtime::process_motor_feedback(
    const brainstem::MotorResponse& response,
    const brainstem::FeedbackConfig& config) {
    brainstem::FeedbackResult result;
    result.mode = config.mode;
    result.observed_motor_value = response.value;
    result.environment_before = config.environment_state;
    result.environment_after = config.environment_state;
    if (config.mode == brainstem::FeedbackMode::none) {
        return result;
    }
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before feedback");
    }
    if (config.event_budget == 0 || config.first_feedback_neuron < 9 ||
        static_cast<std::uint64_t>(config.first_feedback_neuron) + 8U >=
            network_.neuron_count()) {
        throw std::invalid_argument("invalid sensorimotor feedback configuration");
    }

    std::vector<bool> motor_contact(network_.neuron_count(), false);
    for (const auto& channel : motor_output_.assignment().channels) {
        for (const NeuronId neuron : channel) {
            motor_contact[neuron] = true;
        }
    }
    motor_contact[motor_output_.assignment().completion_contact] = true;
    for (NeuronId offset = 0; offset < 9; ++offset) {
        const NeuronId neuron = config.first_feedback_neuron + offset;
        if (neuron == motor_output_.assignment().config.response_stimulus_neuron ||
            motor_contact[neuron]) {
            throw std::invalid_argument("feedback channels overlap motor or control contacts");
        }
    }

    const auto enqueue_feedback = [this, &result, &config](
                                      const std::int8_t source_channel,
                                      const NeuronId offset) {
        const NeuronId sensory_neuron = config.first_feedback_neuron + offset;
        result.sensory_events.push_back(
            brainstem::FeedbackEvent{source_channel, sensory_neuron});
        enqueue_fire(sensory_neuron);
    };
    const auto enqueue_value = [&enqueue_feedback](const std::uint8_t value) {
        for (std::uint32_t bit = 0; bit < 8; ++bit) {
            if ((value & static_cast<std::uint8_t>(1U << bit)) != 0) {
                enqueue_feedback(static_cast<std::int8_t>(bit), bit);
            }
        }
        enqueue_feedback(-1, 8);
    };
    const auto process_pending = [this, &result, &config]() {
        while (has_ready_event()) {
            if (result.processed_events >= config.event_budget) {
                result.aborted = true;
                return false;
            }
            const Event event = pop_next_event();
            ++result.processed_events;
            process_event(event);
        }
        return true;
    };

    if (config.mode == brainstem::FeedbackMode::per_motor_event) {
        for (const brainstem::MotorActivation& activation : response.activations) {
            if (activation.channel < 0) {
                continue;
            }
            enqueue_feedback(
                activation.channel, static_cast<NeuronId>(activation.channel));
            if (!process_pending()) {
                break;
            }
        }
        return result;
    }

    if (!response.valid()) {
        return result;
    }
    if (config.mode == brainstem::FeedbackMode::virtual_environment) {
        const std::uint8_t rotated = static_cast<std::uint8_t>(
            (config.environment_state << 1U) | (config.environment_state >> 7U));
        result.environment_after = rotated ^ response.value;
        enqueue_value(result.environment_after);
    } else {
        enqueue_value(response.value);
    }
    static_cast<void>(process_pending());
    return result;
}

brainstem::EvaluationRouteResult Runtime::configure_evaluation_route(
    const brainstem::EvaluationRouteConfig& config) {
    if (config.weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("evaluation route weight exceeds configured maximum");
    }
    evaluation_route_ = brainstem_.create_evaluation_route(network_, config);
    evaluation_route_configured_ = true;
    return evaluation_route_;
}

EvaluationFeedbackResult Runtime::process_evaluation_feedback(
    const std::uint8_t success_percent,
    const brainstem::EvaluationSignalKind kind,
    const std::uint64_t scheduled_time) {
    if (!evaluation_route_configured_) {
        throw std::logic_error("evaluation route is not configured");
    }
    EvaluationFeedbackResult result;
    result.encoding = brainstem::Brainstem::encode_evaluation(success_percent, kind);
    if (evaluation_route_.config.mode == brainstem::EvaluationRouteMode::none) {
        return result;
    }
    const std::uint64_t due = scheduled_time == 0 ? simulated_time_ : scheduled_time;
    if (due < simulated_time_) {
        throw std::invalid_argument("evaluation feedback cannot be scheduled in the past");
    }
    const Metrics before = metrics_;
    for (const std::uint8_t channel : result.encoding.active_channels) {
        if (channel >= evaluation_route_.input_neurons.size()) {
            throw std::logic_error("evaluation encoding exceeds configured input channels");
        }
        enqueue_event(EventKind::feedback_fire,
                      evaluation_route_.input_neurons[channel], due);
    }
    if (due == simulated_time_) {
        result.processed_events = run();
    }
    result.feedback_fires = metrics_.feedback_fire_events - before.feedback_fire_events;
    result.feedback_impulses = metrics_.feedback_impulses - before.feedback_impulses;
    result.receipts_with_eligibility =
        metrics_.feedback_receipts_with_eligibility -
        before.feedback_receipts_with_eligibility;
    result.receipts_without_eligibility =
        metrics_.feedback_receipts_without_eligibility -
        before.feedback_receipts_without_eligibility;
    result.receipts_with_multiple_eligibilities =
        metrics_.feedback_receipts_with_multiple_eligibilities -
        before.feedback_receipts_with_multiple_eligibilities;
    result.created_eligibilities = metrics_.feedback_created_eligibilities -
                                   before.feedback_created_eligibilities;
    result.unique_context_matches = metrics_.feedback_context_unique_matches -
                                    before.feedback_context_unique_matches;
    result.ambiguous_context_matches =
        metrics_.feedback_context_ambiguous_matches -
        before.feedback_context_ambiguous_matches;
    result.missing_context_matches = metrics_.feedback_context_missing_matches -
                                     before.feedback_context_missing_matches;
    result.expired_context_matches = metrics_.feedback_context_expired_matches -
                                     before.feedback_context_expired_matches;
    return result;
}

ModulatoryFeedbackResult Runtime::process_modulatory_feedback(
    const std::int8_t polarity,
    const std::uint64_t scheduled_time) {
    if (!evaluation_route_configured_) {
        throw std::logic_error("evaluation route is not configured");
    }
    if (polarity < -1 || polarity > 1) {
        throw std::invalid_argument("modulatory polarity must be -1, 0 or 1");
    }
    ModulatoryFeedbackResult result;
    result.polarity = polarity;
    const bool accepted =
        config_.modulation_mode == ModulationMode::ternary ||
        (polarity > 0 && config_.modulation_mode == ModulationMode::positive) ||
        (polarity < 0 && config_.modulation_mode == ModulationMode::negative);
    if (config_.modulation_mode == ModulationMode::none ||
        (polarity != 0 && !accepted) ||
        evaluation_route_.config.mode == brainstem::EvaluationRouteMode::none) {
        return result;
    }
    const std::uint64_t due = scheduled_time == 0 ? simulated_time_ : scheduled_time;
    if (due < simulated_time_) {
        throw std::invalid_argument("modulatory feedback cannot be scheduled in the past");
    }
    const Metrics before = metrics_;
    // All evaluation inputs carry the same scalar field.  Thus polarity never
    // identifies an action and the ordinary neural route decides where it arrives.
    for (const NeuronId input : evaluation_route_.input_neurons) {
        enqueue_event(EventKind::modulation_fire, input, due, 0, 0, true,
                      polarity == 0 ? 2 : polarity);
    }
    if (due == simulated_time_) result.processed_events = run();
    result.neural_fires = metrics_.modulation_fire_events - before.modulation_fire_events;
    result.neural_impulses = metrics_.modulation_impulses - before.modulation_impulses;
    result.receipts_with_trace = metrics_.modulation_receipts_with_trace -
                                 before.modulation_receipts_with_trace;
    result.receipts_without_trace = metrics_.modulation_receipts_without_trace -
                                    before.modulation_receipts_without_trace;
    result.positive_changes = metrics_.plasticity_positive_changes -
                              before.plasticity_positive_changes;
    result.negative_changes = metrics_.plasticity_negative_changes -
                              before.plasticity_negative_changes;
    result.zero_to_one_changes = metrics_.plasticity_zero_to_one_changes -
                                 before.plasticity_zero_to_one_changes;
    result.suppressed_writes = metrics_.plasticity_suppressed_writes -
                               before.plasticity_suppressed_writes;
    return result;
}

bool Runtime::evaluation_route_configured() const noexcept {
    return evaluation_route_configured_;
}

brainstem::ActionContextRoute Runtime::configure_action_context(
    const brainstem::ActionContextConfig& config) {
    if (config.weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("action context weight exceeds configured maximum");
    }
    action_context_route_ = brainstem_.create_action_context_route(network_, config);
    action_context_configured_ = true;
    return action_context_route_;
}

ActionContextEmission Runtime::process_action_context(
    const brainstem::ActionContextSignal signal,
    const std::uint8_t observed_motor_value,
    const std::uint64_t scheduled_time,
    const bool feedback_origin) {
    if (!action_context_configured_) {
        throw std::logic_error("action context route is not configured");
    }
    ActionContextEmission result;
    result.signal = signal;
    result.observed_motor_value = observed_motor_value;
    const auto mode = action_context_route_.config.mode;
    const bool boundary_allowed =
        mode == brainstem::ActionContextMode::boundary ||
        mode == brainstem::ActionContextMode::general_circuit ||
        mode == brainstem::ActionContextMode::combined;
    const bool motor_allowed =
        mode == brainstem::ActionContextMode::motor_copy ||
        mode == brainstem::ActionContextMode::combined;
    if (mode == brainstem::ActionContextMode::none ||
        (signal == brainstem::ActionContextSignal::motor_copy && !motor_allowed) ||
        (signal != brainstem::ActionContextSignal::motor_copy && !boundary_allowed)) {
        return result;
    }
    const std::uint64_t due = scheduled_time == 0 ? simulated_time_ : scheduled_time;
    if (due < simulated_time_) {
        throw std::invalid_argument("action context cannot be scheduled in the past");
    }
    if (signal == brainstem::ActionContextSignal::start ||
        signal == brainstem::ActionContextSignal::end) {
        result.active_inputs.push_back(action_context_route_.boundary_inputs[
            signal == brainstem::ActionContextSignal::start ? 0U : 1U]);
    } else {
        for (std::uint32_t bit = 0; bit < 8; ++bit) {
            if ((observed_motor_value & static_cast<std::uint8_t>(1U << bit)) != 0) {
                result.active_inputs.push_back(action_context_route_.motor_inputs[bit]);
            }
        }
    }
    if (result.active_inputs.empty()) {
        return result;
    }
    std::uint64_t signature = 14695981039346656037ULL;
    mix_hash(signature, static_cast<std::uint8_t>(signal) + 1U);
    for (const auto input : result.active_inputs) mix_hash(signature, input + 1U);
    result.signature = static_cast<std::uint32_t>(signature ^ (signature >> 32U));
    if (result.signature == 0) result.signature = 1;
    const Metrics before = metrics_;
    for (const auto input : result.active_inputs) {
        enqueue_event(EventKind::fire, input, due, 0, result.signature,
                      feedback_origin);
    }
    if (due == simulated_time_) {
        result.processed_events = run();
    }
    result.created_traces = metrics_.context_traces_created -
                            before.context_traces_created;
    result.bound_eligibilities = metrics_.eligibility_context_bindings -
                                 before.eligibility_context_bindings;
    return result;
}

bool Runtime::action_context_configured() const noexcept {
    return action_context_configured_;
}

void Runtime::set_trace_stream(std::ostream* stream) noexcept {
    trace_stream_ = stream;
}

void Runtime::set_fire_observer(FireObserver observer) {
    fire_observer_ = std::move(observer);
}

void Runtime::set_impulse_observer(ImpulseObserver observer) {
    impulse_observer_ = std::move(observer);
}

void Runtime::set_recovery_observer(RecoveryObserver observer) {
    recovery_observer_ = std::move(observer);
}

void Runtime::set_subthreshold_relaxation_observer(
    SubthresholdRelaxationObserver observer) {
    subthreshold_relaxation_observer_ = std::move(observer);
}

void Runtime::set_plasticity_observer(PlasticityObserver observer) {
    plasticity_observer_ = std::move(observer);
}

void Runtime::set_experimental_plasticity_write_interceptor(
    PlasticityWriteInterceptor interceptor) {
    plasticity_write_interceptor_ = std::move(interceptor);
}

MotorCausality Runtime::analyze_motor_causality(
    const brainstem::MotorResponse& response,
    const brainstem::MotorCircuitResult& circuit,
    const std::vector<ImpulseObservation>& impulses) {
    MotorCausality result;
    result.technical_abort =
        response.status == brainstem::MotorResponseStatus::aborted;
    result.completed = response.valid();

    const brainstem::MotorActivation* completion = nullptr;
    const brainstem::MotorActivation* first_data = nullptr;
    std::set<NeuronId> causally_reached;
    for (const brainstem::MotorActivation& activation : response.activations) {
        if (activation.channel >= 0) {
            if (first_data == nullptr) {
                first_data = &activation;
            }
            causally_reached.insert(activation.neuron);
        } else {
            completion = &activation;
            break;
        }
    }
    if (completion != nullptr) {
        result.completion_event = completion->event_index;
    }
    if (result.completed &&
        (first_data == nullptr || first_data->event_index >= completion->event_index)) {
        result.completed_without_prior_data = true;
        return result;
    }
    if (first_data == nullptr) {
        return result;
    }
    result.first_data_event = first_data->event_index;
    if (result.completed) {
        result.completed_after_data = true;
        result.events_between_data_and_completion =
            completion->event_index - first_data->event_index - 1U;
    }

    const std::set<SynapseId> circuit_synapses(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    for (const ImpulseObservation& impulse : impulses) {
        if (impulse.event_index <= first_data->event_index ||
            (completion != nullptr && impulse.event_index >= completion->event_index) ||
            circuit_synapses.find(impulse.synapse) == circuit_synapses.end()) {
            continue;
        }
        ++result.observed_circuit_impulses;
        if (impulse.firing_suppressed) {
            ++result.suppressed_circuit_impulses;
        }
        if (!impulse.receiver_fired) {
            continue;
        }
        ++result.firing_circuit_impulses;
        if (causally_reached.find(impulse.sender) == causally_reached.end()) {
            continue;
        }
        causally_reached.insert(impulse.receiver);
        if (result.completed_after_data && impulse.receiver == completion->neuron) {
            result.proven_circuit_path = true;
        }
    }
    return result;
}

LocalMemoryCausality Runtime::analyze_local_motor_memory(
    const brainstem::MotorResponse& response,
    const brainstem::MotorCircuitResult& circuit,
    const std::vector<ImpulseObservation>& impulses) const {
    LocalMemoryCausality result;
    if (circuit.config.mode != brainstem::MotorCircuitMode::activity_dependent ||
        circuit.relay_neurons.empty() || !motor_output_.configured()) {
        return result;
    }

    const NeuronId accumulator = circuit.relay_neurons.front();
    const NeuronId completion = motor_output_.assignment().completion_contact;
    std::uint64_t first_data_event = 0;
    std::uint64_t completion_event = 0;
    for (const brainstem::MotorActivation& activation : response.activations) {
        if (activation.channel >= 0 && first_data_event == 0) {
            first_data_event = activation.event_index;
        }
        if (activation.channel < 0) {
            completion_event = activation.event_index;
            break;
        }
    }
    if (first_data_event == 0 || completion_event == 0 ||
        completion_event <= first_data_event) {
        return result;
    }
    const std::set<SynapseId> circuit_ids(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    const std::int64_t motor_weight = std::max<std::int32_t>(1, circuit.config.motor_weight);
    result.required_accumulator_inputs = static_cast<std::uint32_t>(
        (config_.firing_threshold + motor_weight - 1) / motor_weight);

    const auto classify_snapshot = [&](const ImpulseObservation& observation,
                                       const NeuronId receiver) {
        std::uint32_t relevant = 0;
        for (const SynapseId id : observation.local_event_snapshot) {
            const neural::Synapse& item = network_.synapse(id);
            if (item.weight <= 0) {
                ++result.inactive_entries;
            }
            if (circuit_ids.find(id) != circuit_ids.end() &&
                item.receiver == receiver && item.weight > 0) {
                ++relevant;
            } else {
                ++result.unassigned_entries;
            }
        }
        return relevant;
    };

    for (const ImpulseObservation& observation : impulses) {
        if (!observation.receiver_fired ||
            observation.event_index < first_data_event ||
            observation.event_index > completion_event) {
            continue;
        }
        if (observation.receiver == accumulator) {
            result.accumulator_fired = true;
            result.retained_accumulator_inputs = std::max(
                result.retained_accumulator_inputs,
                classify_snapshot(observation, accumulator));
        } else if (observation.receiver == completion) {
            result.completion_fired = true;
            result.retained_completion_inputs = std::max(
                result.retained_completion_inputs,
                classify_snapshot(observation, completion));
        }
    }
    result.accumulator_reconstructable =
        result.retained_accumulator_inputs >= result.required_accumulator_inputs;
    result.completion_reconstructable = result.retained_completion_inputs >= 1;
    result.full_path_reconstructable = result.accumulator_reconstructable &&
                                       result.completion_reconstructable;
    return result;
}

void Runtime::set_experimental_synapse_weight(
    const SynapseId synapse, const std::int32_t weight) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot interrupt a synapse during an active response");
    }
    if (weight < 0 || weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("experimental weight is outside configured bounds");
    }
    network_.synapse(synapse).weight = weight;
}

void Runtime::set_experimental_synapse_effect(
    const SynapseId synapse, const SynapticEffect effect) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot change synaptic effect during an active response");
    }
    network_.synapse(synapse).effect = effect;
}

void Runtime::set_experimental_synapse_protection(
    const SynapseId synapse, const bool protected_state) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot change synapse protection during an active response");
    }
    network_.synapse(synapse).plasticity_protected = protected_state;
}

void Runtime::set_experimental_neuron_membrane(
    const NeuronId neuron, const std::int64_t membrane_potential) {
    if (motor_output_.active()) {
        throw std::logic_error(
            "cannot change membrane state during an active response");
    }
    if (membrane_potential < 0) {
        throw std::invalid_argument(
            "experimental membrane potential must be non-negative");
    }
    network_.neuron(neuron).membrane_potential = membrane_potential;
}

void Runtime::set_experimental_eligibility_records(
    const NeuronId neuron,
    const std::vector<neural::EligibilityRecord>& selected_records) {
    if (neuron >= network_.neuron_count()) {
        throw std::out_of_range("experimental eligibility neuron is out of range");
    }
    std::vector<std::vector<neural::EligibilityRecord>> records(
        network_.neuron_count());
    std::vector<std::uint64_t> generations(network_.neuron_count());
    std::vector<std::uint64_t> overwrites(network_.neuron_count());
    for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
        records[id] = id == neuron ? selected_records
                                   : network_.eligibility_records(id);
        generations[id] = network_.eligibility_generation(id);
        overwrites[id] = network_.eligibility_overwrites(id);
    }
    network_.restore_eligibility(
        network_.eligibility_candidate_capacity(),
        network_.eligibility_decision_capacity(), records, generations,
        overwrites);
}

void Runtime::set_experimental_plasticity_writes_suppressed(
    const bool suppressed) noexcept {
    config_.experimental_plasticity_writes_suppressed = suppressed;
}

ExperimentalTransientTransfer Runtime::set_experimental_transient_component_from(
    const Runtime& source, const ExperimentalTransientComponent component) {
    if (motor_output_.active() || source.motor_output_.active()) {
        throw std::logic_error(
            "cannot transplant transient state during an active response");
    }
    if (network_.neuron_count() != source.network_.neuron_count() ||
        network_.synapse_count() != source.network_.synapse_count() ||
        config_.local_event_memory_capacity !=
            source.config_.local_event_memory_capacity ||
        config_.eligibility_candidate_capacity !=
            source.config_.eligibility_candidate_capacity ||
        config_.eligibility_decision_capacity !=
            source.config_.eligibility_decision_capacity ||
        config_.context_trace_capacity != source.config_.context_trace_capacity) {
        throw std::invalid_argument(
            "transient state transfer requires matching runtime structure");
    }
    for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
        const auto& left = network_.neuron(id);
        const auto& right = source.network_.neuron(id);
        if (left.firing_threshold != right.firing_threshold ||
            left.first_incoming != right.first_incoming ||
            left.first_outgoing != right.first_outgoing ||
            left.incoming_count != right.incoming_count ||
            left.outgoing_count != right.outgoing_count) {
            throw std::invalid_argument(
                "transient state transfer requires matching neuron topology");
        }
    }
    for (SynapseId id = 0; id < network_.synapse_count(); ++id) {
        const auto& left = network_.synapse(id);
        const auto& right = source.network_.synapse(id);
        if (left.sender != right.sender || left.receiver != right.receiver ||
            left.next_incoming != right.next_incoming ||
            left.next_outgoing != right.next_outgoing ||
            left.effect != right.effect ||
            left.plasticity_protected != right.plasticity_protected) {
            throw std::invalid_argument(
                "transient state transfer requires matching synapse topology");
        }
    }

    ExperimentalTransientTransfer result;
    const auto replace_events = [&](const auto& selected) {
        std::vector<Event> merged;
        merged.reserve(queue_.size() + source.queue_.size());
        for (const auto& event : queue_) {
            if (!selected(event.kind)) merged.push_back(event);
        }
        for (const auto& event : source.queue_) {
            if (selected(event.kind)) merged.push_back(event);
        }
        std::sort(merged.begin(), merged.end(),
            [](const Event& left, const Event& right) {
                if (left.scheduled_time != right.scheduled_time) {
                    return left.scheduled_time < right.scheduled_time;
                }
                return left.sequence < right.sequence;
            });
        for (std::size_t index = 1; index < merged.size(); ++index) {
            if (merged[index - 1].sequence == merged[index].sequence) {
                throw std::logic_error(
                    "transient state transfer produced duplicate event sequence");
            }
        }
        queue_.assign(merged.begin(), merged.end());
        next_event_sequence_ = std::max(
            next_event_sequence_, source.next_event_sequence_);
        result.coupled_events = static_cast<std::size_t>(std::count_if(
            source.queue_.begin(), source.queue_.end(),
            [&](const Event& event) { return selected(event.kind); }));
        update_queue_watermark();
    };

    switch (component) {
    case ExperimentalTransientComponent::membrane:
        for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
            auto& target = network_.neuron(id).membrane_potential;
            const auto value = source.network_.neuron(id).membrane_potential;
            result.changed_values += target != value;
            target = value;
        }
        break;
    case ExperimentalTransientComponent::input_activity:
        for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
            auto& target = network_.neuron(id).input_event_count;
            const auto value = source.network_.neuron(id).input_event_count;
            result.changed_values += target != value;
            target = value;
        }
        break;
    case ExperimentalTransientComponent::local_event_memory: {
        std::vector<std::vector<SynapseId>> events(network_.neuron_count());
        std::vector<std::uint64_t> overwrites(network_.neuron_count());
        for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
            events[id] = source.network_.local_event_memory(id);
            overwrites[id] = source.network_.local_event_overwrites(id);
            result.changed_values += events[id] != network_.local_event_memory(id) ||
                overwrites[id] != network_.local_event_overwrites(id);
        }
        network_.restore_local_event_memory(
            source.network_.local_event_memory_capacity(), events, overwrites);
        break;
    }
    case ExperimentalTransientComponent::eligibility: {
        std::vector<std::vector<neural::EligibilityRecord>> records(
            network_.neuron_count());
        std::vector<std::uint64_t> generations(network_.neuron_count());
        std::vector<std::uint64_t> overwrites(network_.neuron_count());
        for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
            records[id] = source.network_.eligibility_records(id);
            generations[id] = source.network_.eligibility_generation(id);
            overwrites[id] = source.network_.eligibility_overwrites(id);
            result.changed_values += records[id] !=
                    network_.eligibility_records(id) ||
                generations[id] != network_.eligibility_generation(id) ||
                overwrites[id] != network_.eligibility_overwrites(id);
        }
        network_.restore_eligibility(
            source.network_.eligibility_candidate_capacity(),
            source.network_.eligibility_decision_capacity(), records,
            generations, overwrites);
        replace_events([](const EventKind kind) {
            return kind == EventKind::eligibility_expiry ||
                kind == EventKind::eligibility_decay;
        });
        break;
    }
    case ExperimentalTransientComponent::synaptic_activity:
        for (SynapseId id = 0; id < network_.synapse_count(); ++id) {
            auto& target = network_.synapse(id).activity_count;
            const auto value = source.network_.synapse(id).activity_count;
            result.changed_values += target != value;
            target = value;
        }
        break;
    case ExperimentalTransientComponent::recovery:
        for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
            auto& target = network_.neuron(id);
            const auto& value = source.network_.neuron(id);
            result.changed_values +=
                target.refractory_events_remaining !=
                    value.refractory_events_remaining ||
                target.recovery_generation != value.recovery_generation ||
                target.scheduled_recovery_time != value.scheduled_recovery_time ||
                target.subthreshold_relaxation_generation !=
                    value.subthreshold_relaxation_generation ||
                target.scheduled_subthreshold_relaxation_time !=
                    value.scheduled_subthreshold_relaxation_time;
            target.refractory_events_remaining =
                value.refractory_events_remaining;
            target.recovery_generation = value.recovery_generation;
            target.scheduled_recovery_time = value.scheduled_recovery_time;
            target.subthreshold_relaxation_generation =
                value.subthreshold_relaxation_generation;
            target.scheduled_subthreshold_relaxation_time =
                value.scheduled_subthreshold_relaxation_time;
        }
        replace_events([](const EventKind kind) {
            return kind == EventKind::recovery ||
                kind == EventKind::subthreshold_relaxation;
        });
        break;
    case ExperimentalTransientComponent::time_and_queue:
        throw std::logic_error(
            "simulated time and the complete queue require a full-state copy");
    case ExperimentalTransientComponent::context: {
        std::vector<std::vector<neural::ContextTrace>> traces(
            network_.neuron_count());
        std::vector<std::uint64_t> generations(network_.neuron_count());
        std::vector<std::uint64_t> overwrites(network_.neuron_count());
        for (NeuronId id = 0; id < network_.neuron_count(); ++id) {
            traces[id] = source.network_.context_traces(id);
            generations[id] = source.network_.context_generation(id);
            overwrites[id] = source.network_.context_overwrites(id);
            result.changed_values += traces[id] != network_.context_traces(id) ||
                generations[id] != network_.context_generation(id) ||
                overwrites[id] != network_.context_overwrites(id);
        }
        network_.restore_context_traces(
            source.network_.context_trace_capacity(), traces,
            generations, overwrites);
        replace_events([](const EventKind kind) {
            return kind == EventKind::context_expiry;
        });
        break;
    }
    case ExperimentalTransientComponent::random_state:
        result.changed_values += rng_.state() != source.rng_.state();
        result.changed_values += growth_rng_.state() != source.growth_rng_.state();
        rng_ = source.rng_;
        growth_rng_ = source.growth_rng_;
        break;
    case ExperimentalTransientComponent::diagnostic_counters:
        result.changed_values += !(metrics_ == source.metrics_);
        metrics_ = source.metrics_;
        break;
    }
    result.valid = network_.validate();
    if (!result.valid) {
        throw std::logic_error("transient state transfer produced invalid network");
    }
    return result;
}

void Runtime::set_structural_growth_enabled(const bool enabled) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot change structural growth during an active response");
    }
    config_.growth_enabled = enabled;
}

SynapseId Runtime::add_experimental_synapse(
    const NeuronId sender, const NeuronId receiver, const std::int32_t weight,
    const bool plasticity_protected, const SynapticEffect effect) {
    if (weight < 0 || weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("experimental weight is outside configured bounds");
    }
    if (network_.has_synapse(sender, receiver)) {
        throw std::invalid_argument("experimental synapse already exists");
    }
    const SynapseId synapse = network_.add_synapse(sender, receiver, weight, effect);
    network_.synapse(synapse).plasticity_protected = plasticity_protected;
    return synapse;
}

const ExperimentConfig& Runtime::config() const noexcept {
    return config_;
}

const neural::Network& Runtime::network() const noexcept {
    return network_;
}

const Metrics& Runtime::metrics() const noexcept {
    return metrics_;
}

Diagnostics Runtime::diagnostics() const {
    Diagnostics result;
    result.neuron_count = network_.neuron_count();
    result.synapse_count = network_.synapse_count();
    result.estimated_memory_bytes = sizeof(*this) + network_.estimated_memory_bytes() -
                                    sizeof(neural::Network) +
                                    queue_.size() * sizeof(Event) +
                                    motor_output_.estimated_memory_bytes();
    result.queued_events = queue_.size();
    result.ready_events = static_cast<std::size_t>(std::count_if(
        queue_.begin(), queue_.end(),
        [this](const Event& event) { return event.scheduled_time <= simulated_time_; }));
    result.future_events = result.queued_events - result.ready_events;
    result.simulated_time = simulated_time_;
    result.weight_distribution = network_.weight_distribution();
    result.metrics = metrics_;
    result.state_fingerprint = state_fingerprint();
    return result;
}

std::uint64_t Runtime::state_fingerprint() const noexcept {
    std::uint64_t hash = network_.state_fingerprint();
    mix_hash(hash, rng_.state());
    if (config_.growth_enabled) mix_hash(hash, growth_rng_.state());
    mix_hash(hash, config_.neuron_count);
    mix_hash(hash, config_.width);
    mix_hash(hash, config_.height);
    mix_hash(hash, config_.depth);
    mix_hash(hash, config_.initial_outgoing_synapses);
    mix_hash(hash, static_cast<std::uint64_t>(config_.firing_threshold));
    mix_hash(hash, config_.synaptic_learning_threshold);
    mix_hash(hash, config_.activity_phase_limit);
    mix_hash(hash, static_cast<std::uint32_t>(config_.initial_synapse_weight));
    mix_hash(hash, static_cast<std::uint32_t>(config_.maximum_synapse_weight));
    mix_hash(hash, config_.input_weight_budget);
    mix_hash(hash, config_.refractory_input_events);
    mix_hash(hash, static_cast<std::uint8_t>(config_.recovery_mode));
    mix_hash(hash, config_.recovery_delay);
    mix_hash(hash, config_.membrane_relaxation_percent);
    mix_hash(hash, config_.local_event_memory_capacity);
    mix_hash(hash, static_cast<std::uint8_t>(config_.eligibility_selection));
    mix_hash(hash, static_cast<std::uint8_t>(config_.eligibility_representation));
    mix_hash(hash, static_cast<std::uint8_t>(config_.eligibility_history));
    mix_hash(hash, config_.eligibility_candidate_capacity);
    mix_hash(hash, config_.eligibility_decision_capacity);
    mix_hash(hash, config_.eligibility_duration);
    mix_hash(hash, static_cast<std::uint8_t>(config_.context_trace_mode));
    mix_hash(hash, static_cast<std::uint8_t>(config_.context_binding_mode));
    mix_hash(hash, config_.context_trace_capacity);
    mix_hash(hash, config_.context_trace_duration);
    mix_hash(hash, static_cast<std::uint8_t>(config_.plasticity_trace_mode));
    mix_hash(hash, static_cast<std::uint8_t>(config_.plasticity_decay_mode));
    mix_hash(hash, static_cast<std::uint8_t>(config_.modulation_mode));
    mix_hash(hash, static_cast<std::uint32_t>(config_.plasticity_positive_step));
    mix_hash(hash, static_cast<std::uint32_t>(config_.plasticity_negative_step));
    mix_hash(hash, static_cast<std::uint32_t>(config_.plasticity_maximum_weight));
    mix_hash(hash, config_.consume_trace_on_modulation ? 1 : 0);
    mix_hash(hash, config_.batch_simultaneous_inputs ? 1 : 0);
    mix_hash(hash, static_cast<std::uint8_t>(config_.inhibitory_plasticity_mode));
    if (config_.subthreshold_relaxation_enabled) {
        mix_hash(hash, 0xB550ULL);
        mix_hash(hash, config_.subthreshold_relaxation_delay);
    }
    if (config_.experimental_plasticity_writes_suppressed) {
        mix_hash(hash, 0xB580ULL);
    }
    mix_hash(hash, evaluation_route_configured_ ? 1 : 0);
    if (evaluation_route_configured_) {
        mix_hash(hash, static_cast<std::uint8_t>(evaluation_route_.config.mode));
        mix_hash(hash, evaluation_route_.config.first_input_neuron);
        mix_hash(hash, evaluation_route_.config.graded_channel_count);
        mix_hash(hash, evaluation_route_.config.relay_count);
        mix_hash(hash, evaluation_route_.config.fanout_per_source);
        mix_hash(hash, static_cast<std::uint32_t>(evaluation_route_.config.weight));
        mix_hash(hash, evaluation_route_.config.seed);
        mix_hash(hash, evaluation_route_.config.reserved_neurons.size());
        for (const auto value : evaluation_route_.config.reserved_neurons) mix_hash(hash, value);
        mix_hash(hash, evaluation_route_.input_neurons.size());
        for (const auto value : evaluation_route_.input_neurons) mix_hash(hash, value);
        mix_hash(hash, evaluation_route_.relay_neurons.size());
        for (const auto value : evaluation_route_.relay_neurons) mix_hash(hash, value);
        mix_hash(hash, evaluation_route_.target_neurons.size());
        for (const auto value : evaluation_route_.target_neurons) mix_hash(hash, value);
        mix_hash(hash, evaluation_route_.route_synapses.size());
        for (const auto value : evaluation_route_.route_synapses) mix_hash(hash, value);
        mix_hash(hash, evaluation_route_.added_synapses);
        mix_hash(hash, evaluation_route_.adjusted_synapses);
    }
    mix_hash(hash, config_.maximum_events_per_run);
    mix_hash(hash, config_.learning_enabled ? 1 : 0);
    mix_hash(hash, config_.growth_enabled ? 1 : 0);
    mix_hash(hash, static_cast<std::uint8_t>(config_.reset_mode));
    mix_hash(hash, metrics_.processed_events);
    mix_hash(hash, metrics_.transmitted_impulses);
    mix_hash(hash, metrics_.fire_events);
    mix_hash(hash, metrics_.completed_activity_phases);
    mix_hash(hash, metrics_.synaptic_strengthenings);
    mix_hash(hash, metrics_.rejected_strengthenings_at_weight_limit);
    mix_hash(hash, metrics_.synaptic_weakenings);
    mix_hash(hash, metrics_.refractory_suppressed_firings);
    mix_hash(hash, metrics_.local_event_memory_records);
    mix_hash(hash, metrics_.local_event_memory_overwrites);
    mix_hash(hash, metrics_.scheduled_recovery_events);
    mix_hash(hash, metrics_.processed_recovery_events);
    mix_hash(hash, metrics_.stale_recovery_events);
    mix_hash(hash, metrics_.processed_boundary_events);
    mix_hash(hash, metrics_.eligibility_decisions);
    mix_hash(hash, metrics_.eligibility_candidates);
    mix_hash(hash, metrics_.eligibility_positive_candidates);
    mix_hash(hash, metrics_.eligibility_zero_weight_candidates);
    mix_hash(hash, metrics_.eligibility_overwritten_decisions);
    mix_hash(hash, metrics_.scheduled_eligibility_expiries);
    mix_hash(hash, metrics_.processed_eligibility_expiries);
    mix_hash(hash, metrics_.stale_eligibility_expiries);
    mix_hash(hash, metrics_.feedback_fire_events);
    mix_hash(hash, metrics_.feedback_impulses);
    mix_hash(hash, metrics_.feedback_receipts_with_eligibility);
    mix_hash(hash, metrics_.feedback_receipts_without_eligibility);
    mix_hash(hash, metrics_.feedback_receipts_with_multiple_eligibilities);
    mix_hash(hash, metrics_.feedback_created_eligibilities);
    mix_hash(hash, metrics_.context_traces_created);
    mix_hash(hash, metrics_.context_traces_overwritten);
    mix_hash(hash, metrics_.processed_context_expiries);
    mix_hash(hash, metrics_.stale_context_expiries);
    mix_hash(hash, metrics_.eligibility_context_bindings);
    mix_hash(hash, metrics_.eligibility_context_ambiguities);
    mix_hash(hash, metrics_.feedback_context_unique_matches);
    mix_hash(hash, metrics_.feedback_context_ambiguous_matches);
    mix_hash(hash, metrics_.feedback_context_missing_matches);
    mix_hash(hash, metrics_.feedback_context_expired_matches);
    mix_hash(hash, metrics_.feedback_origin_context_matches);
    mix_hash(hash, metrics_.modulation_fire_events);
    mix_hash(hash, metrics_.modulation_impulses);
    mix_hash(hash, metrics_.modulation_receipts_with_trace);
    mix_hash(hash, metrics_.modulation_receipts_without_trace);
    mix_hash(hash, metrics_.plasticity_positive_changes);
    mix_hash(hash, metrics_.plasticity_negative_changes);
    mix_hash(hash, metrics_.plasticity_zero_to_one_changes);
    mix_hash(hash, metrics_.plasticity_rejected_at_weight_limit);
    mix_hash(hash, metrics_.plasticity_rejected_by_budget);
    mix_hash(hash, metrics_.plasticity_consumed_traces);
    mix_hash(hash, metrics_.plasticity_feedback_origin_traces_skipped);
    mix_hash(hash, metrics_.growth_attempts);
    mix_hash(hash, metrics_.grown_synapses);
    mix_hash(hash, metrics_.processed_sensor_bytes);
    mix_hash(hash, metrics_.maximum_queue_size);
    mix_hash(hash, metrics_.event_trace_hash);
    mix_hash(hash, metrics_.inhibitory_impulses);
    mix_hash(hash, metrics_.inhibitory_applied_strength);
    mix_hash(hash, metrics_.simultaneous_input_batches);
    mix_hash(hash, metrics_.inhibitory_positive_changes);
    mix_hash(hash, metrics_.inhibitory_negative_changes);
    if (metrics_.scheduled_subthreshold_relaxations != 0 ||
        metrics_.processed_subthreshold_relaxations != 0 ||
        metrics_.stale_subthreshold_relaxations != 0) {
        mix_hash(hash, metrics_.scheduled_subthreshold_relaxations);
        mix_hash(hash, metrics_.processed_subthreshold_relaxations);
        mix_hash(hash, metrics_.stale_subthreshold_relaxations);
    }
    if (metrics_.plasticity_suppressed_writes != 0) {
        mix_hash(hash, metrics_.plasticity_suppressed_writes);
    }
    mix_hash(hash, simulated_time_);
    mix_hash(hash, next_event_sequence_);
    for (const Event& event : queue_) {
        mix_hash(hash, static_cast<std::uint8_t>(event.kind));
        mix_hash(hash, event.target);
        mix_hash(hash, event.scheduled_time);
        mix_hash(hash, event.sequence);
        mix_hash(hash, event.generation);
        mix_hash(hash, event.context_signature);
        mix_hash(hash, event.context_feedback_origin ? 1 : 0);
        mix_hash(hash, static_cast<std::uint8_t>(event.modulation));
    }
    return hash;
}

std::uint64_t Runtime::simulated_time() const noexcept {
    return simulated_time_;
}

std::size_t Runtime::future_event_count() const noexcept {
    return static_cast<std::size_t>(std::count_if(
        queue_.begin(), queue_.end(),
        [this](const Event& event) { return event.scheduled_time > simulated_time_; }));
}

void Runtime::save(const std::string& path) const {
    if (motor_output_.active()) {
        throw std::logic_error("saving during an active motor response is not supported");
    }
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream) {
        throw std::runtime_error("cannot open state for writing: " + path);
    }

    write_bytes(stream, kPersistenceMagic.data(), kPersistenceMagic.size());
    write_u32(stream, kPersistenceVersion);
    write_config(stream, config_);
    write_u64(stream, rng_.state());
    write_u64(stream, growth_rng_.state());
    write_metrics(stream, metrics_);

    write_u32(stream, network_.width());
    write_u32(stream, network_.height());
    write_u32(stream, network_.depth());
    write_u32(stream, network_.neuron_count());
    for (const neural::Neuron& neuron : network_.neurons()) {
        write_i64(stream, neuron.membrane_potential);
        write_i64(stream, neuron.firing_threshold);
        write_u32(stream, neuron.input_event_count);
        write_u32(stream, neuron.refractory_events_remaining);
        write_u64(stream, neuron.recovery_generation);
        write_u64(stream, neuron.scheduled_recovery_time);
        write_u64(stream, neuron.subthreshold_relaxation_generation);
        write_u64(stream, neuron.scheduled_subthreshold_relaxation_time);
        write_u32(stream, neuron.first_incoming);
        write_u32(stream, neuron.first_outgoing);
        write_u32(stream, neuron.incoming_count);
        write_u32(stream, neuron.outgoing_count);
    }

    write_u32(stream, network_.synapse_count());
    for (const neural::Synapse& synapse : network_.synapses()) {
        write_u32(stream, synapse.sender);
        write_u32(stream, synapse.receiver);
        write_i32(stream, synapse.weight);
        write_u32(stream, synapse.activity_count);
        write_u32(stream, synapse.next_incoming);
        write_u32(stream, synapse.next_outgoing);
        write_u8(stream, synapse.plasticity_protected ? 1 : 0);
        write_u8(stream, static_cast<std::uint8_t>(synapse.effect));
    }

    for (NeuronId neuron = 0; neuron < network_.neuron_count(); ++neuron) {
        const auto events = network_.local_event_memory(neuron);
        write_u32(stream, static_cast<std::uint32_t>(events.size()));
        for (const SynapseId synapse : events) {
            write_u32(stream, synapse);
        }
        write_u64(stream, network_.local_event_overwrites(neuron));
    }

    for (NeuronId neuron = 0; neuron < network_.neuron_count(); ++neuron) {
        write_u64(stream, network_.eligibility_generation(neuron));
        write_u64(stream, network_.eligibility_overwrites(neuron));
        const auto records = network_.eligibility_records(neuron);
        write_u32(stream, static_cast<std::uint32_t>(records.size()));
        for (const auto& record : records) {
            write_u64(stream, record.generation);
            write_u64(stream, record.expires_at);
            write_u8(stream, record.feedback_origin ? 1 : 0);
            write_u32(stream, record.context_signature);
            write_u64(stream, record.context_generation);
            write_u8(stream, record.context_ambiguous ? 1 : 0);
            write_u8(stream, record.context_feedback_origin ? 1 : 0);
            write_u8(stream, record.strength);
            write_u32(stream, static_cast<std::uint32_t>(record.candidates.size()));
            for (const SynapseId candidate : record.candidates) {
                write_u32(stream, candidate);
            }
        }
    }

    for (NeuronId neuron = 0; neuron < network_.neuron_count(); ++neuron) {
        write_u64(stream, network_.context_generation(neuron));
        write_u64(stream, network_.context_overwrites(neuron));
        const auto traces = network_.context_traces(neuron);
        write_u32(stream, static_cast<std::uint32_t>(traces.size()));
        for (const auto& trace : traces) {
            write_u64(stream, trace.generation);
            write_u32(stream, trace.signature);
            write_u64(stream, trace.expires_at);
            write_u8(stream, trace.feedback_origin ? 1 : 0);
        }
    }

    write_u64(stream, simulated_time_);
    write_u64(stream, next_event_sequence_);
    write_u64(stream, queue_.size());
    for (const Event& event : queue_) {
        write_u8(stream, static_cast<std::uint8_t>(event.kind));
        write_u32(stream, event.target);
        write_u64(stream, event.scheduled_time);
        write_u64(stream, event.sequence);
        write_u64(stream, event.generation);
        write_u32(stream, event.context_signature);
        write_u8(stream, event.context_feedback_origin ? 1 : 0);
        write_u8(stream, static_cast<std::uint8_t>(event.modulation));
    }
    write_u8(stream, evaluation_route_configured_ ? 1 : 0);
    if (evaluation_route_configured_) {
        const auto& route_config = evaluation_route_.config;
        write_u8(stream, static_cast<std::uint8_t>(route_config.mode));
        write_u32(stream, route_config.first_input_neuron);
        write_u32(stream, route_config.graded_channel_count);
        write_u32(stream, route_config.relay_count);
        write_u32(stream, route_config.fanout_per_source);
        write_i32(stream, route_config.weight);
        write_u64(stream, route_config.seed);
        const auto write_ids = [&stream](const auto& values) {
            write_u32(stream, static_cast<std::uint32_t>(values.size()));
            for (const auto value : values) write_u32(stream, value);
        };
        write_ids(route_config.reserved_neurons);
        write_ids(evaluation_route_.input_neurons);
        write_ids(evaluation_route_.relay_neurons);
        write_ids(evaluation_route_.target_neurons);
        write_ids(evaluation_route_.route_synapses);
        write_u32(stream, evaluation_route_.added_synapses);
        write_u32(stream, evaluation_route_.adjusted_synapses);
    }
    stream.flush();
    if (!stream) {
        throw std::runtime_error("failed to finish writing EVE state: " + path);
    }
}

Runtime Runtime::load(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("cannot open state for reading: " + path);
    }

    std::array<char, kPersistenceMagic.size()> magic{};
    read_bytes(stream, magic.data(), magic.size());
    if (magic != kPersistenceMagic) {
        throw std::runtime_error("not an EVE Experiment 001 state file");
    }
    const std::uint32_t version = read_u32(stream);
    if (version == 0 || version > kPersistenceVersion) {
        throw std::runtime_error("unsupported EVE state version");
    }

    ExperimentConfig config = read_config(stream, version);
    Runtime runtime(config, 1, SkipInitializationTag{});
    runtime.rng_.set_state(read_u64(stream));
    if (version >= 12) {
        runtime.growth_rng_.set_state(read_u64(stream));
    } else {
        const std::uint64_t growth_state = runtime.rng_.state() ^ kGrowthSeedMask;
        runtime.growth_rng_.set_state(
            growth_state == 0 ? kNonZeroRngState : growth_state);
    }
    runtime.metrics_ = read_metrics(stream, version);

    const std::uint32_t width = read_u32(stream);
    const std::uint32_t height = read_u32(stream);
    const std::uint32_t depth = read_u32(stream);
    const std::uint32_t neuron_count = read_u32(stream);
    if (width != config.width || height != config.height || depth != config.depth ||
        neuron_count != config.neuron_count) {
        throw std::runtime_error("persisted network geometry differs from its configuration");
    }

    std::vector<neural::Neuron> neurons;
    neurons.reserve(neuron_count);
    for (std::uint32_t index = 0; index < neuron_count; ++index) {
        neural::Neuron neuron;
        neuron.membrane_potential = read_i64(stream);
        neuron.firing_threshold = read_i64(stream);
        neuron.input_event_count = read_u32(stream);
        if (version >= 2) {
            neuron.refractory_events_remaining = read_u32(stream);
        }
        if (version >= 5) {
            neuron.recovery_generation = read_u64(stream);
            neuron.scheduled_recovery_time = read_u64(stream);
        }
        if (version >= 14) {
            neuron.subthreshold_relaxation_generation = read_u64(stream);
            neuron.scheduled_subthreshold_relaxation_time = read_u64(stream);
        }
        neuron.first_incoming = read_u32(stream);
        neuron.first_outgoing = read_u32(stream);
        neuron.incoming_count = read_u32(stream);
        neuron.outgoing_count = read_u32(stream);
        neurons.push_back(neuron);
    }

    const std::uint32_t synapse_count = read_u32(stream);
    std::vector<neural::Synapse> synapses;
    synapses.reserve(synapse_count);
    for (std::uint32_t index = 0; index < synapse_count; ++index) {
        neural::Synapse synapse;
        synapse.sender = read_u32(stream);
        synapse.receiver = read_u32(stream);
        synapse.weight = read_i32(stream);
        synapse.activity_count = read_u32(stream);
        synapse.next_incoming = read_u32(stream);
        synapse.next_outgoing = read_u32(stream);
        if (version >= 11) {
            synapse.plasticity_protected = read_bool(stream);
        }
        if (version >= 13) {
            const auto effect = read_u8(stream);
            if (effect > static_cast<std::uint8_t>(SynapticEffect::inhibitory)) {
                throw std::runtime_error("invalid persisted synaptic effect");
            }
            synapse.effect = static_cast<SynapticEffect>(effect);
        }
        synapses.push_back(synapse);
    }
    runtime.network_.restore(
        width, height, depth, std::move(neurons), std::move(synapses));
    for (const neural::Synapse& synapse : runtime.network_.synapses()) {
        if (synapse.weight < 0 || synapse.weight > config.maximum_synapse_weight) {
            throw std::runtime_error("persisted synapse weight is outside configured bounds");
        }
    }

    if (version >= 4) {
        std::vector<std::vector<SynapseId>> memories(neuron_count);
        std::vector<std::uint64_t> overwrites(neuron_count);
        for (NeuronId neuron = 0; neuron < neuron_count; ++neuron) {
            const std::uint32_t size = read_u32(stream);
            if (size > config.local_event_memory_capacity) {
                throw std::runtime_error("persisted local event memory exceeds capacity");
            }
            memories[neuron].reserve(size);
            for (std::uint32_t index = 0; index < size; ++index) {
                memories[neuron].push_back(read_u32(stream));
            }
            overwrites[neuron] = read_u64(stream);
        }
        runtime.network_.restore_local_event_memory(
            config.local_event_memory_capacity, memories, overwrites);
    } else {
        runtime.network_.restore_local_event_memory(
            0, std::vector<std::vector<SynapseId>>(neuron_count),
            std::vector<std::uint64_t>(neuron_count));
    }

    if (version >= 6) {
        std::vector<std::vector<neural::EligibilityRecord>> records(neuron_count);
        std::vector<std::uint64_t> generations(neuron_count);
        std::vector<std::uint64_t> overwrites(neuron_count);
        for (NeuronId neuron = 0; neuron < neuron_count; ++neuron) {
            generations[neuron] = read_u64(stream);
            overwrites[neuron] = read_u64(stream);
            const std::uint32_t record_count = read_u32(stream);
            if (record_count > config.eligibility_decision_capacity) {
                throw std::runtime_error("persisted eligibility exceeds decision capacity");
            }
            records[neuron].reserve(record_count);
            for (std::uint32_t record_index = 0; record_index < record_count;
                 ++record_index) {
                neural::EligibilityRecord record;
                record.generation = read_u64(stream);
                record.expires_at = read_u64(stream);
                if (version >= 7) {
                    record.feedback_origin = read_bool(stream);
                }
                if (version >= 8) {
                    record.context_signature = read_u32(stream);
                    record.context_generation = read_u64(stream);
                    record.context_ambiguous = read_bool(stream);
                    record.context_feedback_origin = read_bool(stream);
                }
                if (version >= 9) {
                    record.strength = read_u8(stream);
                    if (record.strength == 0) {
                        throw std::runtime_error("persisted eligibility has zero strength");
                    }
                }
                const std::uint32_t candidate_count = read_u32(stream);
                if (candidate_count > config.eligibility_candidate_capacity) {
                    throw std::runtime_error(
                        "persisted eligibility exceeds candidate capacity");
                }
                record.candidates.reserve(candidate_count);
                for (std::uint32_t index = 0; index < candidate_count; ++index) {
                    record.candidates.push_back(read_u32(stream));
                }
                records[neuron].push_back(std::move(record));
            }
        }
        runtime.network_.restore_eligibility(
            config.eligibility_candidate_capacity,
            config.eligibility_decision_capacity,
            records, generations, overwrites);
    } else {
        runtime.network_.restore_eligibility(
            0, 0,
            std::vector<std::vector<neural::EligibilityRecord>>(neuron_count),
            std::vector<std::uint64_t>(neuron_count),
            std::vector<std::uint64_t>(neuron_count));
    }

    if (version >= 8) {
        std::vector<std::vector<neural::ContextTrace>> traces(neuron_count);
        std::vector<std::uint64_t> generations(neuron_count);
        std::vector<std::uint64_t> overwrites(neuron_count);
        for (NeuronId neuron = 0; neuron < neuron_count; ++neuron) {
            generations[neuron] = read_u64(stream);
            overwrites[neuron] = read_u64(stream);
            const std::uint32_t trace_count = read_u32(stream);
            if (trace_count > config.context_trace_capacity) {
                throw std::runtime_error("persisted context traces exceed capacity");
            }
            traces[neuron].reserve(trace_count);
            for (std::uint32_t index = 0; index < trace_count; ++index) {
                neural::ContextTrace trace;
                trace.generation = read_u64(stream);
                trace.signature = read_u32(stream);
                trace.expires_at = read_u64(stream);
                trace.feedback_origin = read_bool(stream);
                traces[neuron].push_back(trace);
            }
        }
        runtime.network_.restore_context_traces(
            config.context_trace_capacity, traces, generations, overwrites);
    } else {
        runtime.network_.restore_context_traces(
            0, std::vector<std::vector<neural::ContextTrace>>(neuron_count),
            std::vector<std::uint64_t>(neuron_count),
            std::vector<std::uint64_t>(neuron_count));
    }

    if (version >= 5) {
        runtime.simulated_time_ = read_u64(stream);
        runtime.next_event_sequence_ = read_u64(stream);
    }
    const std::uint64_t queue_size = read_u64(stream);
    if (queue_size > config.maximum_events_per_run) {
        throw std::runtime_error("persisted event queue exceeds the configured event limit");
    }
    for (std::uint64_t index = 0; index < queue_size; ++index) {
        const std::uint8_t kind_value = read_u8(stream);
        const std::uint8_t maximum_kind = version >= 14
            ? static_cast<std::uint8_t>(EventKind::subthreshold_relaxation)
            : version >= 9
            ? static_cast<std::uint8_t>(EventKind::eligibility_decay)
            : version >= 8
            ? static_cast<std::uint8_t>(EventKind::context_expiry)
            : version >= 7
            ? static_cast<std::uint8_t>(EventKind::feedback_impulse)
            : version >= 6 ? static_cast<std::uint8_t>(EventKind::eligibility_expiry)
            : version >= 5 ? static_cast<std::uint8_t>(EventKind::boundary)
            : static_cast<std::uint8_t>(EventKind::impulse);
        if (kind_value > maximum_kind) {
            throw std::runtime_error("invalid event kind in EVE state");
        }
        Event event;
        event.kind = static_cast<EventKind>(kind_value);
        event.target = read_u32(stream);
        if (version >= 5) {
            event.scheduled_time = read_u64(stream);
            event.sequence = read_u64(stream);
            event.generation = read_u64(stream);
            if (version >= 8) {
                event.context_signature = read_u32(stream);
                event.context_feedback_origin = read_bool(stream);
            }
            if (version >= 9) {
                event.modulation = static_cast<std::int8_t>(read_u8(stream));
                if (event.modulation != -1 && event.modulation != 0 &&
                    event.modulation != 1 && event.modulation != 2) {
                    throw std::runtime_error("invalid modulation in EVE state");
                }
            }
        } else {
            event.sequence = runtime.next_event_sequence_++;
        }
        if (event.kind == EventKind::fire || event.kind == EventKind::feedback_fire ||
            event.kind == EventKind::modulation_fire) {
            static_cast<void>(runtime.network_.neuron(event.target));
        } else if (event.kind == EventKind::impulse ||
                   event.kind == EventKind::feedback_impulse ||
                   event.kind == EventKind::modulation_impulse) {
            static_cast<void>(runtime.network_.synapse(event.target));
        } else if (event.kind == EventKind::recovery ||
                   event.kind == EventKind::subthreshold_relaxation ||
                   event.kind == EventKind::eligibility_expiry ||
                   event.kind == EventKind::eligibility_decay ||
                   event.kind == EventKind::context_expiry) {
            static_cast<void>(runtime.network_.neuron(event.target));
        }
        runtime.queue_.push_back(event);
    }
    runtime.update_queue_watermark();

    if (version >= 10) {
        runtime.evaluation_route_configured_ = read_bool(stream);
        if (runtime.evaluation_route_configured_) {
            auto& route = runtime.evaluation_route_;
            const std::uint8_t mode = read_u8(stream);
            if (mode > static_cast<std::uint8_t>(
                           brainstem::EvaluationRouteMode::relay_wiring)) {
                throw std::runtime_error("invalid evaluation route mode in EVE state");
            }
            route.config.mode = static_cast<brainstem::EvaluationRouteMode>(mode);
            route.config.first_input_neuron = read_u32(stream);
            route.config.graded_channel_count = read_u32(stream);
            route.config.relay_count = read_u32(stream);
            route.config.fanout_per_source = read_u32(stream);
            route.config.weight = read_i32(stream);
            route.config.seed = read_u64(stream);
            if (route.config.weight < 0 ||
                route.config.weight > config.maximum_synapse_weight) {
                throw std::runtime_error(
                    "persisted evaluation route weight is outside configured bounds");
            }
            const auto read_ids = [&stream](auto& values, const std::uint32_t maximum,
                                            const char* label) {
                const std::uint32_t count = read_u32(stream);
                if (count > maximum) {
                    throw std::runtime_error(std::string("persisted ") + label +
                                             " exceeds network bounds");
                }
                values.reserve(count);
                for (std::uint32_t index = 0; index < count; ++index) {
                    const std::uint32_t value = read_u32(stream);
                    if (value >= maximum) {
                        throw std::runtime_error(std::string("persisted ") + label +
                                                 " contains an invalid identifier");
                    }
                    values.push_back(value);
                }
            };
            read_ids(route.config.reserved_neurons, neuron_count,
                     "evaluation route reservation");
            read_ids(route.input_neurons, neuron_count,
                     "evaluation route input list");
            read_ids(route.relay_neurons, neuron_count,
                     "evaluation route relay list");
            read_ids(route.target_neurons, neuron_count,
                     "evaluation route target list");
            read_ids(route.route_synapses, synapse_count,
                     "evaluation route synapse list");
            route.added_synapses = read_u32(stream);
            route.adjusted_synapses = read_u32(stream);
        }
    }

    if (stream.peek() != std::char_traits<char>::eof()) {
        throw std::runtime_error("unexpected trailing data in EVE state");
    }
    return runtime;
}

void Runtime::validate_config() const {
    if (config_.neuron_count == 0 || config_.width == 0 || config_.height == 0 ||
        config_.depth == 0) {
        throw std::invalid_argument("network dimensions and neuron count must be non-zero");
    }
    const std::uint64_t volume = static_cast<std::uint64_t>(config_.width) *
                                 config_.height * config_.depth;
    if (volume != config_.neuron_count) {
        throw std::invalid_argument("geometry volume must equal neuron count");
    }
    if (config_.initial_outgoing_synapses >= config_.neuron_count) {
        throw std::invalid_argument(
            "initial outgoing count must leave at least one non-self target");
    }
    if (config_.firing_threshold <= 0 || config_.synaptic_learning_threshold == 0 ||
        config_.activity_phase_limit == 0 || config_.maximum_events_per_run == 0) {
        throw std::invalid_argument("thresholds, phase limit and event limit must be positive");
    }
    if (config_.initial_synapse_weight < 0 || config_.maximum_synapse_weight < 0 ||
        config_.initial_synapse_weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("Experiment 001 weights must be non-negative and bounded");
    }
    if (config_.input_weight_budget == 0) {
        throw std::invalid_argument("input weight budget must be positive or unlimited");
    }
    if (config_.recovery_mode != RecoveryMode::input_dependent &&
        config_.recovery_delay == 0) {
        throw std::invalid_argument("independent recovery delay must be positive");
    }
    if (config_.membrane_relaxation_percent > 100) {
        throw std::invalid_argument("membrane relaxation must be between 0 and 100 percent");
    }
    if (config_.subthreshold_relaxation_enabled &&
        config_.subthreshold_relaxation_delay == 0) {
        throw std::invalid_argument(
            "subthreshold relaxation delay must be positive when enabled");
    }
    if (!config_.subthreshold_relaxation_enabled &&
        config_.subthreshold_relaxation_delay != 0) {
        throw std::invalid_argument(
            "disabled subthreshold relaxation must not configure a delay");
    }
    if (static_cast<std::uint8_t>(config_.eligibility_selection) >
            static_cast<std::uint8_t>(EligibilitySelection::active_phase) ||
        static_cast<std::uint8_t>(config_.eligibility_representation) >
            static_cast<std::uint8_t>(EligibilityRepresentation::unique) ||
        static_cast<std::uint8_t>(config_.eligibility_history) >
            static_cast<std::uint8_t>(EligibilityHistory::merge_unique)) {
        throw std::invalid_argument("invalid eligibility mode");
    }
    if (static_cast<std::uint8_t>(config_.inhibitory_plasticity_mode) >
        static_cast<std::uint8_t>(
            InhibitoryPlasticityMode::contribution_aware)) {
        throw std::invalid_argument("invalid inhibitory plasticity mode");
    }
    if (config_.recovery_mode !=
            RecoveryMode::independent_with_membrane_relaxation &&
        config_.membrane_relaxation_percent != 0) {
        throw std::invalid_argument("membrane relaxation requires recovery mode R2");
    }
    const bool eligibility_enabled =
        config_.eligibility_selection != EligibilitySelection::none;
    if (eligibility_enabled &&
        (config_.local_event_memory_capacity == 0 ||
         config_.eligibility_candidate_capacity == 0 ||
         config_.eligibility_decision_capacity == 0)) {
        throw std::invalid_argument(
            "eligibility requires local memory and positive candidate/decision capacities");
    }
    if (!eligibility_enabled &&
        (config_.eligibility_candidate_capacity != 0 ||
         config_.eligibility_decision_capacity != 0 ||
         config_.eligibility_duration != 0)) {
        throw std::invalid_argument(
            "disabled eligibility must not allocate candidate or decision state");
    }
    if (static_cast<std::uint8_t>(config_.context_trace_mode) >
            static_cast<std::uint8_t>(ContextTraceMode::multiple) ||
        static_cast<std::uint8_t>(config_.context_binding_mode) >
            static_cast<std::uint8_t>(ContextBindingMode::temporal_overlap)) {
        throw std::invalid_argument("invalid action context mode");
    }
    const bool context_enabled =
        config_.context_trace_mode != ContextTraceMode::none;
    if (context_enabled &&
        (config_.context_trace_capacity == 0 ||
         config_.context_trace_duration == 0)) {
        throw std::invalid_argument(
            "action context traces require positive capacity and duration");
    }
    if (!context_enabled &&
        (config_.context_trace_capacity != 0 ||
         config_.context_trace_duration != 0 ||
         config_.context_binding_mode != ContextBindingMode::none)) {
        throw std::invalid_argument(
            "disabled action context must not allocate or bind traces");
    }
    if (config_.context_trace_mode == ContextTraceMode::single &&
        config_.context_trace_capacity != 1) {
        throw std::invalid_argument("single context trace mode requires capacity one");
    }
    if (config_.context_trace_mode == ContextTraceMode::multiple &&
        config_.context_trace_capacity < 2) {
        throw std::invalid_argument("multiple context trace mode requires capacity two or more");
    }
    if (config_.context_binding_mode != ContextBindingMode::none &&
        !eligibility_enabled) {
        throw std::invalid_argument("context binding requires eligibility");
    }
    if (static_cast<std::uint8_t>(config_.plasticity_trace_mode) >
            static_cast<std::uint8_t>(PlasticityTraceMode::membrane_participation) ||
        static_cast<std::uint8_t>(config_.plasticity_decay_mode) >
            static_cast<std::uint8_t>(TraceDecayMode::local_activity) ||
        static_cast<std::uint8_t>(config_.modulation_mode) >
            static_cast<std::uint8_t>(ModulationMode::ternary)) {
        throw std::invalid_argument("invalid three-factor plasticity mode");
    }
    const bool plasticity_enabled =
        config_.plasticity_trace_mode != PlasticityTraceMode::disabled;
    if (plasticity_enabled &&
        (!eligibility_enabled || config_.plasticity_positive_step <= 0 ||
         config_.plasticity_negative_step <= 0 ||
         config_.plasticity_maximum_weight < 0 ||
         config_.plasticity_maximum_weight > config_.maximum_synapse_weight)) {
        throw std::invalid_argument("three-factor plasticity requires bounded eligibility storage");
    }
    if (!plasticity_enabled && config_.modulation_mode != ModulationMode::none) {
        throw std::invalid_argument("modulation requires a plasticity trace mode");
    }
}

void Runtime::create_local_plasticity_trace(
    const NeuronId neuron, const SynapseId candidate, const bool feedback_origin) {
    const std::uint64_t duration =
        config_.plasticity_decay_mode == TraceDecayMode::immediate
            ? 0 : config_.eligibility_duration;
    if (duration > std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
        throw std::overflow_error("plasticity trace expiry time overflow");
    }
    const auto stored = network_.store_eligibility(
        neuron, {candidate}, simulated_time_ + duration,
        config_.eligibility_history, feedback_origin, 0, 0, false, false,
        config_.plasticity_decay_mode == TraceDecayMode::stepped ||
                config_.plasticity_decay_mode == TraceDecayMode::local_activity
            ? 4 : 1);
    if (stored.record.generation == 0) return;
    ++metrics_.eligibility_decisions;
    ++metrics_.eligibility_candidates;
    if (network_.synapse(candidate).weight == 0) {
        ++metrics_.eligibility_zero_weight_candidates;
    } else {
        ++metrics_.eligibility_positive_candidates;
    }
    metrics_.eligibility_overwritten_decisions += stored.overwritten_decisions;
    if (feedback_origin) ++metrics_.feedback_created_eligibilities;
    enqueue_event(EventKind::eligibility_expiry, neuron, stored.record.expires_at,
                  stored.record.generation);
    ++metrics_.scheduled_eligibility_expiries;
    if (config_.plasticity_decay_mode == TraceDecayMode::stepped && duration >= 3) {
        const std::uint64_t interval = std::max<std::uint64_t>(1, duration / 3);
        enqueue_event(EventKind::eligibility_decay, neuron,
                      simulated_time_ + interval, stored.record.generation);
        if (interval * 2 < duration) {
            enqueue_event(EventKind::eligibility_decay, neuron,
                          simulated_time_ + interval * 2,
                          stored.record.generation);
        }
    }
}

void Runtime::apply_local_modulation(
    const NeuronId neuron, const std::int8_t polarity,
    const SynapseId modulation_synapse) {
    const auto records = network_.eligibility_records(neuron);
    std::vector<std::uint64_t> consumed;
    bool has_eligible_trace = false;
    for (const auto& record : records) {
        if (record.feedback_origin) {
            ++metrics_.plasticity_feedback_origin_traces_skipped;
            continue;
        }
        has_eligible_trace = true;
        if (polarity == 1 || polarity == -1) {
            for (const SynapseId candidate : record.candidates) {
                auto& item = network_.synapse(candidate);
                if (item.receiver != neuron) continue;
                const std::int8_t local_polarity =
                    item.effect == SynapticEffect::inhibitory &&
                            config_.inhibitory_plasticity_mode ==
                                InhibitoryPlasticityMode::contribution_aware
                        ? static_cast<std::int8_t>(-polarity)
                        : polarity;
                const std::int32_t weight_before = item.weight;
                std::int32_t proposed_weight = weight_before;
                std::int32_t requested_delta = 0;
                PlasticityWriteOutcome outcome =
                    PlasticityWriteOutcome::changed;
                const auto make_observation =
                    [&](const std::int32_t weight_after,
                        const bool protected_state, const bool changed,
                        const PlasticityWriteOutcome write_outcome) {
                        const auto& contact =
                            network_.synapse(modulation_synapse);
                        PlasticityObservation observation{
                            metrics_.processed_events, simulated_time_,
                            modulation_synapse, contact.sender, neuron,
                            polarity, local_polarity,
                            static_cast<std::uint32_t>(records.size()),
                            record.generation,
                            record.expires_at >= config_.eligibility_duration
                                ? record.expires_at -
                                      config_.eligibility_duration
                                : 0,
                            record.expires_at, record.strength,
                            record.candidates, candidate, item.sender,
                            item.effect, weight_before, weight_after,
                            protected_state, changed, write_outcome,
                            network_.neuron(neuron).membrane_potential,
                            item.activity_count};
                        observation.proposed_weight = proposed_weight;
                        observation.requested_delta = requested_delta;
                        observation.mechanism =
                            PlasticityMechanism::eligibility_modulation;
                        return observation;
                    };
                if (item.plasticity_protected) {
                    if (plasticity_observer_) {
                        plasticity_observer_(make_observation(
                            item.weight, true, false,
                            PlasticityWriteOutcome::protected_synapse));
                    }
                    continue;
                }
                if (local_polarity > 0) {
                    const std::int32_t limit = std::min(
                        config_.maximum_synapse_weight,
                        config_.plasticity_maximum_weight);
                    if (item.weight >= limit) {
                        ++metrics_.plasticity_rejected_at_weight_limit;
                        outcome = PlasticityWriteOutcome::positive_weight_limit;
                    } else {
                        std::uint64_t incoming_weight = 0;
                        if (item.effect == SynapticEffect::excitatory) {
                            for (const SynapseId incoming :
                                 network_.incoming_synapses(neuron)) {
                                const auto& input = network_.synapse(incoming);
                                if (input.effect == SynapticEffect::excitatory &&
                                    input.weight > 0) {
                                    incoming_weight +=
                                        static_cast<std::uint64_t>(input.weight);
                                }
                            }
                        }
                        const std::int32_t requested =
                            config_.plasticity_positive_step * record.strength;
                        const std::int32_t delta =
                            std::min(requested, limit - item.weight);
                        proposed_weight = item.weight + delta;
                        requested_delta = delta;
                        if (item.effect == SynapticEffect::excitatory &&
                            static_cast<std::uint64_t>(delta) >
                                config_.input_weight_budget -
                                    std::min(config_.input_weight_budget,
                                             incoming_weight)) {
                            ++metrics_.plasticity_rejected_by_budget;
                            outcome = PlasticityWriteOutcome::input_weight_budget;
                        } else if (
                            config_.experimental_plasticity_writes_suppressed) {
                            ++metrics_.plasticity_suppressed_writes;
                            outcome = PlasticityWriteOutcome::write_suppressed;
                        } else if (plasticity_write_interceptor_ &&
                                   plasticity_write_interceptor_(
                                       make_observation(
                                           proposed_weight, false, true,
                                           PlasticityWriteOutcome::changed))) {
                            ++metrics_.plasticity_suppressed_writes;
                            outcome = PlasticityWriteOutcome::
                                isolated_write_suppressed;
                        } else {
                            const bool was_zero = item.weight == 0;
                            item.weight += delta;
                            ++metrics_.plasticity_positive_changes;
                            if (item.effect == SynapticEffect::inhibitory) {
                                ++metrics_.inhibitory_positive_changes;
                            }
                            if (was_zero) {
                                ++metrics_.plasticity_zero_to_one_changes;
                            }
                        }
                    }
                } else if (local_polarity < 0 && item.weight > 0) {
                    proposed_weight = std::max<std::int32_t>(
                        0, item.weight -
                               config_.plasticity_negative_step *
                                   record.strength);
                    requested_delta = proposed_weight - item.weight;
                    if (config_.experimental_plasticity_writes_suppressed) {
                        ++metrics_.plasticity_suppressed_writes;
                        outcome = PlasticityWriteOutcome::write_suppressed;
                    } else if (plasticity_write_interceptor_ &&
                               plasticity_write_interceptor_(make_observation(
                                   proposed_weight, false, true,
                                   PlasticityWriteOutcome::changed))) {
                        ++metrics_.plasticity_suppressed_writes;
                        outcome = PlasticityWriteOutcome::
                            isolated_write_suppressed;
                    } else {
                        item.weight = proposed_weight;
                        ++metrics_.plasticity_negative_changes;
                        if (item.effect == SynapticEffect::inhibitory) {
                            ++metrics_.inhibitory_negative_changes;
                        }
                    }
                } else if (local_polarity < 0) {
                    outcome = PlasticityWriteOutcome::negative_zero_floor;
                }
                if (plasticity_observer_) {
                    plasticity_observer_(make_observation(
                        item.weight, false, item.weight != weight_before,
                        outcome));
                }
            }
            if (config_.consume_trace_on_modulation) {
                consumed.push_back(record.generation);
            }
        }
    }
    if (has_eligible_trace) {
        ++metrics_.modulation_receipts_with_trace;
    } else {
        ++metrics_.modulation_receipts_without_trace;
    }
    for (const auto generation : consumed) {
        if (network_.expire_eligibility(neuron, generation)) {
            ++metrics_.plasticity_consumed_traces;
        }
    }
}

void Runtime::process_fire(
    const NeuronId neuron,
    const bool feedback_context,
    const std::uint32_t context_signature,
    const bool context_feedback_origin,
    const std::int8_t modulation) {
    const auto outgoing = network_.outgoing_synapses(neuron);
    if (context_signature != 0 && config_.context_trace_capacity != 0) {
        if (config_.context_trace_duration >
            std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
            throw std::overflow_error("context trace expiry time overflow");
        }
        const auto stored = network_.store_context_trace(
            neuron, context_signature,
            simulated_time_ + config_.context_trace_duration,
            context_feedback_origin);
        if (stored.trace.generation != 0) {
            ++metrics_.context_traces_created;
            if (stored.overwritten) ++metrics_.context_traces_overwritten;
            enqueue_event(EventKind::context_expiry, neuron,
                          stored.trace.expires_at, stored.trace.generation);
            if (config_.context_binding_mode ==
                ContextBindingMode::temporal_overlap) {
                const auto binding = network_.bind_context_to_eligibilities(
                    neuron, stored.trace);
                metrics_.eligibility_context_bindings += binding.bound;
                metrics_.eligibility_context_ambiguities += binding.ambiguous;
            }
        }
    }
    ++metrics_.fire_events;
    if (feedback_context) ++metrics_.feedback_fire_events;
    if (modulation != 0) ++metrics_.modulation_fire_events;
    motor_output_.observe_fire(neuron, metrics_.processed_events);
    if (fire_observer_) {
        fire_observer_(FireObservation{
            metrics_.processed_events,
            simulated_time_,
            neuron,
            static_cast<std::uint32_t>(outgoing.size()),
        });
    }
    if (trace_stream_ != nullptr) {
        trace("event fire neuron=" + std::to_string(neuron) + " outgoing=" +
              std::to_string(outgoing.size()));
    }

    for (const SynapseId synapse : outgoing) {
        const EventKind kind = modulation != 0
            ? EventKind::modulation_impulse
            : feedback_context ? EventKind::feedback_impulse : EventKind::impulse;
        enqueue_event(kind, synapse, simulated_time_, 0, context_signature,
                      context_feedback_origin, modulation);
        ++metrics_.transmitted_impulses;
    }

    if (config_.growth_enabled) {
        ++metrics_.growth_attempts;
        const auto grown = brainstem_.attempt_growth(
            network_, neuron, config_, growth_rng_);
        if (grown.has_value()) {
            ++metrics_.grown_synapses;
            const neural::Synapse& item = network_.synapse(*grown);
            if (trace_stream_ != nullptr) {
                trace("event growth synapse=" + std::to_string(*grown) + " sender=" +
                      std::to_string(item.sender) + " receiver=" +
                      std::to_string(item.receiver));
            }
        }
    }
}

void Runtime::process_impulse(
    const SynapseId synapse,
    const bool feedback_context,
    const std::uint32_t context_signature,
    const bool context_feedback_origin,
    const std::int8_t modulation) {
    const neural::Synapse before = network_.synapse(synapse);
    if (modulation != 0) {
        ++metrics_.modulation_impulses;
        apply_local_modulation(before.receiver, modulation, synapse);
        // A modulatory receptor is a third-factor contact, not an ordinary
        // excitatory membrane input.  Processing stops locally: the signal
        // cannot enter G3, create a postsynaptic trace, or fire a motor output.
        return;
    }
    if (modulation == 0 && !feedback_context &&
        config_.plasticity_decay_mode == TraceDecayMode::local_activity) {
        const auto active = network_.eligibility_records(before.receiver);
        for (const auto& record : active) {
            static_cast<void>(network_.decay_eligibility(
                before.receiver, record.generation));
        }
    }
    if (context_signature != 0 && config_.context_trace_capacity != 0) {
        if (config_.context_trace_duration >
            std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
            throw std::overflow_error("context trace expiry time overflow");
        }
        const auto stored = network_.store_context_trace(
            before.receiver, context_signature,
            simulated_time_ + config_.context_trace_duration,
            context_feedback_origin);
        if (stored.trace.generation != 0) {
            ++metrics_.context_traces_created;
            if (stored.overwritten) ++metrics_.context_traces_overwritten;
            enqueue_event(EventKind::context_expiry, before.receiver,
                          stored.trace.expires_at, stored.trace.generation);
            if (config_.context_binding_mode ==
                ContextBindingMode::temporal_overlap) {
                const auto binding = network_.bind_context_to_eligibilities(
                    before.receiver, stored.trace);
                metrics_.eligibility_context_bindings += binding.bound;
                metrics_.eligibility_context_ambiguities += binding.ambiguous;
            }
        }
    }
    std::size_t eligibility_before = 0;
    if (feedback_context) {
        const auto records = network_.eligibility_records(before.receiver);
        eligibility_before = static_cast<std::size_t>(std::count_if(
            records.begin(), records.end(), [](const auto& record) {
                return !record.feedback_origin;
            }));
        std::set<std::uint32_t> matched_signatures;
        bool ambiguous = false;
        bool has_bound_context = false;
        bool feedback_origin_match = false;
        const auto traces = network_.context_traces(before.receiver);
        for (const auto& record : records) {
            if (record.feedback_origin || record.context_signature == 0) continue;
            has_bound_context = true;
            ambiguous = ambiguous || record.context_ambiguous;
            const bool active = std::any_of(
                traces.begin(), traces.end(), [&](const auto& trace) {
                    return trace.signature == record.context_signature;
                });
            if (active) {
                matched_signatures.insert(record.context_signature);
                feedback_origin_match = feedback_origin_match ||
                                        record.context_feedback_origin;
            }
        }
        if (matched_signatures.size() == 1 && !ambiguous) {
            ++metrics_.feedback_context_unique_matches;
        } else if (matched_signatures.size() > 1 ||
                   (!matched_signatures.empty() && ambiguous)) {
            ++metrics_.feedback_context_ambiguous_matches;
        } else if (has_bound_context) {
            ++metrics_.feedback_context_expired_matches;
        } else {
            ++metrics_.feedback_context_missing_matches;
        }
        if (feedback_origin_match) ++metrics_.feedback_origin_context_matches;
    }
    if (feedback_context) {
        ++metrics_.feedback_impulses;
        if (eligibility_before == 0) {
            ++metrics_.feedback_receipts_without_eligibility;
        } else {
            ++metrics_.feedback_receipts_with_eligibility;
            if (eligibility_before > 1) {
                ++metrics_.feedback_receipts_with_multiple_eligibilities;
            }
        }
    }
    std::int64_t membrane_before = 0;
    std::uint32_t refractory_before = 0;
    if (impulse_observer_) {
        const neural::Neuron& receiver = network_.neuron(before.receiver);
        membrane_before = receiver.membrane_potential;
        refractory_before = receiver.refractory_events_remaining;
    }
    const brainstem::ImpulseResult result =
        brainstem_.receive_impulse(network_, synapse, config_, &rng_);
    if (result.effect == SynapticEffect::inhibitory) {
        ++metrics_.inhibitory_impulses;
        metrics_.inhibitory_applied_strength +=
            static_cast<std::uint64_t>(-static_cast<std::int64_t>(result.applied_weight));
    }
    brainstem::EligibilityResult eligibility;
    const std::uint64_t eligibility_trace_duration =
        config_.plasticity_trace_mode != PlasticityTraceMode::disabled &&
                config_.plasticity_decay_mode == TraceDecayMode::immediate
            ? 0 : config_.eligibility_duration;
    if (modulation == 0 && !feedback_context &&
        config_.plasticity_trace_mode == PlasticityTraceMode::membrane_participation) {
        create_local_plasticity_trace(before.receiver, synapse, false);
    } else if (result.fired && modulation == 0 && !feedback_context &&
               config_.plasticity_trace_mode == PlasticityTraceMode::pre_post_fire) {
        create_local_plasticity_trace(before.receiver, synapse, false);
    } else if (result.fired &&
        config_.eligibility_selection != EligibilitySelection::none) {
        if (config_.eligibility_duration >
            std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
            throw std::overflow_error("eligibility expiry time overflow");
        }
        eligibility = brainstem_.create_eligibility(
            network_, before.receiver,
            simulated_time_ + eligibility_trace_duration, config_, feedback_context);
        if (eligibility.created) {
            ++metrics_.eligibility_decisions;
            metrics_.eligibility_candidates += eligibility.candidates.size();
            metrics_.eligibility_positive_candidates +=
                eligibility.positive_candidates;
            metrics_.eligibility_zero_weight_candidates +=
                eligibility.zero_weight_candidates;
            metrics_.eligibility_overwritten_decisions +=
                eligibility.overwritten_decisions;
            if (feedback_context) ++metrics_.feedback_created_eligibilities;
            if (eligibility.context_signature != 0) {
                ++metrics_.eligibility_context_bindings;
            }
            if (eligibility.context_ambiguous) {
                ++metrics_.eligibility_context_ambiguities;
            }
        }
    }
    if (result.local_event_recorded) {
        ++metrics_.local_event_memory_records;
    }
    if (result.local_event_overwritten) {
        ++metrics_.local_event_memory_overwrites;
    }
    if (result.strengthened) {
        ++metrics_.synaptic_strengthenings;
    }
    if (result.strengthening_rejected_by_weight_limit) {
        ++metrics_.rejected_strengthenings_at_weight_limit;
    }
    if (result.weakened) {
        ++metrics_.synaptic_weakenings;
    }
    if (result.firing_suppressed) {
        ++metrics_.refractory_suppressed_firings;
    }
    if (result.phase_completed) {
        ++metrics_.completed_activity_phases;
    }
    if (result.fired && config_.eligibility_selection ==
                            EligibilitySelection::active_phase) {
        // Delimit the next postsynaptic decision locally.  G3 stays intact.
        network_.reset_incoming_activity(before.receiver);
    }

    if (trace_stream_ != nullptr) {
        std::ostringstream line;
        line << "event impulse synapse=" << synapse << " sender=" << before.sender
             << " receiver=" << before.receiver << " weight=" << result.applied_weight
             << " membrane=" << result.membrane_after
             << " strengthened=" << (result.strengthened ? 1 : 0)
             << " phase=" << (result.phase_completed ? 1 : 0)
             << " fired=" << (result.fired ? 1 : 0);
        trace(line.str());
    }
    if (impulse_observer_) {
        const neural::Neuron& receiver_after = network_.neuron(before.receiver);
        impulse_observer_(ImpulseObservation{
            metrics_.processed_events,
            simulated_time_,
            synapse,
            before.sender,
            before.receiver,
            result.applied_weight,
            result.effect,
            membrane_before,
            receiver_after.membrane_potential,
            refractory_before,
            receiver_after.refractory_events_remaining,
            result.fired,
            result.firing_suppressed,
            feedback_context,
            context_signature,
            context_feedback_origin,
            network_.local_event_memory(before.receiver),
            eligibility.generation,
            eligibility.candidates,
            std::vector<SynapseId>{synapse},
            std::vector<std::int32_t>{result.applied_weight},
            result.effect == SynapticEffect::excitatory
                ? static_cast<std::int64_t>(result.applied_weight) : 0,
            result.effect == SynapticEffect::inhibitory
                ? -static_cast<std::int64_t>(result.applied_weight) : 0,
            static_cast<std::int64_t>(result.applied_weight),
            result.membrane_at_decision,
            result.firing_threshold,
            result.decision_margin,
            result.refractory_before != 0,
            false,
            0,
        });
    }

    if (result.fired) {
        const EventKind kind = modulation != 0
            ? EventKind::modulation_fire
            : feedback_context ? EventKind::feedback_fire : EventKind::fire;
        enqueue_event(kind, before.receiver, simulated_time_, 0, context_signature,
                      context_feedback_origin, modulation);
        if (config_.recovery_mode != RecoveryMode::input_dependent) {
            if (config_.recovery_delay >
                std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
                throw std::overflow_error("recovery time overflow");
            }
            const std::uint64_t recovery_time = simulated_time_ + config_.recovery_delay;
            network_.neuron(before.receiver).scheduled_recovery_time = recovery_time;
            enqueue_event(
                EventKind::recovery, before.receiver, recovery_time,
                result.recovery_generation);
            ++metrics_.scheduled_recovery_events;
        }
        if (eligibility.created) {
            enqueue_event(
                EventKind::eligibility_expiry, before.receiver,
                eligibility.expires_at, eligibility.generation);
            ++metrics_.scheduled_eligibility_expiries;
            if (config_.plasticity_decay_mode == TraceDecayMode::stepped &&
                eligibility_trace_duration >= 3) {
                const std::uint64_t interval =
                    std::max<std::uint64_t>(1, eligibility_trace_duration / 3);
                enqueue_event(EventKind::eligibility_decay, before.receiver,
                              simulated_time_ + interval,
                              eligibility.generation);
                if (interval * 2 < eligibility_trace_duration) {
                    enqueue_event(EventKind::eligibility_decay, before.receiver,
                                  simulated_time_ + interval * 2,
                                  eligibility.generation);
                }
            }
        }
    }
    update_subthreshold_relaxation(before.receiver, result.fired);
}

void Runtime::process_recovery(const Event& event) {
    const brainstem::RecoveryResult result = brainstem_.recover_neuron(
        network_, event.target, event.generation, config_);
    if (result.recovered) {
        ++metrics_.processed_recovery_events;
    }
    if (result.stale) {
        ++metrics_.stale_recovery_events;
    }
    if (recovery_observer_) {
        recovery_observer_(RecoveryObservation{
            metrics_.processed_events,
            simulated_time_,
            event.target,
            event.generation,
            result.membrane_before,
            result.membrane_after,
            result.refractory_before,
            result.refractory_after,
            result.recovered,
            result.stale,
        });
    }
    if (result.recovered) {
        update_subthreshold_relaxation(event.target, false);
    }
}

void Runtime::update_subthreshold_relaxation(
    const NeuronId neuron, const bool fired) {
    if (!config_.subthreshold_relaxation_enabled) return;
    auto& item = network_.neuron(neuron);
    const bool subthreshold = item.membrane_potential > 0 &&
        item.membrane_potential < item.firing_threshold;
    if (fired || !subthreshold) {
        if (item.scheduled_subthreshold_relaxation_time != 0) {
            if (item.subthreshold_relaxation_generation ==
                std::numeric_limits<std::uint64_t>::max()) {
                throw std::overflow_error(
                    "subthreshold relaxation generation overflow");
            }
            ++item.subthreshold_relaxation_generation;
            item.scheduled_subthreshold_relaxation_time = 0;
        }
        return;
    }
    // A local subthreshold state has one fixed lifetime. Further inputs that
    // keep it subthreshold do not accumulate additional valid events.
    if (item.scheduled_subthreshold_relaxation_time != 0) return;
    if (config_.subthreshold_relaxation_delay >
        std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
        throw std::overflow_error("subthreshold relaxation time overflow");
    }
    if (item.subthreshold_relaxation_generation ==
        std::numeric_limits<std::uint64_t>::max()) {
        throw std::overflow_error(
            "subthreshold relaxation generation overflow");
    }
    ++item.subthreshold_relaxation_generation;
    const auto due = simulated_time_ + config_.subthreshold_relaxation_delay;
    item.scheduled_subthreshold_relaxation_time = due;
    enqueue_event(EventKind::subthreshold_relaxation, neuron, due,
                  item.subthreshold_relaxation_generation);
    ++metrics_.scheduled_subthreshold_relaxations;
}

void Runtime::process_subthreshold_relaxation(const Event& event) {
    auto& item = network_.neuron(event.target);
    const auto membrane_before = item.membrane_potential;
    const bool valid = config_.subthreshold_relaxation_enabled &&
        event.generation == item.subthreshold_relaxation_generation &&
        event.scheduled_time == item.scheduled_subthreshold_relaxation_time &&
        item.membrane_potential > 0 &&
        item.membrane_potential < item.firing_threshold;
    if (valid) {
        item.membrane_potential = 0;
        item.scheduled_subthreshold_relaxation_time = 0;
        ++metrics_.processed_subthreshold_relaxations;
    } else {
        // A diagnostic membrane intervention can make the currently due
        // generation stale without going through ordinary input handling.
        // Clear only that consumed generation; never disturb a newer event.
        if (event.generation == item.subthreshold_relaxation_generation &&
            event.scheduled_time ==
                item.scheduled_subthreshold_relaxation_time) {
            item.scheduled_subthreshold_relaxation_time = 0;
        }
        ++metrics_.stale_subthreshold_relaxations;
    }
    if (subthreshold_relaxation_observer_) {
        subthreshold_relaxation_observer_(
            SubthresholdRelaxationObservation{
                metrics_.processed_events, simulated_time_, event.target,
                event.generation, membrane_before, item.membrane_potential,
                valid, !valid});
    }
}

void Runtime::process_eligibility_expiry(const Event& event) {
    if (brainstem_.expire_eligibility(
            network_, event.target, event.generation)) {
        ++metrics_.processed_eligibility_expiries;
    } else {
        ++metrics_.stale_eligibility_expiries;
    }
}

void Runtime::process_eligibility_decay(const Event& event) {
    static_cast<void>(network_.decay_eligibility(event.target, event.generation));
}

void Runtime::process_context_expiry(const Event& event) {
    if (network_.expire_context_trace(event.target, event.generation)) {
        ++metrics_.processed_context_expiries;
    } else {
        ++metrics_.stale_context_expiries;
    }
}

void Runtime::process_event(const Event& event) {
    if (event.scheduled_time > simulated_time_) {
        throw std::logic_error("attempted to process a future event");
    }
    ++metrics_.processed_events;
    update_event_hash(event);
    switch (event.kind) {
    case EventKind::fire:
        process_fire(event.target, false, event.context_signature,
                     event.context_feedback_origin);
        break;
    case EventKind::impulse:
        process_impulse(event.target, false, event.context_signature,
                        event.context_feedback_origin);
        break;
    case EventKind::recovery:
        process_recovery(event);
        break;
    case EventKind::boundary:
        ++metrics_.processed_boundary_events;
        break;
    case EventKind::eligibility_expiry:
        process_eligibility_expiry(event);
        break;
    case EventKind::feedback_fire:
        process_fire(event.target, true, event.context_signature,
                     event.context_feedback_origin);
        break;
    case EventKind::feedback_impulse:
        process_impulse(event.target, true, event.context_signature,
                        event.context_feedback_origin);
        break;
    case EventKind::context_expiry:
        process_context_expiry(event);
        break;
    case EventKind::modulation_fire:
        process_fire(event.target, true, 0, true, event.modulation);
        break;
    case EventKind::modulation_impulse:
        process_impulse(event.target, true, 0, true, event.modulation);
        break;
    case EventKind::eligibility_decay:
        process_eligibility_decay(event);
        break;
    case EventKind::subthreshold_relaxation:
        process_subthreshold_relaxation(event);
        break;
    }
}

void Runtime::enqueue_event(
    const EventKind kind,
    const std::uint32_t target,
    const std::uint64_t scheduled_time,
    const std::uint64_t generation,
    const std::uint32_t context_signature,
    const bool context_feedback_origin,
    const std::int8_t modulation) {
    Event event{kind, target, scheduled_time, next_event_sequence_++, generation,
                context_signature, context_feedback_origin, modulation};
    const auto position = std::upper_bound(
        queue_.begin(), queue_.end(), event,
        [](const Event& left, const Event& right) {
            if (left.scheduled_time != right.scheduled_time) {
                return left.scheduled_time < right.scheduled_time;
            }
            return left.sequence < right.sequence;
        });
    queue_.insert(position, event);
    update_queue_watermark();
}

bool Runtime::has_ready_event() const noexcept {
    return !queue_.empty() && queue_.front().scheduled_time <= simulated_time_;
}

std::uint64_t Runtime::process_ready_wave() {
    std::vector<Event> wave;
    const std::uint64_t wave_time = simulated_time_;
    const std::size_t initial_size = queue_.size();
    for (std::size_t index = 0; index < initial_size && has_ready_event(); ++index) {
        if (queue_.front().scheduled_time != wave_time) break;
        wave.push_back(pop_next_event());
    }
    if (wave.empty()) return 0;

    std::map<NeuronId, std::vector<SynapseId>> ordinary_inputs;
    for (const Event& event : wave) {
        if (event.kind == EventKind::impulse) {
            const auto& synapse = network_.synapse(event.target);
            ordinary_inputs[synapse.receiver].push_back(event.target);
        } else {
            process_event(event);
        }
    }
    for (auto& [receiver, synapses] : ordinary_inputs) {
        static_cast<void>(receiver);
        static_cast<void>(process_simultaneous_impulses(synapses));
    }
    return wave.size();
}

Event Runtime::pop_next_event() {
    if (queue_.empty()) {
        throw std::logic_error("cannot pop an empty event queue");
    }
    const Event event = queue_.front();
    queue_.pop_front();
    return event;
}

void Runtime::update_queue_watermark() noexcept {
    if (queue_.size() > metrics_.maximum_queue_size) {
        metrics_.maximum_queue_size = queue_.size();
    }
}

void Runtime::update_event_hash(const Event& event) noexcept {
    mix_hash(metrics_.event_trace_hash, static_cast<std::uint8_t>(event.kind));
    mix_hash(metrics_.event_trace_hash, event.target);
    mix_hash(metrics_.event_trace_hash, event.scheduled_time);
    mix_hash(metrics_.event_trace_hash, event.sequence);
    mix_hash(metrics_.event_trace_hash, event.generation);
}

void Runtime::trace(const std::string& line) {
    if (trace_stream_ != nullptr) {
        *trace_stream_ << line << '\n';
        if (!*trace_stream_) {
            throw std::runtime_error("failed to write diagnostic event trace");
        }
    }
}

}  // namespace eve::bios
