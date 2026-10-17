#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#define DELAY_US 20000 // Tick delay

#define PARITY_ERR   1
#define TIME_OUT_ERR 2
#define IBF_FULL     3
#define AUX_DATA     4

extern uint8_t scan_code[2];
extern int kdb_i;

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * This function subscribes to keyboard interrupts and enables notifications.
 *
 * @param bit_no Pointer to a variable where the interrupt bit number will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * This function unsubscribes from keyboard interrupts and disables notifications.
 *
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_unsubscribe_int)(void);

/**
 * @brief Checks the status of the keyboard controller.
 *
 * This function checks the status register of the keyboard controller to verify 
 * specific status conditions.
 *
 * @param st The status byte to be checked.
 * @return Returns 0 if the status is as expected, or a non-zero error code otherwise.
 */
int (check_status)(uint8_t st);

/**
 * @brief Gets the status of the keyboard controller.
 *
 * This function reads the status register of the keyboard controller.
 *
 * @param st Pointer to a variable where the status byte will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_get_status)(uint8_t *st);

/**
 * @brief Reads the output buffer of the keyboard.
 *
 * This function reads a byte from the output buffer of the keyboard.
 *
 * @param output Pointer to a variable where the output byte will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_read_out_buffer)(uint8_t *output);

/**
 * @brief Writes a command byte to a specified port.
 *
 * This function writes a command byte to the specified port of the keyboard controller.
 *
 * @param port The port to which the command byte will be written.
 * @param cmdb The command byte to be written.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_write_cmdb)(int port, uint8_t cmdb);

/**
 * @brief Reads a command byte from the keyboard controller.
 *
 * This function reads a command byte that was previously sent to the keyboard controller.
 *
 * @param data Pointer to a variable where the read byte will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_read_ret_cmdb)(uint8_t *data);

/**
 * @brief Re-enables keyboard interrupts.
 *
 * This function re-enables keyboard interrupts that were previously disabled.
 *
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (kbd_reenable_ints)(void);

/**
 * @brief Keyboard interrupt handler.
 *
 * This function is the interrupt handler for keyboard interrupts. It processes the 
 * keyboard input received during the interrupt.
 */
void (kbc_ih)(void);


#endif // KEYBOARD_H
