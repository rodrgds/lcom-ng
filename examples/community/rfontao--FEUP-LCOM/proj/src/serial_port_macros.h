#ifndef _SERIAL_PORT_MACROS_H_
#define _SERIAL_PORT_MACROS_H_


/** @defgroup Serial_port_macros Serial_port_macros
 *
 * @{
 *
 * Macros for serial port
 */

#define UART_COM1 0x3F8     /**< @brief Serial Port COM1 address */
#define UART_COM1_IRQ 4     /**< @brief Serial Port COM1 IRQ line */
#define UART_COM2_ADR 0x2F8 /**< @brief Serial Port COM2 address */
#define UART_COM2_IRQ 3     /**< @brief Serial Port COM2 IRQ line */

#define UART_RBR 0  /**< @brief Receiver Buffer Register */
#define UART_THR 0  /**< @brief Transmitter Holding Register */
#define UART_IER 1  /**< @brief Interrupt Enable Register */    
#define UART_IIR 2  /**< @brief Interrupt Identification Reg. */      
#define UART_FCR 2  /**< @brief FIFO Control Register */     
#define UART_LCR 3  /**< @brief Line Control Register */       
#define UART_MCR 4  /**< @brief Modem Control Register */       
#define UART_LSR 5  /**< @brief Line Status Register */   
#define UART_MSR 6  /**< @brief Modem Status Register */
#define UART_SR 7   /**< @brief Scratchpad Register */

#define UART_DLL 0  /**< @brief Divisor Latch LSB */
#define UART_DLM 1  /**< @brief Divisor Latch MSB */

#define UART_IER_ENABLE_DATA BIT(0)     /**< @brief Enable the Received Data Available Interrupt */
#define UART_IER_ENABLE_TRANS BIT(1)    /**< @brief Enable the Transmitter Holding Register Empty Interrupt*/
#define UART_IER_ENABLE_RECIEV BIT(2)   /**< @brief Enable the Receiver Line Status Interrupt */
#define UART_IER_ENABLE_MODEM BIT(3)    /**< @brief Enable the MODEM Status Interrupt */

#define UART_IIR_INT_PEND BIT(0)                /**< @brief Interrupt is pending */
#define UART_IIR_INT_TYPE BIT(1)|BIT(2)|BIT(3)  /**< @brief Interrupt type */
#define UART_IIR_RECEIVE_AVAIL BIT(2)           /**< @brief Received data available */
#define UART_IIR_TRANS_HOLD BIT(1)              /**< @brief Transmiter holding register empty */

#define FCR_ENABLE_FIFO BIT(0)          /**< @brief Enable both FIFOs */
#define FCR_CLEAR_REC BIT(1)            /**< @brief Clear all bytes in receiver FIFO */
#define FCR_CLEAR_TRANS BIT(2)          /**< @brief Clear all bytes in transmitter FIFO */
#define FCR_ENABLE_64_BYTE BIT(5)       /**< @brief Enable 64 byte FIFO */
#define FCR_FIFO_TRIGGER_LVL BIT(6)     /**< @brief Receiver FIFO trigger Level */

#define LSR_RECEIVER_DATA BIT(0)    /**< @brief Receiver Data */
#define LSR_OVERRUN_ERROR BIT(1)    /**< @brief Overrun Error */
#define LSR_PARITY_ERROR BIT(2)     /**< @brief Parity Error */
#define LSR_FRAMING_ERROR BIT(3)    /**< @brief Framing Error */
#define LSR_BREAK_INTERRUPT BIT(4)  /**< @brief Break Interrupt */
#define LSR_TRANS_HOLD_EMPTY BIT(5) /**< @brief Transmitter Holding register empty */
#define LSR_TRANSMITTER_EMPTY BIT(6)/**< @brief Transmitter empty register */
#define LSR_FIFO_ERROR BIT(7)       /**< @brief FIFO Error */

#define LCR_BITS_PER_CHAR BIT(0)                /**< @brief Number of bits per char */
#define LCR_NUM_STOP_BITS BIT(2)                /**< @brief Number of stop bits */
#define LCR_PARITY_CONTROL BIT(3)|BIT(4)|BIT(5) /**< @brief Parity Control */
#define LCR_BREAK_CONTROL BIT(6)                /**< @brief Break control */
#define LCR_DLAB BIT(7)                         /**< @brief Divisor Latch Access */


/**@}*/
#endif
