#include <lcom/lcf.h>

#include "wall.h"

Wall *construct_wall(int16_t x, int16_t y, uint8_t w, uint8_t h) {
  Wall *wall = (Wall *) malloc(sizeof(Wall));

  if (!wall) {
    printf("%s: malloc() error\n", __func__);
    return NULL;
  }

  wall->x = x;
  wall->y = y;
  wall->w = w;
  wall->h = h;

  return wall;
}

int move_wall_up(Wall *wall) {
  if (!wall) {
    printf("%s: wall is NULL\n", __func__);
    return 1;
  }

  wall->y -= 20;

  if (wall->y < 0)
    wall->y = 0;

  return 0;
}

int move_wall_down(Wall *wall, uint16_t y_max) {
  if (!wall) {
    printf("%s: wall is NULL\n", __func__);
    return 1;
  }

  wall->y += 20;

  if (wall->y + wall->h >= y_max)
    wall->y = y_max - wall->h - 1;

  return 0;
}

void destroy_wall(Wall *wall) {
  if (wall)
    free(wall);
}
