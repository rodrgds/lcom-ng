#include "mouse.h"
#include "i8042.h"
#include "kbc.h"
#include <lcom/lcf.h>

static int mouse_hook_id = MOUSE_IRQ;
static uint8_t current_byte;
static bool has_error = false;
static uint8_t packet_bytes[3];
static int byte_index = 0;
static struct packet pp;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = mouse_hook_id;
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)(void) {
  if (kbc_read_data_ih(&current_byte, KBC_SOURCE_MOUSE) != 0) {
    has_error = true;
    return;
  }
  has_error = false;
}

static void mouse_parse_packet(void) {
  for (int i = 0; i < 3; i++) pp.bytes[i] = packet_bytes[i];

  pp.lb = packet_bytes[0] & MOUSE_LB;
  pp.rb = packet_bytes[0] & MOUSE_RB;
  pp.mb = packet_bytes[0] & MOUSE_MB;
  pp.x_ov = packet_bytes[0] & MOUSE_X_OVF;
  pp.y_ov = packet_bytes[0] & MOUSE_Y_OVF;

  pp.delta_x = (packet_bytes[0] & MOUSE_X_SIGN) ? SIGN_EXTEND(packet_bytes[1]) : packet_bytes[1];
  pp.delta_y = (packet_bytes[0] & MOUSE_Y_SIGN) ? SIGN_EXTEND(packet_bytes[2]) : packet_bytes[2];
}

bool (mouse_sync_and_build_packet)(void) {
  if (has_error) return false;

  if (byte_index == 0 && !(current_byte & MOUSE_SYNC_BIT)) return false;

  packet_bytes[byte_index] = current_byte;
  byte_index++;

  if (byte_index == 3) {
    mouse_parse_packet();
    byte_index = 0;
    return true;
  }
  return false;
}

struct packet *(mouse_get_packet)(void) {
  return &pp;
}

int (mouse_write_cmd)(uint8_t cmd) {
  int tries = 3;
  uint8_t ack_byte;

  while (tries > 0) {
    if (kbc_write_command(KBC_CMD_REG, WRITE_TO_MOUSE) != 0) return 1;
    if (kbc_write_command(KBC_IN_BUF, cmd) != 0) return 1;
    if (kbc_read_data(KBC_OUT_BUF, &ack_byte, KBC_SOURCE_MOUSE) != 0) return 1;

    if (ack_byte == MOUSE_ACK) return 0;
    if (ack_byte == MOUSE_ERR) return 1;

    tries--;
  }

  return 1;
}
