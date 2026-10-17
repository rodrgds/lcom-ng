/**
 * @file game_timer
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief Contains the functions related to the timer of the game
 * 
 */
#ifndef _LCOM_GAME_TIMER_H_
#define _LCOM_GAME_TIMER_H_

/**
 * @brief Initializes the timer image xpm files
 * 
 */
void (timer_files_init)();

/**
 * @brief Draws the game timer on the screen
 * 
 * @returns 0 on success, non-zero otherwise
 */
int game_counter();


#endif /*_LCOM_GAME_TIMER_H_*/
