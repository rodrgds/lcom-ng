#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

extern bool mouse_ready;
extern struct packet pp;

/**
 * @brief Subscribes to mouse interrupts.
 *
 * This function subscribes to mouse interrupts and enables notifications.
 *
 * @param bit_no Pointer to a variable where the interrupt bit number will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * This function unsubscribes from mouse interrupts and disables notifications.
 *
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Gets the status of the mouse.
 *
 * This function reads the status register of the mouse.
 *
 * @param st Pointer to a variable where the status byte will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (mouse_get_status)(uint8_t *st);

/**
 * @brief Writes a command byte to the mouse.
 *
 * This function writes a command byte to the mouse controller.
 *
 * @param cmdb The command byte to be written.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (mouse_write_cmd)(uint8_t cmdb);

/**
 * @brief Reads data from the mouse buffer.
 *
 * This function reads a byte of data from the mouse buffer.
 *
 * @param port The port from which to read the data.
 * @param output Pointer to a variable where the read byte will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (mouse_read_buffer)(uint8_t port, uint8_t *output);

/**
 * @brief Disables data reporting from the mouse.
 *
 * This function sends a command to the mouse to disable data reporting.
 *
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (mouse_disable_data_reporting)();

/**
 * @brief Mouse interrupt handler.
 *
 * This function is the interrupt handler for mouse interrupts. It processes the 
 * mouse input received during the interrupt.
 */
void (mouse_ih)();


#endif // MOUSE_H

