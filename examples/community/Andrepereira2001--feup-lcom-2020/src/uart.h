#pragma once

/** @defgroup uart
 * @{
 *
 * Functions that deal with universal asynchronous receiver-transmitter  interruptions manipulation.
 */

/**
 * @brief subscribes COM1 interrupts
 * 
 * @param bit_no
 * @return int 
 */
int uart_subscribe_int(uint8_t *bit_no);

/**
 * @brief unsubscribes COM1 interrupts
 * 
 * @return int 
 */
int uart_unsubscribe_int();

/**
 * @brief checks if there is a byte pending to be read in the receiver buffer register
 * 
 * @return true 
 * @return false 
 */
bool check_LSR_byte_received();

/**
 * @brief reads byte from the receiver buffer register
 * 
 * @param data
 */
void read_RBR_byte(uint8_t *data);

/**
 * @brief checks is transmitter holding register is empty (valid to write)
 * 
 * @return true 
 * @return false 
 */
bool check_LSR_THR_empty();

/**
 * @brief write byte to transmitter holding register
 * 
 * @param data
 */
void send_THR_byte(uint8_t data);

/**
 * @brief config com1 with 8 bits per char, odd parity, 1 stop bit 
 * 
 * @return int 
 */
int config_uart1();


