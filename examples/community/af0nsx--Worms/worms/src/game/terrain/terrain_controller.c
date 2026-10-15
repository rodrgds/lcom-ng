#include "terrain_controller.h"

#include "../../engine/input/input.h"

int terrain_controller_init(void) {
    return terrain_generate_procedural(input_get_rtc_seed());
}

void terrain_controller_handle_explosion(int cx, int cy, int radius) {
    terrain_explode(cx, cy, radius);
}
