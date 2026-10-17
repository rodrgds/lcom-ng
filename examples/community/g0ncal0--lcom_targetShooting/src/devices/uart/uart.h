#ifndef _LCOM_UART_H_
#define _LCOM_UART_H_

#include <lcom/lcf.h>
#include "queue/queue.h"
#include "../kbc/i8042.h"
#include "../kbc/mouse.h"

/* Base addresses for the COM ports */
#define SER_COM1 0x3F8
#define SER_COM1_IRQ 4

#define SER_COM2 0x2F8
#define SER_COM2_IRQ 3

/* The UART Registers */
#define SER_RBR 0 /** @brief Receiver Buffer */
#define SER_THR 0 /** @brief Transmitter Holding */
#define SER_IER 1 /** @brief Interrupt Enable */ 
#define SER_IIR 2 /** @brief Interrupt Identification Register */
#define SER_FCR 2 /** @brief FIFO Control */
#define SER_LCR 3 /** @brief Line Control */
#define SER_MCR 4 /** @brief Modem Control */
#define SER_LSR 5 /** @brief Line Status */
#define SER_MSR 6 /** @brief Modem Status */
#define SER_DLL 0 /** @brief Divisor Latch Least Significant Byte */
#define SER_DLM 1 /** @brief Divisor Latch Most Significant Byte */

/* LCR - Line Control Register (Read/Write Register) */
#define SER_LCR_WORD_LENGTH (BIT(1) | BIT(0)) /** @brief Bits per character */
#define SER_LCR_WORD_SIZE(N) ((N) - 5) /** @brief Select word (character) length */

#define SER_LCR_STOP_BIT BIT(2) /** @brief Length of the stop bit */
#define SER_LCR_STOP_1 0 /** @brief One stop bit */
#define SER_LCR_STOP_2 1 /** @brief Two stop bits */

#define SER_LCR_PAR (BIT(5) | BIT(4) | BIT(3)) /** @brief Parity Select */
#define SER_LCR_PAR_NONE 0 /** @brief No parity */
#define SER_LCR_PAR_ODD 1 /** @brief Odd parity */
#define SER_LCR_PAR_EVEN 3 /** @brief Even parity */

#define SER_LCR_DLAB BIT(7) /** @brief DLAB: Divisor Latch Access Bit */

#define SER_MAX_FREQ 115200 /** @brief Maximum frequency */

/** @brief Line Control Register */
union lcr {
  uint8_t value;
  struct {
    uint8_t word_length: 2;
    uint8_t stop_bits: 1;
    uint8_t parity: 3;
    uint8_t set_break_en: 1;
    uint8_t dlab: 1;
  };
} typedef lcr_t;

/** @brief Parity - excluding sticky (high and low) parity */
typedef enum { 
  NO_PARITY,
  ODD_PARITY, 
  EVEN_PARITY 
} par_t;

/** @brief Configuration settings */
typedef struct {
    unsigned int bit_rate;
    unsigned int no_bits;
    par_t parity;
    uint8_t stop_bits;
} conf_t;

/* LSR - Line Status Register (Read Only Register) */
#define SER_LSR_RX_RDY BIT(0) /** @brief Receiver Ready */
#define SER_LSR_OVERRUN_ERR BIT(1) /** @brief Overrun Error */
#define SER_LSR_PAR_ERR BIT(2) /** @brief Parity Error */
#define SER_LSR_FR_ERR BIT(3) /** @brief Framing Error */
#define SER_LSR_TX_RDY BIT(5) /** @brief Trasmitter ready */

/* MCR - Modem Control Register (Read/Write Register) */
#define SER_MCR_OUT2 BIT(3) /** @brief Aux Output 2 */
#define SER_MCR_LOOPBACK BIT(4) /** @brief LoopBack Mode 2 */

/* IER - Interrupt Enable Register (Read/Write Register) */
#define SER_IER_RX_INT BIT(0) /** @brief Enable Received Data Interrupt */
#define SER_IER_TX_INT BIT(1) /** @brief Enable Transmitter Empty Interrupt */
#define SER_IER_LINE_ST BIT(2) /** @brief Enable Receiver Line Status Interrupt */

/* IIR - Interrupt Identification Register (Read Only Register) */
#define SER_IIR_INT_NP BIT(0) /** @brief No Interrupt Pending */
#define SER_IIR_INT_ID (BIT(3) | BIT(2) | BIT(1)) /** @brief Interrupt Origin */
#define SER_IIR_TX_INT BIT(1) /** @brief Transmitter Empty*/
#define SER_IIR_RX_INT BIT(2) /** @brief Received Data Available */
#define SER_IIR_TO_INT BIT(3) /** @brief Time Out */
#define SER_IIR_LINE_ST (BIT(2) | BIT(1)) /** @brief Line Status */
#define SER_IIR_FIFO_64 BIT(6) /** @brief 64 Byte FIFO Enabled */
#define SER_IIR_FIFO_ST (BIT(7) | BIT(6)) /** @brief FIFO status */
#define SER_IIR_NO_FIFO 0x00 /** @brief No FIFO */
#define SER_IIR_FIFO_EN BIT(7) /** @brief FIFO Enabled */
#define SER_IIR_FIFO_UN (BIT(6) | BIT(7)) /** @brief Enable but Unusable  */

/* FCR - FIFO Control Register (Write Only Register) */
#define SER_FCR_EN_FIFO BIT(0) /** @brief Enable FIFO */
#define SER_FCR_CLR_RX_FIFO BIT(1) /** @brief Clear Receive FIFO */
#define SER_FCR_CLR_TX_FIFO BIT(2) /** @brief Clear Transmit FIFO */
#define SER_FCR_EN_64_FIFO BIT(5) /** @brief Clear Transmit FIFO */
#define SER_FCR_TRIGGER_LVL (BIT(7) | BIT(6)) /** @brief Interrupt Trigger Level */
#define SER_FCR_TRIGGER_1 0x00 /** @brief Interrupt Trigger Level 1 byte */
#define SER_FCR_TRIGGER_4 BIT(6) /** @brief Interrupt Trigger Level 4 bytes */
#define SER_FCR_TRIGGER_8 BIT(7) /** @brief Interrupt Trigger Level 8 bytes */
#define SER_FCR_TRIGGER_14 (BIT(7) | BIT(6)) /** @brief Interrupt Trigger Level 14 bytes */


/**
 * @brief Gets the transmit queue.
 *
 * @return Pointer to the transmit queue.
 */
queue_t *(ser_get_tx_queue)();

/**
 * @brief Gets the receive queue.
 *
 * @return Pointer to the receive queue.
 */
queue_t *(ser_get_rx_queue)();

/**
 * @brief Gets if the player 2 info is done.
 *
 * @return true if player2 info is done, false otherwise.
 */
bool(ser_get_player2_info_is_done)();


/**
 * @brief Gets the player 2 info.
 *
 * @param pp Pointer to the player 2 info.
 * @return OK on success, non-zero otherwise.
 */
int(ser_get_player2_info)(player2_info_t *pp);

/**
 * @brief Gets if the player 2 is ready to play.
 *
 * @return true if player2 is ready, false otherwise.
 */
bool (ser_get_player2_ready)();

/**
 * @brief Handler to multiplayer start.
 *
 * @return OK on success, non-zero otherwise.
 */
int(ser_handle_start)();

/**
 * @brief Reads data from the receive queue.
 *
 * @return OK on success, non-zero otherwise.
 */
int(ser_read_data_from_rx_queue)();

/**
 * @brief Sends player 2 info to the transmit queue.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param target index of target shot, -1 if no target was shot.
 * @param score Score.
 * @return OK on success, non-zero otherwise.
 */
int(ser_send_player2_info_to_txqueue)(int16_t x, int16_t y, int8_t target, uint16_t score);

/**
 * @brief Sends waiting to the transmit queue.
 *
 * @return OK on success, non-zero otherwise.
 */
int(ser_send_waiting_to_txqueue)();

/**
 * @brief Sets the player 2 info as value of the parameter.
 *
 * @param done true if player2 info is done, false otherwise.
 */
void(ser_set_player2_ready)(bool ready);

/**
 * @brief Subscribes serial port interrupts.
 *
 * @param bit_no Pointer to the bit number to be set.
 * @return OK on success, non-zero otherwise.
 */
int (ser_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes serial port interrupts.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_unsubscribe_int)();

/**
 * @brief Writes a value to a serial port register.
 *
 * @param reg Register offset.
 * @param data Data to write.
 * @return OK on success, non-zero otherwise.
 */
int (ser_write_reg)(uint8_t reg, uint8_t data);

/**
 * @brief Reads a value from a serial port register.
 *
 * @param reg Register offset.
 * @param data Pointer to store the read data.
 * @return OK on success, non-zero otherwise.
 */
int (ser_read_reg)(uint8_t reg, uint8_t *data);

/**
 * @brief Configures the serial port with specified settings.
 *
 * @param addr Base address of the serial port.
 * @param config Configuration settings.
 * @return OK on success, non-zero otherwise.
 */
int (ser_conf)(unsigned short addr, conf_t config);

/**
 * @brief Sets the bit rate of the serial port.
 *
 * @param bit_rate Bit rate to set.
 * @return OK on success, non-zero otherwise.
 */
int (ser_set_bit_rate)(unsigned int bit_rate);

/**
 * @brief Configures the line control register.
 *
 * @param config Pointer to the configuration settings.
 * @return OK on success, non-zero otherwise.
 */
int (ser_set_line_ctrl)(conf_t *config);

/**
 * @brief Enables the FIFO buffers of the serial port.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_enable_fifo)();

/**
 * @brief Disables the FIFO buffers of the serial port.
 * 
 * @return OK on success, non-zero otherwise.

*/
int (ser_disable_fifo)();

/**
 * @brief Enables serial port interrupts.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_enable_int)();

/**
 * @brief Disables serial port interrupts.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_disable_int)();

/**
 * @brief Sends a byte through the serial port.
 *
 * @param b Byte to send.
 * @return OK on success, non-zero otherwise.
 */
int (ser_send_byte)(uint8_t b);

/**
 * @brief Receives a byte from the serial port.
 *
 * @param b Pointer to store the received byte.
 * @return OK on success, non-zero otherwise.
 */
int (ser_receive_byte)(uint8_t *b);

/**
 * @brief Sends data from the transmit queue.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_send_data)();

/**
 * @brief Receives data into the receive queue.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_receive_data)();

/**
 * @brief Interrupt handler for the serial port.
 */
void (ser_ih)();

/**
 * @brief Cleans up the serial port, destroying queues.
 *
 * @return OK on success, non-zero otherwise.
 */
int (ser_exit)();


/**
 * @brief Resets the serial port queues.
 * 
 * @return OK on success, non-zero otherwise.
*/
int ser_reset_queues();

#endif /* _LCOM_UART_H */
