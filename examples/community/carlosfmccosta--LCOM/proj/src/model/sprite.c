/**
 * @file sprite.c
 * @brief Implementation of sprite management.
 *
 * This file contains the implementation of the functions declared in sprite.h.
 */
#include "sprite.h"

struct sprite {
  position_t *pos;   /**< Pointer to the sprite's position (x, y). */
  unsigned int curr; /**< Current animation image index. */
  unsigned int size; /**< Number of animation images. */
  bool flip;         /**< Boolean to keep track of the direction. */
  xpm_image_t *imgs; /**< Array of loaded XPM images. */
};

/* Constructor */
sprite_t *create_sprite(xpm_map_t pics[], unsigned int size, uint16_t x, uint16_t y) {
  if (size == 0)
    return NULL;

  /* Create the new sprite object */
  sprite_t *sp = malloc(sizeof(sprite_t));
  if (!sp) {
    return NULL;
  }

  /* Define the position */
  position_t *pos = malloc(sizeof(position_t));
  if (!pos) {
    free(sp);
    return NULL;
  }

  /* Get the XPMs */
  xpm_image_t *imgs_local = malloc(size * sizeof(xpm_image_t));
  if (!imgs_local) {
    free(pos);
    free(sp);
    return NULL;
  }

  for (unsigned int i = 0; i < size; i++) {
    uint8_t *sprite_data = xpm_load(pics[i], XPM_INDEXED, &imgs_local[i]);
    if (!sprite_data) {
      for (unsigned int j = 0; j < i; j++) {
        free(imgs_local[j].bytes);
      }
      free(imgs_local);
      free(pos);
      free(sp);
      return NULL;
    }
    imgs_local[i].bytes = sprite_data;
  }

  /* Assign Data to sprite */
  sp->imgs = imgs_local;
  sp->pos = pos;
  sp->pos->x = x;
  sp->pos->y = y;
  sp->curr = 0;
  sp->size = size;
  sp->flip = false;

  return sp;
}

/* Destructor */
void destroy_sprite(sprite_t *sp) {
  if (!sp)
    return;

  for (unsigned int i = 0; i < sp->size; i++) {
    free(sp->imgs[i].bytes);
  }

  free(sp->pos);
  free(sp->imgs);
  free(sp);
  sp = NULL;
}

/* Methods */
int draw_sprite(sprite_t *sp) {
  if (!sp) {
    return 1;
  }
  if (sp->curr >= sp->size) {
    sp->curr = 0;
  }
  return draw_xpm(sp->imgs[sp->curr], sp->pos->x, sp->pos->y, sp->flip);
}

int draw_sprite_animate(sprite_t *sp) {
  if (sp == NULL) {
    return 1;
  }
  if (sp->curr >= sp->size) {
    sp->curr = 0;
  }

  // Draw the current frame
  int ret = draw_xpm(sp->imgs[sp->curr], sp->pos->x, sp->pos->y, sp->flip);

  sp->curr++;
  return ret;
}

/* Getters */
position_t *get_sprite_pos(sprite_t *sp) {
  return sp->pos;
}

uint16_t get_sprite_height(sprite_t *sp) {
  return sp->imgs[sp->curr].height;
}

uint16_t get_sprite_width(sprite_t *sp) {
  return sp->imgs[sp->curr].width;
}

unsigned int get_sprite_curr(sprite_t *sp) {
  return sp->curr;
}

unsigned int get_sprite_size(sprite_t *sp) {
  return sp->size;
}

/* Setters */
void set_sprite_curr(sprite_t *sp, unsigned int curr) {
  sp->curr = curr;
}

void set_sprite_flip(sprite_t *sp, bool flip) {
  sp->flip = flip;
}
