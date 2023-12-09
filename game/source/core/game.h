#pragma once

namespace game
{
    class audio;

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
        play_ready,
        playing,
        paused,
        winning,
        win,
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

    template<class T, class = std::enable_if_t<std::is_enum_v<T>>>
    struct state_t
    {
        T state;
        size_t counter;

        state_t& operator=(T new_state)
        {
            this->set(new_state);
            return *this;
        }

        bool operator==(T other_state) const
        {
            return this->state == other_state;
        }

        bool operator!=(T other_state) const
        {
            return this->state != other_state;
        }

        operator T() const
        {
            return this->state;
        }

        void set(T new_state)
        {
            this->state = new_state;
            this->counter = 0;
        }

        void ensure(T new_state)
        {
            if (this->state != new_state)
            {
                this->set(new_state);
            }
        }

        void advance_time()
        {
            this->counter++;
        }
    };

    struct player_data
    {
        size_t index{};
        size_t score{};
        size_t lives{};
        size_t level{};
        game::state_t<game::player_state> state{};
        game::dir dir{};
        ff::point_int pos{};
        ff::point_int press{};
        ff::fixed_int speed_bank{};

        union
        {
            struct
            {
                bool fast : 1;
                bool turned : 1;
                bool collected : 1;
                bool ignore_press_x : 1;
                bool ignore_press_y : 1;
            };

            uint32_t all{};
        } flags;
    };

    using player_array = typename std::array<game::player_data, game::constants::MAX_PLAYERS>;

    struct level_data
    {
        game::tile_type tile(ff::point_size pos) const;
        void tile(ff::point_size pos, game::tile_type value);

        size_t index{};
        size_t max_timer{};
        game::level_type level_type{};
        game::state_t<game::level_state> state{};
        game::tile_array tiles{};
    };

    using level_array = typename std::array<game::level_data, game::constants::MAX_PLAYERS>;

    struct game_data
    {
        game::game_type game_type{};
        game::game_diff game_diff{};
        game::state_t<game::game_state> state{};
        game::player_array players{};
        size_t current_player{};
    };

    struct play_level
    {
        game::game_data* game_data{};
        game::level_data* level{};
        game::audio* audio{};
    };
}
