#pragma once

namespace game
{
    enum class tile_type : unsigned char;
    struct play_level;
    struct player_data;

    class updater
    {
    public:
        updater();

        void update_player_input(game::player_data& player, bool press_left, bool press_right, bool press_up, bool press_down, bool press_speed);
        void update(game::play_level& play);

    private:
        void update_player(game::play_level& play, game::player_data& player);
        void add_score(game::play_level& play, game::player_data& player, game::tile_type tile_type);
        void check_win(game::play_level& play);
    };
}
