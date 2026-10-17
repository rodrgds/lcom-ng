/**
 * @file mouse.h
 * 
 */

#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lab4.h>
#include <stdbool.h>
#include <stdint.h>

#include "KBC.h"

/* IRQ */
#define MOUSE_IRQ 12

/* ACKNOWLEDGEMENT BYTES */
#define MOUSE_SUCCESS_BYTE 0xFA
#define MOUSE_ERROR_BYTE 0xFE
#define MOUSE_TWICE_ERROR_BYTE 0xFC

#define is_first_byte(byte) (byte & BIT(3))
#define sign_extend(byte) (0xFF00 | byte)

/* MOUSE BITS */
#define MOUSE_Y_OVFL BIT(7)
#define MOUSE_X_OVFL BIT(6)
#define MOUSE_Y_MSB BIT(5)
#define MOUSE_X_MSB BIT(4)
#define MOUSE_MIDDLE_BUTTON BIT(2)
#define MOUSE_RIGHT_BUTTON BIT(1)
#define MOUSE_LEFT_BUTTON BIT(0)

typedef struct {
    struct packet pp;
    uint8_t packet_no;
    uint16_t x, y;
    uint16_t prev_x, prev_y;
    bool error;
} mouse_data_t;

/**
 * @brief subscribes the mouse interrupts
 * @param bit_no pointer to a number representing the position dedicated to the mouse in the interrupt vector
 * @return zero on success, non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t* bit_no);

/**
 * @brief unsubscribes mouse interrupts
 * @return zero on success, non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief enables data reporting for the mouse
 * @return zero on success, non-zero otherwise
 */
int (mouse_enable_data_report)(uint32_t wait_ticks);

/**
 * @brief disables data reporting for the mouse
 * @return zero on success, non-zero otherwise
 */
int (mouse_disable_data_report)(uint32_t wait_ticks);

/**
 * @brief reads the newest mouse data
 */
void (mouse_get_data)(mouse_data_t* data, uint32_t wait_ticks);

/**
 * @brief parses the mouse data received from the KBC
 * @param data the data to be parsed
 */
void (mouse_parse_packet)(mouse_data_t* data);

#endif // _LCOM_MOUSE_H_
