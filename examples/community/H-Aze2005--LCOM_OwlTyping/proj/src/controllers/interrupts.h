/**
 * @file interrupts.h
 * @brief Interrupt handling for timer, keyboard, and mouse devices.
 *
 * Declares functions and variables for subscribing, unsubscribing, and handling
 * interrupts from timer, keyboard, and mouse. Also provides handlers for device input.
 */


#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <lcom/lcf.h>
#include "devices/timer.h"
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "views/views.h"
#include "virtual_keyboard_controller.h"
#include "keyboard_map.h"
#include "model/model.h"
#include "controllers/typing_test_controller.h"

/**
 * @brief IRQ set bit masks for timer, keyboard, and mouse.
 */
uint8_t irq_set_timer, irq_set_keyboard, irq_set_mouse;

/**
 * @brief Current application state.
 */
extern State current_state;

/**
 * @brief Subscribes to timer, keyboard, and mouse interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int subscribe_devices();

/**
 * @brief Unsubscribes from timer, keyboard, and mouse interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int unsubscribe_devices();

/**
 * @brief Handles all device interrupts.
 * @param msg_int Interrupt notification message.
 */
void interrupts_handler(fsblkcnt_t msg_int);

/**
 * @brief Handles keyboard input events.
 */
void keyboard_handle_input();

/**
 * @brief Handles mouse input events.
 */
void mouse_handle_input();

#endif
