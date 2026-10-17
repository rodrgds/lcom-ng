
#include <lcom/lcf.h>
#include "i8042.h"

#pragma once 
#include "game_structs.h"

extern uint8_t scancode;
extern int count;
extern int hook_kbd;
extern bool status_error;
extern int counter;
extern uint8_t scancode_bytes[2];


/**
 * @brief Subscribes keyboard interrupts.
 * 
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Interrupt handler.
 * 
 */
void (kbc_ih)();

/**
 * @brief Reenable keyboard interrupts
 * 
 */
int (kbd_enable)();


