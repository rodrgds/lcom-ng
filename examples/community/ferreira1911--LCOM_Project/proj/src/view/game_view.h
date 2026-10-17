#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <lcom/lcf.h>

/**
 * @brief Draws the game over screen with logo, timer, hits and accuracy.
 *
 * Clears the screen, draws the logo and labels, computes accuracy percentage,
 * and displays the relevant numbers.
 *
 * @param mouse_clicks Total number of mouse clicks made by the player
 * @param target_hits Total number of targets successfully hit
 * @param seconds     Total time elapsed in seconds
 */
void (draw_game_over_screen)(uint8_t mouse_clicks, uint8_t target_hits, uint8_t seconds);

/**
 * @brief Draws the timer text and remaining seconds.
 *
 * Loads and displays the timer XPM, then draws the numeric seconds.
 *
 * @param seconds Remaining time in seconds
 */
void (draw_timer)(uint8_t seconds);

/**
 * @brief Draws the hits text and number of successful hits.
 *
 * Loads and displays the hits XPM, then draws the numeric hit count.
 *
 * @param target_hits Number of targets hit
 */
void (draw_hits)(uint8_t target_hits);


/**
 * @brief Draws the fails text and number of misses.
 *
 * Loads and displays the fail XPM, then draws the numeric fail count.
 *
 * @param target_fails Number of targets missed
 */
void (draw_fails)(uint8_t target_fails);

/**
 * @brief Draws the losses text and number of lost targets.
 *
 * Loads and displays the lost XPM, then draws the numeric loss count.
 *
 * @param target_losses Number of targets that disappeared before being hit
 */
void (draw_losses)(uint8_t target_losses);

/**
 * @brief Draws a warning image indicating the player has failed.
 *
 * Displays the "you_fail" XPM at a fixed position on the screen.
 */
void (draw_warning)();

#endif
