#include <lcom/lcf.h>
#include "sprite.h"
#include "video.h"
#include "helpers.h"

Sprite* create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed) {
  // Allocates space for the "object"
  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));

  if(sp == NULL)
    return NULL;

  // Reads the sprite's pixmap
  sp->map = xpm_load(xpm, XPM_8_8_8_8, &(sp->img));
  
  if(sp->map == NULL) {
    free(sp);
    return NULL;
  }
  
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;

  return sp;
}

void update_position(Sprite* sp, int new_x, int new_y){
  sp->x = new_x;
  sp->y = new_y;
}

int update_xpm(Sprite* sp, xpm_map_t xpm){
  if(sp == NULL)
    return 1;
    
  if(sp->map != NULL) free(sp->map);

  sp->map = xpm_load(xpm, XPM_8_8_8_8, &(sp->img));

  if(sp->map == NULL) {
    free(sp);
    return 1;
  }
  return 0;
}

int draw_sprite(Sprite* sp) {

  if (sp->x >= (int)get_hres() || sp->y >= (int)get_vres() || sp->x < 0 || sp->y < 0)
    return 0;

  uint8_t *map = sp->map;
  unsigned bpp = get_bytesperpixel();
  uint32_t color;

  for (int j = 0; j < sp->img.height; j++) {
    for (int i = 0; i < sp->img.width; i++) {
      color = 0;

      for (unsigned k = 0; k < bpp; k++) {
        color += *(map + k) << k * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + j, color))
        return 1;

      map += bpp;
    }
  }

  return 0;
}


void update_sprite(Sprite* sp) {
  sp->x += sp->xspeed;
  sp->y += sp->yspeed;
}

void release_sprite(Sprite *sp) {
  if(sp == NULL)
    return;

  if(sp->map)
    free(sp->map);
  
  free(sp);
}
