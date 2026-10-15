#include <assert.h>
#include <stddef.h>
#include <stdbool.h>

#include "../../../src/config.h"
#include "../../../src/game/game.h"
#include "../../../src/game/water/water.h"
#include "../../../src/game/water/water_controller.h"
#include "../../../src/game/worm/worm.h"

static void test_water_kills_worm(void) {
  // We mock a water and a context or rely on the actual structures
  GameContext *ctx = game_context_create();
  assert(ctx != NULL);

  const Water *w = game_context_get_water(ctx);
  assert(w != NULL);

  // Move worm 0 into the water
  Worm *worm = game_context_get_worm(ctx, 0);
  
  // Create a water at Y=0 (top of screen).
  Water *deadly_water = water_create(0);
  
  assert(worm_is_alive(worm) == true);
  
  // Tick water controller with deadly water at Y=0
  water_controller_tick(deadly_water, ctx);
  
  // Worm should be dead now because its Y >= 0
  assert(worm_is_alive(worm) == false);

  water_destroy(deadly_water);
  game_context_destroy(ctx);
}

static void test_water_deactivates_projectile(void) {
  GameContext *ctx = game_context_create();
  
  // Create a projectile inside the water area
  Projectile *proj = projectile_create(100.0, 600.0, 0.0, 0.0, 10, 10, 50);
  game_context_set_projectile(ctx, proj);

  const Water *w = game_context_get_water(ctx); // water is at 500
  
  assert(projectile_is_active(proj) == true);
  
  water_controller_tick(w, ctx);
  
  // Should be deactivated
  assert(projectile_is_active(proj) == false);
  
  game_context_destroy(ctx);
}

int main(void) {
  test_water_kills_worm();
  test_water_deactivates_projectile();
  return 0;
}
