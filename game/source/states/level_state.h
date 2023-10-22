#pragma once

namespace game
{
    class level_state : public ff::state
    {
    public:
        level_state();

        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void advance_input() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;

    private:
    };
}
