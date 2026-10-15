#ifndef UART_H
#define UART_H

#include <lcom/lcf.h>

/*Uart adresses*/
#define RBR 0
#define THR 0
#define DLL 0
#define DLM 1
#define IER 1
#define IIR 2
#define LCR 3
#define MCR 4
#define LSR 5
#define MSR 6
#define SR 7

/*Line Control Register (LCR)*/

//Number of bits per char
#define B5_CHAR 0
#define B6_CHAR BIT(0)
#define B7_CHAR BIT(1)
#define B8_CHAR (BIT(0) | BIT(1))

//stop bits
#define STOP_B1 0
#define STOP_B2 BIT(2)

//Parity control
#define NO_PARITY 0
#define ODD_PARITY BIT(3)
#define EVEN_PAROTY (BIT(3) | BIT(4))
#define PARITY_B1 (BIT(5) | BIT(3))
#define PARITY_B0 (BIT(5) | BIT(4) | BIT(3))

//divisor Latch
#define DLAB BIT(7)

/*Line status reguster (LSR)*/
#define RECEIVER_DATA BIT(0)
#define OVERRUN_ERROR BIT(1)
#define PARITY_ERROR BIT(2)
#define FRAMING_ERROR BIT(3)
#define BREAK_INTERRUPT BIT(4)
#define HOLDING_REGISTER_EMPTY BIT(5)
#define EMPTY_REGISTER BIT(6)
#define FIFO_ERROR BIT(7)

/*Interrupt enable register (IER)*/
#define EN_REG_DATA_AVAILABLE BIT(0)
#define EN_TRANS_HOLD_REG_EMPTY BIT(1)
#define EN_RECEIVE_LINE_STATUS BIT(2)
#define EN_MODEM_STATUS BIT(3)

/*Interrupt Identification Register (IIR)*/
#define INTERRUPT_PENDING BIT(0)

/*fifo control register (FCR)*/
#define EN_BOTH_FIFOS BIT(0)
#define CLEAR_BYTES_RCVR BIT(1)
#define CLEAR_BYTES_XMIT BIT(2)
#define EN_64_BYTE BIT(5)


#endif