#include "example_ui.h"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/uart16550.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WB_PROTOCOL_VERSION 1u
#define WB_MAGIC_0 0xA5u
#define WB_MAGIC_1 0x5Au
#define WB_MAX_PAYLOAD 10u
#define WB_FRAME_OVERHEAD 6u

#define WB_SCREEN_W 800
#define WB_SCREEN_H 600
#define WB_BOARD_X 178
#define WB_BOARD_Y 102
#define WB_BOARD_W 578
#define WB_BOARD_H 410

#define WB_MAX_SEGMENTS 120u
#define WB_MESSAGE_CAP 40u
#define WB_UART_TX_CAP 256u

#define WB_RETRY_TICKS 18u
#define WB_MAX_ATTEMPTS 5u
#define WB_HEARTBEAT_TICKS 30u
#define WB_RECONNECT_TICKS 300u
#define WB_SNAPSHOT_TICKS 240u

typedef enum {
  WB_MSG_HELLO = 1,
  WB_MSG_HELLO_ACK = 2,
  WB_MSG_STROKE = 3,
  WB_MSG_CURSOR = 4,
  WB_MSG_CLEAR = 5,
  WB_MSG_SNAPSHOT_BEGIN = 6,
  WB_MSG_SNAPSHOT_SEGMENT = 7,
  WB_MSG_SNAPSHOT_END = 8,
  WB_MSG_ACK = 0x70,
  WB_MSG_NACK = 0x71
} wb_message_type_t;

typedef enum {
  WB_RX_MAGIC_0,
  WB_RX_MAGIC_1,
  WB_RX_TYPE,
  WB_RX_SEQUENCE,
  WB_RX_LENGTH,
  WB_RX_PAYLOAD,
  WB_RX_CHECKSUM
} wb_rx_state_t;

typedef struct {
  int16_t x0;
  int16_t y0;
  int16_t x1;
  int16_t y1;
  uint8_t color;
} wb_segment_t;

typedef struct {
  uint8_t type;
  uint8_t length;
  uint8_t payload[WB_MAX_PAYLOAD];
} wb_pending_message_t;

typedef struct {
  wb_rx_state_t state;
  uint8_t type;
  uint8_t sequence;
  uint8_t length;
  uint8_t index;
  uint8_t checksum;
  uint8_t payload[WB_MAX_PAYLOAD];
} wb_frame_parser_t;

typedef struct {
  lcom_irq_t irq;
  bool subscribed;
  uint8_t tx[WB_UART_TX_CAP];
  uint16_t tx_head;
  uint16_t tx_tail;
  bool tx_irq_enabled;
  wb_frame_parser_t parser;
  uint32_t bytes_tx;
  uint32_t bytes_rx;
  uint32_t frames_tx;
  uint32_t frames_rx;
  uint32_t crc_errors;
  uint32_t line_errors;
  uint32_t tx_overflows;
} wb_uart_t;

typedef struct {
  wb_pending_message_t queue[WB_MESSAGE_CAP];
  uint8_t queue_head;
  uint8_t queue_tail;
  uint8_t queue_count;
  bool in_flight;
  wb_pending_message_t current;
  uint8_t current_sequence;
  uint8_t current_attempts;
  uint8_t tx_sequence;
  uint8_t rx_expected;
  uint16_t local_session;
  uint16_t peer_session;
  bool have_peer_session;
  bool peer_sequence_reset_latched;
  bool connected;
  uint32_t sent_tick;
  uint32_t last_rx_tick;
  uint32_t last_hello_tick;
  uint32_t retries;
  uint32_t acknowledgements;
  uint32_t duplicates;
  uint32_t out_of_order;
  uint32_t dropped;
  uint32_t reconnects;
  uint32_t peer_session_resets;
} wb_protocol_t;

typedef struct {
  bool active;
  uint16_t generation;
  uint8_t expected_count;
  uint8_t expected_hash;
  uint8_t received;
  uint8_t running_hash;
  wb_segment_t segments[WB_MAX_SEGMENTS];
  wb_segment_t post[WB_MAX_SEGMENTS];
  uint8_t post_count;
} wb_snapshot_rx_t;

typedef struct {
  bool active;
  bool begin_queued;
  bool end_queued;
  uint16_t generation;
  uint8_t count;
  uint8_t index;
  uint8_t hash;
  wb_segment_t segments[WB_MAX_SEGMENTS];
} wb_snapshot_tx_t;

typedef struct {
  example_canvas_t canvas;
  uint8_t *framebuffer;
  lcom_vbe_mode_info_t mode_info;
  lcom_irq_t timer_irq;
  lcom_irq_t keyboard_irq;
  lcom_irq_t mouse_irq;
  bool timer_subscribed;
  bool keyboard_subscribed;
  bool mouse_subscribed;
  bool mouse_reporting;
  wb_uart_t uart;
  wb_protocol_t protocol;
  wb_segment_t local_segments[WB_MAX_SEGMENTS];
  wb_segment_t remote_segments[WB_MAX_SEGMENTS];
  uint8_t local_count;
  uint8_t remote_count;
  wb_snapshot_rx_t snapshot_rx;
  wb_snapshot_tx_t snapshot_tx;
  uint16_t generation;
  int cursor_x;
  int cursor_y;
  int remote_cursor_x;
  int remote_cursor_y;
  bool remote_cursor_visible;
  uint8_t remote_buttons;
  uint8_t mouse_bytes[3];
  uint8_t mouse_index;
  bool left_button;
  bool cursor_dirty;
  uint8_t selected_color;
  bool local_is_left;
  bool simulate_same_session_restart;
  bool simulated_restart_used;
  bool extended_key;
  bool show_help;
  bool running;
  bool event_error;
  bool render_dirty;
  uint32_t tick;
  uint32_t last_cursor_tick;
  uint32_t last_snapshot_tick;
  uint32_t local_strokes_total;
  uint32_t remote_strokes_total;
  char event_log[4][34];
} wb_app_t;

static const uint32_t wb_palette[5] = {
    0x2D7FF9u, 0xF05D5Eu, 0x12A594u, 0x9B6EF3u, 0xF3A712u};

static void wb_receive_frame(wb_app_t *app, uint8_t type, uint8_t sequence,
                             const uint8_t *payload, uint8_t length);

static int wb_clamp(int value, int low, int high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

static uint16_t wb_read_u16(const uint8_t *src) {
  return (uint16_t)src[0] | (uint16_t)((uint16_t)src[1] << 8);
}

static void wb_write_u16(uint8_t *dst, uint16_t value) {
  dst[0] = (uint8_t)(value & 0xFFu);
  dst[1] = (uint8_t)(value >> 8);
}

static uint8_t wb_crc8_update(uint8_t crc, uint8_t byte) {
  crc ^= byte;
  for (uint8_t bit = 0; bit < 8u; bit++) {
    crc = (crc & 0x80u) != 0u ? (uint8_t)((crc << 1) ^ 0x07u)
                              : (uint8_t)(crc << 1);
  }
  return crc;
}

static uint8_t wb_frame_checksum(uint8_t type, uint8_t sequence, uint8_t length,
                                 const uint8_t *payload) {
  uint8_t crc = 0;
  crc = wb_crc8_update(crc, type);
  crc = wb_crc8_update(crc, sequence);
  crc = wb_crc8_update(crc, length);
  for (uint8_t i = 0; i < length; i++) crc = wb_crc8_update(crc, payload[i]);
  return crc;
}

static bool wb_in_board(int x, int y) {
  return x >= WB_BOARD_X && x < WB_BOARD_X + WB_BOARD_W &&
         y >= WB_BOARD_Y && y < WB_BOARD_Y + WB_BOARD_H;
}

static uint8_t wb_quantize_x(int x) {
  int local = wb_clamp(x - WB_BOARD_X, 0, WB_BOARD_W - 1);
  return (uint8_t)((local * 255 + (WB_BOARD_W - 1) / 2) / (WB_BOARD_W - 1));
}

static uint8_t wb_quantize_y(int y) {
  int local = wb_clamp(y - WB_BOARD_Y, 0, WB_BOARD_H - 1);
  return (uint8_t)((local * 255 + (WB_BOARD_H - 1) / 2) / (WB_BOARD_H - 1));
}

static int16_t wb_dequantize_x(uint8_t x) {
  return (int16_t)(WB_BOARD_X + ((int)x * (WB_BOARD_W - 1) + 127) / 255);
}

static int16_t wb_dequantize_y(uint8_t y) {
  return (int16_t)(WB_BOARD_Y + ((int)y * (WB_BOARD_H - 1) + 127) / 255);
}

static void wb_log(wb_app_t *app, const char *message) {
  for (uint8_t i = 0; i < 3u; i++) {
    memcpy(app->event_log[i], app->event_log[i + 1u], sizeof(app->event_log[i]));
  }
  snprintf(app->event_log[3], sizeof(app->event_log[3]), "%s", message);
  app->render_dirty = true;
}

static uint16_t wb_uart_tx_count(const wb_uart_t *uart) {
  if (uart->tx_head >= uart->tx_tail) {
    return (uint16_t)(uart->tx_head - uart->tx_tail);
  }
  return (uint16_t)(WB_UART_TX_CAP - uart->tx_tail + uart->tx_head);
}

static uint16_t wb_uart_tx_free(const wb_uart_t *uart) {
  return (uint16_t)(WB_UART_TX_CAP - 1u - wb_uart_tx_count(uart));
}

static int wb_uart_set_tx_interrupt(wb_uart_t *uart, bool enabled) {
  if (uart->tx_irq_enabled == enabled) return LCOM_OK;
  uint8_t ier = (uint8_t)(IER_RDA | IER_RLS | (enabled ? IER_THRE : 0u));
  if (lcom_port_write8(COM1_BASE + SER_IER, ier) != LCOM_OK) return LCOM_ERR;
  uart->tx_irq_enabled = enabled;
  return LCOM_OK;
}

static void wb_uart_tx_pump(wb_uart_t *uart) {
  if (uart->tx_head == uart->tx_tail) {
    (void)wb_uart_set_tx_interrupt(uart, false);
    return;
  }

  uint8_t lsr = 0;
  if (lcom_port_read8(COM1_BASE + SER_LSR, &lsr) != LCOM_OK) return;
  if ((lsr & (LSR_OE | LSR_PE | LSR_FE | LSR_BI | LSR_FIFO_ERR)) != 0u) {
    uart->line_errors++;
  }
  if ((lsr & LSR_THRE) == 0u) {
    (void)wb_uart_set_tx_interrupt(uart, true);
    return;
  }

  uint8_t byte = uart->tx[uart->tx_tail];
  if (lcom_port_write8(COM1_BASE + SER_THR, byte) != LCOM_OK) return;
  uart->tx_tail = (uint16_t)((uart->tx_tail + 1u) % WB_UART_TX_CAP);
  uart->bytes_tx++;
  if (uart->tx_head == uart->tx_tail) {
    (void)wb_uart_set_tx_interrupt(uart, false);
  } else {
    (void)wb_uart_set_tx_interrupt(uart, true);
  }
}

static bool wb_uart_queue_frame(wb_uart_t *uart, uint8_t type, uint8_t sequence,
                                const uint8_t *payload, uint8_t length) {
  if (length > WB_MAX_PAYLOAD) return false;
  uint8_t frame[WB_FRAME_OVERHEAD + WB_MAX_PAYLOAD];
  uint8_t frame_length = (uint8_t)(WB_FRAME_OVERHEAD + length);
  if (wb_uart_tx_free(uart) < frame_length) {
    uart->tx_overflows++;
    return false;
  }

  frame[0] = WB_MAGIC_0;
  frame[1] = WB_MAGIC_1;
  frame[2] = type;
  frame[3] = sequence;
  frame[4] = length;
  if (length > 0u) memcpy(&frame[5], payload, length);
  frame[5u + length] = wb_frame_checksum(type, sequence, length, payload);

  for (uint8_t i = 0; i < frame_length; i++) {
    uart->tx[uart->tx_head] = frame[i];
    uart->tx_head = (uint16_t)((uart->tx_head + 1u) % WB_UART_TX_CAP);
  }
  uart->frames_tx++;
  (void)wb_uart_set_tx_interrupt(uart, true);
  wb_uart_tx_pump(uart);
  return true;
}

static void wb_parser_reset(wb_frame_parser_t *parser) {
  memset(parser, 0, sizeof(*parser));
  parser->state = WB_RX_MAGIC_0;
}

static void wb_parser_feed(wb_app_t *app, uint8_t byte) {
  wb_frame_parser_t *parser = &app->uart.parser;
  switch (parser->state) {
  case WB_RX_MAGIC_0:
    if (byte == WB_MAGIC_0) parser->state = WB_RX_MAGIC_1;
    break;
  case WB_RX_MAGIC_1:
    if (byte == WB_MAGIC_1) parser->state = WB_RX_TYPE;
    else parser->state = byte == WB_MAGIC_0 ? WB_RX_MAGIC_1 : WB_RX_MAGIC_0;
    break;
  case WB_RX_TYPE:
    parser->type = byte;
    parser->state = WB_RX_SEQUENCE;
    break;
  case WB_RX_SEQUENCE:
    parser->sequence = byte;
    parser->state = WB_RX_LENGTH;
    break;
  case WB_RX_LENGTH:
    parser->length = byte;
    parser->index = 0;
    if (byte > WB_MAX_PAYLOAD) {
      wb_parser_reset(parser);
    } else {
      parser->state = byte == 0u ? WB_RX_CHECKSUM : WB_RX_PAYLOAD;
    }
    break;
  case WB_RX_PAYLOAD:
    parser->payload[parser->index++] = byte;
    if (parser->index >= parser->length) parser->state = WB_RX_CHECKSUM;
    break;
  case WB_RX_CHECKSUM:
    parser->checksum = byte;
    if (byte == wb_frame_checksum(parser->type, parser->sequence, parser->length,
                                  parser->payload)) {
      app->uart.frames_rx++;
      wb_receive_frame(app, parser->type, parser->sequence, parser->payload,
                       parser->length);
    } else {
      app->uart.crc_errors++;
      (void)wb_uart_queue_frame(&app->uart, WB_MSG_NACK,
                                app->protocol.rx_expected, NULL, 0);
    }
    wb_parser_reset(parser);
    break;
  default:
    wb_parser_reset(parser);
    break;
  }
}

static void wb_uart_service(wb_app_t *app) {
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
    wb_parser_feed(app, byte);
  }
  wb_uart_tx_pump(&app->uart);
}

static int wb_uart_start(wb_uart_t *uart) {
  memset(uart, 0, sizeof(*uart));
  wb_parser_reset(&uart->parser);
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

static void wb_uart_stop(wb_uart_t *uart) {
  (void)lcom_port_write8(COM1_BASE + SER_IER, 0);
  if (uart->subscribed) {
    (void)lcom_irq_unsubscribe(&uart->irq);
    uart->subscribed = false;
  }
}

static bool wb_queue_message(wb_app_t *app, uint8_t type, const uint8_t *payload,
                             uint8_t length) {
  wb_protocol_t *protocol = &app->protocol;
  if (length > WB_MAX_PAYLOAD || protocol->queue_count >= WB_MESSAGE_CAP) {
    protocol->dropped++;
    return false;
  }
  wb_pending_message_t *message = &protocol->queue[protocol->queue_head];
  message->type = type;
  message->length = length;
  if (length > 0u) memcpy(message->payload, payload, length);
  protocol->queue_head = (uint8_t)((protocol->queue_head + 1u) % WB_MESSAGE_CAP);
  protocol->queue_count++;
  return true;
}

static void wb_send_hello(wb_app_t *app) {
  uint8_t payload[4];
  payload[0] = WB_PROTOCOL_VERSION;
  payload[1] = app->local_is_left ? 0u : 1u;
  wb_write_u16(&payload[2], app->protocol.local_session);
  if (wb_queue_message(app, WB_MSG_HELLO, payload, sizeof(payload))) {
    app->protocol.last_hello_tick = app->tick;
  }
}

static void wb_protocol_send_current(wb_app_t *app) {
  wb_protocol_t *protocol = &app->protocol;
  if (!protocol->in_flight) return;
  if (wb_uart_queue_frame(&app->uart, protocol->current.type,
                          protocol->current_sequence, protocol->current.payload,
                          protocol->current.length)) {
    protocol->sent_tick = app->tick;
  }
}

static void wb_protocol_start_next(wb_app_t *app) {
  wb_protocol_t *protocol = &app->protocol;
  if (protocol->in_flight || protocol->queue_count == 0u) return;
  protocol->current = protocol->queue[protocol->queue_tail];
  protocol->queue_tail = (uint8_t)((protocol->queue_tail + 1u) % WB_MESSAGE_CAP);
  protocol->queue_count--;
  protocol->current_sequence = protocol->tx_sequence++;
  protocol->current_attempts = 1;
  protocol->in_flight = true;
  wb_protocol_send_current(app);
}

static void wb_uart_drop_pending_tx(wb_uart_t *uart) {
  uart->tx_head = 0;
  uart->tx_tail = 0;
  (void)wb_uart_set_tx_interrupt(uart, false);
  (void)lcom_port_write8(COM1_BASE + SER_FCR,
                         FCR_ENABLE_FIFO | FCR_CLEAR_TX);
}

static void wb_protocol_reset_outbound(wb_app_t *app) {
  wb_protocol_t *protocol = &app->protocol;
  protocol->queue_head = 0;
  protocol->queue_tail = 0;
  protocol->queue_count = 0;
  protocol->in_flight = false;
  protocol->tx_sequence = 0;
  app->snapshot_tx.active = false;
  wb_uart_drop_pending_tx(&app->uart);
}

static void wb_protocol_restart(wb_app_t *app, const char *reason,
                                bool rotate_session) {
  wb_protocol_t *protocol = &app->protocol;
  protocol->reconnects++;
  protocol->connected = false;
  wb_protocol_reset_outbound(app);
  protocol->rx_expected = 0;
  if (rotate_session) protocol->local_session++;
  protocol->last_rx_tick = app->tick;
  app->snapshot_rx.active = false;
  wb_log(app, reason);
  wb_send_hello(app);
}

static void wb_protocol_tick(wb_app_t *app) {
  wb_protocol_t *protocol = &app->protocol;
  if (protocol->connected &&
      app->tick - protocol->last_rx_tick >= WB_RECONNECT_TICKS) {
    wb_protocol_restart(app, "LINK TIMEOUT - RECONNECTING", true);
  }

  if (!protocol->connected && !protocol->in_flight &&
      protocol->queue_count == 0u &&
      app->tick - protocol->last_hello_tick >= WB_HEARTBEAT_TICKS) {
    wb_send_hello(app);
  }

  if (protocol->in_flight &&
      app->tick - protocol->sent_tick >= WB_RETRY_TICKS) {
    if (protocol->current_attempts < WB_MAX_ATTEMPTS) {
      protocol->current_attempts++;
      protocol->retries++;
      wb_protocol_send_current(app);
      wb_log(app, "PACKET TIMEOUT - RETRY");
    } else {
      protocol->dropped++;
      wb_protocol_restart(app, "RETRY LIMIT - RECONNECTING", true);
    }
  }
  wb_protocol_start_next(app);
}

static void wb_append_segment(wb_segment_t *segments, uint8_t *count,
                              const wb_segment_t *segment) {
  if (*count >= WB_MAX_SEGMENTS) {
    memmove(&segments[0], &segments[1],
            (WB_MAX_SEGMENTS - 1u) * sizeof(segments[0]));
    *count = WB_MAX_SEGMENTS - 1u;
  }
  segments[*count] = *segment;
  (*count)++;
}

static void wb_encode_segment(uint8_t *payload, uint16_t generation,
                              const wb_segment_t *segment) {
  wb_write_u16(payload, generation);
  payload[2] = wb_quantize_x(segment->x0);
  payload[3] = wb_quantize_y(segment->y0);
  payload[4] = wb_quantize_x(segment->x1);
  payload[5] = wb_quantize_y(segment->y1);
  payload[6] = (uint8_t)(segment->color % 5u);
}

static wb_segment_t wb_decode_segment(const uint8_t *payload) {
  wb_segment_t segment;
  segment.x0 = wb_dequantize_x(payload[2]);
  segment.y0 = wb_dequantize_y(payload[3]);
  segment.x1 = wb_dequantize_x(payload[4]);
  segment.y1 = wb_dequantize_y(payload[5]);
  segment.color = (uint8_t)(payload[6] % 5u);
  return segment;
}

static uint8_t wb_segment_hash(uint8_t hash, uint8_t index,
                               const wb_segment_t *segment) {
  hash = wb_crc8_update(hash, index);
  hash = wb_crc8_update(hash, wb_quantize_x(segment->x0));
  hash = wb_crc8_update(hash, wb_quantize_y(segment->y0));
  hash = wb_crc8_update(hash, wb_quantize_x(segment->x1));
  hash = wb_crc8_update(hash, wb_quantize_y(segment->y1));
  return wb_crc8_update(hash, segment->color);
}

static uint8_t wb_history_hash(const wb_segment_t *segments, uint8_t count) {
  uint8_t hash = 0;
  for (uint8_t i = 0; i < count; i++) {
    hash = wb_segment_hash(hash, i, &segments[i]);
  }
  return hash;
}

static void wb_clear_canvas(wb_app_t *app, bool announce) {
  app->local_count = 0;
  app->remote_count = 0;
  app->snapshot_rx.active = false;
  app->snapshot_tx.active = false;
  app->generation++;
  app->render_dirty = true;
  if (announce) {
    uint8_t payload[2];
    wb_write_u16(payload, app->generation);
    (void)wb_queue_message(app, WB_MSG_CLEAR, payload, sizeof(payload));
    wb_log(app, "CANVAS CLEARED FOR BOTH PEERS");
  }
}

static void wb_add_local_segment(wb_app_t *app, int x0, int y0, int x1, int y1) {
  wb_segment_t segment;
  segment.x0 = (int16_t)wb_clamp(x0, WB_BOARD_X, WB_BOARD_X + WB_BOARD_W - 1);
  segment.y0 = (int16_t)wb_clamp(y0, WB_BOARD_Y, WB_BOARD_Y + WB_BOARD_H - 1);
  segment.x1 = (int16_t)wb_clamp(x1, WB_BOARD_X, WB_BOARD_X + WB_BOARD_W - 1);
  segment.y1 = (int16_t)wb_clamp(y1, WB_BOARD_Y, WB_BOARD_Y + WB_BOARD_H - 1);
  segment.color = app->selected_color;
  wb_append_segment(app->local_segments, &app->local_count, &segment);
  app->local_strokes_total++;
  app->render_dirty = true;

  uint8_t payload[7];
  wb_encode_segment(payload, app->generation, &segment);
  (void)wb_queue_message(app, WB_MSG_STROKE, payload, sizeof(payload));
}

static void wb_begin_snapshot_tx(wb_app_t *app) {
  if (app->snapshot_tx.active || !app->protocol.connected) return;
  wb_snapshot_tx_t *snapshot = &app->snapshot_tx;
  memset(snapshot, 0, sizeof(*snapshot));
  snapshot->active = true;
  snapshot->generation = app->generation;
  snapshot->count = app->local_count;
  memcpy(snapshot->segments, app->local_segments,
         (size_t)snapshot->count * sizeof(snapshot->segments[0]));
  for (uint8_t i = 0; i < snapshot->count; i++) {
    snapshot->hash = wb_segment_hash(snapshot->hash, i, &snapshot->segments[i]);
  }
  app->last_snapshot_tick = app->tick;
  wb_log(app, "PERIODIC STROKE RESYNC");
}

static void wb_snapshot_tx_tick(wb_app_t *app) {
  wb_snapshot_tx_t *snapshot = &app->snapshot_tx;
  if (!snapshot->active || app->protocol.queue_count >= 4u) return;

  if (!snapshot->begin_queued) {
    uint8_t payload[4];
    wb_write_u16(payload, snapshot->generation);
    payload[2] = snapshot->count;
    payload[3] = snapshot->hash;
    if (wb_queue_message(app, WB_MSG_SNAPSHOT_BEGIN, payload, sizeof(payload))) {
      snapshot->begin_queued = true;
    }
    return;
  }

  if (snapshot->index < snapshot->count) {
    uint8_t payload[8];
    const wb_segment_t *segment = &snapshot->segments[snapshot->index];
    wb_write_u16(payload, snapshot->generation);
    payload[2] = snapshot->index;
    payload[3] = wb_quantize_x(segment->x0);
    payload[4] = wb_quantize_y(segment->y0);
    payload[5] = wb_quantize_x(segment->x1);
    payload[6] = wb_quantize_y(segment->y1);
    payload[7] = segment->color;
    if (wb_queue_message(app, WB_MSG_SNAPSHOT_SEGMENT, payload, sizeof(payload))) {
      snapshot->index++;
    }
    return;
  }

  if (!snapshot->end_queued) {
    uint8_t payload[4];
    wb_write_u16(payload, snapshot->generation);
    payload[2] = snapshot->count;
    payload[3] = snapshot->hash;
    if (wb_queue_message(app, WB_MSG_SNAPSHOT_END, payload, sizeof(payload))) {
      snapshot->end_queued = true;
      snapshot->active = false;
    }
  }
}

static bool wb_apply_generation(wb_app_t *app, uint16_t generation) {
  if (generation == app->generation) return true;
  if ((uint16_t)(generation - app->generation) < 0x8000u) {
    app->generation = generation;
    app->local_count = 0;
    app->remote_count = 0;
    app->snapshot_rx.active = false;
    app->snapshot_tx.active = false;
    wb_log(app, "NEW CANVAS GENERATION");
    return true;
  }
  return false;
}

static bool wb_process_message(wb_app_t *app, uint8_t type,
                               const uint8_t *payload, uint8_t length) {
  wb_protocol_t *protocol = &app->protocol;
  switch (type) {
  case WB_MSG_HELLO: {
    if (length != 4u || payload[0] != WB_PROTOCOL_VERSION ||
        payload[1] == (app->local_is_left ? 0u : 1u)) {
      return false;
    }
    protocol->connected = true;
    protocol->last_rx_tick = app->tick;
    uint8_t reply[4];
    reply[0] = WB_PROTOCOL_VERSION;
    reply[1] = app->local_is_left ? 0u : 1u;
    wb_write_u16(&reply[2], protocol->local_session);
    (void)wb_queue_message(app, WB_MSG_HELLO_ACK, reply, sizeof(reply));
    wb_log(app, "PEER HELLO - LINK ONLINE");
    app->last_snapshot_tick = app->tick - WB_SNAPSHOT_TICKS;
    return true;
  }
  case WB_MSG_HELLO_ACK:
    if (length != 4u || payload[0] != WB_PROTOCOL_VERSION) return false;
    protocol->connected = true;
    protocol->last_rx_tick = app->tick;
    wb_log(app, "HELLO ACKNOWLEDGED");
    app->last_snapshot_tick = app->tick - WB_SNAPSHOT_TICKS;
    return true;
  case WB_MSG_STROKE: {
    if (length != 7u) return false;
    uint16_t generation = wb_read_u16(payload);
    if (!wb_apply_generation(app, generation)) return true;
    wb_segment_t segment = wb_decode_segment(payload);
    wb_append_segment(app->remote_segments, &app->remote_count, &segment);
    if (app->snapshot_rx.active &&
        app->snapshot_rx.generation == generation) {
      wb_append_segment(app->snapshot_rx.post,
                        &app->snapshot_rx.post_count, &segment);
    }
    app->remote_strokes_total++;
    app->render_dirty = true;
    return true;
  }
  case WB_MSG_CURSOR:
    if (length != 3u) return false;
    app->remote_cursor_x = wb_dequantize_x(payload[0]);
    app->remote_cursor_y = wb_dequantize_y(payload[1]);
    app->remote_buttons = payload[2];
    app->remote_cursor_visible = true;
    app->render_dirty = true;
    return true;
  case WB_MSG_CLEAR: {
    if (length != 2u) return false;
    uint16_t generation = wb_read_u16(payload);
    if (wb_apply_generation(app, generation)) {
      app->local_count = 0;
      app->remote_count = 0;
      app->snapshot_rx.active = false;
      app->snapshot_tx.active = false;
      wb_log(app, "PEER CLEARED THE CANVAS");
    }
    return true;
  }
  case WB_MSG_SNAPSHOT_BEGIN: {
    if (length != 4u) return false;
    uint16_t generation = wb_read_u16(payload);
    if (!wb_apply_generation(app, generation)) return true;
    memset(&app->snapshot_rx, 0, sizeof(app->snapshot_rx));
    app->snapshot_rx.active = true;
    app->snapshot_rx.generation = generation;
    app->snapshot_rx.expected_count =
        payload[2] > WB_MAX_SEGMENTS ? WB_MAX_SEGMENTS : payload[2];
    app->snapshot_rx.expected_hash = payload[3];
    return true;
  }
  case WB_MSG_SNAPSHOT_SEGMENT: {
    if (length != 8u || !app->snapshot_rx.active) return false;
    wb_snapshot_rx_t *snapshot = &app->snapshot_rx;
    if (wb_read_u16(payload) != snapshot->generation ||
        payload[2] != snapshot->received ||
        snapshot->received >= snapshot->expected_count) {
      snapshot->active = false;
      return false;
    }
    wb_segment_t segment;
    segment.x0 = wb_dequantize_x(payload[3]);
    segment.y0 = wb_dequantize_y(payload[4]);
    segment.x1 = wb_dequantize_x(payload[5]);
    segment.y1 = wb_dequantize_y(payload[6]);
    segment.color = (uint8_t)(payload[7] % 5u);
    snapshot->segments[snapshot->received] = segment;
    snapshot->running_hash =
        wb_segment_hash(snapshot->running_hash, snapshot->received, &segment);
    snapshot->received++;
    return true;
  }
  case WB_MSG_SNAPSHOT_END: {
    wb_snapshot_rx_t *snapshot = &app->snapshot_rx;
    if (length != 4u || !snapshot->active ||
        wb_read_u16(payload) != snapshot->generation ||
        payload[2] != snapshot->expected_count ||
        payload[3] != snapshot->expected_hash ||
        snapshot->received != snapshot->expected_count ||
        snapshot->running_hash != snapshot->expected_hash) {
      snapshot->active = false;
      return false;
    }
    app->remote_count = snapshot->received;
    memcpy(app->remote_segments, snapshot->segments,
           (size_t)snapshot->received * sizeof(snapshot->segments[0]));
    for (uint8_t i = 0; i < snapshot->post_count; i++) {
      wb_append_segment(app->remote_segments, &app->remote_count,
                        &snapshot->post[i]);
    }
    snapshot->active = false;
    wb_log(app, "STROKE SNAPSHOT APPLIED");
    app->render_dirty = true;
    return true;
  }
  default:
    return false;
  }
}

static bool wb_sequence_is_older(uint8_t sequence, uint8_t expected) {
  uint8_t distance = (uint8_t)(expected - sequence);
  return distance != 0u && distance < 128u;
}

static void wb_receive_frame(wb_app_t *app, uint8_t type, uint8_t sequence,
                             const uint8_t *payload, uint8_t length) {
  wb_protocol_t *protocol = &app->protocol;
  protocol->last_rx_tick = app->tick;

  if (type == WB_MSG_ACK) {
    if (length == 0u && protocol->in_flight &&
        sequence == protocol->current_sequence) {
      protocol->in_flight = false;
      protocol->acknowledgements++;
      wb_protocol_start_next(app);
    }
    return;
  }
  if (type == WB_MSG_NACK) {
    if (length == 0u && protocol->in_flight &&
        sequence == protocol->current_sequence &&
        protocol->current_attempts < WB_MAX_ATTEMPTS) {
      protocol->current_attempts++;
      protocol->retries++;
      wb_protocol_send_current(app);
    }
    return;
  }

  if (type == WB_MSG_HELLO && length == 4u &&
      payload[0] == WB_PROTOCOL_VERSION &&
      payload[1] != (app->local_is_left ? 0u : 1u)) {
    uint16_t session = wb_read_u16(&payload[2]);
    bool peer_changed =
        !protocol->have_peer_session || session != protocol->peer_session;
    bool explicit_sequence_reset =
        protocol->have_peer_session && session == protocol->peer_session &&
        sequence == 0u && protocol->rx_expected != 0u &&
        !protocol->peer_sequence_reset_latched;
    if (peer_changed || explicit_sequence_reset) {
      bool replacing_peer = protocol->have_peer_session;
      if (replacing_peer) {
        protocol->peer_session_resets++;
        wb_protocol_reset_outbound(app);
      }
      protocol->peer_session = session;
      protocol->have_peer_session = true;
      protocol->peer_sequence_reset_latched = true;
      protocol->rx_expected = sequence;
      app->snapshot_rx.active = false;
      app->remote_count = 0;
      wb_log(app, explicit_sequence_reset
                      ? "PEER SEQUENCE RESET - RESYNC"
                      : "NEW PEER SESSION - RESYNC");
    }
  }

  if (sequence == protocol->rx_expected) {
    if (wb_process_message(app, type, payload, length)) {
      protocol->rx_expected++;
      if (type != WB_MSG_HELLO) {
        protocol->peer_sequence_reset_latched = false;
      }
      (void)wb_uart_queue_frame(&app->uart, WB_MSG_ACK, sequence, NULL, 0);
    } else {
      (void)wb_uart_queue_frame(&app->uart, WB_MSG_NACK,
                                protocol->rx_expected, NULL, 0);
    }
  } else if (wb_sequence_is_older(sequence, protocol->rx_expected)) {
    protocol->duplicates++;
    (void)wb_uart_queue_frame(&app->uart, WB_MSG_ACK, sequence, NULL, 0);
  } else {
    protocol->out_of_order++;
    (void)wb_uart_queue_frame(&app->uart, WB_MSG_NACK,
                              protocol->rx_expected, NULL, 0);
  }
}

static void wb_queue_cursor(wb_app_t *app) {
  if (!app->protocol.connected) return;
  uint8_t payload[3];
  payload[0] = wb_quantize_x(app->cursor_x);
  payload[1] = wb_quantize_y(app->cursor_y);
  payload[2] = app->left_button ? 1u : 0u;
  if (wb_queue_message(app, WB_MSG_CURSOR, payload, sizeof(payload))) {
    app->cursor_dirty = false;
    app->last_cursor_tick = app->tick;
  }
}

static void wb_tick(wb_app_t *app) {
  if (app->simulate_same_session_restart && !app->simulated_restart_used &&
      app->tick >= 180u) {
    app->simulated_restart_used = true;
    wb_protocol_restart(app, "SIMULATED SAME-TOKEN RESTART", false);
  }

  if (app->protocol.connected &&
      (app->cursor_dirty ||
       app->tick - app->last_cursor_tick >= WB_HEARTBEAT_TICKS) &&
      app->protocol.queue_count < 3u) {
    wb_queue_cursor(app);
  }
  if (app->protocol.connected && !app->snapshot_tx.active &&
      app->tick - app->last_snapshot_tick >= WB_SNAPSHOT_TICKS) {
    wb_begin_snapshot_tx(app);
  }
  wb_snapshot_tx_tick(app);
  wb_protocol_tick(app);
  wb_uart_tx_pump(&app->uart);
}

static int wb_wait_mouse_ack(lcom_irq_t mouse_irq) {
  for (;;) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) return LCOM_ERR;
    if ((event.irq_mask & mouse_irq.mask) == 0u) continue;
    uint8_t status = 0;
    if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK) return LCOM_ERR;
    if ((status & KBC_ST_OBF) == 0u || (status & KBC_ST_AUX) == 0u) continue;
    uint8_t reply = 0;
    if (lcom_port_read8(KBC_OUT_BUF, &reply) != LCOM_OK) return LCOM_ERR;
    return reply == MOUSE_ACK ? LCOM_OK : LCOM_ERR;
  }
}

static int wb_enable_mouse(wb_app_t *app) {
  if (lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) != LCOM_OK ||
      lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_ENABLE_DR) != LCOM_OK) {
    return LCOM_ERR;
  }
  if (wb_wait_mouse_ack(app->mouse_irq) != LCOM_OK) return LCOM_ERR;
  app->mouse_reporting = true;
  return LCOM_OK;
}

static void wb_disable_mouse(wb_app_t *app) {
  if (!app->mouse_reporting) return;
  (void)lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE);
  (void)lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_DISABLE_DR);
  app->mouse_reporting = false;
}

static int16_t wb_mouse_delta(uint8_t flags, uint8_t byte, uint8_t sign_bit) {
  if ((flags & BIT(sign_bit)) != 0u) {
    return (int16_t)(0xFF00u | byte);
  }
  return (int16_t)byte;
}

static void wb_select_palette_at(wb_app_t *app, int x, int y) {
  if (x < 42 || x >= 138 || y < 218 || y >= 346) return;
  for (uint8_t i = 0; i < 5u; i++) {
    int swatch_y = 224 + (int)i * 24;
    if (y >= swatch_y && y < swatch_y + 18) {
      app->selected_color = i;
      wb_log(app, "INK COLOR CHANGED");
      return;
    }
  }
}

static void wb_handle_mouse_packet(wb_app_t *app, const uint8_t *packet) {
  if ((packet[0] & (BIT(6) | BIT(7))) != 0u) return;
  int old_x = app->cursor_x;
  int old_y = app->cursor_y;
  bool old_left = app->left_button;
  int16_t dx = wb_mouse_delta(packet[0], packet[1], 4);
  int16_t dy = wb_mouse_delta(packet[0], packet[2], 5);
  app->cursor_x = wb_clamp(app->cursor_x + dx, 0, WB_SCREEN_W - 1);
  app->cursor_y = wb_clamp(app->cursor_y - dy, 0, WB_SCREEN_H - 1);
  app->left_button = (packet[0] & BIT(0)) != 0u;
  app->cursor_dirty = true;
  app->render_dirty = true;

  if (app->left_button && wb_in_board(old_x, old_y) &&
      wb_in_board(app->cursor_x, app->cursor_y) &&
      (old_left || dx != 0 || dy != 0)) {
    wb_add_local_segment(app, old_x, old_y, app->cursor_x, app->cursor_y);
  }
  if (app->left_button && !old_left) {
    wb_select_palette_at(app, app->cursor_x, app->cursor_y);
    if (app->cursor_x >= 40 && app->cursor_x < 142 &&
        app->cursor_y >= 374 && app->cursor_y < 408) {
      wb_clear_canvas(app, true);
    }
  }
}

static void wb_feed_mouse_byte(wb_app_t *app, uint8_t byte) {
  if (app->mouse_index == 0u && (byte & MOUSE_SYNC_BIT) == 0u) return;
  app->mouse_bytes[app->mouse_index++] = byte;
  if (app->mouse_index == 3u) {
    wb_handle_mouse_packet(app, app->mouse_bytes);
    app->mouse_index = 0;
  }
}

static void wb_handle_key(wb_app_t *app, uint8_t byte) {
  if (byte == KBD_TWO_BYTE_PFX) {
    app->extended_key = true;
    return;
  }
  bool break_code = (byte & KBD_BREAK_BIT) != 0u;
  uint8_t code = (uint8_t)(byte & 0x7Fu);
  if (app->extended_key) {
    app->extended_key = false;
    return;
  }
  if (byte == ESC_BREAK) {
    app->running = false;
    return;
  }
  if (break_code) return;

  if (code >= 0x02u && code <= 0x06u) {
    app->selected_color = (uint8_t)(code - 0x02u);
    wb_log(app, "INK COLOR CHANGED");
  } else if (code == 0x2Eu) {
    wb_clear_canvas(app, true);
  } else if (code == 0x23u) {
    app->show_help = !app->show_help;
    app->render_dirty = true;
  } else if (code == 0x13u) {
    wb_protocol_restart(app, "MANUAL SESSION ROTATION", true);
  }
}

static void wb_service_ps2(wb_app_t *app) {
  for (uint8_t count = 0; count < 32u; count++) {
    uint8_t status = 0;
    if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK ||
        (status & KBC_ST_OBF) == 0u) {
      break;
    }
    uint8_t byte = 0;
    if (lcom_port_read8(KBC_OUT_BUF, &byte) != LCOM_OK) break;
    if ((status & KBC_ST_ERR) != 0u) continue;
    if ((status & KBC_ST_AUX) != 0u) wb_feed_mouse_byte(app, byte);
    else wb_handle_key(app, byte);
  }
}

static void wb_draw_disc(example_canvas_t *canvas, int cx, int cy, int radius,
                         uint32_t color) {
  for (int y = -radius; y <= radius; y++) {
    for (int x = -radius; x <= radius; x++) {
      if (x * x + y * y <= radius * radius) {
        example_put_pixel(canvas, cx + x, cy + y, color);
      }
    }
  }
}

static void wb_draw_line(example_canvas_t *canvas, const wb_segment_t *segment,
                         uint32_t color) {
  int x0 = segment->x0;
  int y0 = segment->y0;
  int x1 = segment->x1;
  int y1 = segment->y1;
  int dx = x1 > x0 ? x1 - x0 : x0 - x1;
  int sx = x0 < x1 ? 1 : -1;
  int dy_abs = y1 > y0 ? y1 - y0 : y0 - y1;
  int dy = -dy_abs;
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;
  for (;;) {
    wb_draw_disc(canvas, x0, y0, 3, color);
    if (x0 == x1 && y0 == y1) break;
    int twice = 2 * error;
    if (twice >= dy) {
      error += dy;
      x0 += sx;
    }
    if (twice <= dx) {
      error += dx;
      y0 += sy;
    }
  }
}

static void wb_draw_cursor(example_canvas_t *canvas, int x, int y, uint32_t color,
                           const char *label, bool pressed) {
  example_fill_rect(canvas, x - 10, y - 1, 21, 3, color);
  example_fill_rect(canvas, x - 1, y - 10, 3, 21, color);
  example_stroke_rect(canvas, x - 5, y - 5, 11, 11, pressed ? 3 : 1, color);
  int label_x = wb_clamp(x + 12, WB_BOARD_X + 4, WB_BOARD_X + WB_BOARD_W - 42);
  int label_y = wb_clamp(y + 10, WB_BOARD_Y + 4, WB_BOARD_Y + WB_BOARD_H - 12);
  example_draw_text(canvas, label_x, label_y, label, color, 1);
}

static void wb_draw_status_pill(example_canvas_t *canvas, int x, int y, int w,
                                const char *text, uint32_t color) {
  example_fill_rect(canvas, x, y, w, 24, color);
  example_draw_text(canvas, x + 10, y + 8, text, example_rgb(248, 250, 252), 1);
}

static void wb_render_help(wb_app_t *app) {
  example_canvas_t *canvas = &app->canvas;
  example_fill_rect(canvas, 210, 140, 430, 310, example_rgb(10, 19, 34));
  example_stroke_rect(canvas, 210, 140, 430, 310, 3, example_rgb(91, 143, 249));
  example_draw_text(canvas, 244, 168, "PAIRED WHITEBOARD HELP",
                    example_rgb(241, 245, 249), 3);
  example_draw_text(canvas, 244, 222, "DRAG         DRAW A SHARED STROKE",
                    example_rgb(203, 213, 225), 2);
  example_draw_text(canvas, 244, 254, "1-5          SELECT INK COLOR",
                    example_rgb(203, 213, 225), 2);
  example_draw_text(canvas, 244, 286, "C            CLEAR BOTH PEERS",
                    example_rgb(203, 213, 225), 2);
  example_draw_text(canvas, 244, 318, "R            ROTATE SESSION",
                    example_rgb(203, 213, 225), 2);
  example_draw_text(canvas, 244, 350, "H            CLOSE THIS HELP",
                    example_rgb(203, 213, 225), 2);
  example_draw_text(canvas, 244, 382, "ESC          LEAVE THE SESSION",
                    example_rgb(203, 213, 225), 2);
  example_draw_text(canvas, 244, 424, "ACK + RETRY + SNAPSHOT RESYNC",
                    example_rgb(114, 221, 203), 1);
}

static void wb_render(wb_app_t *app) {
  example_canvas_t *canvas = &app->canvas;
  uint32_t navy = example_rgb(13, 23, 38);
  uint32_t panel = example_rgb(23, 37, 56);
  uint32_t muted = example_rgb(148, 163, 184);
  uint32_t local_accent =
      app->local_is_left ? example_rgb(45, 127, 249) : example_rgb(155, 110, 243);
  uint32_t peer_accent =
      app->local_is_left ? example_rgb(155, 110, 243) : example_rgb(45, 127, 249);

  example_fill_rect(canvas, 0, 0, WB_SCREEN_W, WB_SCREEN_H, navy);
  example_fill_rect(canvas, 0, 0, WB_SCREEN_W, 72, example_rgb(18, 31, 49));
  example_fill_rect(canvas, 0, 69, WB_SCREEN_W, 3, local_accent);
  example_draw_text(canvas, 28, 20, "SERIAL WHITEBOARD",
                    example_rgb(248, 250, 252), 4);
  example_draw_text(canvas, 541, 20, "16 BYTE FRAMES", muted, 2);
  example_draw_text(canvas, 541, 45, "CRC8  SEQ  ACK  RESYNC",
                    example_rgb(105, 192, 180), 1);

  example_fill_rect(canvas, 24, 92, 130, 444, panel);
  example_stroke_rect(canvas, 24, 92, 130, 444, 2, example_rgb(43, 61, 83));
  example_draw_text(canvas, 42, 112,
                    app->local_is_left ? "LEFT DESK" : "RIGHT DESK",
                    example_rgb(226, 232, 240), 2);
  wb_draw_status_pill(canvas, 42, 146, 94,
                      app->protocol.connected ? "ONLINE" : "SYNCING",
                      app->protocol.connected ? example_rgb(18, 165, 148)
                                              : example_rgb(217, 119, 6));
  example_draw_text(canvas, 42, 188, "INK", muted, 1);
  for (uint8_t i = 0; i < 5u; i++) {
    int y = 224 + (int)i * 24;
    example_fill_rect(canvas, 42, y, 78, 18, wb_palette[i]);
    if (i == app->selected_color) {
      example_stroke_rect(canvas, 39, y - 3, 84, 24, 2,
                          example_rgb(248, 250, 252));
    }
    char number[2] = {(char)('1' + i), '\0'};
    example_draw_text(canvas, 128, y + 6, number, muted, 1);
  }
  example_fill_rect(canvas, 40, 374, 102, 34, example_rgb(153, 52, 68));
  example_draw_text(canvas, 58, 386, "C  CLEAR", example_rgb(255, 241, 242), 1);
  example_draw_text(canvas, 42, 430, "H  HELP", muted, 1);
  example_draw_text(canvas, 42, 452, "R  RECONNECT", muted, 1);
  example_draw_text(canvas, 42, 474, "ESC EXIT", muted, 1);

  example_fill_rect(canvas, WB_BOARD_X + 7, WB_BOARD_Y + 8,
                    WB_BOARD_W, WB_BOARD_H, example_rgb(5, 12, 23));
  example_fill_rect(canvas, WB_BOARD_X, WB_BOARD_Y,
                    WB_BOARD_W, WB_BOARD_H, example_rgb(245, 247, 250));
  example_stroke_rect(canvas, WB_BOARD_X, WB_BOARD_Y,
                      WB_BOARD_W, WB_BOARD_H, 3, example_rgb(70, 87, 109));
  for (int y = WB_BOARD_Y + 24; y < WB_BOARD_Y + WB_BOARD_H; y += 24) {
    for (int x = WB_BOARD_X + 24; x < WB_BOARD_X + WB_BOARD_W; x += 24) {
      wb_draw_disc(canvas, x, y, 1, example_rgb(210, 217, 226));
    }
  }

  for (uint8_t i = 0; i < app->remote_count; i++) {
    wb_draw_line(canvas, &app->remote_segments[i],
                 wb_palette[app->remote_segments[i].color % 5u]);
  }
  for (uint8_t i = 0; i < app->local_count; i++) {
    wb_draw_line(canvas, &app->local_segments[i],
                 wb_palette[app->local_segments[i].color % 5u]);
  }

  if (app->remote_cursor_visible) {
    wb_draw_cursor(canvas, app->remote_cursor_x, app->remote_cursor_y,
                   peer_accent, "PEER", (app->remote_buttons & 1u) != 0u);
  }
  wb_draw_cursor(canvas, app->cursor_x, app->cursor_y,
                 local_accent, "YOU", app->left_button);

  example_fill_rect(canvas, 178, 530, 578, 50, panel);
  char line[96];
  snprintf(line, sizeof(line), "TX %u  RX %u  ACK %u  RETRY %u  QUEUE %u",
           app->uart.frames_tx, app->uart.frames_rx,
           app->protocol.acknowledgements, app->protocol.retries,
           app->protocol.queue_count + (app->protocol.in_flight ? 1u : 0u));
  example_draw_text(canvas, 194, 542, line, example_rgb(203, 213, 225), 1);
  snprintf(line, sizeof(line), "LOCAL %u  PEER %u  GEN %u  RECON %u/%u  DROP %u",
           app->local_count, app->remote_count, app->generation,
           app->protocol.reconnects, app->protocol.peer_session_resets,
           app->protocol.dropped);
  example_draw_text(canvas, 194, 560, line, muted, 1);

  for (uint8_t i = 0; i < 4u; i++) {
    if (app->event_log[i][0] == '\0') continue;
    example_draw_text(canvas, 610, 84 + (int)i * 12, app->event_log[i],
                      example_rgb(71, 85, 105), 1);
  }
  if (app->show_help) wb_render_help(app);
  app->render_dirty = false;
}

static bool wb_role_from_args(int argc, char **argv, bool *is_left,
                              bool *simulate_same_session_restart) {
  const char *role = argc > 1 ? argv[1] : getenv("MACHINE_LAB_PAIR_SIDE");
  if (role == NULL) return false;
  if (strcmp(role, "left") == 0) {
    *is_left = true;
    *simulate_same_session_restart =
        argc > 2 && strcmp(argv[2], "restart") == 0;
    return argc <= 2 || *simulate_same_session_restart;
  }
  if (strcmp(role, "right") == 0) {
    *is_left = false;
    *simulate_same_session_restart =
        argc > 2 && strcmp(argv[2], "restart") == 0;
    return argc <= 2 || *simulate_same_session_restart;
  }
  return false;
}

int main(int argc, char **argv) {
  wb_app_t app;
  memset(&app, 0, sizeof(app));
  if (!wb_role_from_args(argc, argv, &app.local_is_left,
                         &app.simulate_same_session_restart)) {
    fprintf(stderr, "usage: serial_whiteboard left|right [restart]\n");
    return 2;
  }

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
  if (lcom_irq_subscribe(KBC_IRQ, 0, &app.keyboard_irq) != LCOM_OK) goto cleanup;
  app.keyboard_subscribed = true;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &app.mouse_irq) != LCOM_OK) goto cleanup;
  app.mouse_subscribed = true;
  if (wb_enable_mouse(&app) != LCOM_OK) goto cleanup;
  if (wb_uart_start(&app.uart) != LCOM_OK) goto cleanup;

  app.protocol.local_session = app.local_is_left ? 0x4C11u : 0x5211u;
  app.cursor_x = app.local_is_left ? WB_BOARD_X + WB_BOARD_W / 3
                                  : WB_BOARD_X + (2 * WB_BOARD_W) / 3;
  app.cursor_y = WB_BOARD_Y + WB_BOARD_H / 2;
  app.remote_cursor_x = WB_BOARD_X + WB_BOARD_W / 2;
  app.remote_cursor_y = WB_BOARD_Y + WB_BOARD_H / 2;
  app.selected_color = app.local_is_left ? 0u : 3u;
  app.running = true;
  app.render_dirty = true;
  wb_log(&app, "UART 115200 8N1 READY");
  wb_send_hello(&app);
  wb_protocol_tick(&app);
  wb_render(&app);
  (void)lcom_vbe_present();

  while (app.running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      app.event_error = true;
      app.protocol.connected = false;
      wb_log(&app, "EVENT WAIT FAILED");
      break;
    }
    if ((event.irq_mask & app.uart.irq.mask) != 0u) wb_uart_service(&app);
    if ((event.irq_mask & (app.keyboard_irq.mask | app.mouse_irq.mask)) != 0u) {
      wb_service_ps2(&app);
    }
    if ((event.irq_mask & app.timer_irq.mask) != 0u) {
      app.tick = (uint32_t)event.tick;
      wb_tick(&app);
      if (app.render_dirty || (app.tick % 2u) == 0u) {
        wb_render(&app);
        (void)lcom_vbe_present();
      }
    }
  }

  wb_render(&app);
  (void)lcom_vbe_present();
  uint8_t local_hash = wb_history_hash(app.local_segments, app.local_count);
  uint8_t remote_hash = wb_history_hash(app.remote_segments, app.remote_count);
  lcom_printf(
      "serial whiteboard role=%s connected=%s local_segments=%u "
      "remote_segments=%u local_hash=%02X remote_hash=%02X tx=%u rx=%u "
      "retries=%u duplicates=%u out_of_order=%u dropped=%u reconnects=%u "
      "peer_resets=%u simulated_restart=%s event_error=%s\n",
      app.local_is_left ? "left" : "right",
      app.protocol.connected ? "yes" : "no", app.local_count, app.remote_count,
      local_hash, remote_hash, app.uart.frames_tx, app.uart.frames_rx,
      app.protocol.retries,
      app.protocol.duplicates, app.protocol.out_of_order, app.protocol.dropped,
      app.protocol.reconnects, app.protocol.peer_session_resets,
      app.simulated_restart_used ? "used" : "off",
      app.event_error ? "yes" : "no");
  result = app.event_error ? 1 : 0;

cleanup:
  wb_uart_stop(&app.uart);
  wb_disable_mouse(&app);
  if (app.mouse_subscribed) (void)lcom_irq_unsubscribe(&app.mouse_irq);
  if (app.keyboard_subscribed) (void)lcom_irq_unsubscribe(&app.keyboard_irq);
  if (app.timer_subscribed) (void)lcom_irq_unsubscribe(&app.timer_irq);
  if (app.framebuffer != NULL) {
    (void)lcom_phys_unmap(app.framebuffer, app.mode_info.framebuffer_size);
  }
  lcom_exit();
  return result;
}
