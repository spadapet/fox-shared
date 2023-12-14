#include "pch.h"
#include "source/core/audio.h"
#include "source/core/game.h"
#include "source/core/updater.h"

game::updater::updater()
{}

void game::updater::update_player_input(
    game::player_data& player,
    bool press_left,
    bool press_right,
    bool press_up,
    bool press_down,
    bool press_speed)
{
    if (press_left && press_right)
    {
        if (player.flags.ignore_press_x)
        {
            press_left = false;
            press_right = false;
        }
        else
        {
            player.flags.ignore_press_x = true;

            if (player.dir == game::dir::right)
            {
                press_right = false;
            }
            else
            {
                press_left = false;
            }
        }
    }
    else
    {
        player.flags.ignore_press_x = false;
    }

    if (press_up && press_down)
    {
        if (player.flags.ignore_press_y)
        {
            press_up = false;
            press_down = false;
        }
        else
        {
            player.flags.ignore_press_y = true;

            if (player.dir == game::dir::down)
            {
                press_down = false;
            }
            else
            {
                press_up = false;
            }
        }
    }
    else
    {
        player.flags.ignore_press_y = false;
    }

    player.flags.press_speed = press_speed;

    // Keep what was pressed before, unless a new direction is pressed
    ff::point_int press(press_right ? 1 : (press_left ? -1 : 0), press_down ? 1 : (press_up ? -1 : 0));
    if (press)
    {
        player.press = press;
    }
}

void game::updater::update(game::play_level& play)
{
    for (size_t i = 0; i < play.game_data->current_player_count(); i++)
    {
        game::player_data& player = play.game_data->players[play.game_data->current_player + i];
        player.state.advance_time();
        player.speed_bank += play.game_data->player_speed(player.flags.press_speed);

        for (; player.speed_bank >= 1_f; player.speed_bank--)
        {
            this->update_player(play, player);
        }
    }

    this->check_win(play);
    this->check_dead(play);
}

void game::updater::update_player(game::play_level& play, game::player_data& player)
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
        player.flags.turned = player.flags.turned && can_turn;

        if (!set_dir && can_turn && was_facing_x && player.press.y && !player.flags.turned)
        {
            int adjacent_row = (player.pos.y / tile_size.y) + player.press.y; // don't turn off the screen
            if (adjacent_row >= 0 && adjacent_row < tile_count.y)
            {
                player.dir = player.press.y > 0 ? game::dir::down : game::dir::up;
                player.flags.turned = true;
                set_dir = true;
            }
        }

        if (!set_dir && can_turn && was_facing_y && player.press.x && !player.flags.turned)
        {
            int adjacent_col = (player.pos.x / tile_size.x) + player.press.x; // don't turn off the screen
            if (adjacent_col >= 0 && adjacent_col < tile_count.x)
            {
                player.dir = player.press.x > 0 ? game::dir::right : game::dir::left;
                player.flags.turned = true;
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
            if (!player.flags.collected)
            {
                game::tile_type tile_type = play.level().tile(tile.cast<size_t>());
                switch (tile_type)
                {
                    case game::tile_type::panel0:
                        player.flags.collected = true;
                        play.level().tile(tile.cast<size_t>(), game::tile_type::none);
                        break;

                    case game::tile_type::panel1:
                        player.flags.collected = true;
                        play.level().tile(tile.cast<size_t>(), game::tile_type::panel0);
                        break;
                }

                if (player.flags.collected)
                {
                    play.audio->play_collect(tile_type);
                    this->add_score(play, player, tile_type);
                }
            }
        }
        else
        {
            player.flags.collected = false;
        }
    }
}

void game::updater::add_score(game::play_level& play, game::player_data& player, game::tile_type tile_type)
{
    player.status->score += play.game_data->score_for_tile(tile_type);
}

void game::updater::check_win(game::play_level& play)
{
    for (game::tile_type tile_type : play.level().tiles)
    {
        switch (tile_type)
        {
            case game::tile_type::panel0:
            case game::tile_type::panel1:
                return;
        }
    }

    play.game_data->state = game::game_state::winning;
}

void game::updater::check_dead(game::play_level& play)
{
    bool found_alive = false;

    for (size_t i = 0; i < play.game_data->current_player_count(); i++)
    {
        game::player_data& player = play.game_data->players[play.game_data->current_player + i];

        if (player.state == game::player_state::dead && play.game_data->coop() && play.player_status().lives > 0)
        {
            play.player_status().lives--;
            player.init_playing(*play.game_data, i);
        }

        found_alive |= (player.state != game::player_state::dead);
    }

    if (!found_alive)
    {
        play.game_data->state = game::game_state::dying;
    }
}
