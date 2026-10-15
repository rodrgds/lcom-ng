#include "app.h"
#include <stdio.h>
int main(void) {
    app_t app;
    app_init(&app);
    app_key(&app, 0x39);
    app_key(&app, 0xB9);
    app_update(&app);
    app_key(&app, 0x19);
    int frozen_y = app.bird_y_fp;
    for (int i = 0; i < 60; i++) app_update(&app);
    if (app.bird_y_fp != frozen_y) { fputs("Pause allowed the bird to fall\n", stderr); return 1; }
    app_key(&app, 0x39);
    app_update(&app);
    if (app.bird_y_fp == frozen_y) { fputs("Space did not resume flight\n", stderr); return 2; }
    app_key(&app, 0x3B);
    frozen_y = app.bird_y_fp;
    app_key(&app, 0x39);
    app_mouse(&app, 10, 0, true);
    for (int i = 0; i < 60; i++) app_update(&app);
    if (app.bird_y_fp != frozen_y || !app.help) { fputs("Help did not capture flight input\n", stderr); return 3; }
    app_key(&app, 0x01);
    if (app.help) { fputs("Escape did not dismiss help\n", stderr); return 4; }
    app_init(&app);
    app_mouse(&app, 0, -4, true);
    if (app.state != 1) { fputs("Click did not activate the visible Play button\n", stderr); return 5; }
    puts("Flappy pause, resume, help capture, and mouse play work");
    return 0;
}
