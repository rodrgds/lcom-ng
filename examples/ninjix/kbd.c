#include "kbd.h"

static uint8_t sc_buf[2];
static uint8_t sc_size = 0;
static bool sc_ready = false;
static bool sc_make = false;

int kbd_process_byte(uint8_t byte) {
  if (sc_ready) sc_size = 0;
  sc_ready = false;
  if (sc_size == 0 && byte == KBD_TWO_BYTE_PFX) {
    sc_buf[0] = byte;
    sc_size = 1;
    return 0;
  }
  if (sc_size == 1) {
    sc_buf[1] = byte;
    sc_size = 2;
    sc_make = !(byte & KBD_BREAK_BIT);
  } else {
    sc_buf[0] = byte;
    sc_size = 1;
    sc_make = !(byte & KBD_BREAK_BIT);
  }
  sc_ready = true;
  return 1;
}

int kbd_get_scancode(bool *make, uint8_t *size, uint8_t bytes[2]) {
  if (!sc_ready || make == NULL || size == NULL || bytes == NULL) return 1;
  *make = sc_make;
  *size = sc_size;
  for (uint8_t i = 0; i < sc_size; i++) bytes[i] = sc_buf[i];
  return 0;
}
