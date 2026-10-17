/**
 * This module contains all functions to manage serial port
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight: 4%
 */
#ifndef __SERIAL_H
#define __SERIAL_H

/** @defgroup Serial Serial
 * @{
 * Functions to manage UART
 */

#define COM1_ADDRESS				0x3F8
#define COM1_IRQ						4

//RBR
#define RECEIVER_BUFFER_RBR				0

//THR
#define TRANSMITTER_HOLDING_THR			0

//IER
#define INTERRUPT_ENABLE_IER			1
#define RECEIVED_DATA_INT				0
#define TRANSMITTER_EMPTY_INT			1
#define LINE_STATUS_INT					2

//IIR
#define INTERRUPT_IDENTIFICATION_IIR	2
#define INTERRUPT_STATUS				0
#define INTERRUPT_ORIGIN				1
#define X64_BYTE_FIFO					5
#define FIFO_STATUS						6

//LSR
#define LINE_STATUS_LSR					5

/**
 * @brief Subscribes the serial port interrupts
 *
 * @return Return serial port ID to verify the interrupts and -1 upon failure
 */
int serial_subscribe_int();

/**
 * @brief Unsubscribes the serial port interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int serial_unsubscribe_int();

/**
 * @brief Identifies the origin of the interrupt
 *
 * @return Return a number from 0 to 4 corresponding to the 5 possibles interrupts and -1 upon failure
 */
int serial_interrupt_identification();

/**
 * @brief Reads the char from the serial port buffer
 *
 * @return The char read
 */
char serial_read_char();

/**
 * @brief Writes a char in the serial port buffer
 *
 * @param temp Char to write in the serial buffer
 *
 * @return Return 0 upon success
 */
int serial_write_char(char temp);

/**@}*/

#endif /* __SERIAL_H */
