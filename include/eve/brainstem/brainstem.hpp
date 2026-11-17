#pragma once

#include "eve/neural/network.hpp"
#include "eve/types.hpp"

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
    bool weakened{};
    bool firing_suppressed{};
    bool phase_completed{};
    std::int32_t applied_weight{};
    std::int64_t membrane_after{};
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
};

}  // namespace eve::brainstem
