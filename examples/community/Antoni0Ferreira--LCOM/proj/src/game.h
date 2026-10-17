#ifndef __GAME_H
#define __GAME_H

#include <lcom/lcf.h>
#include "i8042.h"
#include "UI.h"
#include "mouse.h"
#include "rtc.h"
#include "keyboard.h"
#include "file.h"

/**
 * @defgroup game game
 * @{
 * 
 * Main functions for executing the game
 */

#define PLAYTIME 40 // Time in seconds that the user has to play the game
#define FRAME_RATE 60 // Frame rate of the game

/**
 * @brief Function responsible for handling the main menu. This includes
 * drawing the menu, and letting the user choose an option using the keyboard 
 * or the mouse
 * 
 * @param menubg A xpm map of the menu's background
 * @param imgmenuBG An xpm image of the menu's background
 * @return Return 0 or 2 upon success, 0 if user selects play and 2 if user selects quit; Return 1 otherwise 
 */
int (menu)(uint8_t *menubg, xpm_image_t imgmenuBG);

/**
 * @brief Function responsible for handling the playing portion of the game, like
 * drawing the frame, taking care of the many events that occur, updating positions, etc.
 * 
 * @param bg A xpm map of the game's background
 * @param imgBG An xpm image of the game's background
 * @return Return 0 upon success and non-zero otherwise 
 */
int (play)(uint8_t *bg, xpm_image_t imgBG);

/**
 * @brief Function responsible for handling the end game portion of the project.
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (end)();

/**
 * @}
 */

#endif /* __GAME_H */
