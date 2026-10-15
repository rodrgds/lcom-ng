#include <lcom/lcf.h>

#include "model/sprite/anim_sprite.h"

AnimSprite *create_animSprite(Sprite *const *frames, uint8_t nframes, uint16_t ticks_per_frame, bool loop) {
  AnimSprite *asp = (AnimSprite *) malloc(sizeof(AnimSprite));
  if (asp == NULL) {
    fprintf(stderr, "create_animSprite: failed to allocate memory for sprite.");
    return NULL;
  }

  asp->map = frames;
  asp->num_fig = nframes;
  asp->aspeed = ticks_per_frame;
  asp->cur_aspeed = 0;
  asp->cur_fig = 0;
  asp->loop = loop;
  asp->sp = frames[0];

  return asp;
}

void destroy_animSprite(AnimSprite *asp) {
  if (asp == NULL) {
    fprintf(stderr, "destroy_animSprite: asp pointer cannot be null.");
    return;
  }

  free(asp);
  asp = NULL;
}

void update_animSprite(AnimSprite *asp) {
  if (asp == NULL) {
    fprintf(stderr, "update_animSprite: asp pointer cannot be null.");
    return;
  }
  if (++asp->cur_aspeed < asp->aspeed) {
    return;
  }

  asp->cur_aspeed = 0;
  if (asp->loop) {
    asp->cur_fig = (asp->cur_fig + 1) % asp->num_fig;
  }
  else if (asp->cur_fig + 1 < asp->num_fig) {
    asp->cur_fig++;
  }

  asp->sp = asp->map[asp->cur_fig];
}
