#include <lcom/lcf.h>

#include <stdint.h>

#include "kbd.h"
#include "i8042.h"

int (kbd_state)(uint8_t *stat) {
  if (stat == NULL) {
    printf("null pointer\n");
    return 1;
  }

  if (util_sys_inb(KBC_ST_REG, stat))
    return 1;

  if (*stat & KBC_ST_ERROR) { // check errors
    printf("error in the (serial) communication between the keyboard and the KBC");
    return 1;
  }

  return 0;
}

int (kbd_cmd)(uint8_t info, bool arg) {
  uint8_t stat = 0;
  int tries = 1000; // try for a limited amount of time
  while (tries) {
    if (kbd_state(&stat))
      return 1;

    /* loop while 8042 input buffer is not empty */
    if (!(stat & KBC_ST_IBF)) {
      uint8_t reg = arg ? KBC_CMD_ARG : KBC_CMD_REG; // check if an argument needs to be passed
      if(sys_outb(reg, info)) {
        printf("no proper writing\n");
        return 1;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (kbd_scancode)() {
  uint8_t byte = 0;
  if (util_sys_inb(KBC_OUT_BUF, &byte)) // read only one bite (slow communication)
    return 1;

  if (kbd_inc_code()) {
    scancode[0] = KBD_FIRST_BYTE; // move 0xE0 to the first spot of the array
    scancode[1] = byte; // store byte read in the second spot
  }
  else if (byte == KBD_FIRST_BYTE)
    scancode[1] = byte; // when received, the 0xE0 byte goes to the second spot of the array
  else
    scancode[0] = byte; // one byte scancode (stored in the first spot)

  return 0;
}

int (kbd_ret_value)(uint8_t *data) {
  if (data == NULL) {
    printf("null pointer\n");
    return 1;
  }

  uint8_t stat = 0;
  int tries = 1000; // try for a limited amount of time 
  while (tries) {
    if (kbd_state(&stat))
      return 1;

    /* loop while 8042 output buffer is empty */
    if ((stat & KBC_ST_OBF) && !(stat & KBC_ST_AUX)) { // if OBF is set and AUX is cleared
      if (util_sys_inb(KBC_OUT_BUF, data)) // read return value from output buffer
        return 1;

      if (kbd_state(&stat)) // check error in status byte
        return 1;
      else
        return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (kbd_scancode_size)() {
  if (scancode[0] == KBD_FIRST_BYTE) // scancode has two bytes if 0xE0 is in the array's first spot
    return 2;

  return 1;
}

bool (kbd_make)() {
  int byte_pos = kbd_scancode_size() - 1;
  if (scancode[byte_pos] & KBD_BREAK_BIT) // check if MSB is set to 1 (break code)
    return false;

  return true;
}

bool (kbd_inc_code)() {
  return (scancode[1] == KBD_FIRST_BYTE); // true if byte 0xE0 is in array's second spot
}

int (kbd_esc_break)() {
  if (scancode[0] == ESC_BK_CODE) // scancode byte is 0x81
    return 0;

  return 1;
}

static int hook_id;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("null pointer\n");
    return 1;
  }

  hook_id = KBD_IRQ;
  *bit_no = hook_id;

  /* interrupt notification subscription */
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) { 
    printf("bad subscription\n");
    return 1;
  }

  return 0;
}

int (kbd_unsubscribe_int)() {
  /* 
  unsubscribe a previous subscription of the interrupt 
  notification associated with the specified hook_id 
  */
  if (sys_irqrmpolicy(&hook_id)) {
    printf("bad unsubscription\n");
    return 1;
  }

  return 0;
}

void (kbc_ih)() {
  uint8_t st = 0;
  if (kbd_state(&st)) // read the status register
    return;
  
  /* check if we are set to execute the read operation */
  if (!(st & KBC_ST_OBF) || (st & KBC_ST_AUX)) {
    printf("invalid operation\n");
    return;
  }

  kbd_scancode(); // read scancode byte
}

int (kbd_poll)() {
  uint8_t st = 0;
  if (kbd_state(&st)) // read the status register
    return 1;

  uint8_t byte = 0;
  if (kbd_ret_value(&byte)) // read scancode
    return 1;

  if (byte != KBD_FIRST_BYTE) {
    scancode[0] = byte;
  }
  else {
    scancode[0] = KBD_FIRST_BYTE; // store 0xE0 to the first spot of the array
    
    if (kbd_ret_value(&byte)) // read next scancode byte
      return 1;

    scancode[1] = byte; // store new byte read in the second spot
  }

  return 0;  
}

int (kbd_enable_int)() {
  /* read command byte */
  uint8_t cmd = 0;
  if (kbd_cmd(KBD_RCMD, false))
    return 1;
  if (kbd_ret_value(&cmd))
    return 1;

  cmd |= KBC_CMD_INT_KBD; // enable interrupt on OBF

  /* write command byte */
  if (kbd_cmd(KBD_WCMD, false))
    return 1;
  if (kbd_cmd(cmd, true))
    return 1;

  return 0;
}

uint8_t (get_key)() {
  uint8_t key_code = (kbd_scancode_size() == 2) ? scancode[1] : scancode[0];
  return key_code;
}
