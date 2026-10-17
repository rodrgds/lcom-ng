#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
#include "../initialize_pixmaps.h"
#include "../cursor_view/cursor_view.h"
#include "../score_view/score_view.h"
#include "menu_view.h"

extern uint8_t* pacman_text_pixmap;
extern uint8_t* play_text_pixmap; 
extern uint8_t* rules_text_pixmap; 
extern uint8_t* exit_text_pixmap;
extern uint8_t* win_text_pixmap; 
extern uint8_t* score_text_pixmap; 
extern uint8_t* game_over_text_pixmap;
extern uint8_t* pause_text_pixmap;
extern uint8_t* rules_description1_pixmap;
extern uint8_t* rules_description2_pixmap;
extern uint8_t* rules_description3_pixmap;
extern uint8_t* rules_description4_pixmap;
extern uint8_t* rules_description5_pixmap;
extern uint8_t* rules_description6_pixmap;
extern uint8_t* back_to_menu_text_pixmap;

extern xpm_image_t pacman_text_info;
extern xpm_image_t play_text_info;
extern xpm_image_t rules_text_info;
extern xpm_image_t exit_text_info;
extern xpm_image_t win_text_info;
extern xpm_image_t score_text_info;
extern xpm_image_t game_over_text_info;
extern xpm_image_t pause_text_info;
extern xpm_image_t rules_description1_info;
extern xpm_image_t rules_description2_info;
extern xpm_image_t rules_description3_info;
extern xpm_image_t rules_description4_info;
extern xpm_image_t rules_description5_info;
extern xpm_image_t rules_description6_info;
extern xpm_image_t back_to_menu_text_info;

void (draw_pacman_text)() {
    draw_xpm(pacman_text_pixmap, pacman_text_info, PACMAN_TEXT_X, PACMAN_TEXT_Y);
}

void (draw_rules_text)() {
    draw_xpm(rules_text_pixmap, rules_text_info, RULES_TEXT_X, RULES_TEXT_Y);
}

void (draw_play_text)() {
    draw_xpm(play_text_pixmap, play_text_info, PLAY_TEXT_X, PLAY_TEXT_Y);
}

void (draw_exit_text)() {
    draw_xpm(exit_text_pixmap, exit_text_info, EXIT_TEXT_X, EXIT_TEXT_Y);
}

void (draw_win_text)() {
    draw_xpm(win_text_pixmap, win_text_info, WIN_TEXT_X, WIN_TEXT_Y);
}

void (draw_game_over_text)() {
    draw_xpm(game_over_text_pixmap, game_over_text_info, GAME_OVER_TEXT_X, GAME_OVER_TEXT_Y);
}

void (draw_score_text)() {
    draw_xpm(score_text_pixmap, score_text_info, SCORE_TEXT_X, SCORE_TEXT_Y);
}

void (draw_pause_text)() {
    draw_xpm(pause_text_pixmap, pause_text_info, PAUSE_TEXT_X, PAUSE_TEXT_Y);
}

void (draw_menu)() {
    draw_cursor();
    draw_pacman_text();
    draw_rules_text();
    draw_play_text();
    draw_exit_text();
}

void (erase_menu)() {
    draw_rectangle(0, 0, 1024, 768, BACKGROUND);
}

void (draw_rules_description)() {
    draw_xpm(rules_description1_pixmap, rules_description1_info, RULES_DESCRIPTION1_X, RULES_DESCRIPTION1_Y);
    draw_xpm(rules_description2_pixmap, rules_description2_info, RULES_DESCRIPTION2_X, RULES_DESCRIPTION2_Y);
    draw_xpm(rules_description3_pixmap, rules_description3_info, RULES_DESCRIPTION3_X, RULES_DESCRIPTION3_Y);
    draw_xpm(rules_description4_pixmap, rules_description4_info, RULES_DESCRIPTION4_X, RULES_DESCRIPTION4_Y);
    draw_xpm(rules_description5_pixmap, rules_description5_info, RULES_DESCRIPTION5_X, RULES_DESCRIPTION5_Y);
    draw_xpm(rules_description6_pixmap, rules_description6_info, RULES_DESCRIPTION6_X, RULES_DESCRIPTION6_Y);
    draw_xpm(back_to_menu_text_pixmap, back_to_menu_text_info, BACK_TO_MENU_X, BACK_TO_MENU_Y);
}

void (draw_win_menu)() {
    erase_menu();
    draw_win_text();
    draw_score_text();
    draw_score();
}

void (draw_game_over_menu)() {
    erase_menu();
    draw_game_over_text();
    draw_score_text();
    draw_score();
}
