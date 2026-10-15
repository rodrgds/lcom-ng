#include <lcom/lcf.h>

#include <stdint.h>

#include "devices/timer/timer.h"
#include "game/game.h"

bool quit = false;

extern int timer_int_counter;
extern uint8_t mouse_counter;
extern bool is_scode_complete;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  lcf_log_output("/home/lcom/labs/proj/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

int (interrupts)() {

    if (mouse_enable_stream_mode()) {
        return 1;
    }

    if (mouse_enable_dt_reporting()) {
        return 1;
    }

    if (timer_set_frequency(0, 60)) {
        return 1;
    }

    uint8_t bit_no;
    uint32_t timer_irq_set, kbc_irq_set, mouse_irq_set;
    if (timer_subscribe_int(&bit_no)) {
        return 1;
    }
    timer_irq_set = BIT(bit_no);

    if (kbc_subscribe_int(&bit_no)) {
        return 1;
    }
    kbc_irq_set = BIT(bit_no);

    if (mouse_subscribe_int(&bit_no)) {
        return 1;
    }
    mouse_irq_set = BIT(bit_no);

    int ipc_status;
    message msg;
    while (!quit) {
        int r;
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed %d", r);
            continue;
        }

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_ih();

                        if (run_game()) {
                            return 1;
                        }

                        if (timer_int_counter % 60 == 0) {
                            main_game_timer();
                            waiting_mode_timer();
                        }
                    }

                    if (msg.m_notify.interrupts & kbc_irq_set) {
                        kbc_ih();

                        if (is_scode_complete) {
                            game_kbc();
                        }
                    }

                    if (msg.m_notify.interrupts & mouse_irq_set) {
                        mouse_ih();
                        mouse_parse_packet();

                        if (mouse_counter == 3) {
                            mouse_counter = 0;
                            game_mouse();
                        }
                    }

                    break;
                default:
                    break;  
            }
        }
    }

    if (timer_unsubscribe_int()) {
        return 1;
    }

    if (kbc_unsubscribe_int()) {
        return 1;
    }

    if (mouse_unsubscribe_int()) {
        return 1;
    }

    if (mouse_disable_dt_reporting()) {
        return 1;
    }

    return 0;
}


int (proj_main_loop)(int argc, char **argv) {

    if (map_vram(R800x600_DC)) {
        return 1;
    }

    if (set_graphic_mode(R800x600_DC)) {
        return 1;
    }

    if (create_all_sprites()) {
        return 1;
    }

    if (create_back_buffer()) {
        return 1;
    }

    if (interrupts()) {
        return 1;
    }

    if (vg_exit()) {
        return 1;
    }

    destroy_back_buffer();
    destroy_all_sprites();

    return 0;
}
