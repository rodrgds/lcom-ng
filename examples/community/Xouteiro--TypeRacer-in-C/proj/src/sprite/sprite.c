#include "sprite.h"

/**
 * @brief Creates a sprite object with the given xpm
 * @param sprite xpm to be used
 * @return pointer to the created sprite, NULL otherwise
*/
Sprite* (sprite_create_xpm)(xpm_map_t sprite){
  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if( sp == NULL ) return NULL;

  xpm_image_t img;
  sp->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
  sp->height = img.height;
  sp->width = img.width;

  if( sp->colors == NULL ) {
    free(sp);
    return NULL;
  }
  return sp;
}

/**
 * @brief Creates a sprite object to be used as a button
 * @param width width of the button
 * @param height height of the button
 * @param color color of the button
 * @return pointer to the created sprite, NULL otherwise
*/
Sprite* (sprite_create_button)(uint16_t width, uint16_t height, uint32_t color){
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  sp->height = height;
  sp->width = width;
  sp->colors = NULL;
  sp->color = color;
  sp->pressed = 0;
  return sp;
}

/**
 * @brief Destroys a sprite object
 * @param sprite sprite to be destroyed
*/
void (sprite_destroy)(Sprite *sprite) {
    if (sprite == NULL) return;
    if (sprite->colors) free(sprite->colors);
    free(sprite);
    sprite = NULL;
}
