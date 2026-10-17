#ifndef MOUSE_H
#define MOUSE_H

// Includes
#include <lcom/lcf.h>
#include <stdint.h> 
#include "../i8042.h"
#include "KBC.h"
#include "../Graphics/sprite.h"




/**
 * @brief Enum to represent the state of the mouse packet processing.
 */
typedef enum {
    BYTE_1_WAIT, /**< @brief Waiting for the first byte of the packet */
    BYTE_2_WAIT, /**< @brief Waiting for the second byte of the packet */
    BYTE_3_WAIT  /**< @brief Waiting for the third byte of the packet */
} mouse_packet_state_t;

/**
 * @brief Function to handle mouse interrupts.
 * 
 * This function is called when a mouse interrupt occurs. It processes the mouse data packet
 * and updates the mouse state accordingly.
 */
void (mouse_ih)();

/**
 * @brief Function to subscribe to mouse interrupts.
 * 
 * This function subscribes to mouse interrupts and returns the bit number for the interrupt.
 * 
 * @param bit_no Pointer to a variable to store the bit number for the interrupt.
 * @return 0 on success, non-zero on failure.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Function to unsubscribe from mouse interrupts.
 * 
 * This function unsubscribes from mouse interrupts.
 * 
 * @return 0 on success, non-zero on failure.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Function to process a mouse data packet.
 * 
 * This function processes the mouse data packet and extracts the button states,
 * X/Y displacements, and overflow flags.
 * 
 * @param packet Pointer to the mouse packet structure to be processed.
 * @return 0 on success, non-zero on failure.
 */
int (process_mouse_packet)(struct packet *packet);

/**
 * @brief Function to read mouse data from the input buffer.
 * 
 * This function reads a byte of data from the mouse and stores it in the provided buffer.
 * 
 * @param data Pointer to a variable to store the read data.
 * @return 0 on success, non-zero on failure.
 */
int (read_mouse_data)(uint8_t *data);

int(mouse_write_command)(mouse_commands_t command, uint8_t *command_byte);
int(mouse_enable_data_reporting_costume)();
int(mouse_disable_data_reporting)();


#endif // MOUSE_H

