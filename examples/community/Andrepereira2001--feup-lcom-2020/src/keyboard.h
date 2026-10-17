#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

/** @defgroup keyboard
 * @{
 *
 * Functions that deal with keyboard interruptions manipulation.
 */

/**
 * @brief Subscribes keyboard interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int 0 upon success and non-zero otherwise
 */
int (kbc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief  Unsubscribes keyboard interrupts
 * 
 * @return int 0 upon success and non-zero otherwise
 */
int (kbc_unsubscribe_int)();

/**
 * @brief Handles keyboard interrutps
 * 
 */
void (kbc_ih)();

/**
 * @brief writes the command given to the input buffer
 * 
 * @param cmd is written to the command register
 * @param command is written to the input buffer
 * @return int 0 upon success and non-zero otherwise 
 */
int (kbc_write_command)(uint8_t cmd,uint8_t command);

/**
 * @brief reads the command from the output buffer
 * 
 * @param cmd is written to the command register
 * @param command is read from the output buffer
 * @return int 0 upon success and non-zero otherwise
 */
int (kbc_read_command)(uint8_t cmd,uint8_t * command);

/**
 * @brief gets the information read from the output buffer when full
 * 
 * @return int 0 upon success and non-zero otherwise
 */
int (kbc_get_value)();
