#include "eve/neural/network.hpp"

#include <limits>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <utility>

namespace eve::neural {
namespace {

constexpr std::uint64_t kFnvOffset = 14695981039346656037ULL;
constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

void hash_u64(std::uint64_t& hash, std::uint64_t value) noexcept {
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash ^= (value >> shift) & 0xffU;
        hash *= kFnvPrime;
    }
}

std::uint64_t connection_key(NeuronId sender, NeuronId receiver) noexcept {
    return (static_cast<std::uint64_t>(sender) << 32U) | receiver;
}

}  // namespace

bool Neuron::operator==(const Neuron& other) const noexcept {
    return membrane_potential == other.membrane_potential &&
           firing_threshold == other.firing_threshold &&
           input_event_count == other.input_event_count &&
           refractory_events_remaining == other.refractory_events_remaining &&
           recovery_generation == other.recovery_generation &&
           scheduled_recovery_time == other.scheduled_recovery_time &&
           first_incoming == other.first_incoming &&
           first_outgoing == other.first_outgoing &&
           incoming_count == other.incoming_count && outgoing_count == other.outgoing_count;
}

bool Synapse::operator==(const Synapse& other) const noexcept {
    return sender == other.sender && receiver == other.receiver && weight == other.weight &&
           activity_count == other.activity_count &&
           next_incoming == other.next_incoming && next_outgoing == other.next_outgoing &&
           plasticity_protected == other.plasticity_protected && effect == other.effect;
}

void Network::reset(const ExperimentConfig& config) {
    width_ = config.width;
    height_ = config.height;
    depth_ = config.depth;
    neurons_.assign(config.neuron_count, Neuron{});
    for (Neuron& item : neurons_) {
        item.firing_threshold = config.firing_threshold;
    }
    synapses_.clear();
    const auto expected = static_cast<std::size_t>(config.neuron_count) *
                          config.initial_outgoing_synapses;
    synapses_.reserve(expected);
    local_event_memory_capacity_ = config.local_event_memory_capacity;
    const auto slot_count = static_cast<std::size_t>(config.neuron_count) *
                            local_event_memory_capacity_;
    if (local_event_memory_capacity_ != 0 &&
        slot_count / local_event_memory_capacity_ != config.neuron_count) {
        throw std::overflow_error("local event memory size overflow");
    }
    local_event_slots_.assign(slot_count, kInvalidIndex);
    if (local_event_memory_capacity_ == 0) {
        local_event_write_positions_.clear();
        local_event_sizes_.clear();
        local_event_overwrites_.clear();
    } else {
        local_event_write_positions_.assign(config.neuron_count, 0);
        local_event_sizes_.assign(config.neuron_count, 0);
        local_event_overwrites_.assign(config.neuron_count, 0);
    }

    eligibility_candidate_capacity_ = config.eligibility_candidate_capacity;
    eligibility_decision_capacity_ = config.eligibility_decision_capacity;
    const auto record_count = static_cast<std::size_t>(config.neuron_count) *
                              eligibility_decision_capacity_;
    const auto candidate_count = record_count * eligibility_candidate_capacity_;
    if ((eligibility_decision_capacity_ != 0 &&
         record_count / eligibility_decision_capacity_ != config.neuron_count) ||
        (eligibility_candidate_capacity_ != 0 &&
         candidate_count / eligibility_candidate_capacity_ != record_count)) {
        throw std::overflow_error("eligibility storage size overflow");
    }
    eligibility_slots_.assign(candidate_count, kInvalidIndex);
    eligibility_sizes_.assign(record_count, 0);
    eligibility_record_generations_.assign(record_count, 0);
    eligibility_expiry_times_.assign(record_count, 0);
    eligibility_feedback_origins_.assign(record_count, 0);
    eligibility_context_signatures_.assign(record_count, 0);
    eligibility_context_generations_.assign(record_count, 0);
    eligibility_context_ambiguities_.assign(record_count, 0);
    eligibility_context_feedback_origins_.assign(record_count, 0);
    eligibility_strengths_.assign(record_count, 0);
    if (record_count == 0 || eligibility_candidate_capacity_ == 0) {
        eligibility_write_positions_.clear();
        eligibility_generations_.clear();
        eligibility_overwrites_.clear();
    } else {
        eligibility_write_positions_.assign(config.neuron_count, 0);
        eligibility_generations_.assign(config.neuron_count, 0);
        eligibility_overwrites_.assign(config.neuron_count, 0);
    }

    context_trace_capacity_ = config.context_trace_capacity;
    const auto context_count = static_cast<std::size_t>(config.neuron_count) *
                               context_trace_capacity_;
    if (context_trace_capacity_ != 0 &&
        context_count / context_trace_capacity_ != config.neuron_count) {
        throw std::overflow_error("context trace storage size overflow");
    }
    context_trace_record_generations_.assign(context_count, 0);
    context_trace_signatures_.assign(context_count, 0);
    context_trace_expiry_times_.assign(context_count, 0);
    context_trace_feedback_origins_.assign(context_count, 0);
    if (context_trace_capacity_ == 0) {
        context_trace_write_positions_.clear();
        context_trace_generations_.clear();
        context_trace_overwrites_.clear();
    } else {
        context_trace_write_positions_.assign(config.neuron_count, 0);
        context_trace_generations_.assign(config.neuron_count, 0);
        context_trace_overwrites_.assign(config.neuron_count, 0);
    }
}

SynapseId Network::add_synapse(
    const NeuronId sender,
    const NeuronId receiver,
    const std::int32_t weight,
    const SynapticEffect effect) {
    require_neuron(sender);
    require_neuron(receiver);
    if (sender == receiver) {
        throw std::invalid_argument("self-connections are not allowed");
    }
    if (has_synapse(sender, receiver)) {
        throw std::invalid_argument("duplicate directed synapse");
    }
    if (synapses_.size() >= kInvalidIndex) {
        throw std::overflow_error("synapse index space exhausted");
    }

    const auto id = static_cast<SynapseId>(synapses_.size());
    Neuron& source = neurons_[sender];
    Neuron& target = neurons_[receiver];
    synapses_.push_back(Synapse{
        sender,
        receiver,
        weight,
        0,
        target.first_incoming,
        source.first_outgoing,
        false,
        effect,
    });
    source.first_outgoing = id;
    ++source.outgoing_count;
    target.first_incoming = id;
    ++target.incoming_count;
    return id;
}

bool Network::has_synapse(const NeuronId sender, const NeuronId receiver) const {
    return find_synapse(sender, receiver).has_value();
}

std::optional<SynapseId> Network::find_synapse(
    const NeuronId sender,
    const NeuronId receiver) const {
    require_neuron(sender);
    require_neuron(receiver);
    SynapseId current = neurons_[sender].first_outgoing;
    while (current != kInvalidIndex) {
        require_synapse(current);
        const Synapse& item = synapses_[current];
        if (item.receiver == receiver) {
            return current;
        }
        current = item.next_outgoing;
    }
    return std::nullopt;
}

Coordinate Network::coordinate(const NeuronId id) const {
    require_neuron(id);
    if (width_ == 0 || height_ == 0) {
        throw std::logic_error("network geometry is not initialized");
    }
    const std::uint32_t plane = width_ * height_;
    return Coordinate{id % width_, (id / width_) % height_, id / plane};
}

std::vector<SynapseId> Network::outgoing_synapses(const NeuronId id) const {
    require_neuron(id);
    std::vector<SynapseId> result;
    result.reserve(neurons_[id].outgoing_count);
    SynapseId current = neurons_[id].first_outgoing;
    while (current != kInvalidIndex) {
        require_synapse(current);
        result.push_back(current);
        current = synapses_[current].next_outgoing;
    }
    return result;
}

std::vector<SynapseId> Network::incoming_synapses(const NeuronId id) const {
    require_neuron(id);
    std::vector<SynapseId> result;
    result.reserve(neurons_[id].incoming_count);
    SynapseId current = neurons_[id].first_incoming;
    while (current != kInvalidIndex) {
        require_synapse(current);
        result.push_back(current);
        current = synapses_[current].next_incoming;
    }
    return result;
}

void Network::reset_incoming_activity(const NeuronId id) {
    require_neuron(id);
    SynapseId current = neurons_[id].first_incoming;
    while (current != kInvalidIndex) {
        require_synapse(current);
        Synapse& item = synapses_[current];
        item.activity_count = 0;
        current = item.next_incoming;
    }
}

bool Network::record_local_event(const NeuronId id, const SynapseId synapse_id) {
    require_neuron(id);
    require_synapse(synapse_id);
    if (synapses_[synapse_id].receiver != id) {
        throw std::invalid_argument("local event synapse does not target neuron");
    }
    if (local_event_memory_capacity_ == 0) {
        return false;
    }

    const std::size_t base = static_cast<std::size_t>(id) * local_event_memory_capacity_;
    const std::uint32_t write = local_event_write_positions_[id];
    const bool overwritten = local_event_sizes_[id] == local_event_memory_capacity_;
    local_event_slots_[base + write] = synapse_id;
    local_event_write_positions_[id] = (write + 1U) % local_event_memory_capacity_;
    if (overwritten) {
        ++local_event_overwrites_[id];
    } else {
        ++local_event_sizes_[id];
    }
    return overwritten;
}

std::vector<SynapseId> Network::local_event_memory(const NeuronId id) const {
    require_neuron(id);
    if (local_event_memory_capacity_ == 0) {
        return {};
    }
    const std::uint32_t size = local_event_sizes_[id];
    std::vector<SynapseId> result;
    result.reserve(size);
    if (size == 0) {
        return result;
    }
    const std::size_t base = static_cast<std::size_t>(id) * local_event_memory_capacity_;
    const std::uint32_t oldest = size == local_event_memory_capacity_
                                     ? local_event_write_positions_[id]
                                     : 0U;
    for (std::uint32_t index = 0; index < size; ++index) {
        result.push_back(local_event_slots_[
            base + (oldest + index) % local_event_memory_capacity_]);
    }
    return result;
}

std::uint64_t Network::local_event_overwrites(const NeuronId id) const {
    require_neuron(id);
    if (local_event_memory_capacity_ == 0) {
        return 0;
    }
    return local_event_overwrites_[id];
}

std::uint32_t Network::local_event_memory_capacity() const noexcept {
    return local_event_memory_capacity_;
}

void Network::restore_local_event_memory(
    const std::uint32_t capacity,
    const std::vector<std::vector<SynapseId>>& events,
    const std::vector<std::uint64_t>& overwrites) {
    if (events.size() != neurons_.size() || overwrites.size() != neurons_.size()) {
        throw std::runtime_error("local event memory count does not match neurons");
    }
    local_event_memory_capacity_ = capacity;
    if (capacity == 0) {
        for (std::size_t index = 0; index < events.size(); ++index) {
            if (!events[index].empty() || overwrites[index] != 0) {
                throw std::runtime_error("disabled local event memory contains state");
            }
        }
        local_event_slots_.clear();
        local_event_write_positions_.clear();
        local_event_sizes_.clear();
        local_event_overwrites_.clear();
        return;
    }
    const std::size_t slot_count = neurons_.size() * static_cast<std::size_t>(capacity);
    if (capacity != 0 && slot_count / capacity != neurons_.size()) {
        throw std::runtime_error("restored local event memory size overflow");
    }
    local_event_slots_.assign(slot_count, kInvalidIndex);
    local_event_write_positions_.assign(neurons_.size(), 0);
    local_event_sizes_.assign(neurons_.size(), 0);
    local_event_overwrites_ = overwrites;
    for (NeuronId neuron_id = 0; neuron_id < neurons_.size(); ++neuron_id) {
        if (events[neuron_id].size() > capacity) {
            throw std::runtime_error("restored local event memory exceeds capacity");
        }
        for (const SynapseId synapse_id : events[neuron_id]) {
            require_synapse(synapse_id);
            if (synapses_[synapse_id].receiver != neuron_id) {
                throw std::runtime_error("restored local event has wrong receiver");
            }
            static_cast<void>(record_local_event(neuron_id, synapse_id));
        }
        local_event_overwrites_[neuron_id] = overwrites[neuron_id];
    }
}

EligibilityStoreResult Network::store_eligibility(
    const NeuronId id,
    const std::vector<SynapseId>& input_candidates,
    const std::uint64_t expires_at,
    const EligibilityHistory history,
    const bool feedback_origin,
    const std::uint32_t context_signature,
    const std::uint64_t context_generation,
    const bool context_ambiguous,
    const bool context_feedback_origin,
    const std::uint8_t strength) {
    require_neuron(id);
    if (eligibility_candidate_capacity_ == 0 || eligibility_decision_capacity_ == 0) {
        return {};
    }

    std::vector<SynapseId> candidates = input_candidates;
    if (candidates.size() > eligibility_candidate_capacity_) {
        candidates.erase(
            candidates.begin(), candidates.end() - eligibility_candidate_capacity_);
    }
    for (const SynapseId candidate : candidates) {
        require_synapse(candidate);
        if (synapses_[candidate].receiver != id) {
            throw std::invalid_argument("eligibility candidate has wrong receiver");
        }
    }

    const std::size_t record_base =
        static_cast<std::size_t>(id) * eligibility_decision_capacity_;
    EligibilityStoreResult result;
    if (history == EligibilityHistory::merge_unique) {
        std::vector<SynapseId> merged;
        for (std::uint32_t slot = 0; slot < eligibility_decision_capacity_; ++slot) {
            const std::size_t record = record_base + slot;
            if (eligibility_record_generations_[record] == 0) {
                continue;
            }
            ++result.overwritten_decisions;
            const std::size_t candidate_base = record * eligibility_candidate_capacity_;
            for (std::uint32_t index = 0; index < eligibility_sizes_[record]; ++index) {
                const SynapseId candidate = eligibility_slots_[candidate_base + index];
                if (std::find(merged.begin(), merged.end(), candidate) == merged.end()) {
                    merged.push_back(candidate);
                }
            }
            eligibility_record_generations_[record] = 0;
            eligibility_sizes_[record] = 0;
            eligibility_expiry_times_[record] = 0;
            eligibility_feedback_origins_[record] = 0;
            eligibility_context_signatures_[record] = 0;
            eligibility_context_generations_[record] = 0;
            eligibility_context_ambiguities_[record] = 0;
            eligibility_context_feedback_origins_[record] = 0;
            eligibility_strengths_[record] = 0;
        }
        for (const SynapseId candidate : candidates) {
            const auto found = std::find(merged.begin(), merged.end(), candidate);
            if (found != merged.end()) {
                merged.erase(found);
            }
            merged.push_back(candidate);
        }
        if (merged.size() > eligibility_candidate_capacity_) {
            merged.erase(
                merged.begin(), merged.end() - eligibility_candidate_capacity_);
        }
        candidates = std::move(merged);
    } else if (history == EligibilityHistory::overwrite) {
        for (std::uint32_t slot = 0; slot < eligibility_decision_capacity_; ++slot) {
            const std::size_t record = record_base + slot;
            if (eligibility_record_generations_[record] != 0) {
                ++result.overwritten_decisions;
            }
            eligibility_record_generations_[record] = 0;
            eligibility_sizes_[record] = 0;
            eligibility_expiry_times_[record] = 0;
            eligibility_feedback_origins_[record] = 0;
            eligibility_context_signatures_[record] = 0;
            eligibility_context_generations_[record] = 0;
            eligibility_context_ambiguities_[record] = 0;
            eligibility_context_feedback_origins_[record] = 0;
            eligibility_strengths_[record] = 0;
        }
    }

    std::uint32_t selected = history == EligibilityHistory::retain
                                 ? eligibility_write_positions_[id]
                                 : 0U;
    if (history == EligibilityHistory::retain) {
        bool found_empty = false;
        for (std::uint32_t offset = 0; offset < eligibility_decision_capacity_; ++offset) {
            const std::uint32_t probe =
                (eligibility_write_positions_[id] + offset) % eligibility_decision_capacity_;
            if (eligibility_record_generations_[record_base + probe] == 0) {
                selected = probe;
                found_empty = true;
                break;
            }
        }
        if (!found_empty) {
            ++result.overwritten_decisions;
        }
    }

    std::uint64_t generation = ++eligibility_generations_[id];
    if (generation == 0) {
        generation = ++eligibility_generations_[id];
    }
    const std::size_t record = record_base + selected;
    const std::size_t candidate_base = record * eligibility_candidate_capacity_;
    std::fill_n(
        eligibility_slots_.begin() + static_cast<std::ptrdiff_t>(candidate_base),
        eligibility_candidate_capacity_, kInvalidIndex);
    std::copy(candidates.begin(), candidates.end(),
              eligibility_slots_.begin() + static_cast<std::ptrdiff_t>(candidate_base));
    eligibility_sizes_[record] = static_cast<std::uint32_t>(candidates.size());
    eligibility_record_generations_[record] = generation;
    eligibility_expiry_times_[record] = expires_at;
    eligibility_feedback_origins_[record] = feedback_origin ? 1U : 0U;
    eligibility_context_signatures_[record] = context_signature;
    eligibility_context_generations_[record] = context_generation;
    eligibility_context_ambiguities_[record] = context_ambiguous ? 1U : 0U;
    eligibility_context_feedback_origins_[record] =
        context_feedback_origin ? 1U : 0U;
    eligibility_strengths_[record] = strength == 0 ? 1 : strength;
    eligibility_write_positions_[id] = (selected + 1U) % eligibility_decision_capacity_;
    eligibility_overwrites_[id] += result.overwritten_decisions;
    result.record = EligibilityRecord{
        generation, expires_at, feedback_origin, context_signature,
        context_generation, context_ambiguous, context_feedback_origin,
        eligibility_strengths_[record],
        std::move(candidates)};
    return result;
}

bool Network::expire_eligibility(const NeuronId id, const std::uint64_t generation) {
    require_neuron(id);
    if (eligibility_decision_capacity_ == 0 || generation == 0) {
        return false;
    }
    const std::size_t base = static_cast<std::size_t>(id) * eligibility_decision_capacity_;
    for (std::uint32_t slot = 0; slot < eligibility_decision_capacity_; ++slot) {
        const std::size_t record = base + slot;
        if (eligibility_record_generations_[record] == generation) {
            eligibility_record_generations_[record] = 0;
            eligibility_sizes_[record] = 0;
            eligibility_expiry_times_[record] = 0;
            eligibility_feedback_origins_[record] = 0;
            eligibility_context_signatures_[record] = 0;
            eligibility_context_generations_[record] = 0;
            eligibility_context_ambiguities_[record] = 0;
            eligibility_context_feedback_origins_[record] = 0;
            eligibility_strengths_[record] = 0;
            return true;
        }
    }
    return false;
}

std::uint8_t Network::decay_eligibility(
    const NeuronId id, const std::uint64_t generation) {
    require_neuron(id);
    if (eligibility_decision_capacity_ == 0 || generation == 0) return 0;
    const std::size_t base = static_cast<std::size_t>(id) * eligibility_decision_capacity_;
    for (std::uint32_t slot = 0; slot < eligibility_decision_capacity_; ++slot) {
        const std::size_t record = base + slot;
        if (eligibility_record_generations_[record] != generation) continue;
        auto& strength = eligibility_strengths_[record];
        if (strength >= 4) strength = 2;
        else if (strength == 2) strength = 1;
        else {
            static_cast<void>(expire_eligibility(id, generation));
            return 0;
        }
        return strength;
    }
    return 0;
}

std::vector<EligibilityRecord> Network::eligibility_records(const NeuronId id) const {
    require_neuron(id);
    std::vector<EligibilityRecord> result;
    if (eligibility_decision_capacity_ == 0) {
        return result;
    }
    const std::size_t base = static_cast<std::size_t>(id) * eligibility_decision_capacity_;
    for (std::uint32_t slot = 0; slot < eligibility_decision_capacity_; ++slot) {
        const std::size_t record = base + slot;
        const std::uint64_t generation = eligibility_record_generations_[record];
        if (generation == 0) {
            continue;
        }
        EligibilityRecord item;
        item.generation = generation;
        item.expires_at = eligibility_expiry_times_[record];
        item.feedback_origin = eligibility_feedback_origins_[record] != 0;
        item.context_signature = eligibility_context_signatures_[record];
        item.context_generation = eligibility_context_generations_[record];
        item.context_ambiguous = eligibility_context_ambiguities_[record] != 0;
        item.context_feedback_origin =
            eligibility_context_feedback_origins_[record] != 0;
        item.strength = eligibility_strengths_[record];
        const std::size_t candidate_base = record * eligibility_candidate_capacity_;
        item.candidates.assign(
            eligibility_slots_.begin() + static_cast<std::ptrdiff_t>(candidate_base),
            eligibility_slots_.begin() + static_cast<std::ptrdiff_t>(
                candidate_base + eligibility_sizes_[record]));
        result.push_back(std::move(item));
    }
    std::sort(result.begin(), result.end(), [](const auto& left, const auto& right) {
        return left.generation < right.generation;
    });
    return result;
}

std::uint64_t Network::eligibility_generation(const NeuronId id) const {
    require_neuron(id);
    return eligibility_generations_.empty() ? 0 : eligibility_generations_[id];
}

std::uint64_t Network::eligibility_overwrites(const NeuronId id) const {
    require_neuron(id);
    return eligibility_overwrites_.empty() ? 0 : eligibility_overwrites_[id];
}

std::uint32_t Network::eligibility_candidate_capacity() const noexcept {
    return eligibility_candidate_capacity_;
}

std::uint32_t Network::eligibility_decision_capacity() const noexcept {
    return eligibility_decision_capacity_;
}

std::size_t Network::active_eligibility_count() const noexcept {
    return static_cast<std::size_t>(std::count_if(
        eligibility_record_generations_.begin(), eligibility_record_generations_.end(),
        [](const std::uint64_t generation) { return generation != 0; }));
}

void Network::restore_eligibility(
    const std::uint32_t candidate_capacity,
    const std::uint32_t decision_capacity,
    const std::vector<std::vector<EligibilityRecord>>& records,
    const std::vector<std::uint64_t>& generations,
    const std::vector<std::uint64_t>& overwrites) {
    if (records.size() != neurons_.size() || generations.size() != neurons_.size() ||
        overwrites.size() != neurons_.size()) {
        throw std::runtime_error("eligibility state count does not match neurons");
    }
    eligibility_candidate_capacity_ = candidate_capacity;
    eligibility_decision_capacity_ = decision_capacity;
    const std::size_t record_count = neurons_.size() * decision_capacity;
    const std::size_t candidate_count = record_count * candidate_capacity;
    if ((decision_capacity != 0 && record_count / decision_capacity != neurons_.size()) ||
        (candidate_capacity != 0 && candidate_count / candidate_capacity != record_count)) {
        throw std::runtime_error("restored eligibility storage size overflow");
    }
    eligibility_slots_.assign(candidate_count, kInvalidIndex);
    eligibility_sizes_.assign(record_count, 0);
    eligibility_record_generations_.assign(record_count, 0);
    eligibility_expiry_times_.assign(record_count, 0);
    eligibility_feedback_origins_.assign(record_count, 0);
    eligibility_context_signatures_.assign(record_count, 0);
    eligibility_context_generations_.assign(record_count, 0);
    eligibility_context_ambiguities_.assign(record_count, 0);
    eligibility_context_feedback_origins_.assign(record_count, 0);
    eligibility_strengths_.assign(record_count, 0);
    if (candidate_capacity == 0 || decision_capacity == 0) {
        for (std::size_t id = 0; id < records.size(); ++id) {
            if (!records[id].empty() || generations[id] != 0 || overwrites[id] != 0) {
                throw std::runtime_error("disabled eligibility storage contains state");
            }
        }
        eligibility_write_positions_.clear();
        eligibility_generations_.clear();
        eligibility_overwrites_.clear();
        return;
    }
    eligibility_write_positions_.assign(neurons_.size(), 0);
    eligibility_generations_ = generations;
    eligibility_overwrites_ = overwrites;
    for (NeuronId id = 0; id < neurons_.size(); ++id) {
        if (records[id].size() > decision_capacity) {
            throw std::runtime_error("restored eligibility exceeds decision capacity");
        }
        std::uint32_t slot = 0;
        for (const EligibilityRecord& item : records[id]) {
            if (item.generation == 0 || item.generation > generations[id] ||
                item.candidates.size() > candidate_capacity) {
                throw std::runtime_error("invalid restored eligibility record");
            }
            const std::size_t record = static_cast<std::size_t>(id) * decision_capacity + slot;
            eligibility_record_generations_[record] = item.generation;
            eligibility_expiry_times_[record] = item.expires_at;
            eligibility_feedback_origins_[record] = item.feedback_origin ? 1U : 0U;
            eligibility_context_signatures_[record] = item.context_signature;
            eligibility_context_generations_[record] = item.context_generation;
            eligibility_context_ambiguities_[record] =
                item.context_ambiguous ? 1U : 0U;
            eligibility_context_feedback_origins_[record] =
                item.context_feedback_origin ? 1U : 0U;
            eligibility_strengths_[record] = item.strength == 0 ? 1 : item.strength;
            eligibility_sizes_[record] = static_cast<std::uint32_t>(item.candidates.size());
            const std::size_t base = record * candidate_capacity;
            for (std::size_t index = 0; index < item.candidates.size(); ++index) {
                require_synapse(item.candidates[index]);
                if (synapses_[item.candidates[index]].receiver != id) {
                    throw std::runtime_error("restored eligibility has wrong receiver");
                }
                eligibility_slots_[base + index] = item.candidates[index];
            }
            ++slot;
        }
        eligibility_write_positions_[id] = slot % decision_capacity;
    }
}

ContextStoreResult Network::store_context_trace(
    const NeuronId id,
    const std::uint32_t signature,
    const std::uint64_t expires_at,
    const bool feedback_origin) {
    require_neuron(id);
    if (context_trace_capacity_ == 0 || signature == 0) {
        return {};
    }
    const std::size_t base = static_cast<std::size_t>(id) * context_trace_capacity_;
    std::uint32_t selected = context_trace_write_positions_[id];
    bool found_empty = false;
    for (std::uint32_t offset = 0; offset < context_trace_capacity_; ++offset) {
        const std::uint32_t probe =
            (context_trace_write_positions_[id] + offset) % context_trace_capacity_;
        if (context_trace_record_generations_[base + probe] == 0) {
            selected = probe;
            found_empty = true;
            break;
        }
    }
    ContextStoreResult result;
    result.overwritten = !found_empty;
    if (result.overwritten) {
        ++context_trace_overwrites_[id];
    }
    std::uint64_t generation = ++context_trace_generations_[id];
    if (generation == 0) {
        generation = ++context_trace_generations_[id];
    }
    const std::size_t record = base + selected;
    context_trace_record_generations_[record] = generation;
    context_trace_signatures_[record] = signature;
    context_trace_expiry_times_[record] = expires_at;
    context_trace_feedback_origins_[record] = feedback_origin ? 1U : 0U;
    context_trace_write_positions_[id] = (selected + 1U) % context_trace_capacity_;
    result.trace = ContextTrace{generation, signature, expires_at, feedback_origin};
    return result;
}

bool Network::expire_context_trace(
    const NeuronId id, const std::uint64_t generation) {
    require_neuron(id);
    if (context_trace_capacity_ == 0 || generation == 0) {
        return false;
    }
    const std::size_t base = static_cast<std::size_t>(id) * context_trace_capacity_;
    for (std::uint32_t slot = 0; slot < context_trace_capacity_; ++slot) {
        const std::size_t record = base + slot;
        if (context_trace_record_generations_[record] == generation) {
            context_trace_record_generations_[record] = 0;
            context_trace_signatures_[record] = 0;
            context_trace_expiry_times_[record] = 0;
            context_trace_feedback_origins_[record] = 0;
            return true;
        }
    }
    return false;
}

std::vector<ContextTrace> Network::context_traces(const NeuronId id) const {
    require_neuron(id);
    std::vector<ContextTrace> result;
    if (context_trace_capacity_ == 0) {
        return result;
    }
    const std::size_t base = static_cast<std::size_t>(id) * context_trace_capacity_;
    for (std::uint32_t slot = 0; slot < context_trace_capacity_; ++slot) {
        const std::size_t record = base + slot;
        if (context_trace_record_generations_[record] == 0) {
            continue;
        }
        result.push_back(ContextTrace{
            context_trace_record_generations_[record],
            context_trace_signatures_[record],
            context_trace_expiry_times_[record],
            context_trace_feedback_origins_[record] != 0});
    }
    std::sort(result.begin(), result.end(), [](const auto& left, const auto& right) {
        return left.generation < right.generation;
    });
    return result;
}

ContextBindingResult Network::bind_context_to_eligibilities(
    const NeuronId id, const ContextTrace& trace) {
    require_neuron(id);
    ContextBindingResult result;
    if (eligibility_decision_capacity_ == 0 || trace.signature == 0) {
        return result;
    }
    const std::size_t base = static_cast<std::size_t>(id) *
                             eligibility_decision_capacity_;
    for (std::uint32_t slot = 0; slot < eligibility_decision_capacity_; ++slot) {
        const std::size_t record = base + slot;
        if (eligibility_record_generations_[record] == 0) {
            continue;
        }
        if (eligibility_context_signatures_[record] == 0) {
            eligibility_context_signatures_[record] = trace.signature;
            eligibility_context_generations_[record] = trace.generation;
            eligibility_context_feedback_origins_[record] =
                trace.feedback_origin ? 1U : 0U;
            ++result.bound;
        } else if (eligibility_context_signatures_[record] != trace.signature &&
                   eligibility_context_ambiguities_[record] == 0) {
            eligibility_context_ambiguities_[record] = 1U;
            ++result.ambiguous;
        }
    }
    return result;
}

std::uint32_t Network::context_trace_capacity() const noexcept {
    return context_trace_capacity_;
}

std::uint64_t Network::context_generation(const NeuronId id) const {
    require_neuron(id);
    return context_trace_generations_.empty() ? 0 : context_trace_generations_[id];
}

std::uint64_t Network::context_overwrites(const NeuronId id) const {
    require_neuron(id);
    return context_trace_overwrites_.empty() ? 0 : context_trace_overwrites_[id];
}

std::size_t Network::active_context_trace_count() const noexcept {
    return static_cast<std::size_t>(std::count_if(
        context_trace_record_generations_.begin(),
        context_trace_record_generations_.end(),
        [](const std::uint64_t generation) { return generation != 0; }));
}

void Network::restore_context_traces(
    const std::uint32_t capacity,
    const std::vector<std::vector<ContextTrace>>& traces,
    const std::vector<std::uint64_t>& generations,
    const std::vector<std::uint64_t>& overwrites) {
    if (traces.size() != neurons_.size() || generations.size() != neurons_.size() ||
        overwrites.size() != neurons_.size()) {
        throw std::runtime_error("context trace state count does not match neurons");
    }
    context_trace_capacity_ = capacity;
    const std::size_t count = neurons_.size() * capacity;
    context_trace_record_generations_.assign(count, 0);
    context_trace_signatures_.assign(count, 0);
    context_trace_expiry_times_.assign(count, 0);
    context_trace_feedback_origins_.assign(count, 0);
    if (capacity == 0) {
        for (std::size_t id = 0; id < traces.size(); ++id) {
            if (!traces[id].empty() || generations[id] != 0 || overwrites[id] != 0) {
                throw std::runtime_error("disabled context trace storage contains state");
            }
        }
        context_trace_write_positions_.clear();
        context_trace_generations_.clear();
        context_trace_overwrites_.clear();
        return;
    }
    context_trace_write_positions_.assign(neurons_.size(), 0);
    context_trace_generations_ = generations;
    context_trace_overwrites_ = overwrites;
    for (NeuronId id = 0; id < neurons_.size(); ++id) {
        if (traces[id].size() > capacity) {
            throw std::runtime_error("restored context traces exceed capacity");
        }
        std::uint32_t slot = 0;
        for (const auto& trace : traces[id]) {
            if (trace.generation == 0 || trace.generation > generations[id] ||
                trace.signature == 0) {
                throw std::runtime_error("invalid restored context trace");
            }
            const std::size_t record = static_cast<std::size_t>(id) * capacity + slot;
            context_trace_record_generations_[record] = trace.generation;
            context_trace_signatures_[record] = trace.signature;
            context_trace_expiry_times_[record] = trace.expires_at;
            context_trace_feedback_origins_[record] = trace.feedback_origin ? 1U : 0U;
            ++slot;
        }
        context_trace_write_positions_[id] = slot % capacity;
    }
}

const Neuron& Network::neuron(const NeuronId id) const {
    require_neuron(id);
    return neurons_[id];
}

Neuron& Network::neuron(const NeuronId id) {
    require_neuron(id);
    return neurons_[id];
}

const Synapse& Network::synapse(const SynapseId id) const {
    require_synapse(id);
    return synapses_[id];
}

Synapse& Network::synapse(const SynapseId id) {
    require_synapse(id);
    return synapses_[id];
}

std::uint32_t Network::neuron_count() const noexcept {
    return static_cast<std::uint32_t>(neurons_.size());
}

std::uint32_t Network::synapse_count() const noexcept {
    return static_cast<std::uint32_t>(synapses_.size());
}

std::size_t Network::estimated_memory_bytes() const noexcept {
    return sizeof(*this) + neurons_.capacity() * sizeof(Neuron) +
           synapses_.capacity() * sizeof(Synapse) +
           local_event_slots_.capacity() * sizeof(SynapseId) +
           local_event_write_positions_.capacity() * sizeof(std::uint32_t) +
           local_event_sizes_.capacity() * sizeof(std::uint32_t) +
           local_event_overwrites_.capacity() * sizeof(std::uint64_t) +
           eligibility_slots_.capacity() * sizeof(SynapseId) +
           eligibility_sizes_.capacity() * sizeof(std::uint32_t) +
           eligibility_record_generations_.capacity() * sizeof(std::uint64_t) +
           eligibility_expiry_times_.capacity() * sizeof(std::uint64_t) +
           eligibility_feedback_origins_.capacity() * sizeof(std::uint8_t) +
           eligibility_context_signatures_.capacity() * sizeof(std::uint32_t) +
           eligibility_context_generations_.capacity() * sizeof(std::uint64_t) +
           eligibility_context_ambiguities_.capacity() * sizeof(std::uint8_t) +
           eligibility_context_feedback_origins_.capacity() * sizeof(std::uint8_t) +
           eligibility_strengths_.capacity() * sizeof(std::uint8_t) +
           eligibility_write_positions_.capacity() * sizeof(std::uint32_t) +
           eligibility_generations_.capacity() * sizeof(std::uint64_t) +
           eligibility_overwrites_.capacity() * sizeof(std::uint64_t) +
           context_trace_record_generations_.capacity() * sizeof(std::uint64_t) +
           context_trace_signatures_.capacity() * sizeof(std::uint32_t) +
           context_trace_expiry_times_.capacity() * sizeof(std::uint64_t) +
           context_trace_feedback_origins_.capacity() * sizeof(std::uint8_t) +
           context_trace_write_positions_.capacity() * sizeof(std::uint32_t) +
           context_trace_generations_.capacity() * sizeof(std::uint64_t) +
           context_trace_overwrites_.capacity() * sizeof(std::uint64_t);
}

std::map<std::int32_t, std::uint64_t> Network::weight_distribution() const {
    std::map<std::int32_t, std::uint64_t> result;
    for (const Synapse& item : synapses_) {
        ++result[item.weight];
    }
    return result;
}

std::uint64_t Network::state_fingerprint() const noexcept {
    std::uint64_t hash = kFnvOffset;
    hash_u64(hash, width_);
    hash_u64(hash, height_);
    hash_u64(hash, depth_);
    hash_u64(hash, neurons_.size());
    for (const Neuron& item : neurons_) {
        hash_u64(hash, static_cast<std::uint64_t>(item.membrane_potential));
        hash_u64(hash, static_cast<std::uint64_t>(item.firing_threshold));
        hash_u64(hash, item.input_event_count);
        hash_u64(hash, item.refractory_events_remaining);
        hash_u64(hash, item.recovery_generation);
        hash_u64(hash, item.scheduled_recovery_time);
        hash_u64(hash, item.first_incoming);
        hash_u64(hash, item.first_outgoing);
        hash_u64(hash, item.incoming_count);
        hash_u64(hash, item.outgoing_count);
    }
    hash_u64(hash, synapses_.size());
    for (const Synapse& item : synapses_) {
        hash_u64(hash, item.sender);
        hash_u64(hash, item.receiver);
        hash_u64(hash, static_cast<std::uint32_t>(item.weight));
        hash_u64(hash, item.activity_count);
        hash_u64(hash, item.next_incoming);
        hash_u64(hash, item.next_outgoing);
        hash_u64(hash, item.plasticity_protected ? 1 : 0);
        hash_u64(hash, static_cast<std::uint8_t>(item.effect));
    }
    hash_u64(hash, local_event_memory_capacity_);
    for (NeuronId id = 0; id < neurons_.size(); ++id) {
        const auto events = local_event_memory(id);
        hash_u64(hash, events.size());
        for (const SynapseId synapse_id : events) {
            hash_u64(hash, synapse_id);
        }
        hash_u64(hash, local_event_overwrites(id));
    }
    hash_u64(hash, eligibility_candidate_capacity_);
    hash_u64(hash, eligibility_decision_capacity_);
    for (NeuronId id = 0; id < neurons_.size(); ++id) {
        hash_u64(hash, eligibility_generation(id));
        hash_u64(hash, eligibility_overwrites(id));
        const auto records = eligibility_records(id);
        hash_u64(hash, records.size());
        for (const auto& record : records) {
            hash_u64(hash, record.generation);
            hash_u64(hash, record.expires_at);
            hash_u64(hash, record.feedback_origin ? 1 : 0);
            hash_u64(hash, record.context_signature);
            hash_u64(hash, record.context_generation);
            hash_u64(hash, record.context_ambiguous ? 1 : 0);
            hash_u64(hash, record.context_feedback_origin ? 1 : 0);
            hash_u64(hash, record.strength);
            hash_u64(hash, record.candidates.size());
            for (const SynapseId candidate : record.candidates) {
                hash_u64(hash, candidate);
            }
        }
    }
    hash_u64(hash, context_trace_capacity_);
    for (NeuronId id = 0; id < neurons_.size(); ++id) {
        const auto traces = context_traces(id);
        hash_u64(hash, traces.size());
        for (const auto& trace : traces) {
            hash_u64(hash, trace.generation);
            hash_u64(hash, trace.signature);
            hash_u64(hash, trace.expires_at);
            hash_u64(hash, trace.feedback_origin ? 1 : 0);
        }
        hash_u64(hash, context_trace_generations_.empty()
                           ? 0 : context_trace_generations_[id]);
        hash_u64(hash, context_trace_overwrites_.empty()
                           ? 0 : context_trace_overwrites_[id]);
    }
    return hash;
}

bool Network::validate(std::string* error) const {
    const auto fail = [error](const std::string& message) {
        if (error != nullptr) {
            *error = message;
        }
        return false;
    };

    const std::uint64_t volume = static_cast<std::uint64_t>(width_) * height_ * depth_;
    if (volume != neurons_.size()) {
        return fail("geometry volume does not match neuron count");
    }
    if (neurons_.size() >= kInvalidIndex || synapses_.size() >= kInvalidIndex) {
        return fail("index space exhausted");
    }
    const std::size_t expected_slots =
        neurons_.size() * static_cast<std::size_t>(local_event_memory_capacity_);
    const std::size_t expected_management =
        local_event_memory_capacity_ == 0 ? 0 : neurons_.size();
    if (local_event_slots_.size() != expected_slots ||
        local_event_write_positions_.size() != expected_management ||
        local_event_sizes_.size() != expected_management ||
        local_event_overwrites_.size() != expected_management) {
        return fail("local event memory storage mismatch");
    }
    if (local_event_memory_capacity_ != 0) {
        for (std::size_t index = 0; index < neurons_.size(); ++index) {
            if (local_event_sizes_[index] > local_event_memory_capacity_ ||
                local_event_write_positions_[index] >= local_event_memory_capacity_) {
                return fail("invalid local event memory cursor");
            }
        }
    }
    const std::size_t expected_eligibility_records =
        neurons_.size() * static_cast<std::size_t>(eligibility_decision_capacity_);
    const std::size_t expected_eligibility_candidates =
        expected_eligibility_records * eligibility_candidate_capacity_;
    const std::size_t expected_eligibility_management =
        eligibility_candidate_capacity_ == 0 || eligibility_decision_capacity_ == 0
            ? 0
            : neurons_.size();
    if (eligibility_slots_.size() != expected_eligibility_candidates ||
        eligibility_sizes_.size() != expected_eligibility_records ||
        eligibility_record_generations_.size() != expected_eligibility_records ||
        eligibility_expiry_times_.size() != expected_eligibility_records ||
        eligibility_feedback_origins_.size() != expected_eligibility_records ||
        eligibility_context_signatures_.size() != expected_eligibility_records ||
        eligibility_context_generations_.size() != expected_eligibility_records ||
        eligibility_context_ambiguities_.size() != expected_eligibility_records ||
        eligibility_context_feedback_origins_.size() !=
            expected_eligibility_records ||
        eligibility_strengths_.size() != expected_eligibility_records ||
        eligibility_write_positions_.size() != expected_eligibility_management ||
        eligibility_generations_.size() != expected_eligibility_management ||
        eligibility_overwrites_.size() != expected_eligibility_management) {
        return fail("eligibility storage mismatch");
    }
    for (std::size_t index = 0; index < expected_eligibility_records; ++index) {
        if (eligibility_sizes_[index] > eligibility_candidate_capacity_ ||
            (eligibility_record_generations_[index] == 0 &&
             (eligibility_sizes_[index] != 0 || eligibility_expiry_times_[index] != 0 ||
              eligibility_feedback_origins_[index] != 0 ||
              eligibility_context_signatures_[index] != 0 ||
              eligibility_context_generations_[index] != 0 ||
              eligibility_context_ambiguities_[index] != 0 ||
              eligibility_context_feedback_origins_[index] != 0 ||
              eligibility_strengths_[index] != 0))) {
            return fail("invalid eligibility record");
        }
        if (eligibility_record_generations_[index] != 0) {
            if (eligibility_strengths_[index] == 0) {
                return fail("active eligibility has zero strength");
            }
            const std::size_t neuron_index =
                index / eligibility_decision_capacity_;
            if (eligibility_record_generations_[index] >
                eligibility_generations_[neuron_index]) {
                return fail("eligibility record exceeds local generation");
            }
            const std::size_t candidate_base =
                index * eligibility_candidate_capacity_;
            for (std::uint32_t candidate = 0;
                 candidate < eligibility_sizes_[index]; ++candidate) {
                const SynapseId synapse_id =
                    eligibility_slots_[candidate_base + candidate];
                if (synapse_id >= synapses_.size() ||
                    synapses_[synapse_id].receiver != neuron_index) {
                    return fail("eligibility candidate has invalid receiver");
                }
            }
        }
    }
    const std::size_t expected_context_records =
        neurons_.size() * static_cast<std::size_t>(context_trace_capacity_);
    const std::size_t expected_context_management =
        context_trace_capacity_ == 0 ? 0 : neurons_.size();
    if (context_trace_record_generations_.size() != expected_context_records ||
        context_trace_signatures_.size() != expected_context_records ||
        context_trace_expiry_times_.size() != expected_context_records ||
        context_trace_feedback_origins_.size() != expected_context_records ||
        context_trace_write_positions_.size() != expected_context_management ||
        context_trace_generations_.size() != expected_context_management ||
        context_trace_overwrites_.size() != expected_context_management) {
        return fail("context trace storage mismatch");
    }
    for (std::size_t index = 0; index < expected_context_records; ++index) {
        if (context_trace_record_generations_[index] == 0 &&
            (context_trace_signatures_[index] != 0 ||
             context_trace_expiry_times_[index] != 0 ||
             context_trace_feedback_origins_[index] != 0)) {
            return fail("inactive context trace contains state");
        }
    }
    for (std::size_t index = 0; index < expected_context_management; ++index) {
        if (context_trace_write_positions_[index] >= context_trace_capacity_) {
            return fail("invalid context trace cursor");
        }
    }
    for (std::size_t index = 0; index < expected_eligibility_management; ++index) {
        if (eligibility_write_positions_[index] >= eligibility_decision_capacity_) {
            return fail("invalid eligibility write cursor");
        }
    }

    std::unordered_set<std::uint64_t> connections;
    connections.reserve(synapses_.size());
    for (std::size_t index = 0; index < synapses_.size(); ++index) {
        const Synapse& item = synapses_[index];
        if (item.sender >= neurons_.size() || item.receiver >= neurons_.size()) {
            return fail("synapse endpoint is outside the neuron array");
        }
        if (item.sender == item.receiver) {
            return fail("self-connection found");
        }
        if (!connections.insert(connection_key(item.sender, item.receiver)).second) {
            return fail("duplicate directed connection found");
        }
        if (item.next_incoming != kInvalidIndex && item.next_incoming >= synapses_.size()) {
            return fail("invalid incoming link");
        }
        if (item.next_outgoing != kInvalidIndex && item.next_outgoing >= synapses_.size()) {
            return fail("invalid outgoing link");
        }
        if (static_cast<std::uint8_t>(item.effect) >
            static_cast<std::uint8_t>(SynapticEffect::inhibitory)) {
            return fail("invalid synaptic effect");
        }
    }

    for (std::size_t neuron_index = 0; neuron_index < neurons_.size(); ++neuron_index) {
        const Neuron& item = neurons_[neuron_index];
        std::uint32_t incoming = 0;
        SynapseId current = item.first_incoming;
        while (current != kInvalidIndex) {
            if (current >= synapses_.size()) {
                return fail("invalid incoming head or link");
            }
            if (incoming++ > synapses_.size()) {
                return fail("cycle in incoming links");
            }
            if (synapses_[current].receiver != neuron_index) {
                return fail("incoming link points to the wrong neuron");
            }
            current = synapses_[current].next_incoming;
        }
        if (incoming != item.incoming_count) {
            return fail("incoming count mismatch");
        }

        std::uint32_t outgoing = 0;
        current = item.first_outgoing;
        while (current != kInvalidIndex) {
            if (current >= synapses_.size()) {
                return fail("invalid outgoing head or link");
            }
            if (outgoing++ > synapses_.size()) {
                return fail("cycle in outgoing links");
            }
            if (synapses_[current].sender != neuron_index) {
                return fail("outgoing link points to the wrong neuron");
            }
            current = synapses_[current].next_outgoing;
        }
        if (outgoing != item.outgoing_count) {
            return fail("outgoing count mismatch");
        }
    }
    return true;
}

std::uint32_t Network::width() const noexcept {
    return width_;
}

std::uint32_t Network::height() const noexcept {
    return height_;
}

std::uint32_t Network::depth() const noexcept {
    return depth_;
}

const std::vector<Neuron>& Network::neurons() const noexcept {
    return neurons_;
}

const std::vector<Synapse>& Network::synapses() const noexcept {
    return synapses_;
}

void Network::restore(
    const std::uint32_t width,
    const std::uint32_t height,
    const std::uint32_t depth,
    std::vector<Neuron> neurons,
    std::vector<Synapse> synapses) {
    width_ = width;
    height_ = height;
    depth_ = depth;
    neurons_ = std::move(neurons);
    synapses_ = std::move(synapses);
    local_event_memory_capacity_ = 0;
    local_event_slots_.clear();
    local_event_write_positions_.clear();
    local_event_sizes_.clear();
    local_event_overwrites_.clear();
    eligibility_candidate_capacity_ = 0;
    eligibility_decision_capacity_ = 0;
    eligibility_slots_.clear();
    eligibility_sizes_.clear();
    eligibility_record_generations_.clear();
    eligibility_expiry_times_.clear();
    eligibility_feedback_origins_.clear();
    eligibility_context_signatures_.clear();
    eligibility_context_generations_.clear();
    eligibility_context_ambiguities_.clear();
    eligibility_context_feedback_origins_.clear();
    eligibility_strengths_.clear();
    eligibility_write_positions_.clear();
    eligibility_generations_.clear();
    eligibility_overwrites_.clear();
    context_trace_capacity_ = 0;
    context_trace_record_generations_.clear();
    context_trace_signatures_.clear();
    context_trace_expiry_times_.clear();
    context_trace_feedback_origins_.clear();
    context_trace_write_positions_.clear();
    context_trace_generations_.clear();
    context_trace_overwrites_.clear();
    std::string error;
    if (!validate(&error)) {
        throw std::runtime_error("invalid restored network: " + error);
    }
}

void Network::require_neuron(const NeuronId id) const {
    if (id >= neurons_.size()) {
        throw std::out_of_range("neuron index out of range");
    }
}

void Network::require_synapse(const SynapseId id) const {
    if (id >= synapses_.size()) {
        throw std::out_of_range("synapse index out of range");
    }
}

}  // namespace eve::neural
