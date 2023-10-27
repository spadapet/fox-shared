#pragma once

namespace game
{
    enum class game_type
    {
        one_player,
        two_player,
        coop,
    };

    enum class game_diff
    {
        baby,
        easy,
        normal,
        hard,
    };

    enum class game_state
    {
        none,
        title,
        playing,
        paused,
    };

    enum class player_state
    {
        none,
        playing,
        dying,
        dead,
        game_over,
    };

    enum class level_type
    {
        normal,
        challenge,
        bonus,
    };

    enum class level_state
    {
        none,
        ready,
        playing,
        won,
    };

    enum class tile_type : uint8_t
    {
        none,
        panel0,
        panel1,
    };

    using tile_array = typename std::array<game::tile_type, game::constants::TILE_COUNT_MEM>;

    struct player
    {
        size_t index{};
        size_t score{};
        size_t lives{};
        size_t counter{};
        game::player_state state{};
        ff::point_fixed pos{};
        ff::point_fixed dir{};
        ff::point_fixed press{};
        bool shoot{};
    };

    using player_array = typename std::array<game::player, game::constants::MAX_PLAYERS>;

    struct level
    {
        game::tile_type tile(ff::point_size pos) const;
        void tile(ff::point_size pos, game::tile_type value);

        size_t index{};
        size_t counter{};
        size_t max_timer{};
        game::level_type level_type{};
        game::tile_array tiles{};
        game::level_state state{};
    };

    using level_array = typename std::array<game::level, game::constants::MAX_PLAYERS>;

    struct game_data
    {
        game::game_type game_type{};
        game::game_diff game_diff{};
        game::player_array players{};
        game::game_state state{};
        size_t counter{};
    };

    struct play_level
    {
        game::game_data& game_data;
        game::level& level;
    };
}
