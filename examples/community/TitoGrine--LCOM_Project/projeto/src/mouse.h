#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "MACROS.h"


extern int hook_id_mouse;
extern uint8_t data;
extern bool error_flag;


/**
 * @brief Mouse interrupt handler 
 */
void (mouse_ih)();

/**
 * @brief Subscribes to the mouse's interrupts
 * 
 * @param bit_no
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the mouse's interrupts 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Issues a mouse command
 * 
 * @param command Command that will be issued
 */
int (mouse_cmd)(uint8_t command);

#endif
