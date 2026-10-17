#include "animation.h"

AnimSprite *create_animSprite(sprite_t *sprite, uint32_t num_sprites, uint32_t lin_count, uint32_t hsize, uint32_t vsize) {

  AnimSprite *asp = malloc(sizeof(AnimSprite));

  asp->sp = sprite;

  asp->num_fig = num_sprites;
  asp->num_fig_line = lin_count;

  asp->hsize = hsize;
  asp->vsize = vsize;

  asp->cur_fig = 0;

  return asp;
}

int draw_animSprite(AnimSprite *animSprite, uint8_t num_fig, int x, int y) {
  if (!animSprite || !animSprite->sp || !animSprite->sp->map ||
      !animSprite->num_fig_line || num_fig == 0 || num_fig > animSprite->num_fig)
    return EXIT_FAILURE;

  sprite_t *sprite = animSprite->sp;
  unsigned frame = num_fig - 1;
  unsigned col_pos = frame % animSprite->num_fig_line;
  unsigned line_pos = frame / animSprite->num_fig_line;
  size_t left = (size_t) col_pos * animSprite->hsize;
  size_t top = (size_t) line_pos * animSprite->vsize;
  if (left + animSprite->hsize > sprite->w || top + animSprite->vsize > sprite->h)
    return EXIT_FAILURE;

  uint32_t *map = (uint32_t *) sprite->map;
  for (unsigned height = 4; height + 3 < animSprite->vsize; height++) {
    uint32_t *clr = map + (top + height) * sprite->w + left;
    for (unsigned width = 0; width < animSprite->hsize; width++) {
      if (!(*clr & 0xff000000))
        fill_pixel(x + width, y + height, *clr);
      clr++;
    }
  }

  return EXIT_SUCCESS;
}
