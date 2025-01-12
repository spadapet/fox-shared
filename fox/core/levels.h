#pragma once

#include "core/game.h"

namespace game
{
    class levels
    {
    public:
        levels();

        void init_resources();

        game::level_data get_level(game::game_type type, game::game_diff diff, size_t index);

    private:
        std::vector<game::level_data> levels_;
    };
}
