#include "pch.h"
#include "source/states/app_state.h"

static const float PALETTE_CYCLES_PER_SECOND = 0.25f;

fox::app_state::app_state()
    : render_target(fox::constants::RENDER_SIZE.cast<size_t>(), &ff::dxgi::color_black(), 224)
{}

fox::app_state::~app_state()
{}

ff::dxgi::palette_base* fox::app_state::palette()
{
    return this->player_palettes[0].get();
}

std::shared_ptr<ff::state> fox::app_state::advance_time()
{
    for (auto& i : this->player_palettes)
    {
        if (i)
        {
            i->advance();
        }
    }

    return ff::game::app_state_base::advance_time();
}

void fox::app_state::advance_input()
{
    if (this->allow_debug_commands())
    {
        if (!this->debug_input_events)
        {
            this->debug_input_events = std::make_unique<ff::input_event_provider>(*this->debug_input_mapping.object(),
                std::vector<const ff::input_vk*>{ &ff::input::keyboard(), & ff::input::pointer() });
        }

        if (this->debug_input_events->advance())
        {
            if (this->debug_input_events->event_hit(input_events::ID_DEBUG_RENDER_TOGGLE))
            {
                // TODO
            }

            if (this->debug_input_events->event_hit(input_events::ID_DEBUG_INVINCIBLE_TOGGLE))
            {
                // TODO
            }

            if (this->debug_input_events->event_hit(input_events::ID_DEBUG_COMPLETE_LEVEL))
            {
                // TODO
            }

            if (this->debug_input_events->event_hit(input_events::ID_SHOW_CUSTOM_DEBUG))
            {
                this->debug_command(ff::game::app_state_base::ID_DEBUG_SHOW_UI);
            }
        }
    }

    ff::game::app_state_base::advance_input();
}

void fox::app_state::render(ff::dxgi::command_context_base& context, ff::render_targets& targets)
{
    targets.push(this->render_target);
    ff::game::app_state_base::render(context, targets);
    this->target_rect = targets.pop(context, nullptr, this->palette());

    ff::dxgi::target_base& target = targets.target(context);
    ff::rect_float full_target_rect({}, target.size().logical_pixel_size.cast<float>());
    auto draw = ff::dxgi_client().global_draw_device().begin_draw(context, target, nullptr, full_target_rect, full_target_rect);
    if (draw && this->target_rect.left >= 0.5)
    {
        draw->draw_filled_rectangle(ff::rect_float(full_target_rect.top_left(), this->target_rect.bottom_left()), ff::dxgi::color_black());
        draw->draw_filled_rectangle(ff::rect_float(this->target_rect.top_right(), full_target_rect.bottom_right()), ff::dxgi::color_black());
    }
    else if (draw)
    {
        draw->draw_filled_rectangle(ff::rect_float(full_target_rect.top_left(), this->target_rect.top_right()), ff::dxgi::color_black());
        draw->draw_filled_rectangle(ff::rect_float(this->target_rect.bottom_left(), full_target_rect.bottom_right()), ff::dxgi::color_black());
    }
}

void fox::app_state::frame_rendered(ff::state::advance_t type, ff::dxgi::command_context_base& context, ff::render_targets& targets)
{
    for (ff::ui_view* view : ff::ui::rendered_views())
    {
        view->viewport(this->target_rect);
    }

    ff::game::app_state_base::frame_rendered(type, context, targets);
}

std::shared_ptr<ff::state> fox::app_state::create_initial_game_state()
{
    return {};
}

void fox::app_state::load_resources()
{
    this->debug_input_events.reset();
    this->debug_input_mapping = "game_debug_controls";
    this->palette_data = "palette";

    for (size_t i = 0; i < this->player_palettes.size(); i++)
    {
        std::ostringstream str;
        str << "player_" << i;
        this->player_palettes[i] = std::make_shared<ff::palette_cycle>(this->palette_data.object(), str.str().c_str(), ::PALETTE_CYCLES_PER_SECOND);
    }
}
