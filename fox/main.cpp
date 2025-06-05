#include "pch.h"
#include "fox.resm.id.h"
#include "states/play_state.h"

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    std::unique_ptr<game::play_state> play_state;

    ff::init_game_params params;
    params.game_thread_initialized_func = [&] { play_state = std::make_unique<game::play_state>(); };
    params.game_thread_finished_func = [&] { play_state.reset(); };
    params.game_input_func = [&] { play_state->input(); };
    params.game_update_func = [&] { play_state->update(); };
    params.game_render_offscreen_func = [&](const ff::render_params& params) { play_state->render_offscreen(params.context); };
    params.game_render_screen_func = [&](const ff::render_params& params) { play_state->render(params.context, params.target); };
    params.game_resources_rebuilt = [&] { play_state->init_resources(); };
    params.game_debug_command_func = [&](size_t id) { return play_state->debug_command(id); };
    params.debug_input_mapping = assets::controls::GAME_DEBUG_INPUT;

    return ff::run_game(params);
}
