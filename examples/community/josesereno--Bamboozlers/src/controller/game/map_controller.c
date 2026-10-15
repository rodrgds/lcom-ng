#include "map_controller.h"

bool (is_in_map)(Sprite *item) {
  return item->x + item->width[item->i] <= 15 + MAP_WIDTH && item->x >= 25 &&
         item->y + item->height[item->i] <= 20 + MAP_HEIGHT && item->y >= 25;
}

bool (collide)(Sprite *s1, Sprite *s2) {
  if (s1 == s2)
    return false;

  uint16_t left = (s1->x > s2->x) ? s1->x : s2->x;
  uint16_t right = (s1->x + s1->width[s1->i] < s2->x + s2->width[s2->i]) ? s1->x + s1->width[s1->i] : s2->x + s2->width[s2->i];
  uint16_t top = (s1->y > s2->y) ? s1->y : s2->y;
  uint16_t bottom = (s1->y + s1->height[s1->i] < s2->y + s2->height[s2->i]) ? s1->y + s1->height[s1->i] : s2->y + s2->height[s2->i];

  for (int y = top; y < bottom; ++y) {
    for (int x = left; x < right; ++x) {
      uint32_t s1_color = s1->pixmap_array[s1->i][x - s1->x + (y - s1->y) * s1->width[s1->i]];
      uint32_t s2_color = s2->pixmap_array[s2->i][x - s2->x + (y - s2->y) * s2->width[s2->i]];
      if (s1_color != TRANSPARENT && s2_color != TRANSPARENT) return true;
    }
  }

  return false;
}

void (map_set_up)(int level) {
  panda_state = INACTIVE;
  panda->i = big_plank->i = little_plank->i = counter = 0;
  minutes = 1;
  seconds = 30;
  setPos(little_block, 282, 520);
  setPos(big_block, 364, 512);
  setPos(little_plank, 455, 535);
  setPos(big_plank, 582, 535);
  isRightPressed = isLeftPressed = false;
  map->i = lava->i = level;

  switch (level) {
    case 0:
      setPos(panda, 40, 100);
      setPos(home, 370, 50);
      setPos(bamboo, 370, 350);
      break;
    case 1:
      setPos(panda, 40, 100);
      setPos(home, 530, 330);
      setPos(bamboo, 580, 90);
      break;
    case 2:
      setPos(panda, 230, 100);
      setPos(home, 405, 307);
      setPos(bamboo, 320, 310);
      break;
  }

}
