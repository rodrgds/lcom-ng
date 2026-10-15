#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>


/**
 * @brief Loop of the multiplayer game that uses the mouse, keyboard and timer devices
 * 
 * @return int 1 if an error has occured
 */
int (multiPlayerMode)();

/**
 * @brief Loop of the singleplayer game that uses the rtc, keyboard and timer devices
 * 
 * @return int 1 if an error has occured
 */
int (singlePlayerMode)();

/**
 * @brief initiates all elements of the game, and draws the initial board
 */
void (init_game)();

/**
 * @brief this function handles the keyboard interruption, when the arrows or the specific characters are pressed
 */
void (keyboard_interrupt)();



