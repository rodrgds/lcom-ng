#include <lcom/lcf.h>
#include "Controller/Timer/timer.h"
#include "Controller/Timer/i8254.h"
#include "Controller/Video/graphic.h"
#include "Controller/Keyboard/keyboard.h"
#include "Controller/Mouse/mouse.h"
#include "Controller/RTC/rtc.h"
#include "Model/model.h"
#include "Model/game.h"
#include "State/state.h"
#include "Viewer/view.h"
#include "configs.h"

extern ProgramState programState;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    //lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    //lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}
int init_game(){
    if(timer_set_frequency(TIMER,FREQUENCY)!=0)return 1;
    if(set_frame_buffer(VIDEO_MODE)!=0)return 1;
    if(set_graphic_mode(VIDEO_MODE)!=0)return 1;
    if(timer_subscribe_interrupts()!=0)return 1;
    if(keyboard_subscribe_interrupts()!=0)return 1;
    if(mouse_subscribe_int()!=0)return 1;
    if(rtc_subscribe_int()!=0)return 1;
    if(mouse_write(ENABLE_DATA_REPORTING)!=0)return 1;
    if(mouse_write(SET_STREAM_MODE)!=0)return 1;

    create_sprites();
    return 0;
}
int shut_down(){
    if(vg_exit()!=0)return 1;
    if(timer_unsubscribe_int()!=0)return 1;
    if(keyboard_unsubscribe_interrupts()!=0)return 1;
    if(mouse_unsubscribe_int()!=0)return 1;
    if(rtc_unsubscribe_int()!=0)return 1;
    if(mouse_write(DISABLE_DATA_REPORTING)!=0)return 1;
    destroy_sprites();
    return 0;
}
int (proj_main_loop)(int argc, char *argv[]){
    if(init_game()!=0)return shut_down();
    draw_state();
    int ipc_status,r;
    message msg;
    while(programState != END) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & TIMER_IRQ_SET){
                        timer_state();
                    }
                    if (msg.m_notify.interrupts & KEYBOARD_IRQ_SET){
                        keyboard_state();
                    }
                    if (msg.m_notify.interrupts & MOUSE_IRQ_SET){
                        mouse_state();
                    }
                    if (msg.m_notify.interrupts & 5){
                        rtc_state();
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return shut_down();
}
