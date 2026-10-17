#include <lcom/lcf.h>

#include "mouse.h"
#include "../i8042.h"
#include "../../../utils/utils.h"

#include <stdint.h>

int* mouse_hook_id;
int packet_index;
uint8_t packet_bytes[3];

int (mouse_subscribe_int)(uint8_t *bit_no) {
  mouse_hook_id = (int*) malloc(sizeof(int));
  *mouse_hook_id = MOUSE_IRQ;
  *bit_no = MOUSE_IRQ;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, mouse_hook_id) != 0)
    return 1;
  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(mouse_hook_id) != 0)
    return 1;
  return 0;
}

// not working
int (enable_data_reporting)() {
  for (int i = 0; i < 3; i++) {
    uint8_t* status = (uint8_t*) malloc(sizeof(uint8_t));
    if (util_sys_inb(ST_REG, status) != 0)
      return 1;

    if (*status & IBF)
      continue;

    if (sys_outb(KBC_CMD_REG, WRITE_MOUSE_BYTE) != 0)
      return 1;
    if (sys_outb(OUT_BUF_REG, ENABLE_DATA) != 0)
      return 1;

    uint8_t* acknowledgement = (uint8_t*) malloc(sizeof(uint8_t));
    if (util_sys_inb(OUT_BUF_REG, acknowledgement) != 0)
      return 1;
    if (*acknowledgement == ACK)
      return 0;
    if (*acknowledgement == ERROR)
      return 1;
  }
  return 1;
}

// not working
int (disable_data_reporting)() {
  for (int i = 0; i < 3; i++) {
    uint8_t* status = (uint8_t*) malloc(sizeof(uint8_t));
    if (util_sys_inb(ST_REG, status) != 0)
      return 1;

    if (*status & IBF)
      continue;

    if (sys_outb(KBC_CMD_REG, WRITE_MOUSE_BYTE) != 0)
      return 1;
    if (sys_outb(OUT_BUF_REG, DISABLE_DATA) != 0)
      return 1;

    uint8_t* acknowledgement = (uint8_t*) malloc(sizeof(uint8_t));
    if (util_sys_inb(OUT_BUF_REG, acknowledgement) != 0)
      return 1;
    if (*acknowledgement == ACK)
      return 0;
    if (*acknowledgement == ERROR)
      return 1;
  }
  return 1;
}

void (mouse_int_handler)() {
  uint8_t* current_byte = (uint8_t*) malloc(sizeof(uint8_t));
  if (util_sys_inb(OUT_BUF_REG, current_byte) != 0)
    return;

  uint8_t* status = (uint8_t*) malloc(sizeof(uint8_t));
  if (util_sys_inb(ST_REG, status) != 0)
    return;
  if (*status & (TO_ERR | PAR_ERR))
    return;

  if ((packet_index == 0 && *current_byte & BIT(3)) || packet_index != 0) {
    packet_bytes[packet_index] = *current_byte;
    packet_index++;
  }
}

void (parse_mouse_packet)(struct packet* pp) {
  pp->bytes[0] = packet_bytes[0];
  pp->bytes[1] = packet_bytes[1];
  pp->bytes[2] = packet_bytes[2];
  pp->lb = packet_bytes[0] & BIT(0);
  pp->rb = (packet_bytes[0] & BIT(1)) >> 1;
  pp->mb = (packet_bytes[0] & BIT(2)) >> 2;
  pp->x_ov = (packet_bytes[0] & BIT(6)) >> 6;
  pp->y_ov = (packet_bytes[0] & BIT(7)) >> 7;
  
  if (packet_bytes[0] & BIT(4)) {
    pp->delta_x = 0xff00 | packet_bytes[1];
  } else {
    pp->delta_x = packet_bytes[1];
  }
  if (packet_bytes[0] & BIT(5)) {
    pp->delta_y = 0xff00 | packet_bytes[2];
  } else {
    pp->delta_y = packet_bytes[2];
  } 
}
