#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "kb_controller.h"

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads the scan code from the keyboard output buffer and stores it in a global variable.
 */
void (kbc_ih)();

/**
 * @brief Subscribes keyboard interrupts.
 *
 * Sets up the interrupt policy for the keyboard and enables exclusive access to IRQ1.
 *
 * @param bit_no Pointer to a variable to store the bit mask of the hook ID.
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_subscribe_interruptions)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * Removes the interrupt policy previously set for the keyboard.
 *
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_unsubscribe_interruptions)();

/**
 * @brief Restores the keyboard to its default state.
 *
 * Reads the current command byte, enables interrupts (bit 0), and writes it back.
 *
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_restore)();

/**
 * @brief Reads from a port and increments a counter.
 *
 * Wrapper around util_sys_inb that increments a global counter each time it is called.
 *
 * @param port The port to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, non-zero on failure.
 */
int util_sys_inb_counter(int port, uint8_t *value);

#endif // _LCOM_KBD_H_
