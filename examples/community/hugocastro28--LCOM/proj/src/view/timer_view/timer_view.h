#ifndef TIMER_VIEW_H
#define TIMER_VIEW_H

#define TIMER_X 910
#define TIMER_Y 20
#define TIMER_WIDTH 88
#define TIMER_HEIGHT 25

#define MIN_X 910
#define TIME_DIV_X 932
#define SEC_MSD_X 954
#define SEC_LSD_X 976

/**
 * @brief Computes the digit pixmap of a given digit.
 * @param digit is the digit whose pixmap will be computed.
 * 
 */
void (get_digit_pixmap)(int digit);

/**
 * @brief Draws the remaining time until the end of the game.
 * 
 */
void (draw_timer)();

/**
 * @brief Erases the displayed time using a black rectangle.
 * 
 */
void (erase_timer)();

#endif
