#include "mouse.h"
#include "kbc_utils.h"

extern uint16_t x_resolution;
extern uint16_t y_resolution;

int mouse_hook_id = 2;
uint8_t read_byte;
struct packet mouse_packet;
uint8_t packet_byte = 0;
uint8_t mouse_bytes[3];

uint16_t mouse_x = 500;
uint16_t mouse_y = 300;

int (mouse_subscribe_interrupts)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }

  *bit_no = BIT(mouse_hook_id);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) {
    return 1;
  }

  return 0;
}

int (mouse_unsubscribe_interrupts)() {
  if (sys_irqrmpolicy(&mouse_hook_id) != 0) {
    return 1;
  }

  return 0;
}

int mouse_init() {
  if (mouse_send_command(SET_STREAM_MODE)) {
    return 1;
  }
  if (mouse_send_command(ENABLE_DATA_REPORTING)) {
    return 1;
  }

  return 0;
}

int (mouse_reset)() {
  if (mouse_send_command(DISABLE_DATA_REPORTING)) {
    return 1;
  }

  return 0;
}


void (mouse_ih)() {
  if (read_KBC_output(&read_byte, KBC_WRITE_CMD, 1)) {
  }
}

int (mouse_send_command)(uint8_t command) {
  uint8_t attemps = MAX_ATT;
  uint8_t response = 0;

  while (response != ACK && attemps > 0) {
    attemps--;

    if (write_KBC_command(KBC_IN_BUF, WRITE_BYTE_TO_MOUSE) != 0) {
      return 1;
    }
    if (write_KBC_command(KBC_OUT_BUF, command) != 0) {
      return 1;
    }

    tickdelay(micros_to_ticks(WAIT_KBC));

    if (util_sys_inb(KBC_OUT_BUF, &response) != 0) {
      return 1;
    }
    if (response == ACK) {
      return 0;
    }
  } 

  return 1;
}

int (mouse_packet_sync)() {
  if (packet_byte == 0 && (read_byte & FIRST_BYTE_SET)) {
    mouse_bytes[0] = read_byte;
    packet_byte++;
  }
  else {
    mouse_bytes[packet_byte] = read_byte;
    packet_byte++;
  }

  return 0;
}

int (mouse_parse_packet)() {
  for (int i = 0; i < 3; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }

  mouse_packet.lb = mouse_bytes[0] & MOUSE_LEFT_BUTTON;
  mouse_packet.rb = mouse_bytes[0] & MOUSE_RIGHT_BUTTON;
  mouse_packet.mb = mouse_bytes[0] & MOUSE_MIDDLE_BUTTON;

  // Delta X (sign extension)
  mouse_packet.delta_x = (mouse_bytes[0] & MSB_X_DELTA) ? (mouse_bytes[1] | 0xFF00) : mouse_bytes[1];
  // Delta Y (sign extension)
  mouse_packet.delta_y = (mouse_bytes[0] & MSB_Y_DELTA) ? (mouse_bytes[2] | 0xFF00) : mouse_bytes[2];

  mouse_packet.x_ov = mouse_bytes[0] & MOUSE_X_OVERFLOW;
  mouse_packet.y_ov = mouse_bytes[0] & MOUSE_Y_OVERFLOW;

  return 0;
}

void (mouse_update_position)() {
  int new_x = (int)mouse_x + mouse_packet.delta_x;
  int new_y = (int)mouse_y - mouse_packet.delta_y;

  if (new_x < 0) new_x = 0;
  if (new_y < 0) new_y = 0;
  if (new_x >= x_resolution - 12) new_x = x_resolution - 12;
  if (new_y >= y_resolution - 18) new_y = y_resolution - 18;

  mouse_x = (uint16_t)new_x;
  mouse_y = (uint16_t)new_y;
}
