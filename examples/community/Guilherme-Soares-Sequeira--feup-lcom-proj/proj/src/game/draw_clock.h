#ifndef DRAW_CLOCK_H
#define DRAW_CLOCK_H

#include "../devices/rtc/rtc.h"

/** @defgroup draw_clock draw_clock
 *  @{
 *
 * Constants, structures and functions used for programming the clock
 */

#define HOUR_HANDLE_LENGTH 20   /**< @brief the length of the clock's hour handle */
#define MINUTE_HANDLE_LENGTH 32 /**< @brief the length of the clock's minute handle */

/**
 * @brief Loads the clock xpm image
 *
 */
void(clock_load)();

/**
 * @brief Draws the clock on the screen
 *
 */
void(clock_draw)();

/**@}*/

#endif
