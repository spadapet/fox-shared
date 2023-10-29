#pragma once

namespace game
{
    struct play_level;

    class game_update
    {
    public:
        game_update();

        void update(game::play_level& play);

    private:
    };
}
