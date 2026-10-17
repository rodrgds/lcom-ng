#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "piece.h"
#include "ball.h"
#include "graphics/video_gr.h"
#include "colors.h"
#include "menu.h"
#include "powerups.h"


/// @brief Subscribes the interrupts of all the devices used in the project
/// @return 0 if success, 1 if no success
int subscribe_interrupts();

/// @brief Unsubscribes the interrupts of all the devices used in the project
/// @return 0 if success, 1 if no success
int unsubscribe_interrupts();

/// @brief Handles the interrupts of all the devices used
/// @return 0 if success, 1 if no success
int interrupts();

/// @brief Sets the state of the keys pressed or released
void handle_keys();

#endif
