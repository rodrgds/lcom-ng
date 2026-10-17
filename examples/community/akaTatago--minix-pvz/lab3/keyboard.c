#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

static int kbd_hook_id = 1; 
static uint8_t scancode = 0;
static bool kbc_error = false; 

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = kbd_hook_id;

  return sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}

void (kbc_ih)() {
  uint8_t status;
  kbc_error = false;

  if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
    kbc_error = true;
    return;
  }

  if (status & KBC_OBF) {
    
    if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) {
      kbc_error = true;
      return;
    }

    if ((status & KBC_PAR_ERR) || (status & KBC_TO_ERR)) {
      kbc_error = true; 
    }
  } else {
    kbc_error = true;
  }
}

int (kbc_restore_interrupts)() {
    uint8_t command_byte;

    if (sys_outb(KBC_CMD_REG, KBC_READ_CMD) != 0) return 1;

    if (util_sys_inb(KBC_OUT_BUF, &command_byte) != 0) return 1;

    command_byte |= KBC_INT_KBD;

    if (sys_outb(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return 1;

    if (sys_outb(KBC_OUT_BUF, command_byte) != 0) return 1;

    return 0;
}

uint8_t (kbd_get_scancode)() {
  return scancode;
}

bool (kbd_has_error)() {
  return kbc_error;
}
