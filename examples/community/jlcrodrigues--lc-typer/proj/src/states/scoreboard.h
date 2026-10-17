#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <lcom/lcf.h>
#include "driver.h"
#include "proj.h"
#include "sprite.h"
#include "rtc.h"
#include <stdio.h>
#include <stdlib.h>

#define SCORE_FILE "/home/lcom/labs/proj/src/assets/scores/scores.txt"
#define N_SCORES 5
#define LINE_SIZE 30

/** @defgroup state game_over
 * @{
 *
 * Functions related to the score board state.
 */


/**
 * @brief Draw score board state
 * 
 */
void score_board_draw();

/**
 * @brief Handle event.
 * 
 * @param event Event struct.
 */
void score_board_handle_event(Event event);

/**
 * @brief Updates scoreboard if needed.
 * 
 * @param game Fame struct passed by reference.
 */
int score_board_update(Game* game);

/**
 * @brief Load the score board.
 * 
 */
void get_score_board();

/**
 * @brief Handles events and draws leaderboard. 
 * 
 * @param event Last event that occurred.
 */
void score_board_step(Event event);

/**@}*/

#endif
