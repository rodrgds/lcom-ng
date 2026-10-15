#include <lcom/lcf.h>
#include "drivers/video_card/gpu.h"
#include "drivers/video_card/vbe.h"
#include "drivers/timer/timer.h"
#include "drivers/timer/i8254.h"
#include "drivers/mouse/i8042.h"
#include "drivers/mouse/mouse.h"
#include "drivers/keyboard/mkbd.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/rtc/rtc.h"
#include "view/viewer.h"
#include "view/sprite.h"
#include "model/cursor/cursor.h"
#include "state/state.h"

extern int counter;

extern uint8_t scan_code[2];
extern bool last_byte_read; 
extern int kbd_index;

extern struct packet packet_pp;
extern int pp_index;
extern bool packet_read;

int gameTurnCounter = 600;

rtc_timestamp_t real_time;

int main(int argc, char *argv[]) {

  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/g3/proj/src/trace.txt");

  lcf_log_output("/home/lcom/labs/g3/proj/src/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

int initialize(uint8_t* irqTimer, uint8_t* irqKeyboard, uint8_t* irqMouse, uint8_t* irqRtc){

    cursor_create(0,0);

    if(enable_data_reporting()) return 1; 
    
    if(timer_set_frequency(0,60)) return 1;

    if (load_sprites()) return 1;

    uint8_t bit_no;
    if(timer_subscribe_int(&bit_no)) return 1;
    *irqTimer = BIT(bit_no);

    if(keyboard_subscribe_int(&bit_no)) return 1;
    *irqKeyboard = BIT(bit_no);

    if(mouse_subscribe_int(&bit_no)) return 1;
    *irqMouse = BIT(bit_no);

    if(rtc_subscribe_interrupts(&bit_no)) return 1;
    *irqRtc = bit_no;

    if(rtc_initialize_system()) return 1;

    if (set_mode(VBE_MODE)) return 1;
    vbe_mode_info_t vmi_p;
    if (get_vbe_mode_info(VBE_MODE, &vmi_p)) return 1;
    if (map_info(&vmi_p)) return 1;

    init_state();

    return 0;
}

int terminate(){

    cursor_destroy();
    if(timer_unsubscribe_int()) return 1;
    if(mouse_unsubscribe_int() || disable_data_reporting()) return 1;
    if(keyboard_unsubscribe_int()) return 1;
    if(rtc_unsubscribe_interrupts()) return 1;
    if(vg_exit()) return 1;

    leave_state();
    destroy_sprites();
    
    return 0;
}

int interrupts_handler(){

        uint8_t irqTimer;
        uint8_t irqKeyboard;
        uint8_t irqMouse;
        uint8_t irqRtc;

        int ipc_status,r;
        message msg;

        if (initialize(&irqTimer, &irqKeyboard, &irqMouse, &irqRtc)){
            terminate();
            return 1;
        }

        while(state != EXIT){

            if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                continue;
            }

            if(is_ipc_notify(ipc_status)) {
                switch(_ENDPOINT_P(msg.m_source)){
                    case HARDWARE:{
                        if (msg.m_notify.interrupts & irqKeyboard){
                            kbc_ih();
                            if (last_byte_read){
                                state_kbd_handler(scan_code, kbd_index + 1);
                            }
                        }

                        if (msg.m_notify.interrupts & irqMouse) {
                            mouse_ih();
                            if (packet_read){
                                CursorInput input = read_cursor_input(&packet_pp);
                                state_mouse_handler(input);
                            }
                        }

                        if (msg.m_notify.interrupts & irqTimer) {
                            timer_ih();
                            state_timer_handler();
                        } 

                        
                        if(msg.m_notify.interrupts & irqRtc){
                            rtc_ih(&real_time);
                        }

                        break;


                }
                default:
                    break;
            }
        }
    }
    return 0;
}

int (proj_main_loop)(int argc, char *argv[]){

    if(interrupts_handler()){
        terminate();
        return 1;
    }

    if(terminate()) return 1;

    return 0;
}
