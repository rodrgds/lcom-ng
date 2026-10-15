#ifndef MM_WINNER_H
#define MM_WINNER_H

#include <lcom/lcf.h>

#include "spriteLoader.h"
#include "cursor.h"
#include "key_pressed.h"
#include "button.h"

/**
 * @file mm_winner.h
 *  @brief Multiplayer mode winner state management
 * 
 * This module provides functions to create, manage, and draw the winner state in the multiplayer mode.
 * It handles the display of the winner screen, including the background, winner message, and buttons to play again or return to the menu.
 */

struct mm_winner_imp;
typedef struct mm_winner_imp MmWinner;


/**
 * @brief Creates a new MmWinner object
 * 
 * @param loader Pointer to the SpriteLoader object
 * @param winner The winner of the game (0 - No winner, 1 - Current player wins, 2 - Other player wins)
 * @return Pointer to the created MmWinner object, or NULL if the creation failed
 */
MmWinner *create_mm_winner(SpriteLoader *loader, int winner);

/**
 * @brief Destroys a MmWinner object, freeing all allocated memory
 * 
 * @param mm_winner Pointer to the MmWinner object to be destroyed
 */
void destroy_mm_winner(MmWinner *mm_winner);

/**
 * @brief Draws the MmWinner screen
 * 
 * @param mm_winner Pointer to the MmWinner object to be drawn
 */
void draw_mm_winner(MmWinner *mm_winner);

/**
 * @brief Processes the input from the cursor in the MmWinner state
 * 
 * @param mm_winner Pointer to the MmWinner object
 * @param c Pointer to the Cursor object
 * @return 1 if the game should exit, 0 otherwise, 2 to play again
 */
int process_mm_winner_mouse(MmWinner *mm_winner, Cursor *c);

/**
 * @brief Processes the input from the keyboard in the MmWinner state
 * 
 * @param mm_winner Pointer to the MmWinner object
 * @param kp Pointer to the KeyPressed object containing the state of the keys
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_mm_winner_kbd(MmWinner *mm_winner, KeyPressed *kp);

#endif // MM_WINNER_H
