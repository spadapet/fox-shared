#include "pch.h"
#include "source/core/game.h"
#include "source/core/renderer.h"

static ff::point_float tile_to_center(size_t x, size_t y)
{
    return
    {
        static_cast<float>(x * game::constants::TILE_SIZE_X + game::constants::TILE_SIZE_X / 2),
        static_cast<float>(y * game::constants::TILE_SIZE_Y + game::constants::TILE_SIZE_Y / 2)
    };
}

static ff::point_float tile_index_to_center(size_t index)
{
    return ::tile_to_center(index % game::constants::TILE_COUNT_X, index / game::constants::TILE_COUNT_X);
}

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
    this->shooter = "sprites.shooter"sv;
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
    const game::tile_type* tile = level.tiles.data();

    for (size_t y = 0; y < game::constants::TILE_COUNT_Y; y++)
    {
        for (size_t x = 0; x < game::constants::TILE_COUNT_X; x++, tile++)
        {
            ff::animation_base* anim{};

            switch (*tile)
            {
                case game::tile_type::panel0:
                    anim = this->panel[0][play.total_counter() / 32 % 2].object().get();
                    break;

                case game::tile_type::panel1:
                    anim = this->panel[1][play.total_counter() / 32 % 2].object().get();
                    break;

                case game::tile_type::bomb:
                    {
                        // TODO: Use sprite animation
                        ff::point_float center = ::tile_to_center(x, y);
                        draw.draw_palette_outline_circle(center, 6.0f, 245, 2.0f, true);
                    }
                    break;
            }

            if (x >= game::constants::MOVABLE_TILES.left && x < game::constants::MOVABLE_TILES.right &&
                y >= game::constants::MOVABLE_TILES.top && y < game::constants::MOVABLE_TILES.bottom)
            {
                ff::point_float center = ::tile_to_center(x, y);
                draw.draw_palette_line(center - ff::point_float(3, 0), center + ff::point_float(2, 0), 255 - 14, 1.0f, true);
                draw.draw_palette_line(center - ff::point_float(0, 3), center + ff::point_float(0, 2), 255 - 14, 1.0f, true);
            }

            if (anim)
            {
                transform.position = ::tile_to_center(x, y);
                anim->draw_frame(draw, transform, 0.0f);
            }
        }
    }

    bool render_player = true;
    switch (play.game_data->state)
    {
        case game::game_state::play_ready:
            render_player = play.state_counter() >= game::constants::STATE_PLAY_READY_TIME / 2;
            break;

        case game::game_state::winning:
            render_player = play.state_counter() < game::constants::STATE_GAME_WINNING_TIME / 2;
            break;
    }

    if (render_player)
    {
        for (size_t i = 0; i < play.game_data->current_player_count(); i++)
        {
            const game::player_data& player = play.game_data->players[i + play.game_data->current_player];
            ff::animation_base* anim{};

            switch (player.state)
            {
                case game::player_state::playing:
                case game::player_state::dying:
                    anim = this->ship.object().get();
                    break;
            }

            if (anim)
            {
                transform.position = player.pos.cast<float>();
                transform.rotation = game::dir_to_degrees<float>((player.dir != game::dir::none) ? player.dir : (i ? game::dir::down : game::dir::up));

                ff::dxgi::palette_base* palette = ff::game::app_state_base::get().palette(player.index);
                draw.push_palette(palette);
                anim->draw_frame(draw, transform, 0.0f);
                draw.pop_palette();

                draw.draw_palette_line(transform.position - ff::point_float(3, 0), transform.position + ff::point_float(2, 0), 255 - 14, 1.0f, true);
                draw.draw_palette_line(transform.position - ff::point_float(0, 3), transform.position + ff::point_float(0, 2), 255 - 14, 1.0f, true);
            }
        }

        bool render_shooters = true;
        if (render_shooters)
        {
            for (const game::shooter_data& shooter : play.game_data->shooters)
            {
                ff::animation_base* anim = this->shooter.object().get();
                transform.position = shooter.pos.cast<float>();
                transform.rotation = shooter.index ? 180.0f : 0.0f;
                anim->draw_frame(draw, transform, 0.0f);
            }
        }
    }
}
