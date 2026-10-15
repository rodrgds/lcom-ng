#include "weapon_physics_utils.h"
#include <math.h>
#include <stddef.h>
#include "../../../config.h"
#include "../../../engine/input/input.h"

bool weapon_get_muzzle_spawn(const Worm *worm, uint16_t projectile_size, int32_t *spawn_x, int32_t *spawn_y) {
    if (worm == NULL || spawn_x == NULL || spawn_y == NULL) return false;

    int direction = (worm_get_direction(worm) == WORM_DIR_RIGHT) ? 1 : -1;
    int32_t muzzle_x = worm_get_x(worm) + WORM_HITBOX_WIDTH / 2 +
                       direction * WEAPON_MUZZLE_OFFSET_X;
    int32_t muzzle_y = worm_get_y(worm) + WEAPON_MUZZLE_OFFSET_Y;

    *spawn_x = muzzle_x - projectile_size / 2;
    *spawn_y = muzzle_y - projectile_size / 2;
    return true;
}

bool weapon_velocity_from_angle(double angle, int power, double speed, double *vx, double *vy) {
    if (vx == NULL || vy == NULL || power <= 0) return false;

    double scaled_speed = speed * power / 100.0;
    *vx = cos(angle) * scaled_speed;
    *vy = sin(angle) * scaled_speed;
    return true;
}

bool weapon_prepare_aimed_shot_from_worm(const Worm *worm, uint16_t projectile_size,
                                         double angle, int power, double speed,
                                         int32_t *start_x, int32_t *start_y,
                                         double *vx, double *vy) {
    if (!weapon_get_muzzle_spawn(worm, projectile_size, start_x, start_y)) return false;
    return weapon_velocity_from_angle(angle, power, speed, vx, vy);
}

bool weapon_update_aim_input(double *aim_angle, int *aim_power, bool *space_held) {
    if (aim_angle == NULL || aim_power == NULL || space_held == NULL) return false;

    if (input_is_key_pressed(KEY_UP)) *aim_angle -= 0.04;
    if (input_is_key_pressed(KEY_DOWN)) *aim_angle += 0.04;

    bool space_now = input_is_key_pressed(KEY_SPACE);
    if (space_now && *aim_power < 100) (*aim_power)++;

    if (*space_held && !space_now) {
        *space_held = false;
        return true;
    }

    *space_held = space_now;
    return false;
}

void weapon_physics_bounce_update(Projectile *p, const void *weapon, struct GameContext *ctx,
                                  WeaponCollisionFunc check_collision, double restitution, double friction) {
    if (p == NULL || weapon == NULL || ctx == NULL || check_collision == NULL) return;

    double prev_x = projectile_get_precise_x(p);
    double prev_y = projectile_get_precise_y(p);
    double old_vx = projectile_get_vx(p);
    double old_vy = projectile_get_vy(p);

    projectile_update(p, SCREEN_W, SCREEN_H);

    int cx, cy;
    if (check_collision(weapon, ctx, &cx, &cy)) {
        double new_vy = projectile_get_vy(p);

        bool hit_x = false;
        bool hit_y = false;

        projectile_set_x(p, prev_x + old_vx);
        projectile_set_y(p, prev_y);
        if (check_collision(weapon, ctx, NULL, NULL)) hit_x = true;

        projectile_set_x(p, prev_x);
        projectile_set_y(p, prev_y + old_vy);
        if (check_collision(weapon, ctx, NULL, NULL)) hit_y = true;

        if (!hit_x && !hit_y) {
            hit_x = true;
            hit_y = true;
        }

        projectile_set_x(p, prev_x);
        projectile_set_y(p, prev_y);

        if (hit_x) {
            old_vx = -old_vx * restitution;
        } else if (hit_y) {
            old_vx = old_vx * friction;
        }

        if (hit_y) {
            new_vy = -new_vy * restitution;
        }

        projectile_set_vx(p, old_vx);
        projectile_set_vy(p, new_vy);
    }
}
