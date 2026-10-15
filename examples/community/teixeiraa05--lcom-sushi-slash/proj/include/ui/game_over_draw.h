#ifndef _GAME_OVER_DRAW_H_
#define _GAME_OVER_DRAW_H_

#include <stdint.h>
#include "../game/game_over.h"
#include "../init.h"

/**
 * @file game_over_draw.h
 * @brief Declares game-over screen hit-testing and drawing helpers.
 */

/**
 * @brief Resolve a screen point to a game-over option.
 *
 * @param x Horizontal point coordinate.
 * @param y Vertical point coordinate.
 * @return Game-over option under the point, or GAME_OVER_OPTION_NONE.
 */
GameOverOption ui_game_over_hit_test(int x, int y);

/**
 * @brief Draw the game-over screen.
 *
 * @param game_over Game-over state to render.
 * @param assets Shared assets used for drawing.
 * @return 0 on success, non-zero on failure.
 */
int ui_game_over_draw(const game_over_t *game_over, const Assets *assets);

#endif /* _GAME_OVER_DRAW_H_ */
