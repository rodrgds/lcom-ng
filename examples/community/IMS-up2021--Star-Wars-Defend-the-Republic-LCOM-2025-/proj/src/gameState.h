/**
 * @file gameState.h
 * @brief Declares the game state management functionality and related data types.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>
#include <lcom/lcf.h>

typedef enum {
    MAIN_MENU,
    PLAYING,
    INSTRUCTIONS,
    EXIT
} gameState;

typedef struct Position {
  uint16_t x;
  uint16_t y;
} Position;


extern Position mouse_pos;

/**
 * @brief The main game loop.
 *
 * Initializes game components and then enters a loop to process hardware interrupts
 * (timer, keyboard, mouse). It calls appropriate handlers for these events and manages
 * screen updates with double buffering. The loop continues until the game state is set to EXIT.
 */
void gameLoop(void);

extern gameState state;

void setGameState(gameState newState);

#endif 

