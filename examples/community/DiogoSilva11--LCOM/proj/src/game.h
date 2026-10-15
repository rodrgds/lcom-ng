#ifndef _GAME_H_
#define _GAME_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "timer_info.h"
#include "kbd.h"
#include "mouse.h"
#include "vbe_func.h"

#include "info.h"
#include "controller.h"
#include "sprites.h"

/** @defgroup game game
 * @{
 * 
 * Functions for the game.
 */

/**
 * @brief Movement direction
 */
enum Movement {
  RIGHT,
  LEFT,
  DOWN,
  UP,
  DOWN_RIGHT,
  DOWN_LEFT,
  UP_RIGHT,
  UP_LEFT,
  INVALID
};

/**
 * @brief Crosshair info
 */
typedef struct {
  int x;
  int y;
} Crosshair;

/**
 * @brief Hero info
 */
typedef struct {
  int x;
  int y;
  bool alive;
} Hero;

/**
 * @brief Robot info
 */
typedef struct {
  int x;
  int y;
  bool alive;
} Robot;

/**
 * @brief Fence info
 */
typedef struct {
  int x;
  int y;
} Fence;

/**
 * @brief Bullet info
 */
typedef struct {
  int x;
  int y;
  int xStep;
  int yStep;
  bool hit;
} Bullet;

/**
 * @brief Main loop where the game actions and the handling of interrupts happen
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (gameplay)();

/**
 * @brief Calculate the pixel x-coordinate
 *
 * @param logX logical x-coordiante
 * @return Return the pixel x-coordinate
 */
int (getPixelX)(int logX);

/**
 * @brief Calculate the pixel y-coordinate
 *
 * @param logY logical y-coordiante
 * @return Return the pixel y-coordinate
 */
int (getPixelY)(int logY);

/**
 * @brief Set attributes for all the elements
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (loadGame)();

/**
 * @brief Draw the arena on the console screen
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (displayArena)();

/**
 * @brief Generate the pixels for the crosshair
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (drawCrosshair)();
/**
 * @brief Generate the pixels for the fences
 *
 * @return Return 0 upon success and non-zero otherwise
 */

int (drawFences)();

/**
 * @brief Generate the pixels for the hero
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (drawHero)();

/**
 * @brief Generate the pixels for the robots
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (drawRobots)();

/**
 * @brief Generate the pixels for the bullet
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (drawBullet)();

/**
 * @brief Check if the Hero won or lost the game
 *
 * @return Return false upon success and true otherwise
 */
bool (validGame)();

/**
 * @brief Update crosshair coordinates
 *
 * @param x x displacement 
 * @param y y displacement
 */
void (moveCrosshair)(int16_t x, int16_t y);

/**
 * @brief Check collision between two entities
 *
 * @param x1 first object x position
 * @param y1 first object y position
 * @param w1 first object width
 * @param h1 first object height 
 * @param x2 second object x position
 * @param y2 second object y position
 * @param w2 second object width
 * @param h2 second object height
 * @return Return true upon success and false otherwise
 */
bool (checkCollision)(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

/**
 * @brief Checks if the hero can move and updates his status 
 *
 * @param newX new x position
 * @param newY new y position
 * @return Return true upon success and false otherwise
 */
bool (canHeroMove)(uint16_t newX, uint16_t newY);

/**
 * @brief Get the movement key and move hero 
 *
 * @param key_code movement key
 */
void (moveHero)(uint8_t key_code);

/**
 * @brief Get shortest path between two moving entities
 *
 * @param x1 first entity x position
 * @param y1 first entity y position
 * @param x1 second entity x position
 * @param y1 second entity y position
 * @return Return movement direction
 */
enum Movement (shortestPath)(int x1, int y1, int x2, int y2);

/**
 * @brief Checks if a robot can move and updates its status 
 *
 * @param id robot index
 * @param newX new x position
 * @param newY new y position
 * @return Return true upon success and false otherwise
 */
bool (canRobotMove)(int id, uint16_t newX, uint16_t newY);

/**
 * @brief Move all robots towards the hero
 */
void (moveRobots)();

/**
 * @brief Calculate the crosshair's center pixel x-position
 *
 * @return Return the crosshair's center pixel x-position
 */
int (crosshairCenterX)();

/**
 * @brief Calculate the crosshair's center pixel y-position
 *
 * @return Return the crosshair's center pixel y-position
 */
int (crosshairCenterY)();

/**
 * @brief Reset the bullet's status after a hit 
 */
void (bulletReset)();

/**
 * @brief shoot the bullet giving it a direction
 */
void (shoot)();

/**
 * @brief Get the bullet direction according to crosshair placement
 */
void (bulletDirection)();

/**
 * @brief Checks if the bullet has hit an object
 *
 * @param newX new x position
 * @param newY new y position
 * @return Return true upon success and false otherwise
 */
bool (checkHit)(uint16_t newX, uint16_t newY);

/**
 * @brief The movement of the bullet within the shot action
 */
void (bulletTravel)();

/**
 * @brief Keep the bullet with the hero
 */
void (bulletStore)();

/**@}*/

#endif /* _GAME_H */
