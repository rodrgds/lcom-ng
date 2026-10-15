#include "animatedSprite.h"

struct animatedSprite_imp {
  uint16_t width, height; // dimensions
  uint8_t **map;
  uint8_t aspeed; // number of frames per animation
  uint8_t num_images; // number of images in the animation
};

uint16_t get_anim_sprite_width(AnimSprite *sp) {
  if (sp == NULL) {
    return 0;
  }
  return sp->width;
}

uint16_t get_anim_sprite_height(AnimSprite *sp) {
  if (sp == NULL) {
    return 0;
  }
  return sp->height;
}

uint8_t get_anim_sprite_aspeed(AnimSprite *sp) {
  if (sp == NULL) {
    return 0;
  }
  return sp->aspeed;
}

uint8_t get_anim_sprite_num_images(AnimSprite *sp) {
  if (sp == NULL) {
    return 0;
  }
  return sp->num_images;
}

AnimSprite *create_anim_sprite(uint8_t anim_speed, uint8_t no_pic, const xpm_map_t pic1, ...) {
  // allocate space for the "object"
  AnimSprite *asp = malloc(sizeof(AnimSprite));

  // allocate array of pointers to pixmaps
  asp->map = malloc(no_pic * sizeof(char *));

  xpm_image_t img;
  asp->map[0] = xpm_load(pic1, XPM_8_8_8, &img);
  if (asp->map[0] == NULL) {
      free(asp->map);
      free(asp);
      return NULL;
  }
  asp->width = img.width;
  asp->height = img.height;
  asp->num_images = no_pic;
  asp->aspeed = anim_speed;

  // load pixmaps
  va_list ap;
  va_start(ap, pic1);
  for (int i = 1; i < no_pic; i++) {
      const xpm_map_t tmp = va_arg(ap, xpm_map_t);
      asp->map[i] = xpm_load(tmp, XPM_8_8_8, &img);
      if (asp->map[i] == NULL || img.width != asp->width || img.height != asp->height) {
          for (int j = 0; j < i; j++) {
            free(asp->map[j]);
          }
          free(asp->map);
          free(asp);
          va_end(ap);
          return NULL;
      }
  }
  va_end(ap);
  return asp;
}


void destroy_anim_sprite(AnimSprite *sp) {
  if( sp == NULL ){
    return;
  }
  for(int i = 0; i < sp->num_images; i++){
    if( sp->map[i] ){
      free(sp->map[i]);
    }
  }
  free(sp->map);
  free(sp);
  sp = NULL; 
}

void draw_anim_sprite(AnimSprite *sp, uint16_t x, uint16_t y, uint8_t current_image) {
  if (sp == NULL) {
    return;
  }
  vg_draw_xpm(sp->map[current_image],x,y ,sp->width, sp->height);
}
