#include "pch.h"
#include "source/core/game.h"
#include "source/core/game_update.h"

game::game_update::game_update()
{}

void game::game_update::update(game::play_level& play)
{
    if (!play.game_data || !play.level || play.game_data->state != game::game_state::playing)
    {
        return;
    }

    play.level->counter++;

    for (game::player_data& player : play.game_data->players)
    {
        player.counter++;

        if (player.press.x > 0_f)
        {
            player.dir = { 1, 0 };
        }
        else if (player.press.x < 0_f)
        {
            player.dir = { -1, 0 };
        }
        else if (player.press.y > 0_f)
        {
            player.dir = { 0, 1 };
        }
        else if (player.press.y < 0_f)
        {
            player.dir = { 0, -1 };
        }
    }
}
