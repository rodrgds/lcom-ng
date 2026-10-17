#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "sprite.h"

/**
 * @brief Struct used to store the information of a player
 * 
 */
typedef struct {
  char name[20];          /**< Player's name */
  unsigned int points;    /**< Player's points */
  Sprite* avatar;         /**< Avatar chosen by the player */
} player;


/* ============= */
/*  CONSTRUCTOR  */
/* ============= */
/**
 * @brief Create an empty player object. Allocates memory for the player struct.
 * 
 * @return player*  Returns a valid player pointer if it was possible to allocate enough memory,
 *                  returns NULL if it was not
 */
player* create_empty_player();


/* ============ */
/*  DESTRUCTOR  */
/* ============ */
/**
 * @brief Descructor for the player, frees the memory of a given player
 * 
 * @param p     Player pointer that points to the player that will be deleted
 */
void delete_player(player* p);


/* ======================== */
/*  Public Player Functions  */
/* ======================== */

/**
 * @brief Sets the player's name
 * 
 * @param p       Player pointer that points to the player whose name will be set
 * @param name    Name that will be atributed to the player
 */
void set_name(player* p, char* name);

/**
 * @brief Sets the player's avatar
 * 
 * @param p       Player pointer that points to the player whose avatar will be set
 * @param avatar  Avatar that will be atributed to the player
 */
void set_avatar(player* p, Sprite* avatar);

/**
 * @brief Updates the player's points, increments or decrements them according to signal
 *        of the points parameter
 * 
 * @param p 
 * @param points 
 */
void update_points(player* p, int points);


#endif
