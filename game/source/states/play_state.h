#pragma once

#include "source/core/game.h"
#include "source/core/game_render.h"
#include "source/core/game_update.h"

namespace game
{
    class title_page;
    class title_page_view_model;

    class play_state : public ff::state
    {
    public:
        play_state(game::game_type game_type, game::game_diff game_diff);
        virtual ~play_state() override;

        // ff::state
        virtual void advance_input() override;
        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;
        virtual size_t child_state_count() override;
        virtual ff::state* child_state(size_t index) override;

    private:
        void init_playing();
        void init_resources();

        game::game_data game_data;
        game::level_array levels;
        game::play_level play_level;
        game::game_update game_update;
        game::game_render game_render;

        std::unique_ptr<ff::input_event_provider> player_input[game::constants::MAX_PLAYERS];
        ff::signal_connection resource_connection;

        Noesis::Ptr<game::title_page_view_model> title_page_vm;
        Noesis::Ptr<game::title_page> title_page;
        std::shared_ptr<ff::state> title_state;
    };
}
