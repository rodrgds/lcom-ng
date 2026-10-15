#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/timer.h"
#include "drivers/video.h"
#include "helper/structs.h"
#include "states/menu.h"
#include "states/game.h"

int (*map_funcs[2][4])(void) = { // array com as funcoes map_funcs[0][1] para desenhar o menu state
    { load_menu_state, draw_menu_state, kbd_menu_ih, mouse_menu_ih },
    { load_game_state, draw_game_state, kbd_game_ih, mouse_game_ih }
};
int state = 0; // menu state

extern struct Sprite *mouse;
bool done = false;
extern int packet_byte_count;
extern uint8_t packet[3];
struct mouse_packet pack;
extern vbe_mode_info_t mode_info;
extern unsigned int vram_size;
extern uint8_t* video_mem;
extern uint8_t* temp_mem;


int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {
    uint8_t mouse_irq, timer_irq, kbd_irq;

    if (setVBECommand(0x02, 0x115) != 0) return 1;
    get_map_mem(0x115);

    if (timer_int_sub(&timer_irq) != 0) return 1;
    if (keyboard_subscribe_int(&kbd_irq) != 0) return 1;
    if (mouse_int_sub(&mouse_irq) != 0) return 1;
    if (mouse_set_reporting(true) != 0) return 1;
    if (timer_set_freq(20) != 0) return 1; // mouse was to laggy at 60 fps

    if (map_funcs[state][0]() != 0) return 1;

    int ipc_status;
    message msg;
    while (!done) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
                printf("driver_receive failed");
                continue;
        }
        if (is_ipc_notify(ipc_status)) {
            if (_ENDPOINT_P(msg.m_source) != HARDWARE) continue;

            if (msg.m_notify.interrupts & mouse_irq) {
                mouse_ih();

                if (packet_byte_count%3 == 0 && packet_byte_count != 0) {
                    mouse_get_packet(&pack);
                    if (map_funcs[state][3]() != 0) return 1;
                }

            }
            if (msg.m_notify.interrupts & timer_irq) {
                if (map_funcs[state][1]() != 0) return 1;
                buffer_flip();
            }

            if (msg.m_notify.interrupts & kbd_irq) {
                kbc_ih();
                if (map_funcs[state][2]() != 0) return 1;
            }
        }
    }
    if (mouse_set_reporting(false) != 0) return 1;
    if (mouse_int_unsub() != 0) return 1;
    if (timer_int_unsub() != 0) return 1;
    if (keyboard_unsubscribe_int() != 0) return 1;
    if (setBIOSMode(0x03) != 0) return 1;
    printf("Game exited with success!");

    return 0;
}
