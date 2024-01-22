#pragma once

namespace game
{
    struct play_level;

    class renderer
    {
    public:
        renderer();

        void init_resources();
        bool can_render(const game::play_level& play) const;
        void render(ff::dxgi::draw_base& draw, const game::play_level& play);

        void draw_text(ff::dxgi::draw_base& draw, ff::point_int pos, std::string_view text, int color, int outline_color = 0);
        void draw_text_small(ff::dxgi::draw_base& draw, ff::point_int pos, std::string_view text, int color, int outline_color = 0);

    private:
        ff::auto_resource<ff::animation_base> ship;
        ff::auto_resource<ff::animation_base> panel[2][2]; // type, power
        ff::auto_resource<ff::animation_base> shooter;
        ff::auto_resource<ff::animation_base> player_shot;
        ff::auto_resource<ff::animation_base> explosion;
        ff::auto_resource<ff::sprite_font> game_font;
        ff::auto_resource<ff::sprite_font> game_font_small;
    };
}
