#include <lcom/lcf.h>
#include "../headers/keyboard.h"


int hookid = 1; // set to 1 so as not to collide with timer's 0
#ifdef LAB3
extern int cnt;
#endif
uint8_t data[2]; // Array for storing scancodes
bool kbd_valid;
bool two_byte_code; // IH flags


// Susbcribes to keyboard interrupts
int (subscribe_kbc)(uint8_t *bit_no) {
  *bit_no = hookid;
  if (sys_irqsetpolicy(IRQ1 , IRQ_REENABLE | IRQ_EXCLUSIVE, &hookid)) {
    return 1;
  }
  return 0;
}

// Unsubscribes from keyboard interrupts
int (unsubscribe_kbc)() {
  if (sys_irqrmpolicy(&hookid)) {
    return 1;
  }
  return 0;
}

// preserve gets wether what's read is in a valid state or not
int kbd_get_status_byte(bool *preserve) {
  uint8_t kbd_status;
  if (util_sys_inb(KBD_STAT_REG, &kbd_status)) {
    return 1; // ERROR reading status register
  }
  if (kbd_status & (PARITY | TIMEOUT | AUX)) {
    *preserve = false;
    return 0; // ERROR parity, timeout or mouse - read but discard
  }
  if (kbd_status & OBF) {
    *preserve = true;
    return 0; // Checks if there is data to read from the buffer
  }
  return 1;
}


void (kbc_ih)(void) {
  kbd_valid = false;
  two_byte_code = false; // Resets flags
  bool preserve;
  uint8_t scancode;

  if (kbd_get_status_byte(&preserve)) { // If there's errors reading status, exit
    return;
  }
  if (util_sys_inb(OUT_BUF, &scancode)) { // If there's errors read OUT_BUF, exit
    return;
  }
  if (!preserve) { // If OUT_BUF is not in valid state, exit
    return;
  }

  // Saves the scancode for two and single byte codes
  if (scancode == TWO_BYTE) {
    data[0] = scancode;
    two_byte_code = true;
  } else if (two_byte_code) {
    data[1] = scancode;
    two_byte_code = false;
  } else {
    data[0] = scancode;
  }
  
  kbd_valid = true; // Everything went well
  return;
}


// Reads command byte to out
int kbc_cmd_read(uint8_t *out) {
  sys_outb(KBC_CMD_REG, READ_CMD);
  util_sys_inb(OUT_BUF, out);
  return 0;
}

// Writes command
int kbc_cmd_write(uint8_t command) {
  sys_outb(KBC_CMD_REG, WRITE_CMD);
  sys_outb(KBC_ARG_REG, command);
  return 0;
}
