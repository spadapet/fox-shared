#include "pch.h"
#include "source/states/app_state.h"
#include "source/states/play_state.h"

game::play_state::play_state()
    : resource_connection(game::app_state::get().reload_resources_sink().connect(std::bind(&game::play_state::init_resources, this)))
{
    this->init_resources();
}

std::shared_ptr<ff::state> game::play_state::advance_time()
{
    return ff::state::advance_time();
}

void game::play_state::advance_input()
{
    ff::state::advance_input();
}

void game::play_state::render(ff::dxgi::command_context_base & context, ff::render_targets & targets)
{
    ff::state::advance_input();
}

void game::play_state::init_resources()
{
    ff::auto_resource<ff::input_mapping> player_mapping("player_controls");
    this->player_input[0] = std::make_unique<ff::input_event_provider>(*player_mapping.object(),
        std::vector<const ff::input_vk*>{ &ff::input::keyboard(), &ff::input::gamepad(0) });
}
