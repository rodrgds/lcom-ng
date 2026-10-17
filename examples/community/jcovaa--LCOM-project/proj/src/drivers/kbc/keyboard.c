#include "keyboard.h"
#include "kbc_utils.h"

int keyboard_hook_id = 1;
uint8_t scancode;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no) {
  /* subscribe the KBC interrupts */
  if (bit_no == NULL) return 1;
  *bit_no = BIT(keyboard_hook_id);
  
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) {
    return 1;
  }
  
  return 0;
}

int (keyboard_unsubscribe_interrupts)() {
  /* unsubscribe the KBC interrupts */
  if (sys_irqrmpolicy(&keyboard_hook_id) != 0)
    return 1;
  
  return 0;
}

void (kbc_ih)() {
  if (read_KBC_output(&scancode, KBC_OUT_BUF, 0) != 0) {
  }
}

int (restore_interrupts)() {
  uint8_t command;
  
  /* Use KBC command 0x20, which must be written to 0x64 */
  if (write_KBC_command(KBC_IN_BUF, KBC_READ_CMD) != 0) return 1;
  /* But the value of the “command byte” must be read from 0x60 */
  if (read_KBC_output(&command, KBC_OUT_BUF, 0) != 0) return 1;

  command |= ENABLE_INTERRUPT;

  /* Use KBC command 0x60, which must be written to 0x64 */
  if (write_KBC_command(KBC_IN_BUF, KBC_WRITE_CMD) != 0) return 1;
  /* But the new value of the “command byte” must be written to 0x60 */
  if (write_KBC_command(KBC_WRITE_CMD, command) != 0) return 1;

  return 0;
}
