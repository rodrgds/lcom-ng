/**
 * @file scores.h
 * @brief Header file for managing game scores.
 */
#ifndef _SCORES_H
#define _SCORES_H

#include <lcom/lcf.h>
#include "../controller/rtc/rtc.h"

#define MAX_NAME_LENGTH 6
#define MAX_SCORES 9
#define MAX_SCORES_PER_LEVEL 3

/**
 * Structure to represent a score entry.
 */
typedef struct {
  char player_name[MAX_NAME_LENGTH];
  int level;
  int score;
  int year;
  int month;
  int day;
} ScoreEntry;

/**
 * Array to store the scoreboard entries.
 */
ScoreEntry scoreboard[MAX_SCORES];

/**
 * Load a new score entry into the scoreboard.
 * @param player_name The name of the player.
 * @param level The level at which the score was achieved.
 * @param score The score achieved.
 * @param year The year the score was achieved.
 * @param month The month the score was achieved.
 * @param day The day the score was achieved.
 */
void load_scoreboard(const char *player_name, int level, int score, int year, int month, int day);

/**
 * Sort the scoreboard entries by level and score.
 */
void sort_scores();

/**
 * Save the scoreboard entries to a file.
 * @param filename The name of the file to save to.
 */
void save_scores(const char *filename);

/**
 * Load scoreboard entries from a file.
 * @param filename The name of the file to load from.
 */
void load_scores(const char *filename);

/**
 * Verify if a player's score qualifies for the scoreboard.
 * @param level The level at which the score was achieved.
 * @param score The score achieved.
 * @return True if the score qualifies for the scoreboard, false otherwise.
 */
bool verify_if_go_to_scoreboard(int level, int score);

/**
 * Insert a new score entry into the scoreboard.
 * @param player_name The name of the player.
 * @param level The level at which the score was achieved.
 * @param score The score achieved.
 */
void insert_score(const char *player_name, int level, int score);

#endif
