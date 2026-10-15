/**
 * @file start_state.h
 * @brief Start/splash screen.
 */
#ifndef _TECLAS_START_STATE_H_
#define _TECLAS_START_STATE_H_

/**
 * @brief Initialize the start/splash screen.
 *
 * @return 0 on success, non-zero on failure.
 */
int start_state_init();

/**
 * @brief Draw the start/splash screen.
 */
void start_state_draw();

/**
 * @brief Free resources used by the start screen.
 */
void start_state_destroy();

#endif
