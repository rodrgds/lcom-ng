#include <lcom/lcf.h>

#include "keyboard.h"
#include "../i8042.h"
#include "../../../utils/utils.h"

#include <stdint.h>

int* kb_hook_id;
bool make_code;
int num_bytes;
uint8_t scan_bytes[2];
bool full_scancode;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  kb_hook_id = (int*) malloc(sizeof(int));
  *kb_hook_id = KEYBOARD_IRQ;
  *bit_no = KEYBOARD_IRQ;
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, kb_hook_id) != 0)
    return 1;
  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(kb_hook_id) != 0)
    return 1;
  return 0;
}

int (kbc_enable_int)() {
  uint8_t* status = (uint8_t*) malloc(sizeof(uint8_t));
  if (sys_outb(KBC_CMD_REG, READ_CMD) != 0)
    return 1;
  if (util_sys_inb(OUT_BUF_REG, status) != 0)
    return 1;

  *status |= BIT(0);

  if (sys_outb(KBC_CMD_REG, WRITE_CMD) != 0)
    return 1;
  if (sys_outb(OUT_BUF_REG, *status) != 0)
    return 1;
  return 0;
}

void (keyboard_int_handler)() {
  for (int i = 0; i < 3; i++) {
    uint8_t* status = (uint8_t*) malloc(sizeof(uint8_t));
    if (util_sys_inb(ST_REG, status) != 0)
      return;
    if (!(*status & OBF))
      return;

    uint8_t* scancode = (uint8_t*) malloc(sizeof(uint8_t));
    if (util_sys_inb(OUT_BUF_REG, scancode) != 0)
      return;

    if (util_sys_inb(ST_REG, status) != 0)
      return;
    if (*status & (PAR_ERR | TO_ERR))
      return;

    num_bytes++;
    if (*scancode == SCAN_MSB) {
      full_scancode = false;
      scan_bytes[0] = SCAN_MSB;
      return;
    } else {
      full_scancode = true;
      if (num_bytes == 2)
        scan_bytes[1] = *scancode;
      else
        scan_bytes[0] = *scancode;
      if (*scancode & BIT(7))
        make_code = false;
      else
        make_code = true;
      return;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}
