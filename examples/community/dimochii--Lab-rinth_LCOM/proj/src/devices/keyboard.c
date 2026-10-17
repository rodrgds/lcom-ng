#include "keyboard.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the keyboard.
 */

uint8_t k_scancode;
int hook_id_keyboard = 1;

/**
 * @brief Subscribes keyboard interrupts.
 * This function subscribes keyboard interrupts by having the correct IRQ policy.
 * It updates the irq_set with the bitmask for the keyboard interrupt line.
 * @param irq_set Pointer to the variable that will store the IRQ bitmask.
 * @return 0 if success and 1 if error.
 */
int (keyboard_subscribe_int)(uint8_t *irq_set) {
  if (irq_set == NULL)
    return 1;

  *irq_set = BIT(hook_id_keyboard);
  
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard) != 0)
    return 1;

  return 0;
}


/**
 * @brief Unsubscribes keyboard interrupts.
 * This function unsubscribes keyboard interrupts by removing the IRQ policy that was set.
 * @return 0 if success and 1 if error.
 */
int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_keyboard) != 0)
    return 1;
  
  return 0;
}

/**
 * @brief Keyboard interrupt handler.
 * This function is the interrupt handler for keyboard interrupts.
 * It reads the scancode from the output buffer of the keyboard controller.
 */
void (kbc_ih)() {
    if (read_kbc(OUT_BUF, &k_scancode, 0) != 0) 
        printf("Read scancode error\n");
}


/**
 * @brief Restores keyboard interrupts.
 * This function restores keyboard interrupts by changing the command byte
 * of the keyboard controller to enable interrupts.
 * @return 0 if success and 1 if error.
 */
int (restore_interrupts)() {
    if (write_kbc(KBC_CMD, READ_COMM_BYTE) != 0)
        return 1;

    uint8_t command_byte;
    if (read_kbc(OUT_BUF, &command_byte, 0) != 0)
        return 1;

    command_byte = command_byte | ENABLE_INT_KEYBOARD;

    if (write_kbc(KBC_CMD, WRITE_COMM_BYTE) != 0)
        return 1;

    if (write_kbc(IN_BUF, command_byte) != 0)
        return 1;

    return 0;
}
