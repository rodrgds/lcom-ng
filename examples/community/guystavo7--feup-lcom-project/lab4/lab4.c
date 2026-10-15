// IMPORTANT: you must include the following line in all your C files
#include "mouse.h"
#include "utilsMouse.h"
#include <lcom/lcf.h>

#include "../lab2/timer.h"
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab4/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

void getInputs(int* i) {
    mouse_ih();
    if (!isReady()) return;

    struct packet p;

    p.bytes[0] = getByte1();
    p.bytes[1] = getByte2();
    p.bytes[2] = getByte3();
    p.rb = pressedRightButton();
    p.mb = pressedMiddleButton();
    p.lb = pressedLeftButton();
    p.delta_x = getDeltaX();
    p.delta_y = getDeltaY();
    p.x_ov = isOverflowingX();
    p.y_ov = isOverflowingY();
    mouse_print_packet(&p);
    (*i)--;
    cleanBytes();
}

int(mouse_test_packet)(uint32_t cnt) {
    int hook_id = MOUSE_IRQ_LINE;

    mouse_enable_data_reporting();
    mouseSubscribe(&hook_id);
    int r, ipc_status;
    message msg;
    int i = (int)cnt;

    while (i > 0) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Driver receive failed: %d\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << MOUSE_IRQ_LINE)) getInputs(&i);
                break;

            default:
                break;
        }
    }

    mouseUnsubscribe(&hook_id);
    disableDataReport();
    return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
    int hook_id_mouse = MOUSE_IRQ_LINE;
    uint8_t hook_id_timer = 0;

    mouseSubscribe(&hook_id_mouse);
    timer_subscribe_int(&hook_id_timer);
    enableDataReport();

    int r, ipc_status, i = 0;
    message msg;

    while (timer_get_counter() <= idle_time * 60) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Driver receive failed: %d\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << MOUSE_IRQ_LINE)) {
                    getInputs(&i);
					timer_reset_counter();
                }

                if (msg.m_notify.interrupts & (1 << 0)) timer_int_handler();
                break;

            default:
                break;
        }
    }

    mouseUnsubscribe(&hook_id_mouse);
    timer_unsubscribe_int();
    disableDataReport();

    return 0;
}
