#ifndef PLAYER_H_
#define PLAYER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "game/sprite/sprite.h"

/**
* @brief Represents a player in the game.
*/
typedef struct
{
    int life;
    int damage;
    int x;
    int y;
    Sprite *sprite;
} player;

/**
 * @brief Creates a new player instance.
 * 
 * @param life The initial life points of the player.
 * @param damage The initial damage points of the player.
 * @param x The initial x-coordinate of the player on the game map.
 * @param y The initial y-coordinate of the player on the game map.
 * @param sp A pointer to the sprite associated with the player.
 * @return A pointer to the newly created player instance, or NULL if memory allocation fails.
 */
player *(createPlayer)(int life,int damage,int x,int y, Sprite *sp);



#endif

