#pragma once
#include <lcom/lcf.h>
#include <stdint.h>

/** @defgroup life life
 * @{
 * Functions for manipulating life objects
 */

/**
 * @brief      Represents one live object.
 */
struct Lives{
  int x;				/**< x coordinate on screen */
  int y; 				/**< y coordinate on screen */
  bool pickupable;		/**< flag marking if live is active */
};

/**
 * @brief      Initialize life structure.
 */
void init_lives();

/**
 * @brief      Destroy lives structure and free memory used by it
 */
void destroy_lives();

/**
 * @brief      Detects live collision at sepecified coordinates with obstacles.
 *
 * @param[in]  x  { x coordinate }
 * @param[in]  y  { y coordinate }
 *
 * @return     { bool denoting if a collision occured }
 */
bool detect_live_collision(int x, int y);

/**
 * @brief      Create a life object at a random location on screen
 */
void  create_life();

/**
 * @brief      Draws lives sprites to screen.
 */
void draw_lives();

/**
 * @brief      Add a live to player on life pickup
 */
void pick_life();

/**
 * @brief      Update lives objects.
 */
void update_lives();
