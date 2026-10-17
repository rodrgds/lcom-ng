#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"
#include "kbc.h"

int kb_hook_id = KBD_HOOK_BIT;
uint8_t data;

uint8_t scancode[2];
int scancode_size = 1;
bool second_byte = false;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(kb_hook_id);

  if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &kb_hook_id))
    return 1;

  return 0;
}

int(kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&kb_hook_id))
    return 1;
  return 0;
}

// MUDAR ISTO
void(kbc_ih)() {
  uint8_t stat;

  if (util_sys_inb(KBC_STAT_REG, &stat))
    return;

  if (stat & OBF) {
    if (util_sys_inb(KBC_OUT_BUF, &data))
      return;
  }
}

void(process_scancode)() {
  if (data == TWO_BYTES) {
    scancode_size = 2;
    scancode[0] = data;
    second_byte = true;
  }
  else if (second_byte) {
    scancode[1] = data;
    second_byte = false;
  }
  else {
    scancode[0] = data;
  }
}

int(kbd_restore)() {
  uint8_t cmd;

  if (kbc_write_command(KBC_CTRL_REG, READ_COMMAND))
    return 1;

  if (kbc_read_output(KBC_OUT_BUF, &cmd, 0))
    return 1;

  cmd |= BIT(0);

  if (kbc_write_command(KBC_CTRL_REG, WRITE_COMMAND))
    return 1;

  if (kbc_write_command(KBC_IN_BUF, cmd))
    return 1;

  return 0;
}

int(kbd_disable_int)() {
  uint8_t cmd;

  if (kbc_write_command(KBC_CTRL_REG, READ_COMMAND))
    return 1;

  if (kbc_read_output(KBC_OUT_BUF, &cmd, 0))
    return 1;

  cmd = (cmd >> 1) << 1;

  if (kbc_write_command(KBC_CTRL_REG, WRITE_COMMAND))
    return 1;

  if (kbc_write_command(KBC_IN_BUF, cmd))
    return 1;

  return 0;
}
