#pragma once

#include <lcom/lcf.h>

/**
 * @brief Handles the overall game
 * 
 */
typedef struct {
  char* target_word;
  unsigned long ellapsed_time;
  bool running;
} game;

/**
 * @brief Gets a game instance 
 * 
 */
game *(game_get)();

/**
 * @brief Resets the game 
 * 
 */
void (game_reset)();
