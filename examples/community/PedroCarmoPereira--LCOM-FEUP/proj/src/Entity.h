#ifndef _ENTITY_H
#define _ENTITY_H

#include <lcom/lcf.h>

/** @defgroup Entity Entity
 * @{
 */

typedef struct position_t{
  int x;
  int y;
} Position;

typedef enum entityType {
  PLAYER,
  MOUSE,
  ENEMY1,
  ENEMY2,
  BOSS,
  OBSTACLE,
  LASERF,
  LASERE,
  PLAYBUTTON,
  HIGHSCORESBUTTON,
  QUITBUTTON,
  MENUBUTTON,
} EntityType;

typedef struct entity_t{
  int8_t xspeed;
  int8_t yspeed;
  xpm_image_t img;
  Position p;
  EntityType type;
  float angle;
} Entity;

/*
@brief loads xpm and initializes some variables

@param t type of entity

@return an entity struct
*/

Entity entity(EntityType t);

/*
@brief sets the position x value of an entity

@param e pointer to the entity

@param nx x value to set
*/

void setX(Entity * e, int nx);

/*
@brief sets the position y value of an entity

@param e pointer to the entity

@param ny y value to set
*/

void setY(Entity * e, int ny);

/*
@brief sets the position y and x value of an entity

@param e pointer to the entity

@param nx x value to set

@param ny y value to set
*/

void setPosition(Entity *e, int nx, int ny);

/*
@brief sets the speed ys and xs value of an entity

@param e pointer to the entity

@param xs x speed value to set

@param ys y speed value to set
*/

void setSpeed(Entity *e, int8_t xs, int8_t ys);

/*
@brief draws entity

@param e entity to draw
*/

void drawEntity(Entity * e);

/*
@brief checks if two entities collided

@param e1 first entity

@param e2 second entity

@return true if collided, false if it didnt
*/

bool colision(const Entity *e1, const Entity *e2);

/*
@brief checks if entity is offscreen

@param e entity

@return true if is offscreen, false if it isnt
*/

bool isOffScreen(Entity e);

#endif
