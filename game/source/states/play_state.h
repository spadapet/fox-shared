#pragma once

namespace game
{
    class play_state : public ff::state
    {
    public:
        play_state();

        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void advance_input() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;

    private:
        void init_resources();

        ff::signal_connection resource_connection;
        std::array<std::unique_ptr<ff::input_event_provider>, game::constants::MAX_PLAYERS> player_input;
    };
}
