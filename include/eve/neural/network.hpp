#pragma once

#include "eve/types.hpp"

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace eve::neural {

struct Neuron {
    std::int64_t membrane_potential{};
    std::int64_t firing_threshold{};
    std::uint32_t input_event_count{};
    std::uint32_t refractory_events_remaining{};
    std::uint64_t recovery_generation{};
    std::uint64_t scheduled_recovery_time{};
    std::uint64_t subthreshold_relaxation_generation{};
    std::uint64_t scheduled_subthreshold_relaxation_time{};
    SynapseId first_incoming{kInvalidIndex};
    SynapseId first_outgoing{kInvalidIndex};
    std::uint32_t incoming_count{};
    std::uint32_t outgoing_count{};

    bool operator==(const Neuron& other) const noexcept;
};

struct Synapse {
    NeuronId sender{};
    NeuronId receiver{};
    std::int32_t weight{};
    std::uint32_t activity_count{};
    SynapseId next_incoming{kInvalidIndex};
    SynapseId next_outgoing{kInvalidIndex};
    bool plasticity_protected{};
    SynapticEffect effect{SynapticEffect::excitatory};

    bool operator==(const Synapse& other) const noexcept;
};

struct EligibilityRecord {
    std::uint64_t generation{};
    std::uint64_t expires_at{};
    bool feedback_origin{};
    std::uint32_t context_signature{};
    std::uint64_t context_generation{};
    bool context_ambiguous{};
    bool context_feedback_origin{};
    std::uint8_t strength{1};
    std::vector<SynapseId> candidates;

    bool operator==(const EligibilityRecord& other) const noexcept {
        return generation == other.generation && expires_at == other.expires_at &&
               feedback_origin == other.feedback_origin &&
               context_signature == other.context_signature &&
               context_generation == other.context_generation &&
               context_ambiguous == other.context_ambiguous &&
               context_feedback_origin == other.context_feedback_origin &&
               strength == other.strength &&
               candidates == other.candidates;
    }
};

struct ContextTrace {
    std::uint64_t generation{};
    std::uint32_t signature{};
    std::uint64_t expires_at{};
    bool feedback_origin{};

    bool operator==(const ContextTrace& other) const noexcept {
        return generation == other.generation && signature == other.signature &&
               expires_at == other.expires_at &&
               feedback_origin == other.feedback_origin;
    }
};

struct ContextStoreResult {
    ContextTrace trace;
    bool overwritten{};
};

struct ContextBindingResult {
    std::uint32_t bound{};
    std::uint32_t ambiguous{};
};

struct EligibilityStoreResult {
    EligibilityRecord record;
    std::uint32_t overwritten_decisions{};
};

class Network {
public:
    void reset(const ExperimentConfig& config);

    [[nodiscard]] SynapseId add_synapse(
        NeuronId sender,
        NeuronId receiver,
        std::int32_t weight,
        SynapticEffect effect = SynapticEffect::excitatory);
    [[nodiscard]] bool has_synapse(NeuronId sender, NeuronId receiver) const;
    [[nodiscard]] std::optional<SynapseId> find_synapse(
        NeuronId sender,
        NeuronId receiver) const;

    [[nodiscard]] Coordinate coordinate(NeuronId neuron) const;
    [[nodiscard]] std::vector<SynapseId> outgoing_synapses(NeuronId neuron) const;
    [[nodiscard]] std::vector<SynapseId> incoming_synapses(NeuronId neuron) const;
    void reset_incoming_activity(NeuronId neuron);

    [[nodiscard]] bool record_local_event(NeuronId neuron, SynapseId synapse);
    [[nodiscard]] std::vector<SynapseId> local_event_memory(NeuronId neuron) const;
    [[nodiscard]] std::uint64_t local_event_overwrites(NeuronId neuron) const;
    [[nodiscard]] std::uint32_t local_event_memory_capacity() const noexcept;
    void restore_local_event_memory(
        std::uint32_t capacity,
        const std::vector<std::vector<SynapseId>>& events,
        const std::vector<std::uint64_t>& overwrites);

    [[nodiscard]] EligibilityStoreResult store_eligibility(
        NeuronId neuron,
        const std::vector<SynapseId>& candidates,
        std::uint64_t expires_at,
        EligibilityHistory history,
        bool feedback_origin = false,
        std::uint32_t context_signature = 0,
        std::uint64_t context_generation = 0,
        bool context_ambiguous = false,
        bool context_feedback_origin = false,
        std::uint8_t strength = 1);
    [[nodiscard]] bool expire_eligibility(NeuronId neuron, std::uint64_t generation);
    [[nodiscard]] std::uint8_t decay_eligibility(
        NeuronId neuron, std::uint64_t generation);
    [[nodiscard]] std::vector<EligibilityRecord> eligibility_records(
        NeuronId neuron) const;
    [[nodiscard]] std::uint64_t eligibility_generation(NeuronId neuron) const;
    [[nodiscard]] std::uint64_t eligibility_overwrites(NeuronId neuron) const;
    [[nodiscard]] std::uint32_t eligibility_candidate_capacity() const noexcept;
    [[nodiscard]] std::uint32_t eligibility_decision_capacity() const noexcept;
    [[nodiscard]] std::size_t active_eligibility_count() const noexcept;
    void restore_eligibility(
        std::uint32_t candidate_capacity,
        std::uint32_t decision_capacity,
        const std::vector<std::vector<EligibilityRecord>>& records,
        const std::vector<std::uint64_t>& generations,
        const std::vector<std::uint64_t>& overwrites);

    [[nodiscard]] ContextStoreResult store_context_trace(
        NeuronId neuron,
        std::uint32_t signature,
        std::uint64_t expires_at,
        bool feedback_origin = false);
    [[nodiscard]] bool expire_context_trace(NeuronId neuron, std::uint64_t generation);
    [[nodiscard]] std::vector<ContextTrace> context_traces(NeuronId neuron) const;
    [[nodiscard]] ContextBindingResult bind_context_to_eligibilities(
        NeuronId neuron, const ContextTrace& trace);
    [[nodiscard]] std::uint32_t context_trace_capacity() const noexcept;
    [[nodiscard]] std::uint64_t context_generation(NeuronId neuron) const;
    [[nodiscard]] std::uint64_t context_overwrites(NeuronId neuron) const;
    [[nodiscard]] std::size_t active_context_trace_count() const noexcept;
    void restore_context_traces(
        std::uint32_t capacity,
        const std::vector<std::vector<ContextTrace>>& traces,
        const std::vector<std::uint64_t>& generations,
        const std::vector<std::uint64_t>& overwrites);

    [[nodiscard]] const Neuron& neuron(NeuronId id) const;
    [[nodiscard]] Neuron& neuron(NeuronId id);
    [[nodiscard]] const Synapse& synapse(SynapseId id) const;
    [[nodiscard]] Synapse& synapse(SynapseId id);

    [[nodiscard]] std::uint32_t neuron_count() const noexcept;
    [[nodiscard]] std::uint32_t synapse_count() const noexcept;
    [[nodiscard]] std::size_t estimated_memory_bytes() const noexcept;
    [[nodiscard]] std::map<std::int32_t, std::uint64_t> weight_distribution() const;
    [[nodiscard]] std::uint64_t state_fingerprint() const noexcept;
    [[nodiscard]] bool validate(std::string* error = nullptr) const;

    [[nodiscard]] std::uint32_t width() const noexcept;
    [[nodiscard]] std::uint32_t height() const noexcept;
    [[nodiscard]] std::uint32_t depth() const noexcept;

    [[nodiscard]] const std::vector<Neuron>& neurons() const noexcept;
    [[nodiscard]] const std::vector<Synapse>& synapses() const noexcept;
    void restore(
        std::uint32_t width,
        std::uint32_t height,
        std::uint32_t depth,
        std::vector<Neuron> neurons,
        std::vector<Synapse> synapses);

private:
    void require_neuron(NeuronId id) const;
    void require_synapse(SynapseId id) const;

    std::uint32_t width_{};
    std::uint32_t height_{};
    std::uint32_t depth_{};
    std::vector<Neuron> neurons_;
    std::vector<Synapse> synapses_;
    std::uint32_t local_event_memory_capacity_{};
    std::vector<SynapseId> local_event_slots_;
    std::vector<std::uint32_t> local_event_write_positions_;
    std::vector<std::uint32_t> local_event_sizes_;
    std::vector<std::uint64_t> local_event_overwrites_;
    std::uint32_t eligibility_candidate_capacity_{};
    std::uint32_t eligibility_decision_capacity_{};
    std::vector<SynapseId> eligibility_slots_;
    std::vector<std::uint32_t> eligibility_sizes_;
    std::vector<std::uint64_t> eligibility_record_generations_;
    std::vector<std::uint64_t> eligibility_expiry_times_;
    std::vector<std::uint8_t> eligibility_feedback_origins_;
    std::vector<std::uint32_t> eligibility_context_signatures_;
    std::vector<std::uint64_t> eligibility_context_generations_;
    std::vector<std::uint8_t> eligibility_context_ambiguities_;
    std::vector<std::uint8_t> eligibility_context_feedback_origins_;
    std::vector<std::uint8_t> eligibility_strengths_;
    std::vector<std::uint32_t> eligibility_write_positions_;
    std::vector<std::uint64_t> eligibility_generations_;
    std::vector<std::uint64_t> eligibility_overwrites_;
    std::uint32_t context_trace_capacity_{};
    std::vector<std::uint64_t> context_trace_record_generations_;
    std::vector<std::uint32_t> context_trace_signatures_;
    std::vector<std::uint64_t> context_trace_expiry_times_;
    std::vector<std::uint8_t> context_trace_feedback_origins_;
    std::vector<std::uint32_t> context_trace_write_positions_;
    std::vector<std::uint64_t> context_trace_generations_;
    std::vector<std::uint64_t> context_trace_overwrites_;
};

}  // namespace eve::neural
