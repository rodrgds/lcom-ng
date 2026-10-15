/**
 * @file game_over
 * @brief This file has the implementation of the functions related to the game over menu.
 */

#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

#include "model/cursor/cursor_input.h"
#include "model/model_utils.h"

/**
 * @brief Struct that represents the GameOver state.
 *
 * This struct contains the title, and two buttons for Play and Exit.
 */
typedef struct {

  char* result;
  char* winner; 

  Button playButton;
  Button exitButton;

} GameOver;

extern GameOver* gameOver;

/**
 * @brief Creates a new game over state.
 *
 * This function initializes a new game over state with a title and two buttons for Play and Exit.
 * The positions of the title and buttons are set, and their sprites are linked.
 * 
 * @return void.
 */
void create_game_over();

/**
 * @brief Checks if the play button has been clicked.
 * 
 * @param cursor CursorInput struct that contains the current cursor input.
 *
 * This function checks if the current cursor position is within the bounds of the play button.
 * 
 * @return true if the play button was clicked, false otherwise.
 */
bool game_over_clicked_play(CursorInput* cursor);

/**
 * @brief Checks if the exit button has been clicked.
 * 
 * @param cursor CursorInput struct that contains the current cursor input.
 *
 * This function checks if the current cursor position is within the bounds of the exit button.
 * 
 * @return true if the exit button was clicked, false otherwise.
 */
bool game_over_clicked_exit(CursorInput* cursor);

/**
 * @brief Destroys the game over state.
 *
 * This function frees the memory allocated for the game over state and its associated elements.
 * 
 * @return void.
 */
void destroy_game_over();

#endif
