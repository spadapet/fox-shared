#pragma once

#include "core/audio.h"
#include "core/game.h"
#include "core/levels.h"
#include "core/renderer.h"
#include "core/updater.h"

namespace game
{
    class play_state
    {
    public:
        play_state(game::game_type game_type = game::game_type::none, game::game_diff game_diff = game::game_diff::none);
        ~play_state();

        void init_resources();
        bool debug_command(size_t id);
        void input();
        void update();
        void render_offscreen(ff::dxgi::command_context_base& context);
        void render(ff::dxgi::command_context_base& context, ff::dxgi::target_base& target);

    private:
        void find_next_player();
        void init_from_title();
        void init_playing();
        void init_next_level();
        void init_shooters();
        void init_playing_resources();

        game::audio audio;
        game::levels levels;
        game::game_data game_data;
        game::play_level play_level;
        game::updater updater;
        game::renderer renderer;

        std::unique_ptr<ff::input_event_provider> player_input[game::constants::MAX_PLAYERS];
        std::unique_ptr<ff::palette_cycle> palette[game::constants::MAX_PLAYERS];

        // rendering
        std::shared_ptr<ff::dxgi::depth_base> depth;
    };
}
