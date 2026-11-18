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
           synapses_.capacity() * sizeof(Synapse);
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
