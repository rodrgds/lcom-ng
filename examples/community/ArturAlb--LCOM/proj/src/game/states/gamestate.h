/**
 * @file gamestate.h
 *
 */

#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <stdint.h>
#include <math.h>

#include "../models/list.h"
#include "../models/object.h"
#include "../models/bullet.h"

#include "../../images/Tank_1_Down.xpm"
#include "../../images/Tank_2_Down.xpm"
#include "../../images/Cross.xpm"
#include "../../images/Bullet.xpm"

/**
 * @brief struct that handles the state of the game
 * 
 * @param player object that stores the player postion and sprites
 * 
 * @param player_2 object that stores the 2nd player postion and sprites
 * 
 * @param mouse object with information ragarding the mouses position, buttons clicked and sprite
 * 
 * @param bullets list of all the bullets
 * 
 * @param walls list of all the walls
 * 
 * @param bullet_cooldown_2 bullet cooldown for the 2nd player
 * 
 * @param winner tells who is the winner, is 0 while the game is going on
 * 
 */
typedef struct {
    Object *player;
    Object *player_2;
    Object *mouse;
    List *bullets;
    List *walls;
    int bullet_cooldown_2;
    int winner;
} GameState;


/**
 * @brief Create a Game object
 * 
 * @return GameState* 
 */
GameState* createGame();
/**
 * @brief Create a Bullet object
 * 
 * @param state gamestate
 * @param xOrigin x position of the firing player
 * @param yOrigin y position of the firing player
 * @param xDestination x position of the mouse
 * @param yDestination y position of the mouse
 * @param owner player whos firing the bullet
 */
void createBullet(GameState *state, uint16_t xOrigin, uint16_t yOrigin, uint16_t xDestination, uint16_t yDestination, Object *owner);
/**
 * @brief creates and displays the walls of the arena
 * 
 * @param state gamestate
 * @param x starting x for the display
 * @param y starting y for the display
 * @param width width of the wall
 * @param height height of the wall
 */
void insertWall(GameState *state, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
/**
 * @brief handles the bullet movement and collisions between them the walls and the players
 * 
 * @param state gamestate
 */
void gameStep(GameState *state);
/**
 * @brief handles data sent from the KBC
 * 
 * @param obj gamestate
 * @param data data sent from the keyboard
 */
void process_scancode(GameState* obj, kbd_data_t* data);

#endif // _LCOM_GAME_STATE_H_
