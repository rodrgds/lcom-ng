#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <lcom/lcf.h>
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Laser.h"

/** @defgroup GameState GameState
 * @{
 */

typedef struct entityVector{
  Entity curr;
  struct entityVector * next;
} * EntityVector;

typedef struct  enemyVector{
  Enemy curr;
  struct enemyVector * next;
} * EnemyVector;

typedef struct gamestate_t {
  unsigned score;
  unsigned phase;
  unsigned ek;
  Player player;
  EntityVector obstacles;
  EnemyVector enemies;
  LaserVector laser;
  LaserVector enemyFire;
} GameState;

/*
@brief moves the asteroids

@param v vector of obstacles to move
*/

void moveObstacles(EntityVector *v);

/*
@brief moves the lasers

@param g gamestate struct to move enemy lasers and player laser
*/

void moveLasers(GameState * g);

/*
@brief adds asteroids

@param v vector of asteroids

@param e entity to save in the vector
*/

void addObstacle(EntityVector *v, Entity *e);

/*
@brief adds enemies

@param v vector of enemies

@param e entity to save in the vector
*/

void addEnemy(EnemyVector *v, Enemy *e);

/*
@brief erases asteroids when they are offscreen

@param v vector of asteroids to erase
*/

void freeObstacles(EntityVector *v);

/*
@brief erases enemies when they are destroyed

@param v vector of enemies to erase

@return enemies killed
*/

int freeEnemies(EnemyVector * v);

/*
@brief damages enemies if the laser collided with it

@param g gamestate struct to access laser and enemies
*/

void dmgEnemies(GameState * g);

/*
@brief checks if lasers collided with asteroids

@param g gamestate struct to access lasers
*/

void LaserColide(GameState *g);

/*
@brief adds a new laser

@param g gamestate struct to add lasers

@param mousex x position of the mouse

@param mousey y position of the mouse
*/

void generateLaser(GameState * g, int mousex, int mousey);

/*
@brief adds lasers to enemies

@param g gamestate struct to add lasers
*/

void enemiesFire(GameState * g);

/*
@brief adds asteroids

@param g gamestate struct to add asteroids
*/

void generateAsteroids(GameState * g);

/*
@brief adds enemies

@param g gamestate struct to add enemies
*/

void generateEnemies(GameState * g);

/*
@brief adds boss enemy

@param g gamestate struct to add boss enemy
*/

void generateBoss(GameState * g);

/*
@brief draws the asteroids

@param v vector of obstacles to draw
*/

void drawObstacles(EntityVector v);

/*
@brief draws the lasers

@param v vector of lasers to draw
*/

void drawLaser(LaserVector v);

/*
@brief draws the enemies

@param v vector of enemies to draw
*/

void drawEnemies(EnemyVector v);

/*
@brief initializes GameState struct

@return GameState struct
*/

GameState gamestate();

/*
@brief draws all the gamestate sprites

@param g gamestate struct
*/

void drawGS(GameState g);

/*
@brief checks if the player crashed against an asteroid

@param g gamestate struct

@return true if it crashed or false if it didnt
*/

bool playerCrashed(const GameState * g);

/*
@brief damages the player if it crashed against an asteroid or was shot

@param g gamestate struct
*/

void dmgPlayer(GameState *g);

#endif /*_GAMESTATE_H*/
