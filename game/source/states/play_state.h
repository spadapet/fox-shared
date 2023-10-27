#pragma once

#include "source/core/game.h"

namespace game
{
    class play_state : public ff::state
    {
    public:
        play_state(game::game_type game_type, game::game_diff game_diff);

        virtual void advance_input() override;
        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;

    private:
        void init_game();
        void init_resources();

        game::game_data game_data;
        game::level_array levels;
        game::play_level play_level;

        ff::signal_connection resource_connection;
        std::unique_ptr<ff::input_event_provider> player_input[game::constants::MAX_PLAYERS];
        game::player_array players;
    };
}
