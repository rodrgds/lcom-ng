/**
 * @file proj.h
 * @brief Main hardware initialization, interrupt macros, and Minix specific setup.
 */

#ifndef _PROJ_H_
#define _PROJ_H_

#include <stdint.h>

#define VIDEO_MODE 0x115 
#define FPS 60

extern uint8_t timer_bit_no;
extern uint8_t kbd_bit_no;
extern uint8_t mouse_bit_no;
extern uint8_t rtc_bit_no;
extern uint32_t timer_counter;
extern uint8_t mouse_byte;
extern bool mouse_error;

/**
 * @brief Subscribes interrupts, sets configurations, and switches to graphics mode.
 * * @return 0 on success, non-zero on failure.
 */
int system_init();

/**
 * @brief Unsubscribes interrupts and restores Minix text mode.
 * * @return 0 on success, non-zero on failure.
 */
int system_cleanup();

#endif
