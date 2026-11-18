#include "eve/neural/network.hpp"

#include <limits>
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
           next_incoming == other.next_incoming && next_outgoing == other.next_outgoing;
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
}

SynapseId Network::add_synapse(
    const NeuronId sender,
    const NeuronId receiver,
    const std::int32_t weight) {
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
           local_event_overwrites_.capacity() * sizeof(std::uint64_t);
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
