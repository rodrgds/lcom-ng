#include "game_view.h"

extern Sprite *cursor;
extern Sprite *bomb;

extern int cursor_x;
extern int cursor_y;

extern ArenaPhase arena_phase;


void draw_cursor(uint8_t *buffer) {
  if (get_state() == ARENA && arena_phase == BATTLE_PHASE) draw_sprite(bomb, cursor_x, cursor_y, buffer);
  else draw_sprite(cursor, cursor_x, cursor_y, buffer);
}






