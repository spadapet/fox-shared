#pragma once

namespace game
{
    class app_state : public ff::game::root_state_base
    {
    public:
        app_state();

        // ff::game_state_base
        virtual bool debug_command_override(size_t command_id) override;

    protected:
        virtual void load_resources() override;

    private:
        virtual std::shared_ptr<ff::state> create_initial_game_state() override;
    };
}
