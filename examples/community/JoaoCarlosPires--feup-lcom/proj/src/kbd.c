#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include <kbd.h>

// All the following functions were created during the labs

// Global Variables for Keyboard
uint8_t scancode;
uint32_t cnt;
bool error;

void(kbc_ih)() { // If there was some error, the byte read from the OB should be discarded

  uint8_t scan;
  uint8_t stat;

  util_sys_inb(STAT_REG, &stat); // Reads the status register

  if (stat & OBF) {               
    util_sys_inb(OUT_BUF, &scan); // Reads the output buffer (OB)
    scancode = scan;
    if ((stat & (PAR_ERR | TO_ERR)) == 0) {
      error = false;
    }
    else {
      error = true;
    }
  }
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t new_var;
  sys_inb(port, &new_var);
  *value = (uint8_t) new_var;
  cnt++;
  return 0;
}

int get_MSB(uint8_t val, uint8_t *msb) {
  *msb = (uint8_t)((val) >> 7);
  return 0;
}

int get_LSB(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)((val << 7) >> 7);
  return 0;
}
