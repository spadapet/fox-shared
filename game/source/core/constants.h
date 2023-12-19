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
    extern const size_t ID_SPEED;
    extern const size_t ID_SHOOT;
    extern const size_t ID_ACTION;
    extern const size_t ID_CANCEL;
    extern const size_t ID_PAUSE;

    extern const size_t ID_DEBUG_RENDER_TOGGLE;
    extern const size_t ID_DEBUG_INVINCIBLE_TOGGLE;
    extern const size_t ID_DEBUG_COMPLETE_LEVEL;
}

namespace game::constants
{
    constexpr size_t MAX_PLAYERS = 2;
    constexpr size_t MAX_SHOOTERS = 2;
    constexpr size_t MAX_SHOTS = 16;
    constexpr size_t RENDER_SIZE_X = 480; // 1920/4
    constexpr size_t RENDER_SIZE_Y = 270; // 1080/4
    constexpr size_t TILE_SIZE_X = 30;
    constexpr size_t TILE_SIZE_Y = 18;
    constexpr size_t TILE_COUNT_X = 16;
    constexpr size_t TILE_COUNT_Y = 15;
    constexpr size_t TILE_COUNT_MEM_Y = 16; // Extra row is stored just to make 256 tiles total
    constexpr size_t TILE_COUNT = TILE_COUNT_X * TILE_COUNT_Y;
    constexpr size_t TILE_COUNT_MEM = TILE_COUNT_X * TILE_COUNT_MEM_Y;

    const ff::point_size RENDER_SIZE(RENDER_SIZE_X, RENDER_SIZE_Y);
    const ff::rect_size MOVABLE_TILES(2, 2, TILE_COUNT_X - 2, TILE_COUNT_Y - 2);
    const ff::point_int TILE_SIZE(static_cast<int>(TILE_SIZE_X), static_cast<int>(TILE_SIZE_Y));
    const ff::rect_int MOVABLE_AREA = MOVABLE_TILES.cast<int>() * TILE_SIZE;
    const ff::rect_int MOVABLE_AREA_CENTER_TILE = MOVABLE_AREA.deflate(TILE_SIZE / 2);

    const ff::fixed_int SHOOTER_SPEED_NORMAL = 2;
    const ff::fixed_int SHOT_SPEED_NORMAL = 1.125;
    const ff::fixed_int PLAYER_SPEED_SLOW = 1.25;
    const ff::fixed_int PLAYER_SPEED_FAST = 2.5;
    constexpr int TILE_TURN_PAST_CENTER = 2;
    constexpr int TILE_COLLECT_NEAR_CENTER = 4;

    const ff::point_int PLAYER_START[2] = { { 255, 126 }, { 225, 126 } };

    constexpr size_t STATE_PLAY_READY_TIME = 90;
    constexpr size_t STATE_GAME_WINNING_TIME = 120;
    constexpr size_t STATE_GAME_DYING_TIME = 120;
    constexpr size_t STATE_GAME_OVER_TIME = 120;
    constexpr size_t STATE_PLAYER_DYING_TIME = 120;

    constexpr size_t SCORE_PANEL_0 = 10;
    constexpr size_t SCORE_PANEL_1 = 20;
}
