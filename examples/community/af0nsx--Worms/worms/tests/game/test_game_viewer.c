#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// Mock definitions
typedef struct GameContext GameContext;
typedef struct GameViewer GameViewer;
typedef struct WormViewer WormViewer;
typedef struct Worm Worm;
typedef struct Cursor Cursor;
typedef struct Explosion Explosion;
#include "../../src/game/worm/worm.h"
#include "../../src/engine/graphics/sprite.h"
#include "../../src/engine/graphics/animated_sprite.h"
#include "../../src/assets/assets.h"
// Mock assets
const Sprite* assets_get_background(void) { return (Sprite*)1; }
const Sprite* assets_get_water(void) { return (Sprite*)2; }
const Sprite* assets_get_wind_l(void) { return (Sprite*)3; }
const Sprite* assets_get_wind_r(void) { return (Sprite*)4; }
const Sprite* assets_get_crosshair(void) { return (Sprite*)5; }
const Sprite* assets_get_cursor(void) { return (Sprite*)6; }
const Sprite* assets_get_bazooka(void) { return (Sprite*)7; }
Sprite *const* assets_get_explosion(void) { return NULL; }
uint8_t assets_get_explosion_count(void) { return 1; }

// Mock AnimatedSprite
AnimatedSprite *animated_sprite_create(Sprite *const *frames, uint8_t frame_count, uint8_t ticks_per_frame, bool loop) {
  (void)frames; (void)frame_count; (void)ticks_per_frame; (void)loop;
  return (AnimatedSprite*)8;
}
void animated_sprite_destroy(AnimatedSprite *as) { (void)as; }
void animated_sprite_reset(AnimatedSprite *as) { (void)as; }
bool animated_sprite_is_done(const AnimatedSprite *as) { (void)as; return false; }
const Sprite* animated_sprite_get_current_frame(const AnimatedSprite *as) { (void)as; return (Sprite*)9; }
void animated_sprite_tick(AnimatedSprite *as) { (void)as; }
// Mock terrain viewer
int terrain_viewer_draw_calls = 0;
void terrain_viewer_draw(void) { terrain_viewer_draw_calls++; }

// Mock renderer
int renderer_clear_calls = 0;
int renderer_flip_calls = 0;
int renderer_draw_sprite_calls = 0;

void renderer_clear(void) { renderer_clear_calls++; }
int renderer_flip(void) { renderer_flip_calls++; return 0; }
int renderer_draw_sprite(const Sprite *sprite, int x, int y) {
  (void)sprite; (void)x; (void)y;
  renderer_draw_sprite_calls++;
  return 0;
}

// Mock cursor viewer
int cursor_viewer_draw_calls = 0;

void cursor_viewer_draw(const Cursor *cursor) {
  (void)cursor;
  cursor_viewer_draw_calls++;
}

// Mock explosion viewer/model
Explosion *game_context_get_explosion(const GameContext *context, int index) {
  (void)context; (void)index;
  return NULL;
}

bool explosion_is_active(const Explosion *explosion) {
  (void)explosion;
  return false;
}

void explosion_viewer_draw(const Explosion *explosion) {
  (void)explosion;
}

// Mock worm viewer
int worm_viewer_create_calls = 0;
int worm_viewer_destroy_calls = 0;
int worm_viewer_draw_calls = 0;

WormViewer *worm_viewer_create(void) {
  worm_viewer_create_calls++;
  return (WormViewer*) 1; // Dummy pointer
}

void worm_viewer_destroy(WormViewer *wv) {
  (void)wv;
  worm_viewer_destroy_calls++;
}

void worm_viewer_draw(WormViewer *wv, const Worm *worm) {
  (void)wv; (void)worm;
  worm_viewer_draw_calls++;
}

// Mock worm model
int worm_create_calls = 0;
int worm_destroy_calls = 0;

Worm *worm_create(int32_t x, int32_t y) {
  (void)x; (void)y;
  worm_create_calls++;
  return (Worm*) 5;
}

void worm_destroy(Worm *worm) {
  (void)worm;
  worm_destroy_calls++;
}

void worm_set_state(Worm *worm, WormState state) {
  (void)worm; (void)state;
}

void worm_set_direction(Worm *worm, WormDirection dir) {
  (void)worm; (void)dir;
}

// Mock game context
Worm *game_context_get_worm(const GameContext *context, int index) {
  (void)context; (void)index;
  return (Worm*) 2; // Dummy pointer
}

Cursor *game_context_get_cursor(const GameContext *context) {
  (void)context;
  return (Cursor*) 4;
}

// Include source to test
#include "../../src/game/game_viewer.c"

void test_game_viewer_create_destroy(void) {
  worm_viewer_create_calls = 0;
  worm_viewer_destroy_calls = 0;

  GameViewer *gv = game_viewer_create();
  assert(gv != NULL);
  assert(worm_viewer_create_calls == 9);

  game_viewer_destroy(gv);
  assert(worm_viewer_destroy_calls == 9);
}

void test_game_viewer_draw(void) {
  GameViewer *gv = game_viewer_create();
  GameContext *ctx = (GameContext*) 3; // Dummy pointer

  renderer_clear_calls = 0;
  renderer_flip_calls = 0;
  renderer_draw_sprite_calls = 0;
  cursor_viewer_draw_calls = 0;
  terrain_viewer_draw_calls = 0;
  worm_viewer_draw_calls = 0;

  game_viewer_draw(gv, ctx);

  assert(renderer_clear_calls == 1);
  assert(renderer_flip_calls == 1);
  assert(terrain_viewer_draw_calls == 1);
  assert(renderer_draw_sprite_calls == 2); // background, water
  assert(cursor_viewer_draw_calls == 1);
  assert(worm_viewer_draw_calls == 9);

  game_viewer_destroy(gv);
}

int main(void) {
  test_game_viewer_create_destroy();
  test_game_viewer_draw();
  return 0;
}
