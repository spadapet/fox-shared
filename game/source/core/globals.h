#pragma once

namespace game::strings
{
    extern const std::string_view ID_GAME_OPTIONS;
}

namespace game::input_events
{
    extern const size_t ID_UP;
    extern const size_t ID_DOWN;
    extern const size_t ID_LEFT;
    extern const size_t ID_RIGHT;
    extern const size_t ID_SHOOT_UP;
    extern const size_t ID_SHOOT_DOWN;
    extern const size_t ID_SHOOT_LEFT;
    extern const size_t ID_SHOOT_RIGHT;
    extern const size_t ID_ACTION;
    extern const size_t ID_CANCEL;
    extern const size_t ID_PAUSE;
    extern const size_t ID_START;

    extern const size_t ID_DEBUG_RENDER_TOGGLE;
    extern const size_t ID_DEBUG_INVINCIBLE_TOGGLE;
    extern const size_t ID_DEBUG_COMPLETE_LEVEL;
}

namespace game::constants
{
    constexpr size_t MAX_PLAYERS = 2;
    const ff::point_fixed RENDER_SIZE(480, 270); // 1920x1080 / 4
}
