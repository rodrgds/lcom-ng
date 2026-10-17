#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../../devices/util/KBC.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/graphics/graphics.h"
#include "../elements/sprite/sprite.h"
#include "../draw/draw.h"
#include "../elements/elements.h"
#include "../moves/moves.h"
#include "../settings.h"


///@brief Handles the timer interrupt.
void handle_timer_interrupt();

///@brief Handles the keyboard interrupt.
void handle_keyboard_interrupt();

///@brief Handles the mouse interrupt.
void handle_mouse_interrupt();

///@brief Performs a move action based on the game state, a check move function, and a move player function.
///@param state The game state in which the move action should be performed.
///@param check_move Pointer to the move check function.
///@param move_player Pointer to the move player function.
void move_action(GameState state, int (*check_move)(), void (*move_player)());

#endif
