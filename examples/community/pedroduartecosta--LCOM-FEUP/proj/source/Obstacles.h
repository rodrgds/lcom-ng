#ifndef __OBSTACLES_H
#define __OBSTACLES_H

#include "Bitmap.h"

/** @defgroup Obstacles Obstacles
 * @{
 * functions regarding the obstacles
 */

typedef struct{
	int x,y; //Coordenadas do obstaculo
	Bitmap* type; // bitmap referente ao obstaculo
	int ht,wh;
	int use; //igual a 0 quando nao esta a ser usado e igual 1 quando esta a ser usado
	int vel;
} Obstacles;

/**
 * @brief creates a new obstacle
 *
 * @return Null pointer to the obstacle
 */
Obstacles *newObstacle(int x,int y);

/**
 * @brief moves the obstacle
 *
 * @param obstacle
 */
void updateObstacle(Obstacles* obs);
/**
 * @brief draws the obstacle
 *
 * @param obstacle
 */
void drawObstacle(Obstacles* obs);
/**
 * @brief deletes the obstacle
 *
 * @param obstacle
 */
void deleteObstacle(Obstacles* obs);
/**
 * @brief updates the obstacle
 *
 * @param obstacle
 */
void updateObstacleUse(Obstacles* obs);
/**
 * @brief updates the obstacle spped
 *
 * @param obstacle
 * @param int telling if the speed of the obstacle is to be changed
 */
void updateObstacleSpeed(Obstacles* obs,int incSpeed);//incSpeed=1 para aumentar a velocidade
/**@}*/

#endif
