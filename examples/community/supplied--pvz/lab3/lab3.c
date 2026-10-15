#include <lcom/lcf.h>

#include "keyboard.h"
#include "i8042.h"
#include "timer.h"

uint32_t get_sys_inb_counter();

int (kbd_test_scan)() {
    uint8_t bit_no;
    if (kbd_subscribe_int(&bit_no) != 0) return 1;

    int ipc_status, r;
    message msg;
    uint32_t irq_set = BIT(bit_no);
    uint8_t size = 1, bytes[2];
    bool make;

    while (true) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & irq_set) {
                kbc_ih();
                if (get_kbd_error()) continue;

                if (get_scancode() == TWO_BYTE_CODE) {
                    size = 2;
                    bytes[0] = get_scancode();
                    continue;
                }

                bytes[size - 1] = get_scancode();
                make = !(get_scancode() & BIT(7));
                kbd_print_scancode(make, size, bytes);
                size = 1;

                if (get_scancode() == ESC_BREAKCODE) break;
            }
        }
    }

    kbd_unsubscribe_int();
    kbd_print_no_sysinb(get_sys_inb_counter());
    return 0;
}

int (kbd_test_poll)() {
    uint8_t size = 1, bytes[2];
    bool make;

    while (true) {
        if (kbd_poll() != 0 || get_kbd_error()) continue;

        if (get_scancode() == TWO_BYTE_CODE) {
            size = 2;
            bytes[0] = get_scancode();
            continue;
        }

        bytes[size - 1] = get_scancode();
        make = !(get_scancode() & BIT(7));
        kbd_print_scancode(make, size, bytes);
        size = 1;

        if (get_scancode() == ESC_BREAKCODE) break;
    }

    kbc_restore_interrupts();
    kbd_print_no_sysinb(get_sys_inb_counter());
    return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {
    uint8_t bit_no_kbd, bit_no_timer;
    if (kbd_subscribe_int(&bit_no_kbd) != 0) return 1;
    if (timer_subscribe_int(&bit_no_timer) != 0) return 1;


    int ipc_status, r;
    message msg;

    uint32_t irq_set_kbd   = BIT(bit_no_kbd);
    uint32_t irq_set_timer = BIT(bit_no_timer);

    uint8_t size = 1, bytes[2];
    bool make;

    while (true) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {

            /* Timer */
            if (msg.m_notify.interrupts & irq_set_timer) {
                timer_int_handler();
                if (get_timer_counter() / 60 >= n) break; /* timeout por inatividade */
            }

            /* Teclado */
            if (msg.m_notify.interrupts & irq_set_kbd) {
                kbc_ih();
                if (get_kbd_error()) continue;

                if (get_scancode() == TWO_BYTE_CODE) {
                    size = 2;
                    bytes[0] = get_scancode();
                    continue;
                }

                bytes[size - 1] = get_scancode();
                make = !(get_scancode() & BIT(7));
                kbd_print_scancode(make, size, bytes);
                size = 1;

                reset_timer_counter();

                if (get_scancode() == ESC_BREAKCODE) break;
            }
        }
    }

    kbd_unsubscribe_int();
    timer_unsubscribe_int();
    return 0;
}

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");
    lcf_log_output("/home/lcom/labs/lab3/output.txt");

    if (lcf_start(argc, argv))
        return 1;

    lcf_cleanup();
    return 0;
}
