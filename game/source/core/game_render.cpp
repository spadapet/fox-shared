#include "pch.h"
#include "source/core/game.h"
#include "source/core/game_render.h"

game::game_render::game_render()
{
    this->init_resources();
}

void game::game_render::init_resources()
{
    this->ship = "sprites.ship"sv;

    this->panel[0][0] = "sprites.panel0[0]"sv;
    this->panel[0][1] = "sprites.panel0[1]"sv;
    this->panel[1][0] = "sprites.panel1[0]"sv;
    this->panel[1][1] = "sprites.panel1[1]"sv;
}

void game::game_render::render(ff::dxgi::draw_base & draw, game::play_level & play)
{
    if (!play.game_data || !play.level)
    {
        return;
    }

    if (play.game_data->state != game::game_state::playing &&
        play.game_data->state != game::game_state::paused)
    {
        return;
    }

    ff::dxgi::transform transform;

    for (size_t i = 0; i < play.level->tiles.size(); i++)
    {
        const game::tile_type tile = play.level->tiles[i];
        ff::sprite_base* sprite{};

        switch (tile)
        {
            case game::tile_type::panel0:
                sprite = this->panel[0][play.level->counter / 32 % 2].object().get();
                break;

            case game::tile_type::panel1:
                sprite = this->panel[1][play.level->counter / 32 % 2].object().get();
                break;

            case game::tile_type::bomb:
                break;
        }

        if (sprite)
        {
            transform.position.x = static_cast<float>(i % game::constants::TILE_COUNT_X * game::constants::TILE_SIZE_X + game::constants::TILE_SIZE_X / 2);
            transform.position.y = static_cast<float>(i / game::constants::TILE_COUNT_X * game::constants::TILE_SIZE_Y + game::constants::TILE_SIZE_Y / 2);

            draw.draw_sprite(sprite->sprite_data(), transform);
        }
    }

    for (const game::player_data& player : play.game_data->players)
    {
        ff::sprite_base* sprite{};

        switch (player.state)
        {
            case game::player_state::playing:
                sprite = this->ship.object().get(); // [game::math::dir_to_index(player.dir)] .object().get();
                break;
        }

        if (sprite)
        {
            transform.position = player.pos.cast<float>();
            transform.rotation = game::math::dir_to_index(player.dir) * 90.0f;
            draw.draw_sprite(sprite->sprite_data(), transform);
        }
    }
}
