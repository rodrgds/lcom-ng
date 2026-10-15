#include "pvz_platform.h"
#include <stdio.h>
#include <string.h>

#include "scene.h"
#include "state.h"
#include "video.h"
#include "render.h"
#include "constants.h"
#include "wave.h"
#include "rtc.h"
#include "plant_select.h"
#include "sprites.h"
#include "input.h"

static const uint8_t glyphs[26][7] = {
    {0x04,0x0A,0x11,0x1F,0x11,0x11,0x11}, /* A */
    {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}, /* B */
    {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}, /* C */
    {0x1C,0x12,0x11,0x11,0x11,0x12,0x1C}, /* D */
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}, /* E */
    {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}, /* F */
    {0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}, /* G */
    {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}, /* H */
    {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}, /* I */
    {0x07,0x02,0x02,0x02,0x02,0x12,0x0C}, /* J */
    {0x11,0x12,0x14,0x18,0x14,0x12,0x11}, /* K */
    {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}, /* L */
    {0x11,0x1B,0x15,0x11,0x11,0x11,0x11}, /* M */
    {0x11,0x19,0x15,0x13,0x11,0x11,0x11}, /* N */
    {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}, /* O */
    {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}, /* P */
    {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}, /* Q */
    {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}, /* R */
    {0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E}, /* S */
    {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}, /* T */
    {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}, /* U */
    {0x11,0x11,0x11,0x11,0x11,0x0A,0x04}, /* V */
    {0x11,0x11,0x11,0x15,0x15,0x1B,0x11}, /* W */
    {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}, /* X */
    {0x11,0x11,0x0A,0x04,0x04,0x04,0x04}, /* Y */
    {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}  /* Z */
};

static const uint8_t number_glyphs[10][7] = {
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}, /* 0 */
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}, /* 1 */
    {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F}, /* 2 */
    {0x1F,0x02,0x04,0x02,0x01,0x11,0x0E}, /* 3 */
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}, /* 4 */
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E}, /* 5 */
    {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E}, /* 6 */
    {0x1F,0x01,0x02,0x04,0x08,0x08,0x08}, /* 7 */
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}, /* 8 */
    {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C}  /* 9 */
};

static void draw_char(uint16_t x, uint16_t y, char c, int scale, uint32_t color) {
    const uint8_t *g;
    int row, col;

    if (c >= 'A' && c <= 'Z') {
        g = glyphs[c - 'A'];
    } else if (c >= '0' && c <= '9') {
        g = number_glyphs[c - '0'];
    } else {
        return;
    }

    for (row = 0; row < 7; row++) {
        for (col = 0; col < 5; col++) {
            if (g[row] & (0x10 >> col)) {
                vg_draw_rectangle(x + col * scale, y + row * scale,
                                  scale, scale, color);
            }
        }
    }
}

void draw_string(uint16_t x, uint16_t y, const char *str, int scale, uint32_t color) {
    int i = 0;

    while (str[i] != '\0') {
        draw_char(x + i * 6 * scale, y, str[i], scale, color);
        i++;
    }
}

void draw_sun_counter() {
    const GameInfo *info = get_game_info();
    char buf[16];
    int len = 0;
    int text_width;
    int draw_x;
    int draw_y = 68;

    sprintf(buf, "%d", info->sun_count);

    while (buf[len] != '\0') len++;
    text_width = len * 6 * 2;
    draw_x = 25 + (83 - text_width) / 2;

    draw_string((uint16_t) draw_x, (uint16_t) draw_y, buf, 2, 0x000000);
}

static void draw_text_centered(uint16_t y, const char *str, int scale, uint32_t color) {
    int len = 0;
    const char *p;
    uint16_t x;
    int i;

    for (p = str; *p; p++) len++;
    x = (800 - len * 6 * scale) / 2;

    for (i = 0; i < len; i++) {
        draw_char(x + i * 6 * scale, y, str[i], scale, color);
    }
}

static void draw_text_right(uint16_t right_x, uint16_t y, const char *str, int scale, uint32_t color) {
    int len = 0;
    const char *p;
    uint16_t x;
    int i;

    for (p = str; *p; p++) len++;
    x = right_x - len * 6 * scale;

    for (i = 0; i < len; i++) {
        draw_char(x + i * 6 * scale, y, str[i], scale, color);
    }
}

static void draw_last_played(uint16_t right_x, uint16_t y, uint32_t color) {
    char buf[32];

    rtc_format_last_played(buf, sizeof(buf));
    draw_text_right(right_x, y, buf, 2, color);
}

static void menu_button(int x, int y, int w, int h, const char *label, bool primary) {
    bool hover = get_mouse_x() >= x && get_mouse_x() < x + w && get_mouse_y() >= y && get_mouse_y() < y + h;
    uint32_t color = primary ? (hover ? 0x4F833A : 0x3B662C) : (hover ? 0x40543C : 0x293C2B);
    vg_draw_rectangle(x, y, w, h, color);
    vg_draw_rectangle(x, y, w, 2, primary ? 0xB9DA80 : 0x6A8065);
    draw_string(x + (w - (int)strlen(label) * 12) / 2, y + (h - 14) / 2, label, 2, 0xFFF3D3);
}

static void render_main_menu(void) {
    draw_map();
    vg_draw_rectangle(104, 66, 592, 462, 0x152D21);
    vg_draw_rectangle(104, 66, 592, 4, 0x9AD671);
    draw_text_centered(108, "PLANTS VS", 4, 0xFFF3D3);
    draw_text_centered(154, "ZOMBIES", 7, 0xB9DA80);
    draw_text_centered(232, "GROW A DEFENCE FOR YOUR LAWN", 2, CLR_GRAY);
    menu_button(MENU_START_X, MENU_START_Y, MENU_START_W, MENU_START_H, "PLAY  ENTER", true);
    menu_button(MENU_EXIT_X, MENU_EXIT_Y, MENU_EXIT_W, MENU_EXIT_H, "QUIT  ESC", false);
    draw_text_centered(454, "F1  HOW TO PLAY", 2, 0xFFF3D3);
    draw_text_centered(490, "FIVE WAVES  SEVEN PLANTS  ONE LAWN", 1, CLR_GRAY);
    vg_draw_rectangle(0, 566, 800, 34, 0x152D21);
    draw_string(24, 578, "LAST PLAYED", 1, CLR_GRAY);
    draw_last_played(776, 574, CLR_GRAY);
}

static void render_playing(void) {
    /* HUD is drawn in world.c before the units. */
}

static void render_paused(void) {
    vg_draw_rectangle(180, 160, 440, 296, 0x152D21);
    draw_text_centered(194, "PAUSED", 5, 0xFFF3D3);
    menu_button(230, 266, 340, 48, "RESUME  ENTER", true);
    menu_button(230, 328, 340, 42, "MAIN MENU  Q", false);
    menu_button(230, 384, 340, 42, "QUIT  X", false);
}

static void render_game_over(void) {
    draw_map();
    vg_draw_rectangle(104, 160, 592, 278, 0x38231F);
    draw_text_centered(204, "LAWN LOST", 6, 0xF6BB94);
    draw_text_centered(292, "THE ZOMBIES BROKE THROUGH", 2, 0xFFF3D3);
    draw_text_centered(366, "ENTER  BACK TO MENU", 2, 0xFFF3D3);
}

static void render_victory(void) {
    draw_map();
    vg_draw_rectangle(104, 160, 592, 278, 0x152D21);
    draw_text_centered(204, "LAWN SAVED", 6, 0xB9DA80);
    draw_text_centered(292, "ALL FIVE WAVES DEFEATED", 2, 0xFFF3D3);
    draw_text_centered(366, "ENTER  BACK TO MENU", 2, 0xFFF3D3);
}

static void render_help(void) {
    draw_map();
    vg_draw_rectangle(86, 58, 628, 488, 0x152D21);
    draw_text_centered(88, "HOW TO PLAY", 4, 0xB9DA80);
    draw_string(118, 154, "CHOOSE SIX PLANTS FOR YOUR DECK", 2, 0xFFF3D3);
    draw_string(118, 184, "CLICK TO ADD OR DRAG TO REORDER", 2, CLR_GRAY);
    draw_string(118, 232, "CLICK SUNS TO GROW YOUR BUDGET", 2, 0xFFF3D3);
    draw_string(118, 262, "PLANT SUNFLOWERS TO MAKE MORE", 2, CLR_GRAY);
    draw_string(118, 310, "1 TO 6  SELECT A SEED", 2, 0xFFF3D3);
    draw_string(118, 340, "ARROWS  MOVE    ENTER  PLANT", 2, CLR_GRAY);
    draw_string(118, 388, "S  SHOVEL    ESC  CANCEL OR PAUSE", 2, 0xFFF3D3);
    draw_string(118, 418, "P  PAUSE    RIGHT CLICK  CANCEL", 2, CLR_GRAY);
    draw_text_centered(496, "F1 OR ESC  BACK", 2, 0xB9DA80);
}

void draw_wave_banner(WaveBannerType type, int ticks_remaining) {
    int fade_start;
    uint32_t bg_color, bar_color;
    uint32_t txt_color;

    if (type == WAVE_BANNER_NONE || ticks_remaining <= 0) return;

    fade_start = WAVE_BANNER_TICKS / 3;
    txt_color  = (ticks_remaining < fade_start) ? CLR_GRAY : CLR_WHITE;

    if (type == WAVE_BANNER_FINAL) {
        bg_color  = (ticks_remaining < fade_start) ? CLR_OVERLAY : CLR_DKRED;
        bar_color = CLR_RED;
    } else { /* WAVE_BANNER_HUGE */
        bg_color  = (ticks_remaining < fade_start) ? CLR_OVERLAY : 0x4A3500;
        bar_color = 0xCC8800;
    }

    /* Faixa central */
    vg_draw_rectangle(0, 240, 800, 120, bg_color);
    vg_draw_rectangle(0, 238, 800, 4, bar_color);
    vg_draw_rectangle(0, 358, 800, 4, bar_color);

    if (type == WAVE_BANNER_FINAL) {
        draw_text_centered(283, "FINAL WAVE", 5, txt_color);
    } else {
        draw_text_centered(265, "HUGE WAVE", 5, txt_color);
        draw_text_centered(320, "INCOMING", 4, bar_color);
    }
}

/* ── Auxiliar: desenha sprite de planta centrado numa célula ─────── */
static void draw_plant_sprite_in_cell(PlantType type, int cx, int cy, int cw, int ch) {
    AnimSprite *spr = NULL;
    switch (type) {
        case SUNFLOWER:   spr = get_sunflower_idle_sprite();   break;
        case PEASHOOTER:  spr = get_peashooter_idle_sprite();  break;
        case REPEATER:    spr = get_repeater_idle_sprite();    break;
        case CABBAGE:     spr = get_cabbage_card_sprite();     break;
        case WALL_NUT:    spr = get_wall_nut_idle_sprite();    break;
        case POTATO_MINE: spr = get_potato_mine_idle_sprite(); break;
        case SPIKEWEED:   spr = get_spikeweed_card_sprite();   break;
    }
    if (spr == NULL || spr->pixmaps == NULL || spr->pixmaps[0] == NULL) return;
    int dx = cx + (cw - spr->width)  / 2;
    int dy = cy + (ch - spr->height) / 2;
    if (type == CABBAGE) {
        dy -= 10;
    }
    vg_draw_pixmap(spr->pixmaps[0], spr->width, spr->height, dx, dy);
}

static const char *plant_name(PlantType type) {
    switch (type) {
        case SUNFLOWER: return "SUNFLOWER";
        case PEASHOOTER: return "PEASHOOTER";
        case REPEATER: return "REPEATER";
        case CABBAGE: return "CABBAGE";
        case WALL_NUT: return "WALL NUT";
        case POTATO_MINE: return "POTATO";
        case SPIKEWEED: return "SPIKEWEED";
    }
    return "";
}

static void selection_card(int x, int y, PlantType type, bool selected, bool focused, int slot) {
    uint32_t border = focused ? 0xFFF3D3 : selected ? 0x9AD671 : 0x68835B;
    vg_draw_rectangle(x, y, SEL_CARD_W, SEL_CARD_H, border);
    vg_draw_rectangle(x + 2, y + 2, SEL_CARD_W - 4, SEL_CARD_H - 4, selected ? 0x315C32 : 0x253C29);
    draw_plant_sprite_in_cell(type, x, y - 10, SEL_CARD_W, SEL_CARD_H);
    vg_draw_rectangle(x + 2, y + 64, SEL_CARD_W - 4, 24, selected ? 0x315C32 : 0x253C29);
    const char *name = plant_name(type);
    draw_string(x + (SEL_CARD_W - strlen(name) * 6) / 2, y + 66, name, 1, 0xFFF3D3);
    char cost[12];
    snprintf(cost, sizeof(cost), "%d SUN", get_plant_cost(type));
    draw_string(x + (SEL_CARD_W - strlen(cost) * 6) / 2, y + 78, cost, 1, CLR_GRAY);
    if (slot >= 0) {
        char number[2] = {'1' + slot, 0};
        vg_draw_rectangle(x + 4, y + 4, 15, 19, 0x152D21);
        draw_string(x + 6, y + 6, number, 2, 0xFFF3D3);
    }
}

void render_plant_selection(void) {
    const PlantType *available = get_available_plants();
    int deck_size = get_selected_deck_size();
    draw_map();
    vg_draw_rectangle(74, 16, 652, 550, 0x152D21);
    draw_text_centered(38, "CHOOSE YOUR PLANTS", 3, 0xFFF3D3);
    draw_text_centered(76, "CLICK TO ADD  DRAG TO CHOOSE A SLOT", 2, CLR_GRAY);
    for (int idx = 0; idx < TOTAL_AVAILABLE_PLANTS; idx++) {
        int x = SEL_GRID_X + (idx % SEL_COLS) * (SEL_CARD_W + SEL_CARD_GAP);
        int y = SEL_GRID_Y + (idx / SEL_COLS) * (SEL_CARD_H + SEL_CARD_GAP);
        PlantType type = available[idx];
        selection_card(x, y, type, is_plant_in_deck(type), idx == get_selection_cursor() && !is_cursor_on_button(), get_deck_slot_for_type(type));
    }
    char count[40];
    snprintf(count, sizeof(count), "YOUR DECK   %d OF 6 SELECTED", deck_size);
    draw_text_centered(336, count, 2, deck_size == 6 ? 0xB9DA80 : CLR_GRAY);
    for (int i = 0; i < MAX_DECK_SIZE; i++) {
        int x = SEL_DECK_X + i * (SEL_CARD_W + SEL_DECK_GAP);
        int plant = get_deck_plant_at_slot(i);
        if (plant >= 0) selection_card(x, SEL_DECK_Y, (PlantType)plant, true, false, i);
        else {
            vg_draw_rectangle(x, SEL_DECK_Y, SEL_CARD_W, SEL_CARD_H, 0x354A37);
            char num[2] = {'1' + i, 0};
            draw_string(x + 38, SEL_DECK_Y + 36, num, 2, CLR_GRAY);
        }
    }
    if (deck_size == MAX_DECK_SIZE) menu_button(SEL_BTN_X, SEL_BTN_Y, SEL_BTN_W, SEL_BTN_H, "PLAY  SPACE", true);
    else {
        vg_draw_rectangle(SEL_BTN_X, SEL_BTN_Y, SEL_BTN_W, SEL_BTN_H, 0x354A37);
        draw_text_centered(SEL_BTN_Y + 16, "CHOOSE SIX", 2, CLR_GRAY);
    }
    if (is_deck_full_flash_active()) draw_text_centered(542, "DECK FULL  REMOVE A PLANT FIRST", 1, 0xF6BB94);
    else draw_text_centered(542, "ARROWS  MOVE   ENTER  ADD OR REMOVE   ESC  BACK   F1  HELP", 1, CLR_GRAY);
    if (is_dragging_plant()) draw_plant_sprite_in_cell(get_dragged_plant_type(), get_mouse_x() - SEL_CARD_W / 2, get_mouse_y() - SEL_CARD_H / 2, SEL_CARD_W, SEL_CARD_H);
}

void render_scene(void) {
    GameState cur = get_game_state();

    switch (cur) {
        case STATE_HELP:            render_help();            break;
        case STATE_MAIN_MENU:       render_main_menu();       break;
        case STATE_PLANT_SELECTION: render_plant_selection(); break;
        case STATE_PLAYING:         render_playing();         break;
        case STATE_PAUSED:          render_paused();          break;
        case STATE_GAME_OVER:       render_game_over();       break;
        case STATE_VICTORY:         render_victory();         break;
        default: break;
    }
}
