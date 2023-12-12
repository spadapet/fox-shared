#include "pch.h"
#include "source/core/levels.h"
#include "source/states/app_state.h"
#include "source/states/play_state.h"
#include "source/ui/title_page.xaml.h"

game::play_state::play_state(game::game_type game_type, game::game_diff game_diff)
    : game_data{ game_type, game_diff, game::game_state::title }
    , resource_connection(ff::game::app_state_base::get().reload_resources_sink().connect(std::bind(&game::play_state::init_resources, this)))
    , title_page_vm(Noesis::MakePtr<game::title_page_view_model>(this->game_data))
    , title_page(Noesis::MakePtr<game::title_page>(this->title_page_vm))
    , title_state(std::make_shared<ff::ui_view_state>(std::make_shared<ff::ui_view>(this->title_page)))
{
    for (size_t i = 0; i < this->game_data.total_player_count(); i++)
    {
        game::player_data& player = this->game_data.players[i];
        player.index = i;
        player.status = &this->game_data.statuses[i * !this->game_data.coop()];
        player.level = &this->game_data.levels[i * !this->game_data.coop()];
        player.state = (i < this->game_data.total_player_count()) ? game::player_state::playing : game::player_state::none;
    }

    this->init_resources();
}

game::play_state::~play_state()
{
    // Just needed because of include files for smart pointer destruction
}

void game::play_state::advance_input()
{
    if (this->game_data.state == game::game_state::playing)
    {
        for (const auto& input : this->player_input)
        {
            input->advance();
        }

        for (game::player_data& player : this->game_data.players)
        {
            ff::input_event_provider& input = *this->player_input[player.index];
            this->updater.update_player_input(player,
                input.digital_value(game::input_events::ID_LEFT),
                input.digital_value(game::input_events::ID_RIGHT),
                input.digital_value(game::input_events::ID_UP),
                input.digital_value(game::input_events::ID_DOWN),
                input.digital_value(game::input_events::ID_SPEED));
        }
    }

    ff::state::advance_input();
}

std::shared_ptr<ff::state> game::play_state::advance_time()
{
    this->game_data.state.advance_time();

    switch (this->game_data.state)
    {
        case game::game_state::play_init:
            this->init_playing();
            break;

        case game::game_state::play_ready:
            if (this->game_data.state.counter >= game::constants::STATE_PLAY_READY_TIME)
            {
                this->game_data.state = game::game_state::playing;
            }
            break;

        case game::game_state::playing:
            this->updater.update(this->play_level);
            break;

        case game::game_state::winning:
            if (this->game_data.state.counter >= game::constants::STATE_WINNING_TIME)
            {
                this->game_data.state = game::game_state::win;
            }
            break;

        case game::game_state::win:
            this->game_data.state = game::game_state::play_init;
            this->game_data.player_status().level++;
            break;

        case game::game_state::dying:
            if (this->game_data.state.counter >= game::constants::STATE_DYING_TIME)
            {
                this->game_data.state = game::game_state::dead;
            }
            break;

        case game::game_state::dead:
            //if (this->game_data.player_status().lives)
            //{
            //    this->game_data.player_status().lives--;
            //}

            this->game_data.current_player = (this->game_data.current_player + 1) % this->game_data.player_turn_count();
            break;
    }

    return ff::state::advance_time();
}

void game::play_state::render(ff::dxgi::command_context_base& context, ff::render_targets& targets)
{
    if (this->renderer.can_render(this->play_level))
    {
        ff::dxgi::target_base& target = targets.target(context, ff::render_target_type::palette);
        ff::dxgi::depth_base& depth = targets.depth(context);
        if (ff::dxgi::draw_ptr draw = ff::dxgi_client().global_draw_device().begin_draw(context, target, &depth))
        {
            this->renderer.render(*draw, this->play_level);
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
    this->game_data.state = game::game_state::play_ready;

    for (size_t i = 0; i < this->game_data.current_player_count(); i++)
    {
        this->game_data.players[this->game_data.current_player + i].init_playing(this->game_data, i);
    }

    this->game_data.level() = game::get_level(this->game_data.game_type, this->game_data.game_diff, this->game_data.player_status().level);
    this->play_level = { &this->game_data, &this->audio };
}

void game::play_state::init_resources()
{
    ff::auto_resource<ff::input_mapping> player_mapping[]
    {
        "player_controls"sv,
        "player_controls_left"sv,
        "player_controls_right"sv
    };

    for (size_t i = 0; i < game::constants::MAX_PLAYERS; i++)
    {
        std::vector<const ff::input_vk*> devices{ &ff::input::keyboard() };

        if (!this->game_data.coop())
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

        const ff::input_mapping& mapping = !this->game_data.coop() ? *player_mapping[0].object() : *player_mapping[i ? 2 : 1].object();
        this->player_input[i] = std::make_unique<ff::input_event_provider>(mapping, std::move(devices));
    }

    this->renderer.init_resources();
}
