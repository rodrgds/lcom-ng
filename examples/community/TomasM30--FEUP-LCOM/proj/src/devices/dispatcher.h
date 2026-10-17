#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <lcom/lcf.h>

#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "mouse/mouse.h"
#include "video/video.h"
#include "rtc/rtc.h"

#include "../game/game.h"

/** @defgroup dispatcher dispatcher
 *  @brief Functions for handling the devices' interrupts
*/

static uint8_t irq_set_timer, irq_set_keyboard, irq_set_mouse, irq_set_rtc; /*!< IRQ set for all different devices*/
extern int timer_counter; /*!< Counter for the timer*/
extern uint8_t scancode; /*!< Scancode of the last key pressed*/
extern uint8_t byte_index; /*!< Index of the byte of the mouse packet*/
extern struct packet packet; /*!< Mouse packet*/
extern int mouse_x, mouse_y; /*!< Mouse coordinates*/

extern GameState state; /*!< Current state of the game*/

extern uint8_t date_rtc[3], time_rtc[3]; /*!< Date[day,month,year] and time[hours,minutes,seconds] arrays of the RTC*/

/**
 * @brief Subscribes the devices' interrupts
 * @return 0 upon success, 1 otherwise
*/
int subscribe_devices();

/**
 * @brief Unsubscribes the devices' interrupts
 * @return 0 upon success, 1 otherwise
*/
int unsubscribe_devices();

/**
 * @brief Handles the interrupts of all the devices
 * @return 0 upon success, 1 otherwise
*/
int dispatcher();

/**
 * @brief Handles the timer interrupts
*/
void timer_handler();

/**
 * @brief Handles the keyboard interrupts
*/
void keyboard_handler();

/**
 * @brief Handles the mouse interrupts
*/
void mouse_handler();

/**
 * @brief Handles the RTC interrupts
*/
void rtc_handler();

#endif
