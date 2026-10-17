#pragma once

/** @defgroup blood blood
 * @{
 * Functions for manipulating blood objects
 */

/**
 * @brief      Represents one blood object
 */
struct Blood{
  uint32_t x; 				/**< x coordinate on screen */
  uint32_t y; 				/**< y coordinate on screen */
  bool bloodExists; 		/**< flag marking if blood is active */
  unsigned long long timer; /**< counter denoting the time which the blood is active */
};


/**
 * @brief      Initialize blood structure.
 */
void init_blood();

/**
 * @brief      Destroy blood structure and free memory used by it
 */
void destroy_blood();


/**
 * @brief      Creates a blood object at specific x and y coordinates.
 *
 * @param[in]  x     { x coordinate }
 * @param[in]  y     { y coordinate }
 */
void create_blood(uint32_t x, uint32_t y);

/**
 * @brief      Draw blood sprites to screen
 */
void draw_blood();

/**
 * @brief      Update blood objects
 */
void update_blood();
