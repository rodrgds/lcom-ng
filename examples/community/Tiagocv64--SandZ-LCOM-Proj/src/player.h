#pragma once
#include <lcom/lcf.h>
#include "weapon.h"

/** @defgroup player player
 * @{
 * Functions for manipulating the player inside the game
 */

/**
 * @brief      Represents the player object
 */
struct Player_state{
  bool leftPressed;				/**< flag marking if moving left key is pressed */
  bool rightPressed;			/**< flag marking if moving right key is pressed */
  bool upPressed;				/**< flag marking if moving up key is pressed */
  bool downPressed;				/**< flag marking if moving down key is pressed */
  int directionY; 				/**< representing the y direction */
  int directionX; 				/**< representing the x direction */
  unsigned int lives;			/**< current lives */
  bool godMode;					/**< flag marking if player is able to take damage */
  struct Weapon weaponEquiped;	/**< current equiped weapon */
};

/**
 * @brief      Detects player collision at sepecified coordinates with obstacles.
 *
 * @param[in]  x  { x coordinate }
 * @param[in]  y  { y coordinate }
 *
 * @return     { bool denoting if a collision occured }
 */
bool detect_player_collision(int x, int y);

/**
 * @brief      Initialize player structure
 */
void init_player();

/**
 * @brief      Destroy player structure and free memory used by it
 */
void destroy_player();

/**
 * @brief      Draw player sprite to screen
 */
void draw_player();

/**
 * @brief      Change player state based on keyboard makecode
 *
 * @param[in]  key_code  { keyboard makecode }
 */
void change_player_state(uint8_t key_code);

/**
 * @brief      Move player according to its direction
 */
void move_player();

/**
 * @brief      Reload currently equipped weapon
 */
void reload_player();

/**
 * @brief      Fire with currently equipped weapon 
 */
void fire_player();

/**
 * @brief      Change currently equipped weapon acording to keyboard makecode
 *
 * @param[in]  key_code  { keyboard makecode }
 */
void change_weapon(uint8_t key_code);

/**
 * @brief      Update player's points
 */
void update_points();

/**
 * @brief      Update player
 */
void update_player();

/**
 * @brief      Pause current player state
 */
void pause_player();

/**
 * @brief      Resume previously paused player state
 */
void resume_player();
