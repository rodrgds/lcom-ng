#pragma once

#include <lcom/lcf.h>
#include "controller.h"
#include "xpms.h"

/**
 * @brief Struct that represents the mouse pointer, and its position
 */
struct Cursor{
    int x;
    int y;
};

/**
 * @brief When in the menu, it changes the selected option to the option above the currently selected one.
 * When the current selected option is the top one, it goes to the last one in the menu.
 * 
 */
void (upMenuOption)();

/**
 * @brief When in the menu, it changes the selected option to the option below the currently selected one.
 * When the current selected option is the last one, it goes to the first one in the menu.
 * 
 */
void (downMenuOption)();

/**
 * @brief Returns the current entry in the menu
 *
 * @return value of the current entry in the menu 
 */
int (getMenuOption)();

/**
* @brief checks and sets the menu option
*/
void (setMenuOption)(int option);

/**
 * @brief Struct for the player and all its attributes.
 */
struct Player{
    int x_pos;          /*!< @brief Player's position on the x-axis */
    int y_pos;          /*!< @brief Player's position on the y-axis */
    unsigned int color; /*!< @brief Color of the player */
    int y_velocity;     /*!< @brief Distance moved, on the y-axis, per unit of time */
    int x_velocity;     /*!< @brief Distance moved, on the x-axis, per unit of time */
    int height;         /*!< @brief Player's height */
    int width;          /*!< @brief Player's width */
    int score;          /*!< @brief Points scored in the current match */
};

/**
 * @brief Struct that represents a ball, and all its attributes
 */
struct Ball{
    int x_pos;          /*!< @brief Ball's position on the x-axis */
    int y_pos;          /*!< @brief Ball's position on the y-axis */
    int x_velocity;     /*!< @brief Distance, on the x-axis, per unit of time */
    int y_velocity;     /*!< @brief Distance, on the y-axis, per unit of time */
    unsigned int color; /*!< @brief Color of the ball */
    int height;         /*!< @brief Ball's height */
    int width;          /*!< @brief Ball's width */
};

/**
 * @brief Struct that represents the arena, and all its attributes
 */
struct Arena{
    int x_max;          /*!< @brief Arena farthest corner on the x-axis */
    int x_min;          /*!< @brief Arena closest corner on the x-axis */
    int y_max;          /*!< @brief Arena farthest corner on the y-axis */
    int y_min;          /*!< @brief Arena closest corner on the y-axis */
    int height;         /*!< @brief Length of the arena */
    int width;          /*!< @brief Width of the arena */
};

/**
 * @brief Enum that represents the type of collision that happened
 */
enum CollisionType {
  NoCollision,          /*!< @brief No collision happened */
  SideCollision,        /*!< @brief Side collision happened */
  TopBottomCollision    /*!< @brief Top or bottom collision happened */
};

/**
 * @brief Enum that represents the winner of the match
 * 
 */
enum Winner {
    PLAYER1,            /*!< @brief Player 1 won */
    PLAYER2,            /*!< @brief Player 2 won */
    NO_WINNER           /*!< @brief No winner yet */
};

/**
 * @brief Resets the positions of the players and the ball to their initial positions in the arena
 */
void (resetPositions)();

/**
 * @brief Resets positions and determines initial velocity of the ball
 */
void (reset)();

/**
 * @brief Resets the game, i.e. resets the positions of the players and the ball to their initial positions and resets the score of the players
 */
void (resetGame)();

/**
 * @brief Moves the player up, if possible
 *
 * @param player Player to be moved
 */
void (movePlayerUp)(struct Player *player);

/**
 * @brief Moves the player down, if possible
 *
 * @param player Player to be moved
 */
void (movePlayerDown)(struct Player *player);

/**
 * @brief Moves the player right, if possible
 *
 * @param player Player to be moved
 */
void (movePlayerRight)(struct Player *player);

/**
 * @brief Moves the player left, if possible
 *
 * @param player Player to be moved
 */
void (movePlayerLeft)(struct Player *player);

/**
 * @brief Activates powerup 1 (horizontal movement)
 */
void (activatePowerup1)();

/**
 * @brief Activates powerup 2 (bigger player)
 */
void(activatePowerup2)();

/**
 * @brief Resets the powerup to its default state
 */
void (resetPowerup)();

/**
 * @brief Moves the AI player based on the position of the ball
 * 
 * @param player 
 */
void (moveAI)(struct Player *player);

/**
 * @brief Checks if the ball is colliding with the player, and if so, returns the type of collision
 * 
 * @param player 
 * @return enum CollisionType 
 */
enum CollisionType (checkBallCollision)(struct Player *player);

/**
 * @brief Moves the ball in the correct direction depending if there was a collision or not
 * 
 * @return true 
 * @return false 
 */
bool (moveBall)();

/**
 * @brief verifies if there was a goal
 * 
 * @return true 
 * @return false 
 */
bool (isGoal)();

/**
 * @brief Checks if the score threshold was reached and returns the winner
 * 
 * @return enum Winner 
 */
enum Winner (checkScore)();

/**
* @brief Moves the cursor by dx and dy
*/
void (moveCursor)(int dx, int dy);

/**
* @brief Compresses data to send through serial port
*/
uint8_t (compressData)(int data, int compressor);

/**
* @brief Decompresses the data
*/
uint8_t (decompressData)(uint8_t data, int decompressor);

/**
* @brief Switches the players
*/
void (switchPlayers)();

/**
*  @brief Switches the color of the players
*/
void (switchColorPlayers)();

/**
*  @brief Switches the ball direction
*/
void (switchBall)();
