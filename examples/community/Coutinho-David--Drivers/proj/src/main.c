#include <lcom/lcf.h>
#include <stdint.h>
#include "game.h"
#include "controllers/timer.h"
#include "controllers/keyboard.h"
#include "controllers/mouse.h"
#include "controllers/graphics.h"

GameStates state = MENU_START;


int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/trace.txt");
    lcf_log_output("/home/lcom/output.txt");

    if (lcf_start(argc, argv))
        return 1;

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    int ipc_status;
    int r;
    message msg;
    
    int timer_hookid = 0, keyboard_hookid = 1, mouse_hookid = 12;
    mouse_init();
    graphics_init();


    loadPixmap();
    loadCards();
    loadNumbers();
    loadCursor();

    sys_irqsetpolicy(0, IRQ_REENABLE, &timer_hookid);
    sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hookid);
    sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid);

    while (1) {
        if (exit_request) break;
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            //printf("driver_receive failed: %d\n", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & BIT(0)) timer_ih();
                    if (msg.m_notify.interrupts & BIT(1)) keyboard_ih();
                    if (msg.m_notify.interrupts & BIT(12)) mouse_ih(); 
                    break;
                default:
                    break;
            }
        }
    }

    sys_irqrmpolicy(&timer_hookid);
    sys_irqrmpolicy(&keyboard_hookid);
    sys_irqrmpolicy(&mouse_hookid);
    graphics_exit();
    vg_exit();

    return 0;
}
