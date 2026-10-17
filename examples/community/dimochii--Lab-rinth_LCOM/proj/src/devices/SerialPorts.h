#ifndef __SERIALPORTS_H_
#define __SERIALPORTS_H_

/** @defgroup SerailPorts SerialPorts
 * @{
 *
 * Constants for programming the serial port.
 */

#define IRQ_COM1      4
#define BASE_COM1     0x3F8

#define RBR           0x00        // Receiver Buffer Register             (R)
#define THR           0x00        // Transmitter Holding Register         (W)
#define IER           0x01        // Interrupt Enable Register            (R/W)
#define IIR           0x02        // Interrupt Identification Reg         (R)
#define FCR           0x02        // FIFO Control Register                (W)
#define LCR           0x03        // Line Control Register                (R/W)
#define MCR           0x04        // Modem Control Register               (R/W)
#define LSR           0x05        // Line Status Register                 (R)
#define MSR           0x06        // Modem Status Register                (R)
#define SR	          0x07 	      // Scratchpad Register                  (R/W)

#define DLL	          0x00        // Divisor Latch                        (R/W)                   
#define DLM           0x01        // Divisor Latch                        (R/W)                   

#define LSR_DR        BIT(0)      // Receiver Data
#define LSR_OE        BIT(1)      // Overrun Error
#define LSR_PE        BIT(2)      // Parity Error
#define LSR_FE        BIT(3)      // Framing Error
#define LSR_BI        BIT(4)      // Break Interrupt
#define LSR_THRE      BIT(5)      // Transmitter Holding Register Empty
#define LSR_TER       BIT(6)      // Transmitter Empty Register
#define LSR_F_ERROR   BIT(7)      // FIFO Error

#define IER_ERBFI     BIT(0)      // Enables the Received Data Available Interrupt
#define IER_ETBEI     BIT(1)      // Enables the Transmitter Holding Register Empty Interrupt
#define IER_ELSI      BIT(2)      // Enables the Receiver Line Status Interrupt
#define IER_EMSI      BIT(3)      // Enables the MODEM Status Interrupt

#define IIR_NIP       BIT(0)                            // No interrupt is pending
#define IIR_IP        BIT(1) | BIT(2) | BIT(3)          // Interrupt pending
#define IIR_RLS       BIT(1) | BIT(2)                   // Receiver Line Status
#define IIR_RDA       BIT(2)                            // Reived Data Available
#define IIR_CT        BIT(3) | BIT(2)                   // Character Timeout (FIFO)
#define IIR_THRE      BIT(1)                            // Transmitter Holding Register Empty
#define IIR_MS        0x00                              // Modem Status

#define FCR_CLEAR     BIT(0) | BIT(1) | BIT(2)

#define SYNC1         0x53
#define SYNC2         0x54
#define SYNC3         0x55
#define SYNC4         0x56
#define SYNC5         0x57

#endif
