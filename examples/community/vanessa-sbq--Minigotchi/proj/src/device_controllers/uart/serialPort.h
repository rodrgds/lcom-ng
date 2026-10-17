#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#define SERIAL_PORT 0x3F8
#define SERIAL_IRQ_1 4                              //COM1 IRQ line

#define RECEIVE_BUFFER 0x00                         //RBR Receiver Buffer Register
#define TRANSMIT_HOLD 0x00                          //THR Transmitter Holding Register
#define INTERRUPT_ENABLE 0x01                       //IER Interrupt Enable Register
#define INTERRUPT_ID 0x02                           //IIR Interrupt Identification Reg.
#define FIFO_CONTROL 0x02                           //FCR FIFO Control Register
#define LINE_CONTROL 0x03                           //LCR Line Control Register
#define MODEM_CONTROL 0x04                          //MCR Modem Control Register
#define LINE_STATUS 0x05                            //LSR Line Status Register 
#define MODEM_STATUS 0x06                           //MSR Modem Status Register
#define SCRATCH_REG 0x07                            //SR Scratchpad Register    (Read/Write)

//*Addresses 0 and 1 are overloaded, accessing different registers if bit DLAB of the LCR register is set to 1:

#define DIV_LATCH_LSB SERIAL_PORT 0x00            // DLL Divisor Latch LSB
#define DIV_LATCH_MSB SERIAL_PORT 0x01            // DLM Divisor Latch MSB


//Line Status Register (LSR)(Read Only)
#define LSR_DATA_READY BIT(0)
#define LSR_OVERRUN_ERROR BIT(1)
#define LSR_PARITY_ERROR BIT(2)
#define LSR_FRAMING_ERROR BIT(3)
#define LSR_BREAK_INTERRUPT BIT(4)
#define LSR_THR_EMPTY BIT(5)
#define LSR_TSR_EMPTY BIT(6)
#define LSR_FIFO_ERROR BIT(7)

#define IER_DATA_AVAIL_INT BIT(0)
#define IER_THR_EMPTY_INT BIT(1)
#define IER_LINE_STATUS_INT BIT(2)
#define IER_MODEM_STATUS_INT BIT(3)

#define IIR_NO_INTERRUPT BIT(0)
#define IIR_INTERRUPT_ID BIT(1) | BIT(2) | BIT(3)
#define IIR_LINE_STATUS BIT(1) | BIT(2)
#define IIR_DATA_AVAILABLE BIT(2)
#define IIR_CHAR_TIMEOUT BIT(3) | BIT(2)
#define IIR_THR_EMPTY BIT(1)
#define IIR_MODEM_STATUS 0x00

#define FCR_CLEAR BIT(0) | BIT(1) | BIT(2)

#endif
