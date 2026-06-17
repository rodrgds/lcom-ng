#include "utils.h"

#include <stdio.h>

int sys_inb_counter = 0;
int sys_outb_counter = 0;

int util_sys_inb(int port, uint8_t *value) {
  if (value == NULL) return 1;
  sys_inb_counter++;
  return lcom_port_read8((uint16_t)port, value) == LCOM_OK ? 0 : 1;
}

int util_sys_outb(int port, uint8_t value) {
  sys_outb_counter++;
  return lcom_port_write8((uint16_t)port, value) == LCOM_OK ? 0 : 1;
}

void util_force_tty_redraw(void) {
  fflush(stdout);
}

int16_t util_clamp_i16(int32_t value, int16_t min_value, int16_t max_value) {
  if (value < min_value) return min_value;
  if (value > max_value) return max_value;
  return (int16_t)value;
}

int32_t util_clamp_i32(int32_t value, int32_t min_value, int32_t max_value) {
  if (value < min_value) return min_value;
  if (value > max_value) return max_value;
  return value;
}
