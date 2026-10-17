#ifndef __MENU_H
#define __MENU_H
/** @file menu.h
 * @brief file containing declarations of the functions that deal with the game menus
 */

/** @defgroup menu menu
 * @{
 *
 * @brief functions for handling game menus
 */

#include "mouse.h"
#include "keyboard.h"

/**@{
 * @brief enumeration of menu states
 */
typedef enum {
	MENU_INIT,			/**< @brief initial menu state */
	MENU_NEW,			/**< @brief new game menu state */
	MENU_LOAD,			/**< @brief load menu state */
	MENU_SINGLE,		/**< @brief single player menu state */
	MENU_HOT,			/**< @brief hot seat menu state */
	MENU_SUDDEN,		/**< @brief sudden death menu state */
	MENU_DUEL,			/**< @brief two player menu state */
	MENU_DONE			/**< @brief done (ready to play) menu state */
} MenuState;
/** @}*/

/**@{
 * @brief enumeration of game modes
 */
typedef enum {
	SINGLE_MODE,		/**< @brief single player game mode */
	HOT_MODE,			/**< @brief hot seat game mode */
	DUEL_MODE,			/**< @brief two player game mode */
	SUDDEN_MODE,		/**< @brief sudden death game mode */
	SLOT_1_MODE,		/**< @brief load game from slot 1 */
	SLOT_2_MODE,		/**< @brief load game from slot 2 */
	SLOT_3_MODE,		/**< @brief load game from slot 3 */
	SLOT_4_MODE			/**< @brief load game from slot 4 */
} Mode;
/** @}*/

/**@{
 * @brief enumeration of menu buttons
 */
typedef enum {
	NEW_BUTTON,			/**< @brief new game button */
	LOAD_BUTTON,		/**< @brief load game button */
	SINGLE_BUTTON,		/**< @brief single player game button */
	HOT_BUTTON,			/**< @brief hot seat game button */
	SUDDEN_BUTTON,		/**< @brief sudden death game button */
	DUEL_BUTTON,		/**< @brief two player game button */
	SLOT_1_BUTTON,		/**< @brief load game from slot 1 button */
	SLOT_2_BUTTON,		/**< @brief load game from slot 2 button */
	SLOT_3_BUTTON,		/**< @brief load game from slot 3 button */
	SLOT_4_BUTTON,		/**< @brief load game from slot 4 button */
	NONE_BUTTON			/**< @brief for when no button was selected */
} MenuButton;
/** @}*/

/**@{
 * @brief Menu info
 */
typedef struct {
	MenuState state;						/**< @brief menu state */
	Mode mode;								/**< @brief game mode */
	unsigned short number_players;			/**< @brief number of players of the game */
	unsigned short player_names[9][5];		/**< @brief names of the game players */
	unsigned short current_name;			/**< @brief current name that is being filled */
	unsigned short current_letter;			/**< @brief current letter of the name that is being filled */
	char names_done;						/**< @brief indicator that the names have all been filled */
} Menu;
/** @}*/

/**
 * @brief creates a new game menu
 * @return pointer to the new menu created
 */
Menu *create_menu();

/**
 * @brief resets a game menu
 * @param menu to be reset
 */
void reset_menu(Menu *menu);

/**
 * @brief draws the menu according to its current state
 * @param menu menu to be drawed
 * @param base buffer to draw the menu
 * @param origin original buffer to use as background
 * @param numbers numbers xpms to use in the menu drawing
 * @param letters letters xpms to use in the menu drawing
  */
void draw_menu(Menu *menu, char *base, char *origin, char ***numbers, char ***letters);

/**
 * @brief handles keyboard and mouse interruptions while the game is in the menus
 * @param int_irq interruption irq
 * @param mse_irq mouse irq line
 * @param kbd_irq keyboard irq line
 * @param menu menu whose interruptions we are handling
 * @param menu_buffer buffer to draw the menu
 * @param backg_buffer buffer containing the background for the menu
 * @param mouse_buffer buffer to draw the mouse of the menu
 * @param video_mem pointer to the video memory
 * @param numbers array of xpms with the numbers to use in the menu drawings
 * @param letters array of xpms with the letters to use in the menu drawings
 * @param h_res horizontal resolution of the screen
 * @param v_res vertical resolution of the screen
 * @param mouse pointer to the game mouse
 * @param keyboard pointer to the game keyboard
 * @return current state of the menu
 */
MenuState mouse_keyboard_menu_handler(int int_irq, int mse_irq, int kbd_irq, Menu *menu,
		char *menu_buffer, char *backg_buffer, char *mouse_buffer, char *video_mem, char ***numbers, char ***letters,
		unsigned h_res, unsigned v_res, Mouse *mouse, Keyboard *keyboard);

/** @} end of menu */
#endif /* __MENU_H */
