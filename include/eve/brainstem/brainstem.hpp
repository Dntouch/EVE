#pragma once

#include "eve/neural/network.hpp"
#include "eve/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace eve::bios {
class DeterministicRng;
}

namespace eve::brainstem {

struct EncodedByte {
    std::uint64_t byte_index{};
    std::uint8_t value{};
    std::vector<NeuronId> fired_channels;

    bool operator==(const EncodedByte& other) const noexcept {
        return byte_index == other.byte_index && value == other.value &&
               fired_channels == other.fired_channels;
    }
};

struct ImpulseResult {
    bool fired{};
    bool strengthened{};
    bool strengthening_rejected_by_weight_limit{};
    bool weakened{};
    bool firing_suppressed{};
    bool phase_completed{};
    bool local_event_recorded{};
    bool local_event_overwritten{};
    std::int32_t applied_weight{};
    std::int64_t membrane_after{};
    std::uint64_t recovery_generation{};
};

struct RecoveryResult {
    bool recovered{};
    bool stale{};
    std::int64_t membrane_before{};
    std::int64_t membrane_after{};
    std::uint32_t refractory_before{};
    std::uint32_t refractory_after{};
};

struct EligibilityResult {
    bool created{};
    std::uint64_t generation{};
    std::uint64_t expires_at{};
    std::uint32_t overwritten_decisions{};
    std::uint32_t positive_candidates{};
    std::uint32_t zero_weight_candidates{};
    std::vector<SynapseId> candidates;
};

enum class MotorResponseStatus : std::uint8_t {
    no_response = 0,
    completed = 1,
    aborted = 2,
};

enum class InnateTopology : std::uint8_t {
    none = 0,
    structural_paths = 1,
    functional_paths = 2,
    varied_functional_paths = 3,
};

struct InnateWiringConfig {
    InnateTopology topology{InnateTopology::none};
    std::int32_t initial_weight{};
    std::uint64_t topology_seed{1};

    bool operator==(const InnateWiringConfig& other) const noexcept {
        return topology == other.topology && initial_weight == other.initial_weight &&
               topology_seed == other.topology_seed;
    }
};

struct InnatePath {
    NeuronId source{};
    NeuronId first_relay{};
    NeuronId second_relay{};
    NeuronId motor_target{};

    bool operator==(const InnatePath& other) const noexcept {
        return source == other.source && first_relay == other.first_relay &&
               second_relay == other.second_relay && motor_target == other.motor_target;
    }
};

struct InnateWiringResult {
    InnateWiringConfig config;
    std::vector<InnatePath> paths;
    std::uint32_t added_synapses{};
    std::uint32_t adjusted_existing_synapses{};
    std::uint32_t structurally_reachable_motor_contacts{};
    std::uint32_t maximum_shortest_path{};
};

struct MotorReachability {
    std::vector<std::uint32_t> shortest_paths;
    std::uint32_t reachable_contacts{};
    std::uint32_t maximum_shortest_path{};
};

enum class ExplorationMode : std::uint8_t {
    none = 0,
    single = 1,
    multiple = 2,
    localized = 3,
    distributed = 4,
};

enum class ExplorationTargetPolicy : std::uint8_t {
    all_neurons = 0,
    processing_neurons_only = 1,
};

struct ExplorationConfig {
    ExplorationMode mode{ExplorationMode::none};
    ExplorationTargetPolicy target_policy{ExplorationTargetPolicy::all_neurons};
    std::uint32_t activation_count{};
    std::uint32_t activation_budget{};
    std::uint64_t seed{1};
    Coordinate center{};
    std::uint32_t radius{1};
    std::vector<NeuronId> additional_reserved_neurons;
};

struct ExplorationResult {
    ExplorationConfig config;
    std::vector<NeuronId> activated_neurons;
    std::uint32_t eligible_neuron_count{};
    std::uint32_t reserved_neuron_count{};
};

enum class MotorCircuitMode : std::uint8_t {
    none = 0,
    direct_completion_path = 1,
    completion_group = 2,
    activity_dependent = 3,
};

struct MotorCircuitConfig {
    MotorCircuitMode mode{MotorCircuitMode::none};
    std::int32_t motor_weight{1};
    std::int32_t completion_weight{1};
    std::uint32_t relay_count{4};
    std::uint64_t seed{1};
    bool path_enabled{true};
};

struct MotorCircuitResult {
    MotorCircuitConfig config;
    std::vector<NeuronId> relay_neurons;
    std::vector<SynapseId> circuit_synapses;
    std::uint32_t added_synapses{};
    std::uint32_t adjusted_existing_synapses{};
};

enum class FeedbackMode : std::uint8_t {
    none = 0,
    after_completion = 1,
    per_motor_event = 2,
    virtual_environment = 3,
};

struct FeedbackConfig {
    FeedbackMode mode{FeedbackMode::none};
    NeuronId first_feedback_neuron{9};
    std::uint64_t event_budget{100'000};
    std::uint8_t environment_state{};
};

struct FeedbackEvent {
    std::int8_t motor_channel{-1};
    NeuronId sensory_neuron{};
};

struct FeedbackResult {
    FeedbackMode mode{FeedbackMode::none};
    std::uint8_t observed_motor_value{};
    std::uint8_t environment_before{};
    std::uint8_t environment_after{};
    std::vector<FeedbackEvent> sensory_events;
    std::uint64_t processed_events{};
    bool aborted{};
};

struct MotorOutputConfig {
    std::uint32_t contacts_per_channel{1};
    std::uint64_t assignment_seed{1};
    NeuronId response_stimulus_neuron{8};
    NeuronId first_motor_neuron{9};

    bool operator==(const MotorOutputConfig& other) const noexcept {
        return contacts_per_channel == other.contacts_per_channel &&
               assignment_seed == other.assignment_seed &&
               response_stimulus_neuron == other.response_stimulus_neuron &&
               first_motor_neuron == other.first_motor_neuron;
    }
};

struct MotorAssignment {
    MotorOutputConfig config;
    std::array<std::vector<NeuronId>, 8> channels;
    NeuronId completion_contact{kInvalidIndex};

    bool operator==(const MotorAssignment& other) const noexcept {
        return config == other.config && channels == other.channels &&
               completion_contact == other.completion_contact;
    }
};

struct MotorActivation {
    std::uint64_t event_index{};
    NeuronId neuron{};
    std::int8_t channel{-1};

    bool operator==(const MotorActivation& other) const noexcept {
        return event_index == other.event_index && neuron == other.neuron &&
               channel == other.channel;
    }
};

struct MotorResponse {
    MotorResponseStatus status{MotorResponseStatus::no_response};
    std::uint8_t value{};
    std::array<bool, 8> activated_channels{};
    std::vector<MotorActivation> activations;
    std::uint64_t processed_events{};

    [[nodiscard]] bool valid() const noexcept {
        return status == MotorResponseStatus::completed;
    }
};

class MotorOutput {
public:
    void configure(std::uint32_t neuron_count, const MotorOutputConfig& config);
    void begin_response();
    void observe_fire(NeuronId neuron, std::uint64_t event_index);
    void finish_without_completion();
    void abort();

    [[nodiscard]] bool configured() const noexcept;
    [[nodiscard]] bool active() const noexcept;
    [[nodiscard]] const MotorAssignment& assignment() const;
    [[nodiscard]] const MotorResponse& response() const noexcept;
    [[nodiscard]] std::size_t estimated_memory_bytes() const noexcept;

private:
    MotorAssignment assignment_;
    MotorResponse response_;
    std::vector<std::int8_t> roles_;
    bool configured_{};
    bool active_{};
};

class Brainstem {
public:
    void initialize_network(
        neural::Network& network,
        const ExperimentConfig& config,
        bios::DeterministicRng& rng) const;

    [[nodiscard]] ImpulseResult receive_impulse(
        neural::Network& network,
        SynapseId synapse,
        const ExperimentConfig& config,
        bios::DeterministicRng* rng = nullptr) const;

    [[nodiscard]] RecoveryResult recover_neuron(
        neural::Network& network,
        NeuronId neuron,
        std::uint64_t generation,
        const ExperimentConfig& config) const;

    [[nodiscard]] EligibilityResult create_eligibility(
        neural::Network& network,
        NeuronId neuron,
        std::uint64_t expires_at,
        const ExperimentConfig& config) const;

    [[nodiscard]] bool expire_eligibility(
        neural::Network& network,
        NeuronId neuron,
        std::uint64_t generation) const;

    [[nodiscard]] std::optional<SynapseId> select_competing_synapse(
        const neural::Network& network,
        SynapseId strengthened_synapse,
        bios::DeterministicRng& rng) const;

    [[nodiscard]] bool should_attempt_growth(
        std::uint32_t outgoing_count,
        bios::DeterministicRng& rng) const;

    [[nodiscard]] std::optional<SynapseId> attempt_growth(
        neural::Network& network,
        NeuronId sender,
        const ExperimentConfig& config,
        bios::DeterministicRng& rng) const;

    [[nodiscard]] std::vector<EncodedByte> encode_utf8_bytes(
        const std::vector<std::uint8_t>& bytes,
        std::uint32_t neuron_count) const;

    [[nodiscard]] InnateWiringResult create_innate_wiring(
        neural::Network& network,
        const MotorAssignment& motor_assignment,
        const InnateWiringConfig& config) const;

    [[nodiscard]] MotorReachability analyze_motor_reachability(
        const neural::Network& network,
        const MotorAssignment& motor_assignment) const;

    [[nodiscard]] ExplorationResult select_exploration(
        const neural::Network& network,
        const ExplorationConfig& config,
        const std::vector<NeuronId>& reserved_neurons = {}) const;

    [[nodiscard]] MotorCircuitResult create_motor_circuit(
        neural::Network& network,
        const MotorAssignment& motor_assignment,
        const MotorCircuitConfig& config) const;
};

}  // namespace eve::brainstem
