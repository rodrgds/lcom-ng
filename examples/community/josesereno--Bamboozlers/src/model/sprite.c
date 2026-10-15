#include "sprite.h"


Sprite *create_sprite_xpm(xpm_map_t *sprites, int num_sprites, int x, int y){

  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if (sp == NULL) {
    return NULL;
  }

  sp->width = (int *) malloc(sizeof(int) * num_sprites);
  sp->height = (int *) malloc(sizeof(int) * num_sprites);
  sp->pixmap_array = (uint32_t **) malloc(sizeof(uint32_t *) * num_sprites);
  
  sp->x = x;
  sp->y = y;
  sp->xspeed = 0;
  sp->yspeed = 0;
  sp->num_pixmaps = num_sprites;
  sp->i = 0;
 
  if (sp->pixmap_array == NULL) {
    free(sp);
    return NULL;
  }

  for (int i = 0; i < num_sprites; i++) {
    xpm_image_t img;
    memset(&img, 0, sizeof(img));
    sp->pixmap_array[i] = (uint32_t *) xpm_load(sprites[i], XPM_8_8_8_8, &img);
    if (sp->pixmap_array[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(sp->pixmap_array[j]);
      }
      free(sp->pixmap_array);
      free(sp);
      return NULL;
    }
    sp->width[i] = img.width;
    sp->height[i] = img.height;
   
  }
  
  return sp;

}

void destroy_sprite(Sprite *sprite) {
    if (sprite == NULL) return;
    if (sprite->pixmap_array != NULL) {
        for (int i = 0; i < sprite->num_pixmaps; i++) {
            free(sprite->pixmap_array[i]);
        }
        free(sprite->pixmap_array);
    }
    free(sprite);
    sprite = NULL;
}

void setPos(Sprite* sprite, int x, int y){
  sprite->x = x;
  sprite->y = y;
}

