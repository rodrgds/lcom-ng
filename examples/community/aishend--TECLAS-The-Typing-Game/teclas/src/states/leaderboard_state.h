/**
 * @file leaderboard_state.h
 * @brief Leaderboard display screen.
 */
#ifndef _TECLAS_LEADERBOARD_STATE_H_
#define _TECLAS_LEADERBOARD_STATE_H_

#include "../results.h"
#include "../state.h"
#include "../video.h"

/**
 * @brief Initialize the leaderboard state.
 *
 * @return 0 on success, non-zero on failure.
 */
int leaderboard_state_init();

/**
 * @brief Draw the leaderboard state.
 *
 * Renders the leaderboard contents to the active video buffer.
 */
void leaderboard_state_draw();

/**
 * @brief Reload leaderboard data.
 *
 * Refreshes the leaderboard entries from the saved results.
 */
void leaderboard_state_reload();

/**
 * @brief Destroy the leaderboard state.
 *
 * Releases any resources allocated by the leaderboard state.
 */
void leaderboard_state_destroy();

#endif // _TECLAS_LEADERBOARD_STATE_H
