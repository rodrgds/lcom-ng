#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "sprite.h"
#include "animatedSprite.h"

/**
 * @file player.h
 * @brief Player management for the game
 * 
 * This module provides functions to create, manage, and draw the player in the game.
 * It handles the player's position, speed, animations, and movement.
 * 
 */
struct player_imp;
typedef struct player_imp Player;

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  STANDING
} Direction;

/**
 * @brief Returns the X coordinate of the player
 * 
 * @param p Pointer to the player
 * @return The X coordinate of the player
 */
uint16_t get_player_Xpos(Player *p);

/**
 * @brief Returns the Y coordinate of the player
 * 
 * @param p Pointer to the player
 * @return The Y coordinate of the player
 */
uint16_t get_player_Ypos(Player *p);

/**
 * @brief Returns the speed of the player
 * 
 * @param p Pointer to the player
 * @return The speed of the player
 */
uint8_t get_player_speed(Player *p);

/**
 * @brief Returns the sprite of the player standing
 * 
 * @param p Pointer to the player
 * @return The sprite of the player standing
 */
Sprite *get_player_standing(Player *p);

/**
 * @brief Creates a new player with the specified sprite and animations
 * 
 * @param x The x-coordinate of the player
 * @param y The y-coordinate of the player
 * @param standing The sprite for the standing animation
 * @param left_movement The animated sprite for the left movement
 * @param right_movement The animated sprite for the right movement
 * @param up_movement The animated sprite for the up movement
 * @param down_movement The animated sprite for the down movement
 * @return Pointer to the created player, or NULL if the pixmap is invalid
 */
Player *create_player(uint16_t x, uint16_t y, AnimSprite *left_movement, AnimSprite *right_movement, AnimSprite *up_movement, AnimSprite *down_movement, Sprite *standing);

/**
 * @brief Frees the memory allocated for the player
 * 
 * @param p Pointer to the player to be destroyed
 */
void destroy_player(Player *p);

/**
 * @brief Draws the player on the screen at the specified coordinates, acording to the current direction
 * 
 * @param p Pointer to the player to be drawn
 * @param x_initial_grid The initial X coordinate of the grid
 * @param y_initial_grid The initial Y coordinate of the grid
 */
void draw_player(Player *p, uint16_t x_initial_grid, uint16_t y_initial_grid);

/**
 * @brief Moves the player up
 * 
 * @param p Pointer to the player to be updated
 */
void player_move_up(Player *p);

/**
 * @brief Moves the player down
 * 
 * @param p Pointer to the player to be updated
 */
void player_move_down(Player *p);
/**
 * @brief Moves the player left
 * 
 * @param p Pointer to the player to be updated
 */
void player_move_left(Player *p);
/**
 * @brief Moves the player right
 * 
 * @param p Pointer to the player to be updated
 */
void player_move_right(Player *p);

/**
 * @brief Updates the direction of the player if it has changed
 * 
 * @param p Pointer to the player to be updated
 */
void player_stand(Player *p);

#endif /* PLAYER_H */

