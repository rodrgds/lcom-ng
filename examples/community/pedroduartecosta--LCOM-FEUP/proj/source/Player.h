#ifndef __PLAYER_H
#define __PLAYER_H

#include "interface.h"
#include "keyboard.h"
#include "Obstacles.h"
#include "Bitmap.h"

/** @defgroup Player Player
 * @{
 * functions regarding the player
 */
typedef struct{
	int x,y,vel,vel_i; //coordenadas do jogador,velocidade variavel  e velocidade constante
	Bitmap* ash_l;
	Bitmap* ash_r;
	int hPlayer, wPlayer; //width e height
	int counter,sec,change;
}Player;

/**
 * @brief creates a new player
 *
 * @return Null pointer to the new player
 */
Player* newPlayer();
/**
 * @brief draws the player
 *
 * @param player to be drawn
 */

void drawPlayer(Player* p);

/**
 * @brief move player according to input
 *
 * @param player to be moved
 * @param keyboard make code
 */

void movePlayer(Player* p,unsigned long scancode);

/**
 * @brief deletes a player
 *
 * @param player to be deleted
 */


void deletePlayer(Player* p);

/**
 * @brief verifies if player hits the track walls
 *
 * @param player
 */
int checkTrackCollision(Player* p);

/**
 * @brief updates player's counter
 *
 * @param player
 */
void updateCounterPlayer(Player* p);
/**
 * @brief updates player's speed
 *
 * @param player
 * @param int saying if the speed is to bo incresead or not
 */
void updatePlayerSpeed(Player* p,int incSpeed);
/**@}*/
#endif

