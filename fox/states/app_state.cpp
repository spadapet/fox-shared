#include "pch.h"
#include "states/app_state.h"
#include "states/play_state.h"
#include "fox.resm.id.h"

constexpr float PALETTE_CYCLES_PER_SECOND = 0.25f;
constexpr int PALETTE_BLACK = 240;

game::app_state::app_state()
    : ff::game::root_state_base(
        ff::render_target(game::constants::RENDER_SIZE.cast<size_t>(), &ff::color_black(), ::PALETTE_BLACK),
        {
            { std::string(assets::graphics::PALETTE), "player_0", ::PALETTE_CYCLES_PER_SECOND },
            { std::string(assets::graphics::PALETTE), "player_1", ::PALETTE_CYCLES_PER_SECOND },
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

void game::app_state::load_resources()
{
    if (auto play_state = std::dynamic_pointer_cast<game::play_state>(this->game_state()))
    {
        play_state->load_resources();
    }
}

std::shared_ptr<ff::state> game::app_state::create_initial_game_state()
{
    return std::make_shared<game::play_state>();
}
