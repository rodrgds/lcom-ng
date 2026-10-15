/**
 * @file menu_view.h
 * @brief Handles the visual representation of the main menu screen
 * 
 * This file contains declarations for functions related to rendering
 * the main menu screen, including time-sensitive visual effects and
 * animated elements.
 */

#ifndef MENU_VIEW_H
#define MENU_VIEW_H


#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "controller/graphics.h"
#include "controller/rtc.h"

#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"
#include "model/menu.h"
#include "view/sprite_view.h"


/**
 * @brief Determines if the current time represents daytime
 * 
 * Checks if the provided hour and minute fall within the defined
 * daytime period (between DAY_START_HOUR:DAY_START_MINUTE and 
 * DAY_END_HOUR:DAY_END_MINUTE).
 *
 * @param hour Current hour (0-23)
 * @param minute Current minute (0-59)
 * @return true if it's daytime, false otherwise
 */
bool daytime(uint8_t hour, uint8_t minute);

/**
 * @brief Draws the menu background with time-sensitive elements
 * 
 * Fills the screen with a background color that changes based on time of day
 * (light blue during day, dark blue at night). Also renders decorative elements
 * like sun/moon, clouds/stars, the game logo, and a prompt at the bottom.
 *
 * @param buffer Pointer to the graphics buffer where the background will be drawn
 */
void draw_menu_background(uint8_t *buffer);

/**
 * @brief Draws the complete menu screen with animations
 * 
 * Copies the static menu background to the current buffer and draws an animated
 * ship that moves back and forth across the bottom of the screen. The ship
 * sprite changes direction based on its movement.
 */
void draw_menu();


#endif 
