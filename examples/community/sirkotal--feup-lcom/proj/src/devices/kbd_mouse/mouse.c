#include "i8042.h"
#include "kbc.h"
#include "mouse.h"

int mouse_hook_id = 2;

uint8_t mouse_byte;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = mouse_hook_id;
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

int (mouse_int_handler)() {
  if (kbc_read_output(KBC_OUT_BUF, &mouse_byte, true)) {
    return 1;
  }

  if ((packet_index == 0 && (mouse_byte & FIRST_BYTE)) || packet_index > 0) {
    mouse_packet.bytes[packet_index] = mouse_byte;
    packet_index++;
  }

  return 0;
}

int (mouse_write_command)(uint8_t command) {
  uint8_t mouse_res;

  for (int i = 0; i < ATTEMPTS; i++) {
    if (kbc_write_command(KBC_CMD_REG, KBC_CMD_MOUSE_WRITE)) {
      printf("Error writing mouse write command to KBC\n");
      return 1;
    }

    if (kbc_write_command(KBC_IN_BUF, command)) {
      printf("Error writing command to kbc input buffer\n");
      return 1;
    }

    if (util_sys_inb(KBC_OUT_BUF, &mouse_res)) {
      printf("Error reading mouse response\n");
      return 1;
    }

    if (mouse_res == KBC_CMD_MOUSE_ACK) {
      return 0;
    }
  }

  printf("Error writing command to mouse\n");
  return 1;
}

int (generate_mouse_packet)() {
  if (packet_index != 3) {
    return 1;
  }

  mouse_packet.lb = mouse_packet.bytes[0] & KBC_MOUSE_LB;
  mouse_packet.rb = mouse_packet.bytes[0] & KBC_MOUSE_RB;
  mouse_packet.mb = mouse_packet.bytes[0] & KBC_MOUSE_MB;

  mouse_packet.delta_x = 
    (mouse_packet.bytes[0] & KBC_MOUSE_X_SIGN) ?
      (0xFF00 | mouse_packet.bytes[1]) : mouse_packet.bytes[1];

  mouse_packet.delta_y =
    (mouse_packet.bytes[0] & KBC_MOUSE_Y_SIGN) ?
      (0xFF00 | mouse_packet.bytes[2]) : mouse_packet.bytes[2];

  mouse_packet.x_ov = mouse_packet.bytes[0] & KBC_MOUSE_X_OVF;
  mouse_packet.y_ov = mouse_packet.bytes[0] & KBC_MOUSE_Y_OVF;

  packet_index = 0;
  return 0;
}

int (get_current_packet_index)() {
  return packet_index;
}

struct packet (get_mouse_packet)() {
  return mouse_packet;
}

void (mouse_ih)(void) {
  mouse_int_handler();
}
