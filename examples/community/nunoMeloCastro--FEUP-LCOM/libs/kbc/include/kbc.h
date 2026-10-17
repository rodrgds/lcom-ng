#ifndef _KBC_H
#define _KBC_H

#include <stdint.h>

int hook_id;
uint8_t code;

/**
 * @brief Subscribes keyboard interrupts
 * 
 * @param bit_no pointer to bit number
 * 
 * @return Returns 0 if success and 1 if otherwise
 */
int kbc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard/mouse interrupts
 * 
 * @return Returns 0 if success and 1 if otherwise
 */
int kbc_unsubscribe_int(void);

/**
 * @brief subscribes mouse interrupts
 * 
 * @param bit_no pointer to bit number
 * 
 * @return Returns 0 if success and 1 if otherwise
 */
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief keyboard interrupt handler
 * 
 * @return Returns 0 if success and 1 if otherwise
 */
int kbc_int_handler(void);

/**
 * @brief Issue a command to kbc
 * 
 * @param cmd Command to be issued
 * 
 * @return Return 0 if success and 1 otherwise
 */
int kbc_issue_cmd(uint8_t cmd);

/**
 * @brief Read a byte from kbc
 * 
 * @param byte pointer to byte
 * 
 * @return Return 0 if success and 1 otherwise
 */
int kbc_read_data(uint8_t *byte);

/**
 * @brief Issue an argument to kbc
 * 
 * @param cmd argument to be issued
 * 
 * @return Return 0 if success and 1 otherwise
 */
int kbc_issue_arg(uint8_t arg);

#endif /* _KBC_H */
