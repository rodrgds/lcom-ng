#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "graphics.h"
#include "i8042.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions that handle the keyboard and the movements of elements
 */

/**
 * @brief Checks if the output from the KBC is valid and return it
 *
 * @return output from the KBC if valid, -1 if not valid
 */
int kbd_int_handler();
/**
 * @brief Changes the coordinates of the bar
 *
 * Changes the coordinates of the bar according to the input from the KBC. If the players are pushing W or O, the bar goes up, if they're
 * pushing S or K, the bar goes down. The coordinates change according to the speed predefined to the bar.
 *
 * @param b is a struct that contains information about the bar, including it's current coordinates and speed
 */
int movement(Bar *b);
/**
 * @brief Changes the coordinates of the footer
 *
 * Changes the coordinates of the footer, adding to it's current position the speed.
 *
 * @param r is a struct that contains information about the footer, including it's current coordinates and speed
 */
void footer_mov(Footer *r);
/**
 * @brief Changes the coordinates of the ball
 *
 * Changes the coordinates of the ball, adding or subtracting the speed to it's current position. It also deals with situations when the
 * ball reaches one of the bars or the limits of the field, in each the angle will be preserved. When the ball passes by one of the bars
 * and doesn't touch it, the score flag will be changed to 1.
 *
 * @param b1 is a struct with the information of one of the 2 bars of the game, this one on the left
 * @param b2 is a struct with the information of one of the 2 bars of the game, this one on the right
 * @param a is a struct with the information of the ball's game
 */
void ball_movement(Bar *b1, Bar *b2, Ball *a);


#endif
