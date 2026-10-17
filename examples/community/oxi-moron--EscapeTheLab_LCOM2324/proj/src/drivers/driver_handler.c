#include "driver_handler.h"

extern uint32_t counter;
extern uint8_t scancode;
extern struct packet mousePacket;
extern int currentByte;
extern bool game_in_progress;
extern bool rtc_alarm;

int driver_setup() {
    uint8_t timer_bit_no = TIMER_BIT_NO, rtc_bit_no = RTC_BIT_NO
    , kbc_bit_no = KBD_BIT_NO, mouse_bit_no = MOUSE_BIT_NO;

    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_subscribe_int(&rtc_bit_no) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (keyboard_subscribe_int(&kbc_bit_no) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (mouse_subscribe_int(&mouse_bit_no) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    writeCommand(ENABLE_DATA);

    if (uart_config(COM1) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (uart_config(COM2) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (vg_start(DIR_MODE_800X600) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int driver_cleanup() {

    writeCommand(DISABLE_DATA);

    if (mouse_unsubscribe_int() != 0) {
        printf("ERROR: %s\n", __func__);
        vg_exit();
        return 1;
    }

    if (keyboard_unsubscribe_int() != 0) {
        printf("ERROR: %s\n", __func__);
        vg_exit();
        return 1;
    }

    if (rtc_unsubscribe_int() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (timer_unsubscribe_int() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (vg_exit() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    vg_free_buffer();

    return 0;
}

int main_event_loop() {
    int ipc_status, r;
    message msg;

    uint32_t timer_irq_set = BIT(TIMER_BIT_NO), rtc_irq_set = BIT(RTC_BIT_NO),
            irq_set_kbc = BIT(KBD_BIT_NO), irq_set_mouse = BIT(MOUSE_BIT_NO);

    while(game_in_progress) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d\n", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & rtc_irq_set) {
                        rtc_ih();
                        if (rtc_alarm) {
                            state_rtc_event();
                            rtc_alarm = false;
                        }
                    }
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_ih();
                        if (counter % (TIMER_FREQ / FRAME_RATE) == 0) {
                            if (state_draw_frame() != 0) {
                                printf("ERROR: %s\n", __func__);
                                return 1;
                            }
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_kbc) {
                        kbc_ih();
                        state_kbd_event(scancode);
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();
                        if (currentByte == PACKET_SIZE) {
                            currentByte = 0;
                            if (state_mouse_event(mousePacket) != 0) {
                                printf("ERROR: %s\n", __func__);
                                return 1;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {}
    }
    return 0;
}
