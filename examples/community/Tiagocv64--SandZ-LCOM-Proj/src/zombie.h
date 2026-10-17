#pragma once

/** @defgroup zombie zombie
 * @{
 * Functions for manipulating zombie objects
 */

/**
 * @brief      Represents one zombie object
 */
struct Zombie {
  int x; 								/**< x coordinate on screen */
  int y; 								/**< y coordinate on screen */
  int directionY; 						/**< representing the y direction */
  int directionX; 						/**< representing the x direction */
  bool alive;							/**< flag marking if zombie is alive */
  int hp;								/**< current hp */
  uint8_t bitState;						/**< current zombie Bitmap state */
  uint8_t bitTime;						/**< time to next zombie Bitmap state */
};

/**
 * @brief      Initialize zombies structure
 */
void init_zombies();

/**
 * @brief      Destroy zombies structure and free memory used by it
 */
void destroy_zombies();

/**
 * @brief      Detects zombie collision at sepecified coordinates with obstacles.
 *
 * @param[in]  x  { x coordinate }
 * @param[in]  y  { y coordinate }
 *
 * @return     { bool denoting if a collision occured }
 */
bool detect_zombie_collision(int x, int y);

/**
 * @brief      Create a zombie object
 */
void create_zombie();

/**
 * @brief      Attack player
 */
void attack_zombie();

/**
 * @brief      Update zombies objects
 */
void update_zombies();

/**
 * @brief      Draws zombies sprites to screen.
 */
void draw_zombies();

/**
 * @brief      Draws injured zombies hp bar sprites to screen.
 */
void draw_zombies_hp();
