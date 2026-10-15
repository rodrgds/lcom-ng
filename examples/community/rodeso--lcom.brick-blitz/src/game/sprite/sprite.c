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



int drawSprite(Sprite *sprite, int xPos, int yPos) {
    uint32_t current_color;
    for (int i = 0 ; i < sprite->height; i++) {
      for (int j = 0 ; j < sprite->width; j++) {
          current_color = sprite->map[j + i*sprite->width];
          if (current_color == 0x505050) {continue;}
          if (vbe_draw_pixel(xPos + j, yPos + i, current_color)) return 1;
      }
    }
    return 0; 
}
int eraseSprite(Sprite *sprite, int xPos, int yPos) {
    uint32_t current_color = 0x3A0D56; //insert background colour (purple in this case :D);
    for (int i = 0 ; i < sprite->height; i++) {
      for (int j = 0 ; j < sprite->width; j++) {
          if (vbe_draw_pixel(xPos + j, yPos + i, current_color)) return 1;
      }
    }
    return 0; 
}
