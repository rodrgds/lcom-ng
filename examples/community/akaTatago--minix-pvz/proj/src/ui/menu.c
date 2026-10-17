#include "menu.h"
#include "v_card.h"
#include "game.h"
#include "font.h"
#include "name_input.h"

//button coordinates
#define BTN_W 200
#define BTN_H 60
#define BTN_PLAY_X 300
#define BTN_PLAY_Y 160
#define BTN_LEVELS_X 300
#define BTN_LEVELS_Y 250
#define BTN_SCORE_X 300
#define BTN_SCORE_Y 340
#define BTN_EXIT_X 300
#define BTN_EXIT_Y 430

static bool prev_lb = false;

void menu_init() {
    prev_lb = false;
}

void menu_render(int mouse_x, int mouse_y) {
    vg_clear(0x111111);

    font_draw_string("MINIX PVZ", 346, 80, 0xEEEEEE);

    uint32_t play_color = 0x005500;
    uint32_t levels_color = 0x555500;
    uint32_t exit_color = 0x550000;
    uint32_t score_color = 0x550055;

    // detect hover
    if (mouse_x >= BTN_PLAY_X && mouse_x <= BTN_PLAY_X + BTN_W && 
        mouse_y >= BTN_PLAY_Y && mouse_y <= BTN_PLAY_Y + BTN_H) {
        play_color = 0x00FF00;
    }

    if (mouse_x >= BTN_LEVELS_X && mouse_x <= BTN_LEVELS_X + BTN_W && 
        mouse_y >= BTN_LEVELS_Y && mouse_y <= BTN_LEVELS_Y + BTN_H) {
        levels_color = 0xFFFF00;
    }

    if (mouse_x >= BTN_EXIT_X && mouse_x <= BTN_EXIT_X + BTN_W && 
        mouse_y >= BTN_EXIT_Y && mouse_y <= BTN_EXIT_Y + BTN_H) {
        exit_color = 0xFF0000;
    }

    if (mouse_x >= BTN_SCORE_X && mouse_x <= BTN_SCORE_X + BTN_W &&
        mouse_y >= BTN_SCORE_Y && mouse_y <= BTN_SCORE_Y + BTN_H) {
        score_color = 0xAA00AA;
    }

    // draw buttons
    vg_draw_rectangle(BTN_PLAY_X, BTN_PLAY_Y, BTN_W, BTN_H, play_color);
    vg_draw_rectangle(BTN_LEVELS_X, BTN_LEVELS_Y, BTN_W, BTN_H, levels_color);
    vg_draw_rectangle(BTN_EXIT_X, BTN_EXIT_Y, BTN_W, BTN_H, exit_color);
    vg_draw_rectangle(BTN_SCORE_X, BTN_SCORE_Y, BTN_W, BTN_H, score_color);

    font_draw_string("PLAY", BTN_PLAY_X + 76, BTN_PLAY_Y + 23, 0xFFFFFF);
    font_draw_string("LEVELS", BTN_LEVELS_X + 64, BTN_LEVELS_Y + 23, 0xFFFFFF);
    font_draw_string("EXIT", BTN_EXIT_X + 76, BTN_EXIT_Y + 23, 0xFFFFFF);
    font_draw_string("SCORES", BTN_SCORE_X + 60, BTN_SCORE_Y + 23, 0xFFFFFF);

}

void menu_on_mouse(struct packet *pp, int mouse_x, int mouse_y) {
    // handle click
    if (pp->lb && !prev_lb) {
        
        if (mouse_x >= BTN_PLAY_X && mouse_x <= BTN_PLAY_X + BTN_W && 
            mouse_y >= BTN_PLAY_Y && mouse_y <= BTN_PLAY_Y + BTN_H) {

           // game_init_level(1);
            //game_change_state(PLAYING);

            name_input_init();
            game_init_level(1);
            game_change_state(NAME_INPUT);
        }

        if (mouse_x >= BTN_LEVELS_X && mouse_x <= BTN_LEVELS_X + BTN_W && 
            mouse_y >= BTN_LEVELS_Y && mouse_y <= BTN_LEVELS_Y + BTN_H) {
            game_change_state(LEVEL_SELECT);
        }

        if (mouse_x >= BTN_EXIT_X && mouse_x <= BTN_EXIT_X + BTN_W && 
            mouse_y >= BTN_EXIT_Y && mouse_y <= BTN_EXIT_Y + BTN_H) {
            game_set_quit();
        }
        if (mouse_x >= BTN_SCORE_X && mouse_x <= BTN_SCORE_X + BTN_W &&
            mouse_y >= BTN_SCORE_Y && mouse_y <= BTN_SCORE_Y + BTN_H) {
            game_change_state(SCOREBOARD);
        }
    }
    prev_lb = pp->lb;
}

void menu_on_key(uint8_t scancode) {
    // press esc
    if (scancode == 0x81) {
        game_set_quit();
    }
}

void level_select_render(int mouse_x, int mouse_y) {
    vg_clear(0x111111);

    font_draw_string("SELECT A LEVEL", 310, 100, 0xFFFFFF);

    for (int i = 0; i < 10; i++) {
        int row = i / 5;
        int col = i % 5;
        int bx = SQ_START_X + col * (SQ_SIZE + SQ_GAP);
        int by = SQ_START_Y + row * (SQ_SIZE + SQ_GAP);

        uint32_t color = 0x444444; 
        if (mouse_x >= bx && mouse_x <= bx + SQ_SIZE &&
            mouse_y >= by && mouse_y <= by + SQ_SIZE) {
            color = 0x0088FF; 
        }

        vg_draw_rectangle(bx, by, SQ_SIZE, SQ_SIZE, color);
        
        int text_offset_x = (i == 9) ? 19 : 25; 
        font_draw_number(i + 1, bx + text_offset_x, by + 22, 0xFFFFFF);
    }

    uint32_t back_color = 0x555555;
    if (mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 410 && mouse_y <= 460) {
        back_color = 0x888888;
    }
    vg_draw_rectangle(300, 410, 200, 50, back_color);
    font_draw_string("BACK", 376, 426, 0xFFFFFF);
}
