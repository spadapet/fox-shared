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
    this->shooter = "sprites.shooter"sv;
    this->player_shot = "sprites.player_shot"sv;
    this->explosion = "sprites.explosion"sv;
    this->game_font = "game_font"sv;
    this->game_font_small = "game_font_small"sv;
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
                            draw.draw_palette_filled_circle(center, 3, 254);
                        }
                        break;

                    case game::tile_type::warp0:
                    case game::tile_type::warp1:
                    case game::tile_type::warp2:
                    case game::tile_type::warp3:
                        {
                            // TODO: Use sprite animation
                            ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                            draw.draw_palette_outline_circle(center, 7, 242, 1);
                        }
                        break;

                    case game::tile_type::bomb:
                        {
                            // TODO: Use sprite animation
                            ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                            draw.draw_palette_outline_circle(center, 6, 245, 2);
                        }
                        break;
                }

                if (x >= game::constants::MOVABLE_TILES.left && x < game::constants::MOVABLE_TILES.right &&
                    y >= game::constants::MOVABLE_TILES.top && y < game::constants::MOVABLE_TILES.bottom)
                {
                    ff::point_fixed center = game::constants::tile_to_center<ff::fixed_int>(x, y);
                    draw.draw_palette_line(center - ff::point_fixed(2, 0), center + ff::point_fixed(1, 0), 255 - 14, 1);
                    draw.draw_palette_line(center - ff::point_fixed(0, 2), center + ff::point_fixed(0, 1), 255 - 14, 1);
                }

                if (anim)
                {
                    ff::dxgi::pixel_transform transform(game::constants::tile_to_center<ff::fixed_int>(x, y));
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
                if (shot.type == game::shot_type::shooter)
                {
                    draw.draw_palette_outline_circle(shot.pos.cast<ff::fixed_int>(), 6, 246, 3);
                }
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
            if (shot.dir != game::dir::none && shot.type == game::shot_type::player)
            {
                //draw.draw_palette_outline_circle(shot.pos.cast<ff::fixed_int>(), 6, 247, 3);
                ff::dxgi::pixel_transform transform(shot.pos.cast<ff::fixed_int>());
                this->player_shot->draw_frame(draw, transform, 0);
#if DEBUG
                //draw.draw_palette_outline_rectangle((game::constants::SHOT_HIT_BOX + shot.pos).cast<ff::fixed_int>(), 247, 1);
#endif
            }

            if (shot.type == game::shot_type::explosion)
            {
                ff::dxgi::pixel_transform transform(shot.pos.cast<ff::fixed_int>());
                this->explosion->draw_frame(draw, transform, 0);
            }
        }
    }

    if (render_shooters)
    {
        for (const game::shooter_data& shooter : play.game_data->shooters)
        {
            ff::animation_base* anim = this->shooter.object().get();
            ff::dxgi::pixel_transform transform(shooter.pos.cast<ff::fixed_int>(), ff::point_fixed(1, 1), game::dir_to_degrees<int>(shooter.shot_dir) + 90);
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
                ff::dxgi::palette_base* palette = ff::game::app_state_base::get().palette(player.index);
                draw.push_palette(palette);

                int rotation = game::dir_to_degrees<int>((player.dir != game::dir::none) ? player.dir : (i ? game::dir::down : game::dir::up));
                ff::dxgi::pixel_transform transform(player.pos.cast<ff::fixed_int>(), ff::point_fixed(1, 1), rotation);
                anim->draw_frame(draw, transform, 0.0f);

                if (!player.shoot_counter)
                {
                    draw.draw_palette_outline_circle(player.pos.cast<ff::fixed_int>(), 4_f, 243, 1_f);
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
    ff::dxgi::transform transform(pos.cast<float>(), ff::point_float(1, 1), 0, color);
    this->game_font->draw_text(&draw, text, transform, outline_color);
}

void game::renderer::draw_text_small(ff::dxgi::draw_base& draw, ff::point_int pos, std::string_view text, int color, int outline_color)
{
    ff::dxgi::transform transform(pos.cast<float>(), ff::point_float(1, 1), 0, color);
    this->game_font_small->draw_text(&draw, text, transform, outline_color);
}
