#include "keyboard.h"
#include "kbc.h"
#include "i8042.h"
#include <lcom/lcf.h>

static int kbd_hook_id = 1;
static uint8_t scancode = 0;
static bool has_error = false;

uint8_t (kbc_get_scancode)(void) { return scancode; }
bool (kbc_get_error)(void) { return has_error; }

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = (uint8_t) kbd_hook_id;
  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != 0) return 1;
  return 0;
}

int (kbd_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&kbd_hook_id) != 0) return 1;
  return 0;
}

void (kbc_ih)(void) {
  uint8_t temp_scancode;
  if (kbc_read_data_ih(&temp_scancode, KBC_SOURCE_KEYBOARD) != 0) {
    has_error = true;
    return;
  }
  scancode = temp_scancode;
  has_error = false;
}

int (kbc_read_scancode_poll)(void) {
  return kbc_read_data(KBC_OUT_BUF, &scancode, KBC_SOURCE_KEYBOARD);
}

bool (kbc_process_scancode)(bool *is_two_bytes, uint8_t *size, uint8_t bytes[], bool *make, bool *is_esc_pressed) {
  if (scancode == TWO_BYTE_SCANCODE) {
    *is_two_bytes = true;
    bytes[0] = scancode;
    return false;
  }

  if (*is_two_bytes) {
    bytes[1] = scancode;
    *size = 2;
  } else {
    bytes[0] = scancode;
    *size = 1;
  }

  *make = (scancode & BIT(7)) == 0;
  *is_two_bytes = false;

  if (scancode == ESC_BREAKCODE && *size == 1) {
    *is_esc_pressed = true;
  }
  return true;
}

int (kbc_restore_keyboard)(void) {
  uint8_t cmd_byte;

  if (kbc_write_command(KBC_CMD_REG, KBC_READ_CMD) != 0) return 1;
  if (kbc_read_data(KBC_OUT_BUF, &cmd_byte, KBC_SOURCE_KEYBOARD) != 0) return 1;

  cmd_byte = cmd_byte | BIT(0);

  if (kbc_write_command(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return 1;
  if (kbc_write_command(KBC_IN_BUF, cmd_byte) != 0) return 1;

  return 0;
}
