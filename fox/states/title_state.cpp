#include "pch.h"
#include "core/game.h"
#include "states/title_state.h"

game::title_state::title_state(game::game_data& game_data)
    : game_data(game_data)
{
}

void game::title_state::advance_input()
{
    ff::state::advance_input();
}

std::shared_ptr<ff::state> game::title_state::advance_time()
{
    this->game_data.game_type = game::game_type::one_player;
    this->game_data.state = game::game_state::play_init_from_title;

    return ff::state::advance_time();
}

void game::title_state::render(ff::dxgi::command_context_base& context, ff::render_targets& targets)
{
    ff::state::render(context, targets);
}
