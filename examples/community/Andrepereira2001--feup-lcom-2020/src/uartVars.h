#ifndef _LCOM_UARTVARS_H
#define _LCOM_UARTVARS_H

#include <lcom/lcf.h>

/** @defgroup uartVars
 * @{
 *
 * Constants for programming the universal asynchronous receiver-transmitter. 
 */


#define COM1_REG            0x3F8 /**< @brief COM1 Base Address */
#define COM2_REG            0x2F8 /**< @brief COM2 Base Address */

#define UART_IRQ1           4 /**< @brief COM1 iqr line */
#define UART_IRQ2           3 /**< @brief COM2 iqr line */
//registers
#define RBR_REG             0 /**< @brief Receiver Buffer Register */
#define THR_REG             0 /**< @brief Transmitter Holding Register */
#define IER_REG             1 /**< @brief Interrupt Enable Register */
#define IIR_REG             2 /**< @brief Interrupt Identification Reg */
#define FCR_REG             2 /**< @brief FIFO Control Register */
#define LCR_REG             3 /**< @brief Line Control Register */
#define LSR_REG             5 /**< @brief Line Status Register */


#define LCR_5_BITS_CHAR     0 /**< @brief 5 bits per cha */
#define LCR_6_BITS_CHAR     BIT(0) /**< @brief 6 bits per cha */
#define LCR_7_BITS_CHAR     BIT(1) /**< @brief 7 bits per cha */
#define LCR_8_BITS_CHAR     BIT(0) | BIT(1) /**< @brief 8 bits per cha */
#define LCR_2_STOP_BIT      BIT(2) /**< @brief 2 stop bits */
#define LCR_ODD_PARITY      BIT(3) /**< @brief Odd parity */
#define LCR_EVEN_PARITY     BIT(3) | BIT(4) /**< @brief Even parity */
#define LCR_DLAB            BIT(7) /**< @brief Divisor Latch Access*/

#define LSR_DATA_RECEIVED    BIT(0) /**< @brief Set to 1 when there is data for receiving*/
#define LSR_OVER_ERROR      BIT(1) /**< @brief Set to 1 when a characters received is overwritten by another one*/
#define LSR_PARATY_ERROR    BIT(2) /**< @brief Set to 1 when a character with a parity error is received*/
#define LSR_FRAMING_ERROR   BIT(3) /**< @brief Set to 1 when a received character does not have a valid Stop bit*/
#define LSR_THR_EMPTY       BIT(5) /**< @brief When set, means that the UART is ready to accept a new character for transmitting*/
#define LSR_TRANS_EMPTY     BIT(6) /**< @brief When set, means that both the THR and the Transmitter Shift Register are both empty */
#define LSR_FIFO_ERROR      BIT(7) /**< Set to 1 when there is at least one parity error or framing error or break indication in the FIFO*/

#define IER_EN_RDA_INT      BIT(0) /**< @brief Enables the Received Data Available Interrupt*/
#define IER_EN_THR_INT      BIT(1) /**< @brief Enables the Transmitter Holding Register Empty Interrupt*/
#define IER_EN_RLS_INT      BIT(2) /**< @brief Enables the Receiver Line Status Interrupt*/

#define IIR_NO_INTERRUPT    BIT(0) /**< @brief If set, no interrupt is pending*/ 
#define IIR_P_INT_RLS       BIT(1) | BIT(2) /**< @brief Interrupt pending Receiver Line Status*/
#define IIR_P_R_DATA        BIT(2) /**< @brief Interrupt pending Receive Data Available*/
#define IIR_P_INT_TIME      BIT(2) | BIT(3) /**< @brief Interrupt pending Character Timeout (FIFO)*/
#define IIR_P_INT_THR       BIT(1) /**< @brief Interrupt pending Transmitter Holding Register Empty*/

#define FIFO_ENABLE         BIT(0) /**< @brief Set to 1 to enable both FIFOs*/
#define FIFO_CLEAR_RCVR     BIT(1) /**< @brief Set to 1 to clear all bytes in RCVR(receiving) FIFO*/
#define FIFO_CLEAR_XMIT     BIT(2) /**< @brief Set to 1 to clear all bytes in the XMIT(transmiting) FIFO*/
#define FIFO_1              0 /**< @brief FIFO sending 1 byte*/
#define FIFO_4              BIT(6) /**< @brief FIFO sending 4 byte*/
#define FIFO_8              BIT(7) /**< @brief FIFO sending 8 byte*/
#define FIFO_14             BIT(6) | BIT(7) /**< @brief FIFO sending 14 byte*/


#define UART_LOAD           0xFF /**< @brief UART some one enter the multiplayer*/
#define UART_EXIT           0x00 /**< @brief UART some one exit the multiplayer*/
#define UART_SEED           0xF0 /**< @brief Next byte will be seed number*/

#define BEGIN_KEYS_BYTE     0x55 /**< @brief state of the keys in the beginning of the program*/
#define BITE_A_MAKE         BIT(7) /**< @brief keys_byte A make*/
#define BITE_A_BREAK        BIT(6) /**< @brief keys_byte A break*/
#define BITE_D_MAKE         BIT(5) /**< @brief keys_byte D make*/
#define BITE_D_BREAK        BIT(4) /**< @brief keys_byte D break*/
#define BITE_W_MAKE         BIT(3) /**< @brief keys_byte W make*/
#define BITE_W_BREAK        BIT(2) /**< @brief keys_byte W break*/
#define BITE_S_MAKE         BIT(1) /**< @brief keys_byte S make*/
#define BITE_S_BREAK        BIT(0) /**< @brief keys_byte S break*/

#endif
