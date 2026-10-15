#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <stdbool.h>

typedef enum {
    GAME_OVER_ACTION_NONE = 0,
    GAME_OVER_ACTION_QUIT,
} GameOverAction;

/**
 * @brief Draws the game-over screen for the winning team.
 * @param winner_team Winning team index, or -1 for a draw.
 */
void game_over_draw(int winner_team);

/**
 * @brief Updates game-over menu input.
 * @param confirm True when the player confirms leaving the screen.
 * @return Selected game-over action.
 */
GameOverAction game_over_update(bool confirm);

#endif /* GAME_OVER_H */
