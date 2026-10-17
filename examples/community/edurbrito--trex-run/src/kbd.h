#ifndef __KBD_H
#define __KBD_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the keyboard (KBD)
 */

/**
 * @brief Subscribes and enables KBD interrupts
 *
 * @param bit_no address of memory to be initialized with 
 * the bit number to be set in the mask returned upon an interrupt.
 * 
 * @return 0 if OK, 1 if NOT_OK
 */
int (kbd_subscribe_int)(uint8_t *bit_no);


/**
 * @brief Unsubscribes KBD interrupts
 *
 * @return 0 if OK, 1 if NOT_OK
 */
int (kbd_unsubscribe_int)();


/**
 * @brief verifies status by the second param
 * 
 * @param status Keyboard status to get verified 
 * @param bit_check bit of the status to be verified 
 * 
 * @return 0 or 1 depending on input - check constants below for more info
*/
int (kbd_reading_status)(uint8_t status,uint8_t bit_check);


/**
 * @brief KBD interrupt handler
 *        
 *        Grab the scancodes from output buffer
 * 
 */
void (kbc_ih)();


/**
 * @brief write commands to KBD registers
 * 
 * @param reg register to write commands on
 * @param cmd command to be written on register
 * 
 * @return 0 if OK, 1 if NOT_OK
*/
int (kbd_write_commands)(int reg, uint8_t cmd); 


/**
 * @brief reenables KBD interrupts
 * 
 * @return 0 if OK, 1 if NOT_OK
*/
int (kbd_reenable)();


/**
 * @brief tests if input is makecode or breakcode
 * 
 * @param scancode to be tested
 * 
 * @return TRUE if makecode, FALSE if breakcode
*/
bool (kbd_make_code)(uint8_t scancode);


/* CONSTANTS */

#define KBD_IRQ                 1

#define OUT_BUF                 0x60    // OUTPUT BUFFER 
#define IN_BUF                  0x64    // INPUT BUFFER

#define STAT_REG                0x64    // STATUS REGISTER
#define KBC_CMD_REG             0x64    // COMMAND REGISTER

/* RELATED KBC COMMANDS */

#define READ_CMD                0x20    // READ COMMAND BYTE
#define WRITE_CMD               0x60    // WRITE COMMAND BYTE

#define ESC_BREAK_CODE          0x81    // BREAK CODE - ESC KEY
#define S_BREAK_CODE            0x9F    // BREAK CODE - S KEY
#define P_BREAK_CODE            0x99    // BREAK CODE - P KEY
#define M_BREAK_CODE            0xB2    // BREAK CODE - M KEY
#define SPACE_BREAK_CODE        0x39    // BREAK CODE - SPACE KEY
#define DOWN_BREAK_CODE         0x50    // BREAK CODE - DOWN KEY

#define TWO_BYTES_CODE          0xe0    // TO VERIFY IF THE CODE IS 2 BYTES

#define DELAY                   20000   // DELAY USED IN DELAY FUNCTION


/* STATUS BITS */

#define _OBF                    0
#define _IBF                    1
#define _SYS                    2
#define _A2                     3
#define _INH                    4
#define _AUX                    5
#define _TOUT                   6
#define _PAR                    7


/* OTHER CONSTANTS */

#define NO_ERROR                0
#define PARITY_ERROR            1
#define TIMEOUT_ERROR           1
#define BIT_CHECKED             1
#define BIT_NOT_CHECKED         0
#define INITIALIZED             1
#define RESET_SYS               0
#define COMMAND_BYTE            1
#define DATA_BYTE               0
#define INHIBITED               1
#define MOUSE_DATA              1
#define NO_MOUSE_DATA           0
#define FULL_NOT_WRITE          1
#define CAN_WRITE               0

#endif
