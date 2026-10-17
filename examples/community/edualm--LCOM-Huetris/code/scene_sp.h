#ifndef SCENE_SP_H_
#define SCENE_SP_H_

/** @defgroup scene_sp scene_sp
 * @{
 * Functions for controlling the singleplayer game scene.
 */

/**
 * @brief Draws the game controls.
 */

void sceneSingle_drawControls();

/**
 * @brief Draws the board's contents.
 */

void sceneSingle_drawBoard();

/**
 * @brief Draws the next tetrimino.
 */

void sceneSingle_drawNext();

/**
 * @brief Draws the time.
 */

void sceneSingle_drawRTC();

/**@}*/

#endif /* SCENE_SP_H_ */