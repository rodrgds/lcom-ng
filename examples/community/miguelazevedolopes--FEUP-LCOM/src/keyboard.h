#include <lcom/lcf.h>
#include <minix/sysutil.h>
/** @defgroup keyboard keyboard.h
 * @{
 *
 * This file contains the functions related to the keyboard functioning.
 * 
 */

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p, b) sys_inb(p, b)
#endif

//Subscribes interrupts from the keyboard
int keyboard_subscribe_int(uint8_t *irq_bitmask);

//Unsbscribes the keyboard interrupts
int keyboard_unsubscribe_int();

//Reads the status byte and returns it as argument
int keyboard_read_status(uint8_t *status);

//Reads the data from output buffer and returns it as an argument
int keyboard_read_buffer(uint8_t *data);

//checks the status byte flags and true when it can be read and false when it can't
bool keyboard_check_status();

//Writes a command to the kbc
//args is only used whith the Write Command Byte command
int keyboard_write_command(uint32_t cmd, uint32_t args);
/**@}*/
