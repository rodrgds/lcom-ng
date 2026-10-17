#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

/** @brief Subscribe keyboard interrupts
  * 
  * returns 0 on success, non-zero otherwise
 */
int (kbc_subscribe_int)(uint8_t *bit_no);

/** @brief Unsubscribe keyboard interrupts
  * 
  * returns 0 on success, non-zero otherwise
 */
int (kbc_unsubscribe_int)();

/** @brief Check if the scancode is done
  * 
  * returns true if the scancode is done, false otherwise
 */
bool (kbc_scancode_is_done)();

/** @brief Get the scancode
  * 
  * @param scancode pointer to the scancode
 */
void (kbc_get_scancode)(uint8_t* scancode);

#endif /* KEYBOARD_H */
