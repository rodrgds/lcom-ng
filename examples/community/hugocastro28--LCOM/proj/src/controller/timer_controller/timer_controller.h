#ifndef TIMER_CONTROLLER_H
#define TIMER_CONTROLLER_H

/**
 * @brief Initializes the game time to 120 seconds.
 * 
 */
void (initialize_game_time)();

/**
 * @brief Decreases game time every 60 timer interruptions and 
 * moves all ghosts every 10 timer interruptions.
 * 
 */
void (timer_interrupt_handler)();

/**
 * @brief Decreases game time by 1 second.
 * 
 */
void (decrease_game_time)();

/**
 * @brief stores the game time before pause.
 * 
 */
void (pause_game)();

/**
 * @brief Updates the game time to the one previously stored.
 * 
 */
void (continue_game)();

/**
 * @brief Increases game time by 30 seconds.
 * 
 */
void (increase_time)();

/**
 * @brief Check if the time is over and if it is, updates the game state.
 * 
 */
void (check_time_out)();

#endif
