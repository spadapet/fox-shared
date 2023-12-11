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

bool game::game_data::coop() const
{
    return this->game_type == game::game_type::coop;
}

size_t game::game_data::total_player_count() const
{
    switch (this->game_type)
    {
        case game::game_type::one_player:
            return 1;

        case game::game_type::coop:
        case game::game_type::two_player:
            return 2;

        default:
            return 0;
    }
}

size_t game::game_data::current_player_count() const
{
    switch (this->game_type)
    {
        case game::game_type::one_player:
        case game::game_type::two_player:
            return 1;

        case game::game_type::coop:
            return 2;

        default:
            return 0;
    }
}

game::player_score& game::game_data::score() const
{
    return *this->players[this->current_player].score;
}

game::level_data& game::game_data::level() const
{
    return *this->players[this->current_player].level;
}
