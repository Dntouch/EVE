#include "eve/bios/runtime.hpp"
#include "eve/brainstem/brainstem.hpp"
#include "eve/neural/network.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct CheckResult {
    std::string name;
    std::string category;
    bool passed{};
    std::string detail;
};

void require(const bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

eve::ExperimentConfig safety_config(const bool growth_enabled) {
    eve::ExperimentConfig config;
    config.neuron_count = 32;
    config.width = 4;
    config.height = 4;
    config.depth = 2;
    config.initial_outgoing_synapses = 0;
    config.firing_threshold = 2;
    config.maximum_synapse_weight = 4;
    config.learning_enabled = false;
    config.growth_enabled = growth_enabled;
    config.maximum_events_per_run = 10'000;
    return config;
}

eve::brainstem::StructuralProtectionContract safety_contract() {
    auto contract =
        eve::brainstem::StructuralProtectionContract::protected_default(32);
    contract.neuron_roles[0] = eve::brainstem::StructuralRole::innate_sensory;
    contract.neuron_roles[1] = eve::brainstem::StructuralRole::innate_motor;
    contract.neuron_roles[2] =
        eve::brainstem::StructuralRole::evaluation_feedback;
    contract.neuron_roles[3] =
        eve::brainstem::StructuralRole::protected_brainstem;
    contract.neuron_roles[4] =
        eve::brainstem::StructuralRole::transition_contact;
    return contract;
}

eve::brainstem::AutonomousExplorationConfig safety_exploration() {
    eve::brainstem::AutonomousExplorationConfig config;
    config.enabled = true;
    config.activation_budget = 8;
    config.trigger_probability_denominator = 1;
    config.maximum_sender_outgoing = 4;
    config.local_radius = 2;
    config.seed = 0xC0A9ULL;
    return config;
}

eve::bios::Runtime protected_runtime(const std::uint64_t seed = 0xC0A0ULL) {
    eve::bios::Runtime runtime(safety_config(true), seed);
    runtime.configure_structural_protection(safety_contract());
    runtime.configure_autonomous_exploration(safety_exploration());
    return runtime;
}

bool same_contract(
    const eve::brainstem::StructuralProtectionContract& left,
    const eve::brainstem::StructuralProtectionContract& right) {
    return left.enabled == right.enabled &&
        left.neuron_roles == right.neuron_roles &&
        left.allowed_connections == right.allowed_connections;
}

std::string csv_text(std::string value) {
    std::string escaped;
    escaped.reserve(value.size() + 2);
    escaped.push_back('"');
    for (const char item : value) {
        if (item == '"') escaped.push_back('"');
        escaped.push_back(item);
    }
    escaped.push_back('"');
    return escaped;
}

}  // namespace

int main(int argc, char** argv) {
    const std::filesystem::path output = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("structural-safety-preflight.csv");
    std::vector<CheckResult> results;
    const auto check = [&results](
                           const std::string& name,
                           const std::string& category,
                           const std::function<void()>& action) {
        try {
            action();
            results.push_back(CheckResult{name, category, true, "passed"});
        } catch (const std::exception& error) {
            results.push_back(CheckResult{name, category, false, error.what()});
        }
    };

    check("forbidden_role_directions", "role_matrix", [] {
        const auto contract = safety_contract();
        constexpr std::array<eve::NeuronId, 4> protected_ids{{0, 1, 2, 3}};
        constexpr std::array<eve::NeuronId, 3> adaptive_ids{{4, 5, 6}};
        for (const auto protected_id : protected_ids) {
            for (const auto adaptive_id : adaptive_ids) {
                require(!contract.permits(protected_id, adaptive_id),
                        "protected role may send Growth");
                require(!contract.permits(adaptive_id, protected_id),
                        "protected role may receive Growth");
            }
        }
    });

    check("allowed_adaptive_directions", "role_matrix", [] {
        const auto contract = safety_contract();
        require(contract.permits(5, 6), "free-to-free Growth rejected");
        require(contract.permits(5, 4), "free-to-transition Growth rejected");
        require(contract.permits(4, 5), "transition-to-free Growth rejected");
        require(contract.permits(4, 4),
                "transition-to-transition connection type rejected");
    });

    check("missing_contract_rejected", "invalid_configuration", [] {
        eve::bios::Runtime runtime(safety_config(true), 0xC0A1ULL);
        bool rejected = false;
        try {
            runtime.configure_autonomous_exploration(safety_exploration());
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "combined autonomy accepted without a contract");
    });

    check("incomplete_inventory_rejected", "invalid_configuration", [] {
        eve::bios::Runtime runtime(safety_config(false), 0xC0A2ULL);
        auto contract = safety_contract();
        contract.neuron_roles.pop_back();
        bool rejected = false;
        try {
            runtime.configure_structural_protection(contract);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "incomplete role inventory accepted");
    });

    check("invalid_role_rejected", "invalid_configuration", [] {
        eve::bios::Runtime runtime(safety_config(false), 0xC0A3ULL);
        auto contract = safety_contract();
        contract.neuron_roles[7] =
            static_cast<eve::brainstem::StructuralRole>(255);
        bool rejected = false;
        try {
            runtime.configure_structural_protection(contract);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "unknown structural role accepted");
    });

    check("contradictory_matrix_rejected", "invalid_configuration", [] {
        eve::bios::Runtime runtime(safety_config(true), 0xC0A4ULL);
        auto contract = safety_contract();
        const auto free = static_cast<std::size_t>(
            eve::brainstem::StructuralRole::free_processing);
        const auto sensory = static_cast<std::size_t>(
            eve::brainstem::StructuralRole::innate_sensory);
        contract.allowed_connections[free][sensory] = true;
        runtime.configure_structural_protection(contract);
        bool rejected = false;
        try {
            runtime.configure_autonomous_exploration(safety_exploration());
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "unsafe direction matrix accepted for autonomy");
    });

    check("later_growth_enable_rejected", "reconfiguration", [] {
        eve::bios::Runtime runtime(safety_config(false), 0xC0A5ULL);
        runtime.configure_autonomous_exploration(safety_exploration());
        bool rejected = false;
        try {
            runtime.set_structural_growth_enabled(true);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "later Growth activation bypassed protection");
    });

    check("active_contract_change_rejected", "reconfiguration", [] {
        auto runtime = protected_runtime(0xC0A6ULL);
        auto disabled = runtime.structural_protection();
        disabled.enabled = false;
        bool rejected = false;
        try {
            runtime.configure_structural_protection(disabled);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "active combined mode allowed contract removal");
    });

    check("persistence_and_restore", "persistence", [&output] {
        auto original = protected_runtime(0xC0A7ULL);
        original.enqueue_fire(5);
        const auto state = output.string() + ".state";
        original.save(state);
        auto restored = eve::bios::Runtime::load(state);
        require(original.state_fingerprint() == restored.state_fingerprint(),
                "restored protection state differs before continuation");
        static_cast<void>(original.run());
        static_cast<void>(restored.run());
        require(original.state_fingerprint() == restored.state_fingerprint(),
                "restored protection state diverged during continuation");
        auto disabled = restored.structural_protection();
        disabled.enabled = false;
        bool rejected = false;
        try {
            restored.configure_structural_protection(disabled);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        std::error_code ignored;
        std::filesystem::remove(state, ignored);
        require(rejected, "restored combined mode allowed contract removal");
    });

    check("provoked_protected_sender_growth", "forced_growth", [] {
        const auto config = safety_config(true);
        eve::neural::Network network;
        eve::brainstem::Brainstem brainstem;
        eve::bios::DeterministicRng initialization_rng(0xC0A8ULL);
        eve::bios::DeterministicRng growth_rng(0xC0A9ULL);
        brainstem.initialize_network(network, config, initialization_rng);
        const auto contract = safety_contract();
        const auto before = network.synapse_count();
        const auto grown = brainstem.attempt_growth(
            network, 0, config, growth_rng, &contract);
        require(!grown.has_value() && network.synapse_count() == before,
                "provoked protected sender created a connection");
    });

    check("provoked_protected_target_growth", "forced_growth", [] {
        const auto config = safety_config(true);
        eve::neural::Network network;
        eve::brainstem::Brainstem brainstem;
        eve::bios::DeterministicRng initialization_rng(0xC0AAULL);
        eve::bios::DeterministicRng growth_rng(0xC0ABULL);
        brainstem.initialize_network(network, config, initialization_rng);
        const auto contract = safety_contract();
        constexpr eve::NeuronId sender = 5;
        for (eve::NeuronId target = 0; target < config.neuron_count; ++target) {
            if (target != sender && contract.permits(sender, target)) {
                static_cast<void>(network.add_synapse(sender, target, 0));
            }
        }
        const auto before = network.synapse_count();
        for (std::uint32_t attempt = 0; attempt < 2'048; ++attempt) {
            static_cast<void>(brainstem.attempt_growth(
                network, sender, config, growth_rng, &contract));
        }
        require(network.synapse_count() == before,
                "Growth used a protected target after safe targets were exhausted");
    });

    check("neural_state_cannot_change_contract", "independence", [] {
        auto config = safety_config(false);
        config.firing_threshold = 1;
        config.input_weight_budget = 16;
        config.local_event_memory_capacity = 8;
        config.eligibility_selection = eve::EligibilitySelection::all_recent;
        config.eligibility_candidate_capacity = 8;
        config.eligibility_decision_capacity = 8;
        config.eligibility_duration = 4;
        config.plasticity_trace_mode =
            eve::PlasticityTraceMode::membrane_participation;
        config.modulation_mode = eve::ModulationMode::ternary;
        eve::bios::Runtime runtime(config, 0xC0ACULL);
        runtime.configure_structural_protection(safety_contract());

        eve::brainstem::EvaluationRouteConfig route_config;
        route_config.mode =
            eve::brainstem::EvaluationRouteMode::general_wiring;
        route_config.first_input_neuron = 9;
        route_config.graded_channel_count = 5;
        route_config.fanout_per_source = 1;
        route_config.weight = 1;
        route_config.seed = 0xC0ADULL;
        for (eve::NeuronId id = 0; id < 9; ++id) {
            route_config.reserved_neurons.push_back(id);
        }
        const auto route = runtime.configure_evaluation_route(route_config);
        constexpr eve::NeuronId sender = 5;
        constexpr eve::NeuronId receiver = 7;
        for (const auto input : route.input_neurons) {
            static_cast<void>(
                runtime.add_experimental_synapse(input, receiver, 0));
        }
        const auto plastic =
            runtime.add_experimental_synapse(sender, receiver, 0);
        const auto before = runtime.structural_protection();

        runtime.enqueue_fire(sender);
        static_cast<void>(runtime.run());
        require(runtime.network().active_eligibility_count() > 0,
                "neural activity created no plasticity trace");
        const auto feedback = runtime.process_modulatory_feedback(1);
        require(feedback.zero_to_one_changes > 0 &&
                    runtime.network().synapse(plastic).weight == 1,
                "independence check executed no plastic weight write");
        require(same_contract(before, runtime.structural_protection()),
                "neural plasticity changed the structural protection contract");
    });

    std::ofstream stream(output);
    if (!stream) {
        std::cerr << "cannot write structural safety result: " << output << '\n';
        return 2;
    }
    stream << "check,category,passed,detail\n";
    std::size_t failures = 0;
    for (const auto& result : results) {
        failures += !result.passed;
        stream << result.name << ',' << result.category << ',' << result.passed
               << ',' << csv_text(result.detail) << '\n';
    }
    stream << "integrity_gate,summary," << (failures == 0) << ','
           << csv_text(failures == 0
                   ? "all independent structural safety checks passed"
                   : std::to_string(failures) + " structural safety checks failed")
           << '\n';
    std::cout << "structural safety preflight "
              << (failures == 0 ? "passed" : "failed") << " checks="
              << results.size() << " failures=" << failures << '\n';
    return failures == 0 ? 0 : 1;
}
