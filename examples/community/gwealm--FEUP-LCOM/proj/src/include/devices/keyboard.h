#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "events/events.h"

#define KBD_IRQ 1
#define KBD_LONG_CODE (0xE0)
#define KBD_BREAK_CODE (0x80)

// keyboard make codes used for character mapping
#define ESC (0x01)
#define ZERO (0x0B)
#define BACKSPACE (0x0E)
#define Q (0x10)
#define P (0x19)
#define ENTER (0x1C)
#define A (0x1E)
#define L (0x26)
#define Z (0x2C)
#define DOT (0x34)
#define NUM_SEVEN (0x47)
#define NUM_NINE (0x49)
#define NUM_FOUR (0x4B)
#define NUM_SIX (0x4D)
#define NUM_ONE (0x4F)
#define NUM_DOT (0x53)

/**
 * @brief Handles operations to inicialize keyboard for usage
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
int (kbd_init)();

/**
 * @brief Handles operations to terminate keyboard usage
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (kbd_cleanup)();

/**
 * @brief Subscribes keyboard interrupts
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (kbd_subscribe_int)();

/**
 * @brief Unubscribes keyboard interrupts
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Translates a keyboard makecode to a readable character
 * 
 * @param code The makecode to translate
 * @return The translated character
 */
char (kbd_map_code_to_char)(uint8_t code);

/**
 * @brief Handles a keyboard interrupt
 * 
 */
void (kbd_handle_interrupt)(struct event *event);

/**
 * @brief Handles a single keyboard byte
 * 
 */
void (kbd_handle_byte)(uint8_t code);

