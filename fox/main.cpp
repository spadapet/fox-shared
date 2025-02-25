#include "pch.h"
#include "fox.resm.id.h"
#include "states/play_state.h"

#if 0

constexpr float PALETTE_CYCLES_PER_SECOND = 0.25f;
constexpr int PALETTE_BLACK = 240;
palette[0] = { std::string(assets::graphics::PALETTE), "player_0", ::PALETTE_CYCLES_PER_SECOND },
palette[1] = { std::string(assets::graphics::PALETTE), "player_1", ::PALETTE_CYCLES_PER_SECOND },

#endif

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    std::unique_ptr<game::play_state> play_state;

    ff::init_game_params params;
    params.game_thread_initialized_func = [&] { play_state = std::make_unique<game::play_state>(); };
    params.game_thread_finished_func = [&] { play_state.reset(); };
    params.game_input_func = [&] { play_state->input(); };
    params.game_update_func = [&] { play_state->update(); };
    params.game_render_offscreen_func = [&](ff::app_update_t, ff::dxgi::command_context_base& context) { play_state->render_offscreen(context); };
    params.game_render_func = [&](ff::app_update_t, ff::dxgi::command_context_base& context, ff::dxgi::target_base& target) { play_state->render(context, target); };
    params.game_resources_rebuilt = [&] { play_state->init_resources(); };
    params.game_debug_command_func = [&](size_t id) { return play_state->debug_command(id); };
    params.debug_input_mapping = assets::controls::GAME_DEBUG_INPUT;

    return ff::run_game(params);
}
