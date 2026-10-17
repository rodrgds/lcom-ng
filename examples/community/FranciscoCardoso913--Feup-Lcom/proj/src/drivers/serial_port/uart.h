#ifndef UART_H
#define UART_H

#include <lcom/lcf.h>

#define SER_COM1_IRQ 4
#define SER_COM2_IRQ 3

#define SER_BASE 0x3F8
#define SER_RHR SER_BASE
#define SER_THR SER_BASE
#define SER_DLL SER_BASE
#define SER_DLM (SER_BASE + 1)
#define SER_IER (SER_BASE + 1)
#define SER_IIR (SER_BASE + 2)
#define SER_FCR (SER_BASE + 2)
#define SER_LCR (SER_BASE + 3)
#define SER_LSR (SER_BASE + 5)

#define PAR_ODD 1
#define PAR_EVEN 2
#define PAR_ALWAYS_ODD 3
#define PAR_ALWAYS_EVEN 4
#define LCR_DLAB BIT(7)

#define SER_DATA_RECEIVE_INT BIT(0)
#define SER_EMPTY_TRANSMIT_INT BIT(1)
#define SER_RECEIVER_ERR_INT BIT(2)

#define IIR_INT_STATUS BIT(0)
#define INT_ORIGIN_MASK (BIT(1) | BIT(2) | BIT(3))
#define IIR_TRANSMIT_EMPTY 1
#define IIR_DATA_RECEIVED 2
#define IIR_DATA_ERR 3
#define IIR_CHAR_TIMEOUT 4

#define LSR_RECEIVED_DATA BIT(0)
#define LSR_TRANSMITTER_EMPTY BIT(5)

#define SEND_FIFO_SIZE 16

#define FCR_ENABLE_FIFOS BIT(0)
#define FCR_CLEAR_FIFOS (BIT(1) | BIT(2))
#define FCR_TRIGGER_4 BIT(6)
#define FCR_TRIGGER_8 BIT(7)
#define FCR_TRIGGER_14 (BIT(6) | BIT(7))

#define ACK 0xFA
#define NACK 0xFE

#endif
