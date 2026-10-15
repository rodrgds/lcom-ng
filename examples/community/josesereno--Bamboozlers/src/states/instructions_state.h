#ifndef _INSTRUCTIONS_STATE_H_
#define _INSTRUCTIONS_STATE_H_

/**
 * @file instructions_state.h
 * @brief Header file containing function declarations for instructions state update functions.
 */
#include "controller/KBC/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/keyboard/keyboard.h"
#include "controller/video/graphics.h"
#include "viewer/viewer.h"
#include "state.h"

/** @defgroup instructions_state instructions_state
 * @{
  */
extern Sprite* cursor;
/**
 * @brief Updates the menu state based on the keyboard input in the instructions menu.
 */
void (update_keyboard_instructions)();



#endif
