#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include "vector.h"
#include "ball/ball.h"
#include "mouseModel.h"
#include "cue.h"
#include "cushion.h"
#include "pocket.h"
#include "player.h"
#include "button.h"

/**
 * @brief Enum that represents the game state
*/

typedef enum GAME_STATE {
  AIMING,
  SHOOTING,
  WAITING,
  SIMULATING,
  ADVANTAGE,
} GAME_STATE;

/**
 * @brief Struct that represents the table(when playing)
*/

typedef struct Table{
  xpm_image_t img;
  xpm_image_t ui;
  xpm_image_t spinCircle;
  xpm_image_t cursor;
  Button* exitButton;
  bool exitSelected;
  LinearCushion* linearCushions[6];
  CircularCushion* circularCushions[12];
  Pocket* pockets[6];
  Ball** balls;
  uint8_t ballNumber;
  GAME_STATE state;
  Mouse* mouse;
  Cue* cue;
  double maxSpeedShot;
  Player* player1;
  Player* player2;
  bool player1Won;
  Ball* firstBallHit;
  bool pocketedOwnBall;
  // Physics measurments
  double slidingFriction;
  double spinningFriction;
  double rollingFriction;
  double gravityAcceleration;
  double cushionRestitution;
  double cushionFriction;
  size_t ballRadius;
  xpm_image_t* font;
  char* winningPlayerName;
  // Simulation
  Event nextEvent;

} Table;

/**
 * @brief Creates and initializes a new Table structure with all necessary components such as players, balls, cushions, and graphical assets.
 *
 * @return Table* Pointer to the newly created Table structure.
 */

Table * newTable();

/**
 * @brief Destroys a Table object.
 *
 * This function frees the memory allocated for the Table object.
 *
 * @param table Pointer to the Table object to be destroyed.
 */
void destroyTable(Table* table);

/**
 * @brief Draws the table on the screen.
 *
 * This function draws the table on the screen using the specified xpm images.
 *
 * @param table Pointer to the Table object.
 * @return int Returns 0 on success, 1 on failure.
 */

int drawTable(Table* table,int gameTime, int roundTime);

/**
 * @brief Updates the state of the cue stick, including charge level and direction vector.
 *
 * This function updates the state of the cue stick based on the user input (mouse movement).
 * It calculates the charge level and direction vector of the cue stick for shooting the cue ball.
 *
 * @param table Pointer to the Table structure.
 * @param power Indicates whether the cue stick is being charged or not.
 * @return int 0 if the update is successful.
 */

int updateCueState(Table* table, bool power);

/**
 * @brief Determines the collision point between the cue ball and another ball on the table.
 *
 * This function calculates the collision point between the cue ball and another ball
 * on the table based on their positions and directions.
 *
 * @param table Pointer to the Table structure.
 * @param colisionPoint Pointer to a vector_t structure where the collision point will be stored.
 * @return bool True if a collision point is found, false otherwise.
 */

bool getColisionPoint(Table* table, vector_t*collisionPoint);

/**
 * @brief Moves the cue ball to the current mouse position.
 *
 * This function moves the cue ball to the current mouse position, allowing the player
 * to position the cue ball before shooting.
 *
 * @param table Pointer to the Table structure.
 */
void glueBall(Table* table);

/**
 * @brief Checks if the cue ball can be dropped at the current position.
 *
 * This function checks if the cue ball can be dropped at the current mouse position
 * without colliding with other balls or going out of bounds.
 *
 * @param table Pointer to the Table structure.
 * @return bool True if the cue ball can be dropped, false otherwise.
 */

bool canDropBall(Table* table);

/**
 * @brief Switches the turn between players.
 *
 * This function switches the turn between players after a shot is taken.
 * It also resets some game state variables for the new turn.
 *
 * @param table Pointer to the Table structure.
 */

void switchTurn(Table* table);

/**
 * @brief Retrieves the player who is currently playing.
 *
 * This function returns a pointer to the player who is currently playing the game.
 *
 * @param table Pointer to the Table structure.
 * @return Player* Pointer to the playing player.
 */

Player* getPlayingPlayer(Table* table);

/**
 * @brief Retrieves the player who is not currently playing.
 *
 * This function returns a pointer to the player who is not currently playing the game.
 *
 * @param table Pointer to the Table structure.
 * @return Player* Pointer to the not-playing player.
 */
Player* getNotPlayingPlayer(Table* table);

/**
 * @brief Determines if the given ball belongs to the given player.
 *
 * @param player Pointer to the Player structure.
 * @param ball Pointer to the Ball structure.
 * @return bool True if the ball belongs to the player, false otherwise.
 */


bool isPlayerBall(Player* player, Ball* ball);

/**
 * @brief Draws all balls on the table, including their current positions and states.
 *
 * @param table Pointer to the Table structure.
 */

void drawBalls(Table *table);

/**
 * @brief Updates the spin of the cue ball based on the mouse position.
 *
 * @param table Pointer to the Table structure.
 */
bool updateSpin(Table* table);

