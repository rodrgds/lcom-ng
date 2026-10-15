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
  bool show_help;
  bool uart_available;
  bool loopback_enabled;
  example_modifiers_t modifiers;
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
  if (state->show_help) {
    if (inside(mouse->x, mouse->y, 588, 154, 100, 32)) state->show_help = false;
    return;
  }
  if (inside(mouse->x, mouse->y, 28, 202, 94, 28)) { clear_counters(state); return; }
  if (inside(mouse->x, mouse->y, 664, 202, 106, 28)) { state->show_help = true; return; }
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

  if (example_modifier_byte(&state->modifiers, data)) { state->extended_key = false; return; }
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
    if (state->show_help) { state->show_help = false; return; }
    *running = false;
    return;
  }
  if (break_code) return;
  if (example_primary(&state->modifiers)) { if (code == 0x11) *running = false; return; }
  if (code == 0x3Bu) { state->show_help = !state->show_help; return; }
  if (state->show_help) return;
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

static void draw_source_card(example_canvas_t *canvas, const scope_state_t *state,
                             source_kind_t source, int x) {
  uint32_t rate = state->rate[source];
  if (state->history_count) rate = state->history[source][(state->history_head + SCOPE_HISTORY - 1u) % SCOPE_HISTORY];
  example_round_rect(canvas, x, 92, 142, 94, 6, UI_PANEL);
  const char *names[] = {"Timer", "Keyboard", "Mouse", "UART", "RTC"};
  example_fill_rect(canvas, x + 12, 106, 8, 8, source_colors[source]);
  example_label(canvas, x + 28, 100, names[source], UI_DIM, 1);
  char value[32]; snprintf(value, sizeof(value), "%u / s", rate);
  example_label(canvas, x + 12, 124, value, UI_LIGHT, 2);
  snprintf(value, sizeof(value), "Total %llu", (unsigned long long)state->total[source]);
  example_label(canvas, x + 12, 162, value, UI_DIM, 1);
}


static void draw_rate_graph(example_canvas_t *canvas, const scope_state_t *state) {
  const char *names[] = {"Timer", "Keyboard", "Mouse", "UART", "RTC"};
  const int graph_x = 126, graph_y = 240, graph_w = 642;
  example_label(canvas, 144, 206, state->frozen ? "History paused. Device counters remain live." : "60 seconds of activity. Each lane uses its own scale.", state->frozen ? 0xFFB866 : UI_DIM, 1);
  uint8_t count = state->history_count;
  uint8_t oldest = count < SCOPE_HISTORY ? 0 : state->history_head;
  for (uint8_t source = 0; source < SOURCE_COUNT; source++) {
    int y = graph_y + source * 27;
    example_label(canvas, 28, y + 2, names[source], source_colors[source], 1);
    example_fill_rect(canvas, graph_x, y, graph_w, 24, 0x1A2533);
    for (int second = 0; second < 60; second += 10)
      example_fill_rect(canvas, graph_x + second * 10, y, 1, 24, 0x2B3A4B);
    uint32_t maximum = source == SOURCE_TIMER ? 60u : 1u;
    for (uint8_t sample = 0; sample < count; sample++) {
      uint32_t value = state->history[source][(oldest + sample) % SCOPE_HISTORY];
      if (value > maximum) maximum = value;
    }
    for (uint8_t sample = 0; sample < count; sample++) {
      uint32_t value = state->history[source][(oldest + sample) % SCOPE_HISTORY];
      int height = value ? 2 + (int)(value * 19u / maximum) : 1;
      example_fill_rect(canvas, graph_x + (SCOPE_HISTORY - count + sample) * 10 + 1, y + 23 - height, 8, height, value ? source_colors[source] : 0x455568);
    }
    char top[20]; snprintf(top, sizeof(top), "%u", maximum);
    example_label(canvas, 766 - example_label_width(top, 1), y + 3, top, UI_DIM, 1);
  }
  example_label(canvas, graph_x, 378, "60s ago", UI_DIM, 1);
  example_label(canvas, 738, 378, "now", UI_DIM, 1);
}

static void format_byte_ring(char *out, size_t out_size, const uint8_t *values,
                             uint8_t head, uint8_t count) {
  size_t used = 0;
  out[0] = 0;
  uint8_t oldest = count < SCOPE_BYTE_HISTORY ? 0 : head;
  uint8_t shown = count > 8u ? 8u : count;
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
  example_fill_rect(canvas, 486, 412, 286, 142, UI_PANEL);
  example_label(canvas, 500, 420, "Device bytes", UI_LIGHT, 1);
  char bytes[96], line[96];
  format_byte_ring(bytes, sizeof(bytes), state->scancodes, state->scancode_head, state->scancode_count);
  snprintf(line, sizeof(line), "KBD  %s", bytes[0] ? bytes : "Waiting for a key");
  example_draw_text(canvas, 500, 450, line, source_colors[SOURCE_KEYBOARD], 1);
  snprintf(line, sizeof(line), "PS/2 %02X %02X %02X   %u packets", state->mouse_last[0], state->mouse_last[1], state->mouse_last[2], state->mouse_packets);
  example_draw_text(canvas, 500, 472, line, source_colors[SOURCE_MOUSE], 1);
  format_byte_ring(bytes, sizeof(bytes), state->uart_last, state->uart_head, state->uart_count);
  snprintf(line, sizeof(line), "UART %s", state->uart_available ? (bytes[0] ? bytes : "Waiting for bytes") : "Unavailable");
  example_draw_text(canvas, 500, 494, line, source_colors[SOURCE_UART], 1);
  snprintf(line, sizeof(line), "TX %u  RX %u  errors %u", state->uart_tx, state->uart_rx, state->uart_errors);
  example_draw_text(canvas, 500, 516, line, UI_DIM, 1);
  if (state->rtc_valid) snprintf(line, sizeof(line), "RTC %02u:%02u:%02u  polls %u", state->rtc_hour, state->rtc_minute, state->rtc_second, state->rtc_polls);
  else snprintf(line, sizeof(line), "RTC read failed. Polling each second.");
  example_draw_text(canvas, 500, 538, line, source_colors[SOURCE_RTC], 1);
}

static void draw_event_log(example_canvas_t *canvas, const scope_state_t *state) {
  example_fill_rect(canvas, 28, 412, 442, 142, UI_PANEL);
  example_label(canvas, 42, 420, "Recent events", UI_LIGHT, 1);
  uint8_t oldest = state->event_count < SCOPE_EVENT_ROWS ? 0 : state->event_head;
  uint8_t shown = state->event_count < 5 ? state->event_count : 5;
  if (!shown) example_label(canvas, 42, 452, "Move the mouse or press a key to see activity.", UI_DIM, 1);
  for (uint8_t row = 0; row < shown; row++) {
    uint8_t index = (oldest + state->event_count - shown + row) % SCOPE_EVENT_ROWS;
    example_label(canvas, 42, 448 + row * 20, state->events[index], row + 1 == shown ? UI_LIGHT : UI_DIM, 1);
  }
}


static void draw_scope(example_canvas_t *canvas, const scope_state_t *state,
                       const scope_mouse_t *mouse, bool uart_available,
                       bool rtc_irq_available) {
  example_fill_rect(canvas, 0, 0, 800, 600, example_rgb(14, 19, 28));
  (void)rtc_irq_available;
  example_heading(canvas, "Interrupt Scope", uart_available ? "Live devices. RTC time is polled each second." : "UART unavailable. Other devices remain active.", 0x6FD4FF, true);
  example_dark_button(canvas, 572, 20, 90, 34, state->frozen ? "Resume" : "Freeze",
                     state->frozen, inside(mouse->x, mouse->y, 572, 20, 90, 34), true);
  example_dark_button(canvas, 672, 20, 98, 34, state->loopback_enabled ? "Loop on" : "Loop off",
                     state->loopback_enabled, inside(mouse->x, mouse->y, 672, 20, 98, 34), uart_available);

  for (uint8_t source = 0; source < SOURCE_COUNT; source++) {
    draw_source_card(canvas, state, (source_kind_t)source, 28 + source * 150);
  }
  /* Clear and help sit with the history rather than the live controls. */
  example_dark_button(canvas, 28, 202, 94, 28, "Clear  C", false, inside(mouse->x, mouse->y, 28, 202, 94, 28), true);
  example_dark_button(canvas, 664, 202, 106, 28, "Help  F1", false, inside(mouse->x, mouse->y, 664, 202, 106, 28), true);
  draw_rate_graph(canvas, state);
  draw_event_log(canvas, state);
  draw_raw_panel(canvas, state);
  example_fill_rect(canvas, 0, 566, 800, 34, example_rgb(10, 14, 21));
  example_label(canvas, 24, 574, "Space  Pause history     C  Clear     L  Loopback     F1  Help     Esc  Close", UI_DIM, 1);
  if (state->show_help) {
    example_round_rect(canvas, 98, 140, 604, 320, 12, UI_DARK);
    example_label(canvas, 122, 156, "Scope shortcuts", UI_LIGHT, 2);
    example_dark_button(canvas, 588, 154, 100, 32, "Close  F1", false, inside(mouse->x, mouse->y, 588, 154, 100, 32), true);
    const char *lines[] = {"Space   Freeze or resume the rate history", "C   Clear counters and rate history", "L   Toggle the COM1 internal loopback probe", "Move or type   Inspect PS/2 bytes and interrupts", "RTC is polled once a second. Counters stay live.", "F1 / Esc   Close help. Esc again leaves."};
    for (int i = 0; i < 6; i++) example_label(canvas, 124, 218 + i * 34, lines[i], UI_DIM, 1);
  }
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
