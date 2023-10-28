#include "pch.h"
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
        this->game_data->state = game::game_state::play;
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
    Noesis::GUI::LoadComponent(this, "title_page.xaml");
}

game::title_page_view_model* game::title_page::view_model() const
{
    return this->view_model_;
}
