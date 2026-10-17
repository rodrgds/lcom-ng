#ifndef _MENU_H_
#define _MENU_H_

#include <lcom/lcf.h>
#include "../devices/rtc/rtc.h"
#include "game.h"
#include "sprite.h"


/**
 * @brief The available options in the game menu.
 */
typedef enum Option {
  SINGLEPLAYER, /** Singleplayer option */
  MULTIPLAYER, /** Multiplayer option */
  QUIT /** Quit option */
} Option;

Sprite *hours[2]; /** @brief Array of sprites that represent the hours of the clock. */
Sprite *minutes[2];/** @brief Array of sprites that represent the minutes of the clock. */

/**
 * @brief Gets the current option of the menu.
 * @return The current option of the menu.
 */
int(getCurrentOption)();

/**
 * @brief Sets the current option of the menu.
 * @param option The option to set as the current option.
 */
void(setCurrentOption)(int option);

/**
 * @brief Increases the current option of the menu.
 */
void(increaseCurrentOption)();

/**
 * @brief Decreases the current option of the menu.
 */
void(decreaseCurrentOption)();

/**
 * @brief Constructs the hours and minutes sprites based on the current time already stored in the rtc_time structure.
 * @param timeRTC Pointer to the rtc_time structure already filled with the current time.
 * @return 0 on success, non-zero otherwise.
 */
int(readTime)(rtc_time *timeRTC);

/**
 * @brief Draws the game menu, with the current option highlighted, the current time and the corresponding background.
 * @param isDay Boolean value indicating whether it is day or night.
 */
void(draw_menu)(bool isDay);

#endif
