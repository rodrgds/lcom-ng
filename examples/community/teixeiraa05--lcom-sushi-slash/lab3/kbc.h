#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>
#include <stdbool.h>

int(kbc_subscribe_int)(uint8_t *bit_no);

int(kbc_unsubscribe_int)();
 
uint8_t (kbc_get_scancode)();

uint8_t (kbc_has_error)();

int (kbc_read_status)(uint8_t* status);

int (kbc_read_output)();

int (kbc_read_command)(uint8_t *command);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);

int (kbc_int_enable)();

/**
 * @brief Initializes the keyboard driver by subscribing to its interrupts.
 * 
 * @param bit_no Pointer to store the IRQ bitmask hook ID.
 * @return 0 on success, non-zero on failure.
 */
int keyboard_init(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 * 
 * @return 0 on success, non-zero on failure.
 */
int keyboard_cleanup(void);

/**
 * @brief Project-level keyboard interrupt handler.
 * 
 * Reads KBC status and output registers, processes two-byte extended (E0) scancodes,
 * and updates the internal last-received key state.
 */
void keyboard_ih(void);

/**
 * @brief Checks if a new key has been pressed and retrieves its scancode.
 * 
 * Once retrieved, the new-key flag is reset.
 * 
 * @param scancode Pointer to store the retrieved scancode if a new key exists.
 * @return true if a new key scancode was available and retrieved, false otherwise.
 */
bool keyboard_get_scancode(uint8_t *scancode);

/**
 * @brief Checks if the scancode is make or break.
 * 
 * @return true if the scancode is make, false if break.
 */
bool keyboard_is_scancode_make();

/**
 * @brief Translates the current keyboard scancode to its printable ASCII character.
 * 
 * Highly useful for validating character sequences typed during boss battles.
 * 
 * @return The corresponding uppercase character (A-Z, etc.), or 0 if unprintable or unsupported.
 */
char keyboard_scancode_to_ascii();

#endif // __KBC_H
