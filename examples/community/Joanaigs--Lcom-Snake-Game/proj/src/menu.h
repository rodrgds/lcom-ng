#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>
#include "cursor.h"
#include "objects.h"


/**
 * @brief xpm of the main menu page
 *
 */
object main_menu;
/**
 * @brief xpm of the main menu page with the singlePlayer colored
 *
 */
object sp_menu;
/**
 * @brief xpm of the main menu page with the multiPlayer colored
 *
 */
object mp_menu;
/**
 * @brief image of the main menu page with the intructions colored
 *
 */
object inst_menu;
/**
 * @brief image of the main menu page with the exit colored
 *
 */
object exit_menu;



/**
 * @brief Initiates all the menu objects by reading their xpm
 *
 */
void init_menu();

/**
 * @brief draws the menu
 * 
 */
void drawMenu();

/**
 * @brief Deals with the result of movement of the mouse, changing the state of the game if any button is clicked and changing the color of the button if the mouse is on top of it
 * 
 * @param mouse_c mouse object
 * @param p packet with the information of the mouse after movement
 * @return int 1 if the state of the program was changed
 */
int menu(cursor *mouse_c, struct packet *p);

/**
 * @brief  Checks if the mouse is on top of any button
 *  
 * @param mouse_c mouse object
 * @return int 1 if its within the button limits
 */
int menuOptionCollisions(cursor *mouse_c);

/**
 * @brief Loop of the main Menu page that uses mouse, and timer devices
 * 
 * @return int 1 if an error has occurred
 */
int mainMenuLoop();

/**
 * @brief sets false every onclick button and choose button
 */
void set_false_buttons();

