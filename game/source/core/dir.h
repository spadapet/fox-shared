#pragma once

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
        return (dir != game::dir::none) * (static_cast<int>(dir) - 1) * static_cast<T>(90);
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
