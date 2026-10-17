#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Subscribes and enables keyboard interrupts
 * 
 * @param bit variable that will store the keyboard hook id
 * @return int 0 if success, 1 otherwise
*/
int (kbd_subscribe_int)(uint8_t *bit);

/**
 * @brief Reads the scancode from the output buffer
 * 
 * @return int 0 if success, 1 otherwise
*/
void (kbc_ih)();

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return 0 if success, 1 otherwise
*/
int (kbd_unsubscribe_int)();

/**
 * @brief Reads the output buffer of the KBC for keyboard information.
 * 
 * @param output pointer to the variable where the scancode will be stored
 * @return 0 if success, 1 otherwise
 */
int (KBC_read_output_keyboard)(uint8_t* output);

/**
 * @brief Writes a command to the KBC
 * 
 * @param cmdByte command to be written
 * @param port port to write the command to
 * @return 0 if success, 1 otherwise
 */
int (KBC_write_cmd)(uint8_t port, uint8_t cmdByte);


#endif
