#pragma once

#include "source/core/game.h"

namespace game
{
    game::level get_level(size_t index);
    game::level get_challenge(size_t index);
    game::level get_bonus(size_t index);
}
