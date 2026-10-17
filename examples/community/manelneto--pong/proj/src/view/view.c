#include <lcom/lcf.h>

#include "view.h"

#include "model/model.h"

#include "game_view.h"
#include "levels_view.h"
#include "menu_view.h"

#include "../controller/video.h"

#define BACKGROUND_COLOR 0xADD8E6

extern State state;

int draw_frame() {
  if (video_draw_background(BACKGROUND_COLOR)) {
    printf("%s: video_draw_background(BACKGROUND_COLOR: 0x%x) error\n", __func__, BACKGROUND_COLOR);
    return 1;
  }

  if (state == MENU && draw_menu()) {
    printf("%s: draw_menu() error\n", __func__);
    return 1;
  }
  else if (state == LEVELS && draw_levels()) {
    printf("%s: draw_levels() error\n", __func__);
    return 1;
  }
  else if (state == GAME && draw_game()) {
    printf("%s: draw_game() error\n", __func__);
    return 1;
  }

  return 0;
}
