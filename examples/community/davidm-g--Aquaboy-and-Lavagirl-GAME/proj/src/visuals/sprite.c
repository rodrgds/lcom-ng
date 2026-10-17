#include "sprite.h"

uint16_t get_posx(Sprite *sp) {
  return sp->x;
}

uint16_t get_posy(Sprite *sp) {
  return sp->y;
}

void set_posx(Sprite *sp, uint16_t x) {
  sp->x = x;
}

void set_posy(Sprite *sp, uint16_t y) {
  sp->y = y;
}

Sprite *create_sprite(xpm_map_t map, uint16_t x, uint16_t y, int16_t xspeed, int16_t yspeed) {
  xpm_image_t img;
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  sp->isOnGround = false;
  sp->xaccell = 0;
  sp->map = (uint32_t *) xpm_load(map, XPM_8_8_8_8, &img);
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }
  sp->width = img.width;
  sp->height = img.height;
  return sp;
}
void destroy_sprite(Sprite *sp) {
  if (sp == NULL) {
    return;
  }
  if (sp->map) {
    free(sp->map);
  }
  free(sp);
  sp = NULL;
  return;
}
