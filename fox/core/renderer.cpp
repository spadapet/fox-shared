#include "pch.h"
#include "core/game.h"
#include "core/renderer.h"
#include "fox.resm.id.h"

game::renderer::renderer()
{
    this->init_resources();
}

void game::renderer::init_resources()
{
    this->ship = assets::graphics::SPRITES_SHIP;
    this->panel[0][0] = assets::graphics::SPRITES_PANEL0_0;
    this->panel[0][1] = assets::graphics::SPRITES_PANEL0_1;
    this->panel[1][0] = assets::graphics::SPRITES_PANEL1_0;
    this->panel[1][1] = assets::graphics::SPRITES_PANEL1_1;
    this->shooter = assets::graphics::SPRITES_SHOOTER;
    this->game_font = assets::graphics::GAME_FONT;
    this->game_font_small = assets::graphics::GAME_FONT_SMALL;
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
    bool render_score = true;
    bool render_tiles = true;
    bool render_player = true;
    bool render_player_shots = true;
    bool render_shooters = true;
    bool render_shots = (play.game_data->state == game::game_state::playing);

    switch (play.game_data->state)
    {
        case game::game_state::play_ready:
            render_player = play.state_counter() >= game::constants::STATE_PLAY_READY_TIME / 2;
            break;

        case game::game_state::winning:
            render_player = play.state_counter() < game::constants::STATE_GAME_WINNING_TIME / 2;
            break;
    }

    if (render_score)
    {
        this->draw_text(draw, ff::point_int(0, 1), "SCORE: 1234", 254);
        //this->draw_text_small(draw, ff::point_int(0, 18), "SMALL TEXT", 254);
    }

    if (render_tiles)
    {
        const game::tile_type* tile = play.level().tiles.data();

        for (int y = 0; y < game::constants::TILE_COUNT_Y; y++)
        {
            for (int x = 0; x < game::constants::TILE_COUNT_X; x++, tile++)
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

                    case game::tile_type::points:
                        {
                            // TODO: Use sprite animation
                            ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                            ff::color color(254);
                            ff::dxgi::pixel_endpoint_t point{center, &color, 3};
                            draw.draw_circle(point);
                        }
                        break;

                    case game::tile_type::warp0:
                    case game::tile_type::warp1:
                    case game::tile_type::warp2:
                    case game::tile_type::warp3:
                        {
                            // TODO: Use sprite animation
                            ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                            ff::color color(242);
                            ff::dxgi::pixel_endpoint_t point{ center, &color, 7 };
                            draw.draw_circle(point, 1);
                        }
                        break;

                    case game::tile_type::bomb:
                        {
                            // TODO: Use sprite animation
                            ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                            ff::color color(245);
                            ff::dxgi::pixel_endpoint_t point{ center, &color, 6 };
                            draw.draw_circle(point, 2);
                        }
                        break;
                }

                if (x >= game::constants::MOVABLE_TILES.left && x < game::constants::MOVABLE_TILES.right &&
                    y >= game::constants::MOVABLE_TILES.top && y < game::constants::MOVABLE_TILES.bottom)
                {
                    ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                    draw.draw_line(center - ff::point_fixed(2, 0), center + ff::point_fixed(1, 0), 255 - 14, 1);
                    draw.draw_line(center - ff::point_fixed(0, 2), center + ff::point_fixed(0, 1), 255 - 14, 1);
                }

                if (anim)
                {
                    ff::pixel_transform transform(game::constants::tile_to_center<ff::fixed_int>(x, y));
                    anim->draw_frame(draw, transform, 0);
                }
            }
        }
    }

    if (render_shots)
    {
        for (const game::shot_data& shot : play.game_data->shots)
        {
            if (shot.dir != game::dir::none)
            {
                ff::color color(246);
                ff::dxgi::pixel_endpoint_t point{ shot.pos.cast<ff::fixed_int>(), &color, 6};
                draw.draw_circle(point, 3);
#if DEBUG
                //draw.draw_palette_outline_rectangle((game::constants::SHOT_HIT_BOX + shot.pos).cast<ff::fixed_int>(), 247, 1);
#endif
            }
        }
    }

    if (render_player_shots)
    {
        for (const game::shot_data& shot : play.game_data->player_shots)
        {
            if (shot.dir != game::dir::none)
            {
                ff::color color(247);
                ff::dxgi::pixel_endpoint_t point{ shot.pos.cast<ff::fixed_int>(), &color, 6 };
                draw.draw_circle(point, 3);
#if DEBUG
                //draw.draw_palette_outline_rectangle((game::constants::SHOT_HIT_BOX + shot.pos).cast<ff::fixed_int>(), 247, 1);
#endif
            }
        }
    }

    if (render_shooters)
    {
        for (const game::shooter_data& shooter : play.game_data->shooters)
        {
            ff::animation_base* anim = this->shooter.object().get();
            ff::pixel_transform transform(shooter.pos.cast<ff::fixed_int>(), ff::point_fixed(1, 1), game::dir_to_degrees<int>(shooter.shot_dir) + 90);
            anim->draw_frame(draw, transform, 0.0f);
        }
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
                ff::dxgi::palette_base* palette = ff::game::root_state_base::get().palette(player.index);
                draw.push_palette(palette);

                int rotation = game::dir_to_degrees<int>((player.dir != game::dir::none) ? player.dir : (i ? game::dir::down : game::dir::up));
                ff::pixel_transform transform(player.pos.cast<ff::fixed_int>(), ff::point_fixed(1, 1), rotation);
                anim->draw_frame(draw, transform, 0.0f);

                if (!player.shoot_counter)
                {
                    ff::color color(243);
                    ff::dxgi::pixel_endpoint_t point{ player.pos.cast<ff::fixed_int>(), &color, 4 };
                    draw.draw_circle(point, 1);
                }

                draw.pop_palette();
#if DEBUG
                //draw.draw_palette_outline_rectangle((game::constants::player_hit_box(player.index, player.dir) + player.pos).cast<ff::fixed_int>(), 247, 1);
#endif
            }
        }
    }
}

void game::renderer::draw_text(ff::dxgi::draw_base& draw, ff::point_int pos, std::string_view text, int color, int outline_color)
{
    ff::transform transform(pos.cast<float>(), ff::point_float(1, 1), 0, color);
    this->game_font->draw_text(&draw, text, transform, outline_color);
}

void game::renderer::draw_text_small(ff::dxgi::draw_base& draw, ff::point_int pos, std::string_view text, int color, int outline_color)
{
    ff::transform transform(pos.cast<float>(), ff::point_float(1, 1), 0, color);
    this->game_font_small->draw_text(&draw, text, transform, outline_color);
}
