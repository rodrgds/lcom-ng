/**
 * @file scoreboard.h
 * @brief Score tracking and display.
 */

#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"

#define MAX_SCORES 10

/**
 * @brief Represents a single highscore record.
 */
typedef struct {
    char name[13];      /**< @brief Player's input name. */
    int score;          /**< @brief Player's score for the level. */
    int suns_spent;     /**< @brief Total suns used. */
    int level;          /**< @brief The completed level. */
    rtc_date date;      /**< @brief Timestamp of completion. */
    bool valid;         /**< @brief Indicates if the entry holds valid initialized data. */
} score_entry_t;

/**
 * @brief Initializes the scoreboard matrix.
 */
void scoreboard_init();

/**
 * @brief Adds a score to the internal record after completing a level.
 * * @param name Player's name.
 * @param level The level completed.
 * @param score The achieved score.
 * @param suns_spent The total suns spent.
 * @param date Pointer to the current RTC timestamp.
 */
void scoreboard_add(const char *name, int level, int score, int suns_spent, rtc_date *date);

/**
 * @brief Renders the visual scoreboard UI.
 */
void scoreboard_render();

/**
 * @brief Gets the best score entry for a specific level.
 * * @param level The target level.
 * @return Pointer to the score_entry_t, or NULL if no valid entry exists.
 */
score_entry_t* scoreboard_get(int level);

#endif