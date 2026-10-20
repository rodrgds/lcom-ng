#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <stdio.h>
#include <string.h>

#include "scoreboard.h"
#include "game.h"

#include "../config.h"
#include "../controller/keyboard-mouse/kbc.h"
#include "../controller/keyboard-mouse/mouse.h"
#include "../controller/rtc/rtc.h"
#include "../controller/video/graphics.h"
#include "../model/sprite.h"
#include "../view/renderer.h"
#include "../view/font.h"

#include "../../assets/backgrounds/bg_scoreboard.xpm"
#include "../../assets/other/logo_pompower.xpm"

extern uint8_t scancode;

void (get_scoreboard)(uint8_t kb_irq_set, uint8_t mouse_irq_set, uint8_t timer_irq_set, int final_score) {
    sprite *score_bg = create_sprite((xpm_map_t) bg_scoreboard_xpm, 0, 0);
    sprite *logo_sp = create_sprite((xpm_map_t) logo_pompower_xpm, 0, 0);

    int ipc_status, r;
    message msg;
    int last_buttons = 0;
    bool showing = true;

    {
        rtc_date rd;
        if (rtc_read_date(&rd) == 0) {
            FILE *fw = fopen("scores.txt", "a");
            if (fw) {
                const char *mode_str;
                switch (current_mode) {
                    case MODE_BASIC:
                        mode_str = "BASIC";
                        break;
                    case MODE_CHALLENGE:
                        mode_str = "CHALLENGE";
                        break;
                    default:
                        mode_str = "HARDCORE";
                        break;
                }
                fprintf(fw, "%d %s %02u %02u %02u\n", final_score, mode_str, rd.day, rd.month, rd.year);
                fclose(fw);
            }
        }
    }

    struct hs { 
        int score; 
        char mode[16];
        rtc_date d; 
    } entries[128];

    int n = 0;
    {
        FILE *fr = fopen("scores.txt", "r");
        if (fr) {
            while (n < 128) {
                int s; 
                char mode[16];
                unsigned int dd, mm, yy;
                if (fscanf(fr, "%d %15s %u %u %u", &s, mode, &dd, &mm, &yy) != 5) break;
                entries[n].score = s;
                strcpy(entries[n].mode, mode);
                entries[n].d.day = dd;
                entries[n].d.month = mm;
                entries[n].d.year = yy;
                n++;
            }
            fclose(fr);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (entries[j].score > entries[i].score) { 
                struct hs t = entries[i];
                entries[i] = entries[j];
                entries[j] = t;
            }
        }
    }
    
    int topn = n < 8 ? n : 8;

    while (showing) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & kb_irq_set) {
                        kbc_ih();
                        if (scancode == 0x01) showing = false;
                    }
                    if (msg.m_notify.interrupts & mouse_irq_set) {
                        process_mouse_interrupt();
                        int mx, my;
                        mouse_get_pos(&mx, &my);
                        int buttons = mouse_get_buttons();
                        int pressed = (buttons & 1) && !(last_buttons & 1);
                        if (pressed) showing = false;
                        last_buttons = buttons;
                    }
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        clear_buffer();

                        if (score_bg) {
                            for (int yy = 0; yy < get_v_res(); yy += score_bg->height)
                                for (int xx = 0; xx < get_h_res(); xx += score_bg->width) {
                                    score_bg->x = xx;
                                    score_bg->y = yy;
                                    draw_sprite(score_bg);
                                }
                        }

                        if (logo_sp) { 
                            logo_sp->x = 40; 
                            logo_sp->y = 8; 
                            draw_sprite(logo_sp); 
                        }

                        int left_x = 160;
                        int start_y = 140;
                        int line_h = 56;

                        int title_x = 80;
                        int title_y = 105;
                        
                        draw_text("SCORES", title_x, title_y);

                        for (int i = 0; i < topn; i++) {
                            int y = start_y + i*line_h;
                            int day = entries[i].d.day;
                            int month = entries[i].d.month;
                            int year = entries[i].d.year;
                            
                            char date_buffer[16];
                            snprintf(date_buffer, sizeof(date_buffer), "%02d/%02d/%02d", day, month, year);
                            draw_text_centered(date_buffer, left_x + 100, y);
                            
                            draw_text_centered(entries[i].mode, left_x + 250, y);

                            char score_buffer[16];
                            snprintf(score_buffer, sizeof(score_buffer), "%d", entries[i].score);
                            draw_text_centered(score_buffer, left_x + 400, y);
                        }

                        int cmx = 0, cmy = 0; 
                        mouse_get_pos(&cmx, &cmy);

                        if (cursor_sprite) {
                            cursor_sprite->x = cmx; 
                            cursor_sprite->y = cmy; 
                            draw_sprite(cursor_sprite); 
                        }
                        swap_buffer();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    destroy_sprite(&score_bg);
    destroy_sprite(&logo_sp);
}
