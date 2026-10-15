#include "animation.h"

animation_t *animation_create(uint32_t frame_amount, void (*on_end)(void *)) {
  animation_t *anim = (animation_t *) malloc(sizeof(animation_t));
  if (anim == NULL)
    return NULL;

  anim->frame_index = 0;
  anim->on_end = on_end;

  anim->frames = queue_create(frame_amount);
  if (anim->frames == NULL)
    return NULL;

  anim->rotate = queue_create(frame_amount);
  if (anim->rotate == NULL)
    return NULL;

  anim->x = queue_create(frame_amount);
  if (anim->x == NULL) {
    queue_destroy(&anim->frames, queue_destroy_nothing);
    return NULL;
  }

  anim->y = queue_create(frame_amount);
  if (anim->y == NULL) {
    queue_destroy(&anim->frames, queue_destroy_nothing);
    queue_destroy(&anim->x, queue_destroy_nothing);
    return NULL;
  }

  return anim;
}

int animation_add_frame(animation_t *animation, sprite_t *sprite, uint32_t x, uint32_t y, bool rotate) {
  if (animation == NULL || sprite == NULL)
    return 1;

  uint32_t *xp = malloc(sizeof(uint32_t));
  uint32_t *yp = malloc(sizeof(uint32_t));
  bool *rot = malloc(sizeof(bool));
  *xp = x;
  *yp = y;
  *rot = rotate;

  return queue_push(animation->frames, sprite) || queue_push(animation->x, xp) || queue_push(animation->y, yp) || queue_push(animation->rotate, rot);
}

void animation_run(animation_t **animation, void *ptr) {
  if (animation == NULL)
    return;

  animation_t *anim = *animation;
  if (anim == NULL || anim->frames == NULL) return;
  anim->frame_index += 1;

	if (anim->frame_index == anim->frames->curr_size - 1)
  {
    *animation = NULL;
    if (anim->on_end == NULL) return;
    
    anim->on_end(ptr);
    animation_destroy(anim);
    return;
  };

  vg_set_redraw();
}

int animation_running(animation_t *animation)
{
  if (animation == NULL) return 0;

  return animation->frame_index < animation->frames->curr_size;
}

int animation_draw(animation_t *animation) {
  if (animation == NULL)
    return 1;

  uint32_t *x = queue_at(animation->x, animation->frame_index);
  uint32_t *y = queue_at(animation->y, animation->frame_index);
  sprite_t *sprite = queue_at(animation->frames, animation->frame_index);
  bool *rot = queue_at(animation->rotate, animation->frame_index);
  if (x == NULL || y == NULL || sprite == NULL || rot == NULL)
    return 1;

  sprite_move(sprite, *x, *y);
  return *rot ? sprite_draw_rotate(sprite) : sprite_draw(sprite);
}

void animation_destroy(animation_t *animation) {
  if (animation == NULL)
    return;

  queue_destroy(&animation->frames, queue_destroy_nothing);
  queue_destroy(&animation->x, free);
  queue_destroy(&animation->y, free);
  queue_destroy(&animation->rotate, free);
  free(animation);
}
