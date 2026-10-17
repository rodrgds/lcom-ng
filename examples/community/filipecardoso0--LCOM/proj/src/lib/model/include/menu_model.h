#ifndef MENU_MODEL_H
#define MENU_MODEL_H

#include "xpm.h"
#include "../include/sprite.h"
#include "../../../drivers/mouse/include/mouse.h"


/* XPM IMAGES */
#include "../../../images/exitbutton.xpm"
#include "../../../images/exitbutton_selected.xpm"
#include "../../../images/snake.io.xpm"
#include "../../../images/start.xpm"
#include "../../../images/start_selected.xpm"
#include "../../../images/mouse_cursor_update.xpm"

/**
 * @brief Struct that represents the menu. It contains the width and height of the screen and its components as well.
 * 
 */
struct menu {
  uint16_t width, height;
  int current_entry;
  int n_entries;
  sprite_t* models[3];
  sprite_t* selected_models[3];
  mouse_cursor_t* mouse_cursor, mouse_cursor_dbuffer; 
};

typedef struct menu menu_t;

/**
 * @brief Creates a new menu.
 * 
 * @param width Menu's width.
 * @param height Menu's height.
 * @return menu_t* Points to the menu created.
 */
menu_t* menu_new(uint16_t width, uint16_t height);

/**
 * @brief Deletes a menu.
 * 
 * @param menu Points to the menu to be deleted.
 */
void menu_delete(menu_t** menu);

/**
 * @brief Returns the menu's width.
 * 
 * @param menu Points to the menu that will retrieve its width.
 * @return uint16_t Menu's width value.
 */
uint16_t menu_get_width(menu_t* menu);

/**
 * @brief Returns the menu's height.
 * 
 * @param menu Points to the menu that will retrieve its height.
 * @return uint16_t Menu's height value.
 */
uint16_t menu_get_height(menu_t* menu);

/**
 * @brief Updates the menu's next entry.
 * 
 * @param menu Points to the menu that contains the entry to be updated.
 */
void menu_next_entry(menu_t* menu);

/**
 * @brief Updates the menu's previous entry.
 * 
 * @param menu Points to the menu that contains the entry to be updated.
 */
void menu_prev_entry(menu_t* menu);

/**
 * @brief Checks if the user has selected the exit entry.
 * 
 * @param menu Points to the menu that contains the exit entry.
 * @return true if has selected the exit entry.
 * @return false otherwise.
 */
bool menu_is_selected_exit(menu_t* menu);

/**
 * @brief Checks if the user has selected the play entry.
 * 
 * @param menu Points to the menu that contains the play entry.
 * @return true if has selected the play entry.
 * @return false otherwise.
 */
bool menu_is_selected_1p(menu_t* menu);

/**
 * @brief Checks if a specified entry has been selected.
 * 
 * @param menu Points to the menu that contains the input entry.
 * @param i entry index.
 * @return true the user has selected the specified entry.
 * @return false otherwise.
 */
bool menu_is_selected(menu_t* menu, int i);

#endif
