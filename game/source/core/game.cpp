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

void game::player_data::init_playing(const game::game_data& game, size_t current_index)
{
    this->state = (game.coop() && game.state == game::game_state::playing)
        ? game::player_state::playing // TODO: state for resurrecting during coop
        : game::player_state::playing;

    this->pos = game::constants::PLAYER_START[current_index];
    this->dir = game::constants::PLAYER_START_DIR[current_index];
    this->press = {};
    this->speed_bank = {};
    this->flags.all = {};

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
            debug_fail_msg("Invalid game_type");
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
            debug_fail_msg("Invalid game_type");
            return 0;
    }
}

size_t game::game_data::score_for_tile(game::tile_type tile_type) const
{
    switch (tile_type)
    {
        case game::tile_type::panel0:
            return game::constants::SCORE_PANEL_0;

        case game::tile_type::panel1:
            return game::constants::SCORE_PANEL_1;

        default:
            return 0;
    }
}

ff::fixed_int game::game_data::player_speed(bool press_speed) const
{
    return press_speed
        ? game::constants::PLAYER_SPEED_FAST
        : game::constants::PLAYER_SPEED_SLOW;
}

game::player_status& game::game_data::player_status() const
{
    return *this->players[this->current_player].status;
}

game::level_data& game::game_data::level() const
{
    return *this->players[this->current_player].level;
}

game::player_status& game::play_level::player_status() const
{
    return this->game_data->player_status();
}

game::level_data& game::play_level::level() const
{
    return this->game_data->level();
}

size_t game::play_level::state_counter() const
{
    return this->game_data->state.state_counter;
}

size_t game::play_level::total_counter() const
{
    return this->game_data->state.total_counter;
}
