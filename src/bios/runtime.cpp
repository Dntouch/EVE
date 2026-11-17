#include "eve/bios/runtime.hpp"

#include <array>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace eve::bios {
namespace {

constexpr std::array<char, 8> kPersistenceMagic{{'E', 'V', 'E', '0', '0', '1', '\r', '\n'}};
constexpr std::uint32_t kPersistenceVersion = 2;
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
    write_u64(stream, metrics.synaptic_weakenings);
    write_u64(stream, metrics.refractory_suppressed_firings);
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
    if (version >= 2) {
        metrics.synaptic_weakenings = read_u64(stream);
        metrics.refractory_suppressed_firings = read_u64(stream);
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
    queue_.push_back(Event{EventKind::fire, neuron});
    update_queue_watermark();
}

std::uint64_t Runtime::run() {
    std::uint64_t processed = 0;
    while (!queue_.empty()) {
        if (processed >= config_.maximum_events_per_run) {
            throw std::runtime_error(
                "event processing limit reached with " + std::to_string(queue_.size()) +
                " events still queued");
        }

        const Event event = queue_.front();
        queue_.pop_front();
        ++processed;
        ++metrics_.processed_events;
        update_event_hash(event);

        if (event.kind == EventKind::fire) {
            process_fire(event.target);
        } else if (event.kind == EventKind::impulse) {
            process_impulse(event.target);
        } else {
            throw std::logic_error("unknown event kind");
        }
    }
    return processed;
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

void Runtime::set_trace_stream(std::ostream* stream) noexcept {
    trace_stream_ = stream;
}

void Runtime::set_fire_observer(FireObserver observer) {
    fire_observer_ = std::move(observer);
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
                                    queue_.size() * sizeof(Event);
    result.queued_events = queue_.size();
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
    mix_hash(hash, config_.maximum_events_per_run);
    mix_hash(hash, config_.learning_enabled ? 1 : 0);
    mix_hash(hash, config_.growth_enabled ? 1 : 0);
    mix_hash(hash, static_cast<std::uint8_t>(config_.reset_mode));
    mix_hash(hash, metrics_.processed_events);
    mix_hash(hash, metrics_.transmitted_impulses);
    mix_hash(hash, metrics_.fire_events);
    mix_hash(hash, metrics_.completed_activity_phases);
    mix_hash(hash, metrics_.synaptic_strengthenings);
    mix_hash(hash, metrics_.synaptic_weakenings);
    mix_hash(hash, metrics_.refractory_suppressed_firings);
    mix_hash(hash, metrics_.growth_attempts);
    mix_hash(hash, metrics_.grown_synapses);
    mix_hash(hash, metrics_.processed_sensor_bytes);
    mix_hash(hash, metrics_.maximum_queue_size);
    mix_hash(hash, metrics_.event_trace_hash);
    for (const Event& event : queue_) {
        mix_hash(hash, static_cast<std::uint8_t>(event.kind));
        mix_hash(hash, event.target);
    }
    return hash;
}

void Runtime::save(const std::string& path) const {
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

    write_u64(stream, queue_.size());
    for (const Event& event : queue_) {
        write_u8(stream, static_cast<std::uint8_t>(event.kind));
        write_u32(stream, event.target);
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

    const std::uint64_t queue_size = read_u64(stream);
    if (queue_size > config.maximum_events_per_run) {
        throw std::runtime_error("persisted event queue exceeds the configured event limit");
    }
    for (std::uint64_t index = 0; index < queue_size; ++index) {
        const std::uint8_t kind_value = read_u8(stream);
        if (kind_value > static_cast<std::uint8_t>(EventKind::impulse)) {
            throw std::runtime_error("invalid event kind in EVE state");
        }
        const Event event{static_cast<EventKind>(kind_value), read_u32(stream)};
        if (event.kind == EventKind::fire) {
            static_cast<void>(runtime.network_.neuron(event.target));
        } else {
            static_cast<void>(runtime.network_.synapse(event.target));
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
}

void Runtime::process_fire(const NeuronId neuron) {
    const auto outgoing = network_.outgoing_synapses(neuron);
    ++metrics_.fire_events;
    if (fire_observer_) {
        fire_observer_(FireObservation{
            metrics_.processed_events,
            neuron,
            static_cast<std::uint32_t>(outgoing.size()),
        });
    }
    trace("event fire neuron=" + std::to_string(neuron) + " outgoing=" +
          std::to_string(outgoing.size()));

    for (const SynapseId synapse : outgoing) {
        queue_.push_back(Event{EventKind::impulse, synapse});
        ++metrics_.transmitted_impulses;
    }
    update_queue_watermark();

    if (config_.growth_enabled) {
        ++metrics_.growth_attempts;
        const auto grown = brainstem_.attempt_growth(network_, neuron, config_, rng_);
        if (grown.has_value()) {
            ++metrics_.grown_synapses;
            const neural::Synapse& item = network_.synapse(*grown);
            trace("event growth synapse=" + std::to_string(*grown) + " sender=" +
                  std::to_string(item.sender) + " receiver=" +
                  std::to_string(item.receiver));
        }
    }
}

void Runtime::process_impulse(const SynapseId synapse) {
    const neural::Synapse before = network_.synapse(synapse);
    const brainstem::ImpulseResult result =
        brainstem_.receive_impulse(network_, synapse, config_, &rng_);
    if (result.strengthened) {
        ++metrics_.synaptic_strengthenings;
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

    std::ostringstream line;
    line << "event impulse synapse=" << synapse << " sender=" << before.sender
         << " receiver=" << before.receiver << " weight=" << result.applied_weight
         << " membrane=" << result.membrane_after
         << " strengthened=" << (result.strengthened ? 1 : 0)
         << " phase=" << (result.phase_completed ? 1 : 0)
         << " fired=" << (result.fired ? 1 : 0);
    trace(line.str());

    if (result.fired) {
        queue_.push_back(Event{EventKind::fire, before.receiver});
        update_queue_watermark();
    }
}

void Runtime::update_queue_watermark() noexcept {
    if (queue_.size() > metrics_.maximum_queue_size) {
        metrics_.maximum_queue_size = queue_.size();
    }
}

void Runtime::update_event_hash(const Event& event) noexcept {
    metrics_.event_trace_hash ^= static_cast<std::uint8_t>(event.kind);
    metrics_.event_trace_hash *= kFnvPrime;
    for (unsigned shift = 0; shift < 32; shift += 8) {
        metrics_.event_trace_hash ^= (event.target >> shift) & 0xffU;
        metrics_.event_trace_hash *= kFnvPrime;
    }
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
