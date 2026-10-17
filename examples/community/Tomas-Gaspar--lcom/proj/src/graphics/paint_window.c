#include <lcom/lcf.h>

#include "paint_window.h"

#include "../controllers/video/macro_video.h"
#include "../controllers/video/video_gr.h"
#include "../cursor.h"
#include "drawing_state_graphics.h"

#include "../getters.h"

#include "img/screens/guessLight.xpm"
#include "img/screens/guessDark.xpm"
#include "img/screens/waitingLight.xpm"
#include "img/screens/waitingDrawingLight.xpm"
#include "img/screens/waitingDark.xpm"
#include "img/screens/waitingDrawingDark.xpm"
#include "img/screens/waitingGuessingLight.xpm"
#include "img/screens/waitingGuessingDark.xpm"

#include "img/buttons/START.xpm"
#include "img/buttons/GUESS.xpm"
#include "img/buttons/EXIT.xpm"
#include "img/buttons/SINGLEPLAYER.xpm"
#include "img/buttons/MULTIPLAYER.xpm"

int (paint_image)(char* map[], uint16_t x, uint16_t y){
    xpm_image_t img;

    if (xpm_load((xpm_map_t) map, XPM_8_8_8, &img) == NULL) return 1;

    if (vg_draw_xpm(&img, x, y, true)) return 1;
    return 0;
}

int (paint_image_cursor)(char* map[], int16_t x, int16_t y){
    xpm_image_t img;

    if (xpm_load((xpm_map_t) map, XPM_8_8_8, &img) == NULL) return 1;

    if (vg_draw_xpm(&img, x, y, false)) {
        printf("error\n");
        return 1;
    }

    return 0;
}

int paint_main_menu() {
    if (get_color_theme() == LIGHTMODE) {
        if (vg_draw_rectangle(0, 0, 800, 600, WHITE) != 0)
        return 1;
    }
    else {
        if (vg_draw_rectangle(0, 0, 800, 600, 0x121212) != 0)
        return 1;
    }
    paint_image(singlePlayer_button, 300, 140);
    paint_image(multiPlayer_button, 300, 250);
    paint_image(exit_button, 300, 360);

    return 0;
}

int paint_multiplayer_menu() {
    if (get_color_theme() == LIGHTMODE) {
        if (vg_draw_rectangle(0, 0, 800, 600, WHITE) != 0)
        return 1;
    }
    else {
        if (vg_draw_rectangle(0, 0, 800, 600, 0x121212) != 0)
        return 1;
    }
    paint_image(start_button, 300, 140);
    paint_image(guess_button, 300, 250);
    paint_image(exit_button, 300, 360);

    return 0;
}

int paint_guessing() {
    
    if (get_color_theme() == LIGHTMODE) {
        paint_image(guessLight, 0, 0);
    }
    else {
        paint_image(guessDark, 0, 0);
    }

    return 0;
}

int paint_waiting_drawing() {
    if (get_color_theme() == LIGHTMODE) {
        if (vg_draw_rectangle(0, 0, 800, 600, 0xffffff) != 0)
            return 1;
        paint_image(waitingLight, 0, 0);
        paint_image(waitingDrawingLight,260,400);
    }
    else {
        if (vg_draw_rectangle(0, 0, 800, 600, 0x121212) != 0)
            return 1;
        paint_image(waitingDark, 0, 0);
        paint_image(waitingDrawingDark,260,400);
    }

    return 0;
}

int paint_waiting_guessing() {
 if (get_color_theme() == LIGHTMODE) {
        if (vg_draw_rectangle(0, 0, 800, 600, 0xffffff) != 0)
            return 1;
        paint_image(waitingLight, 0, 0);
        paint_image(waitingGuessingLight,260,400);

    }
    else {
        if (vg_draw_rectangle(0, 0, 800, 600, 0x121212) != 0)
            return 1;
        paint_image(waitingDark, 0, 0);
        paint_image(waitingGuessingDark,260,400);
    }

    return 0;
}
