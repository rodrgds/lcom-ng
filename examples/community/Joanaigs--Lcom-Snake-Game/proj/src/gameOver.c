#include "gameOver.h"
#include "cursor.h"
#include "graphics.h"
#include "header.h"
#include "i8042.h"
#include "images/continue_colored.xpm"
#include "images/gameOver.xpm"
#include "images/mouse_cursor.xpm"
#include "keyboard.h"
#include "mouse.h"
#include "objects.h"
#include "apple.h"
#include "proj.h"
#include "snake.h"
#include "timer.h"

xpm_image_t img;
uint8_t *map;
xpm_image_t imgColored;
uint8_t *mapColored;
bool on_continue = false;

int menuContinueCollisions(cursor *mouse_c) {
    if (mouse_c->y >= 386 && mouse_c->y <= 432 && mouse_c->x >= 331 && mouse_c->x <= 445)
        return 1;
    return 0;
}

int continueMenu(cursor *mouse_c, struct packet *p) {
    struct mouse_ev event = mouse_get_event(p);

    if (menuContinueCollisions(mouse_c)) {

        if (event.type == LB_RELEASED) {
            baseState = mainMenu;
            return 1;
        }
        if (!on_continue) {
            on_continue = true;
            draw_xpm(imgColored, mapColored, (h_res - imgColored.width) / 2, (v_res + 120 - imgColored.height) / 2);
            drawGameOverElements();
            draw_xpm_video_mem(mouse_c->img, mouse_c->img.bytes, mouse_c->x, mouse_c->y);
        }
    } else {
        if (on_continue) {

            draw_xpm(img, map, (h_res - img.width) / 2, (v_res + 120 - img.height) / 2);
            drawGameOverElements();
            draw_xpm_video_mem(mouse_c->img, mouse_c->img.bytes, mouse_c->x, mouse_c->y);
        }
    }



    return 0;
}

int gameOverPage() {
    map = xpm_load((xpm_map_t) game_over_xpm, XPM_8_8_8, &img);
    mapColored = xpm_load((xpm_map_t) continue_colored_xpm, XPM_8_8_8, &(imgColored));
    int x = (h_res - img.width) / 2;
    int y = (v_res + 120 - img.height) / 2;
    draw_xpm(img, map, x, y);
    draw_xpm(timeClock.img, timeClock.map, x + 91 + 20, y + 90);
    draw_number(gameTime, x + 91 + 20 + 45, y + 90 + 10);
    if (mouse_enable_data_report(true))
        return 1;
    cursor c;
    c.x = 200;
    c.y = 200;
    xpm_load((xpm_map_t) mouse_cursor, XPM_8_8_8, &(c.img));

    Apple apple;
    initApple(&apple, x + 91 + 20, y + 140, red);
    drawApple(apple);
    draw_number(numOfApplesEaten, x + 91 + 20 + 45, y + 140 + 10);
    copy_buffer_to_mem();
    message msg;
    int ipc_status, r;
    uint8_t irq_keyboard = 1, irq_timer = 0, irq_mouse = 2;

    if (keyboard_subscribe(&irq_keyboard, 0))
        return 1;
    if (timer_subscribe(&irq_timer, 1))
        return 1;
    if (mouse_subscribe(&irq_mouse, 2))
        return 1;

    int good = 1;
    while (good) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_keyboard) {
                        kbc_ih();
                        if (done) {
                            if (scanCode[0] == ENTER_BREAK_CODE) {
                                good = 0;
                                baseState = mainMenu;
                            }
                            size = 0;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_timer) {
                        copy_buffer_to_mem();
                        draw_xpm_video_mem(c.img, c.img.bytes, c.x, c.y);
                    }

                    if (msg.m_notify.interrupts & irq_mouse) {
                        mouse_ih();
                        if (mouse_number_bytes >= 3) {
                            struct packet p = parse_packet();
                            c.x += p.delta_x;
                            c.y -= p.delta_y;
                            if (continueMenu(&c, &p))
                                good = 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
 if (mouse_enable_data_report(false))
        return 1;
 if (keyboard_unsubscribe())
        return 1;
 if (timer_unsubscribe_int()) return 1;
 if (mouse_unsubscribe())
        return 1;
    return 0;
}

void drawGameOverElements(){
    int x = (h_res - img.width) / 2;
    int y = (v_res + 120 - img.height) / 2;
    draw_xpm(timeClock.img, timeClock.map, x + 91 + 20, y + 90);
    draw_number(gameTime, x + 91 + 20 + 45, y + 90 + 10);
    Apple apple;
    initApple(&apple, x + 91 + 20, y + 140, red);
    drawApple(apple);
    draw_number(numOfApplesEaten, x + 91 + 20 + 45, y + 140 + 10);
    on_continue = false;
    copy_buffer_to_mem();
}
