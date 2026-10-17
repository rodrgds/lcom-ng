#ifndef __HANDLER_H
#define __HANDLER_H

#include "keyboard.h"
#include "mouse.h"
#include "i8254.h"
/** @defgroup handler handler
 * @{
 *
 * Functions that handle the game and menu, handling the interruptions of the peripherals
 */

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @return Returns bit order if successful; negative value on failure
 */
int kbd_subscribe_int(void);
/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and -1 otherwise
 */
int kbd_unsubscribe_int(void);
/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe_int();
/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and -1 otherwise
 */
int mouse_unsubscribe_int();
/**
 * @brief Subscribes and enables timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void );
/**
 * @brief Unsubscribes timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();
/**
 * @brief Subscribes and enables real time clock interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int rtc_subscribe_int();
/**
 * @brief Unsubscribes real time clock interrupts
 *
 * @return Return 0 upon success and -1 otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Handles all the peripherals interruptions while in the menu
 *
 * In the beginning, all elements of the menu are initialized. Handles mouse, keyboard and timer interruptions. It also handles rtc,
 * using polling to get the date.
 * In each interruption of the mouse the cursor is updated and it is verified in any of the options (Play or Exit) has been chosen.
 * In each interruption of the timer the footer is updated. The interruption of the keyboard only recognizes if the ESC key is pressed,
 * that cause the application to terminate. If the Play option is chosen by the mouse, the application will show the game, and the
 * function selected will be game_handler()
 *
 * @see game_handler
 *
 * @return Return 1 upon success and -1 otherwise
 */
int menu();
/**
 * @brief Initializes all the elements of the game
 *
 * Initializes all elements of 2 bars, a ball and calls the function game_show to paint the game. Finally it actualizes the virtual address
 * to the VRAM, showing the game, and calls the function game.
 *
 * @see game
 * @see game_show
 *
 * @return Return 1 upon success and -1 otherwise
 */
int game_handler();
/**
 * @brief Handles all the peripherals interruptions while in the game
 *
 * Handles keyboard and timer interruptions. It also handles RTC, using polling to get the date and hour.
 * In each interruption of the keyboard, the state machine of each bar is updated, so that both bars can move at the same time if each
 * correspondent key is pressed.
 * In each interruption of the timer, the bars and ball positions are updated and so is the date.
 * When the game ends, either by pressing ESC key or when te time runs out, it is shown who the winner is.
 *
 * @see winner
 *
 * @param b1 is a struct with the information of one of the 2 bars of the game, this one on the left
 * @param b2 is a struct with the information of one of the 2 bars of the game, this one on the right
 * @param a is a struct with the information of the ball's game
 *
 * @return Return 1 upon success and -1 otherwise
 */
int game(Bar *b1, Bar *b2, Ball *a);
/**
 * @brief Initializes all the elements of the game, giving them the initial information to star the game again.
 *
 *  Only happens after a score
 *
 * @param b1 is a struct with the information of one of the 2 bars of the game, this one on the left
 * @param b2 is a struct with the information of one of the 2 bars of the game, this one on the right
 * @param a is a struct with the information of the ball's game
 *
 */
void reset_game(Bar *b1, Bar *b2, Ball *a);
/**
 * @brief Paints in the buffer the pixmap correspondent to the winner or if it is a draw.
 *
 * Checks the score of each bar and calculates the winner.
 *
 * @param b1 is a struct with the information of one of the 2 bars of the game, this one on the left
 * @param b2 is a struct with the information of one of the 2 bars of the game, this one on the right
 */
void winner(Bar *b1, Bar *b2);
/**
 * @brief Enable IO-sensitive operations and call sef_startup to handle initialization from the system service. Calls the menu
 *
 * @return 1 always
 */
int main();

#endif
