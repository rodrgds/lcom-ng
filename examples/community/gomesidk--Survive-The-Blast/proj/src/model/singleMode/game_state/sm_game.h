#ifndef _SM_GAME_H_
#define _SM_GAME_H_

#include <lcom/lcf.h>

#include "player.h"
#include "bomb.h"
#include "wall.h"
#include "spriteLoader.h"
#include "animatedSprite.h"
#include "sprite.h"
#include "cursor.h"
#include "explosion.h"
#include "bomb_options.h"
#include "key_pressed.h"
#include "score.h"
#include "button.h"

/**
 * @file sm_game.h
 * @brief Single Mode game state management
 * 
 * This module provides functions to create, destroy, draw, and process input for the Single Mode game state.
 * It manages the player, walls, bombs, explosions, and the game timer.
 */

struct sm_game_imp;
typedef struct sm_game_imp SmGame;

/**
 * @brief Creates a new SingleMode object, which contains a player and a grid of walls
 * 
 * @param loader Pointer to the SpriteLoader object
 * @return Pointer to the created SingleMode object, or NULL if the creation failed
 */
SmGame *create_sm_game(SpriteLoader *loader, Score* score);

/**
 * @brief Destroys a SingleMode object, freeing all allocated memory
 * 
 * @param sm Pointer to the SingleMode object to be destroyed
 */
void destroy_sm_game(SmGame *smg);

/**
 * @brief Draws the SingleMode object on the screen
 * 
 * @param sm Pointer to the SingleMode object to be drawn
 */
void draw_sm_game(SmGame *smg);

/**
 * @brief Processes the input from the keyboard
 * 
 * @param sm Pointer to the SingleMode object
 * @param key Pointer to the KeyPressed object containing the state of the keys
 * @return 0 if the game should continue, 1 go back to the menu
 */
int process_sm_game_kbd(SmGame *smg, KeyPressed *key);

/**
 * @brief Processes the input from the mouse
 * 
 * @param sm Pointer to the SingleMode object
 * @param c Pointer to the Cursor object
 * @return 0 if the game should continue, 1 go back to the menu
 */
int process_sm_game_mouse(SmGame *smg, Cursor *c);

/**
 * @brief Processes the game timer, checking for bomb spawning and explosions
 * 
 * @param sm Pointer to the SingleMode object
 * @return 0 if the game should continue, 1 go back to the menu, 2 go to died state
 */
int process_sm_game_timer(SmGame *smg);


#endif 

