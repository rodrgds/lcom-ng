/**
 * @file keyboard.h
 * @brief This file contains the necessary functions to process keyboard interrupts and sending of commands to the KBC
 * @date 2024-06-02
 *
 */


#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
/**
 * @brief Subscribes keyboard interrupts using irq_reenable and irq_exclusive policy
 * 
 * @param bit_no hook id bit position of keyboard
 * @return 0 upon success, 1 otherwise 
 */
int (kbd_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return 0 upon success, 1 otherwise
 * 
 */
int (kbd_unsubscribe_int)();
/**
 * @brief Verifies if output buffer is full
 * @return 0 if so, 1 otherwise
 * 
 */
int (verify_obf)(uint8_t status_reg);
/**
 * @brief Verifies if there was a timeout error
 * 
 * @param status_reg register to be analysed
 * @return 1 if there's an error, 0 otherwise
 * 
 */
int (verify_timeout)(uint8_t status_reg);
/**
 * @brief Verifies if there was a parity error in the status register
 * 
 * @param status_reg register to be analysed
 * @return 1 if there's an error, 0 otherwise
 * 
 */
int (verify_parity)(uint8_t status_reg);
/**
 * @brief Verifies if a scan code is a make code
 * 
 * @param scancode scan code to be analysed
 * @return true if so, false otherwise
 */
bool (is_make)(uint8_t scancode);
/**
 * @brief Check if a byte is the first or second one of the scan code
 * 
 * @param scancode bye to be checked
 * @return 1 if it's the first byte, 2 if it's the second
 * 
 */
uint8_t (scan_n_bytes)(uint8_t scancode);
/**
 * @brief Reads status register to check for erros and if OBF is full, then a scan code is read and placed in code
 * 
 * @param code scancode content
 * @return 0 upon success, 1 otherwise
 * 
 */
int (read_keyboard_buffer)(uint8_t *code);
/**
 * @brief Enables keyboard interrupts
 * @return 0 upon success, 1 otherwise
 * 
 */
int (enable_interrupts)();

#endif
