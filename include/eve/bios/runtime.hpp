#pragma once

#include "eve/brainstem/brainstem.hpp"
#include "eve/neural/network.hpp"
#include "eve/types.hpp"

#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace eve::bios {

class DeterministicRng {
public:
    explicit DeterministicRng(std::uint64_t seed = 1) noexcept;

    [[nodiscard]] std::uint64_t next() noexcept;
    [[nodiscard]] std::uint32_t bounded(std::uint32_t exclusive_upper_bound);
    [[nodiscard]] std::uint64_t state() const noexcept;
    void set_state(std::uint64_t state);

private:
    std::uint64_t state_{};
};

enum class EventKind : std::uint8_t {
    fire = 0,
    impulse = 1,
};

struct Event {
    EventKind kind{EventKind::fire};
    std::uint32_t target{};

    bool operator==(const Event& other) const noexcept {
        return kind == other.kind && target == other.target;
    }
};

struct Diagnostics {
    std::uint32_t neuron_count{};
    std::uint32_t synapse_count{};
    std::size_t estimated_memory_bytes{};
    std::size_t queued_events{};
    std::map<std::int32_t, std::uint64_t> weight_distribution;
    Metrics metrics;
    std::uint64_t state_fingerprint{};
};

struct FireObservation {
    std::uint64_t event_index{};
    NeuronId neuron{};
    std::uint32_t outgoing_count{};
};

using FireObserver = std::function<void(const FireObservation&)>;

class Runtime {
public:
    explicit Runtime(ExperimentConfig config = {}, std::uint64_t seed = 1);

    void enqueue_fire(NeuronId neuron);
    [[nodiscard]] std::uint64_t run();
    [[nodiscard]] std::vector<brainstem::EncodedByte> process_utf8_bytes(
        const std::vector<std::uint8_t>& bytes);
    [[nodiscard]] std::vector<brainstem::EncodedByte> process_utf8_text(
        const std::string& utf8_text);

    void configure_motor_output(const brainstem::MotorOutputConfig& config);
    [[nodiscard]] brainstem::MotorResponse request_response(
        bool emit_response_stimulus = true,
        std::uint64_t safety_event_limit = 0);
    [[nodiscard]] bool motor_output_configured() const noexcept;
    [[nodiscard]] const brainstem::MotorAssignment& motor_assignment() const;
    [[nodiscard]] brainstem::InnateWiringResult configure_innate_wiring(
        const brainstem::InnateWiringConfig& config);
    [[nodiscard]] brainstem::MotorReachability motor_reachability() const;
    [[nodiscard]] brainstem::ExplorationResult enqueue_exploration(
        const brainstem::ExplorationConfig& config);
    [[nodiscard]] brainstem::MotorCircuitResult configure_motor_circuit(
        const brainstem::MotorCircuitConfig& config);
    [[nodiscard]] brainstem::FeedbackResult process_motor_feedback(
        const brainstem::MotorResponse& response,
        const brainstem::FeedbackConfig& config);

    void set_trace_stream(std::ostream* stream) noexcept;
    void set_fire_observer(FireObserver observer);

    [[nodiscard]] const ExperimentConfig& config() const noexcept;
    [[nodiscard]] const neural::Network& network() const noexcept;
    [[nodiscard]] const Metrics& metrics() const noexcept;
    [[nodiscard]] Diagnostics diagnostics() const;
    [[nodiscard]] std::uint64_t state_fingerprint() const noexcept;

    void save(const std::string& path) const;
    [[nodiscard]] static Runtime load(const std::string& path);

private:
    struct SkipInitializationTag {};
    Runtime(ExperimentConfig config, std::uint64_t seed, SkipInitializationTag);

    void validate_config() const;
    void process_fire(NeuronId neuron);
    void process_impulse(SynapseId synapse);
    void update_event_hash(const Event& event) noexcept;
    void update_queue_watermark() noexcept;
    void trace(const std::string& line);

    ExperimentConfig config_;
    neural::Network network_;
    brainstem::Brainstem brainstem_;
    DeterministicRng rng_;
    std::deque<Event> queue_;
    Metrics metrics_;
    std::ostream* trace_stream_{};
    FireObserver fire_observer_;
    brainstem::MotorOutput motor_output_;
};

}  // namespace eve::bios
