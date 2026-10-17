#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "game/layout.h"
#include "game/model/cell_background.h"

/**
 * @brief Handles the status of a player word submission
 * 
 */
typedef enum {
  TOO_SHORT,
  NOT_IN_WORDLIST,
  SUBMITTED,
  FINISHED
} local_player_submission_status;

/**
 * @brief Handles a player instance
 * 
 */
typedef struct {
  uint8_t x, y; // The position in the board where the player is editing
  size_t pos;
  
  char board[COLS * ROWS + 2]; // The player's guesses and current word
  cell_background background[COLS * ROWS]; // The player's guesses backgrounds
} local_player;

local_player *(local_player_get)();

void (local_player_reset)();

char (local_player_read)();
char (local_player_read_before)();
char (local_player_read_after)();
char (local_player_read_last_of_row)();

void (local_player_write)(char ch);

void (local_player_goto)(uint8_t x, uint8_t y);
void (local_player_forward)();
void (local_player_backward)();
void (local_player_downward)();
