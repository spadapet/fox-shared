#include "pch.h"
#include "source/core/game_render.h"
#include "source/core/game_update.h"
#include "source/states/app_state.h"
#include "source/states/play_state.h"

game::play_state::play_state(game::game_type game_type, game::game_diff game_diff)
    : game_data{ game_type, game_diff }
    , resource_connection(game::app_state::get().reload_resources_sink().connect(std::bind(&game::play_state::init_resources, this)))
    , play_level{ this->game_data, this->levels[0] }
{
    this->init_resources();
}

void game::play_state::advance_input()
{
    for (const auto& input : this->player_input)
    {
        input->advance();
    }

    for (auto& player: this->players)
    {
        ff::input_event_provider& input = *this->player_input[player.index];
        player.shoot = input.event_hit(game::input_events::ID_SHOOT);
        player.press =
        {
            input.digital_value(game::input_events::ID_RIGHT) ? 1_f : (input.digital_value(game::input_events::ID_LEFT) ? -1_f : 0_f),
            input.digital_value(game::input_events::ID_DOWN) ? 1_f : (input.digital_value(game::input_events::ID_UP) ? -1_f : 0_f)
        };
    }

    ff::state::advance_input();
}

std::shared_ptr<ff::state> game::play_state::advance_time()
{
    game::update(this->play_level);
    return ff::state::advance_time();
}

void game::play_state::render(ff::dxgi::command_context_base& context, ff::render_targets& targets)
{
    ff::dxgi::target_base& target = targets.target(context, ff::render_target_type::palette);
    ff::dxgi::depth_base& depth = targets.depth(context);
    ff::dxgi::draw_ptr draw = ff::dxgi_client().global_draw_device().begin_draw(context, target, &depth);

    if (draw)
    {
        game::render(*draw, this->play_level);
    }

    ff::state::render(context, targets);
}

void game::play_state::init_game()
{
}

void game::play_state::init_resources()
{
    ff::auto_resource<ff::input_mapping> player_mapping[]
    {
        "player_controls"sv,
        "player_controls_left"sv,
        "player_controls_right"sv
    };

    const bool coop = this->game_data.game_type == game::game_type::coop;

    for (size_t i = 0; i < game::constants::MAX_PLAYERS; i++)
    {
        std::vector<const ff::input_vk*> devices{ &ff::input::keyboard() };

        if (!coop)
        {
            for (size_t h = 0; h < ff::input::gamepad_count(); h++)
            {
                devices.push_back(&ff::input::gamepad(h));
            }
        }
        else if (i < ff::input::gamepad_count())
        {
            devices.push_back(&ff::input::gamepad(i));
        }

        const ff::input_mapping& mapping = !coop ? *player_mapping[0].object() : *player_mapping[i ? 2 : 1].object();
        this->player_input[i] = std::make_unique<ff::input_event_provider>(mapping, std::move(devices));
    }
}
