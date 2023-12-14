#include "pch.h"
#include "source/states/app_state.h"
#include "source/states/play_state.h"

constexpr float PALETTE_CYCLES_PER_SECOND = 0.25f;
constexpr int PALETTE_BLACK = 240;

game::app_state::app_state()
    : ff::game::app_state_base(
        ff::render_target(game::constants::RENDER_SIZE, &ff::dxgi::color_black(), ::PALETTE_BLACK),
        {
            { "palette", "player_0", ::PALETTE_CYCLES_PER_SECOND },
            { "palette", "player_1", ::PALETTE_CYCLES_PER_SECOND },
        })
{}

bool game::app_state::debug_command_override(size_t command_id)
{
    if (command_id == input_events::ID_DEBUG_RENDER_TOGGLE)
    {
        // TODO
    }
    else if (command_id == input_events::ID_DEBUG_INVINCIBLE_TOGGLE)
    {
        // TODO
    }
    else if (command_id == input_events::ID_DEBUG_COMPLETE_LEVEL)
    {
        // TODO
    }

    return false;
}

std::shared_ptr<ff::state> game::app_state::create_initial_game_state()
{
    return std::make_shared<game::play_state>();
}
