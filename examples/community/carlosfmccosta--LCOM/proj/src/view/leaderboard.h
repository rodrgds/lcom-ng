/**
 * @file leaderboard.h
 * @brief Interface for leaderboard management.
 *
 * This header provides the structure and functions for reading, updating, saving,
 * and freeing leaderboard entries, as well as handling player scores and names.
 */
#ifndef _LEADERBORD_H_
#define _LEADERBORD_H_

#include "model/sprite.h"
#include "assets/letters.h"
#include "write_name.h"

/** @def The max number of player's scores that can go to the leaderboard */

#define MAX_TOP 10

/**
 * @brief Structure representing a leaderboard entry
 */
typedef struct {
    int points;    /**< Player's points */
    char* name;    /**< Player's name */
} leaderboard_t;

/**
 * @brief Global array containing leaderboard entries
 */
leaderboard_t entries[MAX_TOP+1];

/**
 * @brief Reads leaderboard information from a file into the entries array
 * 
 * @param filename Path to the file storing the leaderboard
 * @param entries Array to store the read entries
 * @return Returns 0 on success, 1 if file opening fails
 */
int get_info_lb(const char* filename, leaderboard_t* entries);

/**
 * @brief Updates the leaderboard with a new score and player name, sorts and trims the list
 * 
 * @param score Player's score
 * @param name Player's name
 */
void update_lb(int score, const char* name);

/**
 * @brief Saves the current leaderboard entries to the leaderboard's file
 * 
 * @param filename Path to the file to save the leaderboard
 */
void save_lb(const char* filename);

/**
 * @brief Frees memory allocated for player names in the leaderboard
 */
void free_lb();

/**
 * @brief "Super" leaderboard function. It loads, updates, and saves the leaderboard 
 * 
 * @param score Player's score
 * @param name Player's name
 */
void main_leaderboard(int score, const char* name);

#endif
