#ifndef _LCOM_IUART_H_
#define _LCOM_IUART_H_

#include <lcom/lcf.h>

/** @defgroup serial_port Serial Port (UART) */

/** 
 * @defgroup serial_port_iUART iUART 
 * @ingroup serial_port
 * @{
 * 
 * @brief Constants for the UART device.
 */

#define UART_MAX_BIT_RATE 115200 /**< @brief Max bit rate supported by MINIX */
#define UART_QUEUE_SIZE 50 /**< @brief Maximum capacity for the transmitter and reciever queues */

#define UART_COM1 0x3F8 /**< @brief COM1 base address */
#define UART_COM1_VECTOR 0x0C /**< @brief COM1 vector */
#define IRQ_COM1 4 /**< @brief COM1 IRQ line */

#define UART_COM2 0x2F8 /**< @brief COM2 base address */
#define UART_COM2_VECTOR 0x0B /**< @brief COM2 vector */
#define IRQ_COM2 3 /**< @brief COM2 IRQ line */

/* Accessible Registers */

#define UART_RBR 0x00 /**< @brief R - Receiver Buffer register */
#define UART_THR 0x00 /**< @brief W - Transmitter Holding register */
#define UART_IER 0x01 /**< @brief R/W - Interrupt Enable register */
#define UART_IIR 0x02 /**< @brief R - Interrupt Identification register */
#define UART_FCR 0x02 /**< @brief W - FIFO Control register */
#define UART_LCR 0x03 /**< @brief R/W - Line Control register */
#define UART_LSR 0x05 /**< @brief R - Line Status register */

/* Divisor Latches */

#define UART_DLL 0x00 /**< @brief R/W - Divisor Latch LSB @details Enabled if DLAB of LCR is set to 1*/
#define UART_DLM 0x01 /**< @brief R/W - Divisor Latch MSB @details Enabled if DLAB of LCR is set to 1 */

/* LCR Command Format */

/* LCR - Bits per char */

#define UART_LCR_5BITCHAR 				0 /**< @brief LCR - 5 bits per char */
#define UART_LCR_6BITCHAR 				BIT(0) /**< @brief LCR - 6 bits per char */
#define UART_LCR_7BITCHAR 				BIT(1) /**< @brief LCR - 7 bits per char */
#define UART_LCR_8BITCHAR 				(BIT(0) | BIT(1)) /**< @brief LCR - 8 bits per char */

/* LCR - Stop bits */

#define UART_LCR_1STOP 						0 /**< @brief LCR - 1 stop bit */ 
#define UART_LCR_2STOP 						BIT(2) /**< @brief LCR - 2 stop bits (1 and 1/2 when 5 bits char) */

/* LCR - Parity control */

#define UART_LCR_PARITY_NONE 			0 /**< @brief LCR - No parity */
#define UART_LCR_PARITY_ODD 			BIT(3) /**< @brief LCR - Odd parity */
#define UART_LCR_PARTIY_EVEN 			(BIT(3) | BIT(4)) /**< @brief LCR - Even parity */
#define UART_LCR_PARITY_1 				(BIT(3) | BIT(5)) /**< @brief LCR - Parity is 1 */
#define UART_LCR_PARITY_0 				(BIT(3) | BIT(4) | BIT(5)) /**< @brief LCR - Parity is 0 */

#define UART_LCR_DLAB 						BIT(7) /**< @brief LCR - Divisor latch access */

/* LSR Response Format */

#define UART_LSR_HAS_DATA 				BIT(0) /**< @brief LSR - Data for receiving exists */
#define UART_LSR_ERR_OVERRUN 			BIT(1) /**< @brief LSR - Overrun error */
#define UART_LSR_ERR_PARITY 			BIT(2) /**< @brief LSR - Parity error */
#define UART_LSR_ERR_FRAMING 			BIT(3) /**< @brief LSR -  Framing error */
#define UART_LSR_ERR							(BIT(1) | BIT(2) | BIT(3)) /**< @brief LSR - Error mask */
#define UART_LSR_BREAK_INTERRUPT 	BIT(4) /**< @brief LSR - Break Interrupt */
#define UART_LSR_EMPTY_HOLD 			BIT(5) /**< @brief LSR - Transmitter holding register empty */
#define UART_LSR_ALL_EMPTY 				BIT(6) /**< @brief LSR - Transmitter empty register */
#define UART_LSR_ERR_FIFO 				BIT(7) /**< @brief LSR - FIFO error */

/* IER Command Format */

#define UART_IER_ENABLE_INT_DATA 	BIT(0) /**< @brief IER - Enable Received Data Available interrupt */
#define UART_IER_ENABLE_INT_THR	 	BIT(1) /**< @brief IER - Enables Transmitter Holding Register Empty interrupt */
#define UART_IER_ENABLE_INT_LSR 	BIT(2) /**< @brief IER - Enables the Receiver Line Status interrupt */
#define UART_IER_ENABLE_INT_MODEM BIT(3) /**< @brief IER - Enables the MODEM Status interrupt */

/* IIR Response Format */

#define UART_IIR_NO_INTS					BIT(0) /**< @brief IIR - No pending interruptions */

#define UART_IIR_INT_MASK					(BIT(1) | BIT(2) | BIT(3)) /**< @brief IIR - Interruption mask */
#define UART_IIR_INT_THR_EMPTY		BIT(1) /**< @brief IIR - Transmitter Holding Register Empty interrupt */
#define UART_IIR_INT_RBR_FULL 		BIT(2) /**< @brief IIR - Received Data Available interrupt */
#define UART_IIR_INT_CHAR_TIMEOUT	(BIT(2) | BIT(3)) /**< @brief IIR - Character Timeout (FIFO) interrupt */
#define UART_IIR_INT_LSR_STATUS		(BIT(1) | BIT(2)) /**< @brief IIR - Receiver Line Status interrupt */

#define UART_IIR_64BITFIFO				BIT(5) /**< @brief IIR - 64-byte FIFO enabled */
#define UART_IIR_FIFO_UNUSABLE		BIT(7) /**< @brief IIR - FIFO unusable */
#define UART_IIR_FIFO_ENABLED			(BIT(6) | BIT(7)) /**< @brief IIR - FIFO enabled */

/* IIR Control Format */

#define UART_FCR_FIFO_ENABLE			BIT(0) /**< @brief IIR - Enable FIFO */
#define UART_FCR_FIFO_RCVR_CLEAR 	BIT(1) /**< @brief IIR - Clear all bytes in receiver FIFO */
#define UART_FCR_FIFO_XMIT_CLEAR	BIT(2) /**< @brief IIR - Clear all bytes in transmiter FIFO */
#define UART_FCR_6EBITFIFO_ENABLE BIT(5) /**< @brief IIR - Enable 64 bit FIFO */
#define UART_FCR_FIFO_RCVR_1BYTE 	0 /**< @brief IIR - FIFO receiver trigger level 1 */
#define UART_FCR_FIFO_RCVR_4BYTE  BIT(6) /**< @brief IIR - FIFO receiver trigger level 4 */
#define UART_FCR_FIFO_RCVR_8BYTE	BIT(7) /**< @brief IIR - FIFO receiver trigger level 8 */
#define UART_FCR_FIFO_RCVR_14BYTE BIT(6) | BIT(7) /**< @brief IIR - FIFO receiver trigger level 14 */

/** @} */

#endif /* _LCOM_IUART_H */
