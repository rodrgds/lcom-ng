#include "com_manager.h"

static com_packet_t curr_packet;
static uint8_t bytes[6];
static int idx = 0;
static int full_packet = 0;

int com_add_byte(uint8_t byte) {
  full_packet = 0;

  if (idx == 0 && byte == PROTO_START) {
    bytes[idx] = byte;
    idx++;
    return 0;
  }

  if (idx > 0 && idx < 6) {
    bytes[idx] = byte;
    idx++;
    return 0;
  }

  if (idx == 6 && byte == PROTO_END) {
    bytes[idx] = byte;
    idx = 0;
    curr_packet.type = bytes[1];
    curr_packet.data = bytes[5] << 24 | bytes[4] << 16 | bytes[3] << 8 | bytes[2];
    full_packet = 1;
    return 1;
  }

  idx = 0;
  return 0;
}

int com_send_msg(proto_msg_type_t type, uint32_t content) {
  if (uart_send_byte(PROTO_START))
    return 1;
  if (uart_send_byte(type))
    return 1;
  if (uart_send_byte((uint8_t) content))
    return 1;
  if (uart_send_byte((uint8_t) (content >> 8)))
    return 1;
  if (uart_send_byte((uint8_t) (content >> 16)))
    return 1;
  if (uart_send_byte((uint8_t) (content >> 24)))
    return 1;
  return uart_send_byte(PROTO_END);
}

void com_handle_packet(app_t *app) {
  if (full_packet == 0)
    return;

  char buff[50];

  switch (curr_packet.type) {
    case PROTO_NOTIFY_WIN:
      snprintf(buff, 30, "A player has won ~%zu", curr_packet.data);
      break;
    case PROTO_NOTIFY_LOSS:
      snprintf(buff, 30, "A player has lost ~%zu", curr_packet.data);
      break;
    case PROTO_NOTIFY_DRAW:
      snprintf(buff, 45, "A player tied against the dealer for ~%zu", curr_packet.data);
      break;
    case PROTO_NOTIFY_SURRENDER:
      snprintf(buff, 45, "A player gave up and lost ~%zu", curr_packet.data);
      break;
    default:
      strncpy(buff, "Unrecognized notification", 26);
      break;
  }

  banner_set_message(&app->banner, buff, 90);
}
