/**
 * @file score.h
 * @brief Header file for score management in the game.
 */

#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

/**
 * @brief Struct to represent the score and lives of the player.
 */
typedef struct {
  int score;
  int lives;
} Score;


/**
 * @brief Initializes the score information.
 * @param score_info Pointer to the Score struct to initialize.
 * @param initial_lives The initial number of lives for the player.
 */
void init_score(Score* score_info, int initial_lives);

/**
 * @brief Updates the score by adding points.
 * @param score_info Pointer to the Score struct.
 * @param points Number of points to add to the score.
 */
void update_score(Score* score_info, int points);

/**
 * @brief Decrements the number of lives by one.
 * @param score_info Pointer to the Score struct.
 */
void decrement_lives(Score* score_info);

/**
 * @brief Draws the current score on the screen.
 * @param score_info Pointer to the Score struct.
 */
void draw_score(Score* score_info);

/**
 * @brief Draws the remaining lives on the screen.
 * @param score_info Pointer to the Score struct.
 */
void draw_lives(Score* score_info);

/**
 * @brief Draws the final score on the screen.
 * @param score_info Pointer to the Score struct.
 */
void draw_final_score(Score* score_info);

/**
 * @brief Draws the best score on the screen.
 * @param score_info Pointer to the Score struct.
 */
void draw_best_score(Score* score_info);


/**
 * @brief Draws the time remaining on the screen.
 * @param time_remaining The time remaining to be displayed.
 */
void draw_time_remaining(int time_remaining);


/**
 * @brief Global variable indicating the score value of each alien kill.
 */
extern int alien_score_value;  //how many points each alien kill is worth

/**
 * @brief Global variable to hold the current game score.
 */
extern Score game_score;

/**
 * @brief Global variable to hold the best score achieved.
 */
extern int best_score; 

#endif // SCORE_H
