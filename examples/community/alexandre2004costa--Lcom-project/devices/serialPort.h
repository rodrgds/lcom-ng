#ifndef _LCOM_serialPort_H_
#define _LCOM_serialPort_H_

#include <lcom/lcf.h>
#include "queue.h"


#define COM1_IRQ 4
#define COM1_ADDRS 0x3f8

// Uart registers address

#define RBR 0 // R Receiver Buffer Register
#define THR 0 // W Transmitter Holding Register
#define IER 1 // R/W Interrupt Enable Register
#define IIR 2 // R Interrupt Identification Register
#define FCR 2 // W FIFO Control Register
#define LCR 3 // R/W Line Control Register
#define MCR 4 // R/W Modem Control Register
#define LSR 5 // R Line Status Register
#define MSR 6 // R Modem Status Register
#define SR 7 // R/W Scratchpad Register

// if bit DLAB of LCR is 1 -> addresses 0 and 1 are overloaded -> we acess :

#define DLL 0 // R/W Divisor Latch LSB
#define DLM 1 // R/W Divisor Latch MSB
///////////////////////////////////////////////
// Line Control Register LCR

// BIT 1,0
#define LCR_BITS_PER_CHAR_5 0
#define LCR_BITS_PER_CHAR_6 1
#define LCR_BITS_PER_CHAR_7 2
#define LCR_BITS_PER_CHAR_8 3
// BIT 5,4,3 (Parity control)
#define LCR_NO_PARITY 0
#define LCR_ODD_PARITY BIT(3)
#define LCR_EVEN_PARITY 2 << 3
#define LCR_BIT1_PARITY 5 << 3
#define LCR_BIT0_PARITY 7 << 3

#define LCR_SET_SERIAL_OUTP_0 BIT(6)
#define LCR_DLAB BIT(7) // Divisor Latch Acess
///////////////////////////////////////////////
// Line Status Register LSR

#define LSR_DATA_FOR_RECEIVING BIT(0)
#define LSR_OVERRUN_ERROR BIT(1)
#define LSR_PARITY_ERROR BIT(2)
#define LSR_FRAMING_ERROR BIT(3)
#define LSR_BREAK_INTERRUPT BIT(4)
#define LSR_THR_EMPTY BIT(5)
#define LSR_THR_TSR_EMPTY BIT(6)
#define LSR_FIFO_ERROR BIT(7)

///////////////////////////////////////////////
// Interrupt Enable Register IER

#define IER_ENABLE_DATA_AVALBL_INT BIT(0)
#define IER_ENABLE_THR_EMPTY_INT BIT(1)
#define IER_ENABLE_RECEIVER_LS_INT BIT(2)

///////////////////////////////////////////////
// Interrupt Identification Register IIR

#define IIR_NO_INTERRUPT BIT(0)
// Interrupt priority
#define IIR_RECEIVER_LINE_STATUS (3 << 1)
#define IIR_CHARACT_TIMEOUT (3 << 2)
#define IIR_RECEIVED_DATA_AVLBL BIT(2)
#define IIR_THR_EMPTY BIT(1)


///////////////////////////////////////////////
// FIFO Control Register FCR

#define ENABLE_BOTH_FIFOS BIT(0)
#define CLEAN_BYTES_RCVR BIT(1)
#define CLEAN_BYTES_XMIT BIT(2)

///////////////////////////////////////////////
// Interrupt Identification Register IIR

#define CHARACTER_TIMEOUT BIT(3)
#define FIFO_64BYTE_ENABLED BIT(5)


int (serialRead)(int address, uint8_t *command);

int (serialWrite)(int address, uint8_t command);

int (setParameters)(int numberBitsPerChar, int parityControl);

int (initialSetup)();

int (enableSerialInterrupts)();

int (serial_subscribe)(uint8_t *bit_no);

int (serial_unsubscribe)();

int (serial_ih)();

int (ableToWrite)();

int (ableToRead)();

int (sendByte)();

int (readByte)();

uint8_t (receiveTop)();

void (addToSend)(uint8_t byte);

int (enableFifos)();

int (configureBitrate)(int bitRate);

#endif
