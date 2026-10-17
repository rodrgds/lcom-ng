/**
 * @file typing_test_controller.h
 * @brief Controller for managing the typing test logic and statistics.
 *
 * Declares functions and variables for starting, resuming, and managing the typing test,
 * as well as tracking and calculating typing statistics such as WPM and accuracy.
 */

#ifndef TYPING_TEST_CONTROLLER_H
#define TYPING_TEST_CONTROLLER_H

#include "views/typing_test_view.h"
#include "views/countdown_timer_view.h"
#include "texts/phrases.h"
#include "model/model.h"
#include "controllers/virtual_keyboard_controller.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Final words per minute (WPM) after the test.
 */
int final_wpm;

/**
 * @brief Final accuracy percentage after the test.
 */
int final_accuracy;

/**
 * @brief Array storing performance data for each key.
 */
KeyPerformance perf_map[NUM_KEYS];

/**
 * @brief Starts the typing test with a given time limit.
 * @param time Time limit for the typing test in seconds.
 */
void start_typing_test(int time);

/**
 * @brief Resumes the typing test after a pause.
 */
void resume_typing_test();

/**
 * @brief Handles typing of a character.
 * @param c The character that was typed.
 */
void type_char(char c);

/**
 * @brief Moves to the next line in the typing test.
 */
void go_to_next_line();

/**
 * @brief Calculates final typing statistics (WPM, accuracy, etc.).
 */
void calculate_stats();

/**
 * @brief Handles countdown timer tick events.
 */
void countdown_tick();

#endif
