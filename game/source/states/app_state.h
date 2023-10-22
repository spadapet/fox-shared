#pragma once

namespace game
{
    class app_state : public ff::game::app_state_base
    {
    public:
        app_state();
        virtual ~app_state() override;

        static game::app_state& get();

        // ff::game_state_base
        virtual bool debug_command_override(size_t command_id) override;

    private:
        virtual std::shared_ptr<ff::state> create_initial_game_state() override;
    };
}
