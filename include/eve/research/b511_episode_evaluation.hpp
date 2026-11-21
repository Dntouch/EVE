#pragma once

#include <cstdint>

namespace eve::research {

struct B511MotorActivity {
    bool early_a{};
    bool early_b{};
    bool response_a{};
    bool response_b{};
    bool exploration_attempted{};
    bool exploration_a{};
    bool exploration_b{};
};

inline bool b511_episode_success(
    const bool target_a, const B511MotorActivity& activity) noexcept {
    if (activity.early_a || activity.early_b) return false;
    const bool observed_a = activity.response_a ||
        (activity.exploration_attempted && activity.exploration_a);
    const bool observed_b = activity.response_b ||
        (activity.exploration_attempted && activity.exploration_b);
    return target_a ? observed_a && !observed_b
                    : observed_b && !observed_a;
}

inline std::int8_t b511_episode_modulation(
    const bool target_a, const B511MotorActivity& activity) noexcept {
    return static_cast<std::int8_t>(
        b511_episode_success(target_a, activity) ? 1 : -1);
}

}  // namespace eve::research
