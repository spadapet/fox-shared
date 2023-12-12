#include "pch.h"
#include "source/core/levels.h"

std::string_view level_tiles[] =
{
    // Level 0
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv,

    // Level 1
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv,
};

static game::level_data get_normal(size_t index)
{
    game::level_data level{};
    level.index = index;
    level.level_type = game::level_type::normal;
    level.max_timer = 120;

    std::string_view tiles = ::level_tiles[index % _countof(::level_tiles)];
    assert(tiles.size() == game::constants::TILE_COUNT_X * game::constants::TILE_COUNT_Y * 2);

    for (size_t i = 0, h = 0; i < tiles.size(); i += 2, h++)
    {
        const char ch = tiles[i];
        switch (ch)
        {
            case 'x':
                level.tiles[h] = game::tile_type::panel0;
                break;

            case 'X':
                level.tiles[h] = game::tile_type::panel1;
                break;

            case 'o':
                level.tiles[h] = game::tile_type::bomb;
                break;
        }
    }

    return level;
}

static game::level_data get_challenge(size_t index)
{
    return {};
}

static game::level_data get_bonus(size_t index)
{
    return {};
}

game::level_data game::get_level(game::game_type type, game::game_diff diff, size_t index)
{
    return ::get_normal(index);
}
