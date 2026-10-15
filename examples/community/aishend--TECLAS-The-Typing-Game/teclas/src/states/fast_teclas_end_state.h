/**
 * @file fast_teclas_end_state.h
 * @brief Fast Teclas end/results screen.
 */
#ifndef _TECLAS_FAST_TECLAS_END_STATE_H_
#define _TECLAS_FAST_TECLAS_END_STATE_H_

#include <stdbool.h>
#include "../config.h"

/**
 * @brief Initialize the fast teclas end screen.
 *
 * @param cfg Game configuration used to display player name and theme.
 */
void fast_teclas_end_state_init(GameConfig cfg);

/**
 * @brief Draw the fast teclas end screen.
 */
void fast_teclas_end_state_draw();

/**
 * @brief Handle a mouse click on the end screen.
 *
 * @return true if the click triggered a state transition, false otherwise.
 */
bool fast_teclas_end_state_handle_click();

#endif
