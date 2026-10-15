#ifndef _LCOM_DRIVER_SERIAL_PORT_H_
#define _LCOM_DRIVER_SERIAL_PORT_H_

#include <lcom/lcf.h>
#include "iUART.h"
#include "../../data_structures/queue/queue.h"

/** 
 * @defgroup serial_port Serial Port (UART)
 * @ingroup drivers
 * @{
 * 
 * @brief Driver for a UART (Universal Asynchronous Receiver/Transmitter) device.
 */

/**
 * @brief Subscribes the program to interruptions from COM1.
 * 
 * @param bit_no Pointer where the bit number signaling the interruption will be saved
 * @return 0 is successful, 1 otherwise
*/
int (uart_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes interruptions from COM1.
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int (uart_unsubscribe_int)();

/**
 * @brief Reads the content from a specific offset of the uart base address.
 * 
 * @param base_addr Base address of the UART device
 * @param offset Offset for the register to read
 * @param out Pointer to where the result will be saved
 * @return 0 if successful, 1 otherwise
*/
int (uart_read)(int base_addr, uint8_t offset, uint8_t *out);

/**
 * @brief Writes a content to a specific offset of the uart base address.
 * 
 * @param base_addr Base address of the UART device
 * @param offset Offset for the register to write
 * @param in Content to be written
 * @return 0 if successful, 1 otherwise
*/
int (uart_write)(int base_addr, uint8_t offset, uint8_t in);

/**
 * @brief Activates/Deactivates Data Latch access.
 * 
 * @param base_addr Base address of the UART device
 * @param enable true to enable the data latch access, false to disable
 * @return 0 if successful, 1 otherwise
*/
int	(uart_DL_access)(int base_addr, int enable);

/**
 * @brief Sets the transmission bit rate between devices.
 * 
 * @param base_addr Base address of the UART device
 * @param rate Transmission rate
 * @return 0 if successful, 1 otherwise
*/
int	(uart_set_bit_rate)(int base_addr, int rate);

/**
 * @brief Reads a byte from the UART FIFO.
 * 
 * @param base_addr Base address of the UART device
 * @return 0 if successful, 1 otherwise
*/
int (uart_fifo_read)(int base_addr);

/**
 * @brief Writes a byte to the UART FIFO.
 * 
 * @param base_addr Base address of the UART device
 * @return 0 if successful, 1 otherwise
*/
int (uart_fifo_write)(int base_addr);

/**
 * @brief Adds a byte to the transmitter queue.
 * 
 * @note This function does not directly interact with the device.
 * @note The data "sent" here is added to a queue, which content will
 * be sent one by one whenever a Transmitter Holding Register Empty interrupt is received.
 * 
 * @param byte Information to send
 * @return 0 if successful, 1 otherwise
*/
int (uart_send_byte)(uint8_t byte);

/**
 * @brief Get a byte from the receiver queue.
 * 
 * @note This function does not directly interact with the device.
 * @note All received data is read from the FIFO and stored in a queue 
 * whenever an Receiver Data Available or Character Timeout interrupts are received.
 * 
 * @param byte Pointer to where the byte will be saved.
 * @return 0 if successful, 1 otherwise
*/
int (uart_get_byte)(uint8_t *byte);

/**
 * @brief Interrupt handler.
 * 
 * @return 1 if a message was received, 0 for all other cases
*/
int (uart_ih)();

/**
 * @brief Setups the device and activates interrupts.
 * 
 * @details Default configuration is the following:
 * - 8 bit character, 1 stop without parity messages.
 * - FIFO enabled, with receiver and transmitter clear on init, 
 * and a FIFO receiver trigger level of 1
 * - Transmitter holding, line status and data available interruptions are turned on.
 * 
 * @warning If the system does not have an active serial port, this function will still return 0.
 * @warning The variable uart_disabled variable can be used to check if the module is enabled (0 if yes).
 * 
 * @param bit_no Pointer where the bit number signaling the interruption will be saved
 * @param bit_rate Data transmission bit rate
 * @return 0 is successful, 1 otherwise
*/
int (uart_init)(uint8_t *bit_no, int bit_rate);

/**
 * @brief Deletes allocated memory by the UART device driver and unsubscribes interrupts.
 * 
 * @return 0 is successful, 1 otherwise
*/
int (uart_disable)();

/** @} */

#endif
