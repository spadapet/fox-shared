#pragma once

namespace game
{
    struct game_data;

    class title_state : public ff::state
    {
    public:
        title_state(game::game_data& game_data);

        // ff::state
        virtual void advance_input() override;
        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;

    private:
        game::game_data& game_data;
    };
}
