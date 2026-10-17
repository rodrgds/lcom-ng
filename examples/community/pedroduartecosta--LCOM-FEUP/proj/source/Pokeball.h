#ifndef __POKEBALL_H
#define __POKEBALL_H

#include "Bitmap.h"

/** @defgroup Pokeball Pokeball
 * @{
 * functions regarding the Pokeball
 */
typedef struct{
	int x,y; //Coordenadas do obstaculo
	Bitmap* poke; // bitmap referente ao obstaculo
	int ht,wh;
	int use; //igual a 0 quando nao esta a ser usado e igual 1 quando esta a ser usado
	int val; //igual a 1 quando ainda nao foi apanhada e igual a 0 quando ja foi apanhada
	int vel;
} Pokeball;

/**
 * @brief creates a new pokeball
 *
 * @param x coordinate
 * @param y coordinate
 *
 * @return Null pointer to the new pokeball
 */
Pokeball *newPoke(int x,int y);
/**
 * @brief moves pokeball
 *
 * @param pokeball to be updated
 */

void updatePoke(Pokeball* pb);
/**
 * @brief draws pokeball on the track
 *
 * @param pokeball to be drawn
 */
void drawPoke(Pokeball* pb);
/**
 * @brief deletes pokeball
 *
 * @param pokeball to be deleted
 */
void deletePoke(Pokeball* pb);

/**
 * @brief updates pokeball
 *
 * @param pokeball to be updated
 */
void updatePokeUse(Pokeball* pb);

/**
 * @brief updates pokeball speedn
 *
 * @param pokeball to be updated
 * @param int regarding if the speed is to be incresead or not
 */

void updatePokeSpeed(Pokeball* pb,int incSpeed);


/**@}*/

#endif
