#include "water.h"
#include <stdlib.h>

struct Water {
    int32_t level_y;
};

Water* water_create(int32_t initial_level) {
    Water *w = malloc(sizeof(Water));
    if (w == NULL) return NULL;
    w->level_y = initial_level;
    return w;
}

void water_destroy(Water *w) {
    if (w == NULL) return;
    free(w);
}

int32_t water_get_level(const Water *w) {
    if (w == NULL) return 0;
    return w->level_y;
}
