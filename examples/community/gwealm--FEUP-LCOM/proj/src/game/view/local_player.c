#include "game/view/local_player.h" 

#include "graphics/graphics.h"

#include "game/layout.h"
#include "game/model/local_player.h"

void local_player_draw() {
  local_player *p = local_player_get();

  size_t curr = 0;
  size_t size = COLS * ROWS;

  uint16_t ix = GAME_BOARD_X, iy = GAME_BOARD_Y;
  uint16_t rx = ix, ry = iy;
  for (curr = 0; curr < size;) {
    graphics_draw_square(rx, ry, p->background[curr]);
    graphics_draw_char(rx + GAME_CELL_PADDING_X, ry + GAME_CELL_PADDING_Y, p->board[curr]);

    rx += GAME_CELL_WIDTH + GAME_BOARD_SPACING_X;
    curr++;
    if (curr % COLS == 0) {
      rx = ix;
      ry += GAME_CELL_HEIGHT + GAME_BOARD_SPACING_Y;
    }
  }
}
