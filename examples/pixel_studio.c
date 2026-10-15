#include "example_ui.h"

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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SPRITE_SIDE 16
#define SPRITE_PIXELS (SPRITE_SIDE * SPRITE_SIDE)
#define HISTORY_CAPACITY 32
#define PALETTE_SIZE 16

typedef enum {
  TOOL_PENCIL,
  TOOL_ERASER,
  TOOL_FILL,
  TOOL_EYEDROPPER,
  TOOL_COUNT
} pixel_tool_t;

typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  bool valid;
} wall_clock_t;

typedef struct {
  uint8_t bytes[3];
  uint8_t index;
} mouse_parser_t;

typedef struct {
  uint8_t pixels[SPRITE_PIXELS];
} sprite_snapshot_t;

typedef struct {
  uint8_t pixels[SPRITE_PIXELS];
  uint8_t saved_pixels[SPRITE_PIXELS];
  sprite_snapshot_t history[HISTORY_CAPACITY];
  uint8_t history_length;
  uint8_t history_cursor;
  uint8_t selected_color;
  uint8_t zoom_index;
  pixel_tool_t tool;
  int key_x, key_y;
  bool keyboard_cursor;
  int mouse_x;
  int mouse_y;
  int last_cell_x;
  int last_cell_y;
  uint8_t previous_buttons;
  uint32_t frame;
  uint32_t committed_edits;
  uint32_t export_count;
  bool drawing;
  bool stroke_changed;
  bool grid_visible;
  bool help_visible;
  example_modifiers_t modifiers;
  bool extended;
  wall_clock_t clock;
  char status[72];
} pixel_studio_t;

static const uint32_t palette[PALETTE_SIZE] = {
    0x000000, 0x172238, 0x31486C, 0x637A9F,
    0xE8EEF5, 0xFFF3D6, 0xF4B860, 0xE76F51,
    0xB83B5E, 0x713E72, 0x4F46A5, 0x4D80C9,
    0x54B7D3, 0x61C98D, 0x3D8A66, 0x9ACD68,
};

static const char *tool_names[TOOL_COUNT] = {
    "Pencil", "Eraser", "Fill", "Pick",
};

static const char *tool_keys[TOOL_COUNT] = {
    "B", "E", "F", "I",
};

static const uint8_t zoom_sizes[3] = {18, 22, 26};

static bool point_in_rect(int px, int py, int x, int y, int w, int h) {
  return px >= x && py >= y && px < x + w && py < y + h;
}

static int clamp_int(int value, int low, int high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

static void set_status(pixel_studio_t *studio, const char *text) {
  snprintf(studio->status, sizeof(studio->status), "%s", text);
}

static int rtc_read_register(uint8_t reg, uint8_t *value) {
  if (lcom_port_write8(RTC_ADDR_REG, reg) != LCOM_OK) return LCOM_ERR;
  return lcom_port_read8(RTC_DATA_REG, value);
}

static uint8_t rtc_decode(uint8_t value, bool binary) {
  if (binary) return value;
  return (uint8_t)((value & 0x0Fu) + ((value >> 4) * 10u));
}

static bool read_wall_clock(wall_clock_t *clock) {
  for (uint8_t sample = 0; sample < 4; sample++) {
    uint8_t reg_a = 0;
    bool ready = false;
    for (uint8_t poll = 0; poll < 4; poll++) {
      if (rtc_read_register(RTC_REG_A, &reg_a) != LCOM_OK) return false;
      if ((reg_a & RTC_UIP) == 0) {
        ready = true;
        break;
      }
    }
    if (!ready) continue;

    uint8_t reg_b = 0;
    uint8_t second = 0;
    uint8_t second_check = 0;
    uint8_t minute = 0;
    uint8_t hour = 0;
    uint8_t day = 0;
    uint8_t month = 0;
    uint8_t year = 0;
    if (rtc_read_register(RTC_REG_B, &reg_b) != LCOM_OK ||
        rtc_read_register(RTC_REG_SECONDS, &second) != LCOM_OK ||
        rtc_read_register(RTC_REG_MINUTES, &minute) != LCOM_OK ||
        rtc_read_register(RTC_REG_HOURS, &hour) != LCOM_OK ||
        rtc_read_register(RTC_REG_DAY, &day) != LCOM_OK ||
        rtc_read_register(RTC_REG_MONTH, &month) != LCOM_OK ||
        rtc_read_register(RTC_REG_YEAR, &year) != LCOM_OK ||
        rtc_read_register(RTC_REG_SECONDS, &second_check) != LCOM_OK) {
      return false;
    }
    if (second != second_check) continue;

    bool binary = (reg_b & RTC_DM) != 0;
    bool afternoon = (hour & BIT(7)) != 0;
    wall_clock_t fresh = {
        .year = (uint16_t)(2000u + rtc_decode(year, binary)),
        .month = rtc_decode(month, binary),
        .day = rtc_decode(day, binary),
        .hour = rtc_decode((uint8_t)(hour & 0x7Fu), binary),
        .minute = rtc_decode(minute, binary),
        .second = rtc_decode(second, binary),
        .valid = false,
    };
    if ((reg_b & RTC_24H) == 0) {
      if (fresh.hour == 12u) fresh.hour = 0;
      if (afternoon) fresh.hour = (uint8_t)(fresh.hour + 12u);
    }
    fresh.valid = fresh.month >= 1u && fresh.month <= 12u &&
                  fresh.day >= 1u && fresh.day <= 31u &&
                  fresh.hour <= 23u && fresh.minute <= 59u &&
                  fresh.second <= 59u;
    if (!fresh.valid) return false;
    *clock = fresh;
    return true;
  }
  return false;
}

static void format_clock(const wall_clock_t *clock, char *out, size_t size) {
  if (!clock->valid) {
    snprintf(out, size, "RTC --:--:--");
    return;
  }
  snprintf(out, size, "%02u:%02u:%02u",
           (unsigned)clock->hour, (unsigned)clock->minute,
           (unsigned)clock->second);
}




static int configure_pit_60hz(void) {
  uint32_t divisor = TIMER_FREQ / 60u;
  if (lcom_port_write8(TIMER_CTRL,
                       TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE) != LCOM_OK) {
    return LCOM_ERR;
  }
  if (lcom_port_write8(TIMER_0, (uint8_t)(divisor & 0xFFu)) != LCOM_OK) {
    return LCOM_ERR;
  }
  return lcom_port_write8(TIMER_0, (uint8_t)((divisor >> 8) & 0xFFu));
}

static int set_mouse_reporting(bool enabled) {
  if (lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) != LCOM_OK) {
    return LCOM_ERR;
  }
  return lcom_port_write8(KBC_IN_BUF,
                          enabled ? MOUSE_CMD_ENABLE_DR : MOUSE_CMD_DISABLE_DR);
}

static void history_reset(pixel_studio_t *studio) {
  memcpy(studio->history[0].pixels, studio->pixels, sizeof(studio->pixels));
  studio->history_length = 1;
  studio->history_cursor = 0;
}

static bool history_commit(pixel_studio_t *studio) {
  if (memcmp(studio->history[studio->history_cursor].pixels,
             studio->pixels, sizeof(studio->pixels)) == 0) {
    return false;
  }
  studio->history_length = (uint8_t)(studio->history_cursor + 1u);
  if (studio->history_length == HISTORY_CAPACITY) {
    memmove(&studio->history[0], &studio->history[1],
            (HISTORY_CAPACITY - 1u) * sizeof(studio->history[0]));
    studio->history_length--;
    studio->history_cursor--;
  }
  memcpy(studio->history[studio->history_length].pixels,
         studio->pixels, sizeof(studio->pixels));
  studio->history_cursor = studio->history_length;
  studio->history_length++;
  studio->committed_edits++;
  return true;
}

static void history_undo(pixel_studio_t *studio) {
  if (studio->history_cursor == 0) {
    set_status(studio, "NOTHING TO UNDO");
    return;
  }
  studio->history_cursor--;
  memcpy(studio->pixels, studio->history[studio->history_cursor].pixels,
         sizeof(studio->pixels));
  set_status(studio, "UNDO RESTORED PREVIOUS SPRITE");
}

static void history_redo(pixel_studio_t *studio) {
  if ((uint8_t)(studio->history_cursor + 1u) >= studio->history_length) {
    set_status(studio, "NOTHING TO REDO");
    return;
  }
  studio->history_cursor++;
  memcpy(studio->pixels, studio->history[studio->history_cursor].pixels,
         sizeof(studio->pixels));
  set_status(studio, "REDO RESTORED NEXT SPRITE");
}

static bool studio_is_dirty(const pixel_studio_t *studio) {
  return memcmp(studio->pixels, studio->saved_pixels,
                sizeof(studio->pixels)) != 0;
}

static uint32_t sprite_checksum(const pixel_studio_t *studio) {
  uint32_t hash = 2166136261u;
  for (size_t i = 0; i < SPRITE_PIXELS; i++) {
    hash ^= studio->pixels[i];
    hash *= 16777619u;
  }
  return hash;
}

static unsigned painted_pixel_count(const pixel_studio_t *studio) {
  unsigned count = 0;
  for (size_t i = 0; i < SPRITE_PIXELS; i++) {
    if (studio->pixels[i] != 0) count++;
  }
  return count;
}

static void seed_sprite(pixel_studio_t *studio) {
  static const char *rows[SPRITE_SIDE] = {
      "................",
      "......4444......",
      ".....455554.....",
      "....45555554....",
      "...4455555544...",
      "...4B455554B4...",
      "...4555555554...",
      "...455F55F554...",
      "...4555555554...",
      "....45555554....",
      ".....444444.....",
      ".....4B44B4.....",
      "....4B4..4B4....",
      "...4BB4..4BB4...",
      "...444....444...",
      "................",
  };
  memset(studio, 0, sizeof(*studio));
  for (uint8_t y = 0; y < SPRITE_SIDE; y++) {
    for (uint8_t x = 0; x < SPRITE_SIDE; x++) {
      char c = rows[y][x];
      uint8_t color = 0;
      if (c >= '0' && c <= '9') color = (uint8_t)(c - '0');
      else if (c >= 'A' && c <= 'F') color = (uint8_t)(10 + c - 'A');
      studio->pixels[(size_t)y * SPRITE_SIDE + x] = color;
    }
  }
  memcpy(studio->saved_pixels, studio->pixels, sizeof(studio->pixels));
  studio->selected_color = 7;
  studio->zoom_index = 1;
  studio->tool = TOOL_PENCIL;
  studio->mouse_x = SCREEN_WIDTH / 2;
  studio->mouse_y = SCREEN_HEIGHT / 2;
  studio->last_cell_x = -1;
  studio->last_cell_y = -1;
  studio->grid_visible = true;
  set_status(studio, "READY - DRAW WITH THE LEFT MOUSE BUTTON");
  history_reset(studio);
}

static void canvas_geometry(const pixel_studio_t *studio, int *x, int *y,
                            int *cell_size) {
  int cell = zoom_sizes[studio->zoom_index];
  int extent = cell * SPRITE_SIDE;
  *x = 402 - extent / 2;
  *y = 310 - extent / 2;
  *cell_size = cell;
}

static bool screen_to_cell(const pixel_studio_t *studio, int screen_x,
                           int screen_y, int *cell_x, int *cell_y) {
  int origin_x = 0;
  int origin_y = 0;
  int cell = 0;
  canvas_geometry(studio, &origin_x, &origin_y, &cell);
  if (!point_in_rect(screen_x, screen_y, origin_x, origin_y,
                     cell * SPRITE_SIDE, cell * SPRITE_SIDE)) {
    return false;
  }
  *cell_x = (screen_x - origin_x) / cell;
  *cell_y = (screen_y - origin_y) / cell;
  return true;
}

static bool set_pixel(pixel_studio_t *studio, int x, int y, uint8_t color) {
  if (x < 0 || y < 0 || x >= SPRITE_SIDE || y >= SPRITE_SIDE) return false;
  size_t index = (size_t)y * SPRITE_SIDE + (size_t)x;
  if (studio->pixels[index] == color) return false;
  studio->pixels[index] = color;
  return true;
}

static bool apply_active_tool(pixel_studio_t *studio, int cell_x, int cell_y) {
  if (studio->tool == TOOL_PENCIL) {
    return set_pixel(studio, cell_x, cell_y, studio->selected_color);
  }
  if (studio->tool == TOOL_ERASER) {
    return set_pixel(studio, cell_x, cell_y, 0);
  }
  return false;
}

static void draw_cell_line(pixel_studio_t *studio, int x0, int y0,
                           int x1, int y1) {
  int dx = x1 > x0 ? x1 - x0 : x0 - x1;
  int sx = x0 < x1 ? 1 : -1;
  int dy_abs = y1 > y0 ? y1 - y0 : y0 - y1;
  int dy = -dy_abs;
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;
  for (;;) {
    if (apply_active_tool(studio, x0, y0)) studio->stroke_changed = true;
    if (x0 == x1 && y0 == y1) break;
    int twice_error = error * 2;
    if (twice_error >= dy) {
      error += dy;
      x0 += sx;
    }
    if (twice_error <= dx) {
      error += dx;
      y0 += sy;
    }
  }
}

static bool flood_fill(pixel_studio_t *studio, int start_x, int start_y,
                       uint8_t replacement) {
  size_t start = (size_t)start_y * SPRITE_SIDE + (size_t)start_x;
  uint8_t target = studio->pixels[start];
  if (target == replacement) return false;
  uint16_t queue[SPRITE_PIXELS];
  bool queued[SPRITE_PIXELS] = {false};
  size_t head = 0;
  size_t tail = 0;
  queue[tail++] = (uint16_t)start;
  queued[start] = true;
  while (head < tail) {
    uint16_t index = queue[head++];
    if (studio->pixels[index] != target) continue;
    studio->pixels[index] = replacement;
    int x = index % SPRITE_SIDE;
    int y = index / SPRITE_SIDE;
    const int next_x[4] = {x - 1, x + 1, x, x};
    const int next_y[4] = {y, y, y - 1, y + 1};
    for (uint8_t direction = 0; direction < 4; direction++) {
      int nx = next_x[direction];
      int ny = next_y[direction];
      if (nx < 0 || ny < 0 || nx >= SPRITE_SIDE || ny >= SPRITE_SIDE) continue;
      size_t next = (size_t)ny * SPRITE_SIDE + (size_t)nx;
      if (!queued[next] && studio->pixels[next] == target) {
        queued[next] = true;
        queue[tail++] = (uint16_t)next;
      }
    }
  }
  return true;
}

static void pick_color(pixel_studio_t *studio, int cell_x, int cell_y) {
  studio->selected_color =
      studio->pixels[(size_t)cell_y * SPRITE_SIDE + (size_t)cell_x];
  studio->tool = TOOL_PENCIL;
  char message[64];
  snprintf(message, sizeof(message), "PICKED PALETTE COLOR %u",
           (unsigned)studio->selected_color);
  set_status(studio, message);
}

static bool export_sprite(pixel_studio_t *studio, const char *path) {
  read_wall_clock(&studio->clock);
  char temporary[1024];
  int path_length = snprintf(temporary, sizeof(temporary), "%s.tmp", path);
  if (path_length < 0 || (size_t)path_length >= sizeof(temporary)) { set_status(studio, "Export path is too long. Choose a shorter path."); return false; }
  FILE *out = fopen(temporary, "wx");
  if (out == NULL) {
    set_status(studio, "EXPORT FAILED - CHECK OUTPUT PATH");
    return false;
  }
  fprintf(out, "/* Pixel Studio export: %04u-%02u-%02u %02u:%02u:%02u */\n",
          (unsigned)studio->clock.year, (unsigned)studio->clock.month,
          (unsigned)studio->clock.day, (unsigned)studio->clock.hour,
          (unsigned)studio->clock.minute, (unsigned)studio->clock.second);
  fprintf(out, "#ifndef PIXEL_STUDIO_SPRITE_H\n");
  fprintf(out, "#define PIXEL_STUDIO_SPRITE_H\n\n");
  fprintf(out, "#include <stdint.h>\n\n");
  fprintf(out, "#define PIXEL_STUDIO_SPRITE_WIDTH 16u\n");
  fprintf(out, "#define PIXEL_STUDIO_SPRITE_HEIGHT 16u\n\n");
  fprintf(out, "static const uint32_t pixel_studio_sprite[256] = {\n");
  for (uint8_t y = 0; y < SPRITE_SIDE; y++) {
    fprintf(out, "  ");
    for (uint8_t x = 0; x < SPRITE_SIDE; x++) {
      uint8_t color_index =
          studio->pixels[(size_t)y * SPRITE_SIDE + (size_t)x];
      uint32_t argb = color_index == 0
                          ? 0u
                          : (0xFF000000u | palette[color_index]);
      fprintf(out, "0x%08Xu%s", (unsigned)argb,
              x + 1u == SPRITE_SIDE ? "" : ", ");
    }
    fprintf(out, "%s\n", y + 1u == SPRITE_SIDE ? "" : ",");
  }
  fprintf(out, "};\n\n#endif\n");
  bool write_ok = ferror(out) == 0;
  bool close_ok = fclose(out) == 0;
  bool ok = write_ok && close_ok && rename(temporary, path) == 0;
  if (!ok) {
    remove(temporary);
    set_status(studio, "Export failed. Original file preserved.");
    return false;
  }
  memcpy(studio->saved_pixels, studio->pixels, sizeof(studio->pixels));
  studio->export_count++;
  set_status(studio, "EXPORTED REUSABLE 16 X 16 C SPRITE");
  return true;
}

static void select_zoom(pixel_studio_t *studio, uint8_t index) {
  if (index >= 3) return;
  studio->zoom_index = index;
  char message[48];
  snprintf(message, sizeof(message), "ZOOM %u PX PER SPRITE PIXEL",
           (unsigned)zoom_sizes[index]);
  set_status(studio, message);
}

static void begin_canvas_action(pixel_studio_t *studio, int cell_x, int cell_y) {
  if (studio->tool == TOOL_FILL) {
    if (flood_fill(studio, cell_x, cell_y, studio->selected_color)) {
      history_commit(studio);
      set_status(studio, "FILLED CONNECTED COLOR REGION");
    } else {
      set_status(studio, "FILL COLOR ALREADY MATCHES");
    }
    return;
  }
  if (studio->tool == TOOL_EYEDROPPER) {
    pick_color(studio, cell_x, cell_y);
    return;
  }
  studio->drawing = true;
  studio->stroke_changed = false;
  studio->last_cell_x = cell_x;
  studio->last_cell_y = cell_y;
  draw_cell_line(studio, cell_x, cell_y, cell_x, cell_y);
}

static void finish_canvas_action(pixel_studio_t *studio) {
  if (!studio->drawing) return;
  studio->drawing = false;
  studio->last_cell_x = -1;
  studio->last_cell_y = -1;
  if (studio->stroke_changed && history_commit(studio)) {
    set_status(studio, studio->tool == TOOL_ERASER
                           ? "ERASER STROKE COMMITTED"
                           : "PENCIL STROKE COMMITTED");
  }
  studio->stroke_changed = false;
}

static void flip_sprite(pixel_studio_t *studio, bool vertical) {
  finish_canvas_action(studio);
  for (int y = 0; y < SPRITE_SIDE; y++) for (int x = 0; x < SPRITE_SIDE; x++) {
    int ox = vertical ? x : SPRITE_SIDE - 1 - x;
    int oy = vertical ? SPRITE_SIDE - 1 - y : y;
    size_t i = (size_t)y * SPRITE_SIDE + x, other = (size_t)oy * SPRITE_SIDE + ox;
    if (i < other) { uint8_t color = studio->pixels[i]; studio->pixels[i] = studio->pixels[other]; studio->pixels[other] = color; }
  }
  history_commit(studio);
  set_status(studio, vertical ? "Flipped vertically. Undo is available." : "Flipped horizontally. Undo is available.");
}

static void handle_left_press(pixel_studio_t *studio, const char *export_path) {
  studio->keyboard_cursor = false;
  if (point_in_rect(studio->mouse_x, studio->mouse_y, 28, 450, 132, 32)) { flip_sprite(studio, false); return; }
  if (point_in_rect(studio->mouse_x, studio->mouse_y, 28, 490, 132, 32)) { flip_sprite(studio, true); return; }
  for (uint8_t tool = 0; tool < TOOL_COUNT; tool++) {
    int y = 128 + tool * 58;
    if (point_in_rect(studio->mouse_x, studio->mouse_y, 28, y, 132, 46)) {
      studio->tool = (pixel_tool_t)tool;
      char message[48];
      snprintf(message, sizeof(message), "%s TOOL SELECTED", tool_names[tool]);
      set_status(studio, message);
      return;
    }
  }
  for (uint8_t color = 0; color < PALETTE_SIZE; color++) {
    int x = 640 + (color % 4u) * 31;
    int y = 184 + (color / 4u) * 31;
    if (point_in_rect(studio->mouse_x, studio->mouse_y, x, y, 25, 25)) {
      studio->selected_color = color;
      studio->tool = TOOL_PENCIL;
      char message[48];
      snprintf(message, sizeof(message), "COLOR %u SELECTED", (unsigned)color);
      set_status(studio, message);
      return;
    }
  }
  for (uint8_t zoom = 0; zoom < 3; zoom++) {
    int x = 640 + zoom * 42;
    if (point_in_rect(studio->mouse_x, studio->mouse_y, x, 374, 36, 32)) {
      select_zoom(studio, zoom);
      return;
    }
  }
  if (point_in_rect(studio->mouse_x, studio->mouse_y, 28, 394, 62, 36)) {
    history_undo(studio);
    return;
  }
  if (point_in_rect(studio->mouse_x, studio->mouse_y, 98, 394, 62, 36)) {
    history_redo(studio);
    return;
  }
  if (point_in_rect(studio->mouse_x, studio->mouse_y, 640, 442, 124, 38)) {
    export_sprite(studio, export_path);
    return;
  }
  if (point_in_rect(studio->mouse_x, studio->mouse_y, 640, 490, 124, 32)) {
    studio->grid_visible = !studio->grid_visible;
    set_status(studio, studio->grid_visible ? "PIXEL GRID VISIBLE"
                                            : "PIXEL GRID HIDDEN");
    return;
  }
  int cell_x = 0;
  int cell_y = 0;
  if (screen_to_cell(studio, studio->mouse_x, studio->mouse_y,
                     &cell_x, &cell_y)) {
    begin_canvas_action(studio, cell_x, cell_y);
  }
}

static void handle_mouse_packet(pixel_studio_t *studio, int dx, int dy,
                                uint8_t buttons, const char *export_path) {
  studio->mouse_x = clamp_int(studio->mouse_x + dx, 0, SCREEN_WIDTH - 1);
  studio->mouse_y = clamp_int(studio->mouse_y - dy, 0, SCREEN_HEIGHT - 1);
  if (studio->help_visible) { studio->previous_buttons = buttons; return; }
  bool left = (buttons & BIT(0)) != 0;
  bool previous_left = (studio->previous_buttons & BIT(0)) != 0;
  bool right = (buttons & BIT(1)) != 0;
  bool previous_right = (studio->previous_buttons & BIT(1)) != 0;
  if (left && !previous_left) handle_left_press(studio, export_path);
  if (left && studio->drawing) {
    int cell_x = 0;
    int cell_y = 0;
    if (screen_to_cell(studio, studio->mouse_x, studio->mouse_y,
                       &cell_x, &cell_y) &&
        (cell_x != studio->last_cell_x || cell_y != studio->last_cell_y)) {
      draw_cell_line(studio, studio->last_cell_x, studio->last_cell_y,
                     cell_x, cell_y);
      studio->last_cell_x = cell_x;
      studio->last_cell_y = cell_y;
    }
  }
  if (!left && previous_left) finish_canvas_action(studio);
  if (right && !previous_right) {
    int cell_x = 0;
    int cell_y = 0;
    if (screen_to_cell(studio, studio->mouse_x, studio->mouse_y,
                       &cell_x, &cell_y)) {
      pick_color(studio, cell_x, cell_y);
    }
  }
  studio->previous_buttons = buttons;
}

static bool mouse_parser_push(mouse_parser_t *parser, uint8_t byte,
                              int *dx, int *dy, uint8_t *buttons) {
  if (parser->index == 0) {
    if (byte == MOUSE_ACK || byte == MOUSE_NACK || byte == MOUSE_ERR) {
      return false;
    }
    if ((byte & MOUSE_SYNC_BIT) == 0) return false;
  }
  parser->bytes[parser->index++] = byte;
  if (parser->index < 3) return false;
  parser->index = 0;
  uint8_t flags = parser->bytes[0];
  if ((flags & (BIT(6) | BIT(7))) != 0) return false;
  *dx = (flags & BIT(4)) != 0
            ? (int)parser->bytes[1] - 256
            : (int)parser->bytes[1];
  *dy = (flags & BIT(5)) != 0
            ? (int)parser->bytes[2] - 256
            : (int)parser->bytes[2];
  *buttons = (uint8_t)(flags & 0x07u);
  return true;
}

static void clear_sprite(pixel_studio_t *studio) {
  bool already_clear = true;
  for (size_t i = 0; i < SPRITE_PIXELS; i++) {
    if (studio->pixels[i] != 0) {
      already_clear = false;
      break;
    }
  }
  if (already_clear) {
    set_status(studio, "SPRITE IS ALREADY CLEAR");
    return;
  }
  memset(studio->pixels, 0, sizeof(studio->pixels));
  history_commit(studio);
  set_status(studio, "CLEARED SPRITE - UNDO IS AVAILABLE");
}

static void handle_keyboard_byte(pixel_studio_t *studio, uint8_t data,
                                 bool *running, const char *export_path) {
  if (example_modifier_byte(&studio->modifiers, data)) { studio->extended = false; return; }
  if (data == KBD_TWO_BYTE_PFX) {
    studio->extended = true;
    return;
  }
  bool break_code = (data & KBD_BREAK_BIT) != 0;
  uint8_t code = (uint8_t)(data & 0x7Fu);
  if (studio->extended) {
    studio->extended = false;
    if (break_code || studio->help_visible || example_primary(&studio->modifiers)) return;
    finish_canvas_action(studio);
    if (code == 0x4B) studio->key_x = clamp_int(studio->key_x - 1, 0, SPRITE_SIDE - 1);
    else if (code == 0x4D) studio->key_x = clamp_int(studio->key_x + 1, 0, SPRITE_SIDE - 1);
    else if (code == 0x48) studio->key_y = clamp_int(studio->key_y - 1, 0, SPRITE_SIDE - 1);
    else if (code == 0x50) studio->key_y = clamp_int(studio->key_y + 1, 0, SPRITE_SIDE - 1);
    else if (code == 0x53) { set_pixel(studio, studio->key_x, studio->key_y, 0); history_commit(studio); }
    else return;
    studio->keyboard_cursor = true;
    return;
  }
  if (code == 0x01 && break_code) {
    if (studio->help_visible) { studio->help_visible = false; return; }
    *running = false;
    return;
  }
  if (break_code) return;
  if (studio->help_visible && code != 0x3B && code != 0x23) return;
  // A keyboard action must never mutate history underneath an uncommitted
  // mouse stroke. Treat the pixels drawn so far as one complete gesture first.
  finish_canvas_action(studio);
  if (example_primary(&studio->modifiers)) {
    if (code == 0x11) { *running = false; return; }
    if (code == 0x1F) { export_sprite(studio, export_path); }
    else if (code == 0x2C && !studio->modifiers.shift) { history_undo(studio); }
    else if (code == 0x15 || (code == 0x2C && studio->modifiers.shift)) { history_redo(studio); }
    return;
  }
  switch (code) {
  case 0x1C:
  case 0x39:
    studio->keyboard_cursor = true;
    begin_canvas_action(studio, studio->key_x, studio->key_y);
    finish_canvas_action(studio);
    break;
  case 0x2D: flip_sprite(studio, false); break; /* X */
  case 0x2F: flip_sprite(studio, true); break; /* V */
  case 0x1A: studio->selected_color = (uint8_t)((studio->selected_color + PALETTE_SIZE - 1) % PALETTE_SIZE); break;
  case 0x1B: studio->selected_color = (uint8_t)((studio->selected_color + 1) % PALETTE_SIZE); break;
  case 0x30: /* B: brush */
  case 0x19:
    studio->tool = TOOL_PENCIL;
    set_status(studio, "PENCIL TOOL SELECTED");
    break;
  case 0x12:
    studio->tool = TOOL_ERASER;
    set_status(studio, "ERASER TOOL SELECTED");
    break;
  case 0x21:
    studio->tool = TOOL_FILL;
    set_status(studio, "FILL TOOL SELECTED");
    break;
  case 0x17:
    studio->tool = TOOL_EYEDROPPER;
    set_status(studio, "EYEDROPPER TOOL SELECTED");
    break;
  case 0x02: select_zoom(studio, 0); break;
  case 0x03: select_zoom(studio, 1); break;
  case 0x04: select_zoom(studio, 2); break;
  case 0x0C:
    if (studio->zoom_index > 0) select_zoom(studio, (uint8_t)(studio->zoom_index - 1u));
    break;
  case 0x0D:
    if (studio->zoom_index < 2) select_zoom(studio, (uint8_t)(studio->zoom_index + 1u));
    break;
  case 0x22:
    studio->grid_visible = !studio->grid_visible;
    set_status(studio, studio->grid_visible ? "PIXEL GRID VISIBLE"
                                            : "PIXEL GRID HIDDEN");
    break;
  case 0x2E: clear_sprite(studio); break;
  case 0x1F:
  case 0x3C:
    export_sprite(studio, export_path);
    break;
  case 0x3B:
  case 0x23:
    studio->help_visible = !studio->help_visible;
    set_status(studio, studio->help_visible ? "SHORTCUT HELP OPEN"
                                            : "SHORTCUT HELP CLOSED");
    break;
  default: break;
  }
}

static void draw_button(example_canvas_t *canvas, int x, int y, int w, int h,
                        const char *label, const char *key, bool selected,
                        bool hovered) {
  example_button(canvas, x, y, w, h, "", selected, hovered, true);
  example_label(canvas, x + 10, y + 13, label, selected ? UI_PAPER : UI_INK, 1);
  example_label(canvas, x + w - 22, y + 13, key, selected ? UI_PAPER : UI_MUTED, 1);
}


static void draw_header(example_canvas_t *canvas, const pixel_studio_t *studio) {
  example_heading(canvas, "Pixel Studio", "A tiny canvas. Every pixel counts.", UI_ACCENT, false);
  example_label(canvas, 578, 18, studio_is_dirty(studio) ? "Unsaved changes" : studio->export_count ? "Sprite exported" : "Sample sprite", UI_INK, 1);
  example_label(canvas, 578, 42, "Cmd/Ctrl+S  Export", UI_MUTED, 1);
}


static void draw_tool_panel(example_canvas_t *canvas, const pixel_studio_t *studio) {
  example_label(canvas, 28, 100, "Tools", UI_MUTED, 1);
  for (uint8_t tool = 0; tool < TOOL_COUNT; tool++) {
    int y = 128 + tool * 58;
    draw_button(canvas, 28, y, 132, 46, tool_names[tool], tool_keys[tool],
                studio->tool == (pixel_tool_t)tool,
                point_in_rect(studio->mouse_x, studio->mouse_y, 28, y, 132, 46));
  }
  example_label(canvas, 28, 366, "History", UI_MUTED, 1);
  example_button(canvas, 28, 394, 62, 36, "Undo", false,
      point_in_rect(studio->mouse_x, studio->mouse_y, 28, 394, 62, 36), studio->history_cursor > 0);
  example_button(canvas, 98, 394, 62, 36, "Redo", false,
      point_in_rect(studio->mouse_x, studio->mouse_y, 98, 394, 62, 36), studio->history_cursor + 1 < studio->history_length);
  example_button(canvas, 28, 450, 132, 32, "Flip horiz.  X", false, point_in_rect(studio->mouse_x,studio->mouse_y,28,450,132,32), true);
  example_button(canvas, 28, 490, 132, 32, "Flip vert.  V", false, point_in_rect(studio->mouse_x,studio->mouse_y,28,490,132,32), true);
}


static void draw_sprite_canvas(example_canvas_t *canvas,
                               const pixel_studio_t *studio) {
  example_fill_rect(canvas, 182, 96, 440, 432, 0xDFE5E9);
  example_stroke_rect(canvas, 182, 96, 440, 432, 1, UI_LINE);
  int origin_x = 0;
  int origin_y = 0;
  int cell = 0;
  canvas_geometry(studio, &origin_x, &origin_y, &cell);
  example_fill_rect(canvas, origin_x - 5, origin_y - 5,
                    cell * SPRITE_SIDE + 10, cell * SPRITE_SIDE + 10,
                    example_rgb(11, 15, 23));
  for (int y = 0; y < SPRITE_SIDE; y++) {
    for (int x = 0; x < SPRITE_SIDE; x++) {
      uint8_t color = studio->pixels[y * SPRITE_SIDE + x];
      uint32_t fill = color == 0
                          ? ((x + y) % 2 == 0 ? example_rgb(57, 64, 79)
                                             : example_rgb(72, 80, 96))
                          : palette[color];
      int gap = studio->grid_visible ? 1 : 0;
      example_fill_rect(canvas, origin_x + x * cell, origin_y + y * cell,
                        cell - gap, cell - gap, fill);
    }
  }
  example_stroke_rect(canvas, origin_x - 2, origin_y - 2,
                      cell * SPRITE_SIDE + 4, cell * SPRITE_SIDE + 4,
                      2, example_rgb(111, 128, 153));
  int hover_x = 0;
  int hover_y = 0;
  bool hover = screen_to_cell(studio, studio->mouse_x, studio->mouse_y, &hover_x, &hover_y);
  if (studio->keyboard_cursor) { hover_x = studio->key_x; hover_y = studio->key_y; hover = true; }
  if (hover) {
    example_stroke_rect(canvas, origin_x + hover_x * cell - 1,
                        origin_y + hover_y * cell - 1,
                        cell + 1, cell + 1, 2,
                        example_rgb(255, 243, 214));
  }
}

static void draw_preview(example_canvas_t *canvas, const pixel_studio_t *studio) {
  example_label(canvas, 640, 100, "Preview  1x / 2x", UI_MUTED, 1);
  example_round_rect(canvas, 640, 124, 124, 40, 5, UI_PAPER);
  for (int py = 0; py < 40; py++) for (int px = 0; px < 124; px++) {
    if (((px / 6) + (py / 6)) % 2) example_put_pixel(canvas,640+px,124+py,0xE2E8EB);
  }
  for (int y = 0; y < SPRITE_SIDE; y++) for (int x = 0; x < SPRITE_SIDE; x++) {
    uint8_t color = studio->pixels[y * SPRITE_SIDE + x];
    if (color) {
      example_put_pixel(canvas, 656 + x, 136 + y, palette[color]);
      /* The 2x preview fits beside the native-size sprite. */
      example_fill_rect(canvas,704+x*2,128+y*2,2,2,palette[color]);
    }
  }
}

static void draw_properties_panel(example_canvas_t *canvas, const pixel_studio_t *studio) {
  draw_preview(canvas, studio);
  example_label(canvas, 640, 164, "Palette", UI_MUTED, 1);
  for (uint8_t color = 0; color < PALETTE_SIZE; color++) {
    int x = 640 + (color % 4u) * 31, y = 184 + (color / 4u) * 31;
    example_round_rect(canvas, x, y, 25, 25, 3, color ? palette[color] : UI_PAPER);
    if (!color) for (int i = 0; i < 21; i++) example_put_pixel(canvas, x + 2 + i, y + 22 - i, 0xB34052);
    if (studio->selected_color == color) example_stroke_rect(canvas, x - 3, y - 3, 31, 31, 2, UI_INK);
  }
  char color_label[32];
  snprintf(color_label, sizeof(color_label), studio->selected_color ? "#%06X" : "Transparent", (unsigned)palette[studio->selected_color]);
  example_label(canvas, 640, 316, color_label, UI_INK, 1);
  example_label(canvas, 640, 344, "Zoom", UI_MUTED, 1);
  for (uint8_t zoom = 0; zoom < 3; zoom++) {
    int x = 640 + zoom * 42;
    char label[4]; snprintf(label, sizeof(label), "%u", zoom + 1u);
    example_button(canvas, x, 374, 36, 32, label, studio->zoom_index == zoom,
      point_in_rect(studio->mouse_x, studio->mouse_y, x, 374, 36, 32), true);
  }
  example_button(canvas, 640, 442, 124, 38, "Export  S", true,
    point_in_rect(studio->mouse_x, studio->mouse_y, 640, 442, 124, 38), true);
  example_button(canvas, 640, 490, 124, 32, studio->grid_visible ? "Grid on  G" : "Grid off  G", false,
    point_in_rect(studio->mouse_x, studio->mouse_y, 640, 490, 124, 32), true);
}


static void draw_footer(example_canvas_t *canvas, const pixel_studio_t *studio) {
  example_fill_rect(canvas, 0, 542, 800, 58, UI_PAPER);
  example_label(canvas, 24, 548, studio->status, UI_INK, 1);
  char details[100]; int cx, cy;
  bool cell = screen_to_cell(studio,studio->mouse_x,studio->mouse_y,&cx,&cy);
  if (studio->keyboard_cursor) { cx=studio->key_x; cy=studio->key_y; cell=true; }
  if (cell) snprintf(details,sizeof(details), "Pixel %d,%d   %s   16 x 16   %u opaque pixels",cx+1,cy+1,tool_names[studio->tool],painted_pixel_count(studio));
  else snprintf(details,sizeof(details), "16 x 16   %u opaque pixels   Right-click to pick a color",painted_pixel_count(studio));
  example_label(canvas, 24, 575, details, UI_MUTED, 1);
}


static void draw_help_overlay(example_canvas_t *canvas) {
  example_round_rect(canvas, 112, 90, 576, 420, 10, UI_DARK);
  example_label(canvas, 142, 112, "Pixel Studio shortcuts", UI_LIGHT, 2);
  const char *lines[] = {"B / P   Pencil     E   Eraser     F   Fill     I   Pick", "Drag   Paint       Right-click   Pick a canvas color", "Arrows   Move pixel cursor     Enter / Space   Apply tool", "Delete   Erase cursor pixel      [ / ]   Previous / next color", "Cmd/Ctrl+Z   Undo", "Cmd/Ctrl+Shift+Z or Ctrl+Y   Redo", "1 / 2 / 3 or - / +   Zoom       G   Grid", "X   Flip horizontally       V   Flip vertically", "S / F2 / Cmd/Ctrl+S   Export C header", "C   Clear canvas       F1 / Esc   Close help"};
  for (int i = 0; i < 10; i++) example_label(canvas, 142, 174 + i * 31, lines[i], UI_DIM, 1);
}

static void draw_mouse_cursor(example_canvas_t *canvas,
                              const pixel_studio_t *studio) {
  int x = studio->mouse_x;
  int y = studio->mouse_y;
  for (int row = 0; row < 14; row++) {
    int width = row < 9 ? row / 2 + 2 : 3;
    example_fill_rect(canvas, x + 2, y + row + 2, width, 1,
                      example_rgb(8, 12, 18));
    example_fill_rect(canvas, x, y + row, width, 1,
                      example_rgb(246, 248, 251));
  }
  example_fill_rect(canvas, x + 5, y + 10, 3, 7, example_rgb(8, 12, 18));
  example_fill_rect(canvas, x + 4, y + 9, 3, 7, example_rgb(246, 248, 251));
}

static void draw_pixel_studio(example_canvas_t *canvas,
                              const pixel_studio_t *studio) {
  example_fill_rect(canvas, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                    UI_BG);
  draw_header(canvas, studio);
  draw_tool_panel(canvas, studio);
  draw_sprite_canvas(canvas, studio);
  draw_properties_panel(canvas, studio);
  draw_footer(canvas, studio);
  if (studio->help_visible) draw_help_overlay(canvas);
  draw_mouse_cursor(canvas, studio);
}

int main(int argc, char **argv) {
  const char *export_path =
      argc > 1 ? argv[1] : "build/pixel_studio_sprite.h";
  int result = 1;
  bool initialized = false;
  bool framebuffer_mapped = false;
  bool timer_subscribed = false;
  bool keyboard_subscribed = false;
  bool mouse_subscribed = false;
  bool mouse_reporting = false;
  uint8_t *framebuffer = NULL;
  lcom_vbe_mode_info_t info = {0};
  lcom_irq_t timer_irq = {0};
  lcom_irq_t keyboard_irq = {0};
  lcom_irq_t mouse_irq = {0};
  mouse_parser_t mouse_parser = {{0, 0, 0}, 0};
  pixel_studio_t studio;
  seed_sprite(&studio);

  if (lcom_init() != LCOM_OK) goto cleanup;
  initialized = true;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &info) != LCOM_OK ||
      lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK) {
    goto cleanup;
  }
  if (lcom_phys_map(info.framebuffer_phys, info.framebuffer_size,
                    (void **)&framebuffer) != LCOM_OK) {
    goto cleanup;
  }
  framebuffer_mapped = true;
  if (configure_pit_60hz() != LCOM_OK) goto cleanup;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) goto cleanup;
  timer_subscribed = true;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &keyboard_irq) != LCOM_OK) goto cleanup;
  keyboard_subscribed = true;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &mouse_irq) != LCOM_OK) goto cleanup;
  mouse_subscribed = true;
  if (set_mouse_reporting(true) != LCOM_OK) goto cleanup;
  mouse_reporting = true;
  read_wall_clock(&studio.clock);

  example_canvas_t canvas = {framebuffer, info};
  bool running = true;
  draw_pixel_studio(&canvas, &studio);
  lcom_vbe_present();
  lcom_printf("Pixel Studio: mouse draws, B/P/E/F/I select tools, "
              "Cmd/Ctrl-Z undo, Shift-Z redo, S exports, ESC exits.\n");
  bool event_failed = false;
  uint64_t last_clock_tick = 0;
  while (running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      event_failed = true;
      set_status(&studio, "RUNTIME EVENT WAIT FAILED");
      break;
    }
    bool redraw = false;
    // Delivered interrupts can coalesce. Poll the RTC using elapsed bus ticks.
    if (event.tick - last_clock_tick >= 60u) {
      read_wall_clock(&studio.clock);
      last_clock_tick = event.tick;
    }
    if ((event.irq_mask & timer_irq.mask) != 0) {
      studio.frame++;
      redraw = true;
    }
    if ((event.irq_mask & keyboard_irq.mask) != 0) {
      uint8_t status = 0;
      uint8_t data = 0;
      if (lcom_port_read8(KBC_ST_REG, &status) == LCOM_OK &&
          (status & KBC_ST_OBF) != 0 && (status & KBC_ST_AUX) == 0 &&
          (status & KBC_ST_ERR) == 0 &&
          lcom_port_read8(KBC_OUT_BUF, &data) == LCOM_OK) {
        handle_keyboard_byte(&studio, data, &running, export_path);
        redraw = true;
      }
    }
    if ((event.irq_mask & mouse_irq.mask) != 0) {
      uint8_t status = 0;
      uint8_t data = 0;
      if (lcom_port_read8(KBC_ST_REG, &status) == LCOM_OK &&
          (status & KBC_ST_OBF) != 0 && (status & KBC_ST_AUX) != 0 &&
          (status & KBC_ST_ERR) == 0 &&
          lcom_port_read8(KBC_OUT_BUF, &data) == LCOM_OK) {
        int dx = 0;
        int dy = 0;
        uint8_t buttons = 0;
        if (mouse_parser_push(&mouse_parser, data, &dx, &dy, &buttons)) {
          handle_mouse_packet(&studio, dx, dy, buttons, export_path);
          redraw = true;
        }
      }
    }
    if (redraw) {
      draw_pixel_studio(&canvas, &studio);
      lcom_vbe_present();
    }
  }
  finish_canvas_action(&studio);
  read_wall_clock(&studio.clock);
  draw_pixel_studio(&canvas, &studio);
  lcom_vbe_present();
  char clock[24];
  format_clock(&studio.clock, clock, sizeof(clock));
  lcom_printf("pixel studio summary pixels=%u edits=%u history=%u/%u "
              "exports=%u checksum=%08x rtc=%s\n",
              painted_pixel_count(&studio), (unsigned)studio.committed_edits,
              (unsigned)(studio.history_cursor + 1u),
              (unsigned)studio.history_length, (unsigned)studio.export_count,
              (unsigned)sprite_checksum(&studio), clock);
  if (!event_failed) result = 0;

cleanup:
  if (mouse_reporting) set_mouse_reporting(false);
  if (mouse_subscribed) lcom_irq_unsubscribe(&mouse_irq);
  if (keyboard_subscribed) lcom_irq_unsubscribe(&keyboard_irq);
  if (timer_subscribed) lcom_irq_unsubscribe(&timer_irq);
  if (framebuffer_mapped) lcom_phys_unmap(framebuffer, info.framebuffer_size);
  if (initialized) lcom_exit();
  return result;
}
