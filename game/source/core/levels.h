#pragma once

#include "source/core/game.h"

namespace game
{
    game::level_data get_level(game::game_type type, game::game_diff diff, size_t index);
}
