#pragma once

#include "source/core/game.h"

namespace game
{
    class audio
    {
    public:
        audio();

        void init_resources();

        void play_collect(game::tile_type type);
        void play_shot();

    private:
    };
}
