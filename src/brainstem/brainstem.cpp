#include "eve/brainstem/brainstem.hpp"

#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <queue>
#include <unordered_set>
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
    if (config.first_motor_neuron <= config.response_stimulus_neuron ||
        config.first_motor_neuron >= neuron_count) {
        throw std::invalid_argument("first motor neuron must follow sensory control channels");
    }

    const std::uint64_t required =
        static_cast<std::uint64_t>(config.contacts_per_channel) * 8U + 1U;
    std::vector<NeuronId> candidates;
    candidates.reserve(neuron_count);
    for (NeuronId neuron = config.first_motor_neuron; neuron < neuron_count; ++neuron) {
        candidates.push_back(neuron);
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

    ImpulseResult result;
    result.local_event_recorded = network.local_event_memory_capacity() != 0;
    result.local_event_overwritten =
        network.record_local_event(synapse.receiver, synapse_id);

    ++neuron.input_event_count;
    ++synapse.activity_count;

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
    if (refractory && config.recovery_mode == RecoveryMode::input_dependent) {
        --neuron.refractory_events_remaining;
    }
    if (!refractory && neuron.membrane_potential >= neuron.firing_threshold) {
        result.fired = true;
        neuron.refractory_events_remaining = config.refractory_input_events;
        if (config.recovery_mode != RecoveryMode::input_dependent) {
            ++neuron.recovery_generation;
            result.recovery_generation = neuron.recovery_generation;
        }
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

RecoveryResult Brainstem::recover_neuron(
    neural::Network& network,
    const NeuronId neuron_id,
    const std::uint64_t generation,
    const ExperimentConfig& config) const {
    neural::Neuron& neuron = network.neuron(neuron_id);
    RecoveryResult result;
    result.membrane_before = neuron.membrane_potential;
    result.refractory_before = neuron.refractory_events_remaining;
    if (config.recovery_mode == RecoveryMode::input_dependent ||
        generation != neuron.recovery_generation ||
        neuron.refractory_events_remaining == 0) {
        result.stale = true;
        result.membrane_after = neuron.membrane_potential;
        result.refractory_after = neuron.refractory_events_remaining;
        return result;
    }

    neuron.refractory_events_remaining = 0;
    neuron.scheduled_recovery_time = 0;
    if (config.recovery_mode ==
        RecoveryMode::independent_with_membrane_relaxation) {
        const std::int64_t retained_percent =
            100 - static_cast<std::int64_t>(config.membrane_relaxation_percent);
        neuron.membrane_potential =
            neuron.membrane_potential * retained_percent / 100;
    }
    result.recovered = true;
    result.membrane_after = neuron.membrane_potential;
    result.refractory_after = neuron.refractory_events_remaining;
    return result;
}

EligibilityResult Brainstem::create_eligibility(
    neural::Network& network,
    const NeuronId neuron,
    const std::uint64_t expires_at,
    const ExperimentConfig& config,
    const bool feedback_origin) const {
    EligibilityResult result;
    if (config.eligibility_selection == EligibilitySelection::none ||
        config.eligibility_candidate_capacity == 0 ||
        config.eligibility_decision_capacity == 0) {
        return result;
    }

    const auto memory = network.local_event_memory(neuron);
    std::vector<SynapseId> selected;
    selected.reserve(std::min<std::size_t>(
        memory.size(), config.eligibility_candidate_capacity));
    std::unordered_set<SynapseId> seen;
    for (auto iterator = memory.rbegin(); iterator != memory.rend(); ++iterator) {
        const SynapseId candidate = *iterator;
        const auto& synapse = network.synapse(candidate);
        if (config.eligibility_selection == EligibilitySelection::positive_weight &&
            synapse.weight <= 0) {
            continue;
        }
        if (config.eligibility_representation == EligibilityRepresentation::unique &&
            !seen.insert(candidate).second) {
            continue;
        }
        selected.push_back(candidate);
        if (selected.size() == config.eligibility_candidate_capacity) {
            break;
        }
    }
    std::reverse(selected.begin(), selected.end());

    const auto stored = network.store_eligibility(
        neuron, selected, expires_at, config.eligibility_history, feedback_origin);
    result.created = stored.record.generation != 0;
    result.generation = stored.record.generation;
    result.expires_at = stored.record.expires_at;
    result.overwritten_decisions = stored.overwritten_decisions;
    result.candidates = stored.record.candidates;
    for (const SynapseId candidate : result.candidates) {
        if (network.synapse(candidate).weight > 0) {
            ++result.positive_candidates;
        } else {
            ++result.zero_weight_candidates;
        }
    }
    return result;
}

bool Brainstem::expire_eligibility(
    neural::Network& network,
    const NeuronId neuron,
    const std::uint64_t generation) const {
    return network.expire_eligibility(neuron, generation);
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

InnateWiringResult Brainstem::create_innate_wiring(
    neural::Network& network,
    const MotorAssignment& motor_assignment,
    const InnateWiringConfig& config) const {
    InnateWiringResult result;
    result.config = config;
    if (config.topology == InnateTopology::none) {
        return result;
    }
    if (config.topology == InnateTopology::structural_paths &&
        config.initial_weight != 0) {
        throw std::invalid_argument("structural innate paths must start at weight zero");
    }
    if ((config.topology == InnateTopology::functional_paths ||
         config.topology == InnateTopology::varied_functional_paths) &&
        config.initial_weight <= 0) {
        throw std::invalid_argument("functional innate paths require positive weight");
    }

    std::vector<NeuronId> sources;
    for (NeuronId neuron = 0; neuron < 8; ++neuron) {
        sources.push_back(neuron);
    }
    sources.push_back(motor_assignment.config.response_stimulus_neuron);

    std::vector<NeuronId> motor_targets;
    for (const auto& channel : motor_assignment.channels) {
        motor_targets.insert(motor_targets.end(), channel.begin(), channel.end());
    }
    motor_targets.push_back(motor_assignment.completion_contact);

    std::vector<bool> reserved(network.neuron_count(), false);
    for (const NeuronId neuron : sources) {
        reserved[neuron] = true;
    }
    for (const NeuronId neuron : motor_targets) {
        reserved[neuron] = true;
    }
    std::vector<NeuronId> relay_candidates;
    for (NeuronId neuron = 0; neuron < network.neuron_count(); ++neuron) {
        if (!reserved[neuron]) {
            relay_candidates.push_back(neuron);
        }
    }
    if (relay_candidates.size() < motor_targets.size() * 2U) {
        throw std::invalid_argument("network has too few neurons for innate motor paths");
    }

    std::uint64_t random_state = config.topology_seed == 0
                                     ? 0x9e3779b97f4a7c15ULL
                                     : config.topology_seed;
    const auto shuffle = [&random_state](std::vector<NeuronId>& values) {
        for (std::size_t remaining = values.size(); remaining > 1; --remaining) {
            const std::size_t selected = static_cast<std::size_t>(
                next_assignment_random(random_state) % remaining);
            std::swap(values[remaining - 1], values[selected]);
        }
    };
    shuffle(relay_candidates);
    shuffle(motor_targets);

    const auto connect = [&network, &result, &config](
                             const NeuronId sender,
                             const NeuronId receiver) {
        const auto existing = network.find_synapse(sender, receiver);
        if (existing.has_value()) {
            neural::Synapse& synapse = network.synapse(*existing);
            if (synapse.weight < config.initial_weight) {
                synapse.weight = config.initial_weight;
                ++result.adjusted_existing_synapses;
            }
            return;
        }
        static_cast<void>(network.add_synapse(sender, receiver, config.initial_weight));
        ++result.added_synapses;
    };

    result.paths.reserve(motor_targets.size());
    for (std::size_t index = 0; index < motor_targets.size(); ++index) {
        const InnatePath path{
            sources[static_cast<std::size_t>(
                next_assignment_random(random_state) % sources.size())],
            relay_candidates[index * 2U],
            relay_candidates[index * 2U + 1U],
            motor_targets[index],
        };
        connect(path.source, path.first_relay);
        connect(path.first_relay, path.second_relay);
        connect(path.second_relay, path.motor_target);
        result.paths.push_back(path);
    }

    const auto unreachable = std::numeric_limits<std::uint32_t>::max();
    std::vector<std::uint32_t> distance(network.neuron_count(), unreachable);
    std::queue<NeuronId> pending;
    for (const NeuronId source : sources) {
        distance[source] = 0;
        pending.push(source);
    }
    while (!pending.empty()) {
        const NeuronId sender = pending.front();
        pending.pop();
        for (const SynapseId synapse : network.outgoing_synapses(sender)) {
            const NeuronId receiver = network.synapse(synapse).receiver;
            if (distance[receiver] != unreachable) {
                continue;
            }
            distance[receiver] = distance[sender] + 1U;
            pending.push(receiver);
        }
    }
    for (const NeuronId target : motor_targets) {
        if (distance[target] == unreachable) {
            continue;
        }
        ++result.structurally_reachable_motor_contacts;
        result.maximum_shortest_path =
            std::max(result.maximum_shortest_path, distance[target]);
    }
    return result;
}

MotorReachability Brainstem::analyze_motor_reachability(
    const neural::Network& network,
    const MotorAssignment& motor_assignment) const {
    const auto unreachable = std::numeric_limits<std::uint32_t>::max();
    std::vector<std::uint32_t> distance(network.neuron_count(), unreachable);
    std::queue<NeuronId> pending;
    for (NeuronId source = 0; source < 8; ++source) {
        distance[source] = 0;
        pending.push(source);
    }
    const NeuronId response_source = motor_assignment.config.response_stimulus_neuron;
    distance[response_source] = 0;
    pending.push(response_source);
    while (!pending.empty()) {
        const NeuronId sender = pending.front();
        pending.pop();
        for (const SynapseId synapse : network.outgoing_synapses(sender)) {
            const NeuronId receiver = network.synapse(synapse).receiver;
            if (distance[receiver] != unreachable) {
                continue;
            }
            distance[receiver] = distance[sender] + 1U;
            pending.push(receiver);
        }
    }

    MotorReachability result;
    for (const auto& channel : motor_assignment.channels) {
        for (const NeuronId target : channel) {
            result.shortest_paths.push_back(distance[target]);
        }
    }
    result.shortest_paths.push_back(distance[motor_assignment.completion_contact]);
    for (const std::uint32_t path : result.shortest_paths) {
        if (path == unreachable) {
            continue;
        }
        ++result.reachable_contacts;
        result.maximum_shortest_path = std::max(result.maximum_shortest_path, path);
    }
    return result;
}

ExplorationResult Brainstem::select_exploration(
    const neural::Network& network,
    const ExplorationConfig& config,
    const std::vector<NeuronId>& reserved_neurons) const {
    ExplorationResult result;
    result.config = config;

    std::vector<bool> reserved(network.neuron_count(), false);
    const auto reserve = [&network, &reserved](const NeuronId neuron) {
        if (neuron >= network.neuron_count()) {
            throw std::invalid_argument("exploration reservation is outside the network");
        }
        reserved[neuron] = true;
    };
    if (config.target_policy == ExplorationTargetPolicy::processing_neurons_only) {
        for (const NeuronId neuron : reserved_neurons) {
            reserve(neuron);
        }
        for (const NeuronId neuron : config.additional_reserved_neurons) {
            reserve(neuron);
        }
    }
    result.reserved_neuron_count = static_cast<std::uint32_t>(
        std::count(reserved.begin(), reserved.end(), true));

    std::vector<NeuronId> candidates;
    candidates.reserve(network.neuron_count());
    for (NeuronId neuron = 0; neuron < network.neuron_count(); ++neuron) {
        if (reserved[neuron]) {
            continue;
        }
        if (config.mode == ExplorationMode::localized &&
            chebyshev_distance(network.coordinate(neuron), config.center) > config.radius) {
            continue;
        }
        candidates.push_back(neuron);
    }
    result.eligible_neuron_count = static_cast<std::uint32_t>(candidates.size());
    if (config.mode == ExplorationMode::none) {
        return result;
    }

    const std::uint32_t requested =
        config.mode == ExplorationMode::single ? 1U : config.activation_count;
    if (requested == 0) {
        throw std::invalid_argument("exploration requires at least one activation");
    }
    if (config.activation_budget == 0 || requested > config.activation_budget) {
        throw std::invalid_argument("exploration exceeds its activation budget");
    }
    if (requested > candidates.size()) {
        throw std::invalid_argument("exploration request exceeds available neurons");
    }

    std::uint64_t random_state =
        config.seed == 0 ? 0x9e3779b97f4a7c15ULL : config.seed;
    if (config.mode != ExplorationMode::distributed) {
        for (std::size_t remaining = candidates.size(); remaining > 1; --remaining) {
            const std::size_t selected = static_cast<std::size_t>(
                next_assignment_random(random_state) % remaining);
            std::swap(candidates[remaining - 1], candidates[selected]);
        }
        result.activated_neurons.assign(
            candidates.begin(), candidates.begin() + requested);
        return result;
    }

    std::array<std::vector<NeuronId>, 8> regions;
    for (const NeuronId neuron : candidates) {
        const Coordinate coordinate = network.coordinate(neuron);
        const std::size_t region =
            (coordinate.x >= network.width() / 2U ? 1U : 0U) |
            (coordinate.y >= network.height() / 2U ? 2U : 0U) |
            (coordinate.z >= network.depth() / 2U ? 4U : 0U);
        regions[region].push_back(neuron);
    }
    for (std::uint32_t index = 0; index < requested; ++index) {
        auto& region = regions[index % regions.size()];
        if (region.empty()) {
            throw std::logic_error("distributed exploration region is empty");
        }
        const std::size_t selected = static_cast<std::size_t>(
            next_assignment_random(random_state) % region.size());
        result.activated_neurons.push_back(region[selected]);
        region.erase(region.begin() + static_cast<std::ptrdiff_t>(selected));
    }
    return result;
}

MotorCircuitResult Brainstem::create_motor_circuit(
    neural::Network& network,
    const MotorAssignment& motor_assignment,
    const MotorCircuitConfig& config) const {
    MotorCircuitResult result;
    result.config = config;
    if (config.mode == MotorCircuitMode::none) {
        return result;
    }
    if (config.motor_weight <= 0 || config.completion_weight <= 0) {
        throw std::invalid_argument("motor circuit weights must be positive");
    }

    std::vector<NeuronId> data_contacts;
    for (const auto& channel : motor_assignment.channels) {
        data_contacts.insert(data_contacts.end(), channel.begin(), channel.end());
    }

    const auto connect = [&network, &result, &config](
                             const NeuronId sender,
                             const NeuronId receiver,
                             const std::int32_t weight) {
        const std::int32_t configured_weight = config.path_enabled ? weight : 0;
        const auto existing = network.find_synapse(sender, receiver);
        if (existing.has_value()) {
            neural::Synapse& synapse = network.synapse(*existing);
            if ((!config.path_enabled && synapse.weight != 0) ||
                (config.path_enabled && synapse.weight < configured_weight)) {
                synapse.weight = configured_weight;
                ++result.adjusted_existing_synapses;
            }
            result.circuit_synapses.push_back(*existing);
            return;
        }
        result.circuit_synapses.push_back(
            network.add_synapse(sender, receiver, configured_weight));
        ++result.added_synapses;
    };

    if (config.mode == MotorCircuitMode::direct_completion_path) {
        for (const NeuronId contact : data_contacts) {
            connect(contact, motor_assignment.completion_contact, config.completion_weight);
        }
        return result;
    }

    const std::uint32_t required_relays =
        config.mode == MotorCircuitMode::completion_group ? config.relay_count : 1U;
    if (required_relays == 0) {
        throw std::invalid_argument("motor completion group requires relay neurons");
    }
    std::vector<bool> reserved(network.neuron_count(), false);
    for (NeuronId neuron = 0; neuron < 8; ++neuron) {
        reserved[neuron] = true;
    }
    reserved[motor_assignment.config.response_stimulus_neuron] = true;
    reserved[motor_assignment.completion_contact] = true;
    for (const NeuronId contact : data_contacts) {
        reserved[contact] = true;
    }
    std::vector<NeuronId> candidates;
    for (NeuronId neuron = 0; neuron < network.neuron_count(); ++neuron) {
        if (!reserved[neuron]) {
            candidates.push_back(neuron);
        }
    }
    if (required_relays > candidates.size()) {
        throw std::invalid_argument("network has too few neurons for motor circuit");
    }
    std::uint64_t random_state =
        config.seed == 0 ? 0x9e3779b97f4a7c15ULL : config.seed;
    for (std::size_t remaining = candidates.size(); remaining > 1; --remaining) {
        const std::size_t selected = static_cast<std::size_t>(
            next_assignment_random(random_state) % remaining);
        std::swap(candidates[remaining - 1], candidates[selected]);
    }
    result.relay_neurons.assign(
        candidates.begin(), candidates.begin() + required_relays);

    if (config.mode == MotorCircuitMode::completion_group) {
        for (std::size_t index = 0; index < data_contacts.size(); ++index) {
            connect(data_contacts[index],
                    result.relay_neurons[index % result.relay_neurons.size()],
                    config.motor_weight);
        }
        for (const NeuronId relay : result.relay_neurons) {
            connect(relay, motor_assignment.completion_contact,
                    config.completion_weight);
        }
        return result;
    }

    const NeuronId accumulator = result.relay_neurons.front();
    for (const NeuronId contact : data_contacts) {
        connect(contact, accumulator, config.motor_weight);
    }
    connect(accumulator, motor_assignment.completion_contact,
            config.completion_weight);
    return result;
}

TechnicalEvaluation Brainstem::evaluate_binary(
    const std::uint8_t actual, const std::uint8_t target) noexcept {
    TechnicalEvaluation result;
    result.actual = actual;
    result.target = target;
    const std::uint8_t differences = static_cast<std::uint8_t>(actual ^ target);
    for (unsigned bit = 0; bit < 8; ++bit) {
        result.matching_bits += ((differences >> bit) & 1U) == 0 ? 1U : 0U;
    }
    result.success_percent = static_cast<std::uint8_t>(result.matching_bits * 25U / 2U);
    return result;
}

EvaluationEncoding Brainstem::encode_evaluation(
    const std::uint8_t success_percent,
    const EvaluationSignalKind kind) {
    if (success_percent > 100 || success_percent % 25 != 0) {
        throw std::invalid_argument("evaluation must be one of 0, 25, 50, 75 or 100 percent");
    }
    EvaluationEncoding result;
    result.kind = kind;
    result.success_percent = success_percent;
    result.active_channels.push_back(
        kind == EvaluationSignalKind::simple ? 0U : success_percent / 25U);
    return result;
}

EvaluationRouteResult Brainstem::create_evaluation_route(
    neural::Network& network,
    const EvaluationRouteConfig& config) const {
    if (config.graded_channel_count < 5 ||
        static_cast<std::uint64_t>(config.first_input_neuron) +
                config.graded_channel_count > network.neuron_count()) {
        throw std::invalid_argument("evaluation input range is invalid");
    }
    if (config.weight <= 0 || config.fanout_per_source == 0) {
        throw std::invalid_argument("evaluation route requires positive weight and fanout");
    }

    EvaluationRouteResult result;
    result.config = config;
    for (std::uint32_t channel = 0; channel < config.graded_channel_count; ++channel) {
        result.input_neurons.push_back(config.first_input_neuron + channel);
    }
    if (config.mode == EvaluationRouteMode::none ||
        config.mode == EvaluationRouteMode::sensory_input) {
        return result;
    }

    std::vector<bool> excluded(network.neuron_count(), false);
    for (const NeuronId neuron : result.input_neurons) excluded.at(neuron) = true;
    for (const NeuronId neuron : config.reserved_neurons) {
        if (neuron < excluded.size()) excluded[neuron] = true;
    }
    std::vector<NeuronId> candidates;
    for (NeuronId neuron = 0; neuron < network.neuron_count(); ++neuron) {
        if (!excluded[neuron]) candidates.push_back(neuron);
    }
    std::uint64_t state = config.seed == 0 ? 0x9e3779b97f4a7c15ULL : config.seed;
    for (std::size_t remaining = candidates.size(); remaining > 1; --remaining) {
        const std::size_t selected = static_cast<std::size_t>(
            next_assignment_random(state) % remaining);
        std::swap(candidates[selected], candidates[remaining - 1]);
    }

    std::size_t cursor = 0;
    if (config.mode == EvaluationRouteMode::relay_wiring) {
        if (config.relay_count == 0 || config.relay_count > candidates.size()) {
            throw std::invalid_argument("evaluation relay count is invalid");
        }
        for (std::uint32_t index = 0; index < config.relay_count; ++index) {
            result.relay_neurons.push_back(candidates[cursor++]);
        }
    }
    const std::uint64_t target_count =
        static_cast<std::uint64_t>(config.fanout_per_source) *
        (config.mode == EvaluationRouteMode::relay_wiring
             ? result.relay_neurons.size() : result.input_neurons.size());
    if (cursor + target_count > candidates.size()) {
        throw std::invalid_argument("network is too small for evaluation route fanout");
    }

    const auto connect = [&](const NeuronId sender, const NeuronId receiver,
                             EvaluationRouteResult& output) {
        const auto existing = network.find_synapse(sender, receiver);
        SynapseId id = 0;
        if (existing.has_value()) {
            id = *existing;
            network.synapse(id).weight = config.weight;
            ++output.adjusted_synapses;
        } else {
            id = network.add_synapse(sender, receiver, config.weight);
            ++output.added_synapses;
        }
        output.route_synapses.push_back(id);
    };

    if (config.mode == EvaluationRouteMode::general_wiring) {
        for (const NeuronId input : result.input_neurons) {
            for (std::uint32_t edge = 0; edge < config.fanout_per_source; ++edge) {
                const NeuronId target = candidates[cursor++];
                result.target_neurons.push_back(target);
                connect(input, target, result);
            }
        }
    } else {
        for (std::size_t index = 0; index < result.input_neurons.size(); ++index) {
            connect(result.input_neurons[index],
                    result.relay_neurons[index % result.relay_neurons.size()], result);
        }
        for (const NeuronId relay : result.relay_neurons) {
            for (std::uint32_t edge = 0; edge < config.fanout_per_source; ++edge) {
                const NeuronId target = candidates[cursor++];
                result.target_neurons.push_back(target);
                connect(relay, target, result);
            }
        }
    }
    return result;
}

}  // namespace eve::brainstem
