#ifndef __KBC_H
#define __KBC_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8254.h"

#define MOUSE 2
/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Configures a timer to generate a square wave 
 * 
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param rate Frequency of the square wave to generate
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_rate(unsigned long timer, unsigned long rate);

/**
 * @brief Reads the the mode in which a timer was programmed
 *
 * @param timer Timer whose status to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer status
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_get_status(unsigned long timer, u8_t *st);

/**
 * @brief Shows the mode in which a timer was programmed
 *
 * Displays in a human friendly way, the configuration of the input
 *  timer, by providing the values (and meanings) of the different 
 *  components of a timer status information
 * 
 * @param timer Timer whose status to read (Ranges from 0 to 2)
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_show_status(unsigned long timer);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bitmask for this interrupt, upon success, 0 upon failure
 */
unsigned long timer_subscribe_int(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

unsigned char kbc_read();

int kbc_unsubscribe_int();

unsigned long kbc_subscribe_int(void );

int mouse_unsubscribe_int();

unsigned long mouse_subscribe_int(void );

unsigned char mouse_read();

void kbc_clean_buf();

void mouse_status();

int write_kbc(unsigned long port, unsigned char byte);

void init_kbc();

#endif __KBC_H
