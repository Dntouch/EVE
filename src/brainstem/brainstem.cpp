#include "eve/brainstem/brainstem.hpp"

#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

namespace eve::brainstem {
namespace {

std::uint32_t absolute_difference(const std::uint32_t left, const std::uint32_t right) {
    return left > right ? left - right : right - left;
}

std::uint32_t chebyshev_distance(const Coordinate left, const Coordinate right) {
    return std::max({
        absolute_difference(left.x, right.x),
        absolute_difference(left.y, right.y),
        absolute_difference(left.z, right.z),
    });
}

std::uint64_t next_assignment_random(std::uint64_t& state) noexcept {
    state ^= state >> 12U;
    state ^= state << 25U;
    state ^= state >> 27U;
    return state * 2685821657736338717ULL;
}

}  // namespace

void MotorOutput::configure(
    const std::uint32_t neuron_count,
    const MotorOutputConfig& config) {
    if (config.contacts_per_channel == 0) {
        throw std::invalid_argument("motor contacts per channel must be positive");
    }
    if (config.response_stimulus_neuron < 8 ||
        config.response_stimulus_neuron >= neuron_count) {
        throw std::invalid_argument(
            "response stimulus must be independent of the eight sensory channels");
    }

    const std::uint64_t required =
        static_cast<std::uint64_t>(config.contacts_per_channel) * 8U + 1U;
    std::vector<NeuronId> candidates;
    candidates.reserve(neuron_count);
    for (NeuronId neuron = 8; neuron < neuron_count; ++neuron) {
        if (neuron != config.response_stimulus_neuron) {
            candidates.push_back(neuron);
        }
    }
    if (required > candidates.size()) {
        throw std::invalid_argument("network is too small for non-overlapping motor contacts");
    }

    std::uint64_t random_state = config.assignment_seed == 0
                                     ? 0x9e3779b97f4a7c15ULL
                                     : config.assignment_seed;
    for (std::size_t remaining = candidates.size(); remaining > 1; --remaining) {
        const std::size_t selected = static_cast<std::size_t>(
            next_assignment_random(random_state) % remaining);
        std::swap(candidates[remaining - 1], candidates[selected]);
    }

    assignment_ = {};
    assignment_.config = config;
    roles_.assign(neuron_count, static_cast<std::int8_t>(-2));
    std::size_t candidate_index = 0;
    for (std::size_t channel = 0; channel < assignment_.channels.size(); ++channel) {
        auto& contacts = assignment_.channels[channel];
        contacts.reserve(config.contacts_per_channel);
        for (std::uint32_t contact = 0; contact < config.contacts_per_channel; ++contact) {
            const NeuronId neuron = candidates[candidate_index++];
            contacts.push_back(neuron);
            roles_[neuron] = static_cast<std::int8_t>(channel);
        }
    }
    assignment_.completion_contact = candidates[candidate_index];
    roles_[assignment_.completion_contact] = -1;
    response_ = {};
    configured_ = true;
    active_ = false;
}

void MotorOutput::begin_response() {
    if (!configured_) {
        throw std::logic_error("motor output is not configured");
    }
    if (active_) {
        throw std::logic_error("a motor response is already active");
    }
    response_ = {};
    active_ = true;
}

void MotorOutput::observe_fire(
    const NeuronId neuron,
    const std::uint64_t event_index) {
    if (!active_ || neuron >= roles_.size()) {
        return;
    }
    const std::int8_t role = roles_[neuron];
    if (role == -2) {
        return;
    }
    response_.activations.push_back(MotorActivation{event_index, neuron, role});
    if (role >= 0) {
        const auto channel = static_cast<std::size_t>(role);
        response_.activated_channels[channel] = true;
        response_.value |= static_cast<std::uint8_t>(1U << channel);
        return;
    }
    response_.status = MotorResponseStatus::completed;
    active_ = false;
}

void MotorOutput::finish_without_completion() {
    if (!active_) {
        return;
    }
    response_.status = MotorResponseStatus::no_response;
    active_ = false;
}

void MotorOutput::abort() {
    if (!active_) {
        return;
    }
    response_.status = MotorResponseStatus::aborted;
    active_ = false;
}

bool MotorOutput::configured() const noexcept {
    return configured_;
}

bool MotorOutput::active() const noexcept {
    return active_;
}

const MotorAssignment& MotorOutput::assignment() const {
    if (!configured_) {
        throw std::logic_error("motor output is not configured");
    }
    return assignment_;
}

const MotorResponse& MotorOutput::response() const noexcept {
    return response_;
}

std::size_t MotorOutput::estimated_memory_bytes() const noexcept {
    std::size_t bytes = roles_.capacity() * sizeof(std::int8_t);
    for (const auto& contacts : assignment_.channels) {
        bytes += contacts.capacity() * sizeof(NeuronId);
    }
    bytes += response_.activations.capacity() * sizeof(MotorActivation);
    return bytes;
}

void Brainstem::initialize_network(
    neural::Network& network,
    const ExperimentConfig& config,
    bios::DeterministicRng& rng) const {
    network.reset(config);
    for (NeuronId sender = 0; sender < config.neuron_count; ++sender) {
        while (network.neuron(sender).outgoing_count < config.initial_outgoing_synapses) {
            const NeuronId receiver = rng.bounded(config.neuron_count);
            if (receiver == sender || network.has_synapse(sender, receiver)) {
                continue;
            }
            static_cast<void>(
                network.add_synapse(sender, receiver, config.initial_synapse_weight));
        }
    }
}

ImpulseResult Brainstem::receive_impulse(
    neural::Network& network,
    const SynapseId synapse_id,
    const ExperimentConfig& config,
    bios::DeterministicRng* const rng) const {
    neural::Synapse& synapse = network.synapse(synapse_id);
    neural::Neuron& neuron = network.neuron(synapse.receiver);

    ++neuron.input_event_count;
    ++synapse.activity_count;

    ImpulseResult result;
    if (config.learning_enabled &&
        synapse.activity_count >= config.synaptic_learning_threshold) {
        if (synapse.weight < config.maximum_synapse_weight) {
            if (config.input_weight_budget ==
                std::numeric_limits<std::uint64_t>::max()) {
                ++synapse.weight;
                result.strengthened = true;
            } else {
                std::uint64_t positive_input_weight = 0;
                for (const SynapseId incoming : network.incoming_synapses(synapse.receiver)) {
                    const std::int32_t weight = network.synapse(incoming).weight;
                    if (weight > 0) {
                        positive_input_weight += static_cast<std::uint32_t>(weight);
                    }
                }
                if (positive_input_weight < config.input_weight_budget) {
                    ++synapse.weight;
                    result.strengthened = true;
                } else {
                    if (rng == nullptr) {
                        throw std::logic_error(
                            "input-weight competition requires a deterministic RNG");
                    }
                    const auto competitor =
                        select_competing_synapse(network, synapse_id, *rng);
                    if (competitor.has_value()) {
                        --network.synapse(*competitor).weight;
                        ++synapse.weight;
                        result.strengthened = true;
                        result.weakened = true;
                    }
                }
            }
        } else {
            result.strengthening_rejected_by_weight_limit = true;
        }
        synapse.activity_count = 0;
    }

    result.applied_weight = synapse.weight;
    const std::int64_t weight = result.applied_weight;
    if (weight > 0 && neuron.membrane_potential >
                          std::numeric_limits<std::int64_t>::max() - weight) {
        neuron.membrane_potential = std::numeric_limits<std::int64_t>::max();
    } else {
        neuron.membrane_potential += weight;
    }

    const bool refractory = neuron.refractory_events_remaining > 0;
    if (refractory) {
        --neuron.refractory_events_remaining;
    }
    if (!refractory && neuron.membrane_potential >= neuron.firing_threshold) {
        result.fired = true;
        neuron.refractory_events_remaining = config.refractory_input_events;
        if (config.reset_mode == ResetMode::to_zero) {
            neuron.membrane_potential = 0;
        } else {
            neuron.membrane_potential -= neuron.firing_threshold;
        }
    } else if (refractory && neuron.membrane_potential >= neuron.firing_threshold) {
        result.firing_suppressed = true;
    }

    if (neuron.input_event_count >= config.activity_phase_limit) {
        result.phase_completed = true;
        neuron.input_event_count = 0;
        network.reset_incoming_activity(synapse.receiver);
    }
    result.membrane_after = neuron.membrane_potential;
    return result;
}

std::optional<SynapseId> Brainstem::select_competing_synapse(
    const neural::Network& network,
    const SynapseId strengthened_synapse,
    bios::DeterministicRng& rng) const {
    const neural::Synapse& strengthened = network.synapse(strengthened_synapse);
    std::vector<SynapseId> candidates;
    for (const SynapseId incoming : network.incoming_synapses(strengthened.receiver)) {
        if (incoming != strengthened_synapse && network.synapse(incoming).weight > 0) {
            candidates.push_back(incoming);
        }
    }
    if (candidates.empty()) {
        return std::nullopt;
    }
    return candidates[rng.bounded(static_cast<std::uint32_t>(candidates.size()))];
}

bool Brainstem::should_attempt_growth(
    const std::uint32_t outgoing_count,
    bios::DeterministicRng& rng) const {
    if (outgoing_count == std::numeric_limits<std::uint32_t>::max()) {
        return false;
    }
    return rng.bounded(outgoing_count + 1U) == 0;
}

std::optional<SynapseId> Brainstem::attempt_growth(
    neural::Network& network,
    const NeuronId sender,
    const ExperimentConfig& config,
    bios::DeterministicRng& rng) const {
    const neural::Neuron& source = network.neuron(sender);
    if (!should_attempt_growth(source.outgoing_count, rng)) {
        return std::nullopt;
    }

    const Coordinate origin = network.coordinate(sender);
    const std::uint32_t maximum_radius =
        std::max({config.width, config.height, config.depth}) - 1U;
    for (std::uint32_t radius = 1; radius <= maximum_radius; ++radius) {
        std::vector<NeuronId> candidates;
        for (NeuronId candidate = 0; candidate < network.neuron_count(); ++candidate) {
            if (candidate == sender || network.has_synapse(sender, candidate)) {
                continue;
            }
            if (chebyshev_distance(origin, network.coordinate(candidate)) == radius) {
                candidates.push_back(candidate);
            }
        }
        if (!candidates.empty()) {
            const NeuronId target = candidates[rng.bounded(
                static_cast<std::uint32_t>(candidates.size()))];
            return network.add_synapse(sender, target, 0);
        }
    }
    return std::nullopt;
}

std::vector<EncodedByte> Brainstem::encode_utf8_bytes(
    const std::vector<std::uint8_t>& bytes,
    const std::uint32_t neuron_count) const {
    if (neuron_count < 8) {
        throw std::invalid_argument("the sensory interface requires at least eight neurons");
    }

    std::vector<EncodedByte> encoded;
    encoded.reserve(bytes.size());
    for (std::size_t index = 0; index < bytes.size(); ++index) {
        EncodedByte item;
        item.byte_index = index;
        item.value = bytes[index];
        for (std::uint32_t bit = 0; bit < 8; ++bit) {
            if ((item.value & static_cast<std::uint8_t>(1U << bit)) != 0) {
                item.fired_channels.push_back(bit);
            }
        }
        encoded.push_back(std::move(item));
    }
    return encoded;
}

}  // namespace eve::brainstem
