#include "video.h"
#include "xpms/mainMenu.h"
#include "xpms/spaceship.h"
#include "xpms/asteroid.h"
#include "xpms/cursor.h"
#include "xpms/bullet.h"
#include "xpms/0.h"
#include "xpms/1.h"
#include "xpms/2.h"
#include "xpms/3.h"
#include "xpms/4.h"
#include "xpms/5.h"
#include "xpms/6.h"
#include "xpms/7.h"
#include "xpms/8.h"
#include "xpms/9.h"
#include "xpms/gameOver.h"
#include "xpms/helpmenu.h"

#include "info.h"

xpm_row_t const *digit_xpms[10] = {
    zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm,
    five_xpm, six_xpm, seven_xpm, eight_xpm, nine_xpm
};

extern int x_init;
extern int y_init;
extern uint8_t* double_buffer;

void draw_main_menu(){
    vg_draw_xpm(mainMenu_xpm, 0, 0, double_buffer);
}

void draw_help_menu(){
    vg_draw_xpm(helpMenu_xpm, 0, 0, double_buffer);
}

void draw_spaceship(int x){
    vg_draw_xpm(spaceship_xpm, x, SPACESHIP_Y_AXIS, double_buffer);
}

void draw_asteroid(int x, int y){
    vg_draw_xpm(asteroid_xpm, x, y, double_buffer);
}

void draw_mouse(){
    vg_draw_xpm(mouse_xpm, x_init, y_init, double_buffer);
}

void draw_bullet(int x, int y){
    vg_draw_xpm(bullet_xpm, x, y, double_buffer);
}

void draw_score(int score, int x, int y) {

    if (score > 99999) score = 99999;
    if (score < 0) score = 0;
    
    int digits[5];
    digits[0] = (score / 10000) % 10;
    digits[1] = (score / 1000) % 10;
    digits[2] = (score / 100) % 10;
    digits[3] = (score / 10) % 10;
    digits[4] = score % 10;

    for (int i = 0; i < 5; i++) {
        vg_draw_xpm(digit_xpms[digits[i]], x, y, double_buffer);
        x += 21;
    }
}

void draw_game_over() {
    vg_draw_xpm(gameOver_xpm, 0, 0, double_buffer);
}

void(draw_time)(int time){
    if (time < 0) time = 0;
    int minutes = time / 60;
    int seconds = time % 60;

    int min_digits[2];
    min_digits[0] = (minutes / 10) % 10;
    min_digits[1] = minutes % 10;

    int sec_digits[2];
    sec_digits[0] = (seconds / 10) % 10;
    sec_digits[1] = seconds % 10;

    int x = 120;
    int y = 4;

    // Draw minutes
    vg_draw_xpm(digit_xpms[min_digits[0]], x, y, double_buffer);
    vg_draw_xpm(digit_xpms[min_digits[1]], x + 21, y, double_buffer);


    // Draw seconds
    vg_draw_xpm(digit_xpms[sec_digits[0]], x + 21 * 2 + 8, y, double_buffer);
    vg_draw_xpm(digit_xpms[sec_digits[1]], x + 21 * 3 + 8, y, double_buffer);
}

