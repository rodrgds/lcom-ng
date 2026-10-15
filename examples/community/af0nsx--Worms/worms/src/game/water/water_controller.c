#include "water_controller.h"

#include <stddef.h>

#include "../../config.h"
#include "../worm/worm.h"
#include "../projectile/projectile.h"
#include "../weapons/bazooka/bazooka.h"
#include "../weapons/grenade/grenade.h"

// Deactivates a projectile when its bottom edge reaches the water level.
static void deactivate_projectile_in_water(Projectile *projectile, int32_t level) {
    if (projectile == NULL || !projectile_is_active(projectile)) return;

    Hitbox hb = projectile_get_hitbox(projectile);
    int32_t bottom_y = hb.y + hb.height;
    if (bottom_y >= level) projectile_deactivate(projectile);
}

void water_controller_tick(const Water *w, GameContext *ctx) {
    if (w == NULL || ctx == NULL) return;

    int32_t level = water_get_level(w);

    for (int i = 0; i < MAX_WORMS; i++) {
        Worm *worm = game_context_get_worm(ctx, i);
        if (worm != NULL && worm_is_alive(worm)) {

            Hitbox hb = worm_get_hitbox(worm);
            int32_t bottom_y = hb.y + hb.height;
            if (bottom_y >= level) {
                worm_take_damage(worm, WATER_KILL_DAMAGE);
                worm_set_state(worm, WORM_DROWNED);
            }
        }
    }

    Bazooka *bazooka = game_context_get_bazooka(ctx);
    Grenade *grenade = game_context_get_grenade(ctx);
    if (bazooka != NULL) deactivate_projectile_in_water(bazooka_get_projectile(bazooka), level);
    if (grenade != NULL) deactivate_projectile_in_water(grenade_get_projectile(grenade), level);
}
