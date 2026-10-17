#ifndef _PLAYER_H
#define _PLAYER_H

#include "Entity.h"

/** @defgroup Player Player
 * @{
 */

typedef struct direction_t {
  bool right;
  bool left;
  bool up;
  bool down;
} Direction;

typedef struct player_t {
  int hp;
  int dmg;
  unsigned fc;
  bool invulnerable;
  Entity e;
  Direction dir;
} Player;

/*
@brief initializes Player struct

@param h hp of the player

@param d damage of the player

@return Player struct
*/

Player player(int h, int d);

/*
@brief draws the player

@param p Player struct to draw
*/

void drawPlayer(Player *p);

/*
@brief moves the player left

@param p Player struct to move
*/

void movePlayerLeft(Player * p);

/*
@brief moves the player right

@param p Player struct to move
*/

void movePlayerRight(Player * p);

/*
@brief moves the player up

@param p Player struct to move
*/

void movePlayerUp(Player * p);

/*
@brief moves the player down

@param Player struct to move
*/

void movePlayerDown(Player * p);

/*
@brief moves the player

@param p Player struct to move
*/

void movePlayer(Player * p);

/*
@brief checks if the player is alive

@param p Player struct to move

@return true if it is alive and false if it isnt
*/

bool alive(Player const * p);

#endif /*_PLAYER_H*/
