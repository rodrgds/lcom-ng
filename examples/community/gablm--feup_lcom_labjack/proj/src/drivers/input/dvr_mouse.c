#include "input.h"

int hook_id_mouse = 2;
uint8_t output = 0;
uint8_t bytes[3] = {0, 0, 0};
uint8_t idx = 0;
mouse_info_t *current_packet = NULL;
int isPacketFull = 0;

int mouse_init(uint8_t *bit_no) {
  if (kbc_write(MOUSE_DATA_REPORT_ENABLE, true))
    return 1;

  if (mouse_subscribe_int(bit_no))
    return 1;

  if (current_packet != NULL)
    free(current_packet);

  current_packet = (mouse_info_t *) malloc(sizeof(mouse_info_t));

  return 0;
}

int mouse_disable() {
  if (mouse_unsubscribe_int())
    return 1;

  if (current_packet != NULL)
    free(current_packet);

  return kbc_write(MOUSE_DATA_REPORT_DISABLE, true);
}

int mouse_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_mouse);

  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int mouse_unsubscribe_int() {
  if (hook_id_mouse < 0)
    return 1;

  return sys_irqrmpolicy(&hook_id_mouse);
}

void(mouse_ih)() {
  kbc_read_output(OUT_BUF, &output, true);
}

int mouse_read_packet() {
  isPacketFull = 0;
  if (idx == 0 && (output & BIT(3))) {
    bytes[idx] = output;
    idx++;
    return 0;
  }
  if (idx > 0 && idx < 3) {
    bytes[idx] = output;
    idx++;
  }
  if (idx == 3) {
    idx = 0;
    if (current_packet == NULL)
      return 0;
    mouse_fill_packet(bytes, current_packet);
    isPacketFull = 1;
    return 1;
  }
  return 0;
}

void mouse_fill_packet(uint8_t *bytes, struct packet *pp) {
  for (int i = 0; i < 3; i++) {
    pp->bytes[i] = bytes[i];
  }
  pp->lb = bytes[0] & MOUSE_CTRL_LB;
  pp->rb = bytes[0] & MOUSE_CTRL_RB;
  pp->mb = bytes[0] & MOUSE_CTRL_MB;
  pp->delta_x = bytes[0] & MOUSE_CTRL_X_SIG
                  ? 0XFF00 | bytes[1]
                  : bytes[1];
  pp->delta_y = bytes[0] & MOUSE_CTRL_Y_SIG
                  ? 0XFF00 | bytes[2]
                  : bytes[2];
  pp->x_ov = bytes[0] & MOUSE_CTRL_X_OFW;
  pp->y_ov = bytes[0] & MOUSE_CTRL_Y_OFW;
}

mouse_info_t *mouse_get_info() {
  return isPacketFull ? current_packet : NULL;
}
