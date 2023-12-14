#pragma once

#include "source/core/constants.h"

namespace game
{
    class audio;
    struct game_data;

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
        play_init_from_title,
        play_init,
        play_ready,
        playing,
        paused,
        winning,
        win,
        dying,
        dead,
        game_over,
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
        T state{};
        size_t state_counter{};
        size_t total_counter{};

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
            if (this->state != new_state)
            {
                this->state = new_state;
                this->state_counter = 0;
            }
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
            this->state_counter++;
            this->total_counter++;
        }

        void set_at(T new_state, size_t at_least_counter)
        {
            if (this->state_counter >= at_least_counter)
            {
                this->set(new_state);
            }
        }
    };

    struct level_data
    {
        game::tile_type tile(ff::point_size pos) const;
        void tile(ff::point_size pos, game::tile_type value);

        size_t index{};
        size_t max_timer{};
        game::level_type level_type{};
        game::tile_array tiles{};
    };

    struct player_status
    {
        size_t score{};
        size_t lives{};
        size_t level{};
    };

    struct player_data
    {
        void init_playing(const game::game_data& game, size_t current_index);

        size_t index{};
        game::player_status* status{};
        game::level_data* level{};
        game::state_t<game::player_state> state{};
        game::dir dir{};
        ff::point_int pos{};
        ff::point_int press{};
        ff::fixed_int speed_bank{};

        union
        {
            struct
            {
                bool press_speed : 1;
                bool turned : 1;
                bool collected : 1;
                bool ignore_press_x : 1;
                bool ignore_press_y : 1;
            };

            uint32_t all{};
        } flags;
    };

    using player_status_array = typename std::array<game::player_status, game::constants::MAX_PLAYERS>;
    using player_array = typename std::array<game::player_data, game::constants::MAX_PLAYERS>;
    using level_array = typename std::array<game::level_data, game::constants::MAX_PLAYERS>;

    struct game_data
    {
        bool coop() const;
        size_t total_player_count() const;
        size_t current_player_count() const;
        size_t score_for_tile(game::tile_type tile_type) const;
        size_t default_lives() const;
        ff::fixed_int player_speed(bool press_speed) const;
        game::player_status& player_status() const;
        game::level_data& level() const;

        game::game_type game_type{};
        game::game_diff game_diff{};
        game::state_t<game::game_state> state{};
        game::level_array levels{};
        game::player_status_array statuses{};
        game::player_array players{};
        size_t current_player{};
    };

    struct play_level
    {
        game::player_status& player_status() const;
        game::level_data& level() const;
        size_t state_counter() const;
        size_t total_counter() const;

        game::game_data* game_data{};
        game::audio* audio{};
    };
}
