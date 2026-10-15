#ifndef _LCOM_DRIVER_INPUT_H_
#define _LCOM_DRIVER_INPUT_H_

#include <lcom/lcf.h>
#include "i8042.h"

#define KBC_MAX_TRIES 10

/**
 * @defgroup input Input
 * @ingroup drivers
 * @{
 * 
 * @brief Driver for input devices: keyboard and mouse.
 */

/**
 * @brief Structure for mouse information.
 */
typedef struct packet mouse_info_t;

/**
 * @brief Writes a value to the keyboard controller (KBC).
 *
 * @param port Port where the value will be written.
 * @param input Value to be written.
 * @return 0 if successful, 1 otherwise.
 */
int kbc_write_input(uint8_t port, uint8_t input);

/**
 * @brief Reads value from the keyboard controller (KBC).
 *
 * @param port Port from where the value will be read.
 * @param output Pointer where the read value will be saved.
 * @param mouse Indicates if the value to be read is from the mouse.
 * @return 0 if successful, 1 otherwise.
 */
int kbc_read_output(uint8_t port, uint8_t *output, bool mouse);

/**
 * @brief Writes a command to the keyboard controller (KBC).
 *
 * @param intent Command to be written.
 * @param mouse Indicates if the command is for the mouse.
 * @return 0 if successful, 1 otherwise.
 */
int kbc_write(uint8_t intent, bool mouse);

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer where the bit number signaling the interruption will be saved
 * @return 0 if successful, 1 otherwise.
 */
int kbd_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes interruptions from keyboard.
 * @return 0 if sucessful, 1 otherwise
*/
int kbd_unsubscribe_int();

/**
 * @brief Handles keyboard interruptions.
 */
void (kbc_ih)();

/**
 * @brief Initializes mouse.
 *
 * @param bit_no Pointer where the bit number signaling the interruption will be saved.
 * @return 0 if successful, 1 otherwise.
 */
int mouse_init(uint8_t *bit_no);

/**
 * @brief Disables the mouse.
 *
 * @return 0 if successful, 1 otherwise.
 */
int mouse_disable();

/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer where the bit number signaling the interruption will be saved.
 * @return 0 if successful, 1 otherwise.
 */
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interruptions.
 *
 * @return 0 if successful, 1 otherwise.
 */
int mouse_unsubscribe_int();

/**
 * @brief Handles interruprions from mouse.
 */
void (mouse_ih)();

/**
 * @brief Fills the packet structure with mouse data.
 *
 * @param bytes Raw data of the packet.
 * @param pp Pointer to the packet structure to be filled.
 */
void mouse_fill_packet(uint8_t *bytes, struct packet *pp);

/**
 * @brief Reads a packet of data from the mouse.
 *
 * @return 0 if successful, 1 otherwise.
 */

int mouse_read_packet();

/**
 * @brief Returns the current mouse information.
 *
 * @return Pointer to the structure containing the mouse information.
 */
mouse_info_t *mouse_get_info();

/** @} */

#endif
