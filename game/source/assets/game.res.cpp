#include "pch.h"
#include "game.res.h"

namespace res
{
    void register_game()
    {
        ff::data_reader assets_reader(::assets::game::data());
        ff::global_resources::add(assets_reader);
    }
}
