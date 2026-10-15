#ifndef MM_GAME_H
#define MM_GAME_H

#include <lcom/lcf.h>
#include "spriteLoader.h"
#include "cursor.h"
#include "key_pressed.h"
#include "serial_port.h"
#include "player.h"
#include "bomb.h"
#include "wall.h"
#include "explosion.h"
#include "bomb_options.h"
#include "sprite.h"
#include "animatedSprite.h"
#include "button.h"

/**
 * @file mm_game.h
 * @brief Multiplayer mode game state management
 * 
 * This module provides functions to create, manage, and draw the multiplayer game state.
 * It handles the players, walls, bombs, explosions, and the game timer.
 */

struct mm_game_imp;
typedef struct mm_game_imp MmGame;

/**
 * @brief Creates a new MmGame object
 * 
 * @param loader Pointer to the SpriteLoader object
 * @param player_number The player number (1 or 2)
 * @return Pointer to the created MmGame object, or NULL if the creation failed
 */
MmGame *create_mm_game(SpriteLoader *loader, uint8_t player_number);

/**
 * @brief Destroys a MmGame object, freeing all allocated memory
 * 
 * @param mm_game Pointer to the MmGame object to be destroyed
 */
void destroy_mm_game(MmGame *mm_game);

/**
 * @brief Draws the MmGame object on the screen
 * 
 * @param mm_game Pointer to the MmGame object to be drawn
 */
void draw_mm_game(MmGame *mm_game);

/**
 * @brief Processes the input from the mouse in the MmGame state
 * 
 * @param mm_game Pointer to the MmGame object
 * @param c Pointer to the Cursor object
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_mm_game_mouse(MmGame *mm_game, Cursor *c);

/**
 * @brief Processes the input from the keyboard in the MmGame state
 * 
 * @param mm_game Pointer to the MmGame object
 * @param kp Pointer to the KeyPressed object containing the state of the keys
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_mm_game_kbd(MmGame *mm_game, KeyPressed *kp);

/**
 * @brief Processes the game timer in the MmGame state
 * 
 * @param mm_game Pointer to the MmGame object
 * @return 0 if the game should continue, 1 to go back to the menu, 2 to go to the winner state
 */
int process_mm_game_timer(MmGame *mm_game);

/**
 * @brief Processes the input from the serial port in the MmGame state
 * 
 * @param mm_game Pointer to the MmGame object
 * @param byte Byte received from the serial port
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_mm_game_sp(MmGame *mm_game, uint8_t byte);

/**
 * @brief Gets the winner of the game
 * 
 * @param mm_game Pointer to the MmGame object
 * @return 0 if no winner, 1 if the current player wins, 2 if the other player wins
 */
int get_winner(MmGame *mm_game);

#endif // MM_GAME_H
