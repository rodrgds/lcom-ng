#include <lcom/lcf.h>
#include "mainmenu.h"
#include "sprite.h"

//Sprites
extern Sprite *background;
extern Sprite *cursor;
extern Sprite *play;
extern Sprite *singleplayer_but;
extern Sprite *instructions_but;
extern Sprite *back_but;
extern Sprite *continue_but;
extern Sprite *exit_but;
extern Sprite *exit_mini_but;
extern Sprite *menu_mini_but;

//images to array
extern xpm_image_t backgrounds[9];   


void show_entrada() { 
    draw_sprite(background);
    draw_sprite(play);
}

void show_cursor() {
    draw_sprite(cursor);
}

void show_menu() {
    background->map = (uint32_t*)backgrounds[1].bytes;
    instructions_but->xi = 455;
    instructions_but ->yi = 375;
    instructions_but->xf = instructions_but->xi+instructions_but->width;
    instructions_but->yf = instructions_but->yi+instructions_but->height;
    exit_but->xi = 455;
    exit_but->yi= 458;
    exit_but->xf = exit_but->xi+exit_but->width;
    exit_but->yf = exit_but->yi+exit_but->height;
    draw_sprite(background);
    draw_sprite(singleplayer_but);
    draw_sprite(instructions_but);
    draw_sprite(exit_but);
}

void show_instructions_menu(int num) {
    background->map = (uint32_t*)backgrounds[num].bytes;
    draw_sprite(background);
    draw_sprite(back_but);
}

void show_pause_menu() {
    background->map = (uint32_t*)backgrounds[5].bytes;
    instructions_but->xi = 455;
    instructions_but ->yi = 359;
    instructions_but->xf = instructions_but->xi+instructions_but->width;
    instructions_but->yf = instructions_but->yi+instructions_but->height;
    exit_but->xi = 455;
    exit_but->yi= 446;
    exit_but->xf = exit_but->xi+exit_but->width;
    exit_but->yf = exit_but->yi+exit_but->height;
    draw_sprite(background);
    draw_sprite(continue_but);
    draw_sprite(instructions_but);
    draw_sprite(exit_but);
}

void show_game_over_menu() {
    background->map = (uint32_t*)backgrounds[8].bytes;
    draw_sprite(background);
    draw_sprite(menu_mini_but);
    draw_sprite(exit_mini_but);
}


