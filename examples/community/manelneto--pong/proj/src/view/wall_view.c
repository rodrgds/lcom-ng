#include <lcom/lcf.h>

#include "wall_view.h"

#include "../controller/video.h"

#define WALL_COLOR 0xFFFF00

int draw_wall(Wall *wall) {
  if (!wall) {
    printf("%s: wall is NULL\n", __func__);
    return 1;
  }

  if (video_draw_rectangle(wall->x, wall->y, wall->w, wall->h, WALL_COLOR)) {
    printf("%s: video_draw_rectangle(wall->x; %d, wall->y: %d, wall->w: %d, wall->h: %d, WALL_COLOR: 0x%x) error\n", __func__, wall->x, wall->y, wall->w, wall->h, WALL_COLOR);
    return 1;
  }

  return 0;
}
