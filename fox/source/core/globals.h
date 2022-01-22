#pragma once

namespace fox::strings
{
    extern const std::string_view ID_GAME_OPTIONS;
}

namespace fox::input_events
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
    extern const size_t ID_SHOW_CUSTOM_DEBUG;
}

namespace fox::constants
{
    const size_t MAX_PLAYERS = 2;

    const ff::fixed_int RENDER_WIDTH = 480; // 1920 / 4
    const ff::fixed_int RENDER_HEIGHT = 270; // 1080 / 4
    const ff::point_fixed RENDER_SIZE(RENDER_WIDTH, RENDER_HEIGHT);
    const ff::rect_fixed RENDER_RECT(0, 0, RENDER_WIDTH, RENDER_HEIGHT);

    const ff::fixed_int RENDER_SCALE = 4; // scale to a 1080p buffer, which then gets scaled to the screen

    const ff::fixed_int RENDER_WIDTH_HIGH = 1920;
    const ff::fixed_int RENDER_HEIGHT_HIGH = 1080;
    const ff::point_fixed RENDER_SIZE_HIGH(RENDER_WIDTH_HIGH, RENDER_HEIGHT_HIGH);
    const ff::rect_fixed RENDER_RECT_HIGH(0, 0, RENDER_WIDTH_HIGH, RENDER_HEIGHT_HIGH);
}
