/**
 * @file alien.h
 * @brief Header file containing definitions and function prototypes for alien manipulation.
 */

#ifndef ALIEN_H
#define ALIEN_H

#include <stdint.h>
#include <stdbool.h>

#define ALIEN_COLUMNS 8
#define ALIEN_ROWS 4 
#define ALIEN_COUNT (ALIEN_COLUMNS * ALIEN_ROWS) 


/**
 * @brief Struct representing an alien entity.
 * @details This struct contains the x and y coordinates of the alien on the screen and a flag 
 * indicating if it's alive or not.
 */
typedef struct {
  uint16_t x;
  uint16_t y;
  bool alive;
} Alien;

/**
 * @brief Array containing all aliens in the game.
 * @details This array stores all alien entities present in the game.
 */
extern Alien aliens[ALIEN_COUNT];

/**
 * @brief Interval for alien shooting.
 * @details This variable represents the interval between alien shots.
 */
extern int alien_shoot_interval;


/**
 * @brief Initializes the aliens at the start of the game.
 * @details This function initializes the positions of all aliens at the start of the game.
 * @param x_res The screen width resolution.
 * @param y_res The screen height resolution.
 */
void init_aliens(uint16_t x_res, uint16_t y_res);

/**
 * @brief Draws an alien sprite at the specified coordinates.
 * @details This function draws an alien sprite at the specified (x, y) coordinates on the screen.
 * @param x The x-coordinate of the upper-left corner of the alien sprite.
 * @param y The y-coordinate of the upper-left corner of the alien sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_alien(uint16_t x, uint16_t y);

/**
 * @brief Deletes an alien sprite at the specified coordinates.
 * @details This function deletes an alien sprite at the specified (x, y) coordinates on the screen 
 * by drawing a black sprite over it.
 * @param x The x-coordinate of the upper-left corner of the alien sprite.
 * @param y The y-coordinate of the upper-left corner of the alien sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_alien(uint16_t x, uint16_t y);

/**
 * @brief Draws all alive aliens on the screen.
 * @details This function draws all alive aliens on the screen.
 */
void draw_aliens(void);

/**
 * @brief Sets an alien as dead.
 * @details This function marks the specified alien as dead and deletes its sprite from the screen.
 * @param index The index of the alien to be marked as dead.
 */
void set_alien_dead(int index);


/**
 * @brief Moves all alive aliens.
 * @details This function moves all alive aliens on the screen according to their current direction.
 */
void move_aliens(void);

/**
 * @brief Checks if all aliens are dead.
 * @details This function checks if all aliens are dead.
 * @return true if all aliens are dead, false otherwise.
 */
bool check_all_aliens_dead(void);

#endif // ALIEN_H
