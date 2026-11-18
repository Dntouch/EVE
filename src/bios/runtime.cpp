#include "eve/bios/runtime.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace eve::bios {
namespace {

constexpr std::array<char, 8> kPersistenceMagic{{'E', 'V', 'E', '0', '0', '1', '\r', '\n'}};
constexpr std::uint32_t kPersistenceVersion = 6;
constexpr std::uint64_t kNonZeroRngState = 0x9e3779b97f4a7c15ULL;
constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

void write_bytes(std::ostream& stream, const char* data, const std::size_t size) {
    stream.write(data, static_cast<std::streamsize>(size));
    if (!stream) {
        throw std::runtime_error("failed to write EVE state");
    }
}

void write_u8(std::ostream& stream, const std::uint8_t value) {
    const char byte = static_cast<char>(value);
    write_bytes(stream, &byte, 1);
}

void write_u32(std::ostream& stream, const std::uint32_t value) {
    std::array<char, 4> bytes{};
    for (unsigned index = 0; index < bytes.size(); ++index) {
        bytes[index] = static_cast<char>((value >> (index * 8U)) & 0xffU);
    }
    write_bytes(stream, bytes.data(), bytes.size());
}

void write_u64(std::ostream& stream, const std::uint64_t value) {
    std::array<char, 8> bytes{};
    for (unsigned index = 0; index < bytes.size(); ++index) {
        bytes[index] = static_cast<char>((value >> (index * 8U)) & 0xffU);
    }
    write_bytes(stream, bytes.data(), bytes.size());
}

void write_i32(std::ostream& stream, const std::int32_t value) {
    write_u32(stream, static_cast<std::uint32_t>(value));
}

void write_i64(std::ostream& stream, const std::int64_t value) {
    write_u64(stream, static_cast<std::uint64_t>(value));
}

void read_bytes(std::istream& stream, char* data, const std::size_t size) {
    stream.read(data, static_cast<std::streamsize>(size));
    if (!stream) {
        throw std::runtime_error("truncated or unreadable EVE state");
    }
}

std::uint8_t read_u8(std::istream& stream) {
    char byte{};
    read_bytes(stream, &byte, 1);
    return static_cast<std::uint8_t>(static_cast<unsigned char>(byte));
}

std::uint32_t read_u32(std::istream& stream) {
    std::array<char, 4> bytes{};
    read_bytes(stream, bytes.data(), bytes.size());
    std::uint32_t value = 0;
    for (unsigned index = 0; index < bytes.size(); ++index) {
        value |= static_cast<std::uint32_t>(static_cast<unsigned char>(bytes[index]))
                 << (index * 8U);
    }
    return value;
}

std::uint64_t read_u64(std::istream& stream) {
    std::array<char, 8> bytes{};
    read_bytes(stream, bytes.data(), bytes.size());
    std::uint64_t value = 0;
    for (unsigned index = 0; index < bytes.size(); ++index) {
        value |= static_cast<std::uint64_t>(static_cast<unsigned char>(bytes[index]))
                 << (index * 8U);
    }
    return value;
}

std::int32_t read_i32(std::istream& stream) {
    return static_cast<std::int32_t>(read_u32(stream));
}

std::int64_t read_i64(std::istream& stream) {
    return static_cast<std::int64_t>(read_u64(stream));
}

bool read_bool(std::istream& stream) {
    const std::uint8_t value = read_u8(stream);
    if (value > 1) {
        throw std::runtime_error("invalid boolean in EVE state");
    }
    return value != 0;
}

void write_config(std::ostream& stream, const ExperimentConfig& config) {
    write_u32(stream, config.neuron_count);
    write_u32(stream, config.width);
    write_u32(stream, config.height);
    write_u32(stream, config.depth);
    write_u32(stream, config.initial_outgoing_synapses);
    write_i64(stream, config.firing_threshold);
    write_u32(stream, config.synaptic_learning_threshold);
    write_u32(stream, config.activity_phase_limit);
    write_i32(stream, config.initial_synapse_weight);
    write_i32(stream, config.maximum_synapse_weight);
    write_u64(stream, config.input_weight_budget);
    write_u32(stream, config.refractory_input_events);
    write_u8(stream, static_cast<std::uint8_t>(config.recovery_mode));
    write_u64(stream, config.recovery_delay);
    write_u32(stream, config.membrane_relaxation_percent);
    write_u32(stream, config.local_event_memory_capacity);
    write_u8(stream, static_cast<std::uint8_t>(config.eligibility_selection));
    write_u8(stream, static_cast<std::uint8_t>(config.eligibility_representation));
    write_u8(stream, static_cast<std::uint8_t>(config.eligibility_history));
    write_u32(stream, config.eligibility_candidate_capacity);
    write_u32(stream, config.eligibility_decision_capacity);
    write_u64(stream, config.eligibility_duration);
    write_u64(stream, config.maximum_events_per_run);
    write_u8(stream, config.learning_enabled ? 1 : 0);
    write_u8(stream, config.growth_enabled ? 1 : 0);
    write_u8(stream, static_cast<std::uint8_t>(config.reset_mode));
}

ExperimentConfig read_config(std::istream& stream, const std::uint32_t version) {
    ExperimentConfig config;
    config.neuron_count = read_u32(stream);
    config.width = read_u32(stream);
    config.height = read_u32(stream);
    config.depth = read_u32(stream);
    config.initial_outgoing_synapses = read_u32(stream);
    config.firing_threshold = read_i64(stream);
    config.synaptic_learning_threshold = read_u32(stream);
    config.activity_phase_limit = read_u32(stream);
    config.initial_synapse_weight = read_i32(stream);
    config.maximum_synapse_weight = read_i32(stream);
    if (version >= 2) {
        config.input_weight_budget = read_u64(stream);
        config.refractory_input_events = read_u32(stream);
    }
    if (version >= 5) {
        const std::uint8_t recovery_mode = read_u8(stream);
        if (recovery_mode > static_cast<std::uint8_t>(
                                RecoveryMode::independent_with_membrane_relaxation)) {
            throw std::runtime_error("invalid recovery mode in EVE state");
        }
        config.recovery_mode = static_cast<RecoveryMode>(recovery_mode);
        config.recovery_delay = read_u64(stream);
        config.membrane_relaxation_percent = read_u32(stream);
    }
    if (version >= 4) {
        config.local_event_memory_capacity = read_u32(stream);
    }
    if (version >= 6) {
        const std::uint8_t selection = read_u8(stream);
        const std::uint8_t representation = read_u8(stream);
        const std::uint8_t history = read_u8(stream);
        if (selection > static_cast<std::uint8_t>(EligibilitySelection::positive_weight) ||
            representation > static_cast<std::uint8_t>(EligibilityRepresentation::unique) ||
            history > static_cast<std::uint8_t>(EligibilityHistory::merge_unique)) {
            throw std::runtime_error("invalid eligibility configuration in EVE state");
        }
        config.eligibility_selection = static_cast<EligibilitySelection>(selection);
        config.eligibility_representation =
            static_cast<EligibilityRepresentation>(representation);
        config.eligibility_history = static_cast<EligibilityHistory>(history);
        config.eligibility_candidate_capacity = read_u32(stream);
        config.eligibility_decision_capacity = read_u32(stream);
        config.eligibility_duration = read_u64(stream);
    }
    config.maximum_events_per_run = read_u64(stream);
    config.learning_enabled = read_bool(stream);
    config.growth_enabled = read_bool(stream);
    const std::uint8_t reset_mode = read_u8(stream);
    if (reset_mode > static_cast<std::uint8_t>(ResetMode::subtract_threshold)) {
        throw std::runtime_error("invalid reset mode in EVE state");
    }
    config.reset_mode = static_cast<ResetMode>(reset_mode);
    return config;
}

void write_metrics(std::ostream& stream, const Metrics& metrics) {
    write_u64(stream, metrics.processed_events);
    write_u64(stream, metrics.transmitted_impulses);
    write_u64(stream, metrics.fire_events);
    write_u64(stream, metrics.completed_activity_phases);
    write_u64(stream, metrics.synaptic_strengthenings);
    write_u64(stream, metrics.rejected_strengthenings_at_weight_limit);
    write_u64(stream, metrics.synaptic_weakenings);
    write_u64(stream, metrics.refractory_suppressed_firings);
    write_u64(stream, metrics.local_event_memory_records);
    write_u64(stream, metrics.local_event_memory_overwrites);
    write_u64(stream, metrics.scheduled_recovery_events);
    write_u64(stream, metrics.processed_recovery_events);
    write_u64(stream, metrics.stale_recovery_events);
    write_u64(stream, metrics.processed_boundary_events);
    write_u64(stream, metrics.eligibility_decisions);
    write_u64(stream, metrics.eligibility_candidates);
    write_u64(stream, metrics.eligibility_positive_candidates);
    write_u64(stream, metrics.eligibility_zero_weight_candidates);
    write_u64(stream, metrics.eligibility_overwritten_decisions);
    write_u64(stream, metrics.scheduled_eligibility_expiries);
    write_u64(stream, metrics.processed_eligibility_expiries);
    write_u64(stream, metrics.stale_eligibility_expiries);
    write_u64(stream, metrics.growth_attempts);
    write_u64(stream, metrics.grown_synapses);
    write_u64(stream, metrics.processed_sensor_bytes);
    write_u64(stream, metrics.maximum_queue_size);
    write_u64(stream, metrics.event_trace_hash);
}

Metrics read_metrics(std::istream& stream, const std::uint32_t version) {
    Metrics metrics;
    metrics.processed_events = read_u64(stream);
    metrics.transmitted_impulses = read_u64(stream);
    metrics.fire_events = read_u64(stream);
    metrics.completed_activity_phases = read_u64(stream);
    metrics.synaptic_strengthenings = read_u64(stream);
    if (version >= 3) {
        metrics.rejected_strengthenings_at_weight_limit = read_u64(stream);
    }
    if (version >= 2) {
        metrics.synaptic_weakenings = read_u64(stream);
        metrics.refractory_suppressed_firings = read_u64(stream);
    }
    if (version >= 4) {
        metrics.local_event_memory_records = read_u64(stream);
        metrics.local_event_memory_overwrites = read_u64(stream);
    }
    if (version >= 5) {
        metrics.scheduled_recovery_events = read_u64(stream);
        metrics.processed_recovery_events = read_u64(stream);
        metrics.stale_recovery_events = read_u64(stream);
        metrics.processed_boundary_events = read_u64(stream);
    }
    if (version >= 6) {
        metrics.eligibility_decisions = read_u64(stream);
        metrics.eligibility_candidates = read_u64(stream);
        metrics.eligibility_positive_candidates = read_u64(stream);
        metrics.eligibility_zero_weight_candidates = read_u64(stream);
        metrics.eligibility_overwritten_decisions = read_u64(stream);
        metrics.scheduled_eligibility_expiries = read_u64(stream);
        metrics.processed_eligibility_expiries = read_u64(stream);
        metrics.stale_eligibility_expiries = read_u64(stream);
    }
    metrics.growth_attempts = read_u64(stream);
    metrics.grown_synapses = read_u64(stream);
    metrics.processed_sensor_bytes = read_u64(stream);
    if (version >= 2) {
        metrics.maximum_queue_size = read_u64(stream);
    }
    metrics.event_trace_hash = read_u64(stream);
    return metrics;
}

void mix_hash(std::uint64_t& hash, const std::uint64_t value) noexcept {
    for (unsigned shift = 0; shift < 64; shift += 8) {
        hash ^= (value >> shift) & 0xffU;
        hash *= kFnvPrime;
    }
}

}  // namespace

DeterministicRng::DeterministicRng(const std::uint64_t seed) noexcept
    : state_(seed == 0 ? kNonZeroRngState : seed) {}

std::uint64_t DeterministicRng::next() noexcept {
    std::uint64_t value = state_;
    value ^= value >> 12U;
    value ^= value << 25U;
    value ^= value >> 27U;
    state_ = value;
    return value * 2685821657736338717ULL;
}

std::uint32_t DeterministicRng::bounded(const std::uint32_t exclusive_upper_bound) {
    if (exclusive_upper_bound == 0) {
        throw std::invalid_argument("random bound must be greater than zero");
    }
    const std::uint64_t bound = exclusive_upper_bound;
    const std::uint64_t rejection_limit = (0ULL - bound) % bound;
    std::uint64_t value = 0;
    do {
        value = next();
    } while (value < rejection_limit);
    return static_cast<std::uint32_t>(value % bound);
}

std::uint64_t DeterministicRng::state() const noexcept {
    return state_;
}

void DeterministicRng::set_state(const std::uint64_t state) {
    if (state == 0) {
        throw std::invalid_argument("random state must not be zero");
    }
    state_ = state;
}

Runtime::Runtime(ExperimentConfig config, const std::uint64_t seed)
    : config_(std::move(config)), rng_(seed) {
    validate_config();
    brainstem_.initialize_network(network_, config_, rng_);
}

Runtime::Runtime(
    ExperimentConfig config,
    const std::uint64_t seed,
    SkipInitializationTag)
    : config_(std::move(config)), rng_(seed) {
    validate_config();
}

void Runtime::enqueue_fire(const NeuronId neuron) {
    static_cast<void>(network_.neuron(neuron));
    enqueue_event(EventKind::fire, neuron, simulated_time_);
}

std::uint64_t Runtime::run() {
    std::uint64_t processed = 0;
    while (has_ready_event()) {
        if (processed >= config_.maximum_events_per_run) {
            throw std::runtime_error(
                "event processing limit reached with " + std::to_string(queue_.size()) +
                " events still queued");
        }
        const Event event = pop_next_event();
        ++processed;
        process_event(event);
    }
    return processed;
}

std::uint64_t Runtime::advance_time_to(
    const std::uint64_t target_time, const std::uint64_t event_limit) {
    if (target_time < simulated_time_) {
        throw std::invalid_argument("simulated time cannot move backwards");
    }
    const std::uint64_t limit =
        event_limit == 0 ? config_.maximum_events_per_run : event_limit;
    std::uint64_t processed = 0;
    while (!queue_.empty() && queue_.front().scheduled_time <= target_time) {
        if (processed >= limit) {
            throw std::runtime_error("time advance event limit reached");
        }
        const Event event = pop_next_event();
        simulated_time_ = event.scheduled_time;
        ++processed;
        process_event(event);
    }
    simulated_time_ = target_time;
    return processed;
}

std::uint64_t Runtime::advance_time_by(
    const std::uint64_t duration, const std::uint64_t event_limit) {
    if (duration > std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
        throw std::overflow_error("simulated time overflow");
    }
    return advance_time_to(simulated_time_ + duration, event_limit);
}

void Runtime::schedule_boundary(const std::uint64_t time, const std::uint32_t marker) {
    if (time < simulated_time_) {
        throw std::invalid_argument("boundary cannot be scheduled in the past");
    }
    enqueue_event(EventKind::boundary, marker, time);
}

std::vector<brainstem::EncodedByte> Runtime::process_utf8_bytes(
    const std::vector<std::uint8_t>& bytes) {
    auto encoded = brainstem_.encode_utf8_bytes(bytes, network_.neuron_count());
    const std::uint64_t first_byte_index = metrics_.processed_sensor_bytes;
    for (brainstem::EncodedByte& item : encoded) {
        item.byte_index += first_byte_index;
    }
    for (const brainstem::EncodedByte& item : encoded) {
        std::ostringstream begin;
        begin << "sensor byte=" << item.byte_index << " value="
              << static_cast<unsigned>(item.value) << " begin";
        trace(begin.str());

        for (const NeuronId channel : item.fired_channels) {
            trace("sensor byte=" + std::to_string(item.byte_index) + " channel=" +
                  std::to_string(channel));
            enqueue_fire(channel);
        }
        static_cast<void>(run());
        ++metrics_.processed_sensor_bytes;
        trace("sensor byte=" + std::to_string(item.byte_index) + " end");
    }
    return encoded;
}

std::vector<brainstem::EncodedByte> Runtime::process_utf8_text(const std::string& utf8_text) {
    std::vector<std::uint8_t> bytes;
    bytes.reserve(utf8_text.size());
    for (const unsigned char byte : utf8_text) {
        bytes.push_back(byte);
    }
    return process_utf8_bytes(bytes);
}

void Runtime::configure_motor_output(const brainstem::MotorOutputConfig& config) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot reconfigure an active motor response");
    }
    motor_output_.configure(network_.neuron_count(), config);
    innate_reservations_.clear();
    motor_circuit_reservations_.clear();
}

brainstem::MotorResponse Runtime::request_response(
    const bool emit_response_stimulus,
    const std::uint64_t safety_event_limit) {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output is not configured");
    }
    const std::uint64_t limit =
        safety_event_limit == 0 ? config_.maximum_events_per_run : safety_event_limit;
    motor_output_.begin_response();
    if (emit_response_stimulus) {
        enqueue_fire(motor_output_.assignment().config.response_stimulus_neuron);
    }

    std::uint64_t processed = 0;
    while (has_ready_event() && motor_output_.active()) {
        if (processed >= limit) {
            motor_output_.abort();
            break;
        }
        const Event event = pop_next_event();
        ++processed;
        process_event(event);
    }
    if (motor_output_.active()) {
        motor_output_.finish_without_completion();
    }
    brainstem::MotorResponse response = motor_output_.response();
    response.processed_events = processed;
    return response;
}

bool Runtime::motor_output_configured() const noexcept {
    return motor_output_.configured();
}

const brainstem::MotorAssignment& Runtime::motor_assignment() const {
    return motor_output_.assignment();
}

brainstem::InnateWiringResult Runtime::configure_innate_wiring(
    const brainstem::InnateWiringConfig& config) {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before innate wiring");
    }
    if (config.initial_weight < 0 ||
        config.initial_weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("innate weight is outside configured synaptic bounds");
    }
    brainstem::InnateWiringResult result = brainstem_.create_innate_wiring(
        network_, motor_output_.assignment(), config);
    for (const brainstem::InnatePath& path : result.paths) {
        innate_reservations_.push_back(path.first_relay);
        innate_reservations_.push_back(path.second_relay);
    }
    std::sort(innate_reservations_.begin(), innate_reservations_.end());
    innate_reservations_.erase(
        std::unique(innate_reservations_.begin(), innate_reservations_.end()),
        innate_reservations_.end());
    return result;
}

brainstem::MotorReachability Runtime::motor_reachability() const {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before reachability analysis");
    }
    return brainstem_.analyze_motor_reachability(
        network_, motor_output_.assignment());
}

brainstem::ExplorationResult Runtime::enqueue_exploration(
    const brainstem::ExplorationConfig& config) {
    brainstem::ExplorationResult result =
        brainstem_.select_exploration(network_, config, exploration_reservations());
    for (const NeuronId neuron : result.activated_neurons) {
        enqueue_fire(neuron);
    }
    return result;
}

std::vector<NeuronId> Runtime::exploration_reservations() const {
    std::vector<NeuronId> reserved;
    if (!motor_output_.configured()) {
        return reserved;
    }
    const brainstem::MotorAssignment& assignment = motor_output_.assignment();
    for (NeuronId neuron = 0; neuron < 8 && neuron < network_.neuron_count(); ++neuron) {
        reserved.push_back(neuron);
    }
    reserved.push_back(assignment.config.response_stimulus_neuron);
    for (NeuronId neuron = assignment.config.response_stimulus_neuron + 1U;
         neuron < assignment.config.first_motor_neuron;
         ++neuron) {
        reserved.push_back(neuron);
    }
    for (const auto& channel : assignment.channels) {
        reserved.insert(reserved.end(), channel.begin(), channel.end());
    }
    reserved.push_back(assignment.completion_contact);
    reserved.insert(reserved.end(), innate_reservations_.begin(), innate_reservations_.end());
    reserved.insert(reserved.end(), motor_circuit_reservations_.begin(),
                    motor_circuit_reservations_.end());
    std::sort(reserved.begin(), reserved.end());
    reserved.erase(std::unique(reserved.begin(), reserved.end()), reserved.end());
    return reserved;
}

brainstem::MotorCircuitResult Runtime::configure_motor_circuit(
    const brainstem::MotorCircuitConfig& config) {
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before motor circuit");
    }
    if (config.motor_weight < 0 || config.completion_weight < 0 ||
        config.motor_weight > config_.maximum_synapse_weight ||
        config.completion_weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("motor circuit weight is outside synaptic bounds");
    }
    brainstem::MotorCircuitResult result = brainstem_.create_motor_circuit(
        network_, motor_output_.assignment(), config);
    motor_circuit_reservations_ = result.relay_neurons;
    return result;
}

brainstem::FeedbackResult Runtime::process_motor_feedback(
    const brainstem::MotorResponse& response,
    const brainstem::FeedbackConfig& config) {
    brainstem::FeedbackResult result;
    result.mode = config.mode;
    result.observed_motor_value = response.value;
    result.environment_before = config.environment_state;
    result.environment_after = config.environment_state;
    if (config.mode == brainstem::FeedbackMode::none) {
        return result;
    }
    if (!motor_output_.configured()) {
        throw std::logic_error("motor output must be configured before feedback");
    }
    if (config.event_budget == 0 || config.first_feedback_neuron < 9 ||
        static_cast<std::uint64_t>(config.first_feedback_neuron) + 8U >=
            network_.neuron_count()) {
        throw std::invalid_argument("invalid sensorimotor feedback configuration");
    }

    std::vector<bool> motor_contact(network_.neuron_count(), false);
    for (const auto& channel : motor_output_.assignment().channels) {
        for (const NeuronId neuron : channel) {
            motor_contact[neuron] = true;
        }
    }
    motor_contact[motor_output_.assignment().completion_contact] = true;
    for (NeuronId offset = 0; offset < 9; ++offset) {
        const NeuronId neuron = config.first_feedback_neuron + offset;
        if (neuron == motor_output_.assignment().config.response_stimulus_neuron ||
            motor_contact[neuron]) {
            throw std::invalid_argument("feedback channels overlap motor or control contacts");
        }
    }

    const auto enqueue_feedback = [this, &result, &config](
                                      const std::int8_t source_channel,
                                      const NeuronId offset) {
        const NeuronId sensory_neuron = config.first_feedback_neuron + offset;
        result.sensory_events.push_back(
            brainstem::FeedbackEvent{source_channel, sensory_neuron});
        enqueue_fire(sensory_neuron);
    };
    const auto enqueue_value = [&enqueue_feedback](const std::uint8_t value) {
        for (std::uint32_t bit = 0; bit < 8; ++bit) {
            if ((value & static_cast<std::uint8_t>(1U << bit)) != 0) {
                enqueue_feedback(static_cast<std::int8_t>(bit), bit);
            }
        }
        enqueue_feedback(-1, 8);
    };
    const auto process_pending = [this, &result, &config]() {
        while (has_ready_event()) {
            if (result.processed_events >= config.event_budget) {
                result.aborted = true;
                return false;
            }
            const Event event = pop_next_event();
            ++result.processed_events;
            process_event(event);
        }
        return true;
    };

    if (config.mode == brainstem::FeedbackMode::per_motor_event) {
        for (const brainstem::MotorActivation& activation : response.activations) {
            if (activation.channel < 0) {
                continue;
            }
            enqueue_feedback(
                activation.channel, static_cast<NeuronId>(activation.channel));
            if (!process_pending()) {
                break;
            }
        }
        return result;
    }

    if (!response.valid()) {
        return result;
    }
    if (config.mode == brainstem::FeedbackMode::virtual_environment) {
        const std::uint8_t rotated = static_cast<std::uint8_t>(
            (config.environment_state << 1U) | (config.environment_state >> 7U));
        result.environment_after = rotated ^ response.value;
        enqueue_value(result.environment_after);
    } else {
        enqueue_value(response.value);
    }
    static_cast<void>(process_pending());
    return result;
}

void Runtime::set_trace_stream(std::ostream* stream) noexcept {
    trace_stream_ = stream;
}

void Runtime::set_fire_observer(FireObserver observer) {
    fire_observer_ = std::move(observer);
}

void Runtime::set_impulse_observer(ImpulseObserver observer) {
    impulse_observer_ = std::move(observer);
}

void Runtime::set_recovery_observer(RecoveryObserver observer) {
    recovery_observer_ = std::move(observer);
}

MotorCausality Runtime::analyze_motor_causality(
    const brainstem::MotorResponse& response,
    const brainstem::MotorCircuitResult& circuit,
    const std::vector<ImpulseObservation>& impulses) {
    MotorCausality result;
    result.technical_abort =
        response.status == brainstem::MotorResponseStatus::aborted;
    result.completed = response.valid();

    const brainstem::MotorActivation* completion = nullptr;
    const brainstem::MotorActivation* first_data = nullptr;
    std::set<NeuronId> causally_reached;
    for (const brainstem::MotorActivation& activation : response.activations) {
        if (activation.channel >= 0) {
            if (first_data == nullptr) {
                first_data = &activation;
            }
            causally_reached.insert(activation.neuron);
        } else {
            completion = &activation;
            break;
        }
    }
    if (completion != nullptr) {
        result.completion_event = completion->event_index;
    }
    if (result.completed &&
        (first_data == nullptr || first_data->event_index >= completion->event_index)) {
        result.completed_without_prior_data = true;
        return result;
    }
    if (first_data == nullptr) {
        return result;
    }
    result.first_data_event = first_data->event_index;
    if (result.completed) {
        result.completed_after_data = true;
        result.events_between_data_and_completion =
            completion->event_index - first_data->event_index - 1U;
    }

    const std::set<SynapseId> circuit_synapses(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    for (const ImpulseObservation& impulse : impulses) {
        if (impulse.event_index <= first_data->event_index ||
            (completion != nullptr && impulse.event_index >= completion->event_index) ||
            circuit_synapses.find(impulse.synapse) == circuit_synapses.end()) {
            continue;
        }
        ++result.observed_circuit_impulses;
        if (impulse.firing_suppressed) {
            ++result.suppressed_circuit_impulses;
        }
        if (!impulse.receiver_fired) {
            continue;
        }
        ++result.firing_circuit_impulses;
        if (causally_reached.find(impulse.sender) == causally_reached.end()) {
            continue;
        }
        causally_reached.insert(impulse.receiver);
        if (result.completed_after_data && impulse.receiver == completion->neuron) {
            result.proven_circuit_path = true;
        }
    }
    return result;
}

LocalMemoryCausality Runtime::analyze_local_motor_memory(
    const brainstem::MotorResponse& response,
    const brainstem::MotorCircuitResult& circuit,
    const std::vector<ImpulseObservation>& impulses) const {
    LocalMemoryCausality result;
    if (circuit.config.mode != brainstem::MotorCircuitMode::activity_dependent ||
        circuit.relay_neurons.empty() || !motor_output_.configured()) {
        return result;
    }

    const NeuronId accumulator = circuit.relay_neurons.front();
    const NeuronId completion = motor_output_.assignment().completion_contact;
    std::uint64_t first_data_event = 0;
    std::uint64_t completion_event = 0;
    for (const brainstem::MotorActivation& activation : response.activations) {
        if (activation.channel >= 0 && first_data_event == 0) {
            first_data_event = activation.event_index;
        }
        if (activation.channel < 0) {
            completion_event = activation.event_index;
            break;
        }
    }
    if (first_data_event == 0 || completion_event == 0 ||
        completion_event <= first_data_event) {
        return result;
    }
    const std::set<SynapseId> circuit_ids(
        circuit.circuit_synapses.begin(), circuit.circuit_synapses.end());
    const std::int64_t motor_weight = std::max<std::int32_t>(1, circuit.config.motor_weight);
    result.required_accumulator_inputs = static_cast<std::uint32_t>(
        (config_.firing_threshold + motor_weight - 1) / motor_weight);

    const auto classify_snapshot = [&](const ImpulseObservation& observation,
                                       const NeuronId receiver) {
        std::uint32_t relevant = 0;
        for (const SynapseId id : observation.local_event_snapshot) {
            const neural::Synapse& item = network_.synapse(id);
            if (item.weight <= 0) {
                ++result.inactive_entries;
            }
            if (circuit_ids.find(id) != circuit_ids.end() &&
                item.receiver == receiver && item.weight > 0) {
                ++relevant;
            } else {
                ++result.unassigned_entries;
            }
        }
        return relevant;
    };

    for (const ImpulseObservation& observation : impulses) {
        if (!observation.receiver_fired ||
            observation.event_index < first_data_event ||
            observation.event_index > completion_event) {
            continue;
        }
        if (observation.receiver == accumulator) {
            result.accumulator_fired = true;
            result.retained_accumulator_inputs = std::max(
                result.retained_accumulator_inputs,
                classify_snapshot(observation, accumulator));
        } else if (observation.receiver == completion) {
            result.completion_fired = true;
            result.retained_completion_inputs = std::max(
                result.retained_completion_inputs,
                classify_snapshot(observation, completion));
        }
    }
    result.accumulator_reconstructable =
        result.retained_accumulator_inputs >= result.required_accumulator_inputs;
    result.completion_reconstructable = result.retained_completion_inputs >= 1;
    result.full_path_reconstructable = result.accumulator_reconstructable &&
                                       result.completion_reconstructable;
    return result;
}

void Runtime::set_experimental_synapse_weight(
    const SynapseId synapse, const std::int32_t weight) {
    if (motor_output_.active()) {
        throw std::logic_error("cannot interrupt a synapse during an active response");
    }
    if (weight < 0 || weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("experimental weight is outside configured bounds");
    }
    network_.synapse(synapse).weight = weight;
}

const ExperimentConfig& Runtime::config() const noexcept {
    return config_;
}

const neural::Network& Runtime::network() const noexcept {
    return network_;
}

const Metrics& Runtime::metrics() const noexcept {
    return metrics_;
}

Diagnostics Runtime::diagnostics() const {
    Diagnostics result;
    result.neuron_count = network_.neuron_count();
    result.synapse_count = network_.synapse_count();
    result.estimated_memory_bytes = sizeof(*this) + network_.estimated_memory_bytes() -
                                    sizeof(neural::Network) +
                                    queue_.size() * sizeof(Event) +
                                    motor_output_.estimated_memory_bytes();
    result.queued_events = queue_.size();
    result.ready_events = static_cast<std::size_t>(std::count_if(
        queue_.begin(), queue_.end(),
        [this](const Event& event) { return event.scheduled_time <= simulated_time_; }));
    result.future_events = result.queued_events - result.ready_events;
    result.simulated_time = simulated_time_;
    result.weight_distribution = network_.weight_distribution();
    result.metrics = metrics_;
    result.state_fingerprint = state_fingerprint();
    return result;
}

std::uint64_t Runtime::state_fingerprint() const noexcept {
    std::uint64_t hash = network_.state_fingerprint();
    mix_hash(hash, rng_.state());
    mix_hash(hash, config_.neuron_count);
    mix_hash(hash, config_.width);
    mix_hash(hash, config_.height);
    mix_hash(hash, config_.depth);
    mix_hash(hash, config_.initial_outgoing_synapses);
    mix_hash(hash, static_cast<std::uint64_t>(config_.firing_threshold));
    mix_hash(hash, config_.synaptic_learning_threshold);
    mix_hash(hash, config_.activity_phase_limit);
    mix_hash(hash, static_cast<std::uint32_t>(config_.initial_synapse_weight));
    mix_hash(hash, static_cast<std::uint32_t>(config_.maximum_synapse_weight));
    mix_hash(hash, config_.input_weight_budget);
    mix_hash(hash, config_.refractory_input_events);
    mix_hash(hash, static_cast<std::uint8_t>(config_.recovery_mode));
    mix_hash(hash, config_.recovery_delay);
    mix_hash(hash, config_.membrane_relaxation_percent);
    mix_hash(hash, config_.local_event_memory_capacity);
    mix_hash(hash, static_cast<std::uint8_t>(config_.eligibility_selection));
    mix_hash(hash, static_cast<std::uint8_t>(config_.eligibility_representation));
    mix_hash(hash, static_cast<std::uint8_t>(config_.eligibility_history));
    mix_hash(hash, config_.eligibility_candidate_capacity);
    mix_hash(hash, config_.eligibility_decision_capacity);
    mix_hash(hash, config_.eligibility_duration);
    mix_hash(hash, config_.maximum_events_per_run);
    mix_hash(hash, config_.learning_enabled ? 1 : 0);
    mix_hash(hash, config_.growth_enabled ? 1 : 0);
    mix_hash(hash, static_cast<std::uint8_t>(config_.reset_mode));
    mix_hash(hash, metrics_.processed_events);
    mix_hash(hash, metrics_.transmitted_impulses);
    mix_hash(hash, metrics_.fire_events);
    mix_hash(hash, metrics_.completed_activity_phases);
    mix_hash(hash, metrics_.synaptic_strengthenings);
    mix_hash(hash, metrics_.rejected_strengthenings_at_weight_limit);
    mix_hash(hash, metrics_.synaptic_weakenings);
    mix_hash(hash, metrics_.refractory_suppressed_firings);
    mix_hash(hash, metrics_.local_event_memory_records);
    mix_hash(hash, metrics_.local_event_memory_overwrites);
    mix_hash(hash, metrics_.scheduled_recovery_events);
    mix_hash(hash, metrics_.processed_recovery_events);
    mix_hash(hash, metrics_.stale_recovery_events);
    mix_hash(hash, metrics_.processed_boundary_events);
    mix_hash(hash, metrics_.eligibility_decisions);
    mix_hash(hash, metrics_.eligibility_candidates);
    mix_hash(hash, metrics_.eligibility_positive_candidates);
    mix_hash(hash, metrics_.eligibility_zero_weight_candidates);
    mix_hash(hash, metrics_.eligibility_overwritten_decisions);
    mix_hash(hash, metrics_.scheduled_eligibility_expiries);
    mix_hash(hash, metrics_.processed_eligibility_expiries);
    mix_hash(hash, metrics_.stale_eligibility_expiries);
    mix_hash(hash, metrics_.growth_attempts);
    mix_hash(hash, metrics_.grown_synapses);
    mix_hash(hash, metrics_.processed_sensor_bytes);
    mix_hash(hash, metrics_.maximum_queue_size);
    mix_hash(hash, metrics_.event_trace_hash);
    mix_hash(hash, simulated_time_);
    mix_hash(hash, next_event_sequence_);
    for (const Event& event : queue_) {
        mix_hash(hash, static_cast<std::uint8_t>(event.kind));
        mix_hash(hash, event.target);
        mix_hash(hash, event.scheduled_time);
        mix_hash(hash, event.sequence);
        mix_hash(hash, event.generation);
    }
    return hash;
}

std::uint64_t Runtime::simulated_time() const noexcept {
    return simulated_time_;
}

std::size_t Runtime::future_event_count() const noexcept {
    return static_cast<std::size_t>(std::count_if(
        queue_.begin(), queue_.end(),
        [this](const Event& event) { return event.scheduled_time > simulated_time_; }));
}

void Runtime::save(const std::string& path) const {
    if (motor_output_.active()) {
        throw std::logic_error("saving during an active motor response is not supported");
    }
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream) {
        throw std::runtime_error("cannot open state for writing: " + path);
    }

    write_bytes(stream, kPersistenceMagic.data(), kPersistenceMagic.size());
    write_u32(stream, kPersistenceVersion);
    write_config(stream, config_);
    write_u64(stream, rng_.state());
    write_metrics(stream, metrics_);

    write_u32(stream, network_.width());
    write_u32(stream, network_.height());
    write_u32(stream, network_.depth());
    write_u32(stream, network_.neuron_count());
    for (const neural::Neuron& neuron : network_.neurons()) {
        write_i64(stream, neuron.membrane_potential);
        write_i64(stream, neuron.firing_threshold);
        write_u32(stream, neuron.input_event_count);
        write_u32(stream, neuron.refractory_events_remaining);
        write_u64(stream, neuron.recovery_generation);
        write_u64(stream, neuron.scheduled_recovery_time);
        write_u32(stream, neuron.first_incoming);
        write_u32(stream, neuron.first_outgoing);
        write_u32(stream, neuron.incoming_count);
        write_u32(stream, neuron.outgoing_count);
    }

    write_u32(stream, network_.synapse_count());
    for (const neural::Synapse& synapse : network_.synapses()) {
        write_u32(stream, synapse.sender);
        write_u32(stream, synapse.receiver);
        write_i32(stream, synapse.weight);
        write_u32(stream, synapse.activity_count);
        write_u32(stream, synapse.next_incoming);
        write_u32(stream, synapse.next_outgoing);
    }

    for (NeuronId neuron = 0; neuron < network_.neuron_count(); ++neuron) {
        const auto events = network_.local_event_memory(neuron);
        write_u32(stream, static_cast<std::uint32_t>(events.size()));
        for (const SynapseId synapse : events) {
            write_u32(stream, synapse);
        }
        write_u64(stream, network_.local_event_overwrites(neuron));
    }

    for (NeuronId neuron = 0; neuron < network_.neuron_count(); ++neuron) {
        write_u64(stream, network_.eligibility_generation(neuron));
        write_u64(stream, network_.eligibility_overwrites(neuron));
        const auto records = network_.eligibility_records(neuron);
        write_u32(stream, static_cast<std::uint32_t>(records.size()));
        for (const auto& record : records) {
            write_u64(stream, record.generation);
            write_u64(stream, record.expires_at);
            write_u32(stream, static_cast<std::uint32_t>(record.candidates.size()));
            for (const SynapseId candidate : record.candidates) {
                write_u32(stream, candidate);
            }
        }
    }

    write_u64(stream, simulated_time_);
    write_u64(stream, next_event_sequence_);
    write_u64(stream, queue_.size());
    for (const Event& event : queue_) {
        write_u8(stream, static_cast<std::uint8_t>(event.kind));
        write_u32(stream, event.target);
        write_u64(stream, event.scheduled_time);
        write_u64(stream, event.sequence);
        write_u64(stream, event.generation);
    }
    stream.flush();
    if (!stream) {
        throw std::runtime_error("failed to finish writing EVE state: " + path);
    }
}

Runtime Runtime::load(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("cannot open state for reading: " + path);
    }

    std::array<char, kPersistenceMagic.size()> magic{};
    read_bytes(stream, magic.data(), magic.size());
    if (magic != kPersistenceMagic) {
        throw std::runtime_error("not an EVE Experiment 001 state file");
    }
    const std::uint32_t version = read_u32(stream);
    if (version == 0 || version > kPersistenceVersion) {
        throw std::runtime_error("unsupported EVE state version");
    }

    ExperimentConfig config = read_config(stream, version);
    Runtime runtime(config, 1, SkipInitializationTag{});
    runtime.rng_.set_state(read_u64(stream));
    runtime.metrics_ = read_metrics(stream, version);

    const std::uint32_t width = read_u32(stream);
    const std::uint32_t height = read_u32(stream);
    const std::uint32_t depth = read_u32(stream);
    const std::uint32_t neuron_count = read_u32(stream);
    if (width != config.width || height != config.height || depth != config.depth ||
        neuron_count != config.neuron_count) {
        throw std::runtime_error("persisted network geometry differs from its configuration");
    }

    std::vector<neural::Neuron> neurons;
    neurons.reserve(neuron_count);
    for (std::uint32_t index = 0; index < neuron_count; ++index) {
        neural::Neuron neuron;
        neuron.membrane_potential = read_i64(stream);
        neuron.firing_threshold = read_i64(stream);
        neuron.input_event_count = read_u32(stream);
        if (version >= 2) {
            neuron.refractory_events_remaining = read_u32(stream);
        }
        if (version >= 5) {
            neuron.recovery_generation = read_u64(stream);
            neuron.scheduled_recovery_time = read_u64(stream);
        }
        neuron.first_incoming = read_u32(stream);
        neuron.first_outgoing = read_u32(stream);
        neuron.incoming_count = read_u32(stream);
        neuron.outgoing_count = read_u32(stream);
        neurons.push_back(neuron);
    }

    const std::uint32_t synapse_count = read_u32(stream);
    std::vector<neural::Synapse> synapses;
    synapses.reserve(synapse_count);
    for (std::uint32_t index = 0; index < synapse_count; ++index) {
        neural::Synapse synapse;
        synapse.sender = read_u32(stream);
        synapse.receiver = read_u32(stream);
        synapse.weight = read_i32(stream);
        synapse.activity_count = read_u32(stream);
        synapse.next_incoming = read_u32(stream);
        synapse.next_outgoing = read_u32(stream);
        synapses.push_back(synapse);
    }
    runtime.network_.restore(
        width, height, depth, std::move(neurons), std::move(synapses));
    for (const neural::Synapse& synapse : runtime.network_.synapses()) {
        if (synapse.weight < 0 || synapse.weight > config.maximum_synapse_weight) {
            throw std::runtime_error("persisted synapse weight is outside configured bounds");
        }
    }

    if (version >= 4) {
        std::vector<std::vector<SynapseId>> memories(neuron_count);
        std::vector<std::uint64_t> overwrites(neuron_count);
        for (NeuronId neuron = 0; neuron < neuron_count; ++neuron) {
            const std::uint32_t size = read_u32(stream);
            if (size > config.local_event_memory_capacity) {
                throw std::runtime_error("persisted local event memory exceeds capacity");
            }
            memories[neuron].reserve(size);
            for (std::uint32_t index = 0; index < size; ++index) {
                memories[neuron].push_back(read_u32(stream));
            }
            overwrites[neuron] = read_u64(stream);
        }
        runtime.network_.restore_local_event_memory(
            config.local_event_memory_capacity, memories, overwrites);
    } else {
        runtime.network_.restore_local_event_memory(
            0, std::vector<std::vector<SynapseId>>(neuron_count),
            std::vector<std::uint64_t>(neuron_count));
    }

    if (version >= 6) {
        std::vector<std::vector<neural::EligibilityRecord>> records(neuron_count);
        std::vector<std::uint64_t> generations(neuron_count);
        std::vector<std::uint64_t> overwrites(neuron_count);
        for (NeuronId neuron = 0; neuron < neuron_count; ++neuron) {
            generations[neuron] = read_u64(stream);
            overwrites[neuron] = read_u64(stream);
            const std::uint32_t record_count = read_u32(stream);
            if (record_count > config.eligibility_decision_capacity) {
                throw std::runtime_error("persisted eligibility exceeds decision capacity");
            }
            records[neuron].reserve(record_count);
            for (std::uint32_t record_index = 0; record_index < record_count;
                 ++record_index) {
                neural::EligibilityRecord record;
                record.generation = read_u64(stream);
                record.expires_at = read_u64(stream);
                const std::uint32_t candidate_count = read_u32(stream);
                if (candidate_count > config.eligibility_candidate_capacity) {
                    throw std::runtime_error(
                        "persisted eligibility exceeds candidate capacity");
                }
                record.candidates.reserve(candidate_count);
                for (std::uint32_t index = 0; index < candidate_count; ++index) {
                    record.candidates.push_back(read_u32(stream));
                }
                records[neuron].push_back(std::move(record));
            }
        }
        runtime.network_.restore_eligibility(
            config.eligibility_candidate_capacity,
            config.eligibility_decision_capacity,
            records, generations, overwrites);
    } else {
        runtime.network_.restore_eligibility(
            0, 0,
            std::vector<std::vector<neural::EligibilityRecord>>(neuron_count),
            std::vector<std::uint64_t>(neuron_count),
            std::vector<std::uint64_t>(neuron_count));
    }

    if (version >= 5) {
        runtime.simulated_time_ = read_u64(stream);
        runtime.next_event_sequence_ = read_u64(stream);
    }
    const std::uint64_t queue_size = read_u64(stream);
    if (queue_size > config.maximum_events_per_run) {
        throw std::runtime_error("persisted event queue exceeds the configured event limit");
    }
    for (std::uint64_t index = 0; index < queue_size; ++index) {
        const std::uint8_t kind_value = read_u8(stream);
        const std::uint8_t maximum_kind = version >= 6
            ? static_cast<std::uint8_t>(EventKind::eligibility_expiry)
            : version >= 5 ? static_cast<std::uint8_t>(EventKind::boundary)
            : static_cast<std::uint8_t>(EventKind::impulse);
        if (kind_value > maximum_kind) {
            throw std::runtime_error("invalid event kind in EVE state");
        }
        Event event;
        event.kind = static_cast<EventKind>(kind_value);
        event.target = read_u32(stream);
        if (version >= 5) {
            event.scheduled_time = read_u64(stream);
            event.sequence = read_u64(stream);
            event.generation = read_u64(stream);
        } else {
            event.sequence = runtime.next_event_sequence_++;
        }
        if (event.kind == EventKind::fire) {
            static_cast<void>(runtime.network_.neuron(event.target));
        } else if (event.kind == EventKind::impulse) {
            static_cast<void>(runtime.network_.synapse(event.target));
        } else if (event.kind == EventKind::recovery ||
                   event.kind == EventKind::eligibility_expiry) {
            static_cast<void>(runtime.network_.neuron(event.target));
        }
        runtime.queue_.push_back(event);
    }
    runtime.update_queue_watermark();

    if (stream.peek() != std::char_traits<char>::eof()) {
        throw std::runtime_error("unexpected trailing data in EVE state");
    }
    return runtime;
}

void Runtime::validate_config() const {
    if (config_.neuron_count == 0 || config_.width == 0 || config_.height == 0 ||
        config_.depth == 0) {
        throw std::invalid_argument("network dimensions and neuron count must be non-zero");
    }
    const std::uint64_t volume = static_cast<std::uint64_t>(config_.width) *
                                 config_.height * config_.depth;
    if (volume != config_.neuron_count) {
        throw std::invalid_argument("geometry volume must equal neuron count");
    }
    if (config_.initial_outgoing_synapses >= config_.neuron_count) {
        throw std::invalid_argument(
            "initial outgoing count must leave at least one non-self target");
    }
    if (config_.firing_threshold <= 0 || config_.synaptic_learning_threshold == 0 ||
        config_.activity_phase_limit == 0 || config_.maximum_events_per_run == 0) {
        throw std::invalid_argument("thresholds, phase limit and event limit must be positive");
    }
    if (config_.initial_synapse_weight < 0 || config_.maximum_synapse_weight < 0 ||
        config_.initial_synapse_weight > config_.maximum_synapse_weight) {
        throw std::invalid_argument("Experiment 001 weights must be non-negative and bounded");
    }
    if (config_.input_weight_budget == 0) {
        throw std::invalid_argument("input weight budget must be positive or unlimited");
    }
    if (config_.recovery_mode != RecoveryMode::input_dependent &&
        config_.recovery_delay == 0) {
        throw std::invalid_argument("independent recovery delay must be positive");
    }
    if (config_.membrane_relaxation_percent > 100) {
        throw std::invalid_argument("membrane relaxation must be between 0 and 100 percent");
    }
    if (static_cast<std::uint8_t>(config_.eligibility_selection) >
            static_cast<std::uint8_t>(EligibilitySelection::positive_weight) ||
        static_cast<std::uint8_t>(config_.eligibility_representation) >
            static_cast<std::uint8_t>(EligibilityRepresentation::unique) ||
        static_cast<std::uint8_t>(config_.eligibility_history) >
            static_cast<std::uint8_t>(EligibilityHistory::merge_unique)) {
        throw std::invalid_argument("invalid eligibility mode");
    }
    if (config_.recovery_mode !=
            RecoveryMode::independent_with_membrane_relaxation &&
        config_.membrane_relaxation_percent != 0) {
        throw std::invalid_argument("membrane relaxation requires recovery mode R2");
    }
    const bool eligibility_enabled =
        config_.eligibility_selection != EligibilitySelection::none;
    if (eligibility_enabled &&
        (config_.local_event_memory_capacity == 0 ||
         config_.eligibility_candidate_capacity == 0 ||
         config_.eligibility_decision_capacity == 0)) {
        throw std::invalid_argument(
            "eligibility requires local memory and positive candidate/decision capacities");
    }
    if (!eligibility_enabled &&
        (config_.eligibility_candidate_capacity != 0 ||
         config_.eligibility_decision_capacity != 0 ||
         config_.eligibility_duration != 0)) {
        throw std::invalid_argument(
            "disabled eligibility must not allocate candidate or decision state");
    }
}

void Runtime::process_fire(const NeuronId neuron) {
    const auto outgoing = network_.outgoing_synapses(neuron);
    ++metrics_.fire_events;
    motor_output_.observe_fire(neuron, metrics_.processed_events);
    if (fire_observer_) {
        fire_observer_(FireObservation{
            metrics_.processed_events,
            simulated_time_,
            neuron,
            static_cast<std::uint32_t>(outgoing.size()),
        });
    }
    if (trace_stream_ != nullptr) {
        trace("event fire neuron=" + std::to_string(neuron) + " outgoing=" +
              std::to_string(outgoing.size()));
    }

    for (const SynapseId synapse : outgoing) {
        enqueue_event(EventKind::impulse, synapse, simulated_time_);
        ++metrics_.transmitted_impulses;
    }

    if (config_.growth_enabled) {
        ++metrics_.growth_attempts;
        const auto grown = brainstem_.attempt_growth(network_, neuron, config_, rng_);
        if (grown.has_value()) {
            ++metrics_.grown_synapses;
            const neural::Synapse& item = network_.synapse(*grown);
            if (trace_stream_ != nullptr) {
                trace("event growth synapse=" + std::to_string(*grown) + " sender=" +
                      std::to_string(item.sender) + " receiver=" +
                      std::to_string(item.receiver));
            }
        }
    }
}

void Runtime::process_impulse(const SynapseId synapse) {
    const neural::Synapse before = network_.synapse(synapse);
    std::int64_t membrane_before = 0;
    std::uint32_t refractory_before = 0;
    if (impulse_observer_) {
        const neural::Neuron& receiver = network_.neuron(before.receiver);
        membrane_before = receiver.membrane_potential;
        refractory_before = receiver.refractory_events_remaining;
    }
    const brainstem::ImpulseResult result =
        brainstem_.receive_impulse(network_, synapse, config_, &rng_);
    brainstem::EligibilityResult eligibility;
    if (result.fired &&
        config_.eligibility_selection != EligibilitySelection::none) {
        if (config_.eligibility_duration >
            std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
            throw std::overflow_error("eligibility expiry time overflow");
        }
        eligibility = brainstem_.create_eligibility(
            network_, before.receiver,
            simulated_time_ + config_.eligibility_duration, config_);
        if (eligibility.created) {
            ++metrics_.eligibility_decisions;
            metrics_.eligibility_candidates += eligibility.candidates.size();
            metrics_.eligibility_positive_candidates +=
                eligibility.positive_candidates;
            metrics_.eligibility_zero_weight_candidates +=
                eligibility.zero_weight_candidates;
            metrics_.eligibility_overwritten_decisions +=
                eligibility.overwritten_decisions;
        }
    }
    if (result.local_event_recorded) {
        ++metrics_.local_event_memory_records;
    }
    if (result.local_event_overwritten) {
        ++metrics_.local_event_memory_overwrites;
    }
    if (result.strengthened) {
        ++metrics_.synaptic_strengthenings;
    }
    if (result.strengthening_rejected_by_weight_limit) {
        ++metrics_.rejected_strengthenings_at_weight_limit;
    }
    if (result.weakened) {
        ++metrics_.synaptic_weakenings;
    }
    if (result.firing_suppressed) {
        ++metrics_.refractory_suppressed_firings;
    }
    if (result.phase_completed) {
        ++metrics_.completed_activity_phases;
    }

    if (trace_stream_ != nullptr) {
        std::ostringstream line;
        line << "event impulse synapse=" << synapse << " sender=" << before.sender
             << " receiver=" << before.receiver << " weight=" << result.applied_weight
             << " membrane=" << result.membrane_after
             << " strengthened=" << (result.strengthened ? 1 : 0)
             << " phase=" << (result.phase_completed ? 1 : 0)
             << " fired=" << (result.fired ? 1 : 0);
        trace(line.str());
    }
    if (impulse_observer_) {
        const neural::Neuron& receiver_after = network_.neuron(before.receiver);
        impulse_observer_(ImpulseObservation{
            metrics_.processed_events,
            simulated_time_,
            synapse,
            before.sender,
            before.receiver,
            result.applied_weight,
            membrane_before,
            receiver_after.membrane_potential,
            refractory_before,
            receiver_after.refractory_events_remaining,
            result.fired,
            result.firing_suppressed,
            network_.local_event_memory(before.receiver),
            eligibility.generation,
            eligibility.candidates,
        });
    }

    if (result.fired) {
        enqueue_event(EventKind::fire, before.receiver, simulated_time_);
        if (config_.recovery_mode != RecoveryMode::input_dependent) {
            if (config_.recovery_delay >
                std::numeric_limits<std::uint64_t>::max() - simulated_time_) {
                throw std::overflow_error("recovery time overflow");
            }
            const std::uint64_t recovery_time = simulated_time_ + config_.recovery_delay;
            network_.neuron(before.receiver).scheduled_recovery_time = recovery_time;
            enqueue_event(
                EventKind::recovery, before.receiver, recovery_time,
                result.recovery_generation);
            ++metrics_.scheduled_recovery_events;
        }
        if (eligibility.created) {
            enqueue_event(
                EventKind::eligibility_expiry, before.receiver,
                eligibility.expires_at, eligibility.generation);
            ++metrics_.scheduled_eligibility_expiries;
        }
    }
}

void Runtime::process_recovery(const Event& event) {
    const brainstem::RecoveryResult result = brainstem_.recover_neuron(
        network_, event.target, event.generation, config_);
    if (result.recovered) {
        ++metrics_.processed_recovery_events;
    }
    if (result.stale) {
        ++metrics_.stale_recovery_events;
    }
    if (recovery_observer_) {
        recovery_observer_(RecoveryObservation{
            metrics_.processed_events,
            simulated_time_,
            event.target,
            event.generation,
            result.membrane_before,
            result.membrane_after,
            result.refractory_before,
            result.refractory_after,
            result.recovered,
            result.stale,
        });
    }
}

void Runtime::process_eligibility_expiry(const Event& event) {
    if (brainstem_.expire_eligibility(
            network_, event.target, event.generation)) {
        ++metrics_.processed_eligibility_expiries;
    } else {
        ++metrics_.stale_eligibility_expiries;
    }
}

void Runtime::process_event(const Event& event) {
    if (event.scheduled_time > simulated_time_) {
        throw std::logic_error("attempted to process a future event");
    }
    ++metrics_.processed_events;
    update_event_hash(event);
    switch (event.kind) {
    case EventKind::fire:
        process_fire(event.target);
        break;
    case EventKind::impulse:
        process_impulse(event.target);
        break;
    case EventKind::recovery:
        process_recovery(event);
        break;
    case EventKind::boundary:
        ++metrics_.processed_boundary_events;
        break;
    case EventKind::eligibility_expiry:
        process_eligibility_expiry(event);
        break;
    }
}

void Runtime::enqueue_event(
    const EventKind kind,
    const std::uint32_t target,
    const std::uint64_t scheduled_time,
    const std::uint64_t generation) {
    Event event{kind, target, scheduled_time, next_event_sequence_++, generation};
    const auto position = std::upper_bound(
        queue_.begin(), queue_.end(), event,
        [](const Event& left, const Event& right) {
            if (left.scheduled_time != right.scheduled_time) {
                return left.scheduled_time < right.scheduled_time;
            }
            return left.sequence < right.sequence;
        });
    queue_.insert(position, event);
    update_queue_watermark();
}

bool Runtime::has_ready_event() const noexcept {
    return !queue_.empty() && queue_.front().scheduled_time <= simulated_time_;
}

Event Runtime::pop_next_event() {
    if (queue_.empty()) {
        throw std::logic_error("cannot pop an empty event queue");
    }
    const Event event = queue_.front();
    queue_.pop_front();
    return event;
}

void Runtime::update_queue_watermark() noexcept {
    if (queue_.size() > metrics_.maximum_queue_size) {
        metrics_.maximum_queue_size = queue_.size();
    }
}

void Runtime::update_event_hash(const Event& event) noexcept {
    mix_hash(metrics_.event_trace_hash, static_cast<std::uint8_t>(event.kind));
    mix_hash(metrics_.event_trace_hash, event.target);
    mix_hash(metrics_.event_trace_hash, event.scheduled_time);
    mix_hash(metrics_.event_trace_hash, event.sequence);
    mix_hash(metrics_.event_trace_hash, event.generation);
}

void Runtime::trace(const std::string& line) {
    if (trace_stream_ != nullptr) {
        *trace_stream_ << line << '\n';
        if (!*trace_stream_) {
            throw std::runtime_error("failed to write diagnostic event trace");
        }
    }
}

}  // namespace eve::bios
