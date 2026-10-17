/**
 * @file sprite.h
 * @brief Header file for managing sprites in the game.
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include <math.h>


#define MAX_ENEMY 32 /**< Maximum enemies during a game */

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/**
 * @brief Structure representing a sprite.
 * 
 * A Sprite contains all necessary information to create, animate, and
 * destroy a pixmap. 
 */
typedef struct {
  uint16_t x,y;             /**< current sprite position */
  uint16_t width, height;   /**< sprite dimensions */
  uint16_t xspeed, yspeed;  /**< current speeds in the x and y direction */
  uint32_t *map;     /**< the sprite pixmap */
} Sprite;

/**
 * @brief Creates a new sprite with the given parameters.
 * 
 * @param xpm The XPM image map of the sprite.
 * @param x The initial x-coordinate of the sprite.
 * @param y The initial y-coordinate of the sprite.
 * @param x_speed The initial x-axis speed of the sprite.
 * @param y_speed The initial y-axis speed of the sprite.
 * @return A pointer to the created sprite, or NULL on failure.
 */
Sprite * create_sprite(xpm_map_t xpm , uint16_t x, uint16_t y, uint16_t x_speed, uint16_t y_speed);


/**
 * @brief Destroys a sprite, freeing its resources.
 * 
 * @param sprite A pointer to the sprite to be destroyed.
 */
void destroy_sprite(Sprite *sprite);

/**
 * @brief Checks if a sprite collides with the screen borders.
 * 
 * @param sprite A pointer to the sprite to be checked.
 * @return True if there is a collision, false otherwise.
 * 
 */
bool check_borders_collisions(Sprite* sprite);

/**
 * @brief Moves a sprite according to its speed, considering screen limits.
 * 
 * @param sprite A pointer to the sprite to be moved.
 */
void move_sprite(Sprite *sprite);

/**
 * @brief Draws a sprite on the screen.
 * 
 * @param sprite A pointer to the sprite to be drawn.
 * @return 0 on success, 1 on failure.
 */
int draw_sprite(Sprite* sprite);

/**
 * @brief Draws a sprite on the screen with a specified angle.
 * 
 * @param sprite A pointer to the sprite to be drawn.
 * @param angle The angle to rotate the sprite.
 * @return 0 on success, 1 on failure.
 */
int draw_sprite_with_angle(Sprite* sprite, double angle);

/**
 * @brief Sets the x-coordinate of a sprite.
 * 
 * @param sprite A pointer to the sprite to be modified.
 * @param x The new x-coordinate.
 */
void set_x_sprite(Sprite* sprite, uint16_t x);

/**
 * @brief Sets the y-coordinate of a sprite.
 * 
 * @param sprite A pointer to the sprite to be modified.
 * @param y The new y-coordinate.
 */
void set_y_sprite(Sprite* sprite, uint16_t y);

/**
 * @brief Initializes all the needed sprites for the game.
 */
void initialize_sprites();


/**
 * @brief Destroys all the sprites used in the game
 */
void destroy_sprites();




/* ------------- Functions to draw different sized numbers ------------------------------------------*/

/**
 * @brief Draws a one-digit number on a certain screen coordinate using that digit sprite
 * 
 * @param x The x coordinate of the position to draw the digit
 * @param y The y coordinate of the position to draw the digit
 * @param digit The digit to be drawn
*/
void draw_digit(int x, int y, int digit);


/**
 * @brief Draws a two-digits number, one digit at a time, starting on a certain position of the screen
 * @param twoDigitNumber The number to be drawn
 * @param xStart  The x coordinate of the screen position to draw the first digit of the number
 * @param yStart  The y coordinate of the screen position to draw the first digit of the number
 * @param digitSize The size of the sprite of each digit
*/
void draw_two_digits(int twoDigitNumber, uint16_t xStart, uint16_t yStart, uint16_t digitSize);


/**
 * @brief Draws a three-digits number, one digit at a time, starting on a certain position of the screen
 * @param threeDigitNumber The number to be drawn
 * @param xStart  The x coordinate of the screen position to draw the first digit of the number
 * @param yStart  The y coordinate of the screen position to draw the first digit of the number
 * @param digitSize The size of the sprite of each digit
*/
void draw_three_digits(int threeDigitNumber, uint16_t xStart, uint16_t yStart, uint16_t digitSize);



/** @} end of sprite */


#endif /* _SPRITE_H_ */

