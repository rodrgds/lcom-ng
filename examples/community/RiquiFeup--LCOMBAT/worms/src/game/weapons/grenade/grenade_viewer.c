#include <stddef.h>
#include "grenade_viewer.h"
#include "../../../assets/assets.h"
#include "../utils/weapon_viewer_utils.h"

void grenade_viewer_draw(const Grenade *grenade) {
    if (grenade == NULL || !grenade_is_active(grenade)) return;

    Projectile *p = grenade_get_projectile(grenade);
    if (p == NULL) return;

    uint8_t count = assets_get_grenade_angles_count();
    Sprite *const *frames = assets_get_grenade_angles();

    weapon_draw_rotated(p, frames, count);
}
