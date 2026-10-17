#ifndef _PLAY_H
#define _PLAY_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "devices/graphics.h"
#include "xpms/up.xpm"
#include "xpms/left.xpm"
#include "xpms/brick.xpm"
#include "xpms/fontS.xpm"
#include "xpms/broken.xpm"
#include "xpms/down.xpm"
#include "xpms/right.xpm"
#include "xpms/wall.xpm"
#include "xpms/bgGame.xpm"
#include "xpms/fruit.xpm"
#include "xpms/bodySnake.xpm"
#include "kbcKeys.h"
#include "leaderboard.h"

#define PIXELOFFSET 48
#define SNAKEMAXSIZE 864

/**
 * Enum: SnakeBodyType
 * @brief Struct used to distinguish the type of the snake
 */
enum SnakeBodyType{
  HEAD,
  BODY,
  TAIL,
  NULLBODY
};

/**
 * Enum: ObjectType
 * @brief Struct used to distinguish the type of the objects in the game
 */
enum ObjectType{
  FRUIT,
  BLOCK,
  BROKEN,
};

/**
 * @brief Struct used to represent an object
 * Struct: Object
 * @param x
 * @param y
 * @param type
 * @param active to know whether or not the object is in play
 */
typedef struct Object_t
{
  int x;

  int y;

  enum ObjectType type;

  bool active;

}Object;


/**
 * @brief Struct used to represent a snake
 * Struct: SnakeBody
 * @param x
 * @param y
 * @param bodySize
 * @param bodyX array used to store the Snake's tail X coordinate
 * @param bodyY array used to store the Snake's tail Y coordinate
 * @param bodyType
 */
typedef struct Snake_t
{
  int y;

  int x;

  int bodySize;

  int bodyX[1000];

  int bodyY[1000];

  enum SnakeBodyType bodyType;

}SnakeBody;

/**
 * @brief Function to handle timer interrupts when the gameState is PLAY_SOLO, is also responsible for printing all the objects
 *
 */
void InterruptHandlerTimer();

/**
 * @brief Function responsible for initiating all the variables of the game, and also resetting them coming back from the Pause Menu
 */
void MenuStarter();

/**
 * @brief Function responsible for randomly spawning fruits within the map borders, also checks for possible collisions (spawn on top of the snake for example)
 */
void spawnFruits();

/**
 * @brief Function responsible for drawing the snake, taking in account the direction it is going
 */
void drawSnake();

/**
 * @brief Function responsible for not only moving the snake head but also for moving its tail
 */
void moveSnake();

/**
 * @brief Function to handle keyboard interrupts when the gameState is PLAY_SOLO, update the snake direction & check for ESC KEY(pause)
 * @param k (key the user pressed in the keyboard)
 */
void InterruptHandlerKBC(enum KEY k);

/**
 * @brief Function responsible for drawing all the objects of the game and also the mouse
 */
void drawObjects();

/**
 * @brief Function responsible for drawing the background of the game
 */
void drawBG();

/**
 * @brief Function responsible for checking possible collisions of the snake. Checks for the border,fruits & blocks.
 * @return 0 if success, 1 otherwise
 */
int CheckColisions();

/**
 * @brief Function responsible for update the movement of the snake, and also forbids the snake from going in the opposed direction
 */
void updateMov();

/**
 * @brief Function responsible for cleaning the pixels that changed from the initial background
 */
void cleanAllBG();

/**
 * @brief Function to handle mouse interrupts when the gameState is PLAY_SOLO. Takes care of the left click and right click functionality.
 */
void InterruptHandlerMouse();

/**
 * @brief Function to check if there is a fruit in the specified position
 * @param x
 * @param y
 * @return true if there is a fruit, false otherwise
 */
bool checkFruit(int x, int y);

/**
 * @brief Function responsible for creating a block in the specified position
 * @param x
 * @param y
 */
void initBlock(int x, int y);

/**
 * @brief Function responsible for spawning all the blocks
 */
void startBlocks();


/**
 * @brief Function responsible for returning the idx of the specified block in its array
 * @param x
 * @param y
 * @return the index, -1 otherwise
 */
int findIdxBlock(int x, int y);

/**
 * @brief Updates the gameState to DEAD, and writes on the leaderboard the score
 * @return 0 if sucess, -1 otherwise
 */
int GameOverLeaderBoard();

/**
 * @brief Function responsible for drawing the current score of the user on the screen
 */
void drawScore();

#endif
