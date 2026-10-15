#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "cursor.h"

/**
 * @brief  Checks if the mouse is on top of the button continue
 *  
 * @param mouse_c mouse object
 * @return int 1 if its within the button limits
 */
int menuContinueCollisions(cursor *mouse_c);

/**
 * @brief Deals with the result of movement of the mouse, changing te state of the game if the button continue is clicked and changing the color of the button if the mouse is on top of it 
 *  
 * @param mouse_c mouse object
 * @param p packet with the information of the mouse after movement
 * @return int 1 if the state of the program was changed
 */
int continueMenu(cursor *mouse_c, struct packet *p);

/**
 * @brief Loop of the gameOver page that uses mouse, keyboard and timer devices
 * 
 * @return int 1 if an error has occurred
 */
int gameOverPage();

void drawGameOverElements();
