#include "pch.h"
#include "game.res.h"

namespace res
{
    void register_game()
    {
        ff::global_resources::add(::assets::game::data());
    }
}
