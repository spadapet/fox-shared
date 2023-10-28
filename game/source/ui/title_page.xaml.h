#pragma once

namespace game
{
    struct game_data;

    class title_page_view_model : public ff::ui::notify_propety_changed_base
    {
    public:
        title_page_view_model() = default;
        title_page_view_model(game::game_data& game_data);

    private:
        void play_command(Noesis::BaseComponent* param);

        game::game_data* game_data{};
        Noesis::Ptr<Noesis::ICommand> play_command_;

        NS_DECLARE_REFLECTION(game::title_page_view_model, ff::ui::notify_propety_changed_base);
    };

    class title_page : public Noesis::UserControl
    {
    public:
        title_page();
        title_page(game::title_page_view_model* view_model);

        game::title_page_view_model* view_model() const;

    private:
        Noesis::Ptr<game::title_page_view_model> view_model_;

        NS_DECLARE_REFLECTION(game::title_page, Noesis::UserControl);
    };
}
