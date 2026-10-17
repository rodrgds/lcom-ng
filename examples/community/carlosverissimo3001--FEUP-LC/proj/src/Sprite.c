#include <lcom/lcf.h>


#include "Sprite.h"

extern xpm_image_t frame;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed) {
  // allocate space for the "object"
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  // xpm_image_t img;

  if (sp == NULL)
    return NULL;

  // read the sprite pixmap
  sp->map = xpm_load(xpm, XPM_8_8_8_8, &(sp->img));

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->x = x, sp->y = y;
  sp->xspeed = xspeed, sp->yspeed = yspeed;
  return sp;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL)
    return;
  if (sp->map)
    free(sp->map);
  free(sp);
  sp = NULL;
}

int draw_sprite(Sprite *sp, bool addToBackgroundFrame) {
  draw_pixmap(sp->x, sp->y, &(sp->img), addToBackgroundFrame);
  return 0;
}




