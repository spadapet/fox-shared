#include "pch.h"
#include "fox.resm.id.h"
#include "source/core/game.h"
#include "source/ui/title_page.xaml.h"

NS_IMPLEMENT_REFLECTION(game::title_page_view_model, "game.title_page_view_model")
{
    NsProp("play_command", &game::title_page_view_model::play_command_);
}

game::title_page_view_model::title_page_view_model(game::game_data& game_data)
    : game_data(&game_data)
    , play_command_(Noesis::MakePtr<ff::ui::delegate_command>(Noesis::MakeDelegate(this, &game::title_page_view_model::play_command)))
{}

void game::title_page_view_model::play_command(Noesis::BaseComponent* param)
{
    assert_ret(this->game_data);

    if (this->game_data->state == game::game_state::title)
    {
        Noesis::String string_type = Noesis::Boxing::CanUnbox<Noesis::String>(param) ? Noesis::Boxing::Unbox<Noesis::String>(param) : 0;
        game::game_type type = static_cast<game::game_type>(std::atoi(string_type.Str()));

        switch (type)
        {
            case game::game_type::one_player:
                this->game_data->game_type = game::game_type::one_player;
                break;

            case game::game_type::two_player:
                this->game_data->game_type = game::game_type::two_player;
                break;

            case game::game_type::coop:
                this->game_data->game_type = game::game_type::coop;
                break;
        }

        if (this->game_data->game_type != game::game_type::none)
        {
            this->game_data->state = game::game_state::play_init_from_title;
        }
    }
}

NS_IMPLEMENT_REFLECTION(game::title_page, "game.title_page")
{
    NsProp("view_model", &game::title_page::view_model);
}

game::title_page::title_page()
    : view_model_(Noesis::MakePtr<game::title_page_view_model>())
{}

game::title_page::title_page(game::title_page_view_model* view_model)
    : view_model_(view_model)
{
    Noesis::GUI::LoadComponent(this, assets::xaml::TITLE_PAGE_XAML.data());
}

game::title_page_view_model* game::title_page::view_model() const
{
    return this->view_model_;
}
