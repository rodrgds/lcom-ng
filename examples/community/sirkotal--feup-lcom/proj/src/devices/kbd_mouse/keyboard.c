#include "i8042.h"
#include "kbc.h"
#include "keyboard.h"

int kbd_hook_id = 1;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = kbd_hook_id;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}

int (kbd_int_handler)() {
  return kbd_read_scancode(&scancode);
}

int (kbd_enable_int)() {
  uint8_t command_byte;

  if (kbc_write_command(KBC_CMD_REG, KBC_CMD_READ)) {
    return 1;
  }

  if (kbc_read_output(KBC_OUT_BUF, &command_byte, false)) {
    return 1;
  }

  command_byte |= KBC_CMD_KBD_INT;

  if (kbc_write_command(KBC_CMD_REG, KBC_CMD_WRITE)) {
    return 1;
  }

  if (kbc_write_command(KBC_IN_BUF, command_byte)) {
    return 1;
  }

  return 0;
}

int (kbd_read_scancode)(uint8_t *scancode) {
  return kbc_read_output(KBC_OUT_BUF, scancode, false);
}

uint8_t (get_scancode)() {
  return scancode;
}

void (kbc_ih)(void) {
  kbd_int_handler();
}
