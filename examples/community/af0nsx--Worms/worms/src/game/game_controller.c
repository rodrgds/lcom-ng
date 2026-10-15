#include "game_controller.h"

#include <stddef.h>

#include "../config.h"
#include "cursor/cursor_controller.h"
#include "physics/physics_controller.h"
#include "explosion/explosion_controller.h"
#include "water/water_controller.h"
#include "weapons/bazooka/bazooka_controller.h"
#include "weapons/grenade/grenade_controller.h"

void game_controller_handle_event(GameContext *context, const GameEvent *event) {
  if (context == NULL || event == NULL) return;

  cursor_controller_handle_event(game_context_get_cursor(context), event);

  if (event->type == EVENT_TICK) {
    game_context_tick(context);
    
    for (int i = 0; i < MAX_WORMS; i++) {
      physics_update_worm(game_context_get_worm(context, i));
    }
    
    explosion_controller_tick_all(context);
    water_controller_tick(game_context_get_water(context), context);
    bazooka_controller_tick(context, game_context_get_bazooka(context));
    grenade_controller_tick(context, game_context_get_grenade(context));
  }
}
