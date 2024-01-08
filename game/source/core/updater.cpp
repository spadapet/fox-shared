#include "pch.h"
#include "source/core/audio.h"
#include "source/core/constants.h"
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
    bool press_speed,
    bool press_shoot)
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

            switch (player.dir)
            {
                case game::dir::right:
                    press_right = false;
                    break;

                case game::dir::left:
                    press_left = false;
                    break;
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

            switch (player.dir)
            {
                case game::dir::down:
                    press_down = false;
                    break;

                case game::dir::up:
                    press_up = false;
                    break;
            }
        }
    }
    else
    {
        player.flags.ignore_press_y = false;
    }

    player.flags.press_speed = press_speed;
    player.flags.press_shoot = press_shoot;

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

        switch (player.state)
        {
            case game::player_state::playing:
                for (player.speed_bank += play.game_data->player_speed(player.flags.press_speed);
                    player.speed_bank >= 1_f && player.state == game::player_state::playing; player.speed_bank--)
                {
                    this->update_player(play, player);
                }
                break;

            case game::player_state::dying:
                player.state.set_at(game::player_state::dead, play.game_data->coop() ? game::constants::STATE_PLAYER_DYING_TIME : 0);
                break;
        }
    }

    for (game::shooter_data& shooter : play.game_data->shooters)
    {
        for (shooter.speed_bank += play.game_data->shooter_speed(); shooter.speed_bank >= 1_f; shooter.speed_bank--)
        {
            this->update_shooter(play, shooter);
        }
    }

    for (game::shot_data& shot : play.game_data->shots)
    {
        for (shot.speed_bank += play.game_data->shot_speed(); shot.speed_bank >= 1_f; shot.speed_bank--)
        {
            this->update_shot(play, shot);
        }
    }

    for (game::shot_data& shot : play.game_data->player_shots)
    {
        for (shot.speed_bank += play.game_data->player_shot_speed(); shot.speed_bank >= 1_f; shot.speed_bank--)
        {
            this->update_shot(play, shot);
        }
    }

    this->check_hit(play);

    if (!this->check_dead(play))
    {
        this->check_win(play);
    }
}

void game::updater::update_player(game::play_level& play, game::player_data& player)
{
    ff::point_int tile = player.pos / game::constants::TILE_SIZE;
    ff::point_int tile_center = game::constants::tile_to_center(tile);

    const bool was_facing_x = game::dir_is_horizontal(player.dir);
    const bool was_facing_y = game::dir_is_vertical(player.dir);
    const bool can_turn = (player.dir == game::dir::none) ||
        (player.dir == game::dir::right && player.pos.x >= tile_center.x && player.pos.x - tile_center.x <= game::constants::TILE_TURN_PAST_CENTER) ||
        (player.dir == game::dir::left && player.pos.x <= tile_center.x && tile_center.x - player.pos.x <= game::constants::TILE_TURN_PAST_CENTER) ||
        (player.dir == game::dir::down && player.pos.y >= tile_center.y && player.pos.y - tile_center.y <= game::constants::TILE_TURN_PAST_CENTER) ||
        (player.dir == game::dir::up && player.pos.y <= tile_center.y && tile_center.y - player.pos.y <= game::constants::TILE_TURN_PAST_CENTER);
    bool set_dir = false;

    // First check for turning 90 degrees, which is higher priority than turning around
    {
        player.flags.turned = player.flags.turned && can_turn;

        if (!set_dir && can_turn && !was_facing_y && player.press.y && !player.flags.turned)
        {
            int adjacent_row = (player.pos.y / game::constants::TILE_SIZE.y) + player.press.y; // don't turn off the screen
            if (adjacent_row >= game::constants::MOVABLE_TILES.top && adjacent_row < game::constants::MOVABLE_TILES.bottom)
            {
                player.dir = player.press.y > 0 ? game::dir::down : game::dir::up;
                player.flags.turned = true;
                set_dir = true;
            }
        }

        if (!set_dir && can_turn && !was_facing_x && player.press.x && !player.flags.turned)
        {
            int adjacent_col = (player.pos.x / game::constants::TILE_SIZE.x) + player.press.x; // don't turn off the screen
            if (adjacent_col >= game::constants::MOVABLE_TILES.left && adjacent_col < game::constants::MOVABLE_TILES.right)
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
            if (player.pos.y > tile_center.y)
            {
                player.pos.y--;
            }
            else if (player.pos.y < tile_center.y)
            {
                player.pos.y++;
            }
        }
        else if (game::dir_is_vertical(player.dir))
        {
            if (player.pos.x > tile_center.x)
            {
                player.pos.x--;
            }
            else if (player.pos.x < tile_center.x)
            {
                player.pos.x++;
            }
        }
    }

    // Keep inside the grid
    {
        player.pos.x = ff::math::clamp(player.pos.x,
            game::constants::MOVABLE_AREA_CENTER_TILE.left,
            game::constants::MOVABLE_AREA_CENTER_TILE.right);

        player.pos.y = ff::math::clamp(player.pos.y,
            game::constants::MOVABLE_AREA_CENTER_TILE.top,
            game::constants::MOVABLE_AREA_CENTER_TILE.bottom);
    }

    // Check for tile collection
    {
        tile = player.pos / game::constants::TILE_SIZE;
        tile_center = game::constants::tile_to_center(tile);
        ff::point_int center_distance = (player.pos - tile_center).abs();

        if (center_distance.x <= game::constants::TILE_COLLECT_NEAR_CENTER &&
            center_distance.y <= game::constants::TILE_COLLECT_NEAR_CENTER)
        {
            if (!player.flags.collected)
            {
                this->player_hit_tile(play, player, tile);
            }
        }
        else
        {
            player.flags.collected = false;
        }
    }

    if (player.shoot_counter)
    {
        player.shoot_counter--;
    }

    if (player.flags.press_shoot)
    {
        player.flags.press_shoot = 0;

        if (!player.shoot_counter)
        {
            player.shoot_counter = play.game_data->player_shot_time();
            this->add_player_shot(play, player.pos, player.dir);
        }
    }
}

void game::updater::update_shooter(game::play_level& play, game::shooter_data& shooter)
{
    switch (shooter.move_dir)
    {
        case game::dir::right:
            if (++shooter.pos.x >= game::constants::MOVABLE_AREA.right)
            {
                shooter.move_dir = game::dir::left;
            }
            break;

        case game::dir::left:
            if (--shooter.pos.x <= game::constants::MOVABLE_AREA.left)
            {
                shooter.move_dir = game::dir::right;
            }
            break;
    }

    ff::point_int tile = shooter.pos / game::constants::TILE_SIZE;

    if (shooter.pos.x - tile.x * game::constants::TILE_SIZE.x == game::constants::TILE_SIZE.x / 2)
    {
        if (shooter.shot_counter)
        {
            shooter.shot_counter--;
        }

        if (!shooter.shot_counter)
        {
            if (shooter.shot_amount)
            {
                shooter.shot_amount--;
                this->add_shot(play, shooter.pos, shooter.shot_dir);
            }
            else
            {
                shooter.shot_counter = ff::math::random_range(1, 32); // static_cast<size_t>(48));
                shooter.shot_amount = ff::math::random_range(static_cast<size_t>(0), static_cast<size_t>(100));

                if (shooter.shot_amount > 95)
                {
                    shooter.shot_amount = 5;
                }
                else if (shooter.shot_amount > 85)
                {
                    shooter.shot_amount = 5;
                }
                else if (shooter.shot_amount > 70)
                {
                    shooter.shot_amount = 3;
                }
                else if (shooter.shot_amount > 55)
                {
                    shooter.shot_amount = 2;
                }
                else if (shooter.shot_amount > 5)
                {
                    shooter.shot_amount = 1;
                }
                else
                {
                    shooter.shot_amount = 0;
                }
            }
        }
    }
}

void game::updater::update_shot(game::play_level& play, game::shot_data& shot)
{
    if (shot.lifetime && !--shot.lifetime)
    {
        shot = {};
        return;
    }

    switch (shot.dir)
    {
        case game::dir::down:
            if (++shot.pos.y > game::constants::RENDER_SIZE_Y + game::constants::TILE_SIZE_Y / 2)
            {
                shot = {};
            }
            break;

        case game::dir::up:
            if (--shot.pos.y < game::constants::TILE_SIZE_Y / -2)
            {
                shot = {};
            }
            break;

        case game::dir::right:
            if (++shot.pos.x > game::constants::RENDER_SIZE_X + game::constants::TILE_SIZE_X / 2)
            {
                shot = {};
            }
            break;

        case game::dir::left:
            if (--shot.pos.x < game::constants::TILE_SIZE_X / -2)
            {
                shot = {};
            }
            break;
    }
}

void game::updater::player_hit_tile(game::play_level& play, game::player_data& player, ff::point_int tile)
{
    const game::tile_type tile_type = play.level().tile(tile);
    game::tile_type warp_to_type = game::tile_type::none;

    switch (tile_type)
    {
        case game::tile_type::panel0:
            play.level().tile(tile, game::tile_type::none);
            break;

        case game::tile_type::panel1:
            play.level().tile(tile, game::tile_type::panel0);
            break;

        case game::tile_type::points:
            play.level().tile(tile, game::tile_type::none);
            break;

        case game::tile_type::warp0: warp_to_type = game::tile_type::warp1; break;
        case game::tile_type::warp1: warp_to_type = game::tile_type::warp2; break;
        case game::tile_type::warp2: warp_to_type = game::tile_type::warp3; break;
        case game::tile_type::warp3: warp_to_type = game::tile_type::warp0; break;

        case game::tile_type::bomb:
            player.state = game::player_state::dying;
            break;

        default:
            return;
    }

    if (warp_to_type != game::tile_type::none)
    {
        ff::point_int dest_pos{};

        for (ff::point_int tile_pos(0, 0); tile_pos.y < game::constants::TILE_COUNT_Y; tile_pos.y++)
        {
            for (tile_pos.x = 0; tile_pos.x < game::constants::TILE_COUNT_X; tile_pos.x++)
            {
                game::tile_type type = play.level().tile(tile_pos);

                if (type == warp_to_type || (type == game::tile_type::warp0 && !dest_pos))
                {
                    dest_pos = game::constants::tile_to_center(tile_pos);
                }
            }
        }

        if (dest_pos)
        {
            player.pos = dest_pos;
        }
    }

    player.flags.collected = true;
    play.audio->play_collect(tile_type);
    this->add_score(play, player, tile_type);
}

void game::updater::add_shot(game::play_level& play, ff::point_int pos, game::dir dir)
{
    for (game::shot_data& shot : play.game_data->shots)
    {
        if (shot.dir == game::dir::none)
        {
            shot = {};
            shot.dir = dir;
            shot.pos = pos;
            play.audio->play_shot();
            break;
        }
    }
}

void game::updater::add_player_shot(game::play_level& play, ff::point_int pos, game::dir dir)
{
    for (game::shot_data& shot : play.game_data->player_shots)
    {
        if (shot.dir == game::dir::none)
        {
            shot = {};
            shot.dir = dir;
            shot.pos = pos;
            shot.lifetime = play.game_data->player_shot_lifetime();
            play.audio->play_player_shot();
            break;
        }
    }
}

void game::updater::add_score(game::play_level& play, game::player_data& player, game::tile_type tile_type)
{
    player.status->score += play.game_data->score_for_tile(tile_type);
}

void game::updater::check_hit(game::play_level& play)
{
    for (size_t i = 0; i < play.game_data->current_player_count(); i++)
    {
        game::player_data& player = play.game_data->players[play.game_data->current_player + i];

        if (player.state == game::player_state::playing)
        {
            const ff::rect_int player_rect = game::constants::player_hit_box(player.index, player.dir) + player.pos;

            for (game::shot_data& shot : play.game_data->shots)
            {
                if (shot.dir != game::dir::none)
                {
                    const ff::rect_int shot_rect = game::constants::SHOT_HIT_BOX + shot.pos;

                    if (player_rect.intersects(shot_rect))
                    {
                        player.state = game::player_state::dying;
                        play.audio->play_player_hit_shot();
                        // TODO: Visual effect
                        break;
                    }
                }
            }
        }
    }

    for (game::shot_data& player_shot : play.game_data->player_shots)
    {
        if (player_shot.dir != game::dir::none)
        {
            const ff::rect_int player_shot_rect = game::constants::PLAYER_SHOT_HIT_BOX + player_shot.pos;

            for (game::shot_data& shot : play.game_data->shots)
            {
                if (shot.dir != game::dir::none)
                {
                    const ff::rect_int shot_rect = game::constants::SHOT_HIT_BOX + shot.pos;

                    if (player_shot_rect.intersects(shot_rect))
                    {
                        shot = {};
                        player_shot = {};
                        play.audio->play_shot_hit_shot();
                        // TODO: Visual effect
                        break;
                    }
                }
            }
        }
    }
}

bool game::updater::check_win(game::play_level& play)
{
    for (game::tile_type tile_type : play.level().tiles)
    {
        switch (tile_type)
        {
            case game::tile_type::panel0:
            case game::tile_type::panel1:
                return false;
        }
    }

    play.game_data->state = game::game_state::winning;
    return true;
}

bool game::updater::check_dead(game::play_level& play)
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
        return true;
    }

    return false;
}
