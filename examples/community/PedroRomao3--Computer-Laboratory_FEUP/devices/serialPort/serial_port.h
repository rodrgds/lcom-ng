/**
 * @file serial_port.h
 * @brief serial port functions
 * @author pedro
 * @date 20/05/2023
 */
#include <lcom/lcf.h>
#ifndef SERIALPORT_H
#define SERIALPORT_H



#define FIXED_FREQUENCY 115200

#define COM2_ADDR 0x2F8
#define COM1_ADDR 0x3F8
#define COM2_IRQ 3
#define COM1_IRQ 4

//mask for interrupts
#define mask_ser_p BIT(6)

#define SERP_RBR 0
#define SERP_THR 0
#define SERP_IER 1
#define SERP_IIR 2
#define SERP_FCR 2
#define SERP_LCR 3
//#define SERP_MCR 4
#define SERP_LSR 5
//#define SERP_MSR 6
#define SERP_SR  7

#define SERP_DLL 0
#define SERP_DLM 1

//Line Control Register

#define LCR_WORD_LENGTH_6 (BIT(1))
#define LCR_WORD_LENGTH_7 (BIT(0) )
#define LCR_WORD_LENGTH_8 (BIT(0) | BIT(1))
#define LCR_STOP_2 (BIT(2))
#define LCR_PARITY_EVEN (BIT(3) | BIT(4))
#define LCR_BREAK_CONTROL (BIT(6))
#define LCR_DLAB (BIT(7))

//Interrupt Enable Register (IER)


#define IER_ENREC_INT BIT(0)
#define IER_ENT_INT BIT(1)
#define IER_EN_LS_INT BIT(2)
#define IER_EN_MS_INT BIT(3)

//Interrupt Identification Register (IIR)

#define IIR_NO_INT_PENDING BIT(0)
#define IIR_ORIGIN BIT(1) | BIT(2) | BIT(3)
#define IIR_ORIGIN_LINE_STATUS BIT(1) | BIT(2)
#define IIR_ORIGIN_RCVD_DATA_AVL BIT(2)
#define IIR_FIFO_CT BIT(3) | BIT(2)
#define IIR_ORIGIN_T_EMP BIT(1)
#define IIR_INT_PENDING 0x00

//FIFO Control Register (FCR)


#define FCR_EN_1 BIT(0) | BIT(1) | BIT(2) 

#define FCR_TRIGGER04  (1 << 6)
//comandos
#define RDY 0xFE
#define QUIT 0xFF
#define STD 0xFD

//setup uart (lcr)


/**
 @brief Sets the bit rate for the serial communication.
This function sets the bit rate for the serial communication by configuring the Line Control Register (LCR) and writing the divisor values to the Divisor Latch Registers (DLL and DLM) of the COM1 address.
 @param bit_rate The desired bit rate for the serial communication.
 @return 0 indicating successful configuration, 1 in case of an error.
*/
int (set_bit_rate)(uint16_t bit_rate);

/**
 @brief Retrieves the Line Control Register (LCR) value.
This function retrieves the value of the Line Control Register (LCR) by reading the data from the COM1 address offsetted by SERP_LCR.
 @param lcr Pointer to store the LCR value.
 @return 0 indicating successful retrieval, non-zero value in case of an error.
*/
int (get_lcr)(uint8_t *lcr);

/**
 @brief Sets up the Line Control Register (LCR) for serial communication.
This function sets up the Line Control Register (LCR) for serial communication by configuring the word length, stop bits, and parity.
 @param length The desired word length (5, 6, 7, or 8).
 @param stop The desired number of stop bits (1 or 2).
 @return 0 indicating successful configuration, non-zero value in case of an error.
*/
int (setup_lcr)(int length, int stop);


//policy...
/**
 @brief Subscribes to the serial port interrupts.
This function subscribes to the serial port interrupts by setting the interrupt policy for the COM1_IRQ and obtaining the corresponding bit number for interrupt handling.
 @param bit_no Pointer to store the bit number associated with the serial port interrupts.
 @return 0 indicating successful subscription, non-zero value in case of an error.
*/
int (serial_subscribe_int) (uint8_t *bit_no);
//ier

/**
 @brief Retrieves the Interrupt Enable Register (IER) value.
This function retrieves the value of the Interrupt Enable Register (IER) by reading the data from the COM1 address offsetted by SERP_IER.
 @param p Pointer to store the IER value.
 @return 0 indicating successful retrieval, non-zero value in case of an error.
*/
int (get_ier)(uint8_t *p);

/**
 @brief Enables receive interrupts in the Interrupt Enable Register (IER).
This function enables receive interrupts in the Interrupt Enable Register (IER) by setting the corresponding flag.
 @return 0 indicating successful enablement, non-zero value in case of an error.
*/
int (ier_enable_receive)();

/**
 @brief Enables transmit and line status interrupts in the Interrupt Enable Register (IER).
This function enables transmit and line status interrupts in the Interrupt Enable Register (IER) by setting the corresponding flags.
 @return 0 indicating successful enablement, non-zero value in case of an error.
*/
int (ier_enable_transmit_lsr)();


//fcr
/**
 @brief Sets the FIFO Control Register (FCR) value.
This function sets the value of the FIFO Control Register (FCR) by writing the provided value to the COM1 address offsetted by SERP_FCR.
 @param fcr The value to be written to the FCR.
 @return 0 indicating successful setting, non-zero value in case of an error.
*/
int (set_fcr)(uint8_t fcr);

/**
 @brief Undoes the changes made to the serial port configuration.
This function undoes the changes made to the serial port configuration by removing the interrupt policy, resetting the FIFO Control Register (FCR) to its default value, and restoring the Interrupt Enable Register (IER) to its original value.
 @return 0 indicating successful undoing of changes, non-zero value in case of an error.
*/
int (serp_undo)();

/**
 @brief Enables the FIFO in the FIFO Control Register (FCR).
This function enables the FIFO in the FIFO Control Register (FCR) by setting the corresponding flag.
 @return 0 indicating successful enablement, non-zero value in case of an error.
*/
int (fifo_en)();


//send character
/**
 @brief Sends a character over the serial port.
This function sends a character over the serial port by writing it to the COM1_ADDR.
 @param char_send The character to be sent.
 @return 0 indicating successful sending, non-zero value in case of an error.
*/
int(send_char)(uint8_t send_char);

//iir
/**
 @brief Retrieves the Interrupt Identification Register (IIR) value.
This function retrieves the value of the Interrupt Identification Register (IIR) by reading from the COM1 address offsetted by SERP_IIR.
 @param iir Pointer to a variable to store the retrieved IIR value.
 @return 0 indicating successful retrieval, non-zero value in case of an error.
*/
int(get_iir)(uint8_t *iir);

//lsr
/**
 @brief Retrieves the Line Status Register (LSR) value.
This function retrieves the value of the Line Status Register (LSR) by reading from the COM1 address offsetted by SERP_LSR.
 @param lsr Pointer to a variable to store the retrieved LSR value.
 @return 0 indicating successful retrieval, non-zero value in case of an error.
*/
int(get_lsr)(uint8_t *lsr);

#endif
