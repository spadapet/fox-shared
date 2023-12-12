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

    private:
        ff::auto_resource<ff::sprite_resource> ship;
        ff::auto_resource<ff::sprite_resource> panel[2][2]; // type, power
    };
}
