#ifndef _GAME_H_
#define _GAME_H_
#include <lcom/lcf.h>

#include "dynamite.h"
#include "player.h"
#include "position.h"
#include "sprite.h"
#include "target.h"

#define MAX_X 1151 /** @brief Maximum x coordinate of the screen */
#define MAX_Y 863 /** @brief Maximum y coordinate of the screen */
#define NUM_TARGETS (NUM_TARGET_LINES * NUM_TARGETS_PER_LINE) /** @brief Number of targets in the game */
#define NUM_TARGET_LINES 3 /** @brief Number of lines of targets in the game */
#define NUM_TARGETS_PER_LINE 7 /** @brief Number of targets per line in the game */



/** @brief Return is the game is in multiplayer mode. */
bool(isMultiplayer)();

/** @brief Set the game to multiplayer mode .
  * 
  * @param value - true if the game is in multiplayer mode, false otherwise.
  */
void(setMultiplayer)(bool value);

/**
 * GENERIC GAME FUNCTIONS
 */

/** @brief Initialize the game.
  * Initializes the game variables and the players .
  * @param multiplayer - true if the game is in multiplayer mode, false otherwise.
  */
void(initGame)(bool multiplayer);

/** @brief Resets the game.
  * Resets the game variables and the players .
  */
void(endGame)();

// GAME UPDATES

/** @brief Updates game countdown.
  * Decreases the time left in the game. If the time is over, ends the game
  */
void(updateTimes)();

/** @brief Updates game targets
  * Updates the position of the targets in the game.
  * If slow time is active, updates the targets at a slower pace.  
  */
void(updateTargets)();

/** @brief Updates game dynamite
  * Updates the position of the dynamite in the game.
  * If slow time is active, updates the dynamite at a slower pace.  
  */
void(updateDynamite)();

// SHOTS AND COLLISIONS

/**
  * @brief Check all collisions with targets and dynamite
  * 
  * @param player the player that will check the collisions with
  * @return the index of the target, which the player shot, or -1 if the player didn't shot in any target
  */
int(checkAllCollisions)(Player *player);

/**
  * @brief Check all collisions with targets
  * 
  * @param player the player that will check the collisions with
  * @return the index of the target, which the player shot, or -1 if the player didn't shot in any target
  */
int(checkCollisionWithTargets)(Player *player);

/**
  * @brief Check collision between one target and one shot of one player
  * 
  * @param player the player that will check the collisions with
  * @param i the index of the target
  * @return the index of the target or -1 if they didn't collided
  */
int(checkCollisionWithTarget)(Player *player, int i);

/**
  * @brief Check all collisions with dynamite
  * 
  * @param player the player that will check the collisions with
  * @return true if collided, false other
  */
bool(checkCollisionWithDynamite)(Player *player);

// SLOW TIME

/**
  * @brief Get if the player can slow time
  * 
  * @return true if the player can slow time, false otherwise
  */
bool(canSlowTime)();

/**
  * @brief Set Slow Time
  * 
  */
void(setSlowTime)();

/**
  * @brief End Slow Time
  * 
  */
void(endSlowTime)();

/**
 * GETTERS AND SETTERS OF GAME VARIABLES
 */

/**
  * @brief Get the player 1
  * 
  * @return pointer to player 1
  */
Player *(getPlayer1) ();

/**
  * @brief Get the player 2
  * 
  * @return pointer to player 2
  */
Player *(getPlayer2) ();

// PLAYER POSITION

/**
  * @brief Add a delta to X value
  * 
  * @param player the player that will add the X
  * @param delta_x the value to be added
  */
void(addToX)(Player *player, int16_t delta_x);

/**
  * @brief Add a delta to Y value
  * 
  * @param player the player that will add the Y
  * @param delta_x the value to be added
  */
void(addToY)(Player *player, int16_t delta_y);

// TARGETS
int16_t(getFallCounterOfTarget)(int i);

/** @brief Increment the fall counter of the target for the sprite animation
  * @param i - the index of the target
  */
void(incrementFallCounterOfTarget)(int i);

/** @brief Checks if the target is active
  * @param i - the index of the target
  * @return true if the target is active, false otherwise
  */
bool(isActiveTarget)(int i);

/** @brief Set the target to active or inactive
  * @param i - the index of the target
  * @param value - true if the target is active, false otherwise
  */
void(setActiveTarget)(int i, bool value);

// DYNAMITE

/** @brief Check if the dynamite is active
  * @return true if the dynamite is active, false otherwise
  */
bool(isActiveDynamite)();


/** @brief Set the dynamite to active or inactive
  * @param value - true if the dynamite is active, false otherwise
  */
void(setActiveDynamite)(bool value);

// SCORE

/** @brief Add value to the score of the player
  * @param player - the player that will have the score added
  * @param value - the value that will be added to the score
  */
void(addToScore)(Player *player, int value);

/** @brief Subtract value to the score of the player
  * @param player - the player that will have the score subtracted
  * @param value - the value that will be subtracted to the score
  */
void(subractToScore)(Player *player, int value);

// TIME

/** @brief Get the time left in the game
  * @return the time left in the game
  */
int(getTimeLeft)();

/** @brief Check if the game is over
  * @return true if the game is over, false otherwise
  */
bool(endTime)();

/**
 * DRAW FUNCTIONS
 */

/** @brief Draw the game in the screen
  * @param isDay - true if the game is in day mode, false otherwise
  */
void(draw_game)(bool isDay);

/** @brief Draw the aim of the player in the game 
  * @param player - the player that will have the aim drawn
  */
void draw_aim(Player *player);

/** @brief Draw the lines below the targets in the game */
void(draw_lines)();

/** @brief Draw the targets in the game
  * If the target is active, draws it in the screen
  */
void(draw_targets)();

/** @brief Draw the dynamite in the game
  * If the dynamite is active, draws it in the screen
  */
void(draw_dynamite)();

/** @brief Draw the score of the player */
void(draw_score)();

/** @brief Draw the time left in the game */
void(draw_timeLeft)();

#endif
