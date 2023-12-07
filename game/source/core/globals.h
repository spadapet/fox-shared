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
    const ff::point_size TILE_SIZE(TILE_SIZE_X, TILE_SIZE_Y);

    const ff::fixed_int PLAYER_SPEED_SLOW = 1;
    const ff::fixed_int PLAYER_SPEED_FAST = 2;
    constexpr int TILE_TURN_PAST_CENTER = 2;
    constexpr int TILE_COLLECT_NEAR_CENTER = 4;
}

namespace game
{
    enum class dir
    {
        none,
        right,
        up,
        left,
        down,
    };

    template<class T>
    constexpr T dir_to_degrees(game::dir dir)
    {
        return (dir != game::dir::none) * (static_cast<int>(dir) - 1) * static_cast<T>(90.0);
    }

    constexpr bool dir_is_horizontal(game::dir dir)
    {
        return dir == game::dir::left || dir == game::dir::right;
    }

    constexpr bool dir_is_vertical(game::dir dir)
    {
        return dir == game::dir::up || dir == game::dir::down;
    }

    ff::point_int dir_to_point(game::dir dir);
}
