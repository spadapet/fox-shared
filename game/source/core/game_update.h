#pragma once

namespace game
{
    enum class tile_type : unsigned char;
    struct play_level;
    struct player_data;

    class game_update
    {
    public:
        game_update();

        void update(game::play_level& play);

    private:
        void update_player(game::play_level& play, game::player_data& player);
        void add_score(game::play_level& play, game::player_data& player, game::tile_type tile_type);
    };
}
