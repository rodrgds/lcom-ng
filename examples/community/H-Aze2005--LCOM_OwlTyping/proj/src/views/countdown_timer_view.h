/**
 * @file countdown_timer_view.h
 * @brief Countdown timer view rendering functions.
 *
 * Declares functions and variables for drawing the countdown timer on the screen,
 * including XPM images for countdown numbers.
 */

#ifndef _COUNTDOWN_TIMER_VIEW_
#define _COUNTDOWN_TIMER_VIEW_

#include "devices/timer.h"
#include "devices/video.h"
#include "model/model.h"

#include "images/chars/count_down_numbers/0.xpm"
#include "images/chars/count_down_numbers/1.xpm"
#include "images/chars/count_down_numbers/2.xpm"
#include "images/chars/count_down_numbers/3.xpm"
#include "images/chars/count_down_numbers/4.xpm"
#include "images/chars/count_down_numbers/5.xpm"
#include "images/chars/count_down_numbers/6.xpm"
#include "images/chars/count_down_numbers/7.xpm"
#include "images/chars/count_down_numbers/8.xpm"
#include "images/chars/count_down_numbers/9.xpm"
#include "images/chars/count_down_numbers/s.xpm"

/**
 * @brief Array of XPM images for countdown digits 0-9.
 */
extern xpm_map_t countdown_keys[10];

/**
 * @brief Draws the countdown timer with the specified remaining time.
 * @param remaining_time The time remaining to display (in seconds).
 */
void draw_countdown_timer(int remaining_time);

#endif
