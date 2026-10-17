#include "game/view/game.h"

#include "graphics/graphics.h"

#include "game/layout.h"
#include "game/model/game.h"
#include "game/view/local_player.h"

void (game_draw)() {
  game_draw_timer();
  game_draw_title();

  local_player_draw();
}

void (game_draw_title)() {
  graphics_draw_text(GAME_TITLE_X, GAME_TITLE_Y, GAME_TITLE);
}

void (game_draw_timer)() {
  game *g = game_get();
  unsigned long game_seconds = g->ellapsed_time;
  unsigned long minutes = game_seconds / 60;
  unsigned long seconds = game_seconds % 60;

  char timer[6] = { 0 };

  timer[1] = (minutes % 10) + '0';
  minutes /= 10;
  timer[0] = (minutes % 10) + '0';

  timer[2] = ':';

  timer[4] = (seconds % 10) + '0';
  seconds /= 10;
  timer[3] = (seconds % 10) + '0';

  graphics_draw_small_text(GAME_TIMER_X, GAME_TIMER_Y, timer);
}
