/**
 * @file results.h
 * @brief Persistent best-test storage and display.
 */
#ifndef _TECLAS_RESULTS_H_
#define _TECLAS_RESULTS_H_

#include "font.h"

/**
 * @brief Maximum number of best test entries stored.
 */
#define B_T_SIZE 5

/**
 * @brief Background color used for value cells.
 */
#define VALUE_BG 0xFFFFFF

/**
 * @brief Background color used for empty cells.
 */
#define EMPTY_BG 0xFFFFFF

/**
 * @brief A saved typing test result.
 */
typedef struct {
    char name[16];
    int score;
    int wpm;
    int correct;
    int mistakes;
    int sec_duration;
    int day;
    int month;
    int year;
} Test;

/**
 * @brief Load the saved best tests from the file.
 *
 * @param best_tests Array to fill with loaded entries.
 * @param count Pointer to receive the number of loaded entries.
 */
void load_best_tests(Test best_tests[B_T_SIZE], int *count);

/**
 * @brief Insert a new test result into the best-tests list.
 *
 * @param best_tests Array of current best test records.
 * @param count Pointer to the current number of valid entries.
 * @param new_test New test entry to insert.
 */
void update_best_tests(Test best_tests[B_T_SIZE], int *count, Test new_test);

/**
 * @brief Save best test results back to storage.
 *
 * @param best_tests Array of best test records to save.
 * @param count Number of valid entries in the array.
 */
void save_best_tests(Test best_tests[B_T_SIZE], int count);

/**
 * @brief Draw the best test entries on screen.
 *
 * @param best_tests Array of best test records.
 * @param count Number of entries to display.
 * @param x Starting x position for drawing.
 * @param y Starting y position for drawing.
 */
void display_best_tests(Test best_tests[B_T_SIZE], int count, uint16_t x, uint16_t y);

#endif // _TECLAS_RESULTS_H_
