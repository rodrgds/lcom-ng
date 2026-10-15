#include "water_viewer.h"

#include <stddef.h>

#include "../../assets/assets.h"
#include "../../engine/graphics/renderer.h"

void water_viewer_draw(const Water *w) {
    if (w == NULL) return;
    
    int32_t level = water_get_level(w);
    const Sprite *water_sprite = assets_get_water();
    if (water_sprite != NULL) {
        renderer_draw_sprite(water_sprite, 0, level);
    }
}
