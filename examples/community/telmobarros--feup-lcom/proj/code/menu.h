/**
 * This module contains all functions that handle the menu and his buttons
 * Jorge Vale: 90%
 * Telmo Barros: 10%
 * Relative Weight: 7%
 */
#ifndef __MENU_H
#define __MENU_H
#include "bitmap.h"
#include <stdint.h>

/** @defgroup Menu Menu
 * @{
 * Functions for manipulating menu
 */

/** @name Main menu states*/
/** @{ */
typedef enum {
	BUTTON0_SELECT,
	BUTTON1_SELECT,
	BUTTON2_SELECT,
	BUTTON3_SELECT,
	BUTTON4_SELECT,
	BUTTON5_SELECT,
	SINGLEPLAYER,
	MULTIPLAYER,
	BATTLE,
	HIGHSCORES,
	INSTRUCTIONS,
	SHUTDOWN,
	DONOTHING
} main_menu_state;
/** @} end of Maine menu states */

/** @name Events of mouse related to click in main menu buttons and pass by */
/** @{ */
typedef enum main_menu_event {
	LEFT_CLICK,
	BUTTON0_ISABOVE,
	BUTTON1_ISABOVE,
	BUTTON2_ISABOVE,
	BUTTON3_ISABOVE,
	BUTTON4_ISABOVE,
	BUTTON5_ISABOVE,
	NOACTION
} main_menu_event;
/** @} end of Maine menu events */

/** @name Button structure */
/** @{
 *
 * Button structure
 */
typedef struct {
	unsigned int x,y;	// button position
	unsigned int width, height; // button dimensions
	uint16_t * above;	// button image when mouse is above the button
	uint16_t * null;	// button image when mouse is not above the button
	unsigned int isAbove;	// flag that stores if mouse is above the button
} Button;
/** @} end of Button structure */

/** @name Menu structure */
/** @{
 *
 * Menu structure
 */
typedef struct {
	Bitmap* background;	// background image
	main_menu_state state;	// current main menu state
	main_menu_event event;	//current mouse main menu event
	Button * buttons;	// pointer to all menu buttons
} Menu;
/** @} end of Menu structure */

/**
 * @brief Menu(main menu) constructor
 *
 * @return A pointer to the menu
 */
Menu * new_main_menu();

/**
 * @brief Updates menu state given the current events
 *
 * @param menu Menu to change
 */
void update_main_menu_state(Menu * menu);

/**
 * @brief Updates menu buttons look based on current state
 *
 * @param menu Menu to change
 */
void update_main_menu(Menu * menu);

/**
 * @brief Draws backgorund image and all the buttons in the video buffer
 *
 * @param menu Menu to draw
 */

void draw_main_menu(Menu * menu);

/**
 * @brief Deletes the menu by releasing the allocated memory
 *
 * @param menu Menu to destroy
 */

void delete_main_menu(Menu * menu);

/**@}*/

#endif /* __MENU_H */

