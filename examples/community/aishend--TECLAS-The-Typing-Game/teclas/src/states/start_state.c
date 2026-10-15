#include "start_state.h"

#include <lcom/lcf.h>

#include "../../xpm/start/start_page.xpm"
#include "../video.h"

static uint8_t *bg_cache;

int start_state_init() {
    bg_cache = video_cache_xpm((xpm_map_t)start_page);
    return bg_cache == NULL ? 1 : 0;
}

void start_state_draw() { video_draw_background(bg_cache); }

void start_state_destroy() {
    free(bg_cache);
    bg_cache = NULL;
}
