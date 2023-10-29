#pragma once

namespace game
{
    struct play_level;

    class game_render
    {
    public:
        game_render();

        void init_resources();
        void render(ff::dxgi::draw_base& draw, game::play_level& play);

    private:
        ff::auto_resource<ff::sprite_resource> player[4]; // right, up, left, down
        ff::auto_resource<ff::sprite_resource> panel[2][2]; // type, power
    };
}
