/** @defgroup menu menu
 * @{
 *
 * utils for menu
 */

#ifndef MENU_H
#define MENU_H

#define PLAY_BUTTON_Y           200
#define HIGHSCORES_BUTTON_Y     400
#define EXIT_BUTTON_Y           600

#include "xpm.h"
#include "font.h"
#include "cursor.h"

/* ------------- XPMs --------------- */

#include "pixmap/menu/exit_button.h"
#include "pixmap/menu/exit_button_over.h"
#include "pixmap/menu/highscores_button.h"
#include "pixmap/menu/highscores_button_over.h"
#include "pixmap/menu/play_button.h"
#include "pixmap/menu/play_button_over.h"
#include "pixmap/menu/highscores.h"

/*------------------------------------*/

typedef enum {
    VOID,
    PLAY_BUTTON_OVER, 
    HIGHSCORES_BUTTON_OVER,
    EXIT_BUTTON_OVER,
    HIGHSCORES
} Menu;

typedef enum {
    NON,
    LB_PRESS,
    LB_UP,
    CLICKED    
} LeftClick;


/**
 * @brief Initializes the menu images and data
*/
int startMenu();

/**
 * @brief Handles the input on the menu wether it is from the mouse or keyboard
 * @param mouse is the input mouse
 * @param kbd is the input keyboard
 * @param pp mouse packet
 * @param kbdData keyboard buffer data
*/
int menuIH(bool mouse, bool kbd, struct packet* pp, uint32_t kbdData);

/**
 * @brief Displays the menu on the screen
*/
int displayMenu();

/**
 * @brief Displays the highscores on the screen
*/
void displayHighscores();

/**
 * @brief Displays the scores on the screen
*/
void displayScores();

#endif

/**@}*/
