#ifndef _GAME_LOOP_H_
#define _GAME_LOOP_H_

/**
 * @file game_loop.h
 * @brief Declares the application interrupt loop entry point.
 */

/**
 * @brief Run the main application loop.
 *
 * Initializes loop-owned assets and device subscriptions, dispatches hardware
 * events to state-specific handlers, and releases loop-owned resources before
 * returning.
 *
 * @return 0 on successful exit, non-zero on failure.
 */
int game_loop(void);

#endif /* _GAME_LOOP_H_ */
