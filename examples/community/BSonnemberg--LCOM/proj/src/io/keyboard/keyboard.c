#include "keyboard.h"

static int hook_id = KBD_IRQ;

static uint8_t idx = 0;
static uint8_t last_byte = 0x0;

int kbd_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = hook_id;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int kbd_unsubscribe_int() {
  return sys_irqrmpolicy(&hook_id);
}

void (kbc_ih)() {
  if (kbc_read_outbuf(&last_byte) != 0) {
    printf("failed to read output buffer in ih");
    last_byte = 0x0;
    idx = 0;
  }
}

int kbd_parse_scancode(scancode *sc) {

  if (last_byte == 0x0) {
    // invalid data; error
    return 2;
  }

  sc->buf[idx] = last_byte;

  if (idx == 0 && last_byte == 0xE0) {
    // needs 2nd byte
    idx++;
    return 1;
  }

  sc->make = !(sc->buf[idx] & BIT(7));
  sc->size = idx+1;
  idx = 0;

  return 0;
}
