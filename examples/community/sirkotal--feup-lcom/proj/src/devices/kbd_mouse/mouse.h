#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <lcom/lab4.h> // for int handler otherwise it breaks idk why

static int packet_index = 0;
static struct packet mouse_packet;

#define FIRST_BYTE BIT(3)

/**
 * @brief Subscribes and enables Mouse interrupts
 * 
 * @param bit_no Bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_int_handler)();

/**
 * @brief Write command to mouse
 * 
 * @param command Command to be written
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_write_command)(uint8_t command);

/**
 * @brief Generate mouse packet
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (generate_mouse_packet)();

/**
 * @brief Get current packet index
 * 
 * @return Return current packet index
 */
int (get_current_packet_index)();

/**
 * @brief Get mouse packet
 * 
 * @return Return mouse packet
 */
struct packet (get_mouse_packet)();

#endif // _LCOM_MOUSE_H_
