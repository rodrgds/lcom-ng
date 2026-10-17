#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include "i8042.h"

/**
 * @file mouse.h
 * @brief Declares functions, structures and enums for mouse interaction and handling.
 *
 * Includes support for subscribing to mouse interrupts, parsing packets,
 * handling communication with the KBC, and running a basic gesture recognition
 * state machine.
 */

typedef struct mouse_reading_t
{
    struct packet elem;
    bool error;
    size_t byte_index;
    
} mouse_reading_t;


typedef enum {
    START,
    UP,
    VERTEX,
    DOWN,
    END
} STATES;

mouse_reading_t get_mouse_reading();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
int(write_command_KBC)(uint8_t port,uint8_t command);
int(mouse_data_reporting)(uint8_t cmd);

#endif
