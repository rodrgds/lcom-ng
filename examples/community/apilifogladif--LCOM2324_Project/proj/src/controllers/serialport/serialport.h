#ifndef _LCOM_SERIALPORT_H_
#define _LCOM_SERIALPORT_H_

/**
 * @file serialport.h
 * 
 * @brief This file contains the serial port functions.
*/

/**
 * @brief Subscribes to serial port interrupts.
 * 
 * @param bitno : IRQ.
 * @return int : 0 if sucess and 1 otherwise.
 */
int ser_subscribe_int(uint8_t* bitno);

/**
 * @brief Unsubscribes to serial port interrupts.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int ser_unsubscribe_int();

/**
 * @brief Serial port interrupt handler.
 * 
 * @return uint8_t : received byte.
 */
uint8_t sp_ih();

/**
 * @brief Function that sends a byte through serial port.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int send(uint8_t val);

/**
 * @brief Function that clears serial port interrupts.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int serial_clear();

#endif
