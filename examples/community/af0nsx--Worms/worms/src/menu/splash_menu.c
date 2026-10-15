#include "splash_menu.h"

#include "../../engine/graphics/renderer.h"
#include "../../engine/graphics/sprite.h"
#include "../../assets/assets.h"
#include "../../config.h"

static int s_ticks = 0;

void splash_menu_reset(void) {
    s_ticks = 0;
}

void splash_menu_draw(void) {
    renderer_draw_rect(0, 0, SCREEN_W, SCREEN_H, COLOR_MENU_BACKDROP);

    const Sprite *logo = assets_get_logo();
    if (logo != NULL) {
        int lx = (SCREEN_W - (int)sprite_get_width(logo))  / 2;
        int ly = (SCREEN_H - (int)sprite_get_height(logo)) / 2;
        renderer_draw_sprite(logo, lx, ly);
    }

    renderer_flip();
}

SplashAction splash_menu_update(void) {
    s_ticks++;
    return (s_ticks >= SPLASH_DURATION_TICKS) ? SPLASH_ACTION_DONE : SPLASH_ACTION_NONE;
}
