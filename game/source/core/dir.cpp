#include "pch.h"
#include "source/core/dir.h"

ff::point_int game::dir_to_point(game::dir dir)
{
    switch (dir)
    {
        case game::dir::right: return ff::point_int(1, 0);
        case game::dir::up: return ff::point_int(0, -1);
        case game::dir::left: return ff::point_int(-1, 0);
        case game::dir::down: return ff::point_int(0, 1);
        default: return ff::point_int();
    }
}
