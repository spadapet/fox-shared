#pragma once

#include "source/core/audio.h"
#include "source/core/game.h"
#include "source/core/levels.h"
#include "source/core/renderer.h"
#include "source/core/updater.h"

namespace game
{
    class title_page;
    class title_page_view_model;

    class play_state : public ff::state
    {
    public:
        play_state(game::game_type game_type = game::game_type::none, game::game_diff game_diff = game::game_diff::none);
        virtual ~play_state() override;

        // ff::state
        virtual void advance_input() override;
        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;
        virtual size_t child_state_count() override;
        virtual ff::state* child_state(size_t index) override;

    private:
        void find_next_player();
        void init_from_title();
        void init_playing();
        void init_next_level();
        void init_shooters();
        void on_reload_resources();
        void init_playing_resources();
        void init_resources();

        game::audio audio;
        game::levels levels;
        game::game_data game_data;
        game::play_level play_level;
        game::updater updater;
        game::renderer renderer;

        std::unique_ptr<ff::input_event_provider> player_input[game::constants::MAX_PLAYERS];
        ff::signal_connection resource_connection;

        Noesis::Ptr<game::title_page_view_model> title_page_vm;
        Noesis::Ptr<game::title_page> title_page;
        std::shared_ptr<ff::state> title_state;
    };
}
