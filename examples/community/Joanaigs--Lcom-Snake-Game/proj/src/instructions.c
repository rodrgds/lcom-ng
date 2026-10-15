#include "instructions.h"
#include "gameOver.h"
#include "graphics.h"
#include "header.h"
#include "i8042.h"
#include "images/instructions.xpm"
#include "images/instructions_back.xpm"
#include "images/mouse_cursor.xpm"
#include "keyboard.h"
#include "objects.h"
#include "apple.h"
#include "snake.h"
#include "timer.h"
#include "mouse.h"
#include "menu.h"
#include "proj.h"

bool on_back=false;


void init_instructions(){
    instructions_menu.map=xpm_load((xpm_map_t)instructions_xpm, XPM_8_8_8, &(instructions_menu.img));
    instructions_back.map=xpm_load((xpm_map_t)instructions_back_xpm, XPM_8_8_8, &(instructions_back.img));
}

int menuInstructionsHover(cursor *mouse_c){
    if(mouse_c->y>=522 && mouse_c->y<=566 && mouse_c->x>=306 && mouse_c->x<=487)
        return 1;
    return 0;
}

int instructionsMenu(cursor *c, struct packet *p){
    struct mouse_ev event = mouse_get_event(p);

    if(menuInstructionsHover(c)) {

        if (event.type == LB_RELEASED) {
            baseState = mainMenu;
            return 1;
        }
        if(!on_back){
            on_back = true;
             draw_xpm(instructions_back.img, instructions_back.map, 0, 0);
            copy_buffer_to_mem();
            draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
        }
    }
    else{
        if(on_back){
            draw_xpm(instructions_menu.img, instructions_menu.map, 0, 0);
            copy_buffer_to_mem();
            on_back=false;
        }
        
    }
    return 0;
}

int instructionsLoop() {
    cursor c;
    c.x = 200;
    c.y = 200;
    xpm_load((xpm_map_t)mouse_cursor, XPM_8_8_8, &(c.img));
    if (mouse_enable_data_report(true))
        return 1;
    init_instructions();
    draw_xpm(instructions_menu.img, instructions_menu.map, 0, 0);
    copy_buffer_to_mem();

    message msg;
    int ipc_status, r;
    uint8_t irq_timer = 0, irq_mouse = 2;

    if (timer_subscribe(&irq_timer, 0)) return 1;
    if(mouse_subscribe(&irq_mouse, 1)) return 1;

    int good = 1;
    while (good) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
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
                            if(instructionsMenu(&c, &p))
                                good=0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        else {
        }
    }
    if (mouse_enable_data_report(false))
        return 1;
    if (timer_unsubscribe_int())
        return 1;
    if (mouse_unsubscribe())
        return 1;
    return 0;
}
