#ifndef G6_SERIAL_PORT_H
#define G6_SERIAL_PORT_H

#include <lcom/lcf.h>

/**
 * @brief Sends a data byte via the serial port.
 * @param com_address Address of the serial port.
 * @param data Data to be sent.
 * @return 0 if successful, non-zero otherwise.
 */
int uart_send_data(uint16_t com_address, uint8_t data);

/**
 * @brief Receives a data byte via the serial port.
 * @param com_address Address of the serial port.
 * @param data Address where the data will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int uart_receive_data(uint16_t com_address, uint8_t* data);

/**
 * @brief Configures the serial port (8-bit mode, no parity and disables interrupts).
 * @param com_address Address of the serial port.
 * @return 0 if successful, non-zero otherwise.
 */
int uart_config(uint16_t com_address);

#endif //G6_SERIAL_PORT_H
