#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <lcom/lcf.h>

#include "queue.h"
#include "utils.h"



#define COM1 0x3F8
#define COM1_IRQ 4

#define RBR 0x00
#define THR  0x00
#define IER 0x01
#define IIR 0x02
#define FCR 0x02
#define LCR 0x03
#define LSR 0x05


#define DIVISOR_LATCH_LSB 0x00
#define DIVISOR_LATCH_MSB 0x01

#define LCR_8_BITS            (BIT(0) | BIT(1))
#define LCR_ODD_PARITY        BIT(3)
#define LCR_DLAB              BIT(7)

#define LSR_RECEIVER_DATA BIT(0)
#define LSR_OVERRUN_ERROR BIT(1)
#define LSR_PARITY_ERROR BIT(2)
#define LSR_FRAMING_ERROR BIT(3)
#define LSR_BREAK_INTERRUPT BIT(4)
#define LSR_TRANSMITTER_HOLD_EMPTY BIT(5)
#define LSR_TRANSMITTER_EMPTY_REG BIT(6)
#define LSR_FIFO_ERROR BIT(7)

#define IER_RDAI BIT(0)
#define IER_THREI BIT(1)
#define IER_RLSI BIT(2)

#define IIR_PENDING                    BIT(0)
#define IIR_LINE_STATUS                (BIT(2) | BIT(1))
#define IIR_DATA_AVAILABLE             (BIT(2)) 
#define IIR_CHARACTER_TIMEOUT          (BIT(2) | BIT(3)) 
#define IIR_TRANSMIT_HOLD_EMPTY        (BIT(1)) 


#define FCR_ENABLE_FIFO                BIT(0)
#define FCR_CLEAR_RCVR_FIFO            BIT(1)
#define FCR_CLEAR_XMIT_FIFO            BIT(2)


#define TRYSYNC      0x53 
#define FIRSTSYNC    0x54
#define SECONDSYNC   0x55

/**
 * @brief Sends a byte of data to the serial port.
 * 
 * This function sends a byte of data to the serial port by pushing it to the send queue.
 * 
 * @param data The byte of data to send.
 * @return int 0 on success, 1 on failure.
 */

int send_SP_data(uint8_t byte);

/**
 * @brief Subscribes to the serial port interrupts.
 * 
 * This function initializes the serial port and subscribes to its interrupts.
 * 
 * @param bit_no Pointer to store the interrupt bit number.
 * @return int 0 on success, 1 on failure.
 */

int (sp_subscribe)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from the serial port interrupts.
 * 
 * This function ends the serial port operation and unsubscribes from its interrupts.
 * 
 * @return int 0 on success, 1 on failure.
 */

int (sp_unsubscribe)();

/**
 * @brief Establishes a connection.
 * 
 * This function establishes a connection by sending and receiving synchronization bytes.
 * 
 * @return int 1 if connection established, 0 otherwise.
 */

int establish_connection();

/**
 * @brief Interrupt handler for the serial port.
 * 
 * This function handles interrupts for the serial port, managing received data and transmission hold empty interrupts.
 */

void sp_ih();
/**
 * @brief Clears the serial port FIFO.
 * 
 * This function clears the FIFO of the serial port.
 * 
 * @return int 0 on success, 1 on failure.
 */
int sp_clear();

/**
 * @brief Resets the multiplayer state.
 * 
 * This function resets the multiplayer state by clearing the send and receive queues and resetting flags.
 */
void resetMultiplayer();


#endif
