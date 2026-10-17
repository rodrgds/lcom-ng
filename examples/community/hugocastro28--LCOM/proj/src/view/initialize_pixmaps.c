#include <lcom/lcf.h>

#include <stdint.h>

#include "include_xpms.h"
#include "view/maze_view/maze_view.h"
#include "view/pacman_view/pacman_view.h"
#include "view/ghosts_view/ghosts_view.h"
#include "view/coins_view/coins_view.h"
#include "view/timer_view/timer_view.h"

uint8_t* maze_pixmap;
uint8_t* pacman_left_pixmap;
uint8_t* pacman_right_pixmap;
uint8_t* pacman_up_pixmap;
uint8_t* pacman_down_pixmap;
uint8_t* ghost_red_pixmap;
uint8_t* ghost_orange_pixmap;
uint8_t* ghost_pink_pixmap;
uint8_t* ghost_blue_pixmap;
uint8_t* small_coin_pixmap;
uint8_t* big_coin_pixmap;

uint8_t* cursor_pixmap;
uint8_t* digit0_pixmap; uint8_t* digit1_pixmap; uint8_t* digit2_pixmap; uint8_t* digit3_pixmap; uint8_t* digit4_pixmap; 
uint8_t* digit5_pixmap; uint8_t* digit6_pixmap; uint8_t* digit7_pixmap; uint8_t* digit8_pixmap; uint8_t* digit9_pixmap;
uint8_t* time_divider_pixmap;
uint8_t* pacman_text_pixmap; uint8_t* play_text_pixmap; uint8_t* rules_text_pixmap; uint8_t* exit_text_pixmap;
uint8_t* win_text_pixmap; uint8_t* score_text_pixmap; uint8_t* game_over_text_pixmap; uint8_t* pause_text_pixmap;
uint8_t* rules_description_pixmap;
uint8_t* back_to_menu_text_pixmap;
uint8_t* rules_description1_pixmap;
uint8_t* rules_description2_pixmap;
uint8_t* rules_description3_pixmap;
uint8_t* rules_description4_pixmap;
uint8_t* rules_description5_pixmap;
uint8_t* rules_description6_pixmap;


xpm_image_t maze_info;
xpm_image_t pacman_left_info;
xpm_image_t pacman_right_info;
xpm_image_t pacman_up_info;
xpm_image_t pacman_down_info;
xpm_image_t ghost_red_info;
xpm_image_t ghost_orange_info;
xpm_image_t ghost_pink_info;
xpm_image_t ghost_blue_info;
xpm_image_t small_coin_info;
xpm_image_t big_coin_info;

xpm_image_t cursor_info;
xpm_image_t digit0_info, digit1_info, digit2_info, digit3_info, digit4_info,
    digit5_info, digit6_info, digit7_info, digit8_info, digit9_info;
xpm_image_t time_divider_info;
xpm_image_t pacman_text_info, play_text_info, rules_text_info, exit_text_info,
    win_text_info, score_text_info, game_over_text_info, pause_text_info;
xpm_image_t rules_description1_info;
xpm_image_t rules_description2_info;
xpm_image_t rules_description3_info;
xpm_image_t rules_description4_info;
xpm_image_t rules_description5_info;
xpm_image_t rules_description6_info;
xpm_image_t back_to_menu_text_info;

void (initialize_maze_pixmap)() {
    maze_pixmap = xpm_load(maze, XPM_INDEXED, &maze_info);
}

void (initialize_pacman_pixmap)() {
    pacman_left_pixmap = xpm_load(pacman_left, XPM_INDEXED, &pacman_left_info);
    pacman_right_pixmap = xpm_load(pacman_right, XPM_INDEXED, &pacman_right_info);
    pacman_up_pixmap = xpm_load(pacman_up, XPM_INDEXED, &pacman_up_info);
    pacman_down_pixmap = xpm_load(pacman_down, XPM_INDEXED, &pacman_down_info);
}

void (initialize_ghost_pixmaps)() {
    ghost_red_pixmap = xpm_load(ghost_red, XPM_INDEXED, &ghost_red_info);
    ghost_orange_pixmap = xpm_load(ghost_orange, XPM_INDEXED, &ghost_orange_info);
    ghost_pink_pixmap = xpm_load(ghost_pink, XPM_INDEXED, &ghost_pink_info);
    ghost_blue_pixmap = xpm_load(ghost_blue, XPM_INDEXED, &ghost_blue_info);
}

void (initialize_coin_pixmaps)() {
    small_coin_pixmap = xpm_load(small_coin, XPM_INDEXED, &small_coin_info);
    big_coin_pixmap = xpm_load(big_coin, XPM_INDEXED, &big_coin_info);
}

void (initialize_menu_button_pixmaps)() {
    pacman_text_pixmap = xpm_load(pacman_text, XPM_INDEXED, &pacman_text_info);
    play_text_pixmap = xpm_load(play_text, XPM_INDEXED, &play_text_info);
    rules_text_pixmap = xpm_load(rules_text, XPM_INDEXED, &rules_text_info);
    exit_text_pixmap = xpm_load(exit_text, XPM_INDEXED, &exit_text_info);
    win_text_pixmap = xpm_load(win_text, XPM_INDEXED, &win_text_info);
    score_text_pixmap = xpm_load(score_text, XPM_INDEXED, &score_text_info);
    game_over_text_pixmap = xpm_load(game_over_text, XPM_INDEXED, &game_over_text_info);
    pause_text_pixmap = xpm_load(pause_text, XPM_INDEXED, &pause_text_info);
    rules_description1_pixmap = xpm_load(rules_description1, XPM_INDEXED, &rules_description1_info);
    rules_description2_pixmap = xpm_load(rules_description2, XPM_INDEXED, &rules_description2_info);    
    rules_description3_pixmap = xpm_load(rules_description3, XPM_INDEXED, &rules_description3_info);
    rules_description4_pixmap = xpm_load(rules_description4, XPM_INDEXED, &rules_description4_info);
    rules_description5_pixmap = xpm_load(rules_description5, XPM_INDEXED, &rules_description5_info);
    rules_description6_pixmap = xpm_load(rules_description6, XPM_INDEXED, &rules_description6_info);
    back_to_menu_text_pixmap = xpm_load(back_to_menu_text, XPM_INDEXED, &back_to_menu_text_info);
}

void (initialize_util_pixmaps)() {
    cursor_pixmap = xpm_load(cursor, XPM_INDEXED, &cursor_info);

    digit0_pixmap = xpm_load(digit0, XPM_INDEXED, &digit0_info);
    digit1_pixmap = xpm_load(digit1, XPM_INDEXED, &digit1_info);
    digit2_pixmap = xpm_load(digit2, XPM_INDEXED, &digit2_info);
    digit3_pixmap = xpm_load(digit3, XPM_INDEXED, &digit3_info);
    digit4_pixmap = xpm_load(digit4, XPM_INDEXED, &digit4_info);
    digit5_pixmap = xpm_load(digit5, XPM_INDEXED, &digit5_info);
    digit6_pixmap = xpm_load(digit6, XPM_INDEXED, &digit6_info);
    digit7_pixmap = xpm_load(digit7, XPM_INDEXED, &digit7_info);
    digit8_pixmap = xpm_load(digit8, XPM_INDEXED, &digit8_info);
    digit9_pixmap = xpm_load(digit9, XPM_INDEXED, &digit9_info);

    time_divider_pixmap = xpm_load(time_divider, XPM_INDEXED, &time_divider_info);
}

void (initialize_all_pixmaps)() {
    initialize_maze_pixmap();
    initialize_pacman_pixmap();
    initialize_ghost_pixmaps();
    initialize_coin_pixmaps();

    initialize_menu_button_pixmaps();
    initialize_util_pixmaps();
}

void (draw_game_elements)() {
    draw_maze();
    draw_pacman();
    draw_ghosts();
    draw_timer();
    draw_small_coins();
    draw_big_coins();
}
