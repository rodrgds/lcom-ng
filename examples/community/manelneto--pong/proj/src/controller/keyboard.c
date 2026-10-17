#include <lcom/lcf.h>

#include "keyboard.h"

scancode code;
int32_t keyboard_hook_id = KEYBOARD_IRQ;

int(keyboard_subscribe_int)(uint8_t *bit_no) {
  *bit_no = keyboard_hook_id;

  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id)) {
    printf("%s: sys_irqsetpolicy(KEYBOARD_IRQ: %d, IRQ_REENABLE | IRQ_EXCLUSIVE, keyboard_hook_id: %d) error\n", __func__, KEYBOARD_IRQ, keyboard_hook_id);
    return 1;
  }

  return 0;
}

int(keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&keyboard_hook_id)) {
    printf("%s: sys_irqrmpolicy(keyboard_hook_id: %d) error\n", __func__, keyboard_hook_id);
    return 1;
  }
  return 0;
}

int(keyboard_read_scancode_byte()) {
  uint8_t output;
  int r = kbc_read_output(&output);
  if (r == -1) {
    keyboard_restore();
    printf("%s: kbc_read_output(output: 0x%x) error (returned %d)\n", __func__, output, r);
    return 1;
  }
  else if (r == 2) {
    if (keyboard_enable_interrupts()) {
      printf("%s: keyboard_enable_interrupts() error\n", __func__);
      return 1;
    }
    printf("%s: kbc_read_output(output: 0x%x) error (returned %d)\n", __func__, output, r);
    return 1;
  }

  if (output == KBD_FIRST_OF_TWO_BYTES) {
    code.size = 0;
    code.bytes[0] = output;
  }
  else if (code.bytes[0] != KBD_FIRST_OF_TWO_BYTES) {
    code.size = 1;
    code.bytes[0] = output;
  }
  else {
    code.size = 2;
    code.bytes[1] = output;
  }

  code.makecode = (bool) !(output & KBD_BREAKCODE);
  return 0;
}

void(kbc_ih)() {
  if (keyboard_read_scancode_byte())
    printf("%s: keyboard_read_scancode_byte() error\n", __func__);
}

void(keyboard_restore()) {
  code.size = 0;
  code.bytes[0] = 0;
  code.bytes[1] = 0;
}

int(keyboard_enable_interrupts)() {
  int r;
  if ((r = kbc_write_command(KBC_READ_CMD))) {
    printf("%s: kbc_write_command(KBC_READ_CMD: %d) error (returned %d)\n", __func__, KBC_READ_CMD, r);
    return 1;
  }

  uint8_t command_byte;
  if ((r = kbc_read_output(&command_byte))) {
    printf("%s: kbc_read_output(command_byte: 0x%x) error (returned %d)\n", __func__, command_byte, r);
    return 1;
  }

  if ((r = kbc_write_command(KBC_WRITE_CMD))) {
    printf("%s: kbc_write_command(KBC_WRITE_CMD: %d) error (returned %d)\n", __func__, KBC_WRITE_CMD, r);
    return 1;
  }

  if ((r = kbc_write_argument(command_byte | KBC_INT))) {
    printf("%s: kbc_write_argument(command_byte | KBC_INT: 0x%x) error (returned %d)\n", __func__, command_byte | KBC_INT, r);
    return 1;
  }

  return 0;
}
