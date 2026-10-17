#include <lcom/lcf.h>

#include "mouse.h"

struct packet mouse_packet;
int32_t mouse_hook_id = MOUSE_IRQ;
uint8_t packet_index = 0;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = mouse_hook_id;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)) {
    printf("%s: sys_irqsetpolicy(MOUSE_IRQ: %d, IRQ_REENABLE | IRQ_EXCLUSIVE, mouse_hook_id: %d) error\n", __func__, MOUSE_IRQ, mouse_hook_id);
    return 1;
  }

  return 0;
}

int(mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hook_id)) {
    printf("%s: sys_irqrmpolicy(mouse_hook_id: %d) error\n", __func__, mouse_hook_id);
    return 1;
  }
  return 0;
}

int mouse_read_packet_byte() {
  uint8_t output;
  int r;
  if ((r = kbc_read_output(&output))) {
    printf("%s: kbc_read_output(output: 0x%x) error (returned %d)\n", __func__, output, r);
    return 1;
  }

  if (packet_index == 0 && !(output & MOUSE_BYTE1))
    return 1;

  mouse_packet.bytes[packet_index] = output;

  if (packet_index == 1)
    mouse_packet.delta_x |= output;
  else if (packet_index == 2) {
    mouse_packet.delta_y |= output;

    uint8_t first_byte = mouse_packet.bytes[0];

    mouse_packet.lb = (bool) (first_byte & MOUSE_LB);
    mouse_packet.rb = (bool) (first_byte & MOUSE_RB);
    mouse_packet.mb = (bool) (first_byte & MOUSE_MB);

    if (first_byte & MOUSE_MSB_X_DELTA)
      mouse_packet.delta_x |= MOUSE_SET_MSB;

    if (first_byte & MOUSE_MSB_Y_DELTA)
      mouse_packet.delta_y |= MOUSE_SET_MSB;

    mouse_packet.x_ov = (bool) (first_byte & MOUSE_X_OVFL);
    mouse_packet.y_ov = (bool) (first_byte & MOUSE_Y_OVFL);
  }

  packet_index++;

  return 0;
}

void(mouse_ih)() {
  mouse_read_packet_byte();
}

int(mouse_restore)() {
  mouse_packet.bytes[0] = 0;
  mouse_packet.bytes[1] = 0;
  mouse_packet.bytes[2] = 0;
  mouse_packet.delta_x = 0;
  mouse_packet.delta_y = 0;
  packet_index = 0;
  return 0;
}

int(mouse_write_command)(uint8_t command) {
  for (int i = 0; i < KBC_MAX_ATTEMPTS; i++) {
    if (kbc_write_command(KBC_WRITE_TO_MOUSE_CMD)) {
      printf("%s: kbc_write_command(KBC_WRITE_TO_MOUSE_CMD: 0x%x) error\n", __func__, KBC_WRITE_CMD);
      return 1;
    }

    if (kbc_write_argument(command)) {
      printf("%s: kbc_write_argument(command: 0x%x) error\n", __func__, command);
      return 1;
    }

    uint8_t acknowledgment_byte;
    if (kbc_read_acknowledgment_byte(&acknowledgment_byte)) {
      printf("%s: kbc_read_acknowledgment_byte(acknowledgment_byte: 0x%x) error\n", __func__, acknowledgment_byte);
      return 1;
    }

    if (acknowledgment_byte == KBC_ACK)
      return 0;

    if (acknowledgment_byte == KBC_ERROR)
      return 1;

    if (tickdelay(micros_to_ticks(KBD_DELAY))) {
      printf("%s: tickdelay(micros_to_ticks(KBD_DELAY: %d)) error\n", __func__, KBD_DELAY);
      return 1;
    }
  }

  return 0;
}

int(mouse_data_reporting)(bool enable) {
  if (enable) {
    if (mouse_write_command(KBC_ENABLE_DATA_REPORTING)) {
      printf("%s: mouse_write_command(KBC_ENABLE_DATA_REPORTING: 0x%x) error\n", __func__, KBC_ENABLE_DATA_REPORTING);
      return 1;
    }
  }
  else {
    if (mouse_write_command(KBC_DISABLE_DATA_REPORTING)) {
      printf("%s: mouse_write_command(KBC_DISABLE_DATA_REPORTING: 0x%x) error\n", __func__, KBC_DISABLE_DATA_REPORTING);
      return 1;
    }
  }
  return 0;
}
