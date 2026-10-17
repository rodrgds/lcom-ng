#include "mouse.h"
#include "kbc.h"
#include <lcom/lcf.h>

int mouse_hook_id = 2;
uint8_t mouse_byte;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(mouse_hook_id);

  return (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id));
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void(mouse_ih)(void) {
  kbc_read_output(&mouse_byte, 1);
}

void(mouse_byte_sync)(uint8_t *byte_counter, uint8_t packet[]) {
  switch (*byte_counter) {
    case 0:
      if (mouse_byte & BIT(3)) {
        packet[0] = mouse_byte;
        (*byte_counter)++;
      }
      break;
    case 1:
      packet[1] = mouse_byte;
      (*byte_counter)++;
      break;
    case 2:
      packet[2] = mouse_byte;
      (*byte_counter)++;
      break;
  }
}

int(mouse_parse_packet)(uint8_t packet[], struct packet *pp) {
  if (packet == NULL || pp == NULL)
    return 1;

  pp->bytes[0] = packet[0];
  pp->bytes[1] = packet[1];
  pp->bytes[2] = packet[2];

  pp->rb = packet[0] & MOUSE_RB;
  pp->mb = packet[0] & MOUSE_MB;
  pp->lb = packet[0] & MOUSE_LB;
  pp->x_ov = packet[0] & MOUSE_X_OVF;
  pp->y_ov = packet[0] & MOUSE_Y_OVF;

  pp->delta_x = (packet[0] & MOUSE_MSB_X) ? (0xFF00 | packet[1]) : packet[1];
  pp->delta_y = (packet[0] & MOUSE_MSB_Y) ? (0xFF00 | packet[2]) : packet[2];

  return 0;
}

int(mouse_write_command)(uint8_t command) {
  int attempts = 10;
  uint8_t mouse_ack;

  while (attempts > 0 && mouse_ack != ACK) {
    if (kbc_write_command(KBC_CMD_REG, KBC_CMD_WRITE_BYTE_TO_MOUSE))
      return 1;
    if (kbc_write_command(KBC_IN_BUF, command))
      return 1;
    tickdelay(micros_to_ticks(DELAY_US));
    if (util_sys_inb(KBC_OUT_BUF, &mouse_ack))
      return 1;
    if (mouse_ack == ACK)
      return 0;
    attempts--;
  }

  return 1;
}
