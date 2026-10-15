#include "../include/game/game_over.h"
#include <stdio.h>

void game_over_init(game_over_t *game_over,
                    uint32_t final_score,
                    game_over_stats_t stats,
                    uint32_t seed) {
    if (game_over == NULL) return;

    game_over->hovered_option = GAME_OVER_OPTION_NONE;
    game_over->final_score = final_score;
    game_over->stats = stats;
    game_over->seed = seed;
}

GameOverAction game_over_update(game_over_t *game_over, GameOverOption hovered_option, bool clicked) {
    if (game_over == NULL) return GAME_OVER_ACTION_NONE;

    game_over->hovered_option = hovered_option;
    if (!clicked) return GAME_OVER_ACTION_NONE;

    switch (hovered_option) {
        case GAME_OVER_OPTION_MAIN_MENU:
            return GAME_OVER_ACTION_MAIN_MENU;
        case GAME_OVER_OPTION_RESTART_SAME_SEED:
            return GAME_OVER_ACTION_RESTART_SAME_SEED;
        case GAME_OVER_OPTION_NONE:
        default:
            return GAME_OVER_ACTION_NONE;
    }
}
