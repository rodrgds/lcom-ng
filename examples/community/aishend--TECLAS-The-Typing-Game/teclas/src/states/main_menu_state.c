#include "main_menu_state.h"
#include "../cursor.h"
#include "../../xpm/start/main_menu_background.xpm"
#include "../ui.h"
#include "../video.h"
#include <lcom/lcf.h>

#define BTN_W 320
#define BTN_H 56
#define BTN_GAP 82
#define BTN_Y0 420
#define BTN_X ((screen_width - BTN_W) / 2)
#define BTN_Y(idx) (BTN_Y0 + (idx) * BTN_GAP)

#define NUM_BUTTONS 4

static const char *btn_labels[NUM_BUTTONS] = {"FAST TECLAS", "DINO GAME", "SURVIVAL", "LEADERBOARD"};
static const GameState btn_targets[NUM_BUTTONS] = {TYPING_TEST, DINO_GAME, SURVIVAL_GAME, LEADERBOARD};

static uint8_t *bg_cache;

int main_menu_state_init() {
    bg_cache = video_cache_xpm((xpm_map_t)main_menu_background);
    
    return bg_cache == NULL ? 1 : 0;
}

void main_menu_state_draw() {
    video_draw_background(bg_cache);
    for (int i = 0; i < NUM_BUTTONS; i++) {
        ui_draw_button(BTN_X, BTN_Y(i), BTN_W, BTN_H, btn_labels[i], ui_button_hit(BTN_X, BTN_Y(i), BTN_W, BTN_H, cursor_x, cursor_y)); 
    }
}

GameState main_menu_state_handle_click() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (ui_button_hit(BTN_X, BTN_Y(i), BTN_W, BTN_H, cursor_x, cursor_y))return btn_targets[i];
    }

    return MAIN_MENU;
}

void main_menu_state_destroy() {
    free(bg_cache);
    bg_cache = NULL;
}
