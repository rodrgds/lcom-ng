#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"

typedef struct {
    int x;
    int y;
    int lives;
    int score;
    Sprite *sprite;
} Player;

/**
 * Creates a new Player object.
 *
 * @param x Initial x-coordinate of the player.
 * @param y Initial y-coordinate of the player.
 * @param lives Initial number of lives for the player.
 * @param score Initial score for the player.
 * @param sprite Pointer to the Sprite associated with the player.
 * @return Pointer to the newly created Player, or NULL if memory allocation fails.
 */
Player *createPlayer(int x, int y, int lives, int score, Sprite *sprite);

/**
 * Destroys a Player object.
 *
 * @param player Pointer to the Player to be destroyed.
 */
void destroyPlayer(Player *player);

/**
 * Draws the player on the screen.
 *
 * @param player Pointer to the Player to be drawn.
 */
void drawPlayer(Player *player);

/**
 * Moves the player horizontally.
 *
 * @param player Pointer to the Player to be moved.
 * @param delta_x Amount to move the player along the x-axis.
 */
void playerMove(Player *player, int delta_x);

/**
 * Damages the player by reducing its lives.
 *
 * @param player Pointer to the Player to be damaged.
 * @return 1 if the player has no lives left, 0 otherwise.
 */
int damagePlayer(Player *player);
