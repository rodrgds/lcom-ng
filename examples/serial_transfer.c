#include "example_ui.h"

#include <lcom/i8254.h>
#include <lcom/i8042.h>
#include <lcom/lcom.h>
#include <lcom/uart16550.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XFER_PROTOCOL_VERSION 1u
#define XFER_MAGIC_0 0xC3u
#define XFER_MAGIC_1 0x7Eu
#define XFER_MAX_PAYLOAD 10u
#define XFER_FRAME_OVERHEAD 6u
#define XFER_UART_TX_CAP 128u

#define XFER_IMAGE_W 16u
#define XFER_IMAGE_H 16u
#define XFER_IMAGE_BYTES (XFER_IMAGE_W * XFER_IMAGE_H)
#define XFER_CHUNK_BYTES 8u
#define XFER_BLOCK_COUNT (XFER_IMAGE_BYTES / XFER_CHUNK_BYTES)

#define XFER_RETRY_TICKS 36u
#define XFER_MAX_ATTEMPTS 6u
#define XFER_COMPLETE_GRACE_TICKS (XFER_RETRY_TICKS * 3u)
#define XFER_FAILURE_GRACE_TICKS (XFER_RETRY_TICKS * 3u)
#define XFER_PEER_TIMEOUT_TICKS 180u
#define XFER_BLOCK_PACE_TICKS 4u
#define XFER_FAULT_OFFSET 56u

typedef enum {
  XFER_MSG_HELLO = 1,
  XFER_MSG_METADATA = 2,
  XFER_MSG_DATA = 3,
  XFER_MSG_DONE = 4,
  XFER_MSG_FAIL = 5,
  XFER_MSG_ACK = 0x70,
  XFER_MSG_NACK = 0x71
} xfer_message_type_t;

typedef enum {
  XFER_FAIL_NONE = 0,
  XFER_FAIL_CRC = 1,
  XFER_FAIL_PEER_TIMEOUT = 2
} xfer_failure_reason_t;

typedef enum {
  XFER_RX_MAGIC_0,
  XFER_RX_MAGIC_1,
  XFER_RX_TYPE,
  XFER_RX_SEQUENCE,
  XFER_RX_LENGTH,
  XFER_RX_PAYLOAD,
  XFER_RX_CHECKSUM
} xfer_rx_state_t;

typedef enum {
  XFER_PHASE_NEGOTIATING,
  XFER_PHASE_METADATA,
  XFER_PHASE_TRANSFERRING,
  XFER_PHASE_VERIFYING,
  XFER_PHASE_COMPLETE,
  XFER_PHASE_FAILED
} xfer_phase_t;

typedef enum {
  XFER_PROCESS_REJECT,
  XFER_PROCESS_ACCEPT,
  XFER_PROCESS_TERMINAL
} xfer_process_result_t;

typedef struct {
  xfer_rx_state_t state;
  uint8_t type;
  uint8_t sequence;
  uint8_t length;
  uint8_t index;
  uint8_t payload[XFER_MAX_PAYLOAD];
} xfer_parser_t;

typedef struct {
  lcom_irq_t irq;
  bool subscribed;
  bool tx_irq_enabled;
  uint8_t tx[XFER_UART_TX_CAP];
  uint16_t tx_head;
  uint16_t tx_tail;
  xfer_parser_t parser;
  uint32_t bytes_tx;
  uint32_t bytes_rx;
  uint32_t frames_tx;
  uint32_t frames_rx;
  uint32_t crc_errors;
  uint32_t line_errors;
  uint32_t tx_overflows;
} xfer_uart_t;

typedef struct {
  example_canvas_t canvas;
  uint8_t *framebuffer;
  lcom_vbe_mode_info_t mode_info;
  lcom_irq_t timer_irq;
  bool timer_subscribed;
  lcom_irq_t keyboard_irq;
  lcom_irq_t mouse_irq;
  bool keyboard_subscribed;
  bool mouse_subscribed;
  bool mouse_reporting;
  bool mouse_ack;
  uint8_t mouse_bytes[3];
  uint8_t mouse_index;
  uint8_t mouse_buttons;
  int mouse_x;
  int mouse_y;
  example_modifiers_t modifiers;
  bool extended_key;
  bool show_help;
  bool keep_open;
  xfer_uart_t uart;
  bool is_sender;
  bool simulate_fault;
  bool simulate_bad_crc;
  bool simulate_payload_corruption;
  bool simulate_done_ack_loss;
  bool simulate_stall;
  bool fault_used;
  bool payload_corruption_used;
  bool done_ack_drop_used;
  bool stall_used;
  bool peer_hello;
  bool peer_activity;
  bool hello_acked;
  bool hello_sent;
  bool metadata_acked;
  bool done_acked;
  bool metadata_received;
  bool transfer_verified;
  bool terminal_failure;
  bool failure_coordinated;
  bool in_flight;
  bool running;
  uint8_t tx_sequence;
  uint8_t rx_expected;
  uint8_t in_flight_type;
  uint8_t in_flight_sequence;
  uint8_t in_flight_length;
  uint8_t in_flight_payload[XFER_MAX_PAYLOAD];
  uint8_t in_flight_attempts;
  uint16_t sent_bytes;
  uint16_t received_bytes;
  uint16_t expected_bytes;
  uint8_t blocks_acked;
  uint8_t blocks_received;
  uint8_t asset[XFER_IMAGE_BYTES];
  uint8_t received[XFER_IMAGE_BYTES];
  uint32_t asset_crc32;
  uint32_t expected_crc32;
  uint32_t tick;
  uint32_t in_flight_tick;
  uint32_t next_action_tick;
  uint32_t complete_tick;
  uint32_t failure_tick;
  uint32_t last_peer_frame_tick;
  uint32_t retries;
  uint32_t nacks;
  uint32_t duplicates;
  uint32_t out_of_order;
  uint32_t rejected;
  uint32_t remote_failures;
  uint8_t failure_reason;
  xfer_phase_t phase;
  char status[48];
} xfer_app_t;

static const uint32_t xfer_palette[6] = {
    0x111827u, 0x315CA8u, 0x5FA8FFu, 0xE8F2FFu, 0xF4B942u, 0xEF6461u};

static const char xfer_sprite_rows[XFER_IMAGE_H][XFER_IMAGE_W + 1u] = {
    "0000011111100000",
    "0001122222211000",
    "0012233333322100",
    "0122333333332210",
    "0123343333433210",
    "0123333333333210",
    "0122222222222210",
    "0122552222552210",
    "0122552222552210",
    "0122222222222210",
    "0122244444222210",
    "0012244444222100",
    "0001222222211000",
    "0000112222110000",
    "0000011111100000",
    "0000000000000000"};

static void xfer_receive_frame(xfer_app_t *app, uint8_t type, uint8_t sequence,
                               const uint8_t *payload, uint8_t length);

static uint16_t xfer_read_u16(const uint8_t *src) {
  return (uint16_t)src[0] | (uint16_t)((uint16_t)src[1] << 8);
}

static void xfer_write_u16(uint8_t *dst, uint16_t value) {
  dst[0] = (uint8_t)(value & 0xFFu);
  dst[1] = (uint8_t)(value >> 8);
}

static uint32_t xfer_read_u32(const uint8_t *src) {
  return (uint32_t)src[0] |
         ((uint32_t)src[1] << 8) |
         ((uint32_t)src[2] << 16) |
         ((uint32_t)src[3] << 24);
}

static void xfer_write_u32(uint8_t *dst, uint32_t value) {
  dst[0] = (uint8_t)(value & 0xFFu);
  dst[1] = (uint8_t)((value >> 8) & 0xFFu);
  dst[2] = (uint8_t)((value >> 16) & 0xFFu);
  dst[3] = (uint8_t)(value >> 24);
}

static uint8_t xfer_crc8_update(uint8_t crc, uint8_t byte) {
  crc ^= byte;
  for (uint8_t bit = 0; bit < 8u; bit++) {
    crc = (crc & 0x80u) != 0u ? (uint8_t)((crc << 1) ^ 0x07u)
                              : (uint8_t)(crc << 1);
  }
  return crc;
}

static uint8_t xfer_frame_checksum(uint8_t type, uint8_t sequence, uint8_t length,
                                   const uint8_t *payload) {
  uint8_t crc = 0;
  crc = xfer_crc8_update(crc, type);
  crc = xfer_crc8_update(crc, sequence);
  crc = xfer_crc8_update(crc, length);
  for (uint8_t i = 0; i < length; i++) crc = xfer_crc8_update(crc, payload[i]);
  return crc;
}

static uint32_t xfer_crc32(const uint8_t *data, uint16_t length) {
  uint32_t crc = 0xFFFFFFFFu;
  for (uint16_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (uint8_t bit = 0; bit < 8u; bit++) {
      uint32_t mask = (uint32_t)-(int32_t)(crc & 1u);
      crc = (crc >> 1) ^ (0xEDB88320u & mask);
    }
  }
  return ~crc;
}

static void xfer_set_status(xfer_app_t *app, xfer_phase_t phase,
                            const char *status) {
  app->phase = phase;
  snprintf(app->status, sizeof(app->status), "%s", status);
}

static uint16_t xfer_uart_tx_count(const xfer_uart_t *uart) {
  if (uart->tx_head >= uart->tx_tail) {
    return (uint16_t)(uart->tx_head - uart->tx_tail);
  }
  return (uint16_t)(XFER_UART_TX_CAP - uart->tx_tail + uart->tx_head);
}

static uint16_t xfer_uart_tx_free(const xfer_uart_t *uart) {
  return (uint16_t)(XFER_UART_TX_CAP - 1u - xfer_uart_tx_count(uart));
}

static int xfer_uart_set_tx_interrupt(xfer_uart_t *uart, bool enabled) {
  if (uart->tx_irq_enabled == enabled) return LCOM_OK;
  uint8_t ier = (uint8_t)(IER_RDA | IER_RLS | (enabled ? IER_THRE : 0u));
  if (lcom_port_write8(COM1_BASE + SER_IER, ier) != LCOM_OK) return LCOM_ERR;
  uart->tx_irq_enabled = enabled;
  return LCOM_OK;
}

static void xfer_uart_tx_pump(xfer_uart_t *uart) {
  if (uart->tx_head == uart->tx_tail) {
    (void)xfer_uart_set_tx_interrupt(uart, false);
    return;
  }
  uint8_t lsr = 0;
  if (lcom_port_read8(COM1_BASE + SER_LSR, &lsr) != LCOM_OK) return;
  if ((lsr & (LSR_OE | LSR_PE | LSR_FE | LSR_BI | LSR_FIFO_ERR)) != 0u) {
    uart->line_errors++;
  }
  if ((lsr & LSR_THRE) == 0u) {
    (void)xfer_uart_set_tx_interrupt(uart, true);
    return;
  }

  uint8_t byte = uart->tx[uart->tx_tail];
  if (lcom_port_write8(COM1_BASE + SER_THR, byte) != LCOM_OK) return;
  uart->tx_tail = (uint16_t)((uart->tx_tail + 1u) % XFER_UART_TX_CAP);
  uart->bytes_tx++;
  (void)xfer_uart_set_tx_interrupt(uart, uart->tx_head != uart->tx_tail);
}

static bool xfer_uart_queue_frame(xfer_uart_t *uart, uint8_t type,
                                  uint8_t sequence, const uint8_t *payload,
                                  uint8_t length) {
  if (length > XFER_MAX_PAYLOAD) return false;
  uint8_t frame[XFER_FRAME_OVERHEAD + XFER_MAX_PAYLOAD];
  uint8_t frame_length = (uint8_t)(XFER_FRAME_OVERHEAD + length);
  if (xfer_uart_tx_free(uart) < frame_length) {
    uart->tx_overflows++;
    return false;
  }
  frame[0] = XFER_MAGIC_0;
  frame[1] = XFER_MAGIC_1;
  frame[2] = type;
  frame[3] = sequence;
  frame[4] = length;
  if (length > 0u) memcpy(&frame[5], payload, length);
  frame[5u + length] = xfer_frame_checksum(type, sequence, length, payload);

  for (uint8_t i = 0; i < frame_length; i++) {
    uart->tx[uart->tx_head] = frame[i];
    uart->tx_head = (uint16_t)((uart->tx_head + 1u) % XFER_UART_TX_CAP);
  }
  uart->frames_tx++;
  (void)xfer_uart_set_tx_interrupt(uart, true);
  xfer_uart_tx_pump(uart);
  return true;
}

static void xfer_parser_reset(xfer_parser_t *parser) {
  memset(parser, 0, sizeof(*parser));
  parser->state = XFER_RX_MAGIC_0;
}

static void xfer_parser_feed(xfer_app_t *app, uint8_t byte) {
  xfer_parser_t *parser = &app->uart.parser;
  switch (parser->state) {
  case XFER_RX_MAGIC_0:
    if (byte == XFER_MAGIC_0) parser->state = XFER_RX_MAGIC_1;
    break;
  case XFER_RX_MAGIC_1:
    if (byte == XFER_MAGIC_1) parser->state = XFER_RX_TYPE;
    else parser->state =
        byte == XFER_MAGIC_0 ? XFER_RX_MAGIC_1 : XFER_RX_MAGIC_0;
    break;
  case XFER_RX_TYPE:
    parser->type = byte;
    parser->state = XFER_RX_SEQUENCE;
    break;
  case XFER_RX_SEQUENCE:
    parser->sequence = byte;
    parser->state = XFER_RX_LENGTH;
    break;
  case XFER_RX_LENGTH:
    parser->length = byte;
    parser->index = 0;
    if (byte > XFER_MAX_PAYLOAD) {
      xfer_parser_reset(parser);
    } else {
      parser->state = byte == 0u ? XFER_RX_CHECKSUM : XFER_RX_PAYLOAD;
    }
    break;
  case XFER_RX_PAYLOAD:
    parser->payload[parser->index++] = byte;
    if (parser->index >= parser->length) parser->state = XFER_RX_CHECKSUM;
    break;
  case XFER_RX_CHECKSUM:
    if (byte == xfer_frame_checksum(parser->type, parser->sequence,
                                    parser->length, parser->payload)) {
      app->uart.frames_rx++;
      xfer_receive_frame(app, parser->type, parser->sequence, parser->payload,
                         parser->length);
    } else {
      app->uart.crc_errors++;
      app->nacks++;
      (void)xfer_uart_queue_frame(&app->uart, XFER_MSG_NACK,
                                  app->rx_expected, NULL, 0);
    }
    xfer_parser_reset(parser);
    break;
  default:
    xfer_parser_reset(parser);
    break;
  }
}

static void xfer_uart_service(xfer_app_t *app) {
  uint8_t iir = 0;
  (void)lcom_port_read8(COM1_BASE + SER_IIR, &iir);
  for (uint8_t count = 0; count < 64u; count++) {
    uint8_t lsr = 0;
    if (lcom_port_read8(COM1_BASE + SER_LSR, &lsr) != LCOM_OK) break;
    if ((lsr & (LSR_OE | LSR_PE | LSR_FE | LSR_BI | LSR_FIFO_ERR)) != 0u) {
      app->uart.line_errors++;
    }
    if ((lsr & LSR_RX_RDY) == 0u) break;
    uint8_t byte = 0;
    if (lcom_port_read8(COM1_BASE + SER_RBR, &byte) != LCOM_OK) break;
    app->uart.bytes_rx++;
    xfer_parser_feed(app, byte);
  }
  xfer_uart_tx_pump(&app->uart);
}

static int xfer_uart_start(xfer_uart_t *uart) {
  memset(uart, 0, sizeof(*uart));
  xfer_parser_reset(&uart->parser);
  if (lcom_irq_subscribe(COM1_IRQ, 0, &uart->irq) != LCOM_OK) return LCOM_ERR;
  uart->subscribed = true;
  if (lcom_port_write8(COM1_BASE + SER_IER, 0) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_LCR, (uint8_t)(LCR_DLAB | 0x03u)) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_DLL, 1) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_DLM, 0) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_LCR, 0x03u) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_MCR, MCR_DTR | MCR_RTS | MCR_OUT2) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_FCR,
                       FCR_ENABLE_FIFO | FCR_CLEAR_RX | FCR_CLEAR_TX) != LCOM_OK ||
      lcom_port_write8(COM1_BASE + SER_IER, IER_RDA | IER_RLS) != LCOM_OK) {
    return LCOM_ERR;
  }
  return LCOM_OK;
}

static void xfer_uart_stop(xfer_uart_t *uart) {
  (void)lcom_port_write8(COM1_BASE + SER_IER, 0);
  if (uart->subscribed) {
    (void)lcom_irq_unsubscribe(&uart->irq);
    uart->subscribed = false;
  }
}

static void xfer_build_asset(xfer_app_t *app) {
  for (uint8_t y = 0; y < XFER_IMAGE_H; y++) {
    for (uint8_t x = 0; x < XFER_IMAGE_W; x++) {
      char digit = xfer_sprite_rows[y][x];
      app->asset[(uint16_t)y * XFER_IMAGE_W + x] =
          digit >= '0' && digit <= '5' ? (uint8_t)(digit - '0') : 0u;
    }
  }
  app->asset_crc32 = xfer_crc32(app->asset, XFER_IMAGE_BYTES);
}

static uint32_t xfer_advertised_crc(const xfer_app_t *app) {
  return app->simulate_bad_crc ? app->asset_crc32 ^ 1u : app->asset_crc32;
}

static void xfer_send_current(xfer_app_t *app) {
  if (!app->in_flight) return;
  if (app->simulate_fault && !app->fault_used &&
      app->in_flight_type == XFER_MSG_DATA &&
      xfer_read_u16(app->in_flight_payload) == XFER_FAULT_OFFSET &&
      app->in_flight_attempts == 1u) {
    app->fault_used = true;
    app->in_flight_tick = app->tick;
    xfer_set_status(app, XFER_PHASE_TRANSFERRING,
                    "SIMULATED DROP - AWAIT TIMEOUT");
    lcom_printf("serial transfer simulated drop offset=%u\n", XFER_FAULT_OFFSET);
    return;
  }
  if (xfer_uart_queue_frame(&app->uart, app->in_flight_type,
                            app->in_flight_sequence, app->in_flight_payload,
                            app->in_flight_length)) {
    app->in_flight_tick = app->tick;
  }
}

static bool xfer_begin_reliable(xfer_app_t *app, uint8_t type,
                                const uint8_t *payload, uint8_t length) {
  if (app->in_flight || length > XFER_MAX_PAYLOAD) return false;
  app->in_flight = true;
  app->in_flight_type = type;
  app->in_flight_sequence = app->tx_sequence++;
  app->in_flight_length = length;
  app->in_flight_attempts = 1;
  if (length > 0u) memcpy(app->in_flight_payload, payload, length);
  xfer_send_current(app);
  return true;
}

static void xfer_enter_receiver_failure(xfer_app_t *app, uint8_t reason,
                                        const char *status) {
  if (app->terminal_failure) return;
  app->terminal_failure = true;
  app->failure_reason = reason;
  app->failure_tick = app->tick;
  app->in_flight = false;
  xfer_set_status(app, XFER_PHASE_FAILED, status);

  uint8_t payload[2] = {reason, app->rx_expected};
  if (!xfer_begin_reliable(app, XFER_MSG_FAIL, payload, sizeof(payload))) {
    app->running = false;
  }
}

static void xfer_send_hello(xfer_app_t *app) {
  uint8_t payload[2] = {
      XFER_PROTOCOL_VERSION, app->is_sender ? 1u : 0u};
  (void)xfer_begin_reliable(app, XFER_MSG_HELLO, payload, sizeof(payload));
}

static void xfer_drive_sender(xfer_app_t *app) {
  if (!app->is_sender || app->in_flight || app->phase == XFER_PHASE_FAILED ||
      app->phase == XFER_PHASE_COMPLETE) {
    return;
  }
  if (app->tick < app->next_action_tick) return;
  if (!app->hello_acked || !app->peer_hello) return;

  if (!app->metadata_acked) {
    uint8_t payload[8];
    payload[0] = XFER_IMAGE_W;
    payload[1] = XFER_IMAGE_H;
    xfer_write_u16(&payload[2], XFER_IMAGE_BYTES);
    xfer_write_u32(&payload[4], xfer_advertised_crc(app));
    xfer_set_status(app, XFER_PHASE_METADATA, "SENDING FILE MANIFEST");
    (void)xfer_begin_reliable(app, XFER_MSG_METADATA, payload, sizeof(payload));
    return;
  }

  if (app->simulate_stall) {
    app->stall_used = true;
    xfer_set_status(app, XFER_PHASE_TRANSFERRING,
                    "SIMULATED STALL - AWAIT PEER");
    return;
  }

  if (app->sent_bytes < XFER_IMAGE_BYTES) {
    uint8_t payload[2u + XFER_CHUNK_BYTES];
    uint16_t remaining = (uint16_t)(XFER_IMAGE_BYTES - app->sent_bytes);
    uint8_t chunk = remaining > XFER_CHUNK_BYTES ? XFER_CHUNK_BYTES
                                                 : (uint8_t)remaining;
    xfer_write_u16(payload, app->sent_bytes);
    memcpy(&payload[2], &app->asset[app->sent_bytes], chunk);
    char status[48];
    snprintf(status, sizeof(status), "SENDING BLOCK %02u OF %02u",
             (unsigned)(app->sent_bytes / XFER_CHUNK_BYTES + 1u),
             (unsigned)XFER_BLOCK_COUNT);
    xfer_set_status(app, XFER_PHASE_TRANSFERRING, status);
    (void)xfer_begin_reliable(app, XFER_MSG_DATA, payload,
                              (uint8_t)(2u + chunk));
    return;
  }

  if (!app->done_acked) {
    uint8_t payload[6];
    xfer_write_u16(payload, XFER_IMAGE_BYTES);
    xfer_write_u32(&payload[2], xfer_advertised_crc(app));
    xfer_set_status(app, XFER_PHASE_VERIFYING, "REQUESTING CRC32 VERIFY");
    (void)xfer_begin_reliable(app, XFER_MSG_DONE, payload, sizeof(payload));
  }
}

static void xfer_acknowledged(xfer_app_t *app) {
  uint8_t type = app->in_flight_type;
  uint8_t length = app->in_flight_length;
  uint8_t payload[XFER_MAX_PAYLOAD];
  memcpy(payload, app->in_flight_payload, length);
  app->in_flight = false;

  if (type == XFER_MSG_HELLO) {
    app->hello_acked = true;
    xfer_set_status(app, XFER_PHASE_NEGOTIATING,
                    app->peer_hello ? "LINK READY - 115200 8N1"
                                    : "HELLO ACK - WAITING FOR PEER");
  } else if (type == XFER_MSG_METADATA) {
    app->metadata_acked = true;
    app->next_action_tick = app->tick + XFER_BLOCK_PACE_TICKS;
    xfer_set_status(app, XFER_PHASE_TRANSFERRING, "MANIFEST ACCEPTED");
  } else if (type == XFER_MSG_DATA && length >= 2u) {
    uint16_t offset = xfer_read_u16(payload);
    uint8_t chunk = (uint8_t)(length - 2u);
    if (offset == app->sent_bytes) {
      app->sent_bytes = (uint16_t)(app->sent_bytes + chunk);
      app->blocks_acked++;
      app->next_action_tick = app->tick + XFER_BLOCK_PACE_TICKS;
    }
  } else if (type == XFER_MSG_DONE) {
    app->done_acked = true;
    app->transfer_verified = true;
    app->complete_tick = app->tick;
    xfer_set_status(app, XFER_PHASE_COMPLETE, "TRANSFER VERIFIED");
  } else if (type == XFER_MSG_FAIL) {
    app->failure_coordinated = true;
  }
  xfer_drive_sender(app);
}

static xfer_process_result_t
xfer_process_message(xfer_app_t *app, uint8_t type, const uint8_t *payload,
                     uint8_t length) {
  switch (type) {
  case XFER_MSG_HELLO:
    if (length != 2u || payload[0] != XFER_PROTOCOL_VERSION ||
        (payload[1] != 0u) == app->is_sender) {
      return XFER_PROCESS_REJECT;
    }
    app->peer_hello = true;
    xfer_set_status(app, XFER_PHASE_NEGOTIATING,
                    app->hello_acked ? "LINK READY - 115200 8N1"
                                     : "PEER HELLO - WAITING FOR ACK");
    return XFER_PROCESS_ACCEPT;
  case XFER_MSG_METADATA:
    if (app->is_sender || length != 8u || payload[0] != XFER_IMAGE_W ||
        payload[1] != XFER_IMAGE_H ||
        xfer_read_u16(&payload[2]) != XFER_IMAGE_BYTES) {
      return XFER_PROCESS_REJECT;
    }
    app->expected_bytes = xfer_read_u16(&payload[2]);
    app->expected_crc32 = xfer_read_u32(&payload[4]);
    app->received_bytes = 0;
    app->blocks_received = 0;
    memset(app->received, 0, sizeof(app->received));
    app->metadata_received = true;
    xfer_set_status(app, XFER_PHASE_TRANSFERRING,
                    "MANIFEST ACCEPTED - READY");
    return XFER_PROCESS_ACCEPT;
  case XFER_MSG_DATA: {
    if (app->is_sender || !app->metadata_received || length < 3u ||
        length > 2u + XFER_CHUNK_BYTES) {
      return XFER_PROCESS_REJECT;
    }
    uint16_t offset = xfer_read_u16(payload);
    uint8_t chunk = (uint8_t)(length - 2u);
    if (offset != app->received_bytes ||
        (uint16_t)(offset + chunk) > app->expected_bytes) {
      return XFER_PROCESS_REJECT;
    }
    memcpy(&app->received[offset], &payload[2], chunk);
    app->received_bytes = (uint16_t)(app->received_bytes + chunk);
    app->blocks_received++;
    if (app->simulate_payload_corruption &&
        !app->payload_corruption_used &&
        app->received_bytes == app->expected_bytes) {
      app->received[XFER_FAULT_OFFSET] ^= 1u;
      app->payload_corruption_used = true;
      lcom_printf("serial transfer simulated payload corruption offset=%u\n",
                  XFER_FAULT_OFFSET);
    }
    char status[48];
    snprintf(status, sizeof(status), "RECEIVED BLOCK %02u OF %02u",
             (unsigned)app->blocks_received, (unsigned)XFER_BLOCK_COUNT);
    xfer_set_status(app, XFER_PHASE_TRANSFERRING, status);
    return XFER_PROCESS_ACCEPT;
  }
  case XFER_MSG_DONE: {
    if (app->is_sender || !app->metadata_received || length != 6u) {
      return XFER_PROCESS_REJECT;
    }
    uint16_t total = xfer_read_u16(payload);
    uint32_t announced_crc = xfer_read_u32(&payload[2]);
    uint32_t received_crc = xfer_crc32(app->received, app->received_bytes);
    if (total != app->expected_bytes || total != app->received_bytes ||
        announced_crc != app->expected_crc32 ||
        received_crc != app->expected_crc32) {
      app->rejected++;
      xfer_enter_receiver_failure(app, XFER_FAIL_CRC,
                                  "CRC32 MISMATCH - ABORTING");
      return XFER_PROCESS_TERMINAL;
    }
    app->transfer_verified = true;
    app->complete_tick = app->tick;
    xfer_set_status(app, XFER_PHASE_COMPLETE, "CRC32 MATCH - FILE COMPLETE");
    return XFER_PROCESS_ACCEPT;
  }
  case XFER_MSG_FAIL:
    if (!app->is_sender || length != 2u ||
        (payload[0] != XFER_FAIL_CRC &&
         payload[0] != XFER_FAIL_PEER_TIMEOUT)) {
      return XFER_PROCESS_REJECT;
    }
    app->in_flight = false;
    app->terminal_failure = true;
    app->failure_coordinated = true;
    app->failure_reason = payload[0];
    app->failure_tick = app->tick;
    app->remote_failures++;
    xfer_set_status(app, XFER_PHASE_FAILED,
                    payload[0] == XFER_FAIL_CRC
                        ? "PEER REJECTED CRC32"
                        : "PEER REPORTED TIMEOUT");
    return XFER_PROCESS_ACCEPT;
  default:
    return XFER_PROCESS_REJECT;
  }
}

static bool xfer_sequence_is_older(uint8_t sequence, uint8_t expected) {
  uint8_t distance = (uint8_t)(expected - sequence);
  return distance != 0u && distance < 128u;
}

static void xfer_receive_frame(xfer_app_t *app, uint8_t type, uint8_t sequence,
                               const uint8_t *payload, uint8_t length) {
  app->peer_activity = true;
  app->last_peer_frame_tick = app->tick;

  if (type == XFER_MSG_ACK) {
    if (length == 0u && app->in_flight &&
        sequence == app->in_flight_sequence) {
      xfer_acknowledged(app);
    }
    return;
  }
  if (type == XFER_MSG_NACK) {
    app->nacks++;
    if (length == 0u && app->in_flight &&
        sequence == app->in_flight_sequence &&
        app->in_flight_attempts < XFER_MAX_ATTEMPTS) {
      app->in_flight_attempts++;
      app->retries++;
      xfer_set_status(app, app->phase, "NACK RECEIVED - RETRANSMIT");
      xfer_send_current(app);
    }
    return;
  }

  if (sequence == app->rx_expected) {
    xfer_process_result_t processed =
        xfer_process_message(app, type, payload, length);
    if (processed == XFER_PROCESS_ACCEPT) {
      app->rx_expected++;
      if (type == XFER_MSG_DONE && !app->is_sender &&
          app->simulate_done_ack_loss && !app->done_ack_drop_used) {
        app->done_ack_drop_used = true;
        lcom_printf("serial transfer simulated DONE ACK loss sequence=%u\n",
                    sequence);
      } else {
        (void)xfer_uart_queue_frame(&app->uart, XFER_MSG_ACK, sequence, NULL, 0);
      }
      xfer_drive_sender(app);
    } else if (processed == XFER_PROCESS_REJECT) {
      app->rejected++;
      app->nacks++;
      (void)xfer_uart_queue_frame(&app->uart, XFER_MSG_NACK,
                                  app->rx_expected, NULL, 0);
    }
  } else if (xfer_sequence_is_older(sequence, app->rx_expected)) {
    app->duplicates++;
    (void)xfer_uart_queue_frame(&app->uart, XFER_MSG_ACK, sequence, NULL, 0);
  } else {
    app->out_of_order++;
    app->nacks++;
    (void)xfer_uart_queue_frame(&app->uart, XFER_MSG_NACK,
                                app->rx_expected, NULL, 0);
  }
}

static void xfer_tick(xfer_app_t *app) {
  if (app->in_flight &&
      app->tick - app->in_flight_tick >= XFER_RETRY_TICKS) {
    if (app->in_flight_attempts < XFER_MAX_ATTEMPTS) {
      app->in_flight_attempts++;
      app->retries++;
      xfer_set_status(app, app->phase, "TIMEOUT - RETRANSMITTING");
      xfer_send_current(app);
    } else {
      app->in_flight = false;
      app->running = false;
      xfer_set_status(app, XFER_PHASE_FAILED, "RETRY BUDGET EXHAUSTED");
    }
  }

  if (!app->is_sender && !app->terminal_failure &&
      app->phase != XFER_PHASE_COMPLETE && app->peer_hello &&
      app->hello_acked && app->peer_activity &&
      app->tick - app->last_peer_frame_tick >= XFER_PEER_TIMEOUT_TICKS) {
    xfer_enter_receiver_failure(app, XFER_FAIL_PEER_TIMEOUT,
                                "PEER INACTIVE - ABORTING");
  }

  xfer_drive_sender(app);
  xfer_uart_tx_pump(&app->uart);
  if (!app->keep_open && !app->show_help && app->phase == XFER_PHASE_COMPLETE &&
      app->tick - app->complete_tick >= XFER_COMPLETE_GRACE_TICKS &&
      app->uart.tx_head == app->uart.tx_tail) {
    app->running = false;
  }
  if (!app->keep_open && !app->show_help && app->terminal_failure && !app->in_flight &&
      app->tick - app->failure_tick >= XFER_FAILURE_GRACE_TICKS &&
      app->uart.tx_head == app->uart.tx_tail) {
    app->running = false;
  }
}

static uint32_t xfer_progress_bytes(const xfer_app_t *app) {
  return app->is_sender ? app->sent_bytes : app->received_bytes;
}

static uint8_t xfer_progress_blocks(const xfer_app_t *app) {
  return app->is_sender ? app->blocks_acked : app->blocks_received;
}

static void xfer_draw_sprite(xfer_app_t *app, int origin_x, int origin_y,
                             bool received_view) {
  example_canvas_t *canvas = &app->canvas;
  const int cell = 10;
  uint16_t available = received_view ? app->received_bytes : XFER_IMAGE_BYTES;
  for (uint8_t y = 0; y < XFER_IMAGE_H; y++) {
    for (uint8_t x = 0; x < XFER_IMAGE_W; x++) {
      uint16_t index = (uint16_t)y * XFER_IMAGE_W + x;
      uint32_t color;
      if (index >= available) {
        color = ((x + y) & 1u) != 0u ? example_rgb(45, 57, 75)
                                     : example_rgb(31, 42, 58);
      } else {
        uint8_t value = received_view ? app->received[index] : app->asset[index];
        color = xfer_palette[value < 6u ? value : 0u];
      }
      example_fill_rect(canvas, origin_x + (int)x * cell,
                        origin_y + (int)y * cell, cell - 1, cell - 1, color);
    }
  }
  example_stroke_rect(canvas, origin_x - 3, origin_y - 3,
                      (int)XFER_IMAGE_W * cell + 6,
                      (int)XFER_IMAGE_H * cell + 6, 2,
                      example_rgb(83, 103, 130));
}

static void xfer_draw_block_ledger(xfer_app_t *app, int x, int y) {
  example_canvas_t *canvas = &app->canvas;
  uint8_t complete = xfer_progress_blocks(app);
  for (uint8_t block = 0; block < XFER_BLOCK_COUNT; block++) {
    int col = block % 8u;
    int row = block / 8u;
    uint32_t color = block < complete ? example_rgb(18, 165, 148)
                                      : example_rgb(45, 57, 75);
    if (app->in_flight && app->in_flight_type == XFER_MSG_DATA &&
        xfer_read_u16(app->in_flight_payload) / XFER_CHUNK_BYTES == block) {
      color = example_rgb(244, 185, 66);
    }
    example_fill_rect(canvas, x + col * 39, y + row * 18, 32, 11, color);
  }
}

static bool xfer_over(const xfer_app_t *app, int x, int y, int w, int h) {
  return app->mouse_x >= x && app->mouse_x < x + w && app->mouse_y >= y && app->mouse_y < y + h;
}

static void xfer_render(xfer_app_t *app) {
  example_canvas_t *canvas = &app->canvas;
  uint32_t accent = app->is_sender ? 0x77B8FF : 0x6BD4BB;
  uint32_t progress = xfer_progress_bytes(app);
  bool failed = app->phase == XFER_PHASE_FAILED;
  bool complete = app->phase == XFER_PHASE_COMPLETE;
  example_fill_rect(canvas, 0, 0, 800, 600, UI_DARK);
  example_heading(canvas, "Serial Transfer", "A sprite, delivered one verified block at a time.", accent, true);
  example_label(canvas, 544, 24, app->is_sender ? "Sender" : "Receiver", accent, 1);
  example_dark_button(canvas, 680, 20, 96, 34, "Help  F1", false, xfer_over(app, 680, 20, 96, 34), true);
  example_label(canvas, 28, 100, app->is_sender ? "Source sprite" : "Received sprite", UI_LIGHT, 1);
  xfer_draw_sprite(app, 58, 166, !app->is_sender);
  example_label(canvas, 28, 348, "16 x 16 indexed color", UI_DIM, 1);
  char line[128];
  snprintf(line, sizeof(line), "CRC32  %08X", app->is_sender ? app->asset_crc32 : app->expected_crc32);
  example_mono(canvas, 28, 380, line, UI_DIM);
  const char *titles[] = {"Connecting to peer", "Preparing sprite", "Sending blocks", "Verifying delivery", "Transfer complete", "Transfer failed"};
  example_label(canvas, 280, 100, (app->phase == XFER_PHASE_TRANSFERRING && !app->is_sender ? "Receiving blocks" : titles[app->phase]), failed ? 0xFFAB9A : UI_LIGHT, 2);
  example_label(canvas, 282, 150, app->status, failed ? 0xFFAB9A : accent, 1);
  const char *steps[] = {"Connect", "Prepare", "Transfer", "Verify"};
  int stage = app->phase == XFER_PHASE_NEGOTIATING ? 0 : app->phase == XFER_PHASE_METADATA ? 1 : app->phase == XFER_PHASE_TRANSFERRING ? 2 : 3;
  for (int step = 0; step < 4; step++) {
    int x = 282 + step * 122;
    example_fill_rect(canvas, x, 196, 108, 3, !failed && (step < stage || complete) ? accent : 0x34455A);
    example_label(canvas, x, 208, steps[step], step == stage || complete ? UI_LIGHT : UI_DIM, 1);
  }
  example_round_rect(canvas, 282, 252, 490, 10, 5, 0x34455A);
  if (progress) example_round_rect(canvas, 282, 252, (int)(progress * 490u / XFER_IMAGE_BYTES), 10, 5, failed ? 0xFFAB9A : accent);
  snprintf(line, sizeof(line), "%u / %u bytes", progress, XFER_IMAGE_BYTES);
  example_mono(canvas, 282, 276, line, UI_LIGHT);
  snprintf(line, sizeof(line), "%u%%", progress * 100u / XFER_IMAGE_BYTES);
  example_mono(canvas, 772 - (int)strlen(line) * 9, 276, line, accent);
  snprintf(line, sizeof(line), "%s blocks  %u / %u", app->is_sender ? "Acknowledged" : "Received", xfer_progress_blocks(app), XFER_BLOCK_COUNT);
  example_label(canvas, 282, 312, line, UI_LIGHT, 1);
  xfer_draw_block_ledger(app, 282, 342);
  snprintf(line, sizeof(line), "%u %s   %u NACKs", app->retries, app->retries == 1u ? "retry" : "retries", app->nacks);
  example_label(canvas, 614, 342, line, UI_DIM, 1);
  example_label(canvas, 614, 372, complete ? "CRC verified" : failed ? "Not verified" : "Checking each block", complete ? accent : UI_DIM, 1);
  const char *notice = failed ? "Delivery failed. Restart the pair to try again." : complete ? "All bytes received and CRC32 matched." : app->retries ? "A block was retried. Delivery is continuing." : "Acknowledgements keep both peers in agreement.";
  if (app->simulate_fault) notice = complete ? "Dropped frame recovered. All bytes verified." : app->retries ? "Dropped frame retried. Delivery is continuing." : app->fault_used ? "A frame was dropped. Waiting for a retry." : "Resilience demo: one data frame will be dropped.";
  example_label(canvas, 282, 422, notice, failed ? 0xFFAB9A : UI_DIM, 1);
  example_fill_rect(canvas, 28, 462, 744, 1, 0x354456);
  snprintf(line, sizeof(line), "COM1  115200 baud  8N1     TX %u frames   RX %u frames", app->uart.frames_tx, app->uart.frames_rx);
  example_label(canvas, 28, 480, line, UI_DIM, 1);
  snprintf(line, sizeof(line), "CRC errors %u     Line errors %u     Duplicate blocks %u", app->uart.crc_errors, app->uart.line_errors, app->duplicates);
  example_label(canvas, 28, 512, line, UI_DIM, 1);
  example_dark_button(canvas, 28, 552, 184, 34, "Keep open  Space", app->keep_open, xfer_over(app, 28, 552, 184, 34), true);
  example_label(canvas, 230, 560, app->keep_open ? "Stays open after delivery. Esc closes." : "Closes automatically after delivery.", UI_DIM, 1);
  if (app->show_help) {
    example_round_rect(canvas, 98, 140, 604, 322, 12, UI_PANEL);
    example_label(canvas, 122, 156, "Transfer controls", UI_LIGHT, 2);
    example_button(canvas, 588, 154, 100, 32, "Close  F1", false, xfer_over(app, 588, 154, 100, 32), true);
    const char *lines[] = {"Transfer begins automatically when peers connect.", "Each block is acknowledged. Lost frames are retried.", "Space   Keep the result open after delivery", "Esc / Cmd or Ctrl + W   Close this peer", "F1 / Esc   Close help first", "Closing a peer before delivery cancels its transfer."};
    for (int i = 0; i < 6; i++) example_label(canvas, 124, 218 + i * 34, lines[i], UI_DIM, 1);
  }
  if (app->mouse_ack) {
    for (int row = 0; row < 14; row++) example_fill_rect(canvas, app->mouse_x, app->mouse_y + row, row < 10 ? row / 2 + 1 : 3, 1, UI_LIGHT);
  }
}

static void xfer_handle_key(xfer_app_t *app, uint8_t byte) {
  if (example_modifier_byte(&app->modifiers, byte)) { app->extended_key = false; return; }
  if (byte == KBD_TWO_BYTE_PFX) { app->extended_key = true; return; }
  if (app->extended_key) { app->extended_key = false; return; }
  if (byte == ESC_BREAK) {
    if (app->show_help) { app->show_help = false; return; }
    if (app->phase != XFER_PHASE_COMPLETE) xfer_set_status(app, XFER_PHASE_FAILED, "TRANSFER CANCELLED ON THIS PEER");
    app->running = false;
    return;
  }
  if (byte & KBD_BREAK_BIT) return;
  if (example_primary(&app->modifiers)) {
    if (byte == 0x11u) {
      if (app->phase != XFER_PHASE_COMPLETE) xfer_set_status(app, XFER_PHASE_FAILED, "TRANSFER CANCELLED ON THIS PEER");
      app->running = false;
    }
    return;
  }
  if (byte == 0x3Bu) app->show_help = !app->show_help;
  else if (!app->show_help && byte == 0x39u) app->keep_open = !app->keep_open;
}

static void xfer_service_input(xfer_app_t *app) {
  for (int n = 0; n < 32; n++) {
    uint8_t status = 0, byte = 0;
    if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK || !(status & KBC_ST_OBF)) return;
    if (lcom_port_read8(KBC_OUT_BUF, &byte) != LCOM_OK) return;
    if (status & KBC_ST_ERR) continue;
    if (!(status & KBC_ST_AUX)) { xfer_handle_key(app, byte); continue; }
    if (!app->mouse_ack && byte == MOUSE_ACK) { app->mouse_ack = true; continue; }
    if (!app->mouse_index && !(byte & MOUSE_SYNC_BIT)) continue;
    app->mouse_bytes[app->mouse_index++] = byte;
    if (app->mouse_index < 3u) continue;
    app->mouse_index = 0;
    uint8_t flags = app->mouse_bytes[0];
    if (flags & (BIT(6) | BIT(7))) continue;
    int dx = (flags & BIT(4)) ? (int)app->mouse_bytes[1] - 256 : app->mouse_bytes[1];
    int dy = (flags & BIT(5)) ? (int)app->mouse_bytes[2] - 256 : app->mouse_bytes[2];
    app->mouse_x += dx; app->mouse_y -= dy;
    if (app->mouse_x < 0) app->mouse_x = 0;
    if (app->mouse_x > 799) app->mouse_x = 799;
    if (app->mouse_y < 0) app->mouse_y = 0;
    if (app->mouse_y > 599) app->mouse_y = 599;
    bool pressed = (flags & BIT(0)) && !(app->mouse_buttons & BIT(0));
    app->mouse_buttons = flags & 7u;
    if (!pressed) continue;
    if (app->show_help) {
      if (xfer_over(app, 588, 154, 100, 32)) app->show_help = false;
    } else if (xfer_over(app, 680, 20, 96, 34)) app->show_help = true;
    else if (xfer_over(app, 28, 552, 184, 34)) app->keep_open = !app->keep_open;
  }
}


static bool xfer_parse_role(int argc, char **argv, xfer_app_t *app) {
  const char *role = argc > 1 ? argv[1] : NULL;
  if (role == NULL) {
    const char *side = getenv("MACHINE_LAB_PAIR_SIDE");
    if (side != NULL) role = strcmp(side, "left") == 0 ? "send" : "receive";
  }
  if (role == NULL) return false;
  if (strcmp(role, "send") == 0 || strcmp(role, "sender") == 0) {
    app->is_sender = true;
  } else if (strcmp(role, "receive") == 0 ||
             strcmp(role, "receiver") == 0) {
    app->is_sender = false;
  } else {
    return false;
  }

  if (argc <= 2) return true;
  if (strcmp(argv[2], "fault") == 0 && app->is_sender) {
    app->simulate_fault = true;
  } else if (strcmp(argv[2], "badcrc") == 0 && app->is_sender) {
    app->simulate_bad_crc = true;
  } else if (strcmp(argv[2], "corrupt") == 0 && !app->is_sender) {
    app->simulate_payload_corruption = true;
  } else if (strcmp(argv[2], "stall") == 0 && app->is_sender) {
    app->simulate_stall = true;
  } else if (strcmp(argv[2], "dropack") == 0 && !app->is_sender) {
    app->simulate_done_ack_loss = true;
  } else {
    return false;
  }
  return true;
}

static const char *xfer_fault_summary(const xfer_app_t *app) {
  if (app->simulate_fault) return app->fault_used ? "drop:recovered" : "drop:armed";
  if (app->simulate_bad_crc) return "badcrc:rejected";
  if (app->simulate_payload_corruption) {
    return app->payload_corruption_used ? "payload:rejected" : "payload:armed";
  }
  if (app->simulate_stall) return app->stall_used ? "stall:timeout" : "stall:armed";
  if (app->simulate_done_ack_loss) {
    return app->done_ack_drop_used ? "done-ack:recovered" : "done-ack:armed";
  }
  return "off";
}

int main(int argc, char **argv) {
  xfer_app_t app;
  memset(&app, 0, sizeof(app));
  if (!xfer_parse_role(argc, argv, &app)) {
    fprintf(stderr,
            "usage: serial_transfer send [fault|badcrc|stall] | "
            "receive [corrupt|dropack]\n");
    return 2;
  }
  xfer_build_asset(&app);
  app.running = true;
  app.mouse_x = 400; app.mouse_y = 300;
  xfer_set_status(&app, XFER_PHASE_NEGOTIATING, "SENDING HELLO");

  int result = 1;
  if (lcom_init() != LCOM_OK) return 1;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &app.mode_info) != LCOM_OK ||
      lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK) {
    goto cleanup;
  }
  if (lcom_phys_map(app.mode_info.framebuffer_phys, app.mode_info.framebuffer_size,
                    (void **)&app.framebuffer) != LCOM_OK) {
    goto cleanup;
  }
  app.canvas.fb = app.framebuffer;
  app.canvas.info = app.mode_info;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &app.timer_irq) != LCOM_OK) goto cleanup;
  app.timer_subscribed = true;
  if (xfer_uart_start(&app.uart) != LCOM_OK) goto cleanup;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &app.keyboard_irq) == LCOM_OK) app.keyboard_subscribed = true;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &app.mouse_irq) == LCOM_OK) {
    app.mouse_subscribed = true;
    app.mouse_reporting = lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) == LCOM_OK && lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_ENABLE_DR) == LCOM_OK;
  }

  xfer_render(&app);
  (void)lcom_vbe_present();

  while (app.running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      xfer_set_status(&app, XFER_PHASE_FAILED, "EVENT WAIT FAILED");
      break;
    }
    if ((app.keyboard_subscribed && (event.irq_mask & app.keyboard_irq.mask)) || (app.mouse_subscribed && (event.irq_mask & app.mouse_irq.mask))) {
      xfer_service_input(&app);
      xfer_render(&app);
      (void)lcom_vbe_present();
    }
    if ((event.irq_mask & app.uart.irq.mask) != 0u) xfer_uart_service(&app);
    if ((event.irq_mask & app.timer_irq.mask) != 0u) {
      app.tick = (uint32_t)event.tick;
      // Wait for the first timer tick before transmitting. Virtual ticks only
      // advance once BOTH paired guests have finished initializing and entered
      // their event loops, so the peer's UART is guaranteed configured here.
      // Sending earlier can race the peer's FIFO setup and drop the HELLO,
      // which made retry counters depend on host scheduling.
      if (!app.hello_sent) {
        xfer_send_hello(&app);
        app.hello_sent = true;
      }
      xfer_tick(&app);
      xfer_render(&app);
      (void)lcom_vbe_present();
    }
  }

  xfer_render(&app);
  (void)lcom_vbe_present();
  uint32_t final_crc = app.is_sender
                           ? app.asset_crc32
                           : xfer_crc32(app.received, app.received_bytes);
  if (app.phase == XFER_PHASE_COMPLETE && app.transfer_verified) {
    lcom_printf(
        "serial transfer role=%s complete bytes=%u crc32=%08X blocks=%u "
        "retries=%u nacks=%u duplicates=%u fault=%s\n",
        app.is_sender ? "sender" : "receiver",
        app.is_sender ? app.sent_bytes : app.received_bytes,
        (unsigned)final_crc,
        app.is_sender ? app.blocks_acked : app.blocks_received,
        app.retries, app.nacks, app.duplicates, xfer_fault_summary(&app));
    result = 0;
  } else {
    lcom_printf(
        "serial transfer role=%s failed bytes=%u retries=%u nacks=%u "
        "rejected=%u reason=%u coordinated=%s fault=%s\n",
        app.is_sender ? "sender" : "receiver",
        app.is_sender ? app.sent_bytes : app.received_bytes,
        app.retries, app.nacks, app.rejected, app.failure_reason,
        app.failure_coordinated ? "yes" : "no", xfer_fault_summary(&app));
  }

cleanup:
  if (app.mouse_reporting) {
    (void)lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE);
    (void)lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_DISABLE_DR);
  }
  if (app.keyboard_subscribed) (void)lcom_irq_unsubscribe(&app.keyboard_irq);
  if (app.mouse_subscribed) (void)lcom_irq_unsubscribe(&app.mouse_irq);
  xfer_uart_stop(&app.uart);
  if (app.timer_subscribed) (void)lcom_irq_unsubscribe(&app.timer_irq);
  if (app.framebuffer != NULL) {
    (void)lcom_phys_unmap(app.framebuffer, app.mode_info.framebuffer_size);
  }
  lcom_exit();
  return result;
}
