#include "int_manager.h"

uint32_t timer_irq_set, kbd_irq_set, mouse_irq_set;
int counter;
bool codecomplete_kbd;
bool readSecond, readThird;
uint8_t scancode;
uint8_t scan_bytes_kbd[2];
uint8_t scan_bytes_mouse[3];

int (initialize)(){
    uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no;
    
    if(timer_subscribe_int(&timer_bit_no) != 0) return 1;
    if(kbc_subscribe_int(&kbd_bit_no) != 0) return 1;
    if(mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    if(mouse_enable_data_report() != 0) return 1;

    timer_irq_set = BIT(timer_bit_no);
    kbd_irq_set = BIT(kbd_bit_no);
    mouse_irq_set = BIT(mouse_bit_no);

    vg_init(DC_800x600_24);
    load_xpms();

    return 0;
}

int (main_loop)(){
    message msg;
    int ipc_status;

    while(state != EXIT) {
        int r;
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: {
                    if (msg.m_notify.interrupts & mouse_irq_set) {
                        mouse_ih();
                        if (!readSecond && !readThird && (scancode & BIT(3))) {
                            scan_bytes_mouse[0] = scancode;
                            readSecond = true;
                        }
                        else if (readSecond) {
                            scan_bytes_mouse[1] = scancode;
                            readThird = true;
                            readSecond = false;
                        }
                        else if (readThird) {
                            scan_bytes_mouse[2] = scancode;
                            readThird = false;
                            for (unsigned int i = 0; i < 3; i++) pkt.bytes[i] = scan_bytes_mouse[i];
                            buildPacket(&pkt);
                            handle_mouse_event(pkt);
                        }
                    }       
                    if (msg.m_notify.interrupts & kbd_irq_set) {
                        kbc_ih();
                        if (codecomplete_kbd) {
                            handle_kbd_event(scan_bytes_kbd);
                        }  

                    }
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_int_handler();

                        if (counter % 1 == 0) {
                            handle_timer_event();
                        }
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

int (finalize)(){
    if(timer_unsubscribe_int() != 0) return 1;
    if(kbc_unsubscribe_int() != 0) return 1;
    if(mouse_unsubscribe_int() != 0) return 1;
    if(mouse_disable_data_report() != 0) return 1;

    if(vg_exit() != 0) return 1;

    return 0;
}
