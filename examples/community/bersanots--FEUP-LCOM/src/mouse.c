#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"
#include "mouse.h"
#include <lcom/utils.h>

int mouse_id = MOUSE_IRQ;
uint8_t reader = 0x00;

void(mouse_ih)() {
  uint8_t control;
  reader = 0x00;

  util_sys_inb(STAT_REG, &control);

  if ((control & STAT_REG_OBF) != 0 || (control & (STAT_REG_PARITY | STAT_REG_TIMEOUT)) == 0) {
    if (util_sys_inb(OUT_BUF, &reader) != 0) {
      reader = 0x00;
      return;
    }
  }
}

int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = mouse_id;

  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_id);

  return 0;
}

int(mouse_unsubscribe_int)() {
  sys_irqrmpolicy(&mouse_id);
  return 0;
}

void parsePacket(struct packet *pp, uint8_t bytes[]) {

  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];

  pp->rb = ((bytes[0] & RBP) >> 1);
  pp->mb = ((bytes[0] & MBP) >> 2);
  pp->lb = (bytes[0] & LBP);

  //Expand second and third byte with signal
  if (bytes[0] & DELTA_X) {
    pp->delta_x = bytes[1] | (0XFF << 8);
  }
  else {
    pp->delta_x = bytes[1];
  }
  if (bytes[0] & DELTA_Y) {
    pp->delta_y = ((0XFF << 8) | bytes[2]);
  }
  else {
    pp->delta_y = bytes[2];
  }
  //x_ov and y_ov are booleans that correspond to bits 6 and 7 of the first byte
  pp->x_ov = ((bytes[0] & X_OVERFLOW) >> 6);
  pp->y_ov = ((bytes[0] & Y_OVERFLOW) >> 7);
}

int mouse_data_report(uint8_t entry) {

  uint8_t control, ack_byte = 0x00;

  do {
    util_sys_inb(STAT_REG, &control);

    if (control & STAT_REG_IBF)
      return 1;

    if (sys_outb(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE) != 0)
      return 1;

    util_sys_inb(STAT_REG, &control);

    if (control & STAT_REG_IBF)
      return 1;

    if (sys_outb(IN_BUF, entry) != 0)
      return 1;

    if (util_sys_inb(OUT_BUF, &ack_byte) != 0)
      return 1;

    if (ack_byte == ERROR)
      return 1;

  } while (ack_byte != ACK);

  return 0;
}
