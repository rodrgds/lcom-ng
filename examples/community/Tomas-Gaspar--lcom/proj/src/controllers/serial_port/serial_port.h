#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include <stdint.h>

/** @ingroup Serial_Port 
 * @{
 */

/**
 * @brief Subscribes to serial port interrupts.
 *
 * @param bit_no Pointer to store the serial port hook id.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes from serial port interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_unsubscribe_int)();

/**
 * @brief Initializes the serial port.
 *
 * This function initializes the serial port for communication. It configures the line control register,
 * the interrupt enable register and the fifo control register.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_init)();

/**
 * @brief Cleans up the serial port.
 *
 * This function performs cleanup tasks for the serial port. It can be called when serial port
 * communication is no longer needed.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_cleanup)();


/**
 * @brief Retrieves the status byte of the serial port.
 *
 * @param status Pointer to store the status byte of the serial port.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_get_status)(uint8_t* status);

/**
 * @brief Reads data from the serial port.
 *
 * This function reads data from the serial port into the receive queue.
 * The function loops until there is no more data to be read from the serial port.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_read)();

/**
 * @brief Writes data to the serial port.
 *
 * This function writes data from the transmit queue to the serial port.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (sp_write)();

/**
 * @brief Serial port interrupt handler.
 *
 * This function is the interrupt handler for serial port interrupts.
 * Depending on the interrupt received it calls either sp_write() or sp_read().
 */
void (sp_ih)();

/** }@*/

#endif
