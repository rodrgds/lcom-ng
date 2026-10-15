#include "example_ui.h"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/vbe.h>

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define EDITOR_MAX_TEXT 4096u
#define EDITOR_WRAP_COLS 70u
#define EDITOR_VISIBLE_ROWS 18u
#define EDITOR_HISTORY 32u

typedef struct {
  char text[EDITOR_MAX_TEXT + 1u];
  size_t len, cursor;
} editor_snapshot_t;

typedef struct {
  char text[EDITOR_MAX_TEXT + 1u];
  size_t len;
  size_t cursor;
  uint16_t scroll_line;
  example_modifiers_t modifiers;
  bool extended;
  bool overwrite;
  bool dirty;
  bool caps_lock;
  bool help;
  editor_snapshot_t history[EDITOR_HISTORY];
  unsigned history_len, history_pos;
  size_t anchor;
  char saved_text[EDITOR_MAX_TEXT + 1u];
  char clipboard[EDITOR_MAX_TEXT + 1u];
  int mouse_x, mouse_y;
  uint8_t mouse_bytes[3], mouse_index, mouse_buttons;
  bool mouse_ack, dragging, close_prompt;
  const char *path;
  bool save_blocked;
  char status[64];
} editor_state_t;

typedef struct {
  uint16_t line;
  uint16_t col;
} visual_pos_t;

static bool is_letter_code(uint8_t code) {
  return (code >= 0x10 && code <= 0x19) || (code >= 0x1E && code <= 0x26) ||
         (code >= 0x2C && code <= 0x32);
}

static char base_scancode_char(uint8_t code) {
  switch (code) {
  case 0x02: return '1';
  case 0x03: return '2';
  case 0x04: return '3';
  case 0x05: return '4';
  case 0x06: return '5';
  case 0x07: return '6';
  case 0x08: return '7';
  case 0x09: return '8';
  case 0x0A: return '9';
  case 0x0B: return '0';
  case 0x0C: return '-';
  case 0x0D: return '=';
  case 0x10: return 'q';
  case 0x11: return 'w';
  case 0x12: return 'e';
  case 0x13: return 'r';
  case 0x14: return 't';
  case 0x15: return 'y';
  case 0x16: return 'u';
  case 0x17: return 'i';
  case 0x18: return 'o';
  case 0x19: return 'p';
  case 0x1A: return '[';
  case 0x1B: return ']';
  case 0x1E: return 'a';
  case 0x1F: return 's';
  case 0x20: return 'd';
  case 0x21: return 'f';
  case 0x22: return 'g';
  case 0x23: return 'h';
  case 0x24: return 'j';
  case 0x25: return 'k';
  case 0x26: return 'l';
  case 0x27: return ';';
  case 0x28: return '\'';
  case 0x29: return '`';
  case 0x2B: return '\\';
  case 0x2C: return 'z';
  case 0x2D: return 'x';
  case 0x2E: return 'c';
  case 0x2F: return 'v';
  case 0x30: return 'b';
  case 0x31: return 'n';
  case 0x32: return 'm';
  case 0x33: return ',';
  case 0x34: return '.';
  case 0x35: return '/';
  case 0x39: return ' ';
  default: return 0;
  }
}

static char shifted_symbol(char c) {
  switch (c) {
  case '`': return '~';
  case '1': return '!';
  case '2': return '@';
  case '3': return '#';
  case '4': return '$';
  case '5': return '%';
  case '6': return '^';
  case '7': return '&';
  case '8': return '*';
  case '9': return '(';
  case '0': return ')';
  case '-': return '_';
  case '=': return '+';
  case '[': return '{';
  case ']': return '}';
  case ';': return ':';
  case '\'': return '"';
  case '\\': return '|';
  case ',': return '<';
  case '.': return '>';
  case '/': return '?';
  default: return c;
  }
}

static char scancode_to_char(const editor_state_t *ed, uint8_t code) {
  char c = base_scancode_char(code);
  bool shift = ed->modifiers.shift != 0;
  if (c == 0 || c == ' ') return c;
  if (is_letter_code(code)) {
    bool upper = shift != ed->caps_lock;
    return upper ? (char)(c - 'a' + 'A') : c;
  }
  return shift ? shifted_symbol(c) : c;
}

static int read_keyboard_byte(uint8_t *data) {
  uint8_t status = 0;
  if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK) return LCOM_ERR;
  if ((status & KBC_ST_OBF) == 0 || (status & KBC_ST_AUX) != 0) return LCOM_NO_DATA;
  return lcom_port_read8(KBC_OUT_BUF, data);
}

static void visual_position_at(const editor_state_t *ed, size_t index, visual_pos_t *out) {
  uint16_t line = 0;
  uint16_t col = 0;
  if (index > ed->len) index = ed->len;
  for (size_t i = 0; i < index; i++) {
    if (ed->text[i] == '\n') {
      line++;
      col = 0;
    } else if (++col >= EDITOR_WRAP_COLS) {
      line++;
      col = 0;
    }
  }
  out->line = line;
  out->col = col;
}

static size_t index_for_visual_position(const editor_state_t *ed, uint16_t target_line,
                                        uint16_t target_col) {
  uint16_t line = 0;
  uint16_t col = 0;
  for (size_t i = 0; i < ed->len; i++) {
    if (line == target_line && col >= target_col) return i;
    if (ed->text[i] == '\n') {
      if (line == target_line) return i;
      line++;
      col = 0;
    } else if (++col >= EDITOR_WRAP_COLS) {
      line++;
      col = 0;
    }
  }
  return ed->len;
}

static uint16_t total_visual_lines(const editor_state_t *ed) {
  visual_pos_t end;
  visual_position_at(ed, ed->len, &end);
  return (uint16_t)(end.line + 1u);
}

static uint16_t word_count(const editor_state_t *ed) {
  uint16_t words = 0;
  bool in_word = false;
  for (size_t i = 0; i < ed->len; i++) {
    bool space = ed->text[i] == ' ' || ed->text[i] == '\n' || ed->text[i] == '\t';
    if (space) in_word = false;
    else if (!in_word) {
      words++;
      in_word = true;
    }
  }
  return words;
}

static void set_status(editor_state_t *ed, const char *status) {
  snprintf(ed->status, sizeof(ed->status), "%s", status);
}

static void ensure_cursor_visible(editor_state_t *ed) {
  visual_pos_t pos;
  visual_position_at(ed, ed->cursor, &pos);
  if (pos.line < ed->scroll_line) ed->scroll_line = pos.line;
  else if (pos.line >= ed->scroll_line + EDITOR_VISIBLE_ROWS) {
    ed->scroll_line = (uint16_t)(pos.line - EDITOR_VISIBLE_ROWS + 1u);
  }
}

static void update_dirty(editor_state_t *ed) {
  ed->dirty = strcmp(ed->text, ed->saved_text) != 0;
}

static void remember_undo(editor_state_t *ed) {
  if (ed->history_len && !strcmp(ed->history[ed->history_pos].text, ed->text)) return;
  if (ed->history_len) ed->history_len = ed->history_pos + 1;
  if (ed->history_len == EDITOR_HISTORY) {
    memmove(ed->history, ed->history + 1, sizeof(ed->history[0]) * (EDITOR_HISTORY - 1));
    ed->history_len--;
  }
  editor_snapshot_t *snap = &ed->history[ed->history_len++];
  memcpy(snap->text, ed->text, ed->len + 1);
  snap->len = ed->len; snap->cursor = ed->cursor;
  ed->history_pos = ed->history_len - 1;
}

static void history_move(editor_state_t *ed, bool redo) {
  remember_undo(ed);
  if ((redo && ed->history_pos + 1 >= ed->history_len) || (!redo && !ed->history_pos)) {
    set_status(ed, redo ? "Nothing to redo" : "Nothing to undo"); return;
  }
  ed->history_pos += redo ? 1 : -1;
  const editor_snapshot_t *snap = &ed->history[ed->history_pos];
  memcpy(ed->text, snap->text, snap->len + 1);
  ed->len = snap->len; ed->cursor = ed->anchor = snap->cursor;
  update_dirty(ed); ensure_cursor_visible(ed);
  set_status(ed, redo ? "Redone" : "Undone");
}

static void erase_selection(editor_state_t *ed) {
  if (ed->anchor == ed->cursor) return;
  size_t first = ed->anchor < ed->cursor ? ed->anchor : ed->cursor;
  size_t last = ed->anchor > ed->cursor ? ed->anchor : ed->cursor;
  memmove(ed->text + first, ed->text + last, ed->len - last + 1);
  ed->len -= last - first;
  ed->anchor = ed->cursor = first;
}

static void copy_selection(editor_state_t *ed, bool cut) {
  size_t first = ed->anchor < ed->cursor ? ed->anchor : ed->cursor;
  size_t last = ed->anchor > ed->cursor ? ed->anchor : ed->cursor;
  if (first == last) { set_status(ed, "Select text with Shift+arrows or drag first"); return; }
  memcpy(ed->clipboard, ed->text + first, last - first);
  ed->clipboard[last - first] = 0;
  if (lcom_clipboard_set_text(ed->clipboard) != LCOM_OK) { set_status(ed, "Copy failed. Selection exceeds clipboard limit or clipboard unavailable"); return; }
  if (cut) { remember_undo(ed); erase_selection(ed); update_dirty(ed); ensure_cursor_visible(ed); }
  set_status(ed, cut ? "Cut to clipboard" : "Copied to clipboard");
}

static void paste(editor_state_t *ed) {
  if (lcom_clipboard_get_text(ed->clipboard, sizeof(ed->clipboard)) != LCOM_OK) { set_status(ed, "Paste failed. Clipboard unavailable or text exceeds 4 KB"); return; }
  for (size_t i = 0; ed->clipboard[i]; i++) {
    unsigned char c = (unsigned char)ed->clipboard[i];
    if ((c < 32 && c != '\n' && c != '\t' && c != '\r') || c > 126) { set_status(ed, "Paste supports plain ASCII text. Convert the text first."); return; }
  }
  char normalized[EDITOR_MAX_TEXT + 1u]; size_t normalized_len = 0;
  for (size_t i = 0; ed->clipboard[i]; i++) {
    char c = ed->clipboard[i];
    if (c == '\r') { if (ed->clipboard[i + 1] == '\n') continue; c = '\n'; }
    unsigned repeat = c == '\t' ? 4 : 1;
    if (normalized_len + repeat > EDITOR_MAX_TEXT) { set_status(ed, "Paste exceeds 4 KB after expanding tabs"); return; }
    while (repeat--) normalized[normalized_len++] = c == '\t' ? ' ' : c;
  }
  normalized[normalized_len] = 0; memcpy(ed->clipboard, normalized, normalized_len + 1);
  size_t n = normalized_len;
  size_t selected = ed->anchor > ed->cursor ? ed->anchor - ed->cursor : ed->cursor - ed->anchor;
  if (!n) { set_status(ed, "Clipboard is empty"); return; }
  if (ed->len - selected + n > EDITOR_MAX_TEXT) { set_status(ed, "Not enough room to paste. Limit is 4 KB"); return; }
  remember_undo(ed); erase_selection(ed);
  memmove(ed->text + ed->cursor + n, ed->text + ed->cursor, ed->len - ed->cursor + 1);
  memcpy(ed->text + ed->cursor, ed->clipboard, n);
  ed->len += n; ed->cursor += n; ed->anchor = ed->cursor;
  update_dirty(ed); ensure_cursor_visible(ed); set_status(ed, "Pasted from clipboard");
}

static void insert_char(editor_state_t *ed, char c) {
  if (ed->len >= EDITOR_MAX_TEXT && ed->anchor == ed->cursor &&
      (!ed->overwrite || ed->cursor >= ed->len || ed->text[ed->cursor] == '\n')) {
    set_status(ed, "DOCUMENT FULL");
    return;
  }
  remember_undo(ed);
  bool selected = ed->anchor != ed->cursor;
  erase_selection(ed);
  if (!selected && ed->overwrite && ed->cursor < ed->len && ed->text[ed->cursor] != '\n') {
    ed->text[ed->cursor++] = c;
  } else {
    memmove(&ed->text[ed->cursor + 1u], &ed->text[ed->cursor], ed->len - ed->cursor + 1u);
    ed->text[ed->cursor++] = c;
    ed->len++;
  }
  ed->anchor = ed->cursor;
  update_dirty(ed);
  set_status(ed, "Editing");
  ensure_cursor_visible(ed);
}

static void delete_at_cursor(editor_state_t *ed) {
  if (ed->anchor != ed->cursor) {
    remember_undo(ed); erase_selection(ed); update_dirty(ed); ensure_cursor_visible(ed); return;
  }
  if (ed->cursor >= ed->len) return;
  remember_undo(ed);
  memmove(&ed->text[ed->cursor], &ed->text[ed->cursor + 1u], ed->len - ed->cursor);
  ed->len--;
  ed->anchor = ed->cursor;
  update_dirty(ed);
  set_status(ed, "Editing");
  ensure_cursor_visible(ed);
}

static void backspace(editor_state_t *ed) {
  if (ed->anchor != ed->cursor) { delete_at_cursor(ed); return; }
  if (ed->cursor == 0) return;
  remember_undo(ed);
  ed->cursor--; ed->anchor = ed->cursor;
  delete_at_cursor(ed);
}

static void move_cursor_line(editor_state_t *ed, int delta) {
  visual_pos_t pos;
  visual_position_at(ed, ed->cursor, &pos);
  int next_line = (int)pos.line + delta;
  if (next_line < 0) next_line = 0;
  ed->cursor = index_for_visual_position(ed, (uint16_t)next_line, pos.col);
  ensure_cursor_visible(ed);
}

static void move_cursor_home(editor_state_t *ed) {
  visual_pos_t pos;
  visual_position_at(ed, ed->cursor, &pos);
  ed->cursor = index_for_visual_position(ed, pos.line, 0);
  ensure_cursor_visible(ed);
}

static void move_cursor_end(editor_state_t *ed) {
  visual_pos_t pos;
  visual_position_at(ed, ed->cursor, &pos);
  size_t next = index_for_visual_position(ed, (uint16_t)(pos.line + 1u), 0);
  if (next > 0 && next <= ed->len && ed->text[next - 1u] == '\n') next--;
  ed->cursor = next;
  ensure_cursor_visible(ed);
}

static void handle_extended_key(editor_state_t *ed, uint8_t code, bool break_code) {
  if (break_code) return;
  size_t anchor = ed->anchor;
  if (example_primary(&ed->modifiers)) {
    if (code == 0x48 || code == 0x47) ed->cursor = 0;
    if (code == 0x50 || code == 0x4F) ed->cursor = ed->len;
    if (ed->modifiers.command) {
      if (code == 0x4B) move_cursor_home(ed);
      if (code == 0x4D) move_cursor_end(ed);
    } else {
      if (code == 0x4B) {
        while (ed->cursor && (ed->text[ed->cursor - 1] == ' ' || ed->text[ed->cursor - 1] == '\n')) ed->cursor--;
        while (ed->cursor && ed->text[ed->cursor - 1] != ' ' && ed->text[ed->cursor - 1] != '\n') ed->cursor--;
      }
      if (code == 0x4D) {
        while (ed->cursor < ed->len && ed->text[ed->cursor] != ' ' && ed->text[ed->cursor] != '\n') ed->cursor++;
        while (ed->cursor < ed->len && (ed->text[ed->cursor] == ' ' || ed->text[ed->cursor] == '\n')) ed->cursor++;
      }
    }
  } else {
    switch (code) {
    case 0x47: move_cursor_home(ed); break;
    case 0x4F: move_cursor_end(ed); break;
    case 0x48: move_cursor_line(ed, -1); break;
    case 0x50: move_cursor_line(ed, 1); break;
    case 0x4B:
      if (ed->cursor > 0) ed->cursor--;
      ensure_cursor_visible(ed);
      break;
    case 0x4D:
      if (ed->cursor < ed->len) ed->cursor++;
      ensure_cursor_visible(ed);
      break;
    case 0x49: move_cursor_line(ed, -(int)EDITOR_VISIBLE_ROWS); break;
    case 0x51: move_cursor_line(ed, EDITOR_VISIBLE_ROWS); break;
    case 0x52:
      ed->overwrite = !ed->overwrite;
      set_status(ed, ed->overwrite ? "OVERWRITE MODE" : "INSERT MODE");
      break;
    case 0x53: delete_at_cursor(ed); break;
    default: break;
    }
  }
  ed->anchor = ed->modifiers.shift ? anchor : ed->cursor;
  ensure_cursor_visible(ed);
}

static void save_document(editor_state_t *ed) {
  if (ed->save_blocked) { set_status(ed, "File cannot be opened safely"); return; }
  char temporary[1024];
  int count = snprintf(temporary, sizeof(temporary), "%s.tmp", ed->path);
  if (count < 0 || (size_t)count >= sizeof(temporary)) {
    set_status(ed, "Path too long. Use a shorter filename"); return;
  }
  FILE *file = fopen(temporary, "wbx");
  if (!file) { set_status(ed, "Save failed. Check folder permissions"); return; }
  bool written = fwrite(ed->text, 1, ed->len, file) == ed->len;
  if (fclose(file) != 0) written = false;
  if (!written || rename(temporary, ed->path) != 0) {
    remove(temporary);
    set_status(ed, "Save failed. Original file preserved"); return;
  }
  memcpy(ed->saved_text, ed->text, ed->len + 1);
  ed->dirty = false;
  set_status(ed, "Saved to disk");
}

static void open_document(editor_state_t *ed) {
  FILE *file = fopen(ed->path, "rb");
  if (!file) {
    if (errno != ENOENT) { ed->save_blocked = true; set_status(ed, "Cannot read file"); }
    return;
  }
  size_t length = fread(ed->text, 1, sizeof(ed->text), file);
  bool failed = ferror(file) || length > EDITOR_MAX_TEXT;
  fclose(file);
  if (failed || memchr(ed->text, '\0', length)) {
    ed->text[0] = 0;
    ed->save_blocked = true;
    set_status(ed, "Open failed. Use a text file under 4 KB"); return;
  }
  ed->len = ed->cursor = ed->anchor = length;
  ed->text[length] = 0;
  ensure_cursor_visible(ed);
  set_status(ed, "Opened from disk");
}

static void request_close(editor_state_t *ed, bool *running) {
  if (ed->dirty) { ed->close_prompt = true; ed->dragging = false; }
  else *running = false;
}

static void handle_keyboard_byte(editor_state_t *ed, uint8_t data, bool *running) {
  if (example_modifier_byte(&ed->modifiers, data)) { ed->extended = false; return; }
  if (data == KBD_TWO_BYTE_PFX) { ed->extended = true; return; }
  bool release = (data & KBD_BREAK_BIT) != 0;
  uint8_t code = data & 0x7F;
  if (ed->extended) {
    if (!ed->help && !ed->close_prompt) handle_extended_key(ed, code, release);
    ed->extended = false; return;
  }
  if (data == ESC_BREAK) {
    if (ed->help) ed->help = false;
    else if (ed->close_prompt) ed->close_prompt = false;
    else request_close(ed, running);
    return;
  }
  if (release) return;
  if (ed->close_prompt) {
    if (code == 0x20) *running = false; /* D: discard */
    else if (code == 0x1F || code == 0x1C) { save_document(ed); if (!ed->dirty) *running = false; }
    return;
  }
  if (ed->help) { if (code == 0x3B) ed->help = false; return; }
  if (example_primary(&ed->modifiers)) {
    switch (code) {
    case 0x11: request_close(ed, running); break;
    case 0x1F: save_document(ed); break;
    case 0x2C: history_move(ed, ed->modifiers.shift != 0); break;
    case 0x15: history_move(ed, true); break;
    case 0x1E: ed->anchor = 0; ed->cursor = ed->len; ensure_cursor_visible(ed); set_status(ed, "All text selected"); break;
    case 0x2E: copy_selection(ed, false); break;
    case 0x2D: copy_selection(ed, true); break;
    case 0x2F: paste(ed); break;
    default: break;
    }
    return;
  }
  if (code == 0x3A) ed->caps_lock = !ed->caps_lock;
  else if (code == 0x3B) { ed->help = true; ed->dragging = false; }
  else if (code == 0x3C) save_document(ed);
  else if (code == 0x0E) backspace(ed);
  else if (code == 0x1C) insert_char(ed, '\n');
  else if (code == 0x0F) { for (unsigned i = 0; i < 4; i++) insert_char(ed, ' '); }
  else { char c = scancode_to_char(ed, code); if (c) insert_char(ed, c); }
}

static bool in_rect(int x, int y, int rx, int ry, int w, int h) {
  return x >= rx && y >= ry && x < rx + w && y < ry + h;
}

static void place_caret(editor_state_t *ed, bool extend) {
  if (!in_rect(ed->mouse_x, ed->mouse_y, 62, 154, 648, 360)) return;
  uint16_t row = (uint16_t)((ed->mouse_y - 154) / 20 + ed->scroll_line);
  uint16_t col = (uint16_t)((ed->mouse_x - 76 + 4) / 9);
  if (ed->mouse_x < 76) col = 0;
  if (col > EDITOR_WRAP_COLS) col = EDITOR_WRAP_COLS;
  ed->cursor = index_for_visual_position(ed, row, col);
  if (!extend) ed->anchor = ed->cursor;
  ensure_cursor_visible(ed);
}

static void handle_mouse_byte(editor_state_t *ed, uint8_t byte, bool *running) {
  if (!ed->mouse_ack) { if (byte == MOUSE_ACK) ed->mouse_ack = true; return; }
  if (!ed->mouse_index && !(byte & MOUSE_SYNC_BIT)) return;
  ed->mouse_bytes[ed->mouse_index++] = byte;
  if (ed->mouse_index < 3) return;
  ed->mouse_index = 0;
  uint8_t flags = ed->mouse_bytes[0];
  if (flags & 0xC0) return;
  ed->mouse_x += ed->mouse_bytes[1] - ((flags & BIT(4)) ? 256 : 0);
  ed->mouse_y -= ed->mouse_bytes[2] - ((flags & BIT(5)) ? 256 : 0);
  if (ed->mouse_x < 0) ed->mouse_x = 0;
  if (ed->mouse_x > 799) ed->mouse_x = 799;
  if (ed->mouse_y < 0) ed->mouse_y = 0;
  if (ed->mouse_y > 599) ed->mouse_y = 599;
  bool left = flags & 1, pressed = left && !(ed->mouse_buttons & 1);
  ed->mouse_buttons = flags & 7;
  if (!left) ed->dragging = false;
  if (ed->help) { if (pressed && in_rect(ed->mouse_x, ed->mouse_y, 606, 124, 42, 32)) ed->help = false; return; }
  if (ed->close_prompt) {
    if (!pressed) return;
    if (in_rect(ed->mouse_x, ed->mouse_y, 178, 333, 140, 40)) { save_document(ed); if (!ed->dirty) *running = false; }
    else if (in_rect(ed->mouse_x, ed->mouse_y, 330, 333, 140, 40)) *running = false;
    else if (in_rect(ed->mouse_x, ed->mouse_y, 482, 333, 140, 40)) ed->close_prompt = false;
    return;
  }
  if (pressed) {
    if (in_rect(ed->mouse_x, ed->mouse_y, 24, 78, 96, 34)) save_document(ed);
    else if (in_rect(ed->mouse_x, ed->mouse_y, 130, 78, 86, 34)) history_move(ed, false);
    else if (in_rect(ed->mouse_x, ed->mouse_y, 224, 78, 86, 34)) history_move(ed, true);
    else if (in_rect(ed->mouse_x, ed->mouse_y, 650, 78, 126, 34)) ed->help = true;
    else { place_caret(ed, ed->modifiers.shift != 0); ed->dragging = true; }
  } else if (left && ed->dragging) place_caret(ed, true);
}

static void draw_visible_text(example_canvas_t *ctx, const editor_state_t *ed, uint32_t frame,
                              int doc_x, int doc_y) {
  const int char_w = 9;
  const int line_h = 20;
  uint16_t line = 0;
  uint16_t col = 0;
  for (size_t i = 0; i < ed->len; i++) {
    if (ed->text[i] == '\n') {
      line++;
      col = 0;
      continue;
    }
    if (line >= ed->scroll_line && line < ed->scroll_line + EDITOR_VISIBLE_ROWS) {
      size_t first = ed->anchor < ed->cursor ? ed->anchor : ed->cursor;
      size_t last = ed->anchor > ed->cursor ? ed->anchor : ed->cursor;
      if (i >= first && i < last) example_fill_rect(ctx, doc_x + col * char_w, doc_y + (line - ed->scroll_line) * line_h - 1, 9, 20, 0xCBE5DD);
      char one[2] = {ed->text[i], 0};
      example_mono(ctx, doc_x + col * char_w,
                        doc_y + (line - ed->scroll_line) * line_h,
                        one, UI_INK);
    }
    if (++col >= EDITOR_WRAP_COLS) {
      line++;
      col = 0;
    }
  }
  visual_pos_t cursor;
  visual_position_at(ed, ed->cursor, &cursor);
  if (((frame / 18u) % 2u) == 0 && cursor.line >= ed->scroll_line &&
      cursor.line < ed->scroll_line + EDITOR_VISIBLE_ROWS) {
    int x = doc_x + cursor.col * char_w;
    int y = doc_y + (cursor.line - ed->scroll_line) * line_h;
    example_fill_rect(ctx, x, y, ed->overwrite ? 11 : 2, 16, example_rgb(58, 104, 181));
  }
}

static void draw_help(example_canvas_t *ctx) {
  example_round_rect(ctx, 140, 112, 520, 380, 10, UI_DARK);
  example_label(ctx, 168, 134, "Writing shortcuts", UI_LIGHT, 2);
  example_button(ctx, 606, 124, 42, 32, "X", false, false, true);
  const char *left[] = {"Cmd/Ctrl+S", "Cmd/Ctrl+Z", "Cmd/Ctrl+Shift+Z", "Cmd/Ctrl+A", "Cmd/Ctrl+C / X / V", "Shift+arrows / drag", "Home / End", "F1 / Esc"};
  const char *right[] = {"Save document", "Undo", "Redo (also Ctrl+Y)", "Select all", "Copy / cut / paste text", "Select text", "Start / end of line", "Close help"};
  for (int i = 0; i < 8; i++) {
    example_label(ctx, 170, 198 + i * 30, left[i], UI_LIGHT, 1);
    example_label(ctx, 364, 198 + i * 30, right[i], UI_DIM, 1);
  }
}

static void draw_word_processor(example_canvas_t *ctx, const editor_state_t *ed, uint32_t frame) {
  example_fill_rect(ctx, 0, 0, 800, 600, 0xEAEDE9);
  example_fill_rect(ctx, 0, 0, 800, 122, UI_PAPER);
  example_label(ctx, 24, 14, "Notes", UI_INK, 2);
  const char *filename = strrchr(ed->path, '/'); filename = filename ? filename + 1 : ed->path;
  char short_name[49]; snprintf(short_name, sizeof(short_name), "%.45s", filename);
  example_label(ctx, 26, 48, short_name, UI_MUTED, 1);
  example_round_rect(ctx, 632, 26, 144, 28, 5, ed->dirty ? 0xF5EACD : 0xE3EEE8);
  example_label(ctx, 644, 31, ed->dirty ? "Unsaved changes" : "No unsaved edits", ed->dirty ? 0x71521C : UI_ACCENT, 1);
  example_button(ctx, 24, 78, 96, 34, "Save", true, in_rect(ed->mouse_x, ed->mouse_y,24,78,96,34), !ed->save_blocked);
  example_button(ctx, 130, 78, 86, 34, "Undo", false, in_rect(ed->mouse_x,ed->mouse_y,130,78,86,34), ed->history_pos > 0 || (ed->history_len && strcmp(ed->history[ed->history_pos].text, ed->text)));
  example_button(ctx, 224, 78, 86, 34, "Redo", false, in_rect(ed->mouse_x,ed->mouse_y,224,78,86,34), ed->history_pos + 1 < ed->history_len);
  example_label(ctx, 334, 87, "Cmd/Ctrl+S", UI_MUTED, 1);
  example_button(ctx, 650, 78, 126, 34, "Shortcuts  F1", false, in_rect(ed->mouse_x,ed->mouse_y,650,78,126,34), true);
  example_round_rect(ctx, 42, 138, 716, 394, 4, UI_PAPER);
  example_fill_rect(ctx, 62, 154, 1, 360, 0xE3E9E4);
  if (!ed->len) {
    example_label(ctx, 76, 154, "Start writing...", UI_MUTED, 1);
    example_label(ctx, 76, 194, "A note, a plan, a thought worth keeping.", UI_MUTED, 1);
  }
  draw_visible_text(ctx, ed, frame, 76, 154);
  uint16_t lines = total_visual_lines(ed);
  if (lines > EDITOR_VISIBLE_ROWS) {
    int thumb_h = (int)(360u * EDITOR_VISIBLE_ROWS / lines);
    int thumb_y = 154 + (int)(360u * ed->scroll_line / lines);
    example_round_rect(ctx, 742, 154, 4, 360, 2, 0xE3E9E4);
    example_round_rect(ctx, 742, thumb_y, 4, thumb_h, 2, 0x8CA59A);
  }
  visual_pos_t cursor; visual_position_at(ed, ed->cursor, &cursor);
  char stats[100];
  snprintf(stats, sizeof(stats), "%u words   %u characters", word_count(ed), (unsigned)ed->len);
  example_label(ctx, 44, 546, stats, UI_MUTED, 1);
  snprintf(stats, sizeof(stats), "Ln %u, Col %u   %s", cursor.line + 1u, cursor.col + 1u, ed->overwrite ? "Overwrite" : "Insert");
  example_label(ctx, 756 - example_label_width(stats, 1), 546, stats, UI_MUTED, 1);
  example_fill_rect(ctx, 0, 574, 800, 26, UI_PAPER);
  example_label(ctx, 24, 578, ed->status, ed->save_blocked ? 0xA13D34 : UI_ACCENT, 1);
  if (ed->help) draw_help(ctx);
  if (ed->close_prompt) {
    example_round_rect(ctx, 154, 214, 492, 178, 10, UI_DARK);
    example_label(ctx, 178, 238, "Save your changes?", UI_LIGHT, 2);
    example_label(ctx, 178, 286, "Your document has changes that are not saved.", UI_DIM, 1);
    example_button(ctx, 178, 333, 140, 40, "Save  Enter", true, false, true);
    example_button(ctx, 330, 333, 140, 40, "Discard  D", false, false, true);
    example_button(ctx, 482, 333, 140, 40, "Cancel  Esc", false, false, true);
  }
  if (ed->mouse_ack) {
    int x = ed->mouse_x, y = ed->mouse_y;
    for (int i = 0; i < 12; i++) example_fill_rect(ctx, x, y + i, i / 2 + 1, 1, UI_INK);
  }
}

int main(int argc, char **argv) {
  if (lcom_init() != LCOM_OK) return 1;
  lcom_vbe_mode_info_t info;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &info) != LCOM_OK) return 1;
  if (lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK) return 1;
  uint8_t *fb = 0;
  if (lcom_phys_map(info.framebuffer_phys, info.framebuffer_size, (void **)&fb) != LCOM_OK) return 1;
  lcom_irq_t timer_irq;
  lcom_irq_t kbd_irq;
  lcom_irq_t mouse_irq;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) return 1;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &kbd_irq) != LCOM_OK) return 1;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &mouse_irq) != LCOM_OK) return 1;
  lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE);
  lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_ENABLE_DR);
  example_canvas_t canvas = {fb, info};
  editor_state_t editor = {0};
  editor.path = argc > 1 ? argv[1] : "build/lcom-notes.txt";
  set_status(&editor, "Ready to write");
  open_document(&editor);
  memcpy(editor.saved_text, editor.text, editor.len + 1);
  remember_undo(&editor);
  editor.mouse_x = 400; editor.mouse_y = 300;
  uint32_t frame = 0;
  bool running = true;
  draw_word_processor(&canvas, &editor, frame);
  lcom_vbe_present();
  while (running) {
    lcom_event_t ev;
    if (lcom_event_wait(&ev) != LCOM_OK) break;
    if ((ev.irq_mask & timer_irq.mask) != 0) {
      frame++;
      draw_word_processor(&canvas, &editor, frame);
      lcom_vbe_present();
    }
    if ((ev.irq_mask & mouse_irq.mask) != 0) {
      uint8_t status, byte;
      while (lcom_port_read8(KBC_ST_REG, &status) == LCOM_OK && (status & KBC_ST_OBF) && (status & KBC_ST_AUX) && lcom_port_read8(KBC_OUT_BUF, &byte) == LCOM_OK) handle_mouse_byte(&editor, byte, &running);
      draw_word_processor(&canvas, &editor, frame); lcom_vbe_present();
    }
    if ((ev.irq_mask & kbd_irq.mask) != 0) {
      uint8_t data = 0;
      if (read_keyboard_byte(&data) == LCOM_OK) {
        handle_keyboard_byte(&editor, data, &running);
        draw_word_processor(&canvas, &editor, frame);
        lcom_vbe_present();
      }
    }
  }
  draw_word_processor(&canvas, &editor, frame);
  lcom_vbe_present();
  lcom_printf("word processor text %s\n", editor.text);
  lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE);
  lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_DISABLE_DR);
  lcom_irq_unsubscribe(&mouse_irq);
  lcom_irq_unsubscribe(&kbd_irq);
  lcom_irq_unsubscribe(&timer_irq);
  lcom_phys_unmap(fb, info.framebuffer_size);
  lcom_exit();
  return 0;
}
