#include "pch.h"
#include "core/levels.h"
#include "fox.resm.id.h"
#include "states/play_state.h"

constexpr float PALETTE_CYCLES_PER_SECOND = 0.25f;
constexpr int PALETTE_BLACK = 240;
static const ff::color COLOR_BLACK(::PALETTE_BLACK);

game::play_state::play_state(game::game_type game_type, game::game_diff game_diff)
    : game_data
    {
        game_type,
        (game_diff == game::game_diff::none) ? game::game_diff::normal : game_diff,
        (game_type == game::game_type::none) ? game::game_state::title : game::game_state::play_init_from_title,
    }
    , play_level{ &this->game_data, &this->audio }
    , texture_low(ff::dxgi::create_render_texture(game::constants::RENDER_SIZE.cast<size_t>(), DXGI_FORMAT_R8_UINT, 1, 1, 1, &::COLOR_BLACK))
    , target_low(ff::dxgi::create_target_for_texture(this->texture_low.dxgi_texture()))
    , depth_low(ff::dxgi::create_depth(game::constants::RENDER_SIZE.cast<size_t>()))
{
    this->init_resources();
}

game::play_state::~play_state()
{
    // Just needed because of include files for smart pointer destruction
}

void game::play_state::input()
{
    if (this->game_data.state == game::game_state::playing)
    {
        for (const auto& input : this->player_input)
        {
            input->update();
        }

        for (game::player_data& player : this->game_data.players)
        {
            ff::input_event_provider& input = *this->player_input[player.index];
            this->updater.update_player_input(player,
                input.digital_value(game::input_events::ID_LEFT),
                input.digital_value(game::input_events::ID_RIGHT),
                input.digital_value(game::input_events::ID_UP),
                input.digital_value(game::input_events::ID_DOWN),
                input.digital_value(game::input_events::ID_SPEED),
                input.digital_value(game::input_events::ID_SHOOT));
        }
    }
}

void game::play_state::update()
{
    this->game_data.state.update();

    switch (this->game_data.state)
    {
        case game::game_state::title:
            this->game_data.game_type = game::game_type::one_player;
            this->game_data.game_diff = game::game_diff::normal;
            this->game_data.state = game::game_state::play_init_from_title;
            break;

        case game::game_state::play_init_from_title:
            this->init_from_title();
            break;

        case game::game_state::play_init:
            this->init_playing();
            break;

        case game::game_state::play_ready:
            this->game_data.state.set_at(game::game_state::playing, game::constants::STATE_PLAY_READY_TIME);
            break;

        case game::game_state::playing:
            this->updater.update(this->play_level);
            break;

        case game::game_state::winning:
            this->game_data.state.set_at(game::game_state::win, game::constants::STATE_GAME_WINNING_TIME);
            break;

        case game::game_state::win:
            this->init_next_level();
            break;

        case game::game_state::dying:
            this->game_data.state.set_at(game::game_state::dead, game::constants::STATE_GAME_DYING_TIME);
            break;

        case game::game_state::dead:
            this->find_next_player();
            break;

        case game::game_state::game_over_ready:
            this->game_data.state.set_at(game::game_state::game_over, game::constants::STATE_GAME_OVER_TIME);
            break;

        case game::game_state::game_over:
            this->game_data.state = game::game_state::title;
            break;
    }
}

void game::play_state::render_offscreen(ff::dxgi::command_context_base& context)
{
    check_ret(this->renderer.can_render(this->play_level));
    check_ret(this->target_low->begin_render(context, &::COLOR_BLACK));

    if (ff::dxgi::draw_ptr draw = ff::dxgi::global_draw_device().begin_draw(context, *this->target_low, this->depth_low.get()))
    {
        this->renderer.render(*draw, this->play_level);
    }

    this->target_low->end_render(context);
}

void game::play_state::render(ff::dxgi::command_context_base& context, ff::dxgi::target_base& target)
{
    check_ret(this->renderer.can_render(this->play_level));

    ff::viewport viewport(game::constants::RENDER_SIZE.cast<size_t>());
    ff::rect_float view_rect = viewport.view(target.size().logical_scaled_size<size_t>()).cast<float>();

    if (ff::dxgi::draw_ptr draw = ff::dxgi::global_draw_device().begin_draw(context, target, nullptr))
    {
        ff::point_float view_scale = view_rect.size() / game::constants::RENDER_SIZE.cast<float>();
        draw->push_palette(this->palette[0].get());
        draw->draw_sprite(this->texture_low.sprite_data(), ff::transform(view_rect.top_left(), view_scale));
        draw->pop_palette();
    }
}

void game::play_state::find_next_player()
{
    if (!this->game_data.coop())
    {
        if (!this->game_data.player_status().lives)
        {
            game::player_data& player = this->game_data.players[this->game_data.current_player];
            player.state = game::player_state::game_over;
            // TODO: Consider showing "enter high score" screen
        }

        for (size_t i = 1; i <= this->game_data.total_player_count(); i++)
        {
            size_t index = (this->game_data.current_player + i) % this->game_data.total_player_count();
            game::player_data& player = this->game_data.players[index];

            if (player.status->lives)
            {
                player.status->lives--;
                this->game_data.current_player = index;
                this->game_data.state = game::game_state::play_init;
                return;
            }
        }
    }

    this->game_data.state = game::game_state::game_over_ready;
}

void game::play_state::init_from_title()
{
    this->game_data.state = game::game_state::play_init;
    this->game_data.current_player = 0;

    for (size_t i = 0; i < this->game_data.total_player_count(); i++)
    {
        game::player_data& player = this->game_data.players[i];
        player.index = i;
        player.status = &this->game_data.statuses[i * !this->game_data.coop()];
        player.level = &this->game_data.levels[i * !this->game_data.coop()];
        player.state = game::player_state::playing;
    }

    for (size_t i = 0; i < this->game_data.total_player_count(); i += this->game_data.current_player_count())
    {
        game::player_status& status = this->game_data.statuses[i];
        status.level = 0;
        status.lives = this->game_data.default_lives() - (i ? 0 : 1);
        status.score = 0;

        game::level_data& level = this->game_data.levels[i];
        level = this->levels.get_level(this->game_data.game_type, this->game_data.game_diff, status.level);
    }

    this->init_playing_resources();
    this->init_shooters();
}

void game::play_state::init_playing()
{
    this->game_data.state = game::game_state::play_ready;

    for (size_t i = 0; i < this->game_data.current_player_count(); i++)
    {
        game::player_data& player = this->game_data.players[this->game_data.current_player + i];
        player.init_playing(this->game_data, i);
    }

    this->init_shooters();
}

void game::play_state::init_next_level()
{
    size_t level = ++this->game_data.player_status().level;
    this->game_data.level() = this->levels.get_level(this->game_data.game_type, this->game_data.game_diff, level);
    this->game_data.state = game::game_state::play_init;
}

void game::play_state::init_shooters()
{
    this->game_data.shooters = {};
    this->game_data.shots = {};
    this->game_data.player_shots = {};

    this->game_data.shooters[0].shot_dir = game::dir::down;
    this->game_data.shooters[0].move_dir = game::dir::right;
    this->game_data.shooters[0].pos = game::constants::MOVABLE_AREA.top_left() - ff::point_size(0, game::constants::TILE_SIZE_Y / 2).cast<int>();

    this->game_data.shooters[1].shot_dir = game::dir::up;
    this->game_data.shooters[1].move_dir = game::dir::left;
    this->game_data.shooters[1].pos = game::constants::MOVABLE_AREA.bottom_right() + ff::point_size(0, game::constants::TILE_SIZE_Y / 2).cast<int>();
}

void game::play_state::init_resources()
{
    ff::auto_resource<ff::palette_data> palette_data = assets::graphics::PALETTE;

    this->init_playing_resources();
    this->renderer.init_resources();
    this->audio.init_resources();
    this->levels.init_resources();

    // Delay blocking on palette_data until after other resources are loaded
    this->palette[0] = std::make_unique<ff::palette_cycle>(palette_data.object(), "player_0", ::PALETTE_CYCLES_PER_SECOND);
    this->palette[1] = std::make_unique<ff::palette_cycle>(palette_data.object(), "player_1", ::PALETTE_CYCLES_PER_SECOND);
}

bool game::play_state::debug_command(size_t id)
{
    return false;
}

void game::play_state::init_playing_resources()
{
    ff::auto_resource<ff::input_mapping> player_mapping[]
    {
        assets::controls::PLAYER_CONTROLS,
        assets::controls::PLAYER_CONTROLS_RIGHT,
        assets::controls::PLAYER_CONTROLS_LEFT,
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
}
