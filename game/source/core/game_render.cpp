#include "pch.h"
#include "source/core/game.h"
#include "source/core/game_render.h"

void game::render(ff::dxgi::draw_base& draw, game::play_level& play)
{
    if (!play.game_data || !play.level)
    {
        return;
    }
}
