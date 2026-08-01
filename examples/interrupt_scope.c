#include "example_ui.h"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/rtc.h>
#include <lcom/uart16550.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SCOPE_TICKS_PER_SECOND 60u
#define SCOPE_HISTORY 60u
#define SCOPE_EVENT_ROWS 10u
#define SCOPE_EVENT_TEXT 52u
#define SCOPE_BYTE_HISTORY 20u
#define RTC_IRQ_LINE 8u

typedef enum {
  SOURCE_TIMER,
  SOURCE_KEYBOARD,
  SOURCE_MOUSE,
  SOURCE_UART,
  SOURCE_RTC,
  SOURCE_COUNT,
} source_kind_t;

typedef struct {
  uint64_t total[SOURCE_COUNT];
  uint32_t rate[SOURCE_COUNT];
  uint32_t history[SOURCE_COUNT][SCOPE_HISTORY];
  uint8_t history_head;
  uint8_t history_count;
  uint64_t ticks;
  uint32_t mouse_packets;
  uint32_t uart_tx;
  uint32_t uart_rx;
  uint32_t uart_errors;
  uint32_t rtc_polls;
  uint64_t rtc_irq_events;
  uint8_t scancodes[SCOPE_BYTE_HISTORY];
  uint8_t scancode_head;
  uint8_t scancode_count;
  uint8_t mouse_last[3];
  uint8_t uart_last[SCOPE_BYTE_HISTORY];
  uint8_t uart_head;
  uint8_t uart_count;
  char events[SCOPE_EVENT_ROWS][SCOPE_EVENT_TEXT];
  uint8_t event_head;
  uint8_t event_count;
  uint8_t rtc_hour;
  uint8_t rtc_minute;
  uint8_t rtc_second;
  bool rtc_valid;
  bool frozen;
  bool uart_available;
  bool loopback_enabled;
  bool extended_key;
} scope_state_t;

typedef struct {
  int x;
  int y;
  uint8_t bytes[3];
  uint8_t byte_count;
  uint8_t buttons;
  uint8_t previous_buttons;
  bool acknowledged;
} scope_mouse_t;

static const char *source_names[SOURCE_COUNT] = {
    "PIT IRQ0", "KBD IRQ1", "MOUSE IRQ12", "UART IRQ4", "RTC POLL",
};

static const uint32_t source_colors[SOURCE_COUNT] = {
    0x6FD4FFu, 0xB69CFFu, 0xFFB866u, 0x61D9A9u, 0xFF7E9Du,
};

static uint8_t bcd_to_binary(uint8_t value) {
  return (uint8_t)(((value >> 4) * 10u) + (value & 0x0Fu));
}

static int rtc_read_register(uint8_t reg, uint8_t *value) {
  if (lcom_port_write8(RTC_ADDR_REG, reg) != LCOM_OK) return LCOM_ERR;
  return lcom_port_read8(RTC_DATA_REG, value);
}

static int read_rtc_time_once(uint8_t *hour, uint8_t *minute, uint8_t *second) {
  uint8_t reg_a = 0;
  uint8_t reg_b = 0;
  for (uint16_t attempt = 0; attempt < 1000u; attempt++) {
    if (rtc_read_register(RTC_REG_A, &reg_a) != LCOM_OK) return LCOM_ERR;
    if ((reg_a & RTC_UIP) == 0) break;
    if (attempt == 999u) return LCOM_TIMEOUT;
  }
  if (rtc_read_register(RTC_REG_B, &reg_b) != LCOM_OK ||
      rtc_read_register(RTC_REG_HOURS, hour) != LCOM_OK ||
      rtc_read_register(RTC_REG_MINUTES, minute) != LCOM_OK ||
      rtc_read_register(RTC_REG_SECONDS, second) != LCOM_OK) {
    return LCOM_ERR;
  }
  bool afternoon = (*hour & BIT(7)) != 0;
  *hour &= (uint8_t)~BIT(7);
  if ((reg_b & RTC_DM) == 0) {
    *hour = bcd_to_binary(*hour);
    *minute = bcd_to_binary(*minute);
    *second = bcd_to_binary(*second);
  }
  if ((reg_b & RTC_24H) == 0) {
    if (*hour == 12u) *hour = 0;
    if (afternoon) *hour = (uint8_t)(*hour + 12u);
  }
  return *hour <= 23u && *minute <= 59u && *second <= 59u ? LCOM_OK : LCOM_ERR;
}

static int read_rtc_time(uint8_t *hour, uint8_t *minute, uint8_t *second) {
  for (uint8_t attempt = 0; attempt < 4u; attempt++) {
    uint8_t first_hour = 0;
    uint8_t first_minute = 0;
    uint8_t first_second = 0;
    uint8_t second_hour = 0;
    uint8_t second_minute = 0;
    uint8_t second_second = 0;
    if (read_rtc_time_once(&first_hour, &first_minute, &first_second) != LCOM_OK ||
        read_rtc_time_once(&second_hour, &second_minute, &second_second) != LCOM_OK) {
      continue;
    }
    if (first_hour == second_hour && first_minute == second_minute &&
        first_second == second_second) {
      *hour = second_hour;
      *minute = second_minute;
      *second = second_second;
      return LCOM_OK;
    }
  }
  return LCOM_ERR;
}

static void log_event(scope_state_t *state, const char *text) {
  snprintf(state->events[state->event_head], SCOPE_EVENT_TEXT, "%s", text);
  state->event_head = (uint8_t)((state->event_head + 1u) % SCOPE_EVENT_ROWS);
  if (state->event_count < SCOPE_EVENT_ROWS) state->event_count++;
}

static void log_event_hex(scope_state_t *state, const char *label, uint8_t value) {
  char text[SCOPE_EVENT_TEXT];
  snprintf(text, sizeof(text), "T%05llu  %-12s 0x%02X",
           (unsigned long long)state->ticks, label, value);
  log_event(state, text);
}

static void record_source(scope_state_t *state, source_kind_t source) {
  state->total[source]++;
  state->rate[source]++;
}

static void push_byte(uint8_t *values, uint8_t *head, uint8_t *count,
                      uint8_t value) {
  values[*head] = value;
  *head = (uint8_t)((*head + 1u) % SCOPE_BYTE_HISTORY);
  if (*count < SCOPE_BYTE_HISTORY) (*count)++;
}

static void capture_rate_window(scope_state_t *state) {
  if (!state->frozen) {
    for (uint8_t source = 0; source < SOURCE_COUNT; source++) {
      state->history[source][state->history_head] = state->rate[source];
    }
    state->history_head = (uint8_t)((state->history_head + 1u) % SCOPE_HISTORY);
    if (state->history_count < SCOPE_HISTORY) state->history_count++;
  }
  memset(state->rate, 0, sizeof(state->rate));
}

static void clear_counters(scope_state_t *state) {
  memset(state->total, 0, sizeof(state->total));
  memset(state->rate, 0, sizeof(state->rate));
  memset(state->history, 0, sizeof(state->history));
  state->history_head = 0;
  state->history_count = 0;
  state->mouse_packets = 0;
  state->uart_tx = 0;
  state->uart_rx = 0;
  state->uart_errors = 0;
  state->rtc_polls = 0;
  state->rtc_irq_events = 0;
  log_event(state, "COUNTERS CLEARED");
}

static bool mouse_send_command(uint8_t command) {
  return lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) == LCOM_OK &&
         lcom_port_write8(KBC_IN_BUF, command) == LCOM_OK;
}

static int read_kbc_byte(bool want_mouse, uint8_t *value) {
  uint8_t status = 0;
  if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK) return LCOM_ERR;
  if ((status & KBC_ST_OBF) == 0) return LCOM_NO_DATA;
  if (((status & KBC_ST_AUX) != 0) != want_mouse) return LCOM_NO_DATA;
  return lcom_port_read8(KBC_OUT_BUF, value);
}

static int signed_mouse_delta(uint8_t flags, uint8_t value, uint8_t sign_bit) {
  if ((flags & BIT(sign_bit)) == 0) return value;
  return (int)(int16_t)(uint16_t)(0xFF00u | value);
}

static bool consume_mouse_byte(scope_state_t *state, scope_mouse_t *mouse,
                               uint8_t value) {
  if (!mouse->acknowledged && value == MOUSE_ACK) {
    mouse->acknowledged = true;
    mouse->byte_count = 0;
    log_event(state, "MOUSE STREAM ACK");
    return false;
  }
  if (value == MOUSE_NACK || value == MOUSE_ERR) {
    mouse->byte_count = 0;
    log_event_hex(state, "MOUSE ERROR", value);
    return false;
  }
  if (mouse->byte_count == 0 && (value & MOUSE_SYNC_BIT) == 0) return false;
  mouse->bytes[mouse->byte_count++] = value;
  if (mouse->byte_count < 3u) return false;

  int dx = signed_mouse_delta(mouse->bytes[0], mouse->bytes[1], 4u);
  int dy = signed_mouse_delta(mouse->bytes[0], mouse->bytes[2], 5u);
  mouse->x += dx;
  mouse->y -= dy;
  if (mouse->x < 0) mouse->x = 0;
  if (mouse->x > 799) mouse->x = 799;
  if (mouse->y < 0) mouse->y = 0;
  if (mouse->y > 599) mouse->y = 599;
  mouse->previous_buttons = mouse->buttons;
  mouse->buttons = (uint8_t)(mouse->bytes[0] & 0x07u);
  memcpy(state->mouse_last, mouse->bytes, sizeof(state->mouse_last));
  state->mouse_packets++;
  mouse->byte_count = 0;

  char text[SCOPE_EVENT_TEXT];
  snprintf(text, sizeof(text), "T%05llu  MOUSE PACKET %+d %+d B%u",
           (unsigned long long)state->ticks, dx, dy, mouse->buttons);
  log_event(state, text);
  return true;
}

static bool inside(int x, int y, int left, int top, int width, int height) {
  return x >= left && x < left + width && y >= top && y < top + height;
}

static void set_uart_loopback(scope_state_t *state, bool enabled) {
  if (!state->uart_available) {
    log_event(state, "UART LOOPBACK UNAVAILABLE");
    return;
  }
  uint8_t mcr = enabled ? (uint8_t)(MCR_LOOP | MCR_OUT2) : MCR_OUT2;
  if (lcom_port_write8(COM1_BASE + SER_MCR, mcr) != LCOM_OK) {
    state->uart_errors++;
    log_event(state, "UART MCR WRITE FAILED");
    return;
  }
  state->loopback_enabled = enabled;
  log_event(state, enabled ? "UART LOOPBACK ON" : "UART LOOPBACK OFF");
}

static void handle_mouse_action(scope_state_t *state, scope_mouse_t *mouse) {
  bool left_pressed = (mouse->buttons & BIT(0)) != 0 &&
                      (mouse->previous_buttons & BIT(0)) == 0;
  if (!left_pressed) return;
  if (inside(mouse->x, mouse->y, 572, 20, 90, 34)) {
    state->frozen = !state->frozen;
    log_event(state, state->frozen ? "RATE DISPLAY FROZEN" : "RATE DISPLAY LIVE");
  } else if (inside(mouse->x, mouse->y, 672, 20, 98, 34)) {
    set_uart_loopback(state, !state->loopback_enabled);
  }
}

static void handle_keyboard(scope_state_t *state, uint8_t data, bool *running) {
  push_byte(state->scancodes, &state->scancode_head, &state->scancode_count, data);
  const char *label = data == KBD_TWO_BYTE_PFX
                          ? "KBD PREFIX"
                          : ((data & KBD_BREAK_BIT) != 0 ? "KBD BREAK" : "KBD MAKE");
  log_event_hex(state, label, data);

  if (data == KBD_TWO_BYTE_PFX) {
    state->extended_key = true;
    return;
  }
  bool break_code = (data & KBD_BREAK_BIT) != 0;
  uint8_t code = (uint8_t)(data & 0x7Fu);
  if (state->extended_key) {
    state->extended_key = false;
    return;
  }
  if (data == ESC_BREAK) {
    *running = false;
    return;
  }
  if (break_code) return;
  if (code == 0x39u) {
    state->frozen = !state->frozen;
    log_event(state, state->frozen ? "RATE DISPLAY FROZEN" : "RATE DISPLAY LIVE");
  } else if (code == 0x2Eu) {
    clear_counters(state);
  } else if (code == 0x26u) {
    set_uart_loopback(state, !state->loopback_enabled);
  }
}

static bool uart_configure_loopback(void) {
  return lcom_port_write8(COM1_BASE + SER_IER, 0) == LCOM_OK &&
         lcom_port_write8(COM1_BASE + SER_LCR, 0x03u) == LCOM_OK &&
         lcom_port_write8(COM1_BASE + SER_FCR,
                          FCR_ENABLE_FIFO | FCR_CLEAR_RX | FCR_CLEAR_TX) == LCOM_OK &&
         lcom_port_write8(COM1_BASE + SER_MCR, MCR_LOOP | MCR_OUT2) == LCOM_OK &&
         lcom_port_write8(COM1_BASE + SER_IER, IER_RDA | IER_RLS) == LCOM_OK;
}

static void uart_send_probe(scope_state_t *state) {
  static const char probe[] = "MACHINE-LAB IRQ ";
  uint8_t lsr = 0;
  if (!state->loopback_enabled) return;
  if (lcom_port_read8(COM1_BASE + SER_LSR, &lsr) != LCOM_OK ||
      (lsr & LSR_THRE) == 0) {
    return;
  }
  uint8_t value = (uint8_t)probe[state->uart_tx % (sizeof(probe) - 1u)];
  if (lcom_port_write8(COM1_BASE + SER_THR, value) == LCOM_OK) {
    state->uart_tx++;
  }
}

static void uart_drain(scope_state_t *state) {
  uint8_t iir = 0;
  if (lcom_port_read8(COM1_BASE + SER_IIR, &iir) != LCOM_OK) {
    state->uart_errors++;
    return;
  }
  for (uint8_t limit = 0; limit < 32u; limit++) {
    uint8_t lsr = 0;
    if (lcom_port_read8(COM1_BASE + SER_LSR, &lsr) != LCOM_OK) {
      state->uart_errors++;
      return;
    }
    if ((lsr & (LSR_OE | LSR_PE | LSR_FE | LSR_BI | LSR_FIFO_ERR)) != 0) {
      state->uart_errors++;
      log_event_hex(state, "UART LSR ERR", lsr);
    }
    if ((lsr & LSR_RX_RDY) == 0) break;
    uint8_t value = 0;
    if (lcom_port_read8(COM1_BASE + SER_RBR, &value) != LCOM_OK) {
      state->uart_errors++;
      break;
    }
    state->uart_rx++;
    push_byte(state->uart_last, &state->uart_head, &state->uart_count, value);
    log_event_hex(state, "UART RX", value);
  }
}

static void poll_rtc(scope_state_t *state) {
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
  state->rtc_polls++;
  if (state->ticks != 0u) record_source(state, SOURCE_RTC);
  if (read_rtc_time(&hour, &minute, &second) != LCOM_OK) {
    state->rtc_valid = false;
    log_event(state, "RTC POLL ERROR");
    return;
  }
  bool changed = state->rtc_valid &&
                 (hour != state->rtc_hour || minute != state->rtc_minute ||
                  second != state->rtc_second);
  state->rtc_hour = hour;
  state->rtc_minute = minute;
  state->rtc_second = second;
  state->rtc_valid = true;
  if (changed) {
    char text[SCOPE_EVENT_TEXT];
    snprintf(text, sizeof(text), "T%05llu  RTC POLL     %02u:%02u:%02u",
             (unsigned long long)state->ticks, hour, minute, second);
    log_event(state, text);
  }
}

static void draw_cursor(example_canvas_t *canvas, const scope_mouse_t *mouse) {
  uint32_t color = mouse->acknowledged ? example_rgb(240, 243, 248)
                                      : example_rgb(255, 184, 102);
  for (int row = 0; row < 16; row++) {
    int width = row < 10 ? row / 2 + 2 : 4;
    example_fill_rect(canvas, mouse->x, mouse->y + row, width, 1, color);
  }
}

static void draw_header_button(example_canvas_t *canvas, int x, int width,
                               const char *label, bool active, bool hover,
                               uint32_t active_color) {
  uint32_t fill = active ? active_color
                         : (hover ? example_rgb(57, 67, 87)
                                  : example_rgb(37, 44, 59));
  example_fill_rect(canvas, x, 20, width, 34, fill);
  example_stroke_rect(canvas, x, 20, width, 34, 1,
                      hover ? example_rgb(143, 158, 187)
                            : example_rgb(68, 78, 98));
  uint32_t text_width = example_text_width(label, 1);
  example_draw_text(canvas, x + (width - (int)text_width) / 2, 33, label,
                    example_rgb(238, 241, 246), 1);
}

static void draw_source_card(example_canvas_t *canvas, const scope_state_t *state,
                             source_kind_t source, int x) {
  const int width = 142;
  uint32_t color = source_colors[source];
  example_fill_rect(canvas, x, 88, width, 94, example_rgb(28, 35, 49));
  example_stroke_rect(canvas, x, 88, width, 94, 1, example_rgb(57, 68, 88));
  example_fill_rect(canvas, x + 12, 100, 10, 10, color);
  example_draw_text(canvas, x + 30, 101, source_names[source],
                    example_rgb(169, 181, 203), 1);
  char rate[24];
  char total[28];
  uint32_t completed_rate = state->rate[source];
  if (state->history_count != 0u) {
    uint8_t latest = (uint8_t)((state->history_head + SCOPE_HISTORY - 1u) %
                               SCOPE_HISTORY);
    completed_rate = state->history[source][latest];
  }
  snprintf(rate, sizeof(rate), "%u / SEC", completed_rate);
  snprintf(total, sizeof(total), "TOTAL %llu",
           (unsigned long long)state->total[source]);
  example_draw_text(canvas, x + 12, 127, rate, example_rgb(240, 243, 247), 2);
  example_draw_text(canvas, x + 12, 158, total, color, 1);
}

static void draw_rate_graph(example_canvas_t *canvas, const scope_state_t *state) {
  const int graph_x = 28;
  const int graph_y = 214;
  const int graph_w = 458;
  const int graph_h = 136;
  example_fill_rect(canvas, graph_x, graph_y, graph_w, graph_h,
                    example_rgb(20, 27, 39));
  example_stroke_rect(canvas, graph_x, graph_y, graph_w, graph_h, 1,
                      example_rgb(55, 67, 87));
  for (int row = 1; row < 4; row++) {
    int y = graph_y + row * graph_h / 4;
    example_fill_rect(canvas, graph_x + 1, y, graph_w - 2, 1,
                      example_rgb(35, 45, 61));
  }
  example_draw_text(canvas, graph_x + 14, graph_y + 12,
                    state->frozen ? "IRQ RATE HISTORY - DISPLAY FROZEN"
                                  : "IRQ RATE HISTORY - 1 SECOND WINDOWS",
                    state->frozen ? example_rgb(255, 184, 102)
                                  : example_rgb(171, 184, 205),
                    1);

  uint8_t count = state->history_count;
  uint8_t oldest = count < SCOPE_HISTORY
                       ? 0
                       : state->history_head;
  for (uint8_t sample = 0; sample < count; sample++) {
    uint8_t index = (uint8_t)((oldest + sample) % SCOPE_HISTORY);
    int x = graph_x + 16 + (SCOPE_HISTORY - count + sample) * 7;
    for (uint8_t source = 0; source < SOURCE_COUNT; source++) {
      uint32_t value = state->history[source][index];
      uint32_t scale = source == SOURCE_TIMER ? 60u : 12u;
      int height = (int)(value * 78u / scale);
      if (height > 78) height = 78;
      if (height < 1 && value != 0) height = 1;
      example_fill_rect(canvas, x + source, graph_y + 119 - height, 1, height,
                        source_colors[source]);
    }
  }
  example_draw_text(canvas, graph_x + 14, graph_y + 119, "PAST",
                    example_rgb(105, 119, 143), 1);
  example_draw_text(canvas, graph_x + graph_w - 52, graph_y + 119, "NOW",
                    example_rgb(105, 119, 143), 1);
}

static void format_byte_ring(char *out, size_t out_size, const uint8_t *values,
                             uint8_t head, uint8_t count) {
  size_t used = 0;
  out[0] = 0;
  uint8_t oldest = count < SCOPE_BYTE_HISTORY ? 0 : head;
  uint8_t shown = count > 12u ? 12u : count;
  uint8_t skipped = (uint8_t)(count - shown);
  for (uint8_t i = 0; i < shown; i++) {
    uint8_t index = (uint8_t)((oldest + skipped + i) % SCOPE_BYTE_HISTORY);
    int written = snprintf(out + used, out_size - used, "%s%02X",
                           i == 0 ? "" : " ", values[index]);
    if (written < 0 || (size_t)written >= out_size - used) break;
    used += (size_t)written;
  }
}

static void draw_raw_panel(example_canvas_t *canvas, const scope_state_t *state) {
  const int x = 506;
  const int y = 214;
  const int width = 266;
  const int height = 330;
  example_fill_rect(canvas, x, y, width, height, example_rgb(24, 31, 44));
  example_stroke_rect(canvas, x, y, width, height, 1, example_rgb(55, 67, 87));
  example_draw_text(canvas, x + 16, y + 14, "RAW DEVICE BYTES",
                    example_rgb(204, 211, 224), 2);

  char bytes[96];
  example_draw_text(canvas, x + 16, y + 50, "KEYBOARD SCANCODES",
                    source_colors[SOURCE_KEYBOARD], 1);
  format_byte_ring(bytes, sizeof(bytes), state->scancodes,
                   state->scancode_head, state->scancode_count);
  example_draw_text(canvas, x + 16, y + 70,
                    bytes[0] == 0 ? "--" : bytes,
                    example_rgb(220, 224, 232), 1);

  example_draw_text(canvas, x + 16, y + 108, "PS/2 PACKET",
                    source_colors[SOURCE_MOUSE], 1);
  snprintf(bytes, sizeof(bytes), "%02X %02X %02X   PACKETS %u",
           state->mouse_last[0], state->mouse_last[1], state->mouse_last[2],
           state->mouse_packets);
  example_draw_text(canvas, x + 16, y + 128, bytes,
                    example_rgb(220, 224, 232), 1);

  example_draw_text(canvas, x + 16, y + 166, "UART RX BYTES",
                    source_colors[SOURCE_UART], 1);
  format_byte_ring(bytes, sizeof(bytes), state->uart_last,
                   state->uart_head, state->uart_count);
  example_draw_text(canvas, x + 16, y + 186,
                    bytes[0] == 0 ? "--" : bytes,
                    example_rgb(220, 224, 232), 1);
  snprintf(bytes, sizeof(bytes), "TX %u  RX %u  ERR %u",
           state->uart_tx, state->uart_rx, state->uart_errors);
  example_draw_text(canvas, x + 16, y + 212, bytes,
                    example_rgb(145, 159, 183), 1);

  example_draw_text(canvas, x + 16, y + 250, "RTC STATUS",
                    source_colors[SOURCE_RTC], 1);
  if (state->rtc_valid) {
    snprintf(bytes, sizeof(bytes), "%02u:%02u:%02u  POLLS %u",
             state->rtc_hour, state->rtc_minute, state->rtc_second,
             state->rtc_polls);
  } else {
    snprintf(bytes, sizeof(bytes), "READ ERROR  POLLS %u", state->rtc_polls);
  }
  example_draw_text(canvas, x + 16, y + 270, bytes,
                    example_rgb(220, 224, 232), 1);
  example_draw_text(canvas, x + 16, y + 298, "IRQ8 SUBSCRIBED - POLL FALLBACK",
                    example_rgb(125, 139, 163), 1);
}

static void draw_event_log(example_canvas_t *canvas, const scope_state_t *state) {
  const int x = 28;
  const int y = 368;
  const int width = 458;
  const int height = 176;
  example_fill_rect(canvas, x, y, width, height, example_rgb(20, 27, 39));
  example_stroke_rect(canvas, x, y, width, height, 1, example_rgb(55, 67, 87));
  example_draw_text(canvas, x + 14, y + 12, "RECENT INTERRUPT EVENTS",
                    example_rgb(171, 184, 205), 1);
  uint8_t oldest = state->event_count < SCOPE_EVENT_ROWS
                       ? 0
                       : state->event_head;
  for (uint8_t row = 0; row < state->event_count; row++) {
    uint8_t index = (uint8_t)((oldest + row) % SCOPE_EVENT_ROWS);
    example_draw_text(canvas, x + 14, y + 34 + row * 13,
                      state->events[index],
                      row + 1u == state->event_count
                          ? example_rgb(226, 231, 239)
                          : example_rgb(128, 143, 167),
                      1);
  }
}

static void draw_scope(example_canvas_t *canvas, const scope_state_t *state,
                       const scope_mouse_t *mouse, bool uart_available,
                       bool rtc_irq_available) {
  example_fill_rect(canvas, 0, 0, 800, 600, example_rgb(14, 19, 28));
  for (int x = 0; x < 800; x += 20) {
    example_fill_rect(canvas, x, 0, 1, 600, example_rgb(18, 24, 34));
  }
  for (int y = 0; y < 600; y += 20) {
    example_fill_rect(canvas, 0, y, 800, 1, example_rgb(18, 24, 34));
  }
  example_fill_rect(canvas, 0, 0, 800, 72, example_rgb(22, 29, 41));
  example_fill_rect(canvas, 0, 70, 800, 2, example_rgb(111, 212, 255));
  example_draw_text(canvas, 26, 16, "INTERRUPT SCOPE",
                    example_rgb(238, 242, 248), 3);
  char subtitle[96];
  snprintf(subtitle, sizeof(subtitle), "LIVE DEVICE MONITOR  TICK %llu  UART %s  RTC IRQ8 %s",
           (unsigned long long)state->ticks,
           uart_available ? "ONLINE" : "OFFLINE",
           rtc_irq_available ? "SUBSCRIBED" : "UNAVAILABLE");
  example_draw_text(canvas, 28, 48, subtitle, example_rgb(133, 149, 175), 1);
  draw_header_button(canvas, 572, 90, state->frozen ? "UNFREEZE" : "FREEZE",
                     state->frozen,
                     inside(mouse->x, mouse->y, 572, 20, 90, 34),
                     example_rgb(159, 105, 61));
  draw_header_button(canvas, 672, 98, state->loopback_enabled ? "LOOP ON" : "LOOP OFF",
                     state->loopback_enabled,
                     inside(mouse->x, mouse->y, 672, 20, 98, 34),
                     example_rgb(45, 134, 105));

  for (uint8_t source = 0; source < SOURCE_COUNT; source++) {
    draw_source_card(canvas, state, (source_kind_t)source, 28 + source * 150);
  }
  draw_rate_graph(canvas, state);
  draw_event_log(canvas, state);
  draw_raw_panel(canvas, state);
  example_fill_rect(canvas, 0, 566, 800, 34, example_rgb(10, 14, 21));
  example_draw_text(canvas, 22, 578,
                    "SPACE FREEZE  C CLEAR  L UART LOOPBACK  MOUSE BUTTONS  ESC EXIT",
                    example_rgb(136, 151, 177), 1);
  draw_cursor(canvas, mouse);
}

int main(void) {
  int result = 1;
  bool initialized = false;
  bool framebuffer_mapped = false;
  bool timer_subscribed = false;
  bool keyboard_subscribed = false;
  bool mouse_subscribed = false;
  bool uart_subscribed = false;
  bool rtc_subscribed = false;
  bool mouse_command_sent = false;
  bool uart_configured = false;
  lcom_vbe_mode_info_t video_info = {0};
  uint8_t *framebuffer = 0;
  lcom_irq_t timer_irq = {0};
  lcom_irq_t keyboard_irq = {0};
  lcom_irq_t mouse_irq = {0};
  lcom_irq_t uart_irq = {0};
  lcom_irq_t rtc_irq = {0};
  scope_state_t state = {0};
  scope_mouse_t mouse = {.x = 400, .y = 300};

  if (lcom_init() != LCOM_OK) goto cleanup;
  initialized = true;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &video_info) != LCOM_OK ||
      lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK ||
      lcom_phys_map(video_info.framebuffer_phys, video_info.framebuffer_size,
                    (void **)&framebuffer) != LCOM_OK) {
    goto cleanup;
  }
  framebuffer_mapped = true;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) goto cleanup;
  timer_subscribed = true;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &keyboard_irq) != LCOM_OK) goto cleanup;
  keyboard_subscribed = true;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &mouse_irq) == LCOM_OK) {
    mouse_subscribed = true;
    mouse_command_sent = mouse_send_command(MOUSE_CMD_ENABLE_DR);
  }
  if (lcom_irq_subscribe(COM1_IRQ, 0, &uart_irq) == LCOM_OK) {
    uart_subscribed = true;
    uart_configured = uart_configure_loopback();
    state.uart_available = uart_configured;
    state.loopback_enabled = uart_configured;
  }
  if (lcom_irq_subscribe(RTC_IRQ_LINE, 0, &rtc_irq) == LCOM_OK) {
    rtc_subscribed = true;
  }

  poll_rtc(&state);
  log_event(&state, "MONITOR ONLINE");
  log_event(&state, "RTC USES POLL FALLBACK");
  example_canvas_t canvas = {framebuffer, video_info};
  bool running = true;
  bool event_failed = false;
  draw_scope(&canvas, &state, &mouse, uart_configured, rtc_subscribed);
  (void)lcom_vbe_present();

  while (running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      event_failed = true;
      break;
    }
    state.ticks = event.tick;

    if ((event.irq_mask & timer_irq.mask) != 0) {
      record_source(&state, SOURCE_TIMER);
      if (uart_configured && state.ticks % 20u == 0u) uart_send_probe(&state);
      if (state.ticks % SCOPE_TICKS_PER_SECOND == 0u) {
        poll_rtc(&state);
        capture_rate_window(&state);
      }
    }
    if ((event.irq_mask & keyboard_irq.mask) != 0) {
      record_source(&state, SOURCE_KEYBOARD);
      uint8_t data = 0;
      if (read_kbc_byte(false, &data) == LCOM_OK) {
        handle_keyboard(&state, data, &running);
      }
    }
    if (mouse_subscribed && (event.irq_mask & mouse_irq.mask) != 0) {
      record_source(&state, SOURCE_MOUSE);
      uint8_t data = 0;
      if (read_kbc_byte(true, &data) == LCOM_OK &&
          consume_mouse_byte(&state, &mouse, data)) {
        handle_mouse_action(&state, &mouse);
      }
    }
    if (uart_subscribed && (event.irq_mask & uart_irq.mask) != 0) {
      record_source(&state, SOURCE_UART);
      uart_drain(&state);
    }
    if (rtc_subscribed && (event.irq_mask & rtc_irq.mask) != 0) {
      state.rtc_irq_events++;
      uint8_t ignored = 0;
      (void)rtc_read_register(RTC_REG_C, &ignored);
      log_event_hex(&state, "RTC IRQ C", ignored);
    }

    draw_scope(&canvas, &state, &mouse, uart_configured, rtc_subscribed);
    (void)lcom_vbe_present();
  }

  draw_scope(&canvas, &state, &mouse, uart_configured, rtc_subscribed);
  (void)lcom_vbe_present();
  lcom_printf("interrupt scope summary ticks=%llu timer_irq=%llu keyboard_irq=%llu "
              "mouse_irq=%llu mouse_packets=%u uart_irq=%llu uart_tx=%u uart_rx=%u "
              "uart_errors=%u rtc_irq=%llu rtc_polls=%u frozen=%u loopback=%u\n",
              (unsigned long long)state.ticks,
              (unsigned long long)state.total[SOURCE_TIMER],
              (unsigned long long)state.total[SOURCE_KEYBOARD],
              (unsigned long long)state.total[SOURCE_MOUSE],
              state.mouse_packets,
              (unsigned long long)state.total[SOURCE_UART],
              state.uart_tx, state.uart_rx, state.uart_errors,
              (unsigned long long)state.rtc_irq_events,
              state.rtc_polls, state.frozen ? 1u : 0u,
              state.loopback_enabled ? 1u : 0u);
  result = event_failed ? 1 : 0;

cleanup:
  if (uart_configured) {
    (void)lcom_port_write8(COM1_BASE + SER_IER, 0);
    (void)lcom_port_write8(COM1_BASE + SER_MCR, 0);
  }
  if (mouse_command_sent) (void)mouse_send_command(MOUSE_CMD_DISABLE_DR);
  if (rtc_subscribed) (void)lcom_irq_unsubscribe(&rtc_irq);
  if (uart_subscribed) (void)lcom_irq_unsubscribe(&uart_irq);
  if (mouse_subscribed) (void)lcom_irq_unsubscribe(&mouse_irq);
  if (keyboard_subscribed) (void)lcom_irq_unsubscribe(&keyboard_irq);
  if (timer_subscribed) (void)lcom_irq_unsubscribe(&timer_irq);
  if (framebuffer_mapped) {
    (void)lcom_phys_unmap(framebuffer, video_info.framebuffer_size);
  }
  if (initialized) lcom_exit();
  return result;
}
