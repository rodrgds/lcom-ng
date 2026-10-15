#include "worm_viewer.h"
#include <stdlib.h>
#include "../../config.h"
#include "../../assets/assets.h"
#include "../../engine/graphics/animated_sprite.h"
#include "../../engine/graphics/renderer.h"

struct WormViewer {
  AnimatedSprite* walk_l;
  AnimatedSprite* walk_r;
  AnimatedSprite* jump_l;
  AnimatedSprite* jump_r;
  AnimatedSprite* fall_l;
  AnimatedSprite* fall_r;
  AnimatedSprite* die;
  AnimatedSprite* idle_l_anims[WORM_IDLE_ANIM_COUNT];
  AnimatedSprite* idle_r_anims[WORM_IDLE_ANIM_COUNT];
  uint8_t current_idle;

  AnimatedSprite* bazooka_pull_l;
  AnimatedSprite* bazooka_pull_r;
  AnimatedSprite* bazooka_retract_l;
  AnimatedSprite* bazooka_retract_r;

  AnimatedSprite* grenade_pull_l;
  AnimatedSprite* grenade_pull_r;
  AnimatedSprite* grenade_retract_l;
  AnimatedSprite* grenade_retract_r;

  AnimatedSprite* prev_anim;
};

static void draw_worm_frame(const Sprite *frame, int x, int y) {
  const uint32_t *colors;
  uint32_t transparent_color;
  uint16_t width, height;
  int min_x, max_x, min_y, max_y;
  int visible_center_x, visible_bottom_y, anchor_x, anchor_y;
  bool found_pixel = false;

  if (frame == NULL) return;

  colors = sprite_get_colors(frame);
  width = sprite_get_width(frame);
  height = sprite_get_height(frame);
  transparent_color = TRANSPARENT_COLOR;
  if (colors == NULL || width == 0 || height == 0) {
    renderer_draw_sprite(frame, x, y);
    return;
  }

  min_x = width;
  max_x = 0;
  min_y = height;
  max_y = 0;

  for (uint16_t row = 0; row < height; row++) {
    for (uint16_t col = 0; col < width; col++) {
      if (colors[col + row * width] == transparent_color) continue;

      if (!found_pixel) {
        min_x = max_x = col;
        min_y = max_y = row;
        found_pixel = true;
      }
      else {
        if (col < min_x) min_x = col;
        if (col > max_x) max_x = col;
        if (row < min_y) min_y = row;
        if (row > max_y) max_y = row;
      }
    }
  }

  if (!found_pixel) {
    renderer_draw_sprite(frame, x, y);
    return;
  }

  visible_center_x = (min_x + max_x) / 2;
  visible_bottom_y = max_y;
  anchor_x = x + WORM_HITBOX_WIDTH / 2;
  anchor_y = y + WORM_HITBOX_HEIGHT;

  renderer_draw_sprite(frame, anchor_x - visible_center_x, anchor_y - visible_bottom_y);
}

WormViewer *worm_viewer_create(void) {
  WormViewer *wv = (WormViewer *) malloc(sizeof(WormViewer));
  if (wv == NULL) return NULL;

  wv->walk_l = NULL;
  wv->walk_r = NULL;
  wv->jump_l = NULL;
  wv->jump_r = NULL;
  wv->fall_l = NULL;
  wv->fall_r = NULL;
  wv->die = NULL;
  for (int i = 0; i < WORM_IDLE_ANIM_COUNT; i++) {
    wv->idle_l_anims[i] = NULL;
    wv->idle_r_anims[i] = NULL;
  }
  wv->bazooka_pull_l = NULL;
  wv->bazooka_pull_r = NULL;
  wv->bazooka_retract_l = NULL;
  wv->bazooka_retract_r = NULL;
  wv->grenade_pull_l = NULL;
  wv->grenade_pull_r = NULL;
  wv->grenade_retract_l = NULL;
  wv->grenade_retract_r = NULL;

  wv->walk_l = animated_sprite_create(assets_get_walk_l(), assets_get_walk_count(), WORM_WALK_TICKS_PER_FRAME, true);
  wv->walk_r = animated_sprite_create(assets_get_walk_r(), assets_get_walk_count(), WORM_WALK_TICKS_PER_FRAME, true);
  wv->jump_l = animated_sprite_create(assets_get_jump_l(), assets_get_jump_count(), WORM_JUMP_TICKS_PER_FRAME, true);
  wv->jump_r = animated_sprite_create(assets_get_jump_r(), assets_get_jump_count(), WORM_JUMP_TICKS_PER_FRAME, true);
  wv->fall_l = animated_sprite_create(assets_get_fall_l(), assets_get_fall_count(), WORM_JUMP_TICKS_PER_FRAME, true);
  wv->fall_r = animated_sprite_create(assets_get_fall_r(), assets_get_fall_count(), WORM_JUMP_TICKS_PER_FRAME, true);
  wv->die = animated_sprite_create(assets_get_die(), assets_get_die_count(), WORM_DIE_TICKS_PER_FRAME, false);

  wv->idle_l_anims[0] = animated_sprite_create(assets_get_idle1_l(), assets_get_idle1_count(), WORM_IDLE_TICKS_PER_FRAME, false);
  wv->idle_l_anims[1] = animated_sprite_create(assets_get_idle2_l(), assets_get_idle2_count(), WORM_IDLE_TICKS_PER_FRAME, false);
  wv->idle_l_anims[2] = animated_sprite_create(assets_get_idle3_l(), assets_get_idle3_count(), WORM_IDLE_TICKS_PER_FRAME, false);
  wv->idle_l_anims[3] = animated_sprite_create(assets_get_idle4_l(), assets_get_idle4_count(), WORM_IDLE_TICKS_PER_FRAME, false);

  wv->idle_r_anims[0] = animated_sprite_create(assets_get_idle1_r(), assets_get_idle1_count(), WORM_IDLE_TICKS_PER_FRAME, false);
  wv->idle_r_anims[1] = animated_sprite_create(assets_get_idle2_r(), assets_get_idle2_count(), WORM_IDLE_TICKS_PER_FRAME, false);
  wv->idle_r_anims[2] = animated_sprite_create(assets_get_idle3_r(), assets_get_idle3_count(), WORM_IDLE_TICKS_PER_FRAME, false);
  wv->idle_r_anims[3] = animated_sprite_create(assets_get_idle4_r(), assets_get_idle4_count(), WORM_IDLE_TICKS_PER_FRAME, false);

  if (!wv->walk_l || !wv->walk_r || !wv->jump_l || !wv->jump_r || !wv->fall_l || !wv->fall_r || !wv->die ||
      !wv->idle_l_anims[0] || !wv->idle_l_anims[1] || !wv->idle_l_anims[2] || !wv->idle_l_anims[3] ||
      !wv->idle_r_anims[0] || !wv->idle_r_anims[1] || !wv->idle_r_anims[2] || !wv->idle_r_anims[3]) {
    worm_viewer_destroy(wv);
    return NULL;
  }

  wv->bazooka_pull_l = animated_sprite_create(assets_get_bazooka_pull_l(), assets_get_bazooka_pull_count(), BAZOOKA_PULL_TICKS, false);
  wv->bazooka_pull_r = animated_sprite_create(assets_get_bazooka_pull_r(), assets_get_bazooka_pull_count(), BAZOOKA_PULL_TICKS, false);
  wv->bazooka_retract_l = animated_sprite_create(assets_get_bazooka_retract_l(), assets_get_bazooka_retract_count(), BAZOOKA_RETRACT_TICKS, false);
  wv->bazooka_retract_r = animated_sprite_create(assets_get_bazooka_retract_r(), assets_get_bazooka_retract_count(), BAZOOKA_RETRACT_TICKS, false);

  wv->grenade_pull_l = animated_sprite_create(assets_get_grenade_pull_l(), assets_get_grenade_pull_count(), BAZOOKA_PULL_TICKS, false);
  wv->grenade_pull_r = animated_sprite_create(assets_get_grenade_pull_r(), assets_get_grenade_pull_count(), BAZOOKA_PULL_TICKS, false);
  wv->grenade_retract_l = animated_sprite_create(assets_get_grenade_retract_l(), assets_get_grenade_retract_count(), BAZOOKA_RETRACT_TICKS, false);
  wv->grenade_retract_r = animated_sprite_create(assets_get_grenade_retract_r(), assets_get_grenade_retract_count(), BAZOOKA_RETRACT_TICKS, false);

  wv->current_idle = rand() % WORM_IDLE_ANIM_COUNT;
  wv->prev_anim = NULL;

  return wv;
}

void worm_viewer_destroy(WormViewer *wv) {
  if (wv == NULL) return;

  animated_sprite_destroy(wv->walk_l);
  animated_sprite_destroy(wv->walk_r);
  animated_sprite_destroy(wv->jump_l);
  animated_sprite_destroy(wv->jump_r);
  animated_sprite_destroy(wv->fall_l);
  animated_sprite_destroy(wv->fall_r);
  animated_sprite_destroy(wv->die);
  for (int i = 0; i < WORM_IDLE_ANIM_COUNT; i++) {
    animated_sprite_destroy(wv->idle_l_anims[i]);
    animated_sprite_destroy(wv->idle_r_anims[i]);
  }

  animated_sprite_destroy(wv->bazooka_pull_l);
  animated_sprite_destroy(wv->bazooka_pull_r);
  animated_sprite_destroy(wv->bazooka_retract_l);
  animated_sprite_destroy(wv->bazooka_retract_r);

  animated_sprite_destroy(wv->grenade_pull_l);
  animated_sprite_destroy(wv->grenade_pull_r);
  animated_sprite_destroy(wv->grenade_retract_l);
  animated_sprite_destroy(wv->grenade_retract_r);

  free(wv);
}

void worm_viewer_draw(WormViewer *wv, const Worm *worm) {
  if (wv == NULL || worm == NULL) return;

  int x = worm_get_x(worm);
  int y = worm_get_y(worm);
  WormState state = worm_get_state(worm);
  WormDirection direction = worm_get_direction(worm);

  AnimatedSprite* current_anim = NULL;

  if (state == WORM_IDLE) {
    AnimatedSprite **idle_anims = (direction == WORM_DIR_RIGHT) ? wv->idle_r_anims : wv->idle_l_anims;
    current_anim = idle_anims[wv->current_idle];
    if (animated_sprite_is_done(current_anim)) {
      wv->current_idle = rand() % WORM_IDLE_ANIM_COUNT;
      animated_sprite_reset(wv->idle_l_anims[wv->current_idle]);
      animated_sprite_reset(wv->idle_r_anims[wv->current_idle]);
      current_anim = idle_anims[wv->current_idle];
    }
  } else if (state == WORM_WALK) {
    current_anim = (direction == WORM_DIR_RIGHT) ? wv->walk_r : wv->walk_l;
  } else if (state == WORM_JUMP) {
    current_anim = (direction == WORM_DIR_RIGHT) ? wv->jump_r : wv->jump_l;
  } else if (state == WORM_FALL) {
    current_anim = (direction == WORM_DIR_RIGHT) ? wv->fall_r : wv->fall_l;
  } else if (state == WORM_DEAD) {
    current_anim = wv->die;
    if (animated_sprite_is_done(current_anim)) {
      return;
    }
  } else if (state == WORM_DROWNED) {
    return;
  } else if (state == WORM_WEAPON_PULL || state == WORM_WEAPON_HOLD || state == WORM_WEAPON_RETRACT) {

    WeaponType weapon = worm_get_weapon(worm);

    if (weapon == WEAPON_BAZOOKA) {
      if (state == WORM_WEAPON_PULL) current_anim = (direction == WORM_DIR_RIGHT) ? wv->bazooka_pull_r : wv->bazooka_pull_l;

      else if (state == WORM_WEAPON_RETRACT) current_anim = (direction == WORM_DIR_RIGHT) ? wv->bazooka_retract_r : wv->bazooka_retract_l;

      else {
        current_anim = (direction == WORM_DIR_RIGHT) ? wv->bazooka_pull_r : wv->bazooka_pull_l;
      }
    } else if (weapon == WEAPON_GRENADE) {
      if (state == WORM_WEAPON_PULL) current_anim = (direction == WORM_DIR_RIGHT) ? wv->grenade_pull_r : wv->grenade_pull_l;

      else if (state == WORM_WEAPON_RETRACT) current_anim = (direction == WORM_DIR_RIGHT) ? wv->grenade_retract_r : wv->grenade_retract_l;
      
      else {
        current_anim = (direction == WORM_DIR_RIGHT) ? wv->grenade_pull_r : wv->grenade_pull_l;
      }
    }
  }

  if (current_anim != wv->prev_anim) {
    if (current_anim != NULL) animated_sprite_reset(current_anim);
    wv->prev_anim = current_anim;
  }

  if (current_anim != NULL) {
    animated_sprite_tick(current_anim);
    const Sprite* frame = animated_sprite_get_current_frame(current_anim);
    draw_worm_frame(frame, x, y);
  }
}
