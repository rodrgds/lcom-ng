/**
 * @file views.h
 * @brief View rendering functions for the application's UI screens and virtual keyboard.
 *
 * Declares functions for drawing the main screens (home, typing, pause, stats),
 * the virtual keyboard, and statistics results. Also declares global variables
 * for final WPM, accuracy, and key performance mapping.
 */

#ifndef _VIEWS_H_
#define _VIEWS_H_

#include "devices/video.h"
#include "images/home_screen.xpm"
#include "images/typing_screen.xpm"
#include "images/pause_screen.xpm"
#include "images/stats_screen.xpm"
#include "utils/utils.h"
#include "devices/mouse.h"
#include "controllers/virtual_keyboard_controller.h"
#include "include_xpm.h"
#include "typing_test_view.h"
#include "views/countdown_timer_view.h"
#include "controllers/typing_test_controller.h"

/**
 * @brief Final words per minute (WPM) after the test.
 */
extern int final_wpm;

/**
 * @brief Final accuracy percentage after the test.
 */
extern int final_accuracy;

/**
 * @brief Array storing performance data for each key.
 */
extern KeyPerformance perf_map[NUM_KEYS];

/**
 * @brief Draws the current screen based on the application state.
 */
void draw_screen();

/**
 * @brief Draws the home screen.
 */
void draw_home();

/**
 * @brief Draws the typing test screen.
 */
void draw_typing();

/**
 * @brief Draws the pause screen.
 */
void draw_pause();

/**
 * @brief Draws the statistics/results screen.
 */
void draw_stats();

/**
 * @brief Draws the statistics result with the given speed and accuracy.
 * @param speed Words per minute (WPM) to display.
 * @param accuracy Accuracy percentage to display.
 */
void draw_stats_result(int speed, int accuracy);

/**
 * @brief Draws the virtual keyboard.
 */
void draw_keyboard();

/**
 * @brief Updates the virtual keyboard display when a key changes state.
 * @param c The character of the key.
 * @param make True if the key is pressed (make), false if released (break).
 */
void draw_keyboard_on_change(char c, bool make);

/**
 * @brief Draws the virtual keyboard with key performance statistics.
 * @param key_perf Array mapping each key to its performance rating.
 */
void draw_stats_keyboard(const KeyPerformanceMap key_perf);

#endif
