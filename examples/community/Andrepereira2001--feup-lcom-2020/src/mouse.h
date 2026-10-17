#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

/** @defgroup mouse
 * @{
 *
 * Functions that deal with mouse interruptions manipulation.
 */

/**
 * @brief Subscribes mouse interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return  int 0 upon success and non-zero otherwise 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief  Handles mouse interrutps 
 * 
 */
void (mouse_ih)();

/**
 * @brief Fills the packet with the information received by the mouse
 * 
 * @param pp updated with information in bytes
 * @param bytes bytes read in the mouse interrupt
 */
void (get_packet)(struct packet *pp, uint8_t bytes[]);

/**
 * @brief checks if the inbuffer is full
 * 
 * @return int 0 upon success and non-zero otherwise 
 */
int (check_in_buffer)();

/**
 * @brief reads the acknowledge byte from the output buffer
 * 
 * @param ack acknowledge byte to be read
 * @return int 0 upon success and non-zero otherwise  
 */
int (mouse_read_ack)(uint8_t *ack);

/**
 * @brief writes the given command to the input buffer
 * 
 * @param cmd given command
 * @return int 0 upon success and non-zero otherwise 
 */
int (mouse_write_command)(uint8_t cmd);

/**
 * @brief Enables stream mode data reporting, by sending the respective command to the mouse.
 * 
 * @return int 0 upon success and non-zero otherwise 
 */
int (mouse_enable_data_reporting_me)();

/**
 * @brief Disables stream mode data reporting, by sending the respective command to the mouse.
 * 
 * @return int 0 upon success and non-zero otherwise 
 */
int (mouse_disable_data_reporting)();

