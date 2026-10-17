#include "menu.h"

#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <stdio.h>
#include <string.h>

#include "../model/sprite.h"
#include "../controller/video/graphics.h"
#include "../controller/keyboard-mouse/kbc.h"
#include "../controller/keyboard-mouse/mouse.h"

#include "../../assets/backgrounds/bg_court.xpm"
#include "../../assets/buttons/btn_normal.xpm"
#include "../../assets/buttons/btn_selected.xpm"
#include "../../assets/backgrounds/bg_settings.xpm"
#include "../../assets/other/logo_pompower.xpm"
#include "../../assets/other/cursor.xpm"

#include "../../assets/notes/arrow_up.xpm"
#include "../../assets/notes/arrow_down.xpm"
#include "../../assets/notes/arrow_left.xpm"
#include "../../assets/notes/arrow_right.xpm"
#include "../../assets/notes/heart.xpm"

#include "renderer.h"
#include "font.h"

extern uint8_t scancode;

typedef enum {
    INPUT_ARROWS,
    INPUT_DFJK,
    INPUT_WASD
} input_mode_t;

typedef enum {
    DIFF_BASIC,
    DIFF_CHALLENGE,
    DIFF_HARDCORE
} diff_mode_t;

static void draw_sprite_at(sprite *sp, int x, int y) {
    if (!sp) return;
    int oldx = sp->x;
    int oldy = sp->y;
    sp->x = x; sp->y = y;
    draw_sprite(sp);
    sp->x = oldx; sp->y = oldy;
}

static void draw_instructions_page(sprite *bg_settings, sprite *btn, sprite *cursor, sprite *arr_up, sprite *arr_down, sprite *arr_left, sprite *arr_right, sprite *heart_sp, int mx, int my, int h_res, int v_res) {
    int w = 320;
    int h = v_res - 80;
    int box_x = h_res / 2 - w / 2;
    int box_y = 40;

    vg_draw_rectangle(box_x, box_y, w, h, 0x9333EA);
    vg_draw_rectangle(box_x + 2, box_y + 2, w - 4, h - 4, 0x0F1A3E);

    int cx = h_res / 2;
    int glyph_h = 8;
    int line = glyph_h + 8;

    draw_text_centered("HOW TO PLAY", cx, 80);

    int y = 80 + 60;
    draw_text_centered("ARROWS RISE FROM THE BOTTOM OF THE SCREEN", cx, y); y += line;
    draw_text_centered("HIT THEM AS THEY REACH THE TARGET ZONE", cx, y); y += line + 8;

    int arrow_y = y;
    int arrow_row_w = 4 * 20;
    int ax = cx - arrow_row_w / 2;
    if (arr_up) draw_sprite_at(arr_up, ax, arrow_y);
    if (arr_down) draw_sprite_at(arr_down, ax + 20, arrow_y);
    if (arr_left) draw_sprite_at(arr_left, ax + 40, arrow_y);
    if (arr_right) draw_sprite_at(arr_right, ax + 60, arrow_y);
    y += 40;

    draw_text_centered("SCORING", cx, y); y += line + 8;
    draw_text_centered("HIT A NOTE - GET 100 PTS", cx, y); y += line;
    draw_text_centered("MISS A NOTE - NO PENALTY", cx, y); y += line + 20;

    draw_text_centered("CHALLENGE OR HARDCORE MODE", cx, y); y += line + 8;
    draw_text_centered("ITEMS' SPEED INCREASES EVERY 2 SECONDS", cx, y); y += line + 20;

    draw_text_centered("HARDCORE MODE", cx, y); y += line + 8;
    if (heart_sp) {
        int w1 = text_width("CLICK THE ");
        int w_heart = heart_sp->width;
        int w2 = text_width(" WITH YOUR MOUSE");
        int total_w = w1 + w_heart + w2;
        int start_x = cx - total_w / 2;

        draw_text("CLICK THE ", start_x, y);
        draw_sprite_at(heart_sp, start_x + w1, y - 4);
        draw_text(" WITH YOUR MOUSE", start_x + w1 + w_heart, y);
        y += line + 8;
    } 
    else {
        draw_text_centered("CLICK HEART WITH YOUR MOUSE", cx, y);
        y += line + 8;
    }
    draw_text_centered("HEART HIT - GET 150 PTS", cx, y); y += line;
    draw_text_centered("HEART MISS - LOSE 50 PTS", cx, y); y += line + 25;

    draw_text_centered("HIT ESC TO EXIT THE GAME", cx, y);

    int btn_w = btn ? btn->width : 80;
    int btn_h = btn ? btn->height : 18;
    int bx_back = cx - btn_w / 2;
    int by_back = v_res - 70;
    if (btn) draw_sprite_at(btn, bx_back, by_back);
    draw_text_centered("BACK", cx, by_back + btn_h/2 - glyph_h/2);

    if (cursor) draw_sprite_at(cursor, mx, my);
}

int menu_run(uint8_t kb_irq_set, uint8_t mouse_irq_set, uint8_t timer_irq_set) {
    sprite *bg = create_sprite((xpm_map_t)bg_court_xpm, 0, 0);
    sprite *btn = create_sprite((xpm_map_t)btn_normal_xpm, 0, 0);
    sprite *btn_sel = create_sprite((xpm_map_t)btn_selected_xpm, 0, 0);

    sprite *bg_settings = create_sprite((xpm_map_t)bg_settings_xpm, 0, 0);
    sprite *logo = create_sprite((xpm_map_t)logo_pompower_xpm, 0, 0);
    sprite *cursor = create_sprite((xpm_map_t)cursor_xpm, 0, 0);

    sprite *arr_up = create_sprite((xpm_map_t)arrow_up_xpm, 0, 0);
    sprite *arr_down = create_sprite((xpm_map_t)arrow_down_xpm, 0, 0);
    sprite *arr_left = create_sprite((xpm_map_t)arrow_left_xpm, 0, 0);
    sprite *arr_right = create_sprite((xpm_map_t)arrow_right_xpm, 0, 0);
    sprite *heart_sp = create_sprite((xpm_map_t)heart_xpm, 0, 0);

    uint8_t* menu_bg_buf = malloc(get_vram_size());
    uint8_t* settings_bg_buf = malloc(get_vram_size());

    input_mode_t input_mode = INPUT_ARROWS;
    diff_mode_t diff_mode = DIFF_BASIC;

    FILE *f = fopen("settings.cfg", "r");
    if (f) {
        char key[64], val[64];
        while (fscanf(f, "%63s = %63s\n", key, val) == 2) {
            if (strcmp(key, "input") == 0) {
                if (strcmp(val, "ARROWS") == 0) input_mode = INPUT_ARROWS;
                else if (strcmp(val, "DFJK") == 0) input_mode = INPUT_DFJK;
                else input_mode = INPUT_WASD;
            } else if (strcmp(key, "mode") == 0) {
                if (strcmp(val, "BASIC") == 0) diff_mode = DIFF_BASIC;
                else if (strcmp(val, "CHALLENGE") == 0) diff_mode = DIFF_CHALLENGE;
                else diff_mode = DIFF_HARDCORE;
            }
        }

        fclose(f);
    }

    clear_buffer();
    if (bg) draw_sprite_at(bg, 0, 0);

    if (logo) draw_sprite_at(logo, get_h_res()/2 - logo->width/2, 40);

    int center_x = get_h_res() / 2;
    int btn_w = btn ? btn->width : 100;
    int btn_h = btn ? btn->height : 30;
    int spacing = 20;
    int bx = center_x - btn_w/2;
    int by_start = get_v_res() / 2 - btn_h - spacing/2;
    int by_howto = by_start + btn_h + spacing;
    int by_settings = by_start + btn_h + spacing;
    
    if (btn_sel) draw_sprite_at(btn_sel, bx, by_start);
    else if (btn) draw_sprite_at(btn, bx, by_start);
    draw_text_centered("START", center_x, by_start + btn_h/2 - 4);
    
    if (btn) draw_sprite_at(btn, bx, by_howto);
    draw_text_centered("HOW TO PLAY", center_x, by_howto + btn_h/2 - 4);
    
    if (btn) draw_sprite_at(btn, bx, by_settings);
    draw_text_centered("SETTINGS", center_x, by_settings + btn_h/2 - 4);

    memcpy(menu_bg_buf, get_back_buffer(), get_vram_size());
    set_static_background(menu_bg_buf);

    uint8_t* howtoplay_background = NULL;

    int ipc_status, r;
    message msg;
    int running = 1;
    int in_settings = 0;
    int in_howtoplay = 0;
    int quit = 0;
    int last_buttons = 0;

    int h_res = get_h_res();
    int v_res = get_v_res();

    auto_render_settings: {
        clear_buffer();
        if (bg_settings) draw_sprite_at(bg_settings, get_h_res()/2 - bg_settings->width/2, get_v_res()/2 - bg_settings->height/2);

        int btn_w = btn ? btn->width : 100;
        int btn_h = btn ? btn->height : 30;
        int gap = 24;
        int total_w = 3*btn_w + 2*gap;
        int start_x = get_h_res()/2 - total_w/2;
        int top_area_y = get_v_res()/2 - 80;
        int ibx0 = start_x;
        int ibx1 = start_x + btn_w + gap;
        int ibx2 = start_x + 2*(btn_w + gap);

        draw_text_centered("SET THE INPUT MODE:", get_h_res() / 2, top_area_y);

        top_area_y += 30;

        if (input_mode == INPUT_ARROWS) { if (btn_sel) draw_sprite_at(btn_sel, ibx0, top_area_y); else if (btn) draw_sprite_at(btn, ibx0, top_area_y); }
        else if (btn) draw_sprite_at(btn, ibx0, top_area_y);
        if (input_mode == INPUT_DFJK) { if (btn_sel) draw_sprite_at(btn_sel, ibx1, top_area_y); else if (btn) draw_sprite_at(btn, ibx1, top_area_y); }
        else if (btn) draw_sprite_at(btn, ibx1, top_area_y);
        if (input_mode == INPUT_WASD) { if (btn_sel) draw_sprite_at(btn_sel, ibx2, top_area_y); else if (btn) draw_sprite_at(btn, ibx2, top_area_y); }
        else if (btn) draw_sprite_at(btn, ibx2, top_area_y);

        int lab_y = top_area_y + btn_h / 2 - 4;

        draw_text_centered("ARROWS", ibx0 + btn_w / 2, lab_y);
        draw_text_centered("DFJK", ibx1 + btn_w / 2, lab_y);
        draw_text_centered("WASD", ibx2 + btn_w / 2, lab_y);

        int diff_y = top_area_y + btn_h + 35;

        draw_text_centered("SET THE DIFFICULTY LEVEL:", get_h_res() / 2, diff_y);

        diff_y += 30;

        int dbx0 = start_x;
        int dbx1 = ibx1;
        int dbx2 = ibx2;
        if (diff_mode == DIFF_BASIC) { if (btn_sel) draw_sprite_at(btn_sel, dbx0, diff_y); else if (btn) draw_sprite_at(btn, dbx0, diff_y); }
        else if (btn) draw_sprite_at(btn, dbx0, diff_y);
        if (diff_mode == DIFF_CHALLENGE) { if (btn_sel) draw_sprite_at(btn_sel, dbx1, diff_y); else if (btn) draw_sprite_at(btn, dbx1, diff_y); }
        else if (btn) draw_sprite_at(btn, dbx1, diff_y);
        if (diff_mode == DIFF_HARDCORE) { if (btn_sel) draw_sprite_at(btn_sel, dbx2, diff_y); else if (btn) draw_sprite_at(btn, dbx2, diff_y); }
        else if (btn) draw_sprite_at(btn, dbx2, diff_y);
        
        int dlab_y = diff_y + btn_h/2 - 4;

        draw_text_centered("BASIC", dbx0 + btn_w / 2, dlab_y);
        draw_text_centered("CHALLENGE", dbx1 + btn_w / 2, dlab_y);
        draw_text_centered("HARDCORE", dbx2 + btn_w / 2, dlab_y);

        int bx_back = get_h_res()/2 - btn_w/2;
        int by_back = get_v_res() - btn_h - 40;
        if (btn) draw_sprite_at(btn, bx_back, by_back);

        int ty = by_back + btn_h/2 - 4;
        draw_text_centered("BACK", bx_back + btn_w/2, ty);

        memcpy(settings_bg_buf, get_back_buffer(), get_vram_size());
    }

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & kb_irq_set) {
                        kbc_ih();
                        if (scancode == 0x01) { running = 0; quit = 1; }
                    }

                    if (msg.m_notify.interrupts & mouse_irq_set) {
                        process_mouse_interrupt();
                        int mx, my;
                        mouse_get_pos(&mx, &my);
                        int buttons = mouse_get_buttons();
                        int pressed = (buttons & 1) && !(last_buttons & 1);

                        int btn_w = btn ? btn->width  : 80;
                        int btn_h = btn ? btn->height : 18;
                        int cx = h_res / 2;
                        int bx = cx - btn_w / 2;

                        if (!in_settings && !in_howtoplay) {
                            int spacing = 20;
                            int by_start = v_res/2 - btn_h - spacing/2;
                            int by_howto = by_start + btn_h + spacing;
                            int by_settings = by_howto + btn_h + spacing;

                            if (pressed && mx >= bx && mx <= bx+btn_w && my >= by_start && my <= by_start+btn_h) running = 0;
                            if (pressed && mx >= bx && mx <= bx+btn_w && my >= by_howto && my <= by_howto+btn_h) {
                                in_howtoplay = 1;
                                howtoplay_background = malloc(get_vram_size());
                                if (howtoplay_background != NULL) {
                                    memcpy(get_back_buffer(), menu_bg_buf, get_vram_size());
                                    draw_instructions_page(bg_settings, btn, NULL, arr_up, arr_down, arr_left, arr_right, heart_sp, 0, 0, h_res, v_res);
                                    memcpy(howtoplay_background, get_back_buffer(), get_vram_size());
                                    set_static_background(howtoplay_background);
                                }
                            }
                            if (pressed && mx >= bx && mx <= bx+btn_w && my >= by_settings && my <= by_settings+btn_h) {
                                in_settings = 1;
                                set_static_background(settings_bg_buf);
                            }
                        }
                        else if (in_howtoplay) {
                            int by_back = v_res - 70;
                            if (pressed && mx >= bx && mx <= bx+btn_w && my >= by_back && my <= by_back+btn_h) {
                                in_howtoplay = 0;
                                set_static_background(menu_bg_buf);
                                if (howtoplay_background != NULL) {
                                    free(howtoplay_background);
                                    howtoplay_background = NULL;
                                }
                            }
                        }
                        else {
                            int top_area_y = get_v_res()/2 - 80 + 30;
                            int b_w = btn ? btn->width : 100;
                            int b_h = btn ? btn->height : 30;
                            int gap = 24;
                            int total_w = 3*b_w + 2*gap;
                            int start_x = get_h_res()/2 - total_w/2;
                            int ibx0 = start_x;
                            int ibx1 = start_x + b_w + gap;
                            int ibx2 = start_x + 2*(b_w + gap);

                            int bx_back = get_h_res()/2 - b_w/2;
                            int by_back = get_v_res() - b_h - 40;
                            if (pressed && mx >= bx_back && mx <= bx_back+b_w && my >= by_back && my <= by_back+b_h) {
                                in_settings = 0;
                                set_static_background(menu_bg_buf);
                            }

                            if (pressed) {
                                if (mx >= ibx0 && mx <= ibx0+b_w && my >= top_area_y && my <= top_area_y+b_h) input_mode = INPUT_ARROWS;
                                else if (mx >= ibx1 && mx <= ibx1+b_w && my >= top_area_y && my <= top_area_y+b_h) input_mode = INPUT_DFJK;
                                else if (mx >= ibx2 && mx <= ibx2+b_w && my >= top_area_y && my <= top_area_y+b_h) input_mode = INPUT_WASD;
                            }

                            int diff_y = top_area_y + b_h + 65;
                            if (pressed) {
                                if (mx >= ibx0 && mx <= ibx0+b_w && my >= diff_y && my <= diff_y+b_h) diff_mode = DIFF_BASIC;
                                else if (mx >= ibx1 && mx <= ibx1+b_w && my >= diff_y && my <= diff_y+b_h) diff_mode = DIFF_CHALLENGE;
                                else if (mx >= ibx2 && mx <= ibx2+b_w && my >= diff_y && my <= diff_y+b_h) diff_mode = DIFF_HARDCORE;
                            }

                            if (pressed) {
                                FILE *fw = fopen("settings.cfg", "w");
                                if (fw) {
                                    const char *in_s = (input_mode==INPUT_ARROWS)?"ARROWS":(input_mode==INPUT_DFJK)?"DFJK":"WASD";
                                    const char *dm_s = (diff_mode==DIFF_BASIC)?"BASIC":(diff_mode==DIFF_CHALLENGE)?"CHALLENGE":"HARDCORE";
                                    fprintf(fw, "input = %s\n", in_s);
                                    fprintf(fw, "mode = %s\n", dm_s);
                                    fclose(fw);
                                }
                                goto auto_render_settings;
                            }
                        }
                        last_buttons = buttons;
                    }
                        
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        int mx = 0, my = 0;
                        mouse_get_pos(&mx, &my);

                        int btn_w = btn ? btn->width  : 80;
                        int btn_h = btn ? btn->height : 18;
                        int glyph_h = 8;
                        int cx = h_res / 2;
                        int bx = cx - btn_w / 2;

                        if (in_howtoplay) {
                            provide_static_background();
                            if (cursor) draw_sprite_at(cursor, mx, my);
                            swap_buffer();

                        } else if (in_settings) {
                            provide_static_background();
                            
                            if (cursor) draw_sprite_at(cursor, mx, my);
                            swap_buffer();

                        } else {

                            provide_static_background();

                            if (logo) draw_sprite_at(logo, cx - logo->width/2, 40);

                            int spacing     = 20;
                            int by_start    = v_res/2 - btn_h - spacing/2;
                            int by_howto    = by_start + btn_h + spacing;
                            int by_settings = by_howto  + btn_h + spacing;

                            draw_sprite_at(btn_sel ? btn_sel : btn, bx, by_start);
                            draw_text_centered("START", cx, by_start + btn_h/2 - glyph_h/2);

                            draw_sprite_at(btn, bx, by_howto);
                            draw_text_centered("HOW TO PLAY", cx, by_howto + btn_h/2 - glyph_h/2);
                            
                            draw_sprite_at(btn, bx, by_settings);
                            draw_text_centered("SETTINGS", cx, by_settings + btn_h/2 - glyph_h/2);

                            if (cursor) draw_sprite_at(cursor, mx, my);
                            swap_buffer();
                        }
                    }

                    break;
                default:
                    break;
            }
        }
    }

    if (in_howtoplay) {
        set_static_background(menu_bg_buf);
        if (howtoplay_background != NULL) {
            free(howtoplay_background);
            howtoplay_background = NULL;
        }
    }

    destroy_sprite(&bg);
    destroy_sprite(&btn);
    destroy_sprite(&btn_sel);
    destroy_sprite(&bg_settings);
    destroy_sprite(&logo);
    destroy_sprite(&cursor);
    destroy_sprite(&heart_sp);
    destroy_sprite(&arr_up);
    destroy_sprite(&arr_down);
    destroy_sprite(&arr_left);
    destroy_sprite(&arr_right);

    if (menu_bg_buf) free(menu_bg_buf);
    if (settings_bg_buf) free(settings_bg_buf);
    set_static_background(NULL);

    return quit;
}
