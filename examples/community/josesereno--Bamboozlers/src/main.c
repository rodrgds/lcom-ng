#include "controller/timer/timer.h"
#include "controller/video/graphics.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/KBC/KBC.h"
#include "controller/RTC/rtc.h"
#include "model/model.h"
#include "model/sprite.h"
#include "states/state.h"

/**
 * @mainpage Game Documentation
 *
 * Bamboozlers is an adaptation of the game Transformice, where the user takes on the role of a panda
 * instead of a mouse. The objective is to build a path using planks and wooden boxes so that the 
 * panda can reach its food, bamboo. After collecting it, the panda must return home in order
* to complete a level of the game.
 *
 * Each level presents a unique combination of platforms and traps that require strategic thinking
  * and must be overcome within a time limit. Additionally, the number of planks and boxes that 
  * the panda can use is also limited.
 */
extern Menu_state menu_state; 
uint8_t rtc_mask;
uint32_t timer_mask, keyboard_mask, mouse_mask;

int (main)(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/project/debug/trace.txt");
    lcf_log_output("/home/lcom/labs/project/debug/output.txt");
    if (lcf_start(argc, argv)) return 1;
    lcf_cleanup();
    return 0;
}

int init() {

    if (timer_set_frequency(0, 30)) return 1;
    if (set_alarm_upd_interrupt(true)) return 1;
    
    if (set_main_buffer(VBE_DIRECT_600p)) return 1;
    set_drawing_buffer();

    if (set_graphic_mode(VBE_DIRECT_600p)) return 1;

    create_sprites();

    if (timer_subscribe_interrupt(&timer_mask)) return 1;
    if (keyboard_subscribe_int(&keyboard_mask)) return 1;
    if (mouse_subscribe_int(&mouse_mask)) return 1;
    if (rtc_subscribe_int(&rtc_mask)) return 1;
    
    if (enable_data_reporting()) return 1;
    
    rtc_upd();
    set_darkMode_alarm();

    return 0;
}


int cleanup() {

    if (set_alarm_upd_interrupt(false)) return 1;
    if (set_text_mode()) return 1;

    destroy_sprites();

    if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe_int()) return 1;
    if (mouse_unsubscribe_int()) return 1;
    if (rtc_unsubscribe_int()!= 0) return 1;

    if (disable_data_reporting()) return 1;

    return 0;
}


int (proj_main_loop)(int argc, char *argv[]) {
  
    if (init() ) return 1;

    draw_menu();

    int ipc_status;
    message msg;
    while (menu_state != EXIT) {
        if (driver_receive(ANY, &msg, &ipc_status) ) {
            printf("Error");
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & timer_mask) update_timer_state();
                    if (msg.m_notify.interrupts & keyboard_mask) update_keyboard_state();
                    if (msg.m_notify.interrupts & mouse_mask)  update_mouse_state();
                    if (msg.m_notify.interrupts & rtc_mask) rtc_ih();
            }
        }
    }
    
    if (cleanup()) return 1;

    return 0;
}
