#pragma once

#include "eve/types.hpp"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace eve::neural {

struct Neuron {
    std::int64_t membrane_potential{};
    std::int64_t firing_threshold{};
    std::uint32_t input_event_count{};
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

    bool operator==(const Synapse& other) const noexcept;
};

class Network {
public:
    void reset(const ExperimentConfig& config);

    [[nodiscard]] SynapseId add_synapse(
        NeuronId sender,
        NeuronId receiver,
        std::int32_t weight);
    [[nodiscard]] bool has_synapse(NeuronId sender, NeuronId receiver) const;

    [[nodiscard]] Coordinate coordinate(NeuronId neuron) const;
    [[nodiscard]] std::vector<SynapseId> outgoing_synapses(NeuronId neuron) const;
    [[nodiscard]] std::vector<SynapseId> incoming_synapses(NeuronId neuron) const;
    void reset_incoming_activity(NeuronId neuron);

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
};

}  // namespace eve::neural
