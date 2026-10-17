#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

int keyboard_hook_id = 1;
uint8_t data = 0x00;
extern uint32_t no_sysinb;

int(KBC_subscribe_int)(uint8_t *bit_no) {
  keyboard_hook_id = *bit_no;
  if (bit_no == NULL)
    return 1;
  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id))
    return 1;
  return 0;
}

int(KBC_unsubscribe_int)() {
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void(KBC_int_handler)(uint8_t *data) {
  readStatusRegister(data);
}

int(readStatusRegister)(uint8_t *read) {
  uint8_t status;
  uint8_t notimportant;
  if (util_sys_inb(statReg, &status)){
    return 1;
  }
  if ((status & (BIT(6) | BIT(5))) == 0 && (status & BIT(0))) {
    util_sys_inb(in_outBuff, read);
    return 0;
  }
  util_sys_inb(in_outBuff, &notimportant);
  return 1;
}

int(renable_int)() {
  uint8_t status;
  if (sys_outb(statReg, ReadCommandByte))
    return 1;
  if (util_sys_inb(in_outBuff, &status))
    return 1;
  status |= BIT(0);
  if (sys_outb(statReg, WriteCommandByte))
    return 1;
  if (sys_outb(in_outBuff, status))
    return 1;
  return 0;
}
