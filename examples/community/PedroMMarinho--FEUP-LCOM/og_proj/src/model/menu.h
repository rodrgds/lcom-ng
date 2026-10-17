#pragma once
#include <lcom/lcf.h>
#include "mouseModel.h"
#include "vector.h"
#include "button.h"

/**
 * @brief Enumerates the possible menu types
 * 
 */
typedef enum MenuType {
  MAIN_MENU,
  GAME_MODE_MENU,
  INSTRUCTIONS_MENU,
  ONLINE_MENU,
  OPTIONS_MENU,
  GAME_OVER_MENU,
} MenuType;

/**
 * @brief Struct that represents a menu
 * 
 * backgroundImg - xpm image of the background
 * mainMenuLogo - xpm image of the logo
 * font - xpm image of the font
 * type - type of the menu
 * nOptions - number of options(buttons)
 * selectedOption - index of the selected option
 * mouse - pointer to the mouse
 * mouseImg - xpm image of the mouse
 * mouseImgHover - xpm image of the mouse when it is hovering
 * buttons - array of pointers to the buttons
 *
 */
typedef struct Menu {
  xpm_image_t backgroundImg;
  xpm_image_t mainMenuLogo;
  xpm_image_t *font;
  MenuType type;  
  int nOptions;
  int selectedOption;
  char* winner;
  Mouse* mouse;
  xpm_image_t mouseImg;
  xpm_image_t mouseImgHover;
  Button** buttons;
} Menu;

/**
 * @brief Creates a new menu
 * 
 * @param type type of the menu
 * @return Menu* Pointer to the menu
 */

Menu* newMenu(MenuType type);

Menu* newGameOverMenu(char* winnerName);

/**
 * @brief Frees the memory allocated for the menu
 * 
 * @param menu Menu to be destroyed
 */

void destroyMenu(Menu* menu);

/**
 * @brief Sets the type of the menu
 * 
 * @param menu Menu to be modified
 * @param type type of the menu
 */
void setMenuType(Menu* menu, MenuType type);

/**
 * @brief Resets the menu to its initial state.
 *
 * This function resets the menu by deselecting any selected option and destroying all buttons in the menu.
 *
 * @param menu Pointer to the Menu structure to reset.
 */
void resetMenu(Menu* menu);
/**
 * @brief Draws the menu on the screen.
 *
 * This function draws the menu background, logo (if it's the main menu), player name (if it's the options menu),
 * ellipses (if it's the online menu), all buttons, and the mouse cursor.
 *
 * @param menu Pointer to the Menu structure representing the menu to draw.
 * @param time The current time, used for animating certain menu elements.
 * @param playerName The name of the player to be displayed in the menu.
 * @return 0 on success, 1 on failure.
 */
int drawMenu(Menu* menu,int time,char* playerName);

/**
 * @brief Checks if the mouse is over any menu option.
 *
 * This function iterates through all the options in the menu and checks if the mouse is over any of them.
 * If the mouse is over an option, it sets the option index else sets index to -1.
 *
 * @param menu Pointer to the Menu structure representing the menu.
 * @param option Pointer to an integer where the index of the option under the mouse will be stored.
 * @return 0 on success.
 */
int seeIfMouseIsOverOption(Menu* menu,int *option);

/**
 * @brief Updates the selected option in the menu.
 *
 * This function updates the menu to reflect which option is currently selected. It sets the selected state
 * of each button in the menu based on the given option index.
 *
 * @param menu Pointer to the Menu structure representing the menu.
 * @param option The index of the option to be selected.
 */
void updateSelectedOption(Menu* menu, int option);

/**
 * @brief Destroys a font.
 *
 * This function frees the memory allocated for a font. It iterates over each character image in the font,
 * frees its bytes, and then frees the font array itself.
 *
 * @param font Pointer to the array of xpm_image_t structures representing the font.
 */
void destroyFont(xpm_image_t* font);
