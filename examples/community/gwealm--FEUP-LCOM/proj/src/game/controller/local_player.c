#include "game/controller/local_player.h"

#include "game/wordlist.h"
#include "game/model/game.h"

void (local_player_start)() {
  local_player_reset();
  
  event_on(MOUSE_CLICK, local_player_handle_mouse_click);
  event_on(KEY_PRESS, local_player_handle_keypress);
}

void (local_player_end)() {
  event_unregister(KEY_PRESS, local_player_handle_keypress);
  event_unregister(MOUSE_CLICK, local_player_handle_mouse_click);
}

void (local_player_write_char)(char c) {
  local_player *p = local_player_get();
  if (p->x == COLS) {
      return;
  }

  local_player_write(c);
  local_player_forward();
}

void (local_player_remove_char)() {
  local_player *p = local_player_get();
  if (local_player_read_after() != '\0') {
      return;
  }

  if (local_player_read() != '\0') {
      local_player_write('\0');
      return;
  }

  if (p->x > 0) {
      local_player_backward();
      local_player_write('\0');
  }
}

void (local_player_select_cell)(uint8_t x, uint8_t y) {
    local_player *p = local_player_get();
    if (x >= COLS || y != p->y) {
        return;
    }

    uint8_t prev_x = p->x;
    local_player_goto(x, p->y);
    
    bool is_blank = local_player_read() == '\0';
    bool is_blank_before = x == 0 || local_player_read_before() == '\0';

    // two or more characters are blank before the selected cell
    if (is_blank_before && is_blank) {
      local_player_goto(prev_x, p->y);
    }
}

local_player_submission_status (local_player_submit_word)(bool (*verifier)(char*, cell_background*)) {
    if (local_player_read_last_of_row() == '\0') {
        return TOO_SHORT;
    }

    local_player *p = local_player_get();
    char* word = p->board + (COLS * p->y);
    if (!game_wordlist_contains(word)) {
        return NOT_IN_WORDLIST;
    }

    cell_background *line = p->background + (COLS * p->y);
    if (verifier(word, line) || p->y >= ROWS - 1) {
        return FINISHED;
    }

    local_player_downward();
    return SUBMITTED;
}

void (local_player_handle_keypress)(struct event *ev) {
  game *g = game_get(); 
  if (!g->running) {
    return;
  }

  char ch = *(char*) ev->data;

  if (ch == '\b') {
    local_player_remove_char();
  } else if (ch >= 'a' && ch <= 'z') {
    local_player_write_char(ch);
  }
}

void (local_player_handle_mouse_click)(struct event *ev) {
  game *g = game_get(); 
  if (!g->running) {
    return;
  }

  uint16_t *pos = ev->data;
  
  uint16_t x = pos[0];
  uint16_t y = pos[1];

  if (x < GAME_BOARD_X || y < GAME_BOARD_Y) {
    return;
  }

  x -= GAME_BOARD_X;
  y -= GAME_BOARD_Y;

  uint16_t board_width = (GAME_CELL_WIDTH + GAME_BOARD_SPACING_X) * COLS - GAME_BOARD_SPACING_X; 
  uint16_t board_height = (GAME_CELL_HEIGHT + GAME_BOARD_SPACING_Y) * ROWS - GAME_BOARD_SPACING_Y;

  if (x >= board_width || y >= board_height) {
    return;
  }

  uint8_t cell_x = x % (GAME_CELL_WIDTH + GAME_BOARD_SPACING_X);
  uint8_t cell_y = y % (GAME_CELL_HEIGHT + GAME_BOARD_SPACING_Y);

  if (cell_x >= GAME_CELL_WIDTH || cell_y >= GAME_CELL_HEIGHT) {
    return;
  }

  uint8_t col = x / (GAME_CELL_WIDTH + GAME_BOARD_SPACING_X);
  uint8_t row = y / (GAME_CELL_HEIGHT + GAME_BOARD_SPACING_Y);

  local_player_select_cell(col, row);
}
