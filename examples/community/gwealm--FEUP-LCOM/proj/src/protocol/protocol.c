#include "protocol/protocol.h"

#include "devices/serial.h"

static bool running = false;
static protocol_step step;
static char* word = NULL;

void (protocol_init)() {
  running = true;
  step = UNKNOWN;

  event_on(SERIAL_PACKET, protocol_handle_packet);
}

void (protocol_cleanup)() {
  event_unregister(SERIAL_PACKET, protocol_handle_packet);

  running = false;
}

void (protocol_send_handshake)(char *target) {
  if (!running) {
    return;
  }

  word = target;

  serial_packet handshake;
  handshake.type = HANDSHAKE;
  serial_send_packet(&handshake);

  step = HANDSHAKE_TRANSMITTED;
}

void (protocol_handle_packet)(struct event *ev) {
  if (!running) {
    return;
  }

  serial_packet *packet = ev->data;

  switch (packet->type) {
    case HANDSHAKE:
      protocol_handle_handshake();
      break;

    case ACK:
      protocol_handle_ack();
      break;

    case WORD:
      event_dispatch(MULTIPLAYER_READY, packet->content, sizeof(packet->content));
      break;

    case SUBMISSION:
      event_dispatch(MULTIPLAYER_SUBMISSION, packet->content, sizeof(packet->content));
      break;

    default:
      break;
  }
}

void (protocol_handle_handshake)() {
  serial_packet packet;
  packet.type = ACK;
  serial_send_packet(&packet);

  step = HANDSHAKE_RECEIVED;
}

void (protocol_handle_ack)() {
  serial_packet packet;

  switch (step) {
    case HANDSHAKE_TRANSMITTED:

      packet.type = WORD;  
      memcpy(&packet.content, &word, sizeof(packet.content));
      serial_send_packet(&packet);

      step = WORD_TRANSMITTED;
      break;

    case WORD_TRANSMITTED:
      event_dispatch(MULTIPLAYER_READY, word, strlen(word));
      break;

    default:
      break;
  }
}

void (protocol_send_submission)(cell_background *bg, size_t n) {
  serial_packet packet;
  packet.type = SUBMISSION;
  for (size_t i = 0; i < n && i < sizeof(packet.content); i++) {
    packet.content[i] = bg[i];
  }
}
