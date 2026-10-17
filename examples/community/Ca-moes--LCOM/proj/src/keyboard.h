#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <i8254.h>

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the i8042
 */

/**
 * @brief Enumerated type for identifying the KBC Status Register state fields
 */
enum kbc_state_field {
  ksf_all,     /*!< state byte */
  ksf_parity,   /*!< Parity Error*/
  ksf_timeout, /*!< Timeout Error */
  ksf_aux,    /*!< Aux mode */
  ksf_inh,    /*!< Inhibit Flag */
  ksf_a2,    /*!< command/data byte */
  ksf_sys,    /*!< System Flag */
  ksf_ibf,    /*!< IN_BUF Full */
  ksf_obf     /*!< OUT_BUF Full */
};

/**
 * @brief Subscribes and enables KBC's interrupts
 * 
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes KBC's interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_unsubscribe_int)();

/**
 * @brief reads the specified field of the status register
 * 
 * @param state a byte read from the status register with information about the KBC state
 * @param field one of the status register's fields
 * @return Return the value of the specified field (0/1)- bit; or the full state byte
 */
uint8_t kbc_read_state_field(const uint8_t state, enum kbc_state_field field);

/**
 * @brief Read Command from the Output Buffer - in Lab3 reads return value from a KBC command
 * 
 * @param cmd address of 8-bit variable to be update with the value read from the Output Buffer -   *        (the value that the KBC command returns)
 * @return Return 0 upon success and no-zero otherwise
 */
int kbc_read_command_return(uint8_t *cmd);

/**
 * @brief Write Command Arguments to register 0x60, after checking the IBF flag
 * 
 * @param cmdArg the argument for the last issued kbc command
 * @return Return 0 upon success and no-zero otherwise
 */
int kbc_write_command_args(uint8_t cmdArg);

/**
 * @brief Issues KBC's read and write commands, after checking the IBF flag
 * 
 * @param state address of 8bit-variable to be updated with the status register's byte
 * @param cmd the KBC read or write command to be issued
 * @return Return 0 upon no errors and no-zero otherwise
 */
int kbc_issue_command(uint8_t *state, uint8_t cmd);

#endif /* __KEYBOARD_H */
