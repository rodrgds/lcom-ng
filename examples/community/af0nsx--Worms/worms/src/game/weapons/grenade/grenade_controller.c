#include "grenade_controller.h"
#include <stddef.h>
#include "../../../config.h"
#include "../../terrain/terrain.h"
#include "../../explosion/explosion_controller.h"
#include "../../worm/worm.h"

#include "../utils/weapon_physics_utils.h"

void grenade_controller_fire_aimed_from_worm(GameContext *context, Worm *active_worm, double angle, int power) {
    if (context == NULL || active_worm == NULL) return;

    int32_t start_x, start_y;
    double vx, vy;
    if (!weapon_prepare_aimed_shot_from_worm(active_worm, GRENADE_HITBOX_SIZE,
                                             angle, power, GRENADE_SPEED,
                                             &start_x, &start_y, &vx, &vy)) {
        return;
    }

    Grenade *grenade = grenade_create(start_x, start_y, vx, vy);
    if (grenade != NULL) game_context_set_grenade(context, grenade);
}

// Wrapper for the callback that matches WeaponCollisionFunc
static bool grenade_collision_wrapper(const void *weapon, struct GameContext *ctx, int *out_cx, int *out_cy) {
    return grenade_check_collision((const Grenade *)weapon, ctx, out_cx, out_cy);
}

void grenade_controller_tick(GameContext *context, Grenade *grenade) {
    if (context == NULL || grenade == NULL || !grenade_is_active(grenade)) return;

    Projectile *p = grenade_get_projectile(grenade);

    weapon_physics_bounce_update(p, grenade, context, grenade_collision_wrapper, GRENADE_RESTITUTION, GRENADE_FRICTION);

    grenade_advance_ticks(grenade);

    if (grenade_get_ticks(grenade) >= GRENADE_EXPLOSION_TICKS) {
        Hitbox hb = projectile_get_hitbox(p);
        int final_cx = hb.x + hb.width / 2;
        int final_cy = hb.y + hb.height / 2;

        grenade_deactivate(grenade);
        explosion_controller_spawn(context, final_cx, final_cy, GRENADE_EXPLOSION_RADIUS, GRENADE_DAMAGE);
    }
}

bool grenade_controller_handle_aim(GameContext *context, Worm *active_worm, double *aim_angle, int *aim_power, bool *space_held) {
    if (weapon_update_aim_input(aim_angle, aim_power, space_held)) {
        grenade_controller_fire_aimed_from_worm(context, active_worm, *aim_angle, *aim_power);
        *aim_power = 0;
        return true;
    }
    return false;
}
