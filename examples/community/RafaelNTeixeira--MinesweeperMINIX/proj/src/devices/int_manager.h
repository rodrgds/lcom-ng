#pragma once

#include <lcom/timer.h>

#include "timer/timer.h"
#include "keyboard/kbc.h"
#include "mouse/mouse.h"
#include "graphic/video.h"
#include "../game/display.h"

#include "event_handler/timer_event_handler.h"
#include "event_handler/mouse_event_handler.h"
#include "event_handler/kbc_event_handler.h"

#include "game/game_state.h"
#include "game/display.h"

/**
 * @brief Initializes the system and sets up the necessary interrupts and devices
 * @return Return 0 upon success and non-zero otherwise
 */
int (initialize)();

/**
 * @brief Main loop of the system. This function enters the main loop of the system and continuously waits for messages from device interrupts
 * @return Return 0 upon success and non-zero otherwise
*/
int (main_loop)();

/**
 * @brief Finalizes the system.
 * @return Return 0 upon success and non-zero otherwise
*/
int (finalize)();
