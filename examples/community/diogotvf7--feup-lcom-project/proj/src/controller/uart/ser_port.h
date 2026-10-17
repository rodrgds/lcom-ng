#include <lcom/lcf.h>

#include "global_vars.h"
#include "../utils.h"
#include "SER.h"
#include "../../model/queue.h"

/**
 * @brief Configures the UART with the desired settings.
 *
 * @return Return OK on success, !OK otherwise.
 */
int config_uart();

/**
 * @brief Sets the DLAB (Divisor Latch Access Bit) of the UART.
 *
 * @param status The status of the DLAB (1 - enable, 0 - disable).
 * @return Return OK on success, !OK otherwise.
 */
int set_uart_dlab(uint8_t status);

/**
 * @brief Subscribes to UART interrupts.
 *
 * @param bit_no Address of memory to store the bit number associated with the UART interrupt.
 * @return Return OK on success, !OK otherwise.
 */
int subscribe_uart_int(uint8_t *bit_no);

/**
 * @brief Sets the FCR (FIFO Control Register) of the UART.
 *
 * @param byte The value to set in the FCR.
 * @return Return OK on success, !OK otherwise.
 */
int set_uart_fcr(uint8_t byte);

/**
 * @brief Unsubscribes from UART interrupts.
 *
 * @return Return OK on success, !OK otherwise.
 */
int unsubscribe_uart_int();

/**
 * @brief Sets the IER (Interrupt Enable Register) of the UART.
 *
 * @param byte The value to set in the IER.
 * @return Return OK on success, !OK otherwise.
 */
int set_uart_ier(uint8_t byte);

/**
 * @brief Retrieves the value of the IER (Interrupt Enable Register) of the UART.
 *
 * @param byte Address of memory to store the value of the IER.
 * @return Return OK on success, !OK otherwise.
 */
int get_uart_ier(uint8_t *byte);

/**
 * @brief Sets the LCR (Line Control Register) of the UART.
 *
 * @param byte The value to set in the LCR.
 * @return Return OK on success, !OK otherwise.
 */
int set_uart_lcr(uint8_t byte);

/**
 * @brief Retrieves the value of the LCR (Line Control Register) of the UART.
 *
 * @param byte Address of memory to store the value of the LCR.
 * @return Return OK on success, !OK otherwise.
 */
int get_uart_lcr(uint8_t *byte);

/**
 * @brief Sets the IIR (Interrupt Identification Register) of the UART.
 *
 * @param byte The value to set in the IIR.
 * @return Return OK on success, !OK otherwise.
 */
int set_uart_iir(uint8_t byte);

/**
 * @brief Retrieves the value of the IIR (Interrupt Identification Register) of the UART.
 *
 * @param byte Address of memory to store the value of the IIR.
 * @return Return OK on success, !OK otherwise.
 */
int get_uart_iir(uint8_t *byte);

/**
 * @brief Retrieves the value of the LSR (Line Status Register) of the UART.
 *
 * @param byte Address of memory to store the value of the LSR.
 * @return Return OK on success, !OK otherwise.
 */
int get_uart_lsr(uint8_t *byte);

/**
 * @brief Sets the frequency of the UART.
 *
 * @param freq The desired frequency.
 * @return Return OK on success, !OK otherwise.
 */
int set_uart_freq(uint16_t freq);

/**
 * @brief UART interrupt handler.
 */
void uart_ih();

/**
 * @brief Sends a byte via UART.
 *
 * @param byte The byte to be sent.
 * @return Return OK on success, !OK otherwise.
 */
int send_uart_byte(uint8_t byte);

/**
 * @brief Sends multiple bytes via UART.
 *
 * @param bytes The array of bytes to be sent.
 * @param size The size of the array.
 * @return Return OK on success, !OK otherwise.
 */
int send_uart_bytes(uint8_t *bytes, uint32_t size);

/**
 * @brief Receives a byte from UART.
 *
 * @param byte Address of memory to store the received byte.
 * @return Return OK on success, !OK otherwise.
 */
int receive_uart_byte(uint8_t *byte);

/**
 * @brief Sends a start message via UART.
 *
 * @return Return OK on success, !OK otherwise.
 */
int send_start_msg();
