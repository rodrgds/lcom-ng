#include "sprite.h"


Sprite *create_sprite(xpm_map_t pic){

  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if( sp == NULL ) return NULL;

  xpm_image_t img;
  sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);
  if( sp->map == NULL ) {
    free(sp);
    return NULL;
  }
  sp->width = img.width;
  sp->height = img.height;
  return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
    if( sp ->map )
      free(sp->map);
    free(sp);
    sp = NULL; // XXX: pointer is passed by value
    // should do this @ the caller
}

