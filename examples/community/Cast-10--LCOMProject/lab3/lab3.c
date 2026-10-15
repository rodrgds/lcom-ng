#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include "keyboard.h"
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#include <lcom/timer.h>

#define DELAY_US 20000

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (kbd_test_scan)(void) {
    int r, ipc_status;
    message msg;
    bool running = true;
    uint8_t bit_no;

    if (kbd_subscribe_int(&bit_no) != 0) return 1;
    uint32_t irq_set = BIT(bit_no);

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        kbd_ih();
                        if (kbd_error) {
                            kbd_error = false;
                            reset_scancode();
                            continue;
                        }
                        if (scancode_complete()) {
                            uint8_t size = get_scancode_size();
                            uint8_t *bytes = get_scancode_bytes();
                            bool is_make = !(bytes[size - 1] & BREAKCODE);
                            if (size == 1 && bytes[0] == (ESC | BREAKCODE)) running = false;
                            if (kbd_print_scancode(is_make, size, bytes) != 0) return 1;
                            reset_scancode();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) return 1;
    return 0;
}

int(kbd_test_poll)() {
    uint8_t status;
    uint8_t scancode;
    bool running = true;

    while (running) {

        if (util_sys_inb(STATUS_REGISTER, &status) != 0)
            continue;


        if (status & OUTPUT_BUFFER_FULL) {

            if (status & (PARITY_ERROR | TIMEOUT_ERROR))
                continue;

            if (util_sys_inb(OUTPUT_BUFFER, &scancode) != 0)
                continue;

            process_scancode_byte(scancode);

            if (scancode_complete()) {

                uint8_t *bytes = get_scancode_bytes();
                uint8_t size = get_scancode_size();
                bool is_make = !(bytes[size-1] & BREAKCODE);

                if (kbd_print_scancode(is_make, size, bytes) != 0)
                    continue;

                if (bytes[size - 1] == (ESC | BREAKCODE)) {
                    running = false;
                }

                reset_scancode();
            }
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }

    enable_keyboard_interrupts();

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
    int r, ipc_status;
    message msg;
    bool running = true;
    uint8_t bit_kbd, bit_timer;
    uint32_t irq_set_timer, irq_set_kbd;
    uint32_t keyboard_time = 0;

    extern uint32_t counter;

    counter = 0;

    if (timer_subscribe_int(&bit_timer) != 0) return 1;
    if (kbd_subscribe_int(&bit_kbd) != 0) {
        timer_unsubscribe_int();
        return 1;
    }

    irq_set_timer = BIT(bit_timer);
    irq_set_kbd = BIT(bit_kbd);

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbd_ih();

                        if (kbd_error) {
                            kbd_error = false;
                            reset_scancode();
                            continue;
                        }

                        keyboard_time = counter;

                        if (scancode_complete()) {
                            uint8_t size = get_scancode_size();
                            uint8_t *bytes = get_scancode_bytes();
                            bool is_make = !(bytes[size - 1] & BREAKCODE);

                            if (kbd_print_scancode(is_make, size, bytes) != 0) {
                                kbd_unsubscribe_int();
                                timer_unsubscribe_int();
                                return 1;
                            }

                            if (size == 1 && bytes[0] == (ESC | BREAKCODE))
                                running = false;

                            reset_scancode();
                        }
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();

                        if ((counter - keyboard_time) >= n * sys_hz())
                            running = false;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;

    return 0;
}
