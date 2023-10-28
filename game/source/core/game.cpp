#include "pch.h"
#include "source/core/game.h"

game::tile_type game::level_data::tile(ff::point_size pos) const
{
    return (pos.x < game::constants::TILE_COUNT_X && pos.y < game::constants::TILE_COUNT_Y)
        ? this->tiles[pos.y * game::constants::TILE_COUNT_X + pos.x]
        : game::tile_type::none;
}

void game::level_data::tile(ff::point_size pos, game::tile_type value)
{
    if (pos.x < game::constants::TILE_COUNT_X && pos.y < game::constants::TILE_COUNT_Y)
    {
        this->tiles[pos.y * game::constants::TILE_COUNT_X + pos.x] = value;
    }
}
