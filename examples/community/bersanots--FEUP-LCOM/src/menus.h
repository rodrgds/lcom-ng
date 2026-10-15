/**
 * @file menus.h
 * @author Bernardo Santos (bernardo.ots@fe.up.pt)
 * @brief Menu handling
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MENUS_H
#define __MENUS_H

#include <lcom/lcf.h>

/** @defgroup menus Menus
 * @{
 *
 * Menu handling
 */

/**
 * @brief Create a mouse object as a sprite.
 * 
 */
void create_mouse();

/**
 * @brief Change the mouse position on the screen.
 * 
 */
void move_mouse();

/**
 * @brief Set the mouse speed.
 * 
 * @param xspeed mouse speed on the x axis
 * @param yspeed mouse speed on the y axis
 */
void set_mouse_speed(int xspeed, int yspeed);

/**
 * @brief Animate the mouse according to its attributes in memory.
 * First erase it, then move it and finally redraw it.
 * 
 */
void draw_mouse();

/**
 * @brief Erase the mouse sprite from memory and release used resources.
 * 
 */
void destroy_mouse();

/**
 * @brief Get the main menu pixmap.
 * 
 * @return main menu pixmap 
 */
uint8_t *get_main_menu_pixmap();

/**
 * @brief Get the instructions screen pixmap.
 * 
 * @return instructions pixmap 
 */
uint8_t *get_instructions_pixmap();

/**
 * @brief Load the main menu pixmap and draw it on the screen.
 * 
 * @return 0 on success, 1 on failure 
 */
int draw_main_menu();

/**
 * @brief Load the instructions pixmap and draw it on the screen.
 * 
 * @return 0 on success, 1 on failure  
 */
int draw_instructions();

/**
 * @brief Menus loop, handling the user's clicks on the screen.
 * 
 * @param st current game state, indicating if the wanted screen is the main menu or the instructions
 * @return next event, depending on the user's action 
 */
event_t menus(state_t st);

/** @} end of menus */

#endif
