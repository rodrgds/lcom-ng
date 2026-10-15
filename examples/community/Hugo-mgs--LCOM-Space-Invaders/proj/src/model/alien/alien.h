#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"

typedef struct {
    int x;
    int y;
    int lives;
    Sprite *sprite;
} Alien;

/**
 * Creates a new Alien object.
 * 
 * @param x The initial x-coordinate of the alien.
 * @param y The initial y-coordinate of the alien.
 * @param lives The number of lives the alien has.
 * @param sprite Pointer to the sprite associated with the alien.
 * @return Pointer to the newly created Alien object, or NULL if memory allocation fails.
 */
Alien *createAlien(int x, int y, int lives, Sprite *sprite);
 
/**
 * Destroys an Alien object and frees its memory.
 * 
 * @param alien Pointer to the Alien object to be destroyed.
 */
void destroyAlien(Alien *alien);
 
/**
 * Draws the Alien on the screen at its current position.
 * 
 * @param alien Pointer to the Alien object to be drawn.
 */
void drawAlien(Alien *alien);
 
/**
 * Moves the Alien in the specified direction.
 * 
 * @param alien Pointer to the Alien object to be moved.
 * @param direction The direction to move the alien: 0 - left, 1 - right, 2 - down.
 */
void alienMove(Alien *alien, int direction);
 
/**
 * Damages the Alien by reducing its lives by one.
 * 
 * @param alien Pointer to the Alien object to be damaged.
 * @return 1 if the alien's lives reach zero or below, 0 otherwise.
 */
int damageAlien(Alien *alien);
