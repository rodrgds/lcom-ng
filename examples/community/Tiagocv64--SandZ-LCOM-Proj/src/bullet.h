#pragma once

/** @defgroup bullet bullet
 * @{
 * Functions for manipulating bullet objects
 */

/**
 * @brief      Represents one bullet object
 */
struct Bullet {
  int id; 								/**< id denoting type of bullet */
  int x; 								/**< x coordinate on screen */
  int y; 								/**< y coordinate on screen */
  int directionY; 						/**< representing the y direction */
  int directionX; 						/**< representing the x direction */
  bool alive;							/**< flag marking if bullet is active */
  unsigned long long disapearCounter; 	/**< counter denoting the time which the bullet is active */
};

/**
 * @brief      Initialize bullets structure.
 */
void init_bullets();

/**
 * @brief      Destroy bullets structure and free memory used by it
 */
void destroy_bullets();

/**
 * @brief      Creates a bullet object with the specified atributes.
 *
 * @param[in]  x           { x coordinate }
 * @param[in]  y           { y coordinate }
 * @param[in]  directionY  { y direction }
 * @param[in]  directionX  { x direction }
 * @param[in]  id          { identifier }
 */
void create_bullet(int x, int y, int directionY, int directionX, int id);

/**
 * @brief      Detects bullet collision at sepecified coordinates with obstacles.
 *
 * @param[in]  x  { x coordinate }
 * @param[in]  y  { y coordinate }
 *
 * @return     { bool denoting if a collision occured }
 */
bool detect_bullet_collision(int x, int y);

/**
 * @brief      Update bullets objects.
 */
void update_bullets();

/**
 * @brief      Draws bullets sprites to screen.
 */
void draw_bullets();
