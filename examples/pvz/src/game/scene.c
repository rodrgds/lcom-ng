#include "pvz_platform.h"
#include <stdio.h>

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

static void render_main_menu(void) {
    vg_draw_rectangle(0, 0, 800, 600, CLR_BG);
    draw_text_centered(120, "PLANTS VS ZOMBIES", 5, CLR_GREEN);
    draw_text_right(760, 20, "LAST PLAYED", 2, CLR_GRAY);
    draw_last_played(760, 48, CLR_GRAY);

    vg_draw_rectangle(MENU_START_X, MENU_START_Y, MENU_START_W, MENU_START_H, CLR_DKGREEN);
    draw_text_centered(MENU_START_Y + 12, "START GAME", 3, CLR_WHITE);

    vg_draw_rectangle(MENU_EXIT_X, MENU_EXIT_Y, MENU_EXIT_W, MENU_EXIT_H, CLR_DKRED);
    draw_text_centered(MENU_EXIT_Y + 12, "EXIT", 3, CLR_WHITE);

    draw_text_centered(430, "ENTER  START", 2, CLR_GRAY);
    draw_text_centered(465, "ESC  EXIT", 2, CLR_GRAY);
}

static void render_playing(void) {
    /* HUD is drawn in loop.c before zombies so it stays in the background. */
}

static void render_paused(void) {
    vg_draw_rectangle(150, 150, 500, 300, CLR_OVERLAY);
    draw_text_centered(180, "PAUSED", 5, CLR_WHITE);
    draw_text_centered(280, "ENTER  RESUME", 3, CLR_GREEN);
    draw_text_centered(330, "Q  MENU", 3, CLR_GRAY);
    draw_text_centered(380, "ESC  QUIT", 3, CLR_RED);
}

static void render_game_over(void) {
    vg_draw_rectangle(0, 0, 800, 600, CLR_DKRED);
    draw_text_centered(200, "GAME OVER", 6, CLR_RED);
    draw_text_centered(350, "PRESS ENTER OR ESC", 3, CLR_WHITE);
}

static void render_victory(void) {
    vg_draw_rectangle(0, 0, 800, 600, CLR_DKGREEN);
    draw_text_centered(200, "YOU WIN", 6, CLR_GREEN);
    draw_text_centered(350, "PRESS ENTER OR ESC", 3, CLR_WHITE);
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

void render_plant_selection(void) {
    const PlantType *available = get_available_plants();
    int deck_size              = get_selected_deck_size();

    /* Fundo: mapa do jogo com painéis escuros só na área da UI */
    draw_map();

    /* Painel título */
    vg_draw_rectangle(0, 0, 800, 80, 0x0D1F0D);

    /* Painel inventário */
    int inv_pad = 12;
    int inv_x = SEL_GRID_X - inv_pad;
    int inv_y = SEL_GRID_Y - inv_pad;
    int inv_w = SEL_COLS * SEL_CARD_W + (SEL_COLS - 1) * SEL_CARD_GAP + inv_pad * 2;
    int inv_h = SEL_ROWS * SEL_CARD_H + (SEL_ROWS - 1) * SEL_CARD_GAP + inv_pad * 2;
    vg_draw_rectangle(inv_x, inv_y, inv_w, inv_h, 0x0D1F0D);

    /* Painel deck */
    int deck_pad = 12;
    int deck_panel_x = SEL_DECK_X - deck_pad;
    int deck_panel_y = SEL_DECK_Y - 50;
    int deck_panel_w = MAX_DECK_SIZE * SEL_CARD_W + (MAX_DECK_SIZE - 1) * SEL_DECK_GAP + deck_pad * 2;
    int deck_panel_h = SEL_CARD_H + 50 + deck_pad + 8;
    vg_draw_rectangle(deck_panel_x, deck_panel_y, deck_panel_w, deck_panel_h, 0x0D1F0D);

    /* Painel botão */
    vg_draw_rectangle(SEL_BTN_X - 4, SEL_BTN_Y - 4, SEL_BTN_W + 8, SEL_BTN_H + 8, 0x0D1F0D);

    /* Título */
    draw_text_centered(20, "CHOOSE YOUR PLANTS", 4, CLR_GREEN);
    draw_text_centered(58, "CLICK OR DRAG TO DECK SLOTS", 2, CLR_GRAY);

    /* Inventário — grelha 3x2 */
    for (int row = 0; row < SEL_ROWS; row++) {
        for (int col = 0; col < SEL_COLS; col++) {
            int idx = row * SEL_COLS + col;
            int cx = SEL_GRID_X + col * (SEL_CARD_W + SEL_CARD_GAP);
            int cy = SEL_GRID_Y + row * (SEL_CARD_H + SEL_CARD_GAP);
            bool has_plant = idx < TOTAL_AVAILABLE_PLANTS;
            PlantType type = has_plant ? available[idx] : PEASHOOTER;
            bool selected = has_plant && is_plant_in_deck(type);

            bool is_cursor  = (idx == get_selection_cursor());
            bool flash      = is_cursor && is_deck_full_flash_active();
            uint32_t bg     = selected ? 0x2E6B2E : 0x3A2A10;
            uint32_t border = flash    ? CLR_RED  :
                              is_cursor ? CLR_WHITE :
                              selected  ? CLR_GREEN : 0x7A5A20;

            vg_draw_rectangle(cx, cy, SEL_CARD_W, SEL_CARD_H, bg);
            vg_draw_rectangle(cx,                   cy,                   SEL_CARD_W, 2, border);
            vg_draw_rectangle(cx,                   cy + SEL_CARD_H - 2, SEL_CARD_W, 2, border);
            vg_draw_rectangle(cx,                   cy,                   2, SEL_CARD_H, border);
            vg_draw_rectangle(cx + SEL_CARD_W - 2, cy,                   2, SEL_CARD_H, border);

            if (has_plant) {
                draw_plant_sprite_in_cell(type, cx, cy, SEL_CARD_W, SEL_CARD_H);
            }

            if (selected) {
                char num[2] = {'0' + get_deck_slot_for_type(type) + 1, '\0'};
                draw_string(cx + 4, cy + 4, num, 2, CLR_WHITE);
            }
        }
    }

    /* Deck — 6 slots fixos, livre ordering */
    draw_text_centered(318, "YOUR DECK", 2, CLR_WHITE);

    /* Contador "X/6 SELECTED" */
    {
        char buf[16];
        uint32_t count_color = (deck_size == 6) ? CLR_GREEN : CLR_GRAY;
        /* "X OF 6 SELECTED" — só A-Z e 0-9 são suportados pelo font */
        buf[0] = '0' + deck_size;
        buf[1] = ' '; buf[2] = 'O'; buf[3] = 'F'; buf[4] = ' ';
        buf[5] = '6'; buf[6] = ' ';
        buf[7] = 'S'; buf[8] = 'E'; buf[9] = 'L'; buf[10] = 'E';
        buf[11] = 'C'; buf[12] = 'T'; buf[13] = 'E'; buf[14] = 'D';
        buf[15] = '\0';
        draw_text_centered(336, buf, 2, count_color);
    }
    for (int i = 0; i < MAX_DECK_SIZE; i++) {
        int sx    = SEL_DECK_X + i * (SEL_CARD_W + SEL_DECK_GAP);
        int sy    = SEL_DECK_Y;
        int plant = get_deck_plant_at_slot(i);
        bool occupied = (plant >= 0);

        uint32_t bg     = occupied ? 0x2E6B2E : 0x1A1A1A;
        uint32_t border = occupied ? CLR_GREEN : CLR_GRAY;

        vg_draw_rectangle(sx, sy, SEL_CARD_W, SEL_CARD_H, bg);
        vg_draw_rectangle(sx,                  sy,                   SEL_CARD_W, 2, border);
        vg_draw_rectangle(sx,                  sy + SEL_CARD_H - 2, SEL_CARD_W, 2, border);
        vg_draw_rectangle(sx,                  sy,                   2, SEL_CARD_H, border);
        vg_draw_rectangle(sx + SEL_CARD_W - 2, sy,                   2, SEL_CARD_H, border);

        /* Número do slot */
        char num[2] = {'1' + i, '\0'};
        draw_string(sx + 4, sy + 4, num, 2, occupied ? CLR_WHITE : CLR_GRAY);

        if (occupied) {
            draw_plant_sprite_in_cell((PlantType)plant, sx, sy, SEL_CARD_W, SEL_CARD_H);
        }
    }

    /* Botão Start */
    bool can_start      = (deck_size == 6);
    bool btn_focused    = is_cursor_on_button() && can_start;
    uint32_t btn_col    = can_start ? CLR_DKGREEN : 0x333333;
    uint32_t border_col = btn_focused ? CLR_WHITE : (can_start ? CLR_GREEN : 0x555555);
    uint32_t txt_col    = can_start ? CLR_WHITE   : CLR_GRAY;
    int bx = SEL_BTN_X, by = SEL_BTN_Y, bw = SEL_BTN_W, bh = SEL_BTN_H;
    /* Borda verde escuro */
    vg_draw_rectangle(bx, by, bw, bh, border_col);
    /* Interior recuado 3px */
    vg_draw_rectangle(bx + 3, by + 3, bw - 6, bh - 6, btn_col);
    draw_text_centered(by + 14, "START GAME", 3, txt_col);

    draw_text_centered(530, "ESC  BACK TO MENU", 2, CLR_WHITE);

    /* Preview drag — planta a seguir o cursor */
    if (is_dragging_plant()) {
        PlantType dt = get_dragged_plant_type();
        int mx = get_mouse_x();
        int my = get_mouse_y();
        draw_plant_sprite_in_cell(dt, mx - SEL_CARD_W / 2, my - SEL_CARD_H / 2,
                                  SEL_CARD_W, SEL_CARD_H);
    }
}

void render_scene(void) {
    GameState cur = get_game_state();

    switch (cur) {
        case STATE_MAIN_MENU:       render_main_menu();       break;
        case STATE_PLANT_SELECTION: render_plant_selection(); break;
        case STATE_PLAYING:         render_playing();         break;
        case STATE_PAUSED:          render_paused();          break;
        case STATE_GAME_OVER:       render_game_over();       break;
        case STATE_VICTORY:         render_victory();         break;
        default: break;
    }
}
