#include "bazooka_controller.h"
#include <stddef.h>
#include "../../../config.h"
#include "../../terrain/terrain.h"
#include "../../explosion/explosion_controller.h"
#include "../../worm/worm.h"
#include "../utils/weapon_physics_utils.h"

void bazooka_controller_fire_aimed_from_worm(GameContext *context, Worm *active_worm, double angle, int power) {
    if (context == NULL || active_worm == NULL) return;

    int32_t start_x, start_y;
    double vx, vy;
    if (!weapon_prepare_aimed_shot_from_worm(active_worm, MISSILE_HITBOX_SIZE,
                                             angle, power, BAZOOKA_SPEED,
                                             &start_x, &start_y, &vx, &vy)) {
        return;
    }

    Bazooka *bazooka = bazooka_create((double)start_x, (double)start_y, vx, vy);
    if (bazooka != NULL) game_context_set_bazooka(context, bazooka);
}

void bazooka_controller_tick(GameContext *context, Bazooka *bazooka) {
    if (context == NULL || bazooka == NULL || !bazooka_is_active(bazooka)) return;

    bazooka_update(bazooka);

    int cx, cy;
    if (bazooka_check_collision(bazooka, context, &cx, &cy)) {
        bazooka_deactivate(bazooka);
        explosion_controller_spawn(context, cx, cy, BAZOOKA_EXPLOSION_RADIUS, BAZOOKA_DAMAGE);
    }
}

bool bazooka_controller_handle_aim(GameContext *context, Worm *active_worm, double *aim_angle, int *aim_power, bool *space_held) {
    if (weapon_update_aim_input(aim_angle, aim_power, space_held)) {
        bazooka_controller_fire_aimed_from_worm(context, active_worm, *aim_angle, *aim_power);
        *aim_power = 0;
        return true;
    }
    return false;
}
