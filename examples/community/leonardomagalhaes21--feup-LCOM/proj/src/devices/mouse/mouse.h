/**
 * @file mouse.h
 * @brief Contains functions for handling mouse interrupts and commands.
 */


#ifndef MOUSE_H_
#define MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "game/sprite/sprite.h"
#include "game/classes/mouse_cursor.h"
#include "devices/graphics/graphics.h"


/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to the variable that will store the bit number of the interrupt.
 * @return 0 on success, 1 on failure.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);



/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return 0 on success, 1 on failure.
 */
int (mouse_unsubscribe_int)();


/**
 * @brief Gets the status from the keyboard controller.
 *
 * @param status Pointer to the variable that will store the status.
 * @return 0 on success, 1 on failure.
 */
int (mouse_get_status)(uint8_t* status);


/**
 * @brief Reads the output from the keyboard controller.
 *
 * @param output Pointer to the variable that will store the output.
 * @return 0 on success, 1 on failure.
 */
int (mouse_read_output)(uint8_t* output);


/**
 * @brief Interrupt handler for mouse controller.
 */
void (mouse_ih)();


/**
 * @brief Writes a command to the specified port of the keyboard controller.
 *
 * @param port The port to write the command to.
 * @param cmd The command to write.
 * @return 0 on success, 1 on failure.
 */
int (kbc_write_cmd)(uint8_t port ,uint8_t cmd);


/**
 * @brief Writes a command to the mouse.
 *
 * @param cmd The command to write.
 * @return 0 on success, 1 on failure.
 */
int (mouse_write_cmd)(uint8_t cmd);


/**
 * @brief Generates a mouse packet from the mouse bytes.
 */
void (mouse_generate_packet)();



/**
 * @brief Synchronizes the mouse bytes.
 */
void (mouse_bytes_sync)();




#endif
