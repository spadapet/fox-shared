#include "pch.h"
#include "source/core/constants.h"
#include "source/core/game.h"

const std::string_view game::strings::ID_GAME_OPTIONS = "game_options";

const size_t game::input_events::ID_UP = ff::stable_hash_func("up"sv);
const size_t game::input_events::ID_DOWN = ff::stable_hash_func("down"sv);
const size_t game::input_events::ID_LEFT = ff::stable_hash_func("left"sv);
const size_t game::input_events::ID_RIGHT = ff::stable_hash_func("right"sv);
const size_t game::input_events::ID_SPEED = ff::stable_hash_func("speed"sv);
const size_t game::input_events::ID_SHOOT = ff::stable_hash_func("shoot"sv);
const size_t game::input_events::ID_ACTION = ff::stable_hash_func("action"sv);
const size_t game::input_events::ID_CANCEL = ff::stable_hash_func("cancel"sv);
const size_t game::input_events::ID_PAUSE = ff::stable_hash_func("pause"sv);

const size_t game::input_events::ID_DEBUG_RENDER_TOGGLE = ff::stable_hash_func("debug_render_toggle"sv);
const size_t game::input_events::ID_DEBUG_INVINCIBLE_TOGGLE = ff::stable_hash_func("invincible_toggle"sv);
const size_t game::input_events::ID_DEBUG_COMPLETE_LEVEL = ff::stable_hash_func("complete_level"sv);

const ff::rect_int& game::constants::player_hit_box(size_t index, game::dir dir)
{
    switch (dir)
    {
    case game::dir::right:
        return game::constants::PLAYER_HIT_BOX_RIGHT;

    case game::dir::up:
        return game::constants::PLAYER_HIT_BOX_UP;

    case game::dir::left:
        return game::constants::PLAYER_HIT_BOX_LEFT;

    case game::dir::down:
        return game::constants::PLAYER_HIT_BOX_DOWN;

    default:
        return index
            ? game::constants::PLAYER_HIT_BOX_DOWN
            : game::constants::PLAYER_HIT_BOX_UP;
    }
}
