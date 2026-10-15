#include "game_viewer.h"

#include <stdlib.h>

#include "../config.h"
#include "../engine/graphics/renderer.h"
#include "../engine/graphics/animated_sprite.h"
#include "../assets/assets.h"
#include "cursor/cursor_viewer.h"
#include "hud/hud.h"
#include "terrain/terrain_viewer.h"
#include "worm/worm_viewer.h"
#include "explosion/explosion_viewer.h"
#include "water/water_viewer.h"
#include "weapons/bazooka/bazooka_viewer.h"
#include "weapons/aim_viewer.h"
#include "weapons/grenade/grenade_viewer.h"

struct GameViewer {
  WormViewer *worm_viewers[MAX_WORMS];
};

GameViewer *game_viewer_create(void) {
  GameViewer *gv = malloc(sizeof(GameViewer));
  if (gv == NULL) return NULL;

  for (int i = 0; i < MAX_WORMS; i++) {
    gv->worm_viewers[i] = worm_viewer_create();
  }

  for (int i = 0; i < MAX_WORMS; i++) {
    if (gv->worm_viewers[i] == NULL) {
      game_viewer_destroy(gv);
      return NULL;
    }
  }

  return gv;
}

void game_viewer_destroy(GameViewer *gv) {
  if (gv == NULL) return;
  for (int i = 0; i < MAX_WORMS; i++) {
    worm_viewer_destroy(gv->worm_viewers[i]);
  }
  free(gv);
}

void game_viewer_draw_scene(GameViewer *gv, const GameContext *context) {
  renderer_clear();

  renderer_draw_sprite(assets_get_background(), 0, 0);
  if (terrain_viewer_draw() != 0) return;
  water_viewer_draw(game_context_get_water(context));

  for (int i = 0; i < MAX_WORMS; i++) {
    worm_viewer_draw(gv->worm_viewers[i], game_context_get_worm(context, i));
  }

  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    Explosion *e = game_context_get_explosion(context, i);
    if (e != NULL && explosion_is_active(e)) {
      explosion_viewer_draw(e);
    }
  }

  Worm *active_worm = game_context_get_active_worm(context);
  if (active_worm != NULL && worm_is_alive(active_worm)) {
    const Sprite *arrow = assets_get_arrow();
    if (arrow != NULL) {
      int cx = (int)worm_get_x(active_worm) + WORM_HITBOX_WIDTH / 2;
      int top = (int)worm_get_y(active_worm);
      int ax = cx - sprite_get_width(arrow) / 2;
      int ay = top - HUD_NAME_OFFSET_Y - sprite_get_height(arrow) - 4;
      renderer_draw_sprite(arrow, ax, ay);
    }
  }

  Bazooka *active_bazooka = game_context_get_bazooka(context);
  if (active_bazooka != NULL) {
    bazooka_viewer_draw(active_bazooka);
  }

  Grenade *active_grenade = game_context_get_grenade(context);
  if (active_grenade != NULL) {
    grenade_viewer_draw(active_grenade);
  }

}

void game_viewer_draw(GameViewer *gv, const GameContext *context) {
  if (gv == NULL || context == NULL) return;
  game_viewer_draw_scene(gv, context);
  hud_draw(context);
  cursor_viewer_draw(game_context_get_cursor(context));
  renderer_flip();
}

void game_viewer_draw_aimed(GameViewer *gv, const GameContext *context, double aim_angle, int aim_power) {
  if (gv == NULL || context == NULL) return;
  game_viewer_draw_scene(gv, context);
  aim_viewer_draw(game_context_get_active_worm(context), aim_angle, aim_power);
  hud_draw(context);
  cursor_viewer_draw(game_context_get_cursor(context));
  renderer_flip();
}
