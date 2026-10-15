/**
 * @file gameState.c
 * @brief Manages the main game loop, game states, and event dispatching.
 *
 * This file contains the central game loop `gameLoop()`, which handles
 * hardware interrupts (timer, keyboard, mouse) and dispatches events
 * to appropriate handlers based on the current game state.
 */

#include <lcom/lcf.h>

#include "gameState.h"
#include "menu.h"
#include "entity.h"
#include "manager.h"
#include "enemy.h"
#include "characters.h"
#include "health.h"

#include "controllers/keyboardMouse/mouse.h"
#include "controllers/keyboardMouse/keyboard.h"
#include "controllers/timer/timer.h"

#include "handlers/mouse_handler.h"
#include "handlers/timer_handler.h"
#include "handlers/kbd_handler.h"


extern bool mouse_ready;
extern Cursor *cursor;
extern uint8_t *double_buffer;
extern int timer_global_counter;

struct packet mouse_packet; 

int kbd_index;
uint8_t scancode[2];

gameState state = MAIN_MENU;

unsigned int health_player = MAX_HEALTH;
unsigned int health_enemy = MAX_HEALTH;

/**
 * @brief The main game loop.
 *
 * Initializes game components and then enters a loop to process hardware interrupts
 * (timer, keyboard, mouse). It calls appropriate handlers for these events and manages
 * screen updates with double buffering. The loop continues until the game state is set to EXIT.
 */
void gameLoop(void) {
    int ipc_status, r;
    message msg;
    bool running = true;
    lcf_log_output("/home/lcom/labs/grupo_2leic18_2/proj/src/output.txt");

    init_cursor();
    init_enemies();
    init_player_units();
    init_healthbar();

    while (running) {
        if (state == EXIT) {
            running = false;
            break;
        }
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(ipc_status)) {
            if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
                if (msg.m_notify.interrupts & timer_irq_set) {
                    timer_int_handler();
                    if (timer_global_counter % 2 == 0) {
                        unsigned int _frame_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
                        memset(double_buffer, 0, _frame_size);

                        timer_event_handler(state);

                        draw_cursor(cursor);

                        vg_swap_buffers();
                    }
                }
                if (msg.m_notify.interrupts & kbd_irq_set) {
                    kbc_ih();

                    if (scancode[kbd_index] == TWO_BYTE_CODE) {
                        kbd_index++;
                        continue;
                    }

                    kbd_index = 0;

                    kbd_event_handler(scancode);

                    scancode[0] = 0;
                    scancode[1] = 0;
                }

            if (msg.m_notify.interrupts & mouse_irq_set) {
                mouse_ih();
                if (mouse_ready) {
                    mouse_event_handler(mouse_packet);
                    mouse_ready = false;
                }
            }
                
            } else {
                printf("Loop: NAO ENTROU no if is_ipc_notify. msg.m_type foi 0x%X.\n", msg.m_type);
            }
        }
    }
}
