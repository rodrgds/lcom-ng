#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8042_kbc.h"


static int kbd_id = KBD_IRQ1;
uint8_t scan_bytes_kbd[2];
bool codecomplete_kbd = false;
int nrbytes_kbd = 0;


int (kbc_subscribe_int)(uint8_t *bit_no) {
    kbd_id = KBD_IRQ1;
    *bit_no = kbd_id;
    return sys_irqsetpolicy(KBD_IRQ1, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_id);
}

int (kbc_unsubscribe_int)() {
    return sys_irqrmpolicy(&kbd_id);
}

int(kbd_get_status)(uint8_t *status) {
    return util_sys_inb(KBC_ST_REG, status);
}

int (kbd_read_out_buffer)(uint8_t *output) {
    return util_sys_inb(KBC_OUT_BUF, output);
}

bool (kbc_can_read)(uint8_t *st) {
  return !(*st & KBC_ERR_SIG & AUX);
}

int (kbc_read)(uint8_t *data) {
  uint8_t st;

  if (kbd_get_status(&st))
      return PARITY_ERR;

  if (st & OBF) {
    if (!kbc_can_read(&st))
      return TIMEOUT_ERR;

    if (util_sys_inb(KBC_OUT_BUF, data))
      return IBF_ERR;

    return 0;
  }
  return AUX_ERR;
}

void (kbc_ih)() {
  if (codecomplete_kbd == true) {
    codecomplete_kbd = false;
    nrbytes_kbd = 0;
  }

  int err;
  uint8_t scancode;
  if ((err = kbc_read(&scancode)) != 0) {
    printf("There was a problem while reading the scancode. Error code: %d\n", err);
  }

  scan_bytes_kbd[nrbytes_kbd] = scancode;
  nrbytes_kbd++;

  if ((nrbytes_kbd == 2) || (scancode != TWO_BYTE_SCAN)) {
    codecomplete_kbd = true;
  }
}




