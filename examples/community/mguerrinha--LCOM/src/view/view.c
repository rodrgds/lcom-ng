#include "view.h"

uint8_t *main_buffer;
uint8_t *secondary_buffer;
uint8_t *chosen_buffer;
uint8_t *menu_buffer;
uint8_t *end_menu_buffer;
uint8_t *controls_menu_buffer;
uint32_t buffer_size;
bool menu_drawn = false;
bool endmenu_drawn = false;
bool controlsmenu_drawn = false;

extern Sprite *mouse;
extern Sprite *menu;
extern Sprite *player1;
extern Sprite *player2;
extern Sprite *p1_up;
extern Sprite *p1_left;
extern Sprite *p1_right;
extern Sprite *p2_up;
extern Sprite *p2_left;
extern Sprite *p2_right;
extern Sprite *arena;
extern Sprite *num0;
extern Sprite *num1;
extern Sprite *num2;
extern Sprite *num3;
extern Sprite *num4;
extern Sprite *num5;
extern Sprite *num6;
extern Sprite *num7;
extern Sprite *num8;
extern Sprite *num9;
extern Sprite *percentage;
extern Sprite *twopoints;
extern Sprite *bar;
extern Sprite *p1win;
extern Sprite *p2win;
extern Sprite *controls;
extern Sprite *numSprites[10];
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern MenuState menuState;
extern PlayerInfo player1_info;
extern PlayerInfo player2_info;
extern Arena arena_matrix;
extern int p1_counter_territory;
extern int p2_counter_territory;
extern int game_counter;
extern rtc_info time_info;

void (draw_next_frame)() {
    
    switch (menuState) {
        case MENU:
            draw_initial_menu();
            draw_mouse();
            break;
        case CONTROLS:
            draw_controls_menu();
            draw_mouse();
            break;
        case PLAY:
            draw_game();
            break;
        case END:
            draw_end_menu();
            draw_mouse();
            break;
    }
}

void (free_menu_buffer)() {
    free(menu_buffer);
    menu_buffer = NULL;
    menu_drawn = false;
}

void (draw_initial_menu)() {
    if (!menu_drawn) {
        menu_buffer = (uint8_t*) malloc(buffer_size);
        if(draw_sprite_xpm(menu, 0, 0, menu_buffer)) return;
        menu_drawn = true;
    }
    if(memcpy(chosen_buffer, menu_buffer, buffer_size) == NULL) return;
    draw_time();
}

void (free_controls_menu_buffer)() {
    free(controls_menu_buffer);
    controls_menu_buffer = NULL;
    controlsmenu_drawn = false;
}

void (draw_controls_menu)() {
    if (!controlsmenu_drawn) {
        controls_menu_buffer = (uint8_t*) malloc(buffer_size);
        if(draw_sprite_xpm(controls, 0, 0, controls_menu_buffer)) return;
        controlsmenu_drawn = true;
    }
    if (memcpy(chosen_buffer, controls_menu_buffer, buffer_size) == NULL) return;
}

void (draw_game)() {
    if (draw_arena(arena)) return;
    if (player1_info.dir == UP) {
        if (draw_sprite_xpm(p1_up, player1_info.x, player1_info.y, chosen_buffer)) return;
    }
    else if (player1_info.dir == DOWN || player1_info.dir == STOP) {
        if (draw_sprite_xpm(player1, player1_info.x, player1_info.y, chosen_buffer)) return;
    }
    else if (player1_info.dir == LEFT) {
        if (draw_sprite_xpm(p1_left, player1_info.x, player1_info.y, chosen_buffer)) return;
    }
    else if (player1_info.dir == RIGHT) {
        if (draw_sprite_xpm(p1_right, player1_info.x, player1_info.y, chosen_buffer)) return;

    }
    if (player2_info.dir == UP) {
        if (draw_sprite_xpm(p2_up, player2_info.x, player2_info.y, chosen_buffer)) return;
    }
    else if (player2_info.dir == DOWN || player2_info.dir == STOP) {
        if (draw_sprite_xpm(player2, player2_info.x, player2_info.y, chosen_buffer)) return;
    }
    else if (player2_info.dir == LEFT) {
        if (draw_sprite_xpm(p2_left, player2_info.x, player2_info.y, chosen_buffer)) return;
    }
    else if (player2_info.dir == RIGHT) {
        if (draw_sprite_xpm(p2_right, player2_info.x, player2_info.y, chosen_buffer)) return;
    }
    draw_scoreboard();
}

void (free_end_menu_buffer)() {
    free(end_menu_buffer);
    end_menu_buffer = NULL;
    endmenu_drawn = false;
}

void (draw_endmenu_score)(int winner) {
    int tens = winner / 10;
    int units = winner % 10;
    if (draw_sprite_xpm(numSprites[tens], 415, 299, end_menu_buffer)) return;
    if (draw_sprite_xpm(numSprites[units], 440, 299, end_menu_buffer)) return;
    if (draw_sprite_xpm(percentage, 465, 299, end_menu_buffer)) return;
}

void (draw_end_menu)() {
    if (!endmenu_drawn) {
        end_menu_buffer = (uint8_t*) malloc(buffer_size);
        if (p1_counter_territory > p2_counter_territory) {
            if (draw_sprite_xpm(p1win, 0, 0, end_menu_buffer)) return;
            draw_endmenu_score(p1_counter_territory);
        } else if (p1_counter_territory < p2_counter_territory) {
            if (draw_sprite_xpm(p2win, 0, 0, end_menu_buffer)) return;
            draw_endmenu_score(p2_counter_territory);
        } else {
            draw_initial_menu();
            menuState = MENU;
        }
        endmenu_drawn = true;
    }
    if(memcpy(chosen_buffer, end_menu_buffer, buffer_size) == NULL) return;
}

void (draw_mouse)() {
    if (draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y, chosen_buffer)) return;
}

int (draw_arena)(Sprite *sprite){
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t color;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            if (arena_matrix.map[h][w] == 0) {
                color = sprite->map[w + h * width];
                if (draw_pixel(w, h, color, chosen_buffer)) return 1;
            }
            else if (arena_matrix.map[h][w] == 1) {
                color = 0xff0000;
                if (draw_pixel(w, h, color, chosen_buffer)) return 1;
            }
            else if (arena_matrix.map[h][w] == 2) {
                color = 0x0000ff;
                if (draw_pixel(w, h, color, chosen_buffer)) return 1;
            }
            else if (arena_matrix.map[h][w] == 3) {
                color = 0xe95833;
                if (draw_pixel(w, h, color, chosen_buffer)) return 1;
            }
            else if (arena_matrix.map[h][w] == 4) {
                color = 0x4052d6;
                if (draw_pixel(w, h, color, chosen_buffer)) return 1;
            }
        }
    }
    return 0;
}

int (draw_numbers)(Sprite *num, int x, int y) {
    if (draw_sprite_xpm(num, x, y, chosen_buffer)) return 1;
    return 0;
}

int (draw_scoreboard)() {
    int tens_p1 = p1_counter_territory / 10;
    int tens_p2 = p2_counter_territory / 10;
    int units_p1 = p1_counter_territory % 10;
    int units_p2 = p2_counter_territory % 10;
    int min = game_counter / 60;
    int sec = game_counter % 60;
    int tens_sec = sec / 10;
    int units_sec = sec % 10;
    if (draw_numbers(numSprites[tens_p1], 445, 568)) return 1;
    if (draw_numbers(numSprites[tens_p2], 705, 568)) return 1;
    if (draw_numbers(numSprites[units_p1], 470, 568)) return 1;
    if (draw_numbers(numSprites[units_p2], 730, 568)) return 1;
    if (draw_sprite_xpm(percentage, 495, 568, chosen_buffer)) return 1;
    if (draw_sprite_xpm(percentage, 755, 568, chosen_buffer)) return 1;
    if (draw_numbers(numSprites[min], 40, 568)) return 1;
    if (draw_sprite_xpm(twopoints, 65, 574, chosen_buffer)) return 1;
    if (draw_numbers(numSprites[tens_sec], 90, 568)) return 1;
    if (draw_numbers(numSprites[units_sec], 115, 568)) return 1;
    return 0;
}

int (draw_time)() {
    int h1 = time_info.hours / 10;
    int h2 = time_info.hours % 10;
    int min1 = time_info.minutes / 10;
    int min2 = time_info.minutes % 10;
    int day1 = time_info.day / 10;
    int day2 = time_info.day % 10;
    int month1 = time_info.month / 10;
    int month2 = time_info.month % 10;
    int year1 = time_info.year / 10;
    int year2 = time_info.year % 10;
    if (draw_numbers(numSprites[h1], 35, 15)) return 1;
    if (draw_numbers(numSprites[h2], 60, 15)) return 1;
    if (draw_sprite_xpm(twopoints, 90, 18, chosen_buffer)) return 1;
    if (draw_numbers(numSprites[min1], 110, 15)) return 1;
    if (draw_numbers(numSprites[min2], 135, 15)) return 1;
    if (draw_numbers(numSprites[day1], 560, 550)) return 1;
    if (draw_numbers(numSprites[day2], 585, 550)) return 1;
    if (draw_sprite_xpm(bar, 610, 550, chosen_buffer)) return 1;
    if (draw_numbers(numSprites[month1], 635, 550)) return 1;
    if (draw_numbers(numSprites[month2], 660, 550)) return 1;
    if (draw_sprite_xpm(bar, 690, 550, chosen_buffer)) return 1;
    if (draw_numbers(numSprites[year1], 715, 550)) return 1;
    if (draw_numbers(numSprites[year2], 740, 550)) return 1;
    return 0;
}

int (draw_sprite_xpm)(Sprite *sprite, int x, int y, uint8_t *buffer) {
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t color;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            color = sprite->map[w + h * width];
            if (color == 0xFFFFFE) continue;
            if (draw_pixel(x + w, y + h, color, buffer)) return 1;
        }
    }
    return 0;
}

int (choose_frame_buffer)(uint16_t mode) {
    if (set_frame_buffer(mode, &main_buffer)) return 1;
    buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    if (DOUBLE_BUFFERING) {
        secondary_buffer = (uint8_t *) malloc(buffer_size);
        chosen_buffer = secondary_buffer;
    } else {
        chosen_buffer = main_buffer;
    }
    return 0;
}

void (sync_to_main_buffer)() {
    if (memcpy(main_buffer, secondary_buffer, buffer_size) == NULL) return;
}
