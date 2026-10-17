#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "serial_port_macros.h"

/** @defgroup Serial_Port Serial_Port
 *
 * @{
 *
 * Constants and functions of the serial port
 */

#define KBD_CODE 'K'    /**< @brief Code to identify group of keyboard scancodes*/
#define MOUSE_CODE 'M'  /**< @brief Code to identify group of mouse scancodes*/


/**
 * @brief Subscribes serial port interrrupts
 * 
 * @param bit_no        Bit number of the IRQ line
 * @return              1 on failure
 */
int serial_port_subscribe_int(int *bit_no);

/**
 * @brief Unsubscribes serial port interrrupts
 * 
 * @return              1 on failure
 */
int serial_port_unsubscribe_int();


/**
 * @brief Disables data transmission with serial port
 * 
 * @return              1 on failure
 */
int serial_port_disable_data();

/**
 * @brief Serial port interrupt handler
 * 
 * @return              1 on failure
 */
void serial_port_ih();

/**
 * @brief Sends the send queue data through the serial port
 * 
 * @return              1 on failure
 */
int serial_port_send();

/**
 * @brief Receives data from the serial port
 * 
 * @return              1 on failure
 */
int serial_port_rec();

/**
 * @brief Writes data to the send queue to be sent to the serial port
 * 
 * @return              1 on failure
 */
void serial_port_write(uint8_t data);

/**
 * @brief Enables serial port to transmit and receive data with FIFOs
 * 
 * @return              1 on failure
 */
int serial_port_enable();

/**
 * @brief Processes the received data from the serial port
 * 
 * @return              1 on failure
 */
void serial_port_interp(uint8_t data);

/**
 * @brief Clears transmission FIFO
 * 
 * @return              1 on failure
 */
void clear_trans();

/**
 * @brief Clears reception FIFO
 * 
 * @return              1 on failure
 */
void clear_rec();

/**@}*/

#endif
