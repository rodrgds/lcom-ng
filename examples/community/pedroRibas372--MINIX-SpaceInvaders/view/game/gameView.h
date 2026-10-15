#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_
#include "../../global.h"
#include "../../controllers/graphicsCard/graphics.h"

/**
 * @brief Draws the xpm image for any number value (lvl, score, hp)
 *
 * @param value the number that is to be printed
 * @param digits the number of consecutive digits that will be printed
 * @param x position referencing to the x axis
 * @param y position referencing to the y axis
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_number_value(int value, int digits, int x, int y);

/**
 * @brief Draws the xpm image for the "LVL" title display;
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawlvl();

/**
 * @brief Draws the xpm image for the "SCR" title display;
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawscr();

/**
 * @brief Draws the xpm image for the "HP" title display;
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawhp();

/**
 * @brief Calls the functions to draw the level, score and HP (including the numbers)
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawHeader();

/**
 * @brief Draws every enemy that was created
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawEnemies();

/**
 * @brief Draws the hero
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawHero();

/**
 * @brief Calls the functions to draw the hero and the enemies
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawGame();
#endif
