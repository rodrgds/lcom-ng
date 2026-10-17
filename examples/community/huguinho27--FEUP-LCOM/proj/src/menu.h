#ifndef MENU_H
#define MENU_H

/** @defgroup Menu Menu
 * @{
 * Functions for manipulating game menu screens
 */

typedef enum {PLAY, _EXIT, RESUME, BACK_MENU} option_st;

struct Menu;
typedef struct Menu menu_t;

/**
 * @brief Creates a new instance of Menu,
 * allocating memory for it
 * @return Pointer to the new allocated Menu structure
 */
menu_t* createMenu();

/**
 * @brief Handles the main menu options
 * @return 1 - play was selected; 2 - highscores selected;
 * 3- exit selected
 */
int selectionHandler();

/**
 * @brief Handles the pause menu options
 * @return 1 - resume; 2 - back to main menu
 */
int pauseMenu();

#endif /**@}*/
