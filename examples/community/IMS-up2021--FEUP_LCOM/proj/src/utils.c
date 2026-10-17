
#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <stdint.h>

#include <stdint.h>

#include "utils.h"


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;
  
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val = val >> 8;

  *msb = (uint8_t) val;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value32 = 0x00;
  int ret_value = sys_inb(port, &value32);

  *value = value32;

  return ret_value;
}



bool (is_char) (char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

char (get_key)(uint8_t keycode) {
    if (keycode < Q_T || keycode > M_T) return '\0';
    if (keycode > P_T) keycode -= 4;
    if (keycode > L_T) keycode -= 5;
    return "QWERTYUIOPASDFGHJKLZXCVBNM"[keycode - Q_T];
}

