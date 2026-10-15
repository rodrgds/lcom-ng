#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdlib.h>
#include <string.h>

#include "video.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"

#include "input/TimerInterpreter.h"
#include "controller/Controller.h"

void lazy_link();

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) {
        return 1;
    }

    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    if (video_init_page_flipping(0x118) != 0) {
        return 1;
    }

    Controller *ctrl = controllerCreate();
    if (!ctrl) {
        vg_exit();
        return 1;
    }

    lazy_link();

    uint8_t timer_bit_no;
    if (timer_subscribe_int(&timer_bit_no) != 0) {
        controllerDestroy(ctrl);
        vg_exit();
        return 1;
    }

    uint8_t kbd_bit_no;
    if (kbd_subscribe_int(&kbd_bit_no) != 0) {
        timer_unsubscribe_int();
        controllerDestroy(ctrl);
        vg_exit();
        return 1;
    }

    uint8_t mouse_bit_no;
    if (mouse_subscribe_int(&mouse_bit_no) != 0) {
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        controllerDestroy(ctrl);
        vg_exit();
        return 1;
    }

    uint32_t timer_irq_set = BIT(timer_bit_no);
    uint32_t kbd_irq_set = BIT(kbd_bit_no);
    uint32_t mouse_irq_set = BIT(mouse_bit_no);

    TimerState tstate;
    timerStateInit(&tstate, 1);

    int r;
    int ipc_status;
    message msg;

    while (ctrl->state.running) {
        r = driver_receive(ANY, &msg, &ipc_status);

        if (r != 0) {
            continue;
        }

        if (!is_ipc_notify(ipc_status)) {
            continue;
        }

        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & kbd_irq_set) {
                    kbd_ih();

                    if (kbd_error) {
                        kbd_error = false;
                        reset_scancode();
                        break;
                    }

                    if (scancode_complete()) {
                        keyboardInterpretScancode(
                            &ctrl->kbd,
                            get_scancode_bytes(),
                            get_scancode_size()
                        );

                        reset_scancode();
                    }
                }

                if (msg.m_notify.interrupts & mouse_irq_set) {
                    mouse_ih();

                    if (packet_complete()) {
                        mouseInterpretPacket(
                            &ctrl->mouse,
                            get_packet(),
                            MODE105_WIDTH,
                            MODE105_HEIGHT
                        );

                        reset_packet();
                    }
                }

                if (msg.m_notify.interrupts & timer_irq_set) {
                    timerInterpretInterrupt(&tstate);

                    if (tstate.should_update) {
                        controllerUpdate(ctrl, tstate.second_elapsed);

                        if (ctrl->state.running) {
                            controllerDraw(ctrl);
                        }
                    }
                }

                break;

            default:
                break;
        }
    }

    mouse_unsubscribe_int();
    kbd_unsubscribe_int();
    timer_unsubscribe_int();

    controllerDestroy(ctrl);
    vg_exit();

    return 0;
}
