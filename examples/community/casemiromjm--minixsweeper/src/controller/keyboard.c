#include "keyboard.h"

uint8_t scancode, status;

int hook_id = KBD_IRQ;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no ==  NULL) return 1;

  *bit_no = hook_id;

  if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) != 0) {return 1;}

  return 0;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) {return 1;}

  return 0;
}

void (kbc_ih)(){
  if (read_KBC_output(KBC_OUT_BUF, &scancode, 0) != 0) printf("Error reading scancode\n");
}

int (kbd_reenable)() {
  uint8_t command_byte;

  // tells i8042 that i want to read its config
  if (write_KBC_command(KBC_CMD_REG, KBC_READ_CMD) != 0) return 1;

  // read i8042 config
  if (read_KBC_output(KBC_OUT_BUF, &command_byte, 0) != 0) return 1;

  // update command to reactivate interrupts
  command_byte = (command_byte | INT);

  if (write_KBC_command(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return 1;

  if (write_KBC_command(KBC_OUT_BUF, command_byte) != 0) return 1;

  return 0;
}

