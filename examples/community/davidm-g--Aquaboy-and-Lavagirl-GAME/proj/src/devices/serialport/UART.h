#ifndef _LCOM_UART_H
#define _LCOM_UART_H

#define COM1 0x3F8
#define COM1_IRQ 4
#define COM1_IRQ_MASK BIT(COM1_IRQ)

/* REGISTERS */

#define RBR 0x00
#define THR 0x00
#define IER 0x01
#define IIR 0x02
#define FCR 0x02
#define LCR 0x03
#define MCR 0x04
#define LSR 0x05
#define MSR 0x06
#define SR 0x07
#define DLL 0x00
#define DLM 0x01

#define SER_NO_INT_PEND BIT(0)
#define INT_ID (BIT(1) | BIT(2) | BIT(3))
#define SER_RX_ERR (BIT(1) | BIT(2) | BIT(3))
#define SER_FIFO_INT (BIT(2) | BIT(3))

#define RECEIVE_DATA_INT BIT(0)
#define SEND_DATA_INT BIT(1)

#define SERIAL_CLEAR 255

#endif
