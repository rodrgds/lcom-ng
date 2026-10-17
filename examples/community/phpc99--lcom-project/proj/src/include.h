#ifndef _INCLUDE_H_
#define _INCLUDE_H_

/**
 * @file include.h
 * @brief Global includes and external variable declarations.

 */

/**
 * @brief Array stores scan code bytes read from the keyboard.
 *
 * This array is used to store up to two bytes of scan codes,
 * which is necessary for handling extended keys.
 */

#include <lcom/lcf.h>
#include "Controller/Mouse_Keyboard/mouse.h"
#include "Controller/Mouse_Keyboard/keyboard.h"
#include "Controller/VBE/GPU.h"
#include "Controller/VBE/VBE.h"


extern uint8_t scan_code[2];
/**
 * @brief Global timer interrupt counter.
 *
 * Keeps track of the number of timer interrupts received.
 */
extern int counter_TIMER;

#endif
