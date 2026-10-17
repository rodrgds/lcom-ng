#include "../utils.h"
#include "UART.h"
#include "queue.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Subscribes the serial port interrupts.
 *
 * @param bit_no Pointer to the variable that will store the bit mask of the subscribed interrupt.
 * @return Return 0 upon success, non-zero otherwise.
 */
int ser_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes the serial port interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int ser_unsubscribe_int();

/**
 * @brief Retrieves the status of the serial port.
 *
 * @param st Pointer to the variable that will store the status.
 * @return Return 0 upon success, non-zero otherwise.
 */
int ser_get_status(uint8_t *st);

/**
 * @brief Initializes the serial port.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int ser_init();

/**
 * @brief Exits the serial port.
 */
void ser_exit();

/**
 * @brief Interrupt handler for the serial port.
 */
void ser_ih();

/**
 * @brief Sends a byte through the serial port.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int ser_send_byte();

/**
 * @brief Reads a byte from the serial port.
 *
 * @return Return the byte read upon success, -1 otherwise.
 */
int ser_read_byte();
