#include "pch.h"
#include "source/states/level_state.h"

game::level_state::level_state()
{}

std::shared_ptr<ff::state> game::level_state::advance_time()
{
    return ff::state::advance_time();
}

void game::level_state::advance_input()
{
    ff::state::advance_input();
}

void game::level_state::render(ff::dxgi::command_context_base & context, ff::render_targets & targets)
{
    ff::state::advance_input();
}
