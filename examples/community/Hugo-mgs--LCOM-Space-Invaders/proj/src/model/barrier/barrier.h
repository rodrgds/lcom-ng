#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"

typedef struct {
    int x;
    int y;
    int lives;
    Sprite *sprite;
} Barrier;

/**
 * Creates a new Barrier object at the specified coordinates.
 * 
 * @param x The x-coordinate of the barrier.
 * @param y The y-coordinate of the barrier.
 * @return Pointer to the newly created Barrier, or NULL if memory allocation fails.
 */
Barrier *createBarrier(int x, int y);
 
/**
 * Destroys a Barrier object and frees its associated memory.
 * 
 * @param barrier Pointer to the Barrier to be destroyed.
 */
void destroyBarrier(Barrier *barrier);
 
/**
 * Draws the specified Barrier on the screen.
 * 
 * @param barrier Pointer to the Barrier to be drawn.
 */
void drawBarrier(Barrier *barrier);
 
/**
 * Applies damage to the Barrier, updating its state and sprite.
 * 
 * @param barrier Pointer to the Barrier to be damaged.
 * @return 1 if the barrier's lives reach zero (destroyed), 0 otherwise.
 */
int damageBarrier(Barrier *barrier);
