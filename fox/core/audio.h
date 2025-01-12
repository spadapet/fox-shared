#pragma once

#include "core/game.h"

namespace game
{
    class audio
    {
    public:
        audio();

        void init_resources();

        void play_collect(game::tile_type type);
        void play_shot();
        void play_shot_hit_shot();
        void play_player_shot();
        void play_player_hit_shot();

    private:
    };
}
