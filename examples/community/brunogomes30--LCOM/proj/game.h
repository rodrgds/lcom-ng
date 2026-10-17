#ifndef GAME_H
#define GAME_H
typedef struct Game Game;
typedef struct Player Player;
typedef struct Gun Gun;
typedef struct Bullet Bullet;

#include "gun.h"
#include "graphics.h"
#include "player.h"
#include "pistoladeagua.h"
#include "cursor.h"
#include "bonecoparado.h"
#include "sprite.h"
#include "player_right_xpm.h"
#include "player_left_xpm.h"
#include "bullet.h"
#include "enemy1_xpm.h"
#include "enemy2_xpm.h"
#include "linkedEntities.h"
#include "enemy.h"
#include "rtc.h"
#include "xpm's/pistol0.h"
#include "xpm's/pistol1.h"
#include "xpm's/pistol2.h"
#include "xpm's/pistol3.h"
#include "xpm's/letters.h"
#include "xpm's/bar.h"
#include "xpm's/numbers.h"

#include "xpm's/explosion0.h"
#include "xpm's/explosion1.h"
#include "xpm's/explosion2.h"
#include "xpm's/explosion3.h"

/** @defgroup game 
 * @{
 *
 * Functions related to the game 
 */


// Defines to manage enemies spawn time / difficulty
#define ENEMIES_TO_INCREASE_DIFFICULTY 2
#define ENEMY_TIME_TO_DECREASE 100
#define ENEMY_INITIAL_TIME 1500
#define ENEMY_MIN_SPAWN_TIME 300
typedef enum {MENU, GAME, GAMEOVER} GameState;

struct Game {
  uint8_t numOfPlayers;
  double gameSpeed; // 1 = real time
  int x_res, y_res, highScore; 
  Date dateHighScore;
  GameState state;
  Player *players[2];
  long unsigned gameTime;
  long unsigned nextSpawn;
  unsigned spawnTime; // time between enemy spawns
  unsigned enemyCount; // counts enemys that were spawn in current spawnTime
  DoubleLinkedList entitiesList;
} ;


/**
 * @brief Init Game struct with the required variables
 * 
 */
void initGame();

/**
 * @brief Load images, so that they don't need to be loaded every time they are used
 * 
 */
void initImages();

/**
 * @brief Update all animations from the entities in the game
 * 
 * @param game 
 */
void updateAnimations(Game * game);

/**
 * @brief Receive keyboard interrupt, updates player input
 * 
 * @param game 
 * @param kbd_code 
 * @return int 
 */
int receiveKeyboardInterrupt(Game * game, uint32_t kbd_code);

/**
 * @brief Receive timer interrupt, move and draw all entities, and also checks for colisions
 * 
 * @param game 
 * @return int 
 */
int receiveTimerInterrupt(Game * game);

/**
 * @brief Receive Mouse interrupt, updates player input
 * 
 * @param game 
 * @param dataPacket 
 * @return int 
 */
int receiveMouseInterrupt(Game * game, struct packet dataPacket);

/**
 * @brief Receives periodic interrupt, sets all enemies to move towards player
 * 
 * @param game 
 */
void receivePeriodicInterrupt(Game *game);

/**
 * @brief Move all entities from the game
 * 
 * @param game 
 * @param gameSpeed 
 */
void moveEntities(Game *game,double gameSpeed);

/**
 * @brief Move given Entity at the given gameSpeed
 * 
 * @param gameSpeed 
 * @param entity 
 */
void moveEntity(double gameSpeed, EntitiesLinkedElement *entity);

/**
 * @brief Move Sprite at the given gamespeed
 * 
 * @param gameSpeed 
 * @param sprite 
 */
void moveSprite(double gameSpeed, Sprite *sprite);

/**
 * @brief Checks for the next animation, and updates if required
 * 
 * @param entity 
 */
void updateEntityAnimation(EntitiesLinkedElement *entity);

/**
 * @brief Spawn enemy at a random location outside of the screen
 * 
 * @param game 
 */
void spawnEnemy(Game *game);

/**
 * @brief Draw all entities
 * 
 * @param game 
 */
void drawEntities(Game *game);

/**
 * @brief Draw a single entity
 * 
 * @param entity 
 */
void drawEntity(EntitiesLinkedElement * entity);

/**
 * @brief Add the Enemy to the game
 * 
 * @param game 
 * @param enemy 
 */
void addEnemy(Game * game, Enemy *enemy);

/**
 * @brief Checks for collisions, and handles them
 * 
 * @param game 
 */
void handleCollisions(Game *game);

/**
 * @brief Handles a single collision between 2 entities
 * 
 * @param game 
 * @param first 
 * @param second 
 */
void handleCollision(Game *game, EntitiesLinkedElement *first, EntitiesLinkedElement *second);

/**
 * @brief Removes an entity
 * 
 * @param game 
 * @param entity 
 */
void destroyEntity(Game *game, EntitiesLinkedElement * entity);

/**
 * @brief Get the sprite from the entity
 * 
 * @param entity 
 * @return Sprite* 
 */
Sprite * getSprite(EntitiesLinkedElement * entity);

/**
 * @brief Deletes all entities that have remove set to 1
 * 
 * @param game 
 */
void deleteEntities(Game *game);


/**
 * @brief Checks if a new highscore was achieved and saves it
 * 
 * @param game 
 * @param score 
 */
void saveHighScore(Game *game, int score);

/**
 * @brief Loads the current highscore
 * 
 * @param game 
 */
void loadHighScore(Game *game);

#endif
