#pragma once

namespace game
{
    enum class dir;
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
        void update_shooter(game::play_level& play, game::shooter_data& shooter);
        void update_shot(game::play_level& play, game::shot_data& shot);
        void player_hit_tile(game::play_level& play, game::player_data& player, ff::point_int tile);
        void add_shot(game::play_level& play, ff::point_int pos, game::dir dir);
        void add_score(game::play_level& play, game::player_data& player, game::tile_type tile_type);
        void check_hit(game::play_level& play);
        bool check_win(game::play_level& play);
        bool check_dead(game::play_level& play);
    };
}
