#include "pch.h"
#include "source/core/game_render.h"
#include "source/core/game_update.h"
#include "source/core/levels.h"
#include "source/states/app_state.h"
#include "source/states/play_state.h"
#include "source/ui/title_page.xaml.h"

game::play_state::play_state(game::game_type game_type, game::game_diff game_diff)
    : game_data{ game_type, game_diff, game::game_state::title }
    , resource_connection(game::app_state::get().reload_resources_sink().connect(std::bind(&game::play_state::init_resources, this)))
    , title_page_vm(Noesis::MakePtr<game::title_page_view_model>(this->game_data))
    , title_page(Noesis::MakePtr<game::title_page>(this->title_page_vm))
    , title_state(std::make_shared<ff::ui_view_state>(std::make_shared<ff::ui_view>(this->title_page)))
{
    this->init_resources();
}

game::play_state::~play_state()
{}

void game::play_state::advance_input()
{
    if (this->game_data.state == game::game_state::playing)
    {
        for (const auto& input : this->player_input)
        {
            input->advance();
        }

        for (auto& player : this->game_data.players)
        {
            ff::input_event_provider& input = *this->player_input[player.index];
            player.shoot = input.event_hit(game::input_events::ID_SHOOT);
            player.press =
            {
                input.digital_value(game::input_events::ID_RIGHT) ? 1_f : (input.digital_value(game::input_events::ID_LEFT) ? -1_f : 0_f),
                input.digital_value(game::input_events::ID_DOWN) ? 1_f : (input.digital_value(game::input_events::ID_UP) ? -1_f : 0_f)
            };
        }
    }

    ff::state::advance_input();
}

std::shared_ptr<ff::state> game::play_state::advance_time()
{
    switch (this->game_data.state)
    {
        case game::game_state::play:
            this->init_playing();
            break;

        case game::game_state::playing:
            game::update(this->play_level);
            break;
    }

    return ff::state::advance_time();
}

void game::play_state::render(ff::dxgi::command_context_base& context, ff::render_targets& targets)
{
    if (this->game_data.state == game::game_state::playing ||
        this->game_data.state == game::game_state::paused)
    {
        ff::dxgi::target_base& target = targets.target(context, ff::render_target_type::palette);
        ff::dxgi::depth_base& depth = targets.depth(context);
        ff::dxgi::draw_ptr draw = ff::dxgi_client().global_draw_device().begin_draw(context, target, &depth);

        if (draw)
        {
            game::render(*draw, this->play_level);
        }
    }

    ff::state::render(context, targets);
}

size_t game::play_state::child_state_count()
{
    switch (this->game_data.state)
    {
        case game::game_state::title:
            return 1;

        default:
            return 0;
    }
}

ff::state* game::play_state::child_state(size_t index)
{
    switch (this->game_data.state)
    {
        case game::game_state::title:
            return this->title_state.get();

        default:
            return nullptr;
    }
}

void game::play_state::init_playing()
{
    this->game_data.state = game::game_state::playing;

    game::player_data& player = this->game_data.players[this->game_data.current_player];
    //player.state == game::player_state::

    for (size_t i = 0; i < game::constants::MAX_PLAYERS; i++)
    {
        this->levels[i] = game::get_level(
            this->game_data.game_type,
            this->game_data.game_diff,
            this->game_data.players[i].level);
    }

    this->play_level = { &this->game_data, &this->levels[this->game_data.current_player] };
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
