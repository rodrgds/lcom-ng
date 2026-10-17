#ifndef GAME_H
#define GAME_H

/** @defgroup state game
 * @{
 *
 * Functions related to the game state.
 */

/**
 * @brief Represents a game. A game consists of a sentence, 
 * a position in the sentence and a value for the time elapsed.
 * 
 */
typedef struct Game {
  /** @brief Player's current position in the game sentence. **/
  int player_position;
  /** @brief The sentence to be completed. **/
  char* text;
  /** @brief The size of the target text. **/
  int text_size;
  /** @brief Amount, in seconds, of time elapsed since the first letter hit. **/
  int time_elapsed;
  /** @brief Amount of misses after the last hit. **/
  int typo_offset;
  /** @brief Overall amount of miss clicks. **/
  int typo_count;
   /** @brief Final wpm result. **/
  int wpm;
} Game;

#include <lcom/lcf.h>
#include "driver.h"
#include "proj.h"
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>

#define LINESIZE 2000
#define NUMBER_OF_STRINGS 14
#define NUM_LINES 3
#define TEXT_Y_START 100
#define TEXT_Y_MAX (TEXT_Y_START + NUM_LINES * LINE_HEIGHT)

/**
 * @brief Initialize a game struct.
 * 
 * @param game Already created game struct passed by reference.
 */
void game_create(Game* game);

/**
 * @brief Restart a game (keep the text).
 * 
 * @param game Already created game struct passed by reference.
 */
void game_restart(Game* game);

/**
 * @brief Draw a game on the screen.
 * Drawing the game means drawing every necessary element for the 
 * game state.
 * 
 * @param game Pointer to the game struct.
 */
void game_draw(Game* game);

/**
 * @brief Handle the latest event. Updates the game values according to it.
 * 
 * @param game Game struct passed by reference.
 * @param event Latest event that occurred.
 */
void game_handle_event(Game* game, Event* event);

/**
 * @brief Draw the game and handle the latest event.
 * This function calls game_draw and game_handle_event
 * 
 * @param game Game struct passed by reference.
 * @param event Latest event that occurred.
 */
void game_step(Game* game, Event* event);

/**
 * @brief Draw the game main text.
 * 
 * @param game Game struct passed by reference.
 */
void draw_text(Game* game);

/**
 * @brief Draw the player's live speed.
 * 
 * @param game Game struct passed by reference.
 */
void draw_wpm(Game* game);

/**
 * @brief Creates the file from determined path and selects a random phrase from there.
 *
 * @param infile file path passed by reference.
 *
 * @return phrase selected.
 */
char* phrase_select(char* infile);

/**@}*/

#endif
