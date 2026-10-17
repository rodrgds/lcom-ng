#ifndef __MOUSE_H
#define __MOUSE_H

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

/**
 * @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse
*/

/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Enable or Disable Data Reporting
 * 
 * @param en True if Enable, False if Disable
 * 
 * @return 0 if OK, 1 if NOT_OK
*/
int mouse_data_reporting(bool en);

/**
 * @brief Revert num from 2comp to no sign representation
 * 
 * @param num Number to be reverted
 * 
 * @param p packet which contains byte 0 to check BIT 4 or 5
 * 
 * @param bit 4 or 5 depending on x-delta or y-delta
 * 
 * @return No sign representation of num
 * 
*/
uint16_t rev2comp(uint8_t num, struct packet * p, uint8_t bit);

/**
 * @brief Assembles the packet according to the struct
 * 
 * @param pack Pack that is going to be assembled
 * 
*/
void packet_assemble(struct packet * pack);

/**
 * @brief Mouse interrupt handler
 *        
 * Grab the scancodes from output buffer
 * 
 */
void (mouse_ih)();

/**
 * @brief Write commands to Mouse
 * 
 * @param cmd Command to be written on register
 * 
 * @return 0 if OK, 1 if NOT_OK
 * 
*/
int mouse_write_commands(uint8_t cmd);

/* CONSTANTS */

#define MOUSE_IRQ               12
#define WRITE_MOUSE             0xD4        //Write Byte to Mouse 

//ARGS OF 0xD4
#define DISABLE_REP             0xF5        //In stream mode should be sent before any command
#define ENABLE_REP              0xF4        //In stream mode only 
#define SET_REMOTE_MODE         0xF0        //Send data on request only
#define READ_DATA               0xEB        //Send data packet request
#define SET_STREAM_MODE         0xEA        //Send data on events 

//acknowledgement bytes
#define ACK                     0xFA        //If everything is ok
#define NACK                    0xFE        //If invalid byte
#define ERROR                   0xFC        //If second consecutive invalid byte

#endif
