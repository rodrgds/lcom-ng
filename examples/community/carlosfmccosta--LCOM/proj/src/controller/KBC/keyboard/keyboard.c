/**
 * @file keyboard.c
 * @brief Implementation of i8042 keyboard controller operations.
 *
 * This file contains the implementation of the functions declared in keyboard.h.
 */

#include <lcom/lcf.h>
#include "keyboard.h"

uint8_t scan_code;
bool make;
int hook_id;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no;
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id) == 0) {
    return 0;
  }
  printf("Error: Subscribing keyboard failed.\n");
  return 1;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) == 0) {
    return 0;
  }
  printf("Error: UnSubscribing keyboard failed.\n");
  return 1;
}

void (kbd_int_handler)() {
  uint8_t data;
  if (!kbd_read_scancode(&data)) {
     scan_code = data;
  }
}

int (kbd_read_scancode)(uint8_t *scan_code) {
  uint8_t stat;
  int retry = 5;
  //Read value from KBD OUT buffer
  while( retry > 0) {
    if (util_sys_inb(KBD_CMD_REG, &stat)) {
      return 1;
    }
    /* loop while 8042 output buffer is empty */
    if( stat & ST_OUTB ) {
      if ( (stat &(ST_PARE | ST_TIM)) == 0 ) {
        if (util_sys_inb(KBC_OUT_BUF, scan_code)) {
          return 1;
        }
        make = ((*scan_code & BIT(7)) == BIT(7));

        return 0;
      } else {
        printf("Error: Reading scancode failed.\n");
        return 1;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
    retry--;
  }
  return 0;
}

int (kbd_code_size)(uint8_t code) {
  int size = 1;
  if (code == FIRST_BYTE_SCANCODE) {
    size = 2;
  }
  return size;
}

bool (kbd_code_make)(uint8_t code) {
  return !(code & BIT(7));
}

uint8_t (read_byte)(uint8_t *data) {
  uint8_t stat;
  int retry = 5;
  while( retry > 0 ) {
    if (util_sys_inb(KBD_CMD_REG, &stat)) {
      return 1;
    }
    /* loop while 8042 output buffer is empty */
    if( (stat & ST_OUTB) == BIT(0) ) {
      if (util_sys_inb(KBC_OUT_BUF, data)) {
        return 1;
      }
      if ( (stat &(ST_PARE | ST_TIM)) == 0 ) {
        return 0;
      } else {
        return 1;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
    retry --;
  }
  return 1;
}

int (write_byte)(uint8_t cmd, uint8_t dest) {
  uint8_t stat;
  int retry = 5;
  while( retry > 0 ) {
    if (util_sys_inb(KBD_CMD_REG, &stat)) {
      return 1;
    }
    /* loop while 8042 input buffer is not empty */
    if( (stat & ST_INPB) == 0 ) {
      sys_outb(dest, cmd); /* no args command */
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    retry--;
  }
  return 0;
}

uint8_t (get_scan_code)() {
  return scan_code;
}

