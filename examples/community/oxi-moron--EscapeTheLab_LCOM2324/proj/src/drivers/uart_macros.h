#ifndef G6_UART_MACROS_H
#define G6_UART_MACROS_H

#define COM1_IRQ 4
#define COM2_IRQ 3

#define COM1 0x3F8
#define COM2 0x2F8

// Register Address Macros
#define LCR 3
#define IER 1
#define THR 0
#define RBR 0
#define LSR 5

#define BITS_PER_CHAR(n) ((n & 0x3) + 5)

// Line Control Register (LCR) macros
#define BPC_8 0x03
#define ODD_PARITY 0x08
#define DLAB_0 0x7F
#define DLAB BIT(7)

// Line Status Register (LSR) macros
#define TRANSMITTER_READY BIT(5)
#define RECEIVER_READY BIT(0)

// Interrupt Enable Register (IER) macros
#define DISABLE_INT 0x8

#define WAIT_UART 20000

#endif //G6_UART_MACROS_H
