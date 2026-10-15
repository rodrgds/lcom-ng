#ifndef G03_INSTRUCTIONS_H
#define G03_INSTRUCTIONS_H

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>
#include "cursor.h"
#include "objects.h"


/**
 * @brief xpm of the instructions page
 *
 */
object instructions_menu;
/**
 * @brief xpm of the instructions with hover in the back button
 *
 */
object instructions_back;


/**
 * @brief Initializes instructions xpms
 *
 */
void init_instructions();

/**
 * @brief Deals with the result of the movement of the mouse, changing te state of the game if the button back is clicked and changing the color of that button if the mouse is on top of it
 * 
 * @param mouse_c mouse object
 * @param p packet with the information of the mouse after movement
 * @return int 1 if the state of the program was changed
 */
int instructionsMenu(cursor *mouse_c, struct packet *p);

/**
 * @brief  Checks if the mouse is on top of the button back
 *  
 * @param mouse_c mouse cursor object
 * @return int 1 if its within the button limits
 */
int menuInstructionsHover(cursor *mouse_c);

/**
 * @brief Loop of the instruction page that uses mouse, and timer devices
 * 
 * @return int 1 if an error has occurred
 */
int instructionsLoop();

#endif //G03_INSTRUCTIONS_H
