/**
 * @file collision.h
 * @brief Collision detection functions.
 */
#ifndef COLLISION_H
#define COLLISION_H

#include "game.h"

/**
 * @brief Checks if two rectangles collide.
 * @param ax X coordinate of rectangle A.
 * @param ay Y coordinate of rectangle A.
 * @param aw Width of rectangle A.
 * @param ah Height of rectangle A.
 * @param bx X coordinate of rectangle B.
 * @param by Y coordinate of rectangle B.
 * @param bw Width of rectangle B.
 * @param bh Height of rectangle B.
 * @return True if they collide, false otherwise.
 */
bool rects_collide(int ax, int ay, int aw, int ah,
                   int bx, int by, int bw, int bh);

/**
 * @brief Checks and handles all game collisions.
 * @param game Pointer to the Game state.
 */                
void check_collisions(Game *game);

#endif
