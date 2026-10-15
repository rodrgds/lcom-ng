#include "aim_viewer.h"
#include <stddef.h>
#include "../../assets/assets.h"
#include "utils/weapon_viewer_utils.h"

void aim_viewer_draw(const Worm *worm, double aim_angle, int aim_power) {
    if (worm == NULL) return;

    WormState ws = worm_get_state(worm);
    WeaponType wt = worm_get_weapon(worm);

    if (wt != WEAPON_BAZOOKA && wt != WEAPON_GRENADE) return;
    if (ws != WORM_WEAPON_PULL && ws != WORM_WEAPON_HOLD && ws != WORM_WEAPON_RETRACT) return;

    int wx = worm_get_x(worm) + WORM_HITBOX_WIDTH / 2;
    int wy = worm_get_y(worm);
    int dir = (worm_get_direction(worm) == WORM_DIR_RIGHT) ? 1 : -1;

    Sprite *const *frames;
    uint8_t count;
    double speed;
    uint16_t projectile_size;

    if (wt == WEAPON_BAZOOKA) {
        frames = assets_get_missile_angles();
        count = assets_get_missile_angles_count();
        speed = BAZOOKA_SPEED;
        projectile_size = MISSILE_HITBOX_SIZE;
    } else {
        frames = assets_get_grenade_angles();
        count = assets_get_grenade_angles_count();
        speed = GRENADE_SPEED;
        projectile_size = GRENADE_HITBOX_SIZE;
    }

    weapon_draw_aim_trajectory(wx, wy, aim_angle, aim_power, dir, frames, count,
                               speed, projectile_size);
}
