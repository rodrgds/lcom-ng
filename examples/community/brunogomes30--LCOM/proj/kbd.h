#ifndef KBD_H
#define KBD_H
#define DELAY_US 20000
#include <stdint.h>

/** @defgroup keyboard 
 * @{
 *
 * Functions related to keyboard 
 */

static int kbd_hook_id;

/**
 * @brief Subscribe keyboard interrupt
 * 
 * @param bit_no 
 * @return int 
 */
int kbd_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribe keyboard interrupt
 * 
 * @return int 
 */
int kbd_unsubscribe_int();

/**
 * @brief Reads current keycode byte
 * 
 * @return uint32_t 
 */
uint32_t kbd_read();

/**
 * @brief Returns keycode of the key pressed
 * 
 * @return uint32_t 
 */
uint32_t kbd_read_code();


/**
 * @brief Read command byte
 * 
 * @param cmd 
 * @return int 
 */
int read_cmd_byte(uint32_t *cmd);

/**
 * @brief Write command byte
 * 
 * @param cmd 
 * @return int 
 */
int write_cmd_byte(uint32_t *cmd);

/**
 * @brief Returns true if its a break code
 * 
 * @param keycode 
 * @return uint8_t 
 */
uint8_t is_break_code(uint32_t keycode);
#endif




