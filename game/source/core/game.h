#pragma once

namespace game
{
    class game_audio;

    enum class game_type
    {
        none,
        one_player,
        two_player,
        coop,
    };

    enum class game_diff
    {
        none,
        baby,
        easy,
        normal,
        hard,
    };

    enum class game_state
    {
        none,
        title,
        play,
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
        none,
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
        bomb,
    };

    using tile_array = typename std::array<game::tile_type, game::constants::TILE_COUNT_MEM>;

    struct player_data
    {
        size_t index{};
        size_t score{};
        size_t lives{};
        size_t level{};
        size_t counter{};
        game::player_state state{};
        ff::point_int pos{};
        ff::point_int press{};
        ff::fixed_int speed{};
        game::dir dir{};
        bool fast : 1{};
        bool turned : 1{};
        bool collected : 1{};
        bool ignore_press_x : 1{};
        bool ignore_press_y : 1{};
    };

    using player_array = typename std::array<game::player_data, game::constants::MAX_PLAYERS>;

    struct level_data
    {
        game::tile_type tile(ff::point_size pos) const;
        void tile(ff::point_size pos, game::tile_type value);

        size_t index{};
        size_t counter{};
        size_t max_timer{};
        game::level_type level_type{};
        game::level_state state{};
        game::tile_array tiles{};
    };

    using level_array = typename std::array<game::level_data, game::constants::MAX_PLAYERS>;

    struct game_data
    {
        game::game_type game_type{};
        game::game_diff game_diff{};
        game::game_state state{};
        game::player_array players{};
        size_t current_player{};
    };

    struct play_level
    {
        game::game_data* game_data{};
        game::level_data* level{};
        game::game_audio* audio{};
    };
}
