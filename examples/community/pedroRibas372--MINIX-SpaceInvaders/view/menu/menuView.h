#ifndef _MENU_VIEW_H_
#define _MENU_VIEW_H_
#include "../../global.h"
#include "../../controllers/graphicsCard/graphics.h"


/**
 * @brief Invokes the other draw functions for the main menu
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawMenu();

/**
 * @brief Draws the xpm image for the Start Button
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawStartButton();

/**
 * @brief Draws the xpm image for the Exit Button
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawExitButton();

/**
 * @brief Draws the xpm image for the Level Button
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawLevelButton();

/**
 * @brief Draws the xpm image for a game Won Scenario
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawWinButton();

/**
 * @brief Draws the xpm image for a game Lost Scenario
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int drawLoseButton();





#endif
