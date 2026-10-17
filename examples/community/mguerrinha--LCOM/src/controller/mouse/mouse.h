#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "KBC.h"
#include "controller/graphics/graphics.h"

/**
 * @brief Struct that represents the necessary information of the mouse.
*/
typedef struct {
    uint8_t r_click;
    uint8_t l_click;
    uint16_t x;
    uint16_t y;
} MouseInfo;

/**
 * @brief Function that reads the current byte of the mouse from the KBC.
*/
void (mouse_ih)();

/**
 * @brief Function that writes a command to the mouse.
 * @param command Command to write to the mouse.
 * @return 0 if the command was successfully written, 1 otherwise.
*/
int (write_to_mouse)(uint8_t command);

/**
 * @brief Function that subscribes the mouse interrupts.
 * @param bit_no Variable to store the bit mask of the mouse interruptions.
 * @return 0 if the subscription was successful, 1 otherwise.
*/
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function that unsubscribes the mouse interrupts.
 * @return 0 if the unsubscription was successful, 1 otherwise.
*/
int (mouse_unsubscribe_int)();

/**
 * @brief Function that syncs the bytes of the mouse into the current packet, according to the current byte and the current index.
*/
void (mouse_sync_bytes)();

/**
 * @brief Function that updates the mouse information according to the current packet.
*/
void (mouse_move_to_info)();

#endif
