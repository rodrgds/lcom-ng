#pragma once

#include <lcom/lcf.h>
#include <stdio.h>

/** 
 * @defgroup Serial Port Macros
 * @{
 * 
 *  Constants for programming the PC's UART serial port.
 */
#define COM1       0x3F8
#define COM1_IRQ   4

#define UART_RBR                        0
#define UART_THR                        0 
#define UART_LSR                        5 

#define LSR_RD                          BIT(0)
#define LSR_OR_ERROR                    BIT(1)
#define LSR_PAR_ERROR                   BIT(2)
#define LSR_FR_ERROR                    BIT(3) 

#define M_GAME_END  0xff
/**@}*/


/** @brief Serial Port interrupt subscription
 * 
 * @param bitno a pointer to a variable to store the IRQ bit number.
 */
int (subscribe_serialport)(uint8_t *bit_no);



/** @brief Serial port interrupt unsubscription.
 */
int (unsubscribe_serialport)();


/** @brief Serial port interrupt handler.
 * 
 * Reads information from the Recieve Buffer if it is full and if so sets ser_read to true.
 */
void ser_ih();


/** @brief Transmits byte to serial port.
 * 
 * Writes a byte data to the serial port Transmitter Holding Register, even if it is full.
 * 
 * @param data byte to be written to THR.
 */
int serial_transmit_force(uint8_t data);
