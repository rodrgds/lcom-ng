#include "../utils.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Keyboard interrupt handler.
 */
void (kbc_ih)();

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer to the variable that will store the keyboard interrupt bit mask.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Checks if the output buffer can be read.
 *
 * @return Return 0 if the output buffer can be read, non-zero otherwise.
 */
int (can_read_outbuf)();

/**
 * @brief Writes a command byte to the keyboard controller.
 *
 * @param port The port to write the command byte to.
 * @param cmdbyte The command byte to write.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (write_to_kbc)(uint8_t port, uint8_t cmdbyte);

/**
 * @brief Reads a value/data from the keyboard controller.
 *
 * @param port The port to read from.
 * @param output Pointer to the variable that will store the read value/data.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (read_value_data_from_kbc)(uint8_t port, uint8_t *output);

/**
 * @brief Restores the interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (restore_interrupts)();

/**
 * @brief Waits for the ESC key to be pressed.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int wait_esc_key();

