#include "example_ui.h"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/rtc.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 10
#define MAP_HEIGHT 8
#define MAP_CELLS (MAP_WIDTH * MAP_HEIGHT)
#define MAP_CELL_SIZE 40
#define MAP_ORIGIN_X 198
#define MAP_ORIGIN_Y 128
#define EDIT_HISTORY_CAPACITY 32
#define PLAY_HISTORY_CAPACITY 64

typedef enum {
  TILE_FLOOR,
  TILE_WALL,
  TILE_GOAL
} base_tile_t;

typedef enum {
  OBJECT_NONE,
  OBJECT_CRATE,
  OBJECT_PLAYER
} map_object_t;

typedef enum {
  BRUSH_FLOOR,
  BRUSH_WALL,
  BRUSH_GOAL,
  BRUSH_CRATE,
  BRUSH_PLAYER,
  BRUSH_COUNT
} map_brush_t;

typedef enum {
  MODE_EDITOR,
  MODE_PLAY
} editor_mode_t;

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
  uint8_t base[MAP_CELLS];
  uint8_t objects[MAP_CELLS];
} map_snapshot_t;

typedef struct {
  uint8_t objects[MAP_CELLS];
  uint32_t moves;
  uint32_t pushes;
} play_snapshot_t;

typedef struct {
  bool valid;
  unsigned players;
  unsigned crates;
  unsigned goals;
  const char *message;
} map_validation_t;

typedef struct {
  uint8_t base[MAP_CELLS];
  uint8_t objects[MAP_CELLS];
  uint8_t saved_base[MAP_CELLS];
  uint8_t saved_objects[MAP_CELLS];
  map_snapshot_t edit_history[EDIT_HISTORY_CAPACITY];
  uint8_t edit_history_length;
  uint8_t edit_history_cursor;
  uint8_t play_objects[MAP_CELLS];
  uint8_t play_initial_objects[MAP_CELLS];
  play_snapshot_t play_history[PLAY_HISTORY_CAPACITY];
  uint8_t play_history_length;
  map_brush_t brush;
  editor_mode_t mode;
  int key_x, key_y;
  bool keyboard_cursor;
  bool save_blocked, loaded_from_disk;
  int mouse_x;
  int mouse_y;
  int last_cell_x;
  int last_cell_y;
  uint8_t previous_buttons;
  uint32_t frame;
  uint32_t committed_edits;
  uint32_t export_count;
  uint32_t moves;
  uint32_t pushes;
  bool drawing;
  bool stroke_changed;
  bool solved;
  bool grid_visible;
  bool help_visible;
  example_modifiers_t modifiers;
  bool extended;
  wall_clock_t clock;
  char status[80];
} tilemap_editor_t;

static const char *brush_names[BRUSH_COUNT] = {
    "Floor", "Wall", "Goal", "Crate", "Player",
};

static const char *brush_keys[BRUSH_COUNT] = {
    "1", "2", "3", "4", "5",
};

static bool point_in_rect(int px, int py, int x, int y, int w, int h) {
  return px >= x && py >= y && px < x + w && py < y + h;
}

static int clamp_int(int value, int low, int high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

static size_t map_index(int x, int y) {
  return (size_t)y * MAP_WIDTH + (size_t)x;
}

static void set_status(tilemap_editor_t *editor, const char *text) {
  snprintf(editor->status, sizeof(editor->status), "%s", text);
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
    snprintf(out, size, "--:--:--");
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

static void seed_map(tilemap_editor_t *editor) {
  memset(editor, 0, sizeof(*editor));
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      size_t index = map_index(x, y);
      bool boundary = x == 0 || y == 0 ||
                      x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1;
      editor->base[index] = boundary ? TILE_WALL : TILE_FLOOR;
      editor->objects[index] = OBJECT_NONE;
    }
  }
  editor->base[map_index(3, 2)] = TILE_WALL;
  editor->base[map_index(4, 2)] = TILE_WALL;
  editor->base[map_index(6, 5)] = TILE_GOAL;
  editor->objects[map_index(4, 3)] = OBJECT_PLAYER;
  editor->objects[map_index(4, 4)] = OBJECT_CRATE;
  memcpy(editor->saved_base, editor->base, sizeof(editor->base));
  memcpy(editor->saved_objects, editor->objects, sizeof(editor->objects));
  memcpy(editor->edit_history[0].base, editor->base, sizeof(editor->base));
  memcpy(editor->edit_history[0].objects, editor->objects,
         sizeof(editor->objects));
  editor->edit_history_length = 1;
  editor->brush = BRUSH_WALL;
  editor->mode = MODE_EDITOR;
  editor->mouse_x = SCREEN_WIDTH / 2;
  editor->mouse_y = SCREEN_HEIGHT / 2;
  editor->last_cell_x = -1;
  editor->last_cell_y = -1;
  editor->grid_visible = true;
  set_status(editor, "READY - PAINT A TILE OR PRESS ENTER TO PLAY");
}

static bool editor_is_dirty(const tilemap_editor_t *editor) {
  return memcmp(editor->base, editor->saved_base, sizeof(editor->base)) != 0 ||
         memcmp(editor->objects, editor->saved_objects,
                sizeof(editor->objects)) != 0;
}

static uint32_t map_checksum(const uint8_t *base, const uint8_t *objects) {
  uint32_t hash = 2166136261u;
  for (size_t i = 0; i < MAP_CELLS; i++) {
    hash ^= base[i];
    hash *= 16777619u;
    hash ^= objects[i];
    hash *= 16777619u;
  }
  return hash;
}

static bool edit_history_commit(tilemap_editor_t *editor) {
  map_snapshot_t *current =
      &editor->edit_history[editor->edit_history_cursor];
  if (memcmp(current->base, editor->base, sizeof(editor->base)) == 0 &&
      memcmp(current->objects, editor->objects,
             sizeof(editor->objects)) == 0) {
    return false;
  }
  editor->edit_history_length =
      (uint8_t)(editor->edit_history_cursor + 1u);
  if (editor->edit_history_length == EDIT_HISTORY_CAPACITY) {
    memmove(&editor->edit_history[0], &editor->edit_history[1],
            (EDIT_HISTORY_CAPACITY - 1u) *
                sizeof(editor->edit_history[0]));
    editor->edit_history_length--;
    editor->edit_history_cursor--;
  }
  map_snapshot_t *next =
      &editor->edit_history[editor->edit_history_length];
  memcpy(next->base, editor->base, sizeof(editor->base));
  memcpy(next->objects, editor->objects, sizeof(editor->objects));
  editor->edit_history_cursor = editor->edit_history_length;
  editor->edit_history_length++;
  editor->committed_edits++;
  return true;
}

static void edit_history_undo(tilemap_editor_t *editor) {
  if (editor->edit_history_cursor == 0) {
    set_status(editor, "NOTHING TO UNDO");
    return;
  }
  editor->edit_history_cursor--;
  const map_snapshot_t *snapshot =
      &editor->edit_history[editor->edit_history_cursor];
  memcpy(editor->base, snapshot->base, sizeof(editor->base));
  memcpy(editor->objects, snapshot->objects, sizeof(editor->objects));
  set_status(editor, "UNDO RESTORED PREVIOUS MAP");
}

static void edit_history_redo(tilemap_editor_t *editor) {
  if ((uint8_t)(editor->edit_history_cursor + 1u) >=
      editor->edit_history_length) {
    set_status(editor, "NOTHING TO REDO");
    return;
  }
  editor->edit_history_cursor++;
  const map_snapshot_t *snapshot =
      &editor->edit_history[editor->edit_history_cursor];
  memcpy(editor->base, snapshot->base, sizeof(editor->base));
  memcpy(editor->objects, snapshot->objects, sizeof(editor->objects));
  set_status(editor, "REDO RESTORED NEXT MAP");
}

static bool map_boundary_closed(const uint8_t *base) {
  for (int x = 0; x < MAP_WIDTH; x++) {
    if (base[map_index(x, 0)] != TILE_WALL ||
        base[map_index(x, MAP_HEIGHT - 1)] != TILE_WALL) {
      return false;
    }
  }
  for (int y = 0; y < MAP_HEIGHT; y++) {
    if (base[map_index(0, y)] != TILE_WALL ||
        base[map_index(MAP_WIDTH - 1, y)] != TILE_WALL) {
      return false;
    }
  }
  return true;
}

static bool crate_in_static_corner(const uint8_t *base,
                                   const uint8_t *objects,
                                   int x, int y) {
  size_t index = map_index(x, y);
  if (objects[index] != OBJECT_CRATE || base[index] == TILE_GOAL) return false;
  bool left_wall = x == 0 || base[map_index(x - 1, y)] == TILE_WALL;
  bool right_wall =
      x == MAP_WIDTH - 1 || base[map_index(x + 1, y)] == TILE_WALL;
  bool up_wall = y == 0 || base[map_index(x, y - 1)] == TILE_WALL;
  bool down_wall =
      y == MAP_HEIGHT - 1 || base[map_index(x, y + 1)] == TILE_WALL;
  return (left_wall || right_wall) && (up_wall || down_wall);
}

static bool goals_reachable_from_player(const uint8_t *base,
                                        const uint8_t *objects) {
  bool visited[MAP_CELLS] = {false};
  uint8_t queue[MAP_CELLS];
  size_t head = 0;
  size_t tail = 0;
  for (size_t i = 0; i < MAP_CELLS; i++) {
    if (objects[i] == OBJECT_PLAYER) {
      queue[tail++] = (uint8_t)i;
      visited[i] = true;
      break;
    }
  }
  if (tail == 0) return false;
  while (head < tail) {
    size_t index = queue[head++];
    int x = (int)(index % MAP_WIDTH);
    int y = (int)(index / MAP_WIDTH);
    const int next_x[4] = {x - 1, x + 1, x, x};
    const int next_y[4] = {y, y, y - 1, y + 1};
    for (uint8_t direction = 0; direction < 4; direction++) {
      int nx = next_x[direction];
      int ny = next_y[direction];
      if (nx < 0 || ny < 0 || nx >= MAP_WIDTH || ny >= MAP_HEIGHT) continue;
      size_t next = map_index(nx, ny);
      if (!visited[next] && base[next] != TILE_WALL) {
        visited[next] = true;
        queue[tail++] = (uint8_t)next;
      }
    }
  }
  for (size_t i = 0; i < MAP_CELLS; i++) {
    if (base[i] == TILE_GOAL && !visited[i]) return false;
  }
  return true;
}

static map_validation_t validate_map(const uint8_t *base,
                                     const uint8_t *objects) {
  map_validation_t validation = {false, 0, 0, 0, "CHECKING"};
  bool object_on_wall = false;
  bool deadlock = false;
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      size_t index = map_index(x, y);
      if (objects[index] == OBJECT_PLAYER) validation.players++;
      if (objects[index] == OBJECT_CRATE) validation.crates++;
      if (base[index] == TILE_GOAL) validation.goals++;
      if (objects[index] != OBJECT_NONE && base[index] == TILE_WALL) {
        object_on_wall = true;
      }
      if (crate_in_static_corner(base, objects, x, y)) deadlock = true;
    }
  }
  if (!map_boundary_closed(base)) {
    validation.message = "OUTER WALL MUST BE CLOSED";
  } else if (object_on_wall) {
    validation.message = "OBJECT CANNOT OCCUPY A WALL";
  } else if (validation.players != 1) {
    validation.message = "MAP NEEDS EXACTLY ONE PLAYER";
  } else if (validation.crates == 0) {
    validation.message = "ADD AT LEAST ONE CRATE";
  } else if (validation.crates != validation.goals) {
    validation.message = "CRATE AND GOAL COUNTS MUST MATCH";
  } else if (deadlock) {
    validation.message = "CRATE IS STUCK IN A NON-GOAL CORNER";
  } else if (!goals_reachable_from_player(base, objects)) {
    validation.message = "A GOAL IS ISOLATED BY WALLS";
  } else {
    validation.valid = true;
    validation.message = "VALID AND READY TO PLAY";
  }
  return validation;
}

static bool screen_to_cell(int screen_x, int screen_y,
                           int *cell_x, int *cell_y) {
  if (!point_in_rect(screen_x, screen_y, MAP_ORIGIN_X, MAP_ORIGIN_Y,
                     MAP_WIDTH * MAP_CELL_SIZE,
                     MAP_HEIGHT * MAP_CELL_SIZE)) {
    return false;
  }
  *cell_x = (screen_x - MAP_ORIGIN_X) / MAP_CELL_SIZE;
  *cell_y = (screen_y - MAP_ORIGIN_Y) / MAP_CELL_SIZE;
  return true;
}

static bool set_brush_at(tilemap_editor_t *editor, int x, int y) {
  size_t index = map_index(x, y);
  uint8_t old_base = editor->base[index];
  uint8_t old_object = editor->objects[index];
  if (editor->brush == BRUSH_PLAYER) {
    bool target_is_only_player = old_object == OBJECT_PLAYER;
    unsigned players = 0;
    for (size_t i = 0; i < MAP_CELLS; i++) {
      if (editor->objects[i] == OBJECT_PLAYER) players++;
    }
    if (target_is_only_player && players == 1) return false;
    for (size_t i = 0; i < MAP_CELLS; i++) {
      if (editor->objects[i] == OBJECT_PLAYER) {
        editor->objects[i] = OBJECT_NONE;
      }
    }
    if (editor->base[index] == TILE_WALL) editor->base[index] = TILE_FLOOR;
    editor->objects[index] = OBJECT_PLAYER;
    return true;
  }
  switch (editor->brush) {
  case BRUSH_FLOOR:
    editor->base[index] = TILE_FLOOR;
    editor->objects[index] = OBJECT_NONE;
    break;
  case BRUSH_WALL:
    editor->base[index] = TILE_WALL;
    editor->objects[index] = OBJECT_NONE;
    break;
  case BRUSH_GOAL:
    editor->base[index] = TILE_GOAL;
    editor->objects[index] = OBJECT_NONE;
    break;
  case BRUSH_CRATE:
    if (editor->base[index] == TILE_WALL) editor->base[index] = TILE_FLOOR;
    editor->objects[index] = OBJECT_CRATE;
    break;
  case BRUSH_PLAYER:
  case BRUSH_COUNT: break;
  }
  return old_base != editor->base[index] ||
         old_object != editor->objects[index];
}

static void draw_brush_line(tilemap_editor_t *editor, int x0, int y0,
                            int x1, int y1) {
  int dx = x1 > x0 ? x1 - x0 : x0 - x1;
  int sx = x0 < x1 ? 1 : -1;
  int dy_abs = y1 > y0 ? y1 - y0 : y0 - y1;
  int dy = -dy_abs;
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;
  for (;;) {
    if (set_brush_at(editor, x0, y0)) editor->stroke_changed = true;
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

static void begin_edit_gesture(tilemap_editor_t *editor, int cell_x,
                               int cell_y) {
  editor->stroke_changed = false;
  if (editor->brush == BRUSH_CRATE || editor->brush == BRUSH_PLAYER) {
    if (set_brush_at(editor, cell_x, cell_y)) {
      edit_history_commit(editor);
      set_status(editor, editor->brush == BRUSH_CRATE
                             ? "CRATE PLACED"
                             : "PLAYER START MOVED");
    }
    return;
  }
  editor->drawing = true;
  editor->last_cell_x = cell_x;
  editor->last_cell_y = cell_y;
  draw_brush_line(editor, cell_x, cell_y, cell_x, cell_y);
}

static void finish_edit_gesture(tilemap_editor_t *editor) {
  if (!editor->drawing) return;
  editor->drawing = false;
  editor->last_cell_x = -1;
  editor->last_cell_y = -1;
  if (editor->stroke_changed && edit_history_commit(editor)) {
    set_status(editor, "PAINT STROKE COMMITTED");
  }
  editor->stroke_changed = false;
}

static void pick_brush(tilemap_editor_t *editor, int cell_x, int cell_y) {
  size_t index = map_index(cell_x, cell_y);
  if (editor->objects[index] == OBJECT_PLAYER) editor->brush = BRUSH_PLAYER;
  else if (editor->objects[index] == OBJECT_CRATE) editor->brush = BRUSH_CRATE;
  else if (editor->base[index] == TILE_WALL) editor->brush = BRUSH_WALL;
  else if (editor->base[index] == TILE_GOAL) editor->brush = BRUSH_GOAL;
  else editor->brush = BRUSH_FLOOR;
  char message[56];
  snprintf(message, sizeof(message), "PICKED %s BRUSH",
           brush_names[editor->brush]);
  set_status(editor, message);
}

static bool all_crates_on_goals(const tilemap_editor_t *editor) {
  bool have_crate = false;
  for (size_t i = 0; i < MAP_CELLS; i++) {
    if (editor->play_objects[i] != OBJECT_CRATE) continue;
    have_crate = true;
    if (editor->base[i] != TILE_GOAL) return false;
  }
  return have_crate;
}

static void enter_play_mode(tilemap_editor_t *editor) {
  finish_edit_gesture(editor);
  map_validation_t validation =
      validate_map(editor->base, editor->objects);
  if (!validation.valid) {
    set_status(editor, validation.message);
    return;
  }
  memcpy(editor->play_objects, editor->objects,
         sizeof(editor->play_objects));
  memcpy(editor->play_initial_objects, editor->objects,
         sizeof(editor->play_initial_objects));
  editor->play_history_length = 0;
  editor->moves = 0;
  editor->pushes = 0;
  editor->solved = all_crates_on_goals(editor);
  editor->mode = MODE_PLAY;
  set_status(editor, "PLAY MODE - MOVE WITH ARROWS OR WASD");
}

static void leave_play_mode(tilemap_editor_t *editor) {
  editor->mode = MODE_EDITOR;
  editor->solved = false;
  set_status(editor, "EDITOR RESTORED - PLAY CHANGES WERE ISOLATED");
}

static void save_play_snapshot(tilemap_editor_t *editor) {
  if (editor->play_history_length == PLAY_HISTORY_CAPACITY) {
    memmove(&editor->play_history[0], &editor->play_history[1],
            (PLAY_HISTORY_CAPACITY - 1u) *
                sizeof(editor->play_history[0]));
    editor->play_history_length--;
  }
  play_snapshot_t *snapshot =
      &editor->play_history[editor->play_history_length++];
  memcpy(snapshot->objects, editor->play_objects,
         sizeof(snapshot->objects));
  snapshot->moves = editor->moves;
  snapshot->pushes = editor->pushes;
}

static void play_undo(tilemap_editor_t *editor) {
  if (editor->play_history_length == 0) {
    set_status(editor, "NO PLAY MOVE TO UNDO");
    return;
  }
  const play_snapshot_t *snapshot =
      &editor->play_history[--editor->play_history_length];
  memcpy(editor->play_objects, snapshot->objects,
         sizeof(editor->play_objects));
  editor->moves = snapshot->moves;
  editor->pushes = snapshot->pushes;
  editor->solved = all_crates_on_goals(editor);
  set_status(editor, "PLAY MOVE UNDONE");
}

static void reset_play(tilemap_editor_t *editor) {
  memcpy(editor->play_objects, editor->play_initial_objects,
         sizeof(editor->play_objects));
  editor->play_history_length = 0;
  editor->moves = 0;
  editor->pushes = 0;
  editor->solved = all_crates_on_goals(editor);
  set_status(editor, "PLAYTEST RESET TO EDITOR MAP");
}

static void move_player(tilemap_editor_t *editor, int dx, int dy) {
  int player_x = -1;
  int player_y = -1;
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      if (editor->play_objects[map_index(x, y)] == OBJECT_PLAYER) {
        player_x = x;
        player_y = y;
      }
    }
  }
  if (player_x < 0 || player_y < 0) {
    set_status(editor, "PLAYER IS MISSING");
    return;
  }
  int target_x = player_x + dx;
  int target_y = player_y + dy;
  if (target_x < 0 || target_y < 0 ||
      target_x >= MAP_WIDTH || target_y >= MAP_HEIGHT) {
    set_status(editor, "BOUNDARY BLOCKS THE PLAYER");
    return;
  }
  size_t player = map_index(player_x, player_y);
  size_t target = map_index(target_x, target_y);
  if (editor->base[target] == TILE_WALL) {
    set_status(editor, "WALL BLOCKS THE PLAYER");
    return;
  }
  bool pushed = false;
  if (editor->play_objects[target] == OBJECT_CRATE) {
    int beyond_x = target_x + dx;
    int beyond_y = target_y + dy;
    if (beyond_x < 0 || beyond_y < 0 ||
        beyond_x >= MAP_WIDTH || beyond_y >= MAP_HEIGHT) {
      set_status(editor, "CRATE CANNOT LEAVE THE MAP");
      return;
    }
    size_t beyond = map_index(beyond_x, beyond_y);
    if (editor->base[beyond] == TILE_WALL ||
        editor->play_objects[beyond] != OBJECT_NONE) {
      set_status(editor, "CRATE IS BLOCKED");
      return;
    }
    save_play_snapshot(editor);
    editor->play_objects[beyond] = OBJECT_CRATE;
    editor->play_objects[target] = OBJECT_NONE;
    pushed = true;
  } else {
    save_play_snapshot(editor);
  }
  editor->play_objects[player] = OBJECT_NONE;
  editor->play_objects[target] = OBJECT_PLAYER;
  editor->moves++;
  if (pushed) editor->pushes++;
  editor->solved = all_crates_on_goals(editor);
  if (editor->solved) {
    set_status(editor, "PUZZLE SOLVED - EVERY CRATE IS ON A GOAL");
  } else if (pushed) {
    set_status(editor, "CRATE PUSHED");
  } else {
    set_status(editor, "PLAYER MOVED");
  }
}

static char serialized_cell(uint8_t base, uint8_t object) {
  if (object == OBJECT_PLAYER) return base == TILE_GOAL ? '+' : 'P';
  if (object == OBJECT_CRATE) return base == TILE_GOAL ? '*' : 'C';
  if (base == TILE_WALL) return '#';
  if (base == TILE_GOAL) return 'G';
  return '.';
}

static bool export_map(tilemap_editor_t *editor, const char *path) {
  if (editor->save_blocked) { set_status(editor,"Cannot overwrite an unreadable map. Choose another output path."); return false; }
  read_wall_clock(&editor->clock);
  map_validation_t validation =
      validate_map(editor->base, editor->objects);
  char temporary[1024];
  int path_length = snprintf(temporary, sizeof(temporary), "%s.tmp", path);
  if (path_length < 0 || (size_t)path_length >= sizeof(temporary)) { set_status(editor, "Save path is too long. Choose a shorter path."); return false; }
  FILE *out = fopen(temporary, "wx");
  if (out == NULL) {
    set_status(editor, "SAVE FAILED - CHECK OUTPUT PATH");
    return false;
  }
  fprintf(out, "# Machine Lab tilemap v1\n");
  fprintf(out, "# saved %04u-%02u-%02uT%02u:%02u:%02u\n",
          (unsigned)editor->clock.year, (unsigned)editor->clock.month,
          (unsigned)editor->clock.day, (unsigned)editor->clock.hour,
          (unsigned)editor->clock.minute, (unsigned)editor->clock.second);
  fprintf(out, "# validation %s\n",
          validation.valid ? "valid" : validation.message);
  fprintf(out, "size %d %d\n", MAP_WIDTH, MAP_HEIGHT);
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      size_t index = map_index(x, y);
      fputc(serialized_cell(editor->base[index],
                            editor->objects[index]), out);
    }
    fputc('\n', out);
  }
  fprintf(out, "end\n");
  bool write_ok = ferror(out) == 0;
  bool close_ok = fclose(out) == 0;
  bool ok = write_ok && close_ok && rename(temporary, path) == 0;
  if (!ok) {
    remove(temporary);
    set_status(editor, "Save failed. Original map preserved.");
    return false;
  }
  memcpy(editor->saved_base, editor->base, sizeof(editor->base));
  memcpy(editor->saved_objects, editor->objects,
         sizeof(editor->objects));
  editor->export_count++;
  set_status(editor, validation.valid
                         ? "SAVED VALID TILEMAP"
                         : "SAVED DRAFT WITH VALIDATION WARNING");
  return true;
}

static void open_map(tilemap_editor_t *editor, const char *path) {
  FILE *in = fopen(path, "rb");
  if (!in) {
    if (errno != ENOENT) { editor->save_blocked = true; set_status(editor, "Cannot read map. Choose a readable file."); }
    return;
  }
  map_snapshot_t loaded = {0}; char line[128]; bool okay = false;
  while (fgets(line, sizeof(line), in)) {
    if (line[0] == '#') continue;
    int width, height; char trailing;
    okay = sscanf(line,"size %d %d %c",&width,&height,&trailing) == 2 && width == MAP_WIDTH && height == MAP_HEIGHT;
    break;
  }
  for (int y=0; y<MAP_HEIGHT && okay; y++) {
    if (!fgets(line,sizeof(line),in)) { okay=false; break; }
    size_t len=strcspn(line,"\r\n");
    if (len != MAP_WIDTH) { okay=false; break; }
    for (int x=0; x<MAP_WIDTH; x++) {
      size_t i=map_index(x,y); char c=line[x];
      loaded.base[i] = c=='#' ? TILE_WALL : (c=='G' || c=='*' || c=='+') ? TILE_GOAL : TILE_FLOOR;
      loaded.objects[i] = (c=='C' || c=='*') ? OBJECT_CRATE : (c=='P' || c=='+') ? OBJECT_PLAYER : OBJECT_NONE;
      if (!strchr("#.GCP*+",c)) { okay=false; break; }
    }
  }
  if (okay) {
    okay=fgets(line,sizeof(line),in) && (!strcmp(line,"end\n") || !strcmp(line,"end\r\n") || !strcmp(line,"end"));
    int extra; do { extra=fgetc(in); } while(extra==' ' || extra=='\r' || extra=='\n' || extra=='\t');
    if (extra != EOF || ferror(in)) okay=false;
  }
  fclose(in);
  if (!okay) { editor->save_blocked=true; set_status(editor,"Invalid map file. Choose another path; original is protected."); return; }
  memcpy(editor->base,loaded.base,sizeof(editor->base)); memcpy(editor->objects,loaded.objects,sizeof(editor->objects));
  memcpy(editor->saved_base,loaded.base,sizeof(editor->base)); memcpy(editor->saved_objects,loaded.objects,sizeof(editor->objects));
  editor->edit_history[0]=loaded; editor->edit_history_cursor=0; editor->edit_history_length=1; editor->loaded_from_disk=true;
  set_status(editor,"Map opened from disk. Continue editing or press Enter to play.");
}

static void handle_left_press(tilemap_editor_t *editor,
                              const char *export_path) {
  editor->keyboard_cursor = false;
  if (point_in_rect(editor->mouse_x, editor->mouse_y, 736, 16, 40, 40)) { editor->help_visible = true; return; }
  if (editor->mode == MODE_PLAY) {
    if (point_in_rect(editor->mouse_x,editor->mouse_y,28,234,132,36)) { play_undo(editor); return; }
    if (point_in_rect(editor->mouse_x,editor->mouse_y,28,280,132,36)) { reset_play(editor); return; }
  }
  if (editor->mode == MODE_EDITOR) {
    if (point_in_rect(editor->mouse_x,editor->mouse_y,640,398,124,30)) { map_validation_t v=validate_map(editor->base,editor->objects); set_status(editor,v.message); return; }
    for (uint8_t brush = 0; brush < BRUSH_COUNT; brush++) {
      int y = 128 + brush * 52;
      if (point_in_rect(editor->mouse_x, editor->mouse_y,
                        28, y, 132, 40)) {
        editor->brush = (map_brush_t)brush;
        char message[48];
        snprintf(message, sizeof(message), "%s BRUSH SELECTED",
                 brush_names[brush]);
        set_status(editor, message);
        return;
      }
    }
    if (point_in_rect(editor->mouse_x, editor->mouse_y,
                      28, 406, 62, 34)) {
      edit_history_undo(editor);
      return;
    }
    if (point_in_rect(editor->mouse_x, editor->mouse_y,
                      98, 406, 62, 34)) {
      edit_history_redo(editor);
      return;
    }
    int cell_x = 0;
    int cell_y = 0;
    if (screen_to_cell(editor->mouse_x, editor->mouse_y,
                       &cell_x, &cell_y)) {
      begin_edit_gesture(editor, cell_x, cell_y);
      return;
    }
  }
  if (point_in_rect(editor->mouse_x, editor->mouse_y,
                    28, 464, 132, 42)) {
    if (editor->mode == MODE_EDITOR) enter_play_mode(editor);
    else leave_play_mode(editor);
    return;
  }
  if (point_in_rect(editor->mouse_x, editor->mouse_y,
                    640, 438, 124, 38) &&
      editor->mode == MODE_EDITOR) {
    export_map(editor, export_path);
    return;
  }
  if (point_in_rect(editor->mouse_x, editor->mouse_y,
                    640, 486, 124, 32)) {
    editor->grid_visible = !editor->grid_visible;
    set_status(editor, editor->grid_visible ? "TILE GRID VISIBLE"
                                            : "TILE GRID HIDDEN");
  }
}

static void handle_mouse_packet(tilemap_editor_t *editor, int dx, int dy,
                                uint8_t buttons, const char *export_path) {
  editor->mouse_x = clamp_int(editor->mouse_x + dx, 0, SCREEN_WIDTH - 1);
  editor->mouse_y = clamp_int(editor->mouse_y - dy, 0, SCREEN_HEIGHT - 1);
  if (editor->help_visible) { editor->previous_buttons = buttons; return; }
  bool left = (buttons & BIT(0)) != 0;
  bool previous_left = (editor->previous_buttons & BIT(0)) != 0;
  bool right = (buttons & BIT(1)) != 0;
  bool previous_right = (editor->previous_buttons & BIT(1)) != 0;
  if (left && !previous_left) handle_left_press(editor, export_path);
  if (left && editor->drawing && editor->mode == MODE_EDITOR) {
    int cell_x = 0;
    int cell_y = 0;
    if (screen_to_cell(editor->mouse_x, editor->mouse_y,
                       &cell_x, &cell_y) &&
        (cell_x != editor->last_cell_x ||
         cell_y != editor->last_cell_y)) {
      draw_brush_line(editor, editor->last_cell_x,
                      editor->last_cell_y, cell_x, cell_y);
      editor->last_cell_x = cell_x;
      editor->last_cell_y = cell_y;
    }
  }
  if (!left && previous_left) finish_edit_gesture(editor);
  if (right && !previous_right && editor->mode == MODE_EDITOR) {
    int cell_x = 0;
    int cell_y = 0;
    if (screen_to_cell(editor->mouse_x, editor->mouse_y,
                       &cell_x, &cell_y)) {
      pick_brush(editor, cell_x, cell_y);
    }
  }
  editor->previous_buttons = buttons;
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

static void validate_and_report(tilemap_editor_t *editor) {
  map_validation_t validation =
      validate_map(editor->base, editor->objects);
  set_status(editor, validation.message);
}

static void select_brush(tilemap_editor_t *editor, map_brush_t brush) {
  editor->brush = brush;
  char message[48];
  snprintf(message, sizeof(message), "%s BRUSH SELECTED",
           brush_names[brush]);
  set_status(editor, message);
}

static void handle_extended_key(tilemap_editor_t *editor, uint8_t code, bool break_code) {
  if (break_code || editor->help_visible || example_primary(&editor->modifiers)) return;
  if (editor->mode == MODE_PLAY) {
    switch (code) {
    case 0x48: move_player(editor, 0, -1); break;
    case 0x50: move_player(editor, 0, 1); break;
    case 0x4B: move_player(editor, -1, 0); break;
    case 0x4D: move_player(editor, 1, 0); break;
    default: break;
    }
  } else {
    finish_edit_gesture(editor);
    if (code == 0x4B) editor->key_x = clamp_int(editor->key_x - 1, 0, MAP_WIDTH - 1);
    else if (code == 0x4D) editor->key_x = clamp_int(editor->key_x + 1, 0, MAP_WIDTH - 1);
    else if (code == 0x48) editor->key_y = clamp_int(editor->key_y - 1, 0, MAP_HEIGHT - 1);
    else if (code == 0x50) editor->key_y = clamp_int(editor->key_y + 1, 0, MAP_HEIGHT - 1);
    else if (code == 0x53) {
      map_brush_t selected=editor->brush; editor->brush=BRUSH_FLOOR;
      begin_edit_gesture(editor,editor->key_x,editor->key_y); finish_edit_gesture(editor); editor->brush=selected;
    } else return;
    editor->keyboard_cursor = true;
  }
}

static void handle_keyboard_byte(tilemap_editor_t *editor, uint8_t data,
                                 bool *running, const char *export_path) {
  if (example_modifier_byte(&editor->modifiers, data)) { editor->extended = false; return; }
  if (data == KBD_TWO_BYTE_PFX) {
    editor->extended = true;
    return;
  }
  bool break_code = (data & KBD_BREAK_BIT) != 0;
  uint8_t code = (uint8_t)(data & 0x7Fu);
  if (editor->extended) {
    handle_extended_key(editor, code, break_code);
    editor->extended = false;
    return;
  }
  if (code == 0x01 && break_code) {
    if (editor->help_visible) { editor->help_visible = false; return; }
    if (editor->mode == MODE_PLAY) { leave_play_mode(editor); return; }
    *running = false;
    return;
  }
  if (break_code) return;
  if (editor->help_visible && code != 0x3B && code != 0x23) return;
  // Keep keyboard history operations atomic with respect to a held mouse drag.
  // The release packet becomes a no-op after this gesture is committed.
  if (editor->mode == MODE_EDITOR) finish_edit_gesture(editor);
  if (example_primary(&editor->modifiers)) {
    if (code == 0x11) { *running = false; return; }
    if (code == 0x1F) { export_map(editor, export_path); }
    else if (code == 0x2C && !editor->modifiers.shift) { if (editor->mode == MODE_PLAY) play_undo(editor); else edit_history_undo(editor); }
    else if (code == 0x15 || (code == 0x2C && editor->modifiers.shift)) { if (editor->mode == MODE_EDITOR) edit_history_redo(editor); }
    return;
  }
  if (code == 0x3B || code == 0x23) {
    editor->help_visible = !editor->help_visible;
    set_status(editor, editor->help_visible ? "SHORTCUT HELP OPEN"
                                            : "SHORTCUT HELP CLOSED");
    return;
  }
  if (code == 0x22) { editor->grid_visible = !editor->grid_visible; return; }
  if (code == 0x39 && editor->mode == MODE_EDITOR) {
    editor->keyboard_cursor=true; begin_edit_gesture(editor,editor->key_x,editor->key_y); finish_edit_gesture(editor); return;
  }
  if (code == 0x1C || code == 0x32 || code == 0x3F) {
    if (editor->mode == MODE_EDITOR) enter_play_mode(editor);
    else leave_play_mode(editor);
    return;
  }
  if (editor->mode == MODE_PLAY) {
    switch (code) {
    case 0x11: move_player(editor, 0, -1); break;
    case 0x1E: move_player(editor, -1, 0); break;
    case 0x1F: move_player(editor, 0, 1); break;
    case 0x20: move_player(editor, 1, 0); break;
    case 0x16: play_undo(editor); break;
    case 0x13: reset_play(editor); break;
    default: break;
    }
    return;
  }
  switch (code) {
  case 0x02: select_brush(editor, BRUSH_FLOOR); break;
  case 0x03: select_brush(editor, BRUSH_WALL); break;
  case 0x04: select_brush(editor, BRUSH_GOAL); break;
  case 0x05: select_brush(editor, BRUSH_CRATE); break;
  case 0x06: select_brush(editor, BRUSH_PLAYER); break;
  case 0x1F:
  case 0x3C: export_map(editor, export_path); break;
  case 0x2F: validate_and_report(editor); break;
  case 0x22:
    editor->grid_visible = !editor->grid_visible;
    set_status(editor, editor->grid_visible ? "TILE GRID VISIBLE"
                                            : "TILE GRID HIDDEN");
    break;
  default: break;
  }
}

static void draw_tile(example_canvas_t *canvas, int x, int y, int size,
                      uint8_t base, uint8_t object, bool compact) {
  uint32_t floor_a = example_rgb(37, 47, 65);
  uint32_t floor_b = example_rgb(40, 51, 70);
  example_fill_rect(canvas, x, y, size, size,
                    ((x / size + y / size) & 1) != 0 ? floor_a : floor_b);
  if (!compact) {
    example_fill_rect(canvas, x + 4, y + 4, 2, 2,
                      example_rgb(58, 70, 91));
    example_fill_rect(canvas, x + size - 7, y + size - 7, 2, 2,
                      example_rgb(27, 35, 49));
  }
  if (base == TILE_WALL) {
    example_fill_rect(canvas, x + 2, y + 2, size - 4, size - 4,
                      example_rgb(63, 83, 112));
    example_fill_rect(canvas, x + 4, y + 4, size - 8,
                      compact ? 2 : 5, example_rgb(91, 116, 149));
    if (!compact) {
      example_fill_rect(canvas, x + 3, y + size / 2 - 1,
                        size - 6, 2, example_rgb(43, 59, 82));
      example_fill_rect(canvas, x + size / 2 - 1, y + 3,
                        2, size / 2 - 4, example_rgb(43, 59, 82));
      example_fill_rect(canvas, x + size / 3 - 1, y + size / 2 + 1,
                        2, size / 2 - 4, example_rgb(43, 59, 82));
    }
  } else if (base == TILE_GOAL) {
    uint32_t goal = example_rgb(244, 184, 96);
    int inset = compact ? 2 : 11;
    example_stroke_rect(canvas, x + inset, y + inset,
                        size - inset * 2, size - inset * 2,
                        compact ? 1 : 3, goal);
    if (!compact) {
      example_fill_rect(canvas, x + size / 2 - 2, y + 7,
                        4, size - 14, goal);
      example_fill_rect(canvas, x + 7, y + size / 2 - 2,
                        size - 14, 4, goal);
    }
  }
  if (object == OBJECT_CRATE) {
    int inset = compact ? 1 : 6;
    example_fill_rect(canvas, x + inset, y + inset,
                      size - inset * 2, size - inset * 2,
                      example_rgb(190, 111, 65));
    example_stroke_rect(canvas, x + inset, y + inset,
                        size - inset * 2, size - inset * 2,
                        compact ? 1 : 3, example_rgb(244, 184, 96));
    if (!compact) {
      example_fill_rect(canvas, x + 12, y + 10, 4, size - 20,
                        example_rgb(126, 72, 55));
      example_fill_rect(canvas, x + size - 16, y + 10, 4, size - 20,
                        example_rgb(126, 72, 55));
      example_fill_rect(canvas, x + 10, y + 12, size - 20, 4,
                        example_rgb(126, 72, 55));
      example_fill_rect(canvas, x + 10, y + size - 16, size - 20, 4,
                        example_rgb(126, 72, 55));
    }
  } else if (object == OBJECT_PLAYER) {
    uint32_t player = example_rgb(84, 183, 211);
    int inset = compact ? 1 : 7;
    example_fill_rect(canvas, x + inset, y + inset,
                      size - inset * 2, size - inset * 2, player);
    if (!compact) {
      example_fill_rect(canvas, x + 12, y + 14, 5, 5,
                        example_rgb(20, 34, 48));
      example_fill_rect(canvas, x + size - 17, y + 14, 5, 5,
                        example_rgb(20, 34, 48));
      example_fill_rect(canvas, x + 14, y + size - 15,
                        size - 28, 3, example_rgb(20, 34, 48));
    }
  }
}

static void draw_header(example_canvas_t *canvas, const tilemap_editor_t *editor) {
  example_heading(canvas, "Tile Forge", "Build a puzzle. Find its solution.", UI_ACCENT, false);
  example_label(canvas, 552, 18, editor->mode == MODE_PLAY ? "Playtest" : "Map editor", UI_INK, 1);
  example_label(canvas, 552, 42, editor_is_dirty(editor) ? "Unsaved changes" : editor->loaded_from_disk || editor->export_count ? "Map saved" : "Demo level", UI_MUTED, 1);
  example_button(canvas,736,16,40,40,"?",false,point_in_rect(editor->mouse_x,editor->mouse_y,736,16,40,40),true);
}


static void draw_brush_swatch(example_canvas_t *canvas, int x, int y,
                              map_brush_t brush) {
  uint8_t base = TILE_FLOOR;
  uint8_t object = OBJECT_NONE;
  if (brush == BRUSH_WALL) base = TILE_WALL;
  else if (brush == BRUSH_GOAL) base = TILE_GOAL;
  else if (brush == BRUSH_CRATE) object = OBJECT_CRATE;
  else if (brush == BRUSH_PLAYER) object = OBJECT_PLAYER;
  draw_tile(canvas, x, y, 24, base, object, true);
}

static void draw_palette_panel(example_canvas_t *canvas, const tilemap_editor_t *editor) {
  example_label(canvas, 28, 100, editor->mode == MODE_PLAY ? "Playtest" : "Tile palette", UI_MUTED, 1);
  if (editor->mode == MODE_EDITOR) {
    for (uint8_t brush = 0; brush < BRUSH_COUNT; brush++) {
      int y = 128 + brush * 52;
      bool selected = editor->brush == (map_brush_t)brush;
      example_button(canvas, 28, y, 132, 40, "", selected,
        point_in_rect(editor->mouse_x, editor->mouse_y, 28, y, 132, 40), true);
      draw_brush_swatch(canvas, 34, y + 8, (map_brush_t)brush);
      example_label(canvas, 64, y + 10, brush_names[brush], selected ? UI_PAPER : UI_INK, 1);
      example_label(canvas, 146, y + 10, brush_keys[brush], selected ? UI_PAPER : UI_MUTED, 1);
    }
    example_button(canvas, 28, 406, 62, 34, "Undo", false,
      point_in_rect(editor->mouse_x, editor->mouse_y, 28, 406, 62, 34), editor->edit_history_cursor > 0);
    example_button(canvas, 98, 406, 62, 34, "Redo", false,
      point_in_rect(editor->mouse_x, editor->mouse_y, 98, 406, 62, 34), editor->edit_history_cursor + 1 < editor->edit_history_length);
  } else {
    example_label(canvas,28,144,"Move your player",UI_MUTED,1);
    example_label(canvas,28,170,"Arrows / WASD",UI_INK,1);
    example_button(canvas,28,234,132,36,"Undo move  U",false,point_in_rect(editor->mouse_x,editor->mouse_y,28,234,132,36),editor->play_history_length>0);
    example_button(canvas,28,280,132,36,"Reset puzzle  R",false,point_in_rect(editor->mouse_x,editor->mouse_y,28,280,132,36),editor->moves>0);
    char stats[40];
    snprintf(stats,sizeof(stats),"%u moves",editor->moves); example_label(canvas,28,364,stats,UI_ACCENT,1);
    snprintf(stats,sizeof(stats),"%u pushes",editor->pushes); example_label(canvas,28,390,stats,UI_MUTED,1);
  }
  example_button(canvas, 28, 464, 132, 42, editor->mode == MODE_PLAY ? "Back to edit" : "Play  Enter", true,
    point_in_rect(editor->mouse_x, editor->mouse_y, 28, 464, 132, 42), true);
}


static void draw_map_canvas(example_canvas_t *canvas,
                            const tilemap_editor_t *editor) {
  example_fill_rect(canvas, 178, 96, 442, 426, 0xDFE5E9);
  example_stroke_rect(canvas, 178, 96, 442, 426, 1,
                      editor->mode == MODE_PLAY ? example_rgb(97, 201, 141)
                                                : UI_LINE);
  example_label(canvas, 198, 102, editor->mode == MODE_PLAY ? "Live playtest" : "Map canvas",
                editor->mode == MODE_PLAY ? UI_ACCENT : UI_MUTED, 1);
  const uint8_t *objects = editor->mode == MODE_PLAY
                               ? editor->play_objects
                               : editor->objects;
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      size_t index = map_index(x, y);
      int tile_x = MAP_ORIGIN_X + x * MAP_CELL_SIZE;
      int tile_y = MAP_ORIGIN_Y + y * MAP_CELL_SIZE;
      draw_tile(canvas, tile_x, tile_y, MAP_CELL_SIZE,
                editor->base[index], objects[index], false);
      if (editor->grid_visible) {
        example_stroke_rect(canvas, tile_x, tile_y,
                            MAP_CELL_SIZE, MAP_CELL_SIZE, 1,
                            example_rgb(20, 27, 39));
      }
    }
  }
  example_stroke_rect(canvas, MAP_ORIGIN_X - 2, MAP_ORIGIN_Y - 2,
                      MAP_WIDTH * MAP_CELL_SIZE + 4,
                      MAP_HEIGHT * MAP_CELL_SIZE + 4, 2,
                      example_rgb(91, 105, 128));
  int hover_x = 0;
  int hover_y = 0;
  bool hovering = screen_to_cell(editor->mouse_x, editor->mouse_y, &hover_x, &hover_y);
  if (editor->keyboard_cursor) { hover_x=editor->key_x; hover_y=editor->key_y; hovering=true; }
  if (editor->mode == MODE_EDITOR && hovering) {
    example_stroke_rect(canvas,
                        MAP_ORIGIN_X + hover_x * MAP_CELL_SIZE - 1,
                        MAP_ORIGIN_Y + hover_y * MAP_CELL_SIZE - 1,
                        MAP_CELL_SIZE + 2, MAP_CELL_SIZE + 2, 2,
                        example_rgb(255, 235, 190));
  }
  if (editor->mode == MODE_PLAY) {
    example_round_rect(canvas,198,466,400,36,5,editor->solved ? 0xD6EBDF : 0xEAF0F1);
    example_label(canvas,212,474,editor->solved ? "Puzzle solved!  Undo, reset, or return to edit." : "Push each crate onto a goal.",UI_ACCENT,1);
  } else {
    char location[80];
    if (hovering) snprintf(location,sizeof(location),"Tile %d, %d    %s brush    Space to paint",hover_x+1,hover_y+1,brush_names[editor->brush]);
    else snprintf(location,sizeof(location),"10 x 8 tiles    Two layers: terrain + objects");
    example_label(canvas,198,474,location,UI_MUTED,1);
  }
}

static void draw_mini_map(example_canvas_t *canvas,
                          const tilemap_editor_t *editor) {
  const uint8_t *objects = editor->mode == MODE_PLAY
                               ? editor->play_objects
                               : editor->objects;
  example_fill_rect(canvas, 640, 278, 124, 104, example_rgb(18, 23, 33));
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      size_t index = map_index(x, y);
      draw_tile(canvas, 652 + x * 10, 290 + y * 10, 10,
                editor->base[index], objects[index], true);
    }
  }
}

static void draw_inspector(example_canvas_t *canvas, const tilemap_editor_t *editor) {
  map_validation_t validation = validate_map(editor->base, editor->objects);
  example_label(canvas, 640, 100, "Map check", UI_MUTED, 1);
  example_round_rect(canvas, 640, 128, 124, 32, 5, validation.valid ? 0xD8EBE3 : 0xF8E1DC);
  example_label(canvas, 650, 134, validation.valid ? "Ready to play" : "Needs a fix", validation.valid ? UI_ACCENT : 0xA13D34, 1);
  char count[40];
  snprintf(count, sizeof(count), "Players   %u", validation.players); example_label(canvas, 640, 178, count, UI_INK, 1);
  snprintf(count, sizeof(count), "Crates     %u", validation.crates); example_label(canvas, 640, 204, count, UI_INK, 1);
  snprintf(count, sizeof(count), "Goals      %u", validation.goals); example_label(canvas, 640, 230, count, UI_INK, 1);
  example_label(canvas, 640, 254, "Overview", UI_MUTED, 1);
  draw_mini_map(canvas, editor);
  if (editor->mode == MODE_EDITOR) {
    example_button(canvas,640,398,124,30,"Validate  V",false,point_in_rect(editor->mouse_x,editor->mouse_y,640,398,124,30),true);
    example_button(canvas, 640, 438, 124, 38, "Save map  S", true,
      point_in_rect(editor->mouse_x, editor->mouse_y, 640, 438, 124, 38), true);
  } else {
    example_label(canvas, 640, 428, editor->solved ? "Puzzle solved!" : "Keep going", UI_ACCENT, 1);
  }
  example_button(canvas, 640, 486, 124, 32, editor->grid_visible ? "Grid on  G" : "Grid off  G", false,
    point_in_rect(editor->mouse_x, editor->mouse_y, 640, 486, 124, 32), true);
}


static void draw_footer(example_canvas_t *canvas, const tilemap_editor_t *editor) {
  example_fill_rect(canvas, 0, 542, 800, 58, UI_PAPER);
  example_label(canvas, 24, 548, editor->status, UI_INK, 1);
  example_label(canvas, 24, 575, editor->mode == MODE_PLAY
    ? "Arrows / WASD Move   U Undo   R Reset   Enter Edit   Esc Edit"
    : "1-5 Tiles   Arrows Cursor   Space Paint   Enter Play   Cmd/Ctrl+S Save   F1 Help", UI_MUTED, 1);
}


static void draw_help_overlay(example_canvas_t *canvas) {
  example_round_rect(canvas,112,90,576,420,10,UI_DARK);
  example_label(canvas,142,112,"Tile Forge shortcuts",UI_LIGHT,2);
  const char *lines[]={"1-5   Choose tile     Drag   Paint     Right-click   Pick", "Arrows   Move tile cursor      Space   Paint tile", "Delete   Erase tile under cursor", "Cmd/Ctrl+Z   Undo edit or playtest move", "Cmd/Ctrl+Shift+Z or Ctrl+Y   Redo edit", "S / F2 / Cmd/Ctrl+S   Save map", "V   Validate map       G   Grid", "Enter / F5   Switch between edit and playtest", "Playtest: Arrows / WASD   Move     U   Undo     R   Reset", "Esc   Back to edit       F1 / Esc   Close help"};
  for(int i=0;i<10;i++) example_label(canvas,142,174+i*31,lines[i],UI_DIM,1);
}

static void draw_mouse_cursor(example_canvas_t *canvas,
                              const tilemap_editor_t *editor) {
  int x = editor->mouse_x;
  int y = editor->mouse_y;
  for (int row = 0; row < 14; row++) {
    int width = row < 9 ? row / 2 + 2 : 3;
    example_fill_rect(canvas, x + 2, y + row + 2, width, 1,
                      example_rgb(8, 12, 18));
    example_fill_rect(canvas, x, y + row, width, 1,
                      example_rgb(246, 248, 251));
  }
  example_fill_rect(canvas, x + 5, y + 10, 3, 7,
                    example_rgb(8, 12, 18));
  example_fill_rect(canvas, x + 4, y + 9, 3, 7,
                    example_rgb(246, 248, 251));
}

static void draw_tilemap_editor(example_canvas_t *canvas,
                                const tilemap_editor_t *editor) {
  example_fill_rect(canvas, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                    UI_BG);
  draw_header(canvas, editor);
  draw_palette_panel(canvas, editor);
  draw_map_canvas(canvas, editor);
  draw_inspector(canvas, editor);
  draw_footer(canvas, editor);
  if (editor->help_visible) draw_help_overlay(canvas);
  draw_mouse_cursor(canvas, editor);
}

int main(int argc, char **argv) {
  const char *export_path =
      argc > 1 ? argv[1] : "build/tilemap_demo.mlmap";
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
  tilemap_editor_t editor;
  seed_map(&editor);
  open_map(&editor, export_path);

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
  read_wall_clock(&editor.clock);

  example_canvas_t canvas = {framebuffer, info};
  bool running = true;
  draw_tilemap_editor(&canvas, &editor);
  lcom_vbe_present();
  lcom_printf("Tile Forge: paint with mouse, 1-5 choose tiles, "
              "Enter playtests, S saves, ESC exits.\n");
  bool event_failed = false;
  uint64_t last_clock_tick = 0;
  while (running) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) {
      event_failed = true;
      set_status(&editor, "RUNTIME EVENT WAIT FAILED");
      break;
    }
    bool redraw = false;
    // Delivered interrupts can coalesce. Poll the RTC using elapsed bus ticks.
    if (event.tick - last_clock_tick >= 60u) {
      read_wall_clock(&editor.clock);
      last_clock_tick = event.tick;
    }
    if ((event.irq_mask & timer_irq.mask) != 0) {
      editor.frame++;
      redraw = true;
    }
    if ((event.irq_mask & keyboard_irq.mask) != 0) {
      uint8_t status = 0;
      uint8_t data = 0;
      if (lcom_port_read8(KBC_ST_REG, &status) == LCOM_OK &&
          (status & KBC_ST_OBF) != 0 && (status & KBC_ST_AUX) == 0 &&
          (status & KBC_ST_ERR) == 0 &&
          lcom_port_read8(KBC_OUT_BUF, &data) == LCOM_OK) {
        handle_keyboard_byte(&editor, data, &running, export_path);
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
          handle_mouse_packet(&editor, dx, dy, buttons, export_path);
          redraw = true;
        }
      }
    }
    if (redraw) {
      draw_tilemap_editor(&canvas, &editor);
      lcom_vbe_present();
    }
  }
  finish_edit_gesture(&editor);
  read_wall_clock(&editor.clock);
  draw_tilemap_editor(&canvas, &editor);
  lcom_vbe_present();
  map_validation_t validation =
      validate_map(editor.base, editor.objects);
  char clock[24];
  format_clock(&editor.clock, clock, sizeof(clock));
  lcom_printf("tilemap editor summary mode=%s valid=%u moves=%u pushes=%u "
              "solved=%u edits=%u exports=%u checksum=%08x rtc=%s\n",
              editor.mode == MODE_PLAY ? "play" : "editor",
              validation.valid ? 1u : 0u, (unsigned)editor.moves,
              (unsigned)editor.pushes, editor.solved ? 1u : 0u,
              (unsigned)editor.committed_edits,
              (unsigned)editor.export_count,
              (unsigned)map_checksum(editor.base, editor.objects), clock);
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
