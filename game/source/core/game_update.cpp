#include "pch.h"
#include "source/core/game.h"
#include "source/core/game_audio.h"
#include "source/core/game_update.h"

game::game_update::game_update()
{}

void game::game_update::update(game::play_level& play)
{
    if (!play.game_data || !play.level || play.game_data->state != game::game_state::playing)
    {
        return;
    }

    play.level->counter++;

    for (game::player_data& player : play.game_data->players)
    {
        player.counter++;
        player.speed += player.fast ? game::constants::PLAYER_SPEED_FAST : game::constants::PLAYER_SPEED_SLOW;

        for (; player.speed >= 1_f; player.speed--)
        {
            this->update_player(play, player);
        }
    }
}

void game::game_update::update_player(game::play_level& play, game::player_data& player)
{
    const ff::point_int tile_count = ff::point_size(game::constants::TILE_COUNT_X, game::constants::TILE_COUNT_Y).cast<int>();
    const ff::point_int tile_size = game::constants::TILE_SIZE.cast<int>();
    ff::point_int tile = player.pos / tile_size;
    ff::point_int tile_center = tile * tile_size + tile_size / 2;

    const bool was_facing_x = game::dir_is_horizontal(player.dir);
    const bool was_facing_y = game::dir_is_vertical(player.dir);
    const bool can_turn =
        (player.dir == game::dir::right && player.pos.x >= tile_center.x && player.pos.x - tile_center.x <= game::constants::TILE_TURN_PAST_CENTER) ||
        (player.dir == game::dir::left && player.pos.x <= tile_center.x && tile_center.x - player.pos.x <= game::constants::TILE_TURN_PAST_CENTER) ||
        (player.dir == game::dir::down && player.pos.y >= tile_center.y && player.pos.y - tile_center.y <= game::constants::TILE_TURN_PAST_CENTER) ||
        (player.dir == game::dir::up && player.pos.y <= tile_center.y && tile_center.y - player.pos.y <= game::constants::TILE_TURN_PAST_CENTER);
    bool set_dir = false;

    // First check for turning 90 degrees, which is higher priority than turning around
    {
        player.turned = player.turned && can_turn;

        if (!set_dir && can_turn && was_facing_x && player.press.y && !player.turned)
        {
            int adjacent_row = (player.pos.y / tile_size.y) + player.press.y; // don't turn off the screen
            if (adjacent_row >= 0 && adjacent_row < tile_count.y)
            {
                player.dir = player.press.y > 0 ? game::dir::down : game::dir::up;
                player.turned = true;
                set_dir = true;
            }
        }

        if (!set_dir && can_turn && was_facing_y && player.press.x && !player.turned)
        {
            int adjacent_col = (player.pos.x / tile_size.x) + player.press.x; // don't turn off the screen
            if (adjacent_col >= 0 && adjacent_col < tile_count.x)
            {
                player.dir = player.press.x > 0 ? game::dir::right : game::dir::left;
                player.turned = true;
                set_dir = true;
            }
        }

        // Now check for turning around (180 degrees)

        if (!set_dir && was_facing_y && player.press.y)
        {
            player.dir = player.press.y > 0 ? game::dir::down : game::dir::up;
            set_dir = true;
        }

        if (!set_dir && was_facing_x && player.press.x)
        {
            player.dir = player.press.x > 0 ? game::dir::right : game::dir::left;
            set_dir = true;
        }

        if (set_dir)
        {
            player.press = {};
        }
    }

    // Move the player
    {
        player.pos += game::dir_to_point(player.dir);
    }

    // Keep aligned to the grid
    {
        if (game::dir_is_horizontal(player.dir))
        {
            player.pos.y = tile_center.y;
        }
        else if (game::dir_is_vertical(player.dir))
        {
            player.pos.x = tile_center.x;
        }
    }

    // Keep inside the grid
    {
        player.pos.x = ff::math::clamp(player.pos.x, tile_size.x / 2, (tile_count.x - 1) * tile_size.x + tile_size.x / 2);
        player.pos.y = ff::math::clamp(player.pos.y, tile_size.y / 2, (tile_count.y - 1) * tile_size.y + tile_size.y / 2);
    }

    // Check for tile collection
    {
        tile = player.pos / tile_size;
        tile_center = tile * tile_size + tile_size / 2;
        ff::point_int center_distance = (player.pos - tile_center).abs();

        if (center_distance.x <= game::constants::TILE_COLLECT_NEAR_CENTER &&
            center_distance.y <= game::constants::TILE_COLLECT_NEAR_CENTER)
        {
            if (!player.collected)
            {
                game::tile_type tile_type = play.level->tile(tile.cast<size_t>());
                switch (tile_type)
                {
                    case game::tile_type::panel0:
                        player.collected = true;
                        play.level->tile(tile.cast<size_t>(), game::tile_type::none);
                        break;

                    case game::tile_type::panel1:
                        player.collected = true;
                        play.level->tile(tile.cast<size_t>(), game::tile_type::panel0);
                        break;
                }

                if (player.collected)
                {
                    play.audio->play_collect(tile_type);
                    this->add_score(play, player, tile_type);
                }
            }
        }
        else
        {
            player.collected = false;
        }
    }
}

void game::game_update::add_score(game::play_level& play, game::player_data& player, game::tile_type tile_type)
{
    switch (tile_type)
    {
        case game::tile_type::panel0:
            player.score += 10;
            break;

        case game::tile_type::panel1:
            player.score += 20;
            break;
    }
}
