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

#define AQUARIUM_TICKS_PER_SECOND 60u
#define AQUARIUM_FISH_COUNT 9u
#define AQUARIUM_BUBBLE_COUNT 26u
#define AQUARIUM_FOOD_COUNT 14u

typedef enum {
  LIGHT_DAWN,
  LIGHT_DAY,
  LIGHT_SUNSET,
  LIGHT_NIGHT,
  LIGHT_COUNT,
} light_phase_t;

typedef struct {
  int x;
  int y;
  int vx;
  int vy;
  uint8_t radius_x;
  uint8_t radius_y;
  uint8_t species;
  uint8_t phase;
  uint32_t color;
  uint32_t accent;
} fish_t;

typedef struct {
  int x;
  int y;
  uint8_t radius;
  uint8_t speed;
} bubble_t;

typedef struct {
  int x;
  int y;
  uint8_t speed;
  bool active;
} food_t;

typedef struct {
  int x;
  int y;
  uint8_t bytes[3];
  uint8_t byte_count;
  uint8_t buttons;
  uint8_t previous_buttons;
  uint32_t packets;
  bool acknowledged;
  bool command_sent;
} aquarium_mouse_t;

typedef struct {
  int16_t *pcm;
  lcom_ac97_buffer_info_t info;
  uint32_t signals;
  bool available;
  bool muted;
} aquarium_audio_t;

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} aquarium_rtc_t;

typedef struct {
  fish_t fish[AQUARIUM_FISH_COUNT];
  bubble_t bubbles[AQUARIUM_BUBBLE_COUNT];
  food_t food[AQUARIUM_FOOD_COUNT];
  uint64_t ticks;
  uint64_t motion_ticks;
  uint64_t feed_until;
  uint32_t rng;
  uint32_t feeds;
  uint32_t lighting_changes;
  uint32_t rtc_reads;
  uint32_t bubble_resets;
  int target_x;
  int target_y;
  uint8_t rtc_year;
  uint8_t rtc_month;
  uint8_t rtc_day;
  uint8_t rtc_hour;
  uint8_t rtc_minute;
  uint8_t rtc_second;
  int8_t light_override;
  light_phase_t rtc_phase;
  bool rtc_valid;
  bool paused;
  bool bubbles_enabled;
  bool extended_key;
  char status[48];
} aquarium_state_t;

static const char *phase_names[LIGHT_COUNT] = {
    "DAWN", "DAY", "SUNSET", "NIGHT",
};

static const char *phase_ids[LIGHT_COUNT] = {
    "dawn", "day", "sunset", "night",
};

static uint8_t bcd_to_binary(uint8_t value) {
  return (uint8_t)(((value >> 4) * 10u) + (value & 0x0Fu));
}

static int rtc_read_register(uint8_t reg, uint8_t *value) {
  if (lcom_port_write8(RTC_ADDR_REG, reg) != LCOM_OK) return LCOM_ERR;
  return lcom_port_read8(RTC_DATA_REG, value);
}

static int read_rtc_once(aquarium_rtc_t *snapshot) {
  uint8_t reg_a = 0;
  uint8_t reg_b = 0;
  for (uint16_t attempt = 0; attempt < 1000u; attempt++) {
    if (rtc_read_register(RTC_REG_A, &reg_a) != LCOM_OK) return LCOM_ERR;
    if ((reg_a & RTC_UIP) == 0) break;
    if (attempt == 999u) return LCOM_TIMEOUT;
  }
  if (rtc_read_register(RTC_REG_B, &reg_b) != LCOM_OK ||
      rtc_read_register(RTC_REG_YEAR, &snapshot->year) != LCOM_OK ||
      rtc_read_register(RTC_REG_MONTH, &snapshot->month) != LCOM_OK ||
      rtc_read_register(RTC_REG_DAY, &snapshot->day) != LCOM_OK ||
      rtc_read_register(RTC_REG_HOURS, &snapshot->hour) != LCOM_OK ||
      rtc_read_register(RTC_REG_MINUTES, &snapshot->minute) != LCOM_OK ||
      rtc_read_register(RTC_REG_SECONDS, &snapshot->second) != LCOM_OK) {
    return LCOM_ERR;
  }

  bool afternoon = (snapshot->hour & BIT(7)) != 0;
  snapshot->hour &= (uint8_t)~BIT(7);
  if ((reg_b & RTC_DM) == 0) {
    snapshot->year = bcd_to_binary(snapshot->year);
    snapshot->month = bcd_to_binary(snapshot->month);
    snapshot->day = bcd_to_binary(snapshot->day);
    snapshot->hour = bcd_to_binary(snapshot->hour);
    snapshot->minute = bcd_to_binary(snapshot->minute);
    snapshot->second = bcd_to_binary(snapshot->second);
  }
  if ((reg_b & RTC_24H) == 0) {
    if (snapshot->hour == 12u) snapshot->hour = 0;
    if (afternoon) snapshot->hour = (uint8_t)(snapshot->hour + 12u);
  }
  if (snapshot->month < 1u || snapshot->month > 12u ||
      snapshot->day < 1u || snapshot->day > 31u ||
      snapshot->hour > 23u || snapshot->minute > 59u ||
      snapshot->second > 59u) {
    return LCOM_ERR;
  }
  return LCOM_OK;
}

static bool rtc_snapshots_match(const aquarium_rtc_t *a,
                                const aquarium_rtc_t *b) {
  return a->year == b->year && a->month == b->month && a->day == b->day &&
         a->hour == b->hour && a->minute == b->minute &&
         a->second == b->second;
}

static int read_rtc(aquarium_state_t *state) {
  for (uint8_t attempt = 0; attempt < 4u; attempt++) {
    aquarium_rtc_t first = {0};
    aquarium_rtc_t second = {0};
    if (read_rtc_once(&first) != LCOM_OK || read_rtc_once(&second) != LCOM_OK) {
      continue;
    }
    if (!rtc_snapshots_match(&first, &second)) continue;
    state->rtc_year = second.year;
    state->rtc_month = second.month;
    state->rtc_day = second.day;
    state->rtc_hour = second.hour;
    state->rtc_minute = second.minute;
    state->rtc_second = second.second;
    state->rtc_valid = true;
    state->rtc_reads++;
    return LCOM_OK;
  }
  return LCOM_ERR;
}

static light_phase_t phase_for_hour(uint8_t hour) {
  if (hour < 6u || hour >= 22u) return LIGHT_NIGHT;
  if (hour < 9u) return LIGHT_DAWN;
  if (hour < 18u) return LIGHT_DAY;
  return LIGHT_SUNSET;
}

static light_phase_t active_phase(const aquarium_state_t *state) {
  if (state->light_override >= 0 &&
      state->light_override < (int8_t)LIGHT_COUNT) {
    return (light_phase_t)state->light_override;
  }
  return state->rtc_phase;
}

static void set_status(aquarium_state_t *state, const char *status) {
  snprintf(state->status, sizeof(state->status), "%s", status);
}

static uint32_t next_random(aquarium_state_t *state) {
  state->rng = state->rng * 1664525u + 1013904223u;
  return state->rng;
}

static void initialize_habitat(aquarium_state_t *state) {
  static const uint32_t body_colors[AQUARIUM_FISH_COUNT] = {
      0xFFB45Fu, 0x65D7D0u, 0xFF7D8Fu, 0x9E8CFFu, 0xF5D56Bu,
      0x67B6F4u, 0xE889D2u, 0x77D393u, 0xF1975Bu,
  };
  static const uint32_t accent_colors[AQUARIUM_FISH_COUNT] = {
      0xFFF0C2u, 0xD0FFF4u, 0xFFE2E8u, 0xE8E2FFu, 0xFFF8C7u,
      0xDCEEFFu, 0xFFE0F7u, 0xDFFFF0u, 0xFFE6CBu,
  };
  state->rng = 0x4D4C4142u;
  for (uint8_t i = 0; i < AQUARIUM_FISH_COUNT; i++) {
    fish_t *fish = &state->fish[i];
    fish->x = 90 + (i % 5u) * 142;
    fish->y = 180 + (i / 5u) * 190 + (i % 3u) * 26;
    fish->vx = (i & 1u) == 0 ? 1 : -1;
    fish->vy = 0;
    fish->radius_x = (uint8_t)(19u + (i % 3u) * 4u);
    fish->radius_y = (uint8_t)(10u + (i % 2u) * 3u);
    fish->species = (uint8_t)(i % 4u);
    fish->phase = (uint8_t)(i * 19u);
    fish->color = body_colors[i];
    fish->accent = accent_colors[i];
  }
  for (uint8_t i = 0; i < AQUARIUM_BUBBLE_COUNT; i++) {
    bubble_t *bubble = &state->bubbles[i];
    bubble->x = 30 + (int)(next_random(state) % 740u);
    bubble->y = 110 + (int)(next_random(state) % 415u);
    bubble->radius = (uint8_t)(2u + next_random(state) % 4u);
    bubble->speed = (uint8_t)(1u + next_random(state) % 3u);
  }
  memset(state->food, 0, sizeof(state->food));
  state->target_x = 400;
  state->target_y = 300;
  state->feed_until = 0;
  state->paused = false;
  state->bubbles_enabled = true;
  set_status(state, "HABITAT CALM");
}

static int audio_start(aquarium_audio_t *audio, size_t byte_count) {
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

static int16_t triangle_sample(uint32_t frame, uint16_t hz, int32_t amplitude) {
  uint32_t period = AC97_DEFAULT_SAMPLE_RATE / hz;
  if (period < 4u) period = 4u;
  uint32_t phase = frame % period;
  int32_t value;
  if (phase < period / 2u) {
    value = -amplitude +
            (int32_t)(phase * (uint32_t)(4 * amplitude) / period);
  } else {
    value = 3 * amplitude -
            (int32_t)(phase * (uint32_t)(4 * amplitude) / period);
  }
  return (int16_t)value;
}

static void play_water_chime(aquarium_audio_t *audio, bool bright) {
  if (!audio->available || audio->muted) return;
  const uint32_t frames = 9600u;
  const uint8_t channels = AC97_DEFAULT_CHANNELS;
  size_t samples = (size_t)frames * channels;
  size_t bytes = samples * sizeof(int16_t);
  if (bytes > audio->info.pcm_bytes || bytes > 0xFFFFu) return;
  for (uint32_t frame = 0; frame < frames; frame++) {
    uint16_t base = bright ? 740u : 440u;
    uint16_t hz = (uint16_t)(base + frame * (bright ? 260u : 130u) / frames);
    int32_t envelope = (int32_t)(frames - frame);
    int32_t amplitude = 4600 * envelope / (int32_t)frames;
    int32_t sample = triangle_sample(frame, hz, amplitude);
    sample += triangle_sample(frame, (uint16_t)(hz * 2u), amplitude / 4);
    audio->pcm[frame * channels] = (int16_t)sample;
    audio->pcm[frame * channels + 1u] = (int16_t)(sample * 4 / 5);
  }
  audio_stop();
  if (audio_start(audio, bytes) == LCOM_OK) audio->signals++;
}

static void scatter_food(aquarium_state_t *state, aquarium_audio_t *audio,
                         int x, int y) {
  state->target_x = x;
  state->target_y = y;
  state->feed_until = state->ticks + 5u * AQUARIUM_TICKS_PER_SECOND;
  state->feeds++;
  for (uint8_t i = 0; i < AQUARIUM_FOOD_COUNT; i++) {
    food_t *food = &state->food[i];
    int spread = (int)(next_random(state) % 35u) - 17;
    food->x = x + spread;
    food->y = y - (int)(next_random(state) % 22u);
    food->speed = (uint8_t)(1u + next_random(state) % 2u);
    food->active = true;
  }
  set_status(state, "FEEDING TIME - FISH ARE ATTRACTED");
  play_water_chime(audio, true);
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

static bool consume_mouse_byte(aquarium_mouse_t *mouse, uint8_t value) {
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
  if (mouse->x < 8) mouse->x = 8;
  if (mouse->x > 791) mouse->x = 791;
  if (mouse->y < 92) mouse->y = 92;
  if (mouse->y > 548) mouse->y = 548;
  mouse->previous_buttons = mouse->buttons;
  mouse->buttons = (uint8_t)(mouse->bytes[0] & 0x07u);
  mouse->packets++;
  mouse->byte_count = 0;
  return true;
}

static void handle_mouse(aquarium_state_t *state, aquarium_audio_t *audio,
                         aquarium_mouse_t *mouse) {
  bool left_pressed = (mouse->buttons & BIT(0)) != 0 &&
                      (mouse->previous_buttons & BIT(0)) == 0;
  state->target_x = mouse->x;
  state->target_y = mouse->y;
  if (left_pressed) scatter_food(state, audio, mouse->x, mouse->y);
}

static void cycle_lighting(aquarium_state_t *state,
                           aquarium_audio_t *audio) {
  if (state->light_override < 0) {
    state->light_override = LIGHT_DAY;
  } else if (state->light_override == LIGHT_NIGHT) {
    state->light_override = -1;
  } else {
    state->light_override++;
  }
  state->lighting_changes++;
  if (state->light_override < 0) {
    set_status(state, "LIGHTING FOLLOWS RTC AGAIN");
  } else {
    char status[48];
    snprintf(status, sizeof(status), "MANUAL LIGHTING - %s",
             phase_names[(uint8_t)state->light_override]);
    set_status(state, status);
  }
  play_water_chime(audio, false);
}

static void handle_keyboard(aquarium_state_t *state, aquarium_audio_t *audio,
                            aquarium_mouse_t *mouse, uint8_t data,
                            bool *running) {
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
  if (code == 0x21u) {
    scatter_food(state, audio, mouse->x, mouse->y);
  } else if (code == 0x26u) {
    cycle_lighting(state, audio);
  } else if (code == 0x19u || code == 0x39u) {
    state->paused = !state->paused;
    set_status(state, state->paused ? "PIT MOTION PAUSED" : "PIT MOTION RESUMED");
  } else if (code == 0x1Fu) {
    audio->muted = !audio->muted;
    set_status(state, audio->muted ? "EVENT TONES MUTED" : "EVENT TONES ENABLED");
  } else if (code == 0x30u) {
    state->bubbles_enabled = !state->bubbles_enabled;
    set_status(state, state->bubbles_enabled ? "BUBBLES ENABLED" : "BUBBLES HIDDEN");
  } else if (code == 0x13u) {
    initialize_habitat(state);
    set_status(state, "HABITAT RESET");
  }
}

static void steer_fish(aquarium_state_t *state, fish_t *fish) {
  bool feeding = state->ticks < state->feed_until;
  if (feeding) {
    int dx = state->target_x - fish->x;
    int dy = state->target_y - fish->y;
    if (dx > 14) fish->vx = fish->species == 3u ? 2 : 1;
    if (dx < -14) fish->vx = fish->species == 3u ? -2 : -1;
    if (dy > 8) fish->vy = 1;
    else if (dy < -8) fish->vy = -1;
    else fish->vy = 0;
  } else {
    uint32_t wave = (state->motion_ticks / 2u + fish->phase) % 120u;
    if (wave < 30u) fish->vy = -1;
    else if (wave < 60u) fish->vy = 0;
    else if (wave < 90u) fish->vy = 1;
    else fish->vy = 0;
  }
  fish->x += fish->vx;
  fish->y += fish->vy;
  if (fish->x < 42) {
    fish->x = 42;
    fish->vx = fish->vx < 0 ? -fish->vx : fish->vx;
  } else if (fish->x > 758) {
    fish->x = 758;
    fish->vx = fish->vx > 0 ? -fish->vx : fish->vx;
  }
  if (fish->y < 128) {
    fish->y = 128;
    fish->vy = 1;
  } else if (fish->y > 492) {
    fish->y = 492;
    fish->vy = -1;
  }
}

static void update_habitat(aquarium_state_t *state) {
  state->ticks++;
  if (state->ticks == 1u ||
      state->ticks % (AQUARIUM_TICKS_PER_SECOND / 4u) == 0u) {
    light_phase_t previous = state->rtc_phase;
    if (read_rtc(state) == LCOM_OK) {
      state->rtc_phase = phase_for_hour(state->rtc_hour);
      if (state->ticks > 1u && state->rtc_phase != previous) {
        state->lighting_changes++;
        char status[48];
        snprintf(status, sizeof(status), "RTC LIGHT CHANGED TO %s",
                 phase_names[state->rtc_phase]);
        set_status(state, status);
      }
    }
  }
  if (state->paused) return;
  state->motion_ticks++;
  if ((state->motion_ticks & 1u) == 0) {
    for (uint8_t i = 0; i < AQUARIUM_FISH_COUNT; i++) {
      steer_fish(state, &state->fish[i]);
    }
  }
  for (uint8_t i = 0; i < AQUARIUM_BUBBLE_COUNT; i++) {
    bubble_t *bubble = &state->bubbles[i];
    if (state->motion_ticks % bubble->speed == 0u) bubble->y--;
    if (bubble->y < 102) {
      bubble->x = 30 + (int)(next_random(state) % 740u);
      bubble->y = 525 + (int)(next_random(state) % 20u);
      bubble->radius = (uint8_t)(2u + next_random(state) % 4u);
      state->bubble_resets++;
    }
  }
  for (uint8_t i = 0; i < AQUARIUM_FOOD_COUNT; i++) {
    food_t *food = &state->food[i];
    if (!food->active) continue;
    if (state->motion_ticks % food->speed == 0u) food->y++;
    if (food->y > 520) food->active = false;
  }
  if (state->ticks >= state->feed_until && state->feed_until != 0u) {
    state->feed_until = 0;
    set_status(state, "HABITAT CALM");
  }
}

static uint8_t color_channel(uint32_t color, uint8_t shift) {
  return (uint8_t)((color >> shift) & 0xFFu);
}

static uint32_t blend_color(uint32_t top, uint32_t bottom,
                            uint32_t numerator, uint32_t denominator) {
  if (denominator == 0) return top;
  uint32_t inverse = denominator - numerator;
  uint8_t red = (uint8_t)((color_channel(top, 16) * inverse +
                           color_channel(bottom, 16) * numerator) /
                          denominator);
  uint8_t green = (uint8_t)((color_channel(top, 8) * inverse +
                             color_channel(bottom, 8) * numerator) /
                            denominator);
  uint8_t blue = (uint8_t)((color_channel(top, 0) * inverse +
                            color_channel(bottom, 0) * numerator) /
                           denominator);
  return example_rgb(red, green, blue);
}

static void fill_circle(example_canvas_t *canvas, int cx, int cy, int radius,
                        uint32_t color) {
  for (int y = -radius; y <= radius; y++) {
    int span = radius;
    while (span > 0 && span * span + y * y > radius * radius) span--;
    example_fill_rect(canvas, cx - span, cy + y, span * 2 + 1, 1, color);
  }
}

static void stroke_circle(example_canvas_t *canvas, int cx, int cy, int radius,
                          uint32_t color) {
  int outer = radius * radius;
  int inner_radius = radius > 1 ? radius - 2 : 0;
  int inner = inner_radius * inner_radius;
  for (int y = -radius; y <= radius; y++) {
    for (int x = -radius; x <= radius; x++) {
      int distance = x * x + y * y;
      if (distance <= outer && distance >= inner) {
        example_put_pixel(canvas, cx + x, cy + y, color);
      }
    }
  }
}

static void fill_ellipse(example_canvas_t *canvas, int cx, int cy,
                         int radius_x, int radius_y, uint32_t color) {
  int rx2 = radius_x * radius_x;
  int ry2 = radius_y * radius_y;
  int limit = rx2 * ry2;
  for (int y = -radius_y; y <= radius_y; y++) {
    int span = radius_x;
    while (span > 0 && span * span * ry2 + y * y * rx2 > limit) span--;
    example_fill_rect(canvas, cx - span, cy + y, span * 2 + 1, 1, color);
  }
}

static void draw_tail(example_canvas_t *canvas, const fish_t *fish,
                      uint32_t color) {
  int direction = fish->vx >= 0 ? -1 : 1;
  int base_x = fish->x + direction * fish->radius_x;
  for (int row = -fish->radius_y; row <= fish->radius_y; row++) {
    int width = fish->radius_y - (row < 0 ? -row : row) + 2;
    int x = base_x + direction * width;
    example_fill_rect(canvas,
                      direction < 0 ? x : base_x,
                      fish->y + row,
                      width, 1, color);
  }
}

static void draw_fish(example_canvas_t *canvas, const fish_t *fish,
                      light_phase_t phase, uint64_t ticks) {
  uint32_t body = fish->color;
  uint32_t accent = fish->accent;
  if (phase == LIGHT_NIGHT) {
    body = blend_color(body, example_rgb(52, 79, 118), 2u, 5u);
    accent = blend_color(accent, example_rgb(103, 154, 196), 1u, 3u);
  }
  int bob = ((ticks / 10u + fish->phase) % 12u) < 6u ? 0 : 1;
  fish_t posed = *fish;
  posed.y += bob;
  draw_tail(canvas, &posed, body);
  fill_ellipse(canvas, posed.x, posed.y, posed.radius_x, posed.radius_y, body);

  int direction = posed.vx >= 0 ? 1 : -1;
  if (posed.species == 0u) {
    example_fill_rect(canvas, posed.x - 2, posed.y - posed.radius_y,
                      4, posed.radius_y * 2 + 1, accent);
  } else if (posed.species == 1u) {
    fill_circle(canvas, posed.x - direction * 5, posed.y, 4, accent);
  } else if (posed.species == 2u) {
    example_fill_rect(canvas, posed.x - posed.radius_x / 2,
                      posed.y - 2, posed.radius_x, 4, accent);
  } else {
    fill_ellipse(canvas, posed.x, posed.y + posed.radius_y / 2,
                 posed.radius_x / 2, 3, accent);
  }
  int eye_x = posed.x + direction * (posed.radius_x - 7);
  fill_circle(canvas, eye_x, posed.y - 3, 3, example_rgb(244, 248, 248));
  fill_circle(canvas, eye_x + direction, posed.y - 3, 1,
              example_rgb(15, 28, 39));
}

static void draw_plant(example_canvas_t *canvas, int x, int base_y,
                       int height, uint32_t stem, uint32_t leaf,
                       uint64_t ticks) {
  int sway = ((ticks / 18u + (uint64_t)x) % 16u) < 8u ? -2 : 2;
  example_fill_rect(canvas, x, base_y - height, 4, height, stem);
  for (int y = base_y - 20; y > base_y - height; y -= 22) {
    int direction = ((y / 20) & 1) == 0 ? 1 : -1;
    fill_ellipse(canvas, x + direction * (8 + sway), y,
                 10, 4, leaf);
  }
}

static void draw_water(example_canvas_t *canvas, light_phase_t phase,
                       uint64_t ticks) {
  static const uint32_t top_colors[LIGHT_COUNT] = {
      0x547EA3u, 0x3B9FC2u, 0x806B91u, 0x172B52u,
  };
  static const uint32_t bottom_colors[LIGHT_COUNT] = {
      0x174E6Bu, 0x12627Du, 0x353E68u, 0x0A203Bu,
  };
  const int top_y = 88;
  const int bottom_y = 540;
  for (int y = top_y; y < bottom_y; y++) {
    uint32_t color = blend_color(top_colors[phase], bottom_colors[phase],
                                 (uint32_t)(y - top_y),
                                 (uint32_t)(bottom_y - top_y));
    example_fill_rect(canvas, 0, y, 800, 1, color);
  }

  uint32_t glint = phase == LIGHT_NIGHT ? example_rgb(83, 132, 177)
                                       : example_rgb(151, 229, 235);
  for (uint8_t i = 0; i < 9u; i++) {
    int x = (int)((i * 103u + ticks / 2u) % 860u) - 60;
    int y = 112 + (i % 3u) * 24;
    example_fill_rect(canvas, x, y, 48, 2, glint);
    example_fill_rect(canvas, x + 12, y + 5, 28, 1, glint);
  }
}

static void draw_habitat_decor(example_canvas_t *canvas,
                               const aquarium_state_t *state,
                               light_phase_t phase) {
  uint32_t sand_top = phase == LIGHT_NIGHT ? example_rgb(73, 83, 92)
                                          : example_rgb(209, 183, 127);
  uint32_t sand_bottom = phase == LIGHT_NIGHT ? example_rgb(48, 57, 68)
                                             : example_rgb(154, 126, 86);
  example_fill_rect(canvas, 0, 516, 800, 24, sand_top);
  example_fill_rect(canvas, 0, 534, 800, 26, sand_bottom);
  for (uint8_t i = 0; i < 22u; i++) {
    int x = 12 + i * 37;
    int y = 522 + (i % 4u) * 7;
    fill_circle(canvas, x, y, 2, example_rgb(115, 104, 85));
  }
  draw_plant(canvas, 72, 527, 124, example_rgb(35, 111, 82),
             example_rgb(64, 165, 111), state->motion_ticks);
  draw_plant(canvas, 710, 530, 156, example_rgb(48, 101, 88),
             example_rgb(86, 154, 125), state->motion_ticks + 11u);
  draw_plant(canvas, 650, 530, 90, example_rgb(45, 116, 75),
             example_rgb(103, 181, 111), state->motion_ticks + 23u);
  fill_ellipse(canvas, 150, 521, 48, 19, example_rgb(70, 84, 90));
  fill_ellipse(canvas, 177, 516, 26, 14, example_rgb(92, 104, 107));
  fill_ellipse(canvas, 580, 525, 58, 21, example_rgb(62, 76, 86));
  fill_ellipse(canvas, 552, 516, 29, 15, example_rgb(86, 99, 104));
}

static void draw_bubbles(example_canvas_t *canvas,
                         const aquarium_state_t *state,
                         light_phase_t phase) {
  if (!state->bubbles_enabled) return;
  uint32_t edge = phase == LIGHT_NIGHT ? example_rgb(82, 130, 169)
                                      : example_rgb(166, 230, 235);
  for (uint8_t i = 0; i < AQUARIUM_BUBBLE_COUNT; i++) {
    const bubble_t *bubble = &state->bubbles[i];
    stroke_circle(canvas, bubble->x, bubble->y, bubble->radius, edge);
    example_put_pixel(canvas, bubble->x - 1, bubble->y - bubble->radius + 1,
                      example_rgb(222, 248, 246));
  }
}

static void draw_food(example_canvas_t *canvas, const aquarium_state_t *state) {
  for (uint8_t i = 0; i < AQUARIUM_FOOD_COUNT; i++) {
    if (!state->food[i].active) continue;
    fill_circle(canvas, state->food[i].x, state->food[i].y, 2,
                example_rgb(242, 186, 83));
  }
}

static void draw_cursor(example_canvas_t *canvas,
                        const aquarium_mouse_t *mouse,
                        bool feeding) {
  uint32_t color = feeding ? example_rgb(255, 210, 100)
                           : example_rgb(231, 245, 246);
  stroke_circle(canvas, mouse->x, mouse->y, feeding ? 11 : 8, color);
  example_fill_rect(canvas, mouse->x - 3, mouse->y, 7, 1, color);
  example_fill_rect(canvas, mouse->x, mouse->y - 3, 1, 7, color);
}

static void draw_phase_icon(example_canvas_t *canvas, light_phase_t phase,
                            int x, int y) {
  if (phase == LIGHT_NIGHT) {
    fill_circle(canvas, x, y, 12, example_rgb(222, 233, 213));
    fill_circle(canvas, x + 6, y - 4, 11, example_rgb(28, 36, 52));
  } else {
    uint32_t color = phase == LIGHT_SUNSET ? example_rgb(255, 151, 98)
                                          : example_rgb(255, 219, 103);
    fill_circle(canvas, x, y, 10, color);
    for (uint8_t ray = 0; ray < 4u; ray++) {
      if (ray == 0u) example_fill_rect(canvas, x - 1, y - 18, 3, 6, color);
      if (ray == 1u) example_fill_rect(canvas, x - 1, y + 13, 3, 6, color);
      if (ray == 2u) example_fill_rect(canvas, x - 18, y - 1, 6, 3, color);
      if (ray == 3u) example_fill_rect(canvas, x + 13, y - 1, 6, 3, color);
    }
  }
}

static void draw_aquarium(example_canvas_t *canvas,
                          const aquarium_state_t *state,
                          const aquarium_mouse_t *mouse,
                          const aquarium_audio_t *audio) {
  light_phase_t phase = active_phase(state);
  draw_water(canvas, phase, state->motion_ticks);
  draw_habitat_decor(canvas, state, phase);
  draw_bubbles(canvas, state, phase);
  for (uint8_t i = 0; i < AQUARIUM_FISH_COUNT; i++) {
    draw_fish(canvas, &state->fish[i], phase, state->motion_ticks);
  }
  draw_food(canvas, state);

  example_fill_rect(canvas, 0, 0, 800, 88, example_rgb(20, 29, 42));
  example_fill_rect(canvas, 0, 86, 800, 2,
                    phase == LIGHT_NIGHT ? example_rgb(88, 140, 193)
                                         : example_rgb(99, 210, 197));
  example_draw_text(canvas, 25, 16, "AMBIENT AQUARIUM",
                    example_rgb(239, 244, 247), 3);
  example_draw_text(canvas, 27, 49, "A QUIET HARDWARE CLOCK HABITAT",
                    example_rgb(141, 159, 181), 1);

  example_fill_rect(canvas, 412, 15, 170, 56, example_rgb(31, 42, 58));
  example_stroke_rect(canvas, 412, 15, 170, 56, 1, example_rgb(63, 80, 103));
  draw_phase_icon(canvas, phase, 436, 43);
  example_draw_text(canvas, 459, 25,
                    state->light_override < 0 ? "RTC LIGHTING" : "MANUAL OVERRIDE",
                    example_rgb(137, 156, 182), 1);
  char rtc_text[32];
  if (state->rtc_valid) {
    snprintf(rtc_text, sizeof(rtc_text), "%02u:%02u:%02u  %s",
             state->rtc_hour, state->rtc_minute, state->rtc_second,
             phase_names[phase]);
  } else {
    snprintf(rtc_text, sizeof(rtc_text), "--:--:--  RTC ERROR");
  }
  example_draw_text(canvas, 459, 47, rtc_text,
                    example_rgb(233, 238, 243), 1);

  example_fill_rect(canvas, 594, 15, 178, 56, example_rgb(31, 42, 58));
  example_stroke_rect(canvas, 594, 15, 178, 56, 1, example_rgb(63, 80, 103));
  example_draw_text(canvas, 610, 25, "PIT MOTION",
                    example_rgb(137, 156, 182), 1);
  char motion_text[32];
  snprintf(motion_text, sizeof(motion_text), "%s  %llu TICKS",
           state->paused ? "PAUSED" : "FLOWING",
           (unsigned long long)state->motion_ticks);
  example_draw_text(canvas, 610, 47, motion_text,
                    state->paused ? example_rgb(255, 183, 102)
                                  : example_rgb(104, 218, 176),
                    1);

  example_fill_rect(canvas, 0, 560, 800, 40, example_rgb(13, 20, 29));
  example_draw_text(canvas, 20, 572,
                    "CLICK / F FEED   L LIGHT   P PAUSE   B BUBBLES   S SOUND   ESC EXIT",
                    example_rgb(143, 160, 183), 1);
  char status[96];
  snprintf(status, sizeof(status), "%s  FEEDS %u  SOUND %s",
           state->status, state->feeds,
           audio->available ? (audio->muted ? "MUTED" : "ON") : "N/A");
  example_draw_text(canvas, 20, 545, status,
                    example_rgb(220, 234, 232), 1);
  draw_cursor(canvas, mouse, state->ticks < state->feed_until);
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
  aquarium_state_t state = {.light_override = -1};
  aquarium_mouse_t mouse = {.x = 400, .y = 300};
  aquarium_audio_t audio = {0};

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

  initialize_habitat(&state);
  if (read_rtc(&state) == LCOM_OK) {
    state.rtc_phase = phase_for_hour(state.rtc_hour);
  } else {
    state.rtc_phase = LIGHT_DAY;
    set_status(&state, "RTC ERROR - DAYLIGHT FALLBACK");
  }
  example_canvas_t canvas = {framebuffer, video_info};
  bool running = true;
  bool event_failed = false;
  draw_aquarium(&canvas, &state, &mouse, &audio);
  (void)lcom_vbe_present();

  while (running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      event_failed = true;
      break;
    }
    if ((event.irq_mask & keyboard_irq.mask) != 0) {
      uint8_t data = 0;
      if (read_kbc_byte(false, &data) == LCOM_OK) {
        handle_keyboard(&state, &audio, &mouse, data, &running);
      }
    }
    if (mouse_subscribed && (event.irq_mask & mouse_irq.mask) != 0) {
      uint8_t data = 0;
      if (read_kbc_byte(true, &data) == LCOM_OK &&
          consume_mouse_byte(&mouse, data)) {
        handle_mouse(&state, &audio, &mouse);
      }
    }
    if ((event.irq_mask & timer_irq.mask) != 0) update_habitat(&state);
    draw_aquarium(&canvas, &state, &mouse, &audio);
    (void)lcom_vbe_present();
  }

  draw_aquarium(&canvas, &state, &mouse, &audio);
  (void)lcom_vbe_present();
  light_phase_t final_phase = active_phase(&state);
  lcom_printf("aquarium summary ticks=%llu motion_ticks=%llu rtc=%02u-%02u-%02uT%02u:%02u:%02u "
              "phase=%s lighting=%s feeds=%u mouse_packets=%u bubble_resets=%u "
              "rtc_reads=%u lighting_changes=%u audio_signals=%u paused=%u\n",
              (unsigned long long)state.ticks,
              (unsigned long long)state.motion_ticks,
              state.rtc_year, state.rtc_month, state.rtc_day,
              state.rtc_hour, state.rtc_minute, state.rtc_second,
              phase_ids[final_phase],
              state.light_override < 0 ? "rtc" : "manual",
              state.feeds, mouse.packets, state.bubble_resets,
              state.rtc_reads, state.lighting_changes, audio.signals,
              state.paused ? 1u : 0u);
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
