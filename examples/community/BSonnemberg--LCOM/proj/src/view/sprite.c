#include "sprite.h"

sprite* create_sprite(xpm_map_t map) {
  
  sprite *sp = (sprite*) malloc(sizeof(sprite));
  if (sp == NULL) return NULL;
  
  xpm_image_t img;
  sp->map = xpm_load(map, XPM_INDEXED, &img);
  
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->width = img.width;
  sp->height = img.height;

  return sp;
}

void destroy_sprite(sprite *sp) {
  if (sp == NULL) return;
  if (sp->map != NULL) {
    free(sp->map);
  }
  free(sp);
  sp = NULL;
}
