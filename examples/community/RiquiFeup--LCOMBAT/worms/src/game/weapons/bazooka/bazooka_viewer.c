#include "bazooka_viewer.h"
#include "../../../assets/assets.h"
#include "../utils/weapon_viewer_utils.h"
#include <stddef.h>

void bazooka_viewer_draw(const Bazooka *bazooka) {
    if (bazooka == NULL || !bazooka_is_active(bazooka)) return;

    Projectile *p = bazooka_get_projectile(bazooka);
    if (p == NULL) return;
    
    uint8_t count = assets_get_missile_angles_count();
    Sprite *const *frames = assets_get_missile_angles();

    weapon_draw_rotated(p, frames, count);
}
