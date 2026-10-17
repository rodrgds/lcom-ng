#ifndef __KBD_H
#define __KBD_H
/** @defgroup Keyboard KBD

 * @{ Functions for the keyboard driver, used for navigation purposes in the game
 */

static int hook_id_kbd = 1; //!< bit order for keyboard to be used in interupt mask

/**
 * @brief Function thats reads STATS_REG for the keyboard code in the register
 *
 * @return keyboard code in sucess; negative value on failure
 */
int read_keyboard();

/**
 * @brief Function thats reads STATS_REG for the keyboard code in the register
 *
 * @return keyboard code in sucess; negative value on failure
 */
void keyboard_handler();

/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @return bit order in interrupt mask; negative value on failure
 */
int keyboard_subscribe_int();

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe_int();

/**
 * @brief Cleans buffer so that no possible loss of information occurs
 *
 * @return void
 */
void clean_OBF();

#endif /* __KBD_H */
