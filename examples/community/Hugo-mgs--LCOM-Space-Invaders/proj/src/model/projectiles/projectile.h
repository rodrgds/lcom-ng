#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"
#include "../alien/alien.h"
#include "../barrier/barrier.h"
#include "../player/player.h"

typedef struct {
    int x;
    int y;
    char type; // 'A' for alien, 'P' for alien
    //int velocity; 
    Sprite *sprite;
    bool active;
} Projectile;

/**
 * Creates a new projectile.
 * 
 * @param x The initial x-coordinate of the projectile.
 * @param y The initial y-coordinate of the projectile.
 * @param type The type of the projectile ('P' for player, 'A' for alien, etc.).
 * @param sprite Pointer to the sprite associated with the projectile.
 * @return Pointer to the newly created Projectile, or NULL if allocation fails.
 */
Projectile *createProjectile(int x, int y, char type, Sprite *sprite);

/**
 * Destroys a projectile and frees its memory.
 * 
 * @param projectile Pointer to the Projectile to be destroyed.
 */
void destroyProjectile(Projectile *projectile);
 
/**
 * Draws the projectile on the screen.
 * 
 * @param projectile Pointer to the Projectile to be drawn.
 */
void drawProjectile(Projectile *projectile);
 
/**
 * Moves the projectile according to its type and updates its state.
 * 
 * @param projectile Pointer to the Projectile to be moved.
 */
void projectileMove(Projectile *projectile);
 
/**
 * Checks for collision between a projectile and an alien.
 * 
 * @param projectile Pointer to the Projectile.
 * @param alien Pointer to the Alien.
 * @return true if a collision is detected, false otherwise.
 */
bool checkCollisionAlien(Projectile *projectile, Alien *alien);
 
/**
 * Checks for collision between a projectile and a barrier.
 * 
 * @param projectile Pointer to the Projectile.
 * @param barrier Pointer to the Barrier.
 * @return true if a collision is detected, false otherwise.
 */
bool checkCollisionBarrier(Projectile *projectile, Barrier *barrier);
 
/**
 * Checks for collision between a projectile and the player.
 * 
 * @param projectile Pointer to the Projectile.
 * @param player Pointer to the Player.
 * @return true if a collision is detected, false otherwise.
 */
bool checkCollisionPlayer(Projectile *projectile, Player *player);
