#include "animated_sprite.h"
#include <stdlib.h>

struct AnimatedSprite {
  Sprite   *const *frames;
  uint8_t    frame_count;
  uint8_t    current_frame;
  uint8_t    ticks_per_frame;
  uint8_t    ticks_left;
  bool       loop;
  bool       done;
};

AnimatedSprite* animated_sprite_create(Sprite *const *frames, uint8_t frame_count, uint8_t ticks_per_frame, bool loop) {

  if (frames == NULL || frame_count == 0) return NULL;

  AnimatedSprite *as = (AnimatedSprite *) malloc(sizeof(AnimatedSprite));
  if (as == NULL) return NULL;

  as->frames = frames; 
  as->frame_count = frame_count;
  as->current_frame = 0;
  as->ticks_per_frame = ticks_per_frame;
  as->ticks_left = ticks_per_frame;
  as->loop = loop;
  as->done = false;

  return as;
}

void animated_sprite_destroy(AnimatedSprite *as) {
  if (as == NULL) return;
  free(as);
}

static void handle_animation_end(AnimatedSprite *as) {
  if (as->loop) {
    as->current_frame = 0;
  } else {
    as->current_frame = as->frame_count - 1;
    as->done = true;
  }
}

void animated_sprite_tick(AnimatedSprite *as) {
  if (as == NULL || as->done) return;

  as->ticks_left--;
  
  // Keep the current frame while it still has ticks left.
  if (as->ticks_left > 0) return;

  // Reset the frame timer and advance to the next frame.
  as->ticks_left = as->ticks_per_frame;
  as->current_frame++;

  // Clamp or loop when the animation reaches the frame limit.
  if (as->current_frame >= as->frame_count) {
    handle_animation_end(as);
  }
}

void animated_sprite_reset(AnimatedSprite *as) {
  if (as == NULL) return;
  as->current_frame = 0;
  as->ticks_left = as->ticks_per_frame;
  as->done = false;
}

const Sprite* animated_sprite_get_current_frame(const AnimatedSprite *as) {
  if (as == NULL || as->frames == NULL) return NULL;
  return as->frames[as->current_frame];
}

bool animated_sprite_is_done(const AnimatedSprite *as) {
  if (as == NULL) return true;
  return as->done;
}
