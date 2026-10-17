#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "events/events.h"
#include "queue/queue.h"

#define SERIAL_IRQ 4

#define SERIAL_COM1_THR 0x3F8
#define SERIAL_COM1_RBR 0x3F8
#define SERIAL_COM1_IER 0x3F9
#define SERIAL_COM1_IIR 0x3FA
#define SERIAL_COM1_FCR 0x3FA
#define SERIAL_COM1_LCR 0x3FB
#define SERIAL_COM1_LSR 0x3FD
#define SERIAL_COM1_DLAB_LOW_BYTE 0x3F8
#define SERIAL_COM1_DLAB_HIGH_BYTE 0x3F9

#define SERIAL_IIR_INT_PENDING BIT(0)
#define SERIAL_IIR_INT (BIT(3) | BIT(2) | BIT(1))

#define SERIAL_IER_MODEM_STATUS BIT(3)
#define SERIAL_IER_RECEIVER_LINE_STATUS BIT(2)
#define SERIAL_IER_TRANSMITTER_HOLDING_REGISTER_EMPTY BIT(1)
#define SERIAL_IER_RECEIVED_DATA_AVAILABLE BIT(0)

#define SERIAL_LSR_FIFO_ERROR BIT(7)
#define SERIAL_LSR_EMPTY_DATA_HOLDING_REGISTERS BIT(6)
#define SERIAL_LSR_EMPTY_TRANSMITTER_HOLDING_REGISTER BIT(5)
#define SERIAL_LSR_BREAK_INTERRUPT BIT(4)
#define SERIAL_LSR_FRAMING_ERROR BIT(3)
#define SERIAL_LSR_PARITY_ERROR BIT(2)
#define SERIAL_LSR_OVERRUN_ERROR BIT(1)
#define SERIAL_LSR_DATA_READY BIT(0)

#define SERIAL_LCR_ENABLE_DLAB BIT(7)
#define SERIAL_LCR_DISABLE_DLAB (0)

#define SERIAL_FCR_ENABLE_FIFO BIT(0)
#define SERIAL_FCR_CLEAR_RECEIVE BIT(1)
#define SERIAL_FCR_CLEAR_TRANSMIT BIT(2)

#define SERIAL_PACKET_SIZE 6

/**
 * @brief Serial interrupts
 */
typedef enum {
  MODEM_STATUS,
  TRANSMITTER_HOLDING_REGISTER_EMPTY,
  RECEIVED_DATA_AVAILABLE,
  RECEIVER_LINE_STATUS
} serial_interrupt;

typedef union {
  
  struct {

    bool interrupt_pending;
    serial_interrupt interrupt;
    
  } get;

  struct {

    bool modem_status;
    bool transmitter_holding_register_empty;
    bool received_data_available;
    bool receiver_line_status;

  } set;

} serial_interrupts;

typedef enum {
  NO_PARITY,
  ODD_PARITY,
  EVEN_PARITY,
  MARK,
  SPACE
} serial_parity;

typedef enum {
  ONE,
  TWO
} serial_stop_bits;

typedef enum {
  FIVE,
  SIX,
  SEVEN,
  EIGHT
} serial_word_length;

typedef enum {
  HANDSHAKE,
  ACK,
  WORD,
  SUBMISSION
} serial_packet_type;
typedef struct {
  serial_packet_type type;
  uint8_t content[SERIAL_PACKET_SIZE - 1];
} serial_packet;

/**
 * @brief Configures serial port for usage
 * 
 * @return 0 if successful, non-zero otherwise 
 */
int (serial_init)();

/**
 * @brief Frees memory used for serial port operations
 * 
 * @return 0 if successful, non-zero otherwise
 */
int (serial_cleanup)();

/**
 * @brief Sets the baud rate for data transfering
 * 
 */
int (serial_set_baudrate)();

/**
 * @brief Subscribes to serial port interrupts
 * 
 * @return 0 if successful, non-zero otherwise 
 */
int (serial_subscribe_interrupts)();

/**
 * @brief Removes serial port interrupt subscription
 * 
 * @return 0 if successful, non-zero otherwise
 */
int (serial_unsubscribe_interrupts)();

/**
 * @brief Handles interrupts related to the serial port
 * 
 */
void (serial_handle_interrupt)(struct event *ev);

/**
 * @brief Identifies what event set an interrupt
 * 
 * @param interrupts Byte in which each bit represents the origin of an interrupt
 * @return 0 if successful, non-zero otherwise
 */
int (serial_get_interrupts)(serial_interrupts *interrupts);

/**
 * @brief Defines which events set interrupts
 * 
 * @param interrupts The events to set interrupts to
 * @return 0 if successful, non-zero otherwise
 */
int (serial_set_interrupts)(const serial_interrupts *interrupts);

/**
 * @brief Sets word format
 * 
 * @param parity Parity 
 * @param stop_bits Stop bits 
 * @param word_length Length of the word
 * 
 */
int (serial_set_line_pattern)(serial_parity parity, serial_stop_bits stop_bits, serial_word_length word_length);

int (serial_enable_fifo)();

/**
 * @brief Checks changes in Receiver Line Status
 * 
 */
void (serial_handle_line_status)();

/**
 * @brief Handles received data
 * 
 */
void (serial_handle_received_data)();

/**
 * @brief Handler for transmitter holding register empty interrupt
 * 
 */
void (serial_handle_transmitter_holding_register_empty)();

/**
 * @brief Sends a byte
 * 
 * @param byte The byte to be sent
 */
void (serial_send_byte)(uint8_t byte);

/**
 * @brief Sends a packet
 * 
 * @param packet Packet to send
 */
void (serial_send_packet)(serial_packet *packet);
