#ifndef SCORE_VIEW_H
#define SCORE_VIEW_H

#define SCORE_Y 500
#define DIGIT_WIDTH 25

/**
 * @brief Computes the digit pixmap of a given digit.
 * @param digit is the digit whose pixmap will be computed.
 * 
 */
void (get_score_digit_pixmap)(int digit);

/**
 * @brief Draws the user total score when the game ends.
 * 
 */
void (draw_score)();

#endif
