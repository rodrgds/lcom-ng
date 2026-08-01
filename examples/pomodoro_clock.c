#include "example_ui.h"

#include <lcom/ac97.h>
#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/rtc.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CLOCK_TICKS_PER_SECOND 60u
#define CLOCK_FOCUS_SECONDS (25u * 60u)
#define CLOCK_BREAK_SECONDS (5u * 60u)
#define CLOCK_DEMO_FOCUS_SECONDS 5u
#define CLOCK_DEMO_BREAK_SECONDS 3u

typedef enum {
  SESSION_FOCUS,
  SESSION_BREAK,
} session_kind_t;

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  bool valid;
  bool binary_mode;
} rtc_snapshot_t;

typedef struct {
  int x;
  int y;
  uint8_t bytes[3];
  uint8_t byte_count;
  uint8_t buttons;
  uint8_t previous_buttons;
  uint32_t packets;
  bool command_sent;
  bool acknowledged;
} clock_mouse_t;

typedef struct {
  int16_t *pcm;
  lcom_ac97_buffer_info_t info;
  uint32_t signals;
  bool available;
  bool muted;
} clock_audio_t;

typedef struct {
  session_kind_t kind;
  uint64_t active_ticks;
  uint64_t lifetime_ticks;
  uint32_t completions;
  uint32_t rtc_reads;
  uint32_t rtc_jumps;
  rtc_snapshot_t rtc;
  bool running;
  bool demo_lengths;
  bool extended_key;
  char status[40];
} clock_state_t;

static uint8_t bcd_to_binary(uint8_t value) {
  return (uint8_t)(((value >> 4) * 10u) + (value & 0x0Fu));
}

static int rtc_read_register(uint8_t reg, uint8_t *value) {
  if (lcom_port_write8(RTC_ADDR_REG, reg) != LCOM_OK) return LCOM_ERR;
  return lcom_port_read8(RTC_DATA_REG, value);
}

static int rtc_wait_for_update(void) {
  for (uint16_t attempt = 0; attempt < 1000u; attempt++) {
    uint8_t reg_a = 0;
    if (rtc_read_register(RTC_REG_A, &reg_a) != LCOM_OK) return LCOM_ERR;
    if ((reg_a & RTC_UIP) == 0) return LCOM_OK;
  }
  return LCOM_TIMEOUT;
}

static int rtc_read_once(rtc_snapshot_t *snapshot) {
  uint8_t reg_b = 0;
  uint8_t hour = 0;
  if (rtc_wait_for_update() != LCOM_OK) return LCOM_ERR;
  if (rtc_read_register(RTC_REG_B, &reg_b) != LCOM_OK ||
      rtc_read_register(RTC_REG_YEAR, &snapshot->year) != LCOM_OK ||
      rtc_read_register(RTC_REG_MONTH, &snapshot->month) != LCOM_OK ||
      rtc_read_register(RTC_REG_DAY, &snapshot->day) != LCOM_OK ||
      rtc_read_register(RTC_REG_HOURS, &hour) != LCOM_OK ||
      rtc_read_register(RTC_REG_MINUTES, &snapshot->minute) != LCOM_OK ||
      rtc_read_register(RTC_REG_SECONDS, &snapshot->second) != LCOM_OK) {
    return LCOM_ERR;
  }

  bool afternoon = (hour & BIT(7)) != 0;
  hour &= (uint8_t)~BIT(7);
  snapshot->binary_mode = (reg_b & RTC_DM) != 0;
  if (!snapshot->binary_mode) {
    snapshot->year = bcd_to_binary(snapshot->year);
    snapshot->month = bcd_to_binary(snapshot->month);
    snapshot->day = bcd_to_binary(snapshot->day);
    hour = bcd_to_binary(hour);
    snapshot->minute = bcd_to_binary(snapshot->minute);
    snapshot->second = bcd_to_binary(snapshot->second);
  }
  if ((reg_b & RTC_24H) == 0) {
    if (hour == 12u) hour = 0;
    if (afternoon) hour = (uint8_t)(hour + 12u);
  }
  snapshot->hour = hour;
  snapshot->valid = snapshot->month >= 1u && snapshot->month <= 12u &&
                    snapshot->day >= 1u && snapshot->day <= 31u &&
                    snapshot->hour <= 23u && snapshot->minute <= 59u &&
                    snapshot->second <= 59u;
  return snapshot->valid ? LCOM_OK : LCOM_ERR;
}

static bool rtc_same(const rtc_snapshot_t *a, const rtc_snapshot_t *b) {
  return a->year == b->year && a->month == b->month && a->day == b->day &&
         a->hour == b->hour && a->minute == b->minute && a->second == b->second;
}

static bool rtc_leap_year(uint16_t year) {
  return (year % 4u == 0u && year % 100u != 0u) || year % 400u == 0u;
}

static uint16_t rtc_days_before_month(uint16_t year, uint8_t month) {
  static const uint16_t days[] = {
      0u, 0u, 31u, 59u, 90u, 120u, 151u,
      181u, 212u, 243u, 273u, 304u, 334u,
  };
  uint16_t result = days[month];
  if (month > 2u && rtc_leap_year(year)) result++;
  return result;
}

static int64_t rtc_serial_seconds(const rtc_snapshot_t *snapshot) {
  uint16_t year = (uint16_t)(2000u + snapshot->year);
  int64_t days = 0;
  for (uint16_t current = 2000u; current < year; current++) {
    days += rtc_leap_year(current) ? 366 : 365;
  }
  days += rtc_days_before_month(year, snapshot->month);
  days += (int64_t)snapshot->day - 1;
  return (((days * 24 + snapshot->hour) * 60 + snapshot->minute) * 60 +
          snapshot->second);
}

static int rtc_read_stable(rtc_snapshot_t *snapshot) {
  rtc_snapshot_t first = {0};
  rtc_snapshot_t second = {0};
  for (uint8_t attempt = 0; attempt < 4u; attempt++) {
    if (rtc_read_once(&first) != LCOM_OK || rtc_read_once(&second) != LCOM_OK) {
      continue;
    }
    if (rtc_same(&first, &second)) {
      *snapshot = second;
      return LCOM_OK;
    }
  }
  return LCOM_ERR;
}

static bool rtc_changed(const rtc_snapshot_t *a, const rtc_snapshot_t *b) {
  return a->valid && b->valid && !rtc_same(a, b);
}

static bool rtc_discontinuous(const rtc_snapshot_t *previous,
                              const rtc_snapshot_t *current) {
  if (!rtc_changed(previous, current)) return false;
  return rtc_serial_seconds(current) - rtc_serial_seconds(previous) != 1;
}

static uint32_t session_seconds(const clock_state_t *state) {
  if (state->demo_lengths) {
    return state->kind == SESSION_FOCUS ? CLOCK_DEMO_FOCUS_SECONDS
                                        : CLOCK_DEMO_BREAK_SECONDS;
  }
  return state->kind == SESSION_FOCUS ? CLOCK_FOCUS_SECONDS : CLOCK_BREAK_SECONDS;
}

static uint64_t session_duration_ticks(const clock_state_t *state) {
  return (uint64_t)session_seconds(state) * CLOCK_TICKS_PER_SECOND;
}

static uint32_t remaining_seconds(const clock_state_t *state) {
  uint64_t duration = session_duration_ticks(state);
  if (state->active_ticks >= duration) return 0;
  uint64_t remaining_ticks = duration - state->active_ticks;
  return (uint32_t)((remaining_ticks + CLOCK_TICKS_PER_SECOND - 1u) /
                    CLOCK_TICKS_PER_SECOND);
}

static void set_status(clock_state_t *state, const char *status) {
  snprintf(state->status, sizeof(state->status), "%s", status);
}

static void reset_session(clock_state_t *state) {
  state->active_ticks = 0;
  state->running = false;
  set_status(state, "SESSION RESET");
}

static void toggle_running(clock_state_t *state) {
  state->running = !state->running;
  set_status(state, state->running ? "MONOTONIC TIMER RUNNING" : "MONOTONIC TIMER PAUSED");
}

static void change_session(clock_state_t *state) {
  state->kind = state->kind == SESSION_FOCUS ? SESSION_BREAK : SESSION_FOCUS;
  reset_session(state);
  set_status(state, state->kind == SESSION_FOCUS ? "FOCUS SELECTED" : "BREAK SELECTED");
}

static int audio_start(clock_audio_t *audio, size_t byte_count) {
  if (!audio->available || byte_count == 0 || byte_count > 0xFFFFu) return LCOM_ERR;
  if (lcom_port_write16(AC97_NAM_BASE + AC97_PCM_FRONT_DAC_RATE,
                        (uint16_t)AC97_DEFAULT_SAMPLE_RATE) != LCOM_OK ||
      lcom_port_write16(AC97_BM_BASE + AC97_PO_PICB,
                        (uint16_t)byte_count) != LCOM_OK) {
    return LCOM_ERR;
  }
  return lcom_port_write8(AC97_BM_BASE + AC97_PO_CR, AC97_PO_CR_RUN);
}

static void audio_stop(void) {
  (void)lcom_port_write8(AC97_BM_BASE + AC97_PO_CR, 0);
}

static void play_session_signal(clock_audio_t *audio, session_kind_t completed) {
  if (!audio->available || audio->muted) return;
  const uint32_t frames = 14000u;
  const uint8_t channels = AC97_DEFAULT_CHANNELS;
  const size_t samples = (size_t)frames * channels;
  const size_t bytes = samples * sizeof(int16_t);
  if (bytes > audio->info.pcm_bytes || bytes > 0xFFFFu) return;

  for (uint32_t frame = 0; frame < frames; frame++) {
    uint16_t hz;
    if (completed == SESSION_FOCUS) {
      hz = frame < frames / 2u ? 660u : 880u;
    } else {
      hz = frame < frames / 2u ? 523u : 659u;
    }
    uint32_t period = AC97_DEFAULT_SAMPLE_RATE / hz;
    if (period < 2u) period = 2u;
    int32_t envelope = (int32_t)(frames - frame);
    int32_t amplitude = 6200 * envelope / (int32_t)frames;
    int16_t sample = (frame % period) < period / 2u
                         ? (int16_t)amplitude
                         : (int16_t)-amplitude;
    audio->pcm[frame * channels] = sample;
    audio->pcm[frame * channels + 1u] = sample;
  }
  audio_stop();
  if (audio_start(audio, bytes) == LCOM_OK) audio->signals++;
}

static bool mouse_send_command(uint8_t command) {
  return lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) == LCOM_OK &&
         lcom_port_write8(KBC_IN_BUF, command) == LCOM_OK;
}

static int signed_mouse_delta(uint8_t flags, uint8_t value, uint8_t sign_bit) {
  if ((flags & BIT(sign_bit)) == 0) return value;
  return (int)(int16_t)(uint16_t)(0xFF00u | value);
}

static bool mouse_consume_byte(clock_mouse_t *mouse, uint8_t value) {
  if (!mouse->acknowledged && value == MOUSE_ACK) {
    mouse->acknowledged = true;
    mouse->byte_count = 0;
    return false;
  }
  if (value == MOUSE_NACK || value == MOUSE_ERR) {
    mouse->byte_count = 0;
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
  mouse->packets++;
  mouse->byte_count = 0;
  return true;
}

static bool inside(int x, int y, int left, int top, int width, int height) {
  return x >= left && x < left + width && y >= top && y < top + height;
}

static void handle_mouse_action(clock_state_t *state, clock_mouse_t *mouse) {
  bool left_pressed = (mouse->buttons & BIT(0)) != 0 &&
                      (mouse->previous_buttons & BIT(0)) == 0;
  if (!left_pressed) return;
  if (inside(mouse->x, mouse->y, 220, 476, 170, 54)) {
    toggle_running(state);
  } else if (inside(mouse->x, mouse->y, 405, 476, 130, 54)) {
    reset_session(state);
  } else if (inside(mouse->x, mouse->y, 550, 476, 130, 54)) {
    change_session(state);
  }
}

static void handle_keyboard(clock_state_t *state, clock_audio_t *audio,
                            uint8_t data, bool *running) {
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
    toggle_running(state);
  } else if (code == 0x13u) {
    reset_session(state);
  } else if (code == 0x0Fu) {
    change_session(state);
  } else if (code == 0x20u) {
    state->demo_lengths = !state->demo_lengths;
    reset_session(state);
    set_status(state, state->demo_lengths ? "DEMO LENGTHS 5S / 3S" : "CLASSIC 25M / 5M");
  } else if (code == 0x32u) {
    audio->muted = !audio->muted;
    set_status(state, audio->muted ? "SIGNAL MUTED" : "SIGNAL ENABLED");
  }
}

static int read_kbc_byte(bool want_mouse, uint8_t *value) {
  uint8_t status = 0;
  if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK) return LCOM_ERR;
  if ((status & KBC_ST_OBF) == 0) return LCOM_NO_DATA;
  bool is_mouse = (status & KBC_ST_AUX) != 0;
  if (is_mouse != want_mouse) return LCOM_NO_DATA;
  return lcom_port_read8(KBC_OUT_BUF, value);
}

static void fill_circle(example_canvas_t *canvas, int cx, int cy, int radius,
                        uint32_t color) {
  for (int y = -radius; y <= radius; y++) {
    int span = 0;
    while (span < radius && span * span + y * y <= radius * radius) span++;
    example_fill_rect(canvas, cx - span, cy + y, span * 2 + 1, 1, color);
  }
}

static void draw_button(example_canvas_t *canvas, int x, int y, int width,
                        const char *label, bool active, bool hover) {
  uint32_t base = active ? example_rgb(45, 137, 111)
                         : (hover ? example_rgb(61, 70, 91)
                                  : example_rgb(43, 50, 67));
  uint32_t border = active ? example_rgb(105, 226, 181)
                           : (hover ? example_rgb(139, 158, 196)
                                    : example_rgb(74, 84, 106));
  example_fill_rect(canvas, x, y, width, 54, base);
  example_stroke_rect(canvas, x, y, width, 54, 2, border);
  uint32_t text_width = example_text_width(label, 2);
  example_draw_text(canvas, x + (width - (int)text_width) / 2, y + 20, label,
                    example_rgb(241, 244, 248), 2);
}

static void draw_progress(example_canvas_t *canvas, const clock_state_t *state) {
  const int x = 151;
  const int y = 412;
  const int width = 498;
  const int height = 18;
  uint64_t duration = session_duration_ticks(state);
  uint64_t elapsed = state->active_ticks > duration ? duration : state->active_ticks;
  int fill = duration == 0 ? 0 : (int)((uint64_t)(width - 4) * elapsed / duration);
  uint32_t accent = state->kind == SESSION_FOCUS ? example_rgb(255, 120, 111)
                                                 : example_rgb(102, 215, 174);
  example_fill_rect(canvas, x, y, width, height, example_rgb(34, 41, 56));
  example_stroke_rect(canvas, x, y, width, height, 1, example_rgb(74, 85, 109));
  example_fill_rect(canvas, x + 2, y + 2, fill, height - 4, accent);
  for (uint8_t marker = 1; marker < 4u; marker++) {
    int marker_x = x + (width * marker) / 4;
    example_fill_rect(canvas, marker_x, y + 3, 1, height - 6,
                      example_rgb(113, 123, 143));
  }
}

static void draw_cursor(example_canvas_t *canvas, const clock_mouse_t *mouse) {
  int x = mouse->x;
  int y = mouse->y;
  uint32_t shadow = example_rgb(9, 12, 18);
  uint32_t tip = mouse->acknowledged ? example_rgb(244, 246, 250)
                                    : example_rgb(241, 176, 94);
  for (int row = 0; row < 17; row++) {
    int width = row < 11 ? row / 2 + 2 : 4;
    example_fill_rect(canvas, x + 2, y + row + 2, width, 1, shadow);
    example_fill_rect(canvas, x, y + row, width, 1, tip);
  }
}

static void draw_clock(example_canvas_t *canvas, const clock_state_t *state,
                       const clock_mouse_t *mouse, const clock_audio_t *audio) {
  uint32_t background = example_rgb(17, 21, 31);
  uint32_t panel = example_rgb(28, 34, 48);
  uint32_t panel_edge = example_rgb(52, 62, 82);
  uint32_t accent = state->kind == SESSION_FOCUS ? example_rgb(255, 120, 111)
                                                 : example_rgb(102, 215, 174);
  example_fill_rect(canvas, 0, 0, 800, 600, background);
  for (int y = 0; y < 600; y += 24) {
    example_fill_rect(canvas, 0, y, 800, 1, example_rgb(20, 25, 36));
  }

  example_fill_rect(canvas, 0, 0, 800, 76, example_rgb(24, 29, 42));
  example_fill_rect(canvas, 0, 74, 800, 2, accent);
  example_draw_text(canvas, 28, 20, "MACHINE LAB", example_rgb(239, 242, 247), 3);
  example_draw_text(canvas, 30, 50, "RTC WALL TIME + PIT SESSION TIME",
                    example_rgb(143, 154, 177), 1);
  example_fill_rect(canvas, 596, 18, 174, 38, panel);
  example_stroke_rect(canvas, 596, 18, 174, 38, 1, panel_edge);
  example_draw_text(canvas, 616, 32,
                    state->demo_lengths ? "DEMO 5S / 3S" : "CLASSIC 25M / 5M",
                    accent, 1);

  example_fill_rect(canvas, 30, 100, 740, 88, panel);
  example_stroke_rect(canvas, 30, 100, 740, 88, 1, panel_edge);
  fill_circle(canvas, 58, 144, 20, example_rgb(54, 91, 151));
  example_stroke_rect(canvas, 49, 138, 18, 12, 1, example_rgb(151, 193, 255));
  example_draw_text(canvas, 52, 141, "RTC", example_rgb(226, 238, 255), 1);
  example_draw_text(canvas, 92, 116, "WALL CLOCK", example_rgb(139, 155, 185), 1);
  example_draw_text(canvas, 92, 143, "RTC CMOS SNAPSHOT", example_rgb(231, 235, 242), 2);
  char wall_time[32];
  char wall_date[32];
  if (state->rtc.valid) {
    snprintf(wall_time, sizeof(wall_time), "%02u:%02u:%02u",
             state->rtc.hour, state->rtc.minute, state->rtc.second);
    snprintf(wall_date, sizeof(wall_date), "20%02u-%02u-%02u",
             state->rtc.year, state->rtc.month, state->rtc.day);
  } else {
    snprintf(wall_time, sizeof(wall_time), "--:--:--");
    snprintf(wall_date, sizeof(wall_date), "RTC READ ERROR");
  }
  example_draw_text(canvas, 456, 116, wall_time, example_rgb(241, 244, 249), 4);
  example_draw_text(canvas, 630, 151, wall_date, example_rgb(141, 153, 176), 1);

  example_fill_rect(canvas, 90, 212, 620, 238, panel);
  example_stroke_rect(canvas, 90, 212, 620, 238, 2, panel_edge);
  fill_circle(canvas, 125, 247, 10, accent);
  example_draw_text(canvas, 150, 235,
                    state->kind == SESSION_FOCUS ? "FOCUS SESSION" : "RECOVERY BREAK",
                    example_rgb(241, 243, 248), 3);
  example_draw_text(canvas, 151, 270, "MONOTONIC PIT IRQ0 - RTC JUMPS CANNOT CHANGE THIS",
                    example_rgb(137, 151, 177), 1);

  uint32_t seconds = remaining_seconds(state);
  char timer_text[16];
  snprintf(timer_text, sizeof(timer_text), "%02u:%02u", seconds / 60u, seconds % 60u);
  uint32_t timer_width = example_text_width(timer_text, 9);
  example_draw_text(canvas, 400 - (int)timer_width / 2, 310, timer_text,
                    example_rgb(247, 248, 250), 9);
  example_draw_text(canvas, 151, 382,
                    state->running ? "RUNNING ON TIMER INTERRUPTS"
                                   : "PAUSED - WALL CLOCK REMAINS INDEPENDENT",
                    state->running ? accent : example_rgb(173, 184, 204), 1);
  draw_progress(canvas, state);

  bool start_hover = inside(mouse->x, mouse->y, 220, 476, 170, 54);
  bool reset_hover = inside(mouse->x, mouse->y, 405, 476, 130, 54);
  bool mode_hover = inside(mouse->x, mouse->y, 550, 476, 130, 54);
  draw_button(canvas, 220, 476, 170, state->running ? "PAUSE" : "START",
              state->running, start_hover);
  draw_button(canvas, 405, 476, 130, "RESET", false, reset_hover);
  draw_button(canvas, 550, 476, 130, "SWITCH", false, mode_hover);

  example_fill_rect(canvas, 0, 556, 800, 44, example_rgb(12, 16, 24));
  example_draw_text(canvas, 24, 570, "SPACE START  R RESET  TAB MODE  D DEMO  M SOUND  ESC EXIT",
                    example_rgb(142, 155, 180), 1);
  char telemetry[96];
  snprintf(telemetry, sizeof(telemetry), "%s  DONE %u  RTC JUMPS %u  SOUND %s",
           state->status, state->completions, state->rtc_jumps,
           audio->available ? (audio->muted ? "MUTED" : "ON") : "N/A");
  example_draw_text(canvas, 24, 541, telemetry, accent, 1);
  draw_cursor(canvas, mouse);
}

static void update_session(clock_state_t *state, clock_audio_t *audio) {
  state->lifetime_ticks++;
  if (state->running) {
    state->active_ticks++;
    if (state->active_ticks >= session_duration_ticks(state)) {
      session_kind_t completed = state->kind;
      state->completions++;
      state->kind = state->kind == SESSION_FOCUS ? SESSION_BREAK : SESSION_FOCUS;
      state->active_ticks = 0;
      state->running = false;
      set_status(state, completed == SESSION_FOCUS
                            ? "FOCUS COMPLETE - TAKE A BREAK"
                            : "BREAK COMPLETE - READY TO FOCUS");
      play_session_signal(audio, completed);
    }
  }

  if (state->lifetime_ticks == 1u ||
      state->lifetime_ticks % (CLOCK_TICKS_PER_SECOND / 4u) == 0u) {
    rtc_snapshot_t fresh = {0};
    state->rtc_reads++;
    if (rtc_read_stable(&fresh) == LCOM_OK) {
      if (rtc_discontinuous(&state->rtc, &fresh)) state->rtc_jumps++;
      state->rtc = fresh;
    }
  }
}

int main(void) {
  int result = 1;
  bool initialized = false;
  bool framebuffer_mapped = false;
  bool timer_subscribed = false;
  bool keyboard_subscribed = false;
  bool mouse_subscribed = false;
  lcom_vbe_mode_info_t video_info = {0};
  uint8_t *framebuffer = 0;
  lcom_irq_t timer_irq = {0};
  lcom_irq_t keyboard_irq = {0};
  lcom_irq_t mouse_irq = {0};
  clock_audio_t audio = {0};
  clock_mouse_t mouse = {.x = 400, .y = 300};
  clock_state_t state = {.kind = SESSION_FOCUS};

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
    mouse.command_sent = mouse_send_command(MOUSE_CMD_ENABLE_DR);
  }

  if (lcom_ac97_get_buffer(&audio.info) == LCOM_OK &&
      lcom_phys_map(audio.info.pcm_phys, audio.info.pcm_bytes,
                    (void **)&audio.pcm) == LCOM_OK) {
    audio.available = true;
  }

  state.rtc.valid = false;
  if (rtc_read_stable(&state.rtc) == LCOM_OK) state.rtc_reads++;
  set_status(&state, "READY - PRESS SPACE");
  example_canvas_t canvas = {framebuffer, video_info};
  bool running = true;
  bool event_failed = false;
  draw_clock(&canvas, &state, &mouse, &audio);
  (void)lcom_vbe_present();

  while (running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      event_failed = true;
      break;
    }

    if (keyboard_subscribed && (event.irq_mask & keyboard_irq.mask) != 0) {
      uint8_t data = 0;
      if (read_kbc_byte(false, &data) == LCOM_OK) {
        handle_keyboard(&state, &audio, data, &running);
      }
    }
    if (mouse_subscribed && (event.irq_mask & mouse_irq.mask) != 0) {
      uint8_t data = 0;
      if (read_kbc_byte(true, &data) == LCOM_OK &&
          mouse_consume_byte(&mouse, data)) {
        handle_mouse_action(&state, &mouse);
      }
    }
    if ((event.irq_mask & timer_irq.mask) != 0) {
      update_session(&state, &audio);
    }
    draw_clock(&canvas, &state, &mouse, &audio);
    (void)lcom_vbe_present();
  }

  draw_clock(&canvas, &state, &mouse, &audio);
  (void)lcom_vbe_present();
  lcom_printf("pomodoro clock summary mode=%s running=%u active_ticks=%llu "
              "lifetime_ticks=%llu completions=%u rtc=%02u-%02u-%02uT%02u:%02u:%02u "
              "rtc_reads=%u rtc_jumps=%u mouse_packets=%u audio_signals=%u\n",
              state.kind == SESSION_FOCUS ? "focus" : "break",
              state.running ? 1u : 0u,
              (unsigned long long)state.active_ticks,
              (unsigned long long)state.lifetime_ticks,
              state.completions, state.rtc.year, state.rtc.month, state.rtc.day,
              state.rtc.hour, state.rtc.minute, state.rtc.second,
              state.rtc_reads, state.rtc_jumps, mouse.packets, audio.signals);
  result = event_failed ? 1 : 0;

cleanup:
  if (audio.available) {
    audio_stop();
    (void)lcom_phys_unmap(audio.pcm, audio.info.pcm_bytes);
  }
  if (mouse.command_sent) (void)mouse_send_command(MOUSE_CMD_DISABLE_DR);
  if (mouse_subscribed) (void)lcom_irq_unsubscribe(&mouse_irq);
  if (keyboard_subscribed) (void)lcom_irq_unsubscribe(&keyboard_irq);
  if (timer_subscribed) (void)lcom_irq_unsubscribe(&timer_irq);
  if (framebuffer_mapped) {
    (void)lcom_phys_unmap(framebuffer, video_info.framebuffer_size);
  }
  if (initialized) lcom_exit();
  return result;
}
