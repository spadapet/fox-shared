#pragma once

namespace fox
{
    class app_state : public ff::game::app_state_base
    {
    public:
        app_state();
        virtual ~app_state() override;

        // ff::game::app_state_base
        virtual ff::dxgi::palette_base* palette() override;

        // ff::state
        virtual std::shared_ptr<ff::state> advance_time() override;
        virtual void advance_input() override;
        virtual void render(ff::dxgi::command_context_base& context, ff::render_targets& targets) override;
        virtual void frame_rendered(ff::state::advance_t type, ff::dxgi::command_context_base& context, ff::render_targets& targets) override;

    private:
        virtual std::shared_ptr<ff::state> create_initial_game_state() override;
        virtual void load_resources() override;

        // Rendering
        std::array<std::shared_ptr<ff::palette_cycle>, constants::MAX_PLAYERS> player_palettes;
        ff::auto_resource<ff::palette_data> palette_data;
        ff::render_target render_target;
        ff::rect_float target_rect{};

        // Debugging
        std::unique_ptr<ff::input_event_provider> debug_input_events;
        ff::auto_resource<ff::input_mapping> debug_input_mapping;
    };
}
