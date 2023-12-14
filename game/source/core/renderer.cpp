#include "pch.h"
#include "source/core/game.h"
#include "source/core/renderer.h"

game::renderer::renderer()
{
    this->init_resources();
}

void game::renderer::init_resources()
{
    this->ship = "sprites.ship"sv;

    this->panel[0][0] = "sprites.panel0[0]"sv;
    this->panel[0][1] = "sprites.panel0[1]"sv;
    this->panel[1][0] = "sprites.panel1[0]"sv;
    this->panel[1][1] = "sprites.panel1[1]"sv;
}

bool game::renderer::can_render(const game::play_level& play) const
{
    if (play.game_data)
    {
        switch (play.game_data->state)
        {
            case game::game_state::play_ready:
            case game::game_state::playing:
            case game::game_state::paused:
            case game::game_state::winning:
            case game::game_state::dying:
                return true;
        }
    }

    return false;
}

void game::renderer::render(ff::dxgi::draw_base& draw, const game::play_level& play)
{
    ff::dxgi::transform transform;
    const game::level_data& level = play.level();

    for (size_t i = 0; i < level.tiles.size(); i++)
    {
        const game::tile_type tile = level.tiles[i];
        ff::sprite_base* sprite{};

        switch (tile)
        {
            case game::tile_type::panel0:
                sprite = this->panel[0][play.total_counter() / 32 % 2].object().get();
                break;

            case game::tile_type::panel1:
                sprite = this->panel[1][play.total_counter() / 32 % 2].object().get();
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

    bool render_player = true;
    switch (play.game_data->state)
    {
        case game::game_state::play_ready:
            render_player = play.state_counter() >= game::constants::STATE_PLAY_READY_TIME / 2;
            break;

        case game::game_state::winning:
            render_player = play.state_counter() < game::constants::STATE_WINNING_TIME / 2;
            break;
    }

    if (render_player)
    {
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
                transform.rotation = game::dir_to_degrees<float>(player.dir);
                draw.draw_sprite(sprite->sprite_data(), transform);
            }
        }
    }
}
