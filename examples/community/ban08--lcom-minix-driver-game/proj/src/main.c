#include <lcom/lcf.h>
#include "video_card.h"
#include "timer.h"
#include "keyboard.h"
#include "game.h"
#include "kb_controller.h"

/// @brief Scan code from keyboard interrupt handler
extern uint8_t scan_code;

/// @brief Timer interrupt counter
extern int counter;

/// @brief Timer hook ID
extern int h_id;

/// @brief Keyboard hook ID
extern int keyboard_hook_id;

/// @brief X position of the player
int x_position = 40;

/// @brief Y position of the player
int y_position = 425;

/// @brief Indicates whether the character is jumping
int is_going_up = 0;

/// @brief Flag to control main loop execution
bool running = true;

int last_scan_code;


int menu = 1;

int first_screen_appearance = 1; //ajuda por causa do refresh do ecra


/**
 * @brief Main entry point of the LCF application.
 *
 * Initializes the LabCom Framework and starts the main loop.
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, 1 on failure
 */
int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    // lcf_trace_calls("/home/lcom/labs/trace.txt"); // optional
    // lcf_log_output("/home/lcom/labs/output.txt"); // optional

    if (lcf_start(argc, argv) == 1) return 1;

    return 0;
}

/**
 * @brief Main loop of the project.
 *
 * Initializes graphics mode, subscribes to timer and keyboard interrupts,
 * and handles events in an IPC loop. Exits when ESC key is pressed or
 * collision occurs.
 *
 * @param argc Argument count (unused)
 * @param argv Argument vector (unused)
 * @return 0 on success, 1 on error
 */
int (proj_main_loop)(int argc, char *argv[]) {
    if (set_frame_buffer(0x115) != 0) {
        printf("Failed to set frame buffer\n");
        return 1;
    }

    if (set_graphic_mode(0x115) != 0) {
        printf("Failed to set graphic mode\n");
        return 1;
    }

    uint8_t irq_mask;
    if (keyboard_subscribe_interruptions(&irq_mask)) return 1;
    uint8_t irq_set;
    h_id = 0;
    if (timer_subscribe_int(&irq_set)) return 1;
    int ipc_status;
    message msg;
    int r, irq_set_timer = irq_set, irq_set_kbd = irq_mask;

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();
                        game_start();  ///< Update game state every timer tick
                    }
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbc_ih();
                        if (scan_code == BREAK_CODE_ESCAPE)
                            running = false;
                        // Space key (0x39) initiates jump
                        if (scan_code == 0x39 && y_position == 425)
                            is_going_up = 1;
                        if (scan_code == 0x1F && menu==1 && last_scan_code != 0x19){
                            menu = 0; // 0x19 é o scan_code da tecla P e 0x1F é o scan code da tecla S
                            first_screen_appearance = 1;
                        }
                        if (scan_code==0x19 && menu == 0 && last_scan_code != 0x19){
                            menu = 1;
                            first_screen_appearance = 1;
                        }
                    }
                    break;
            }
        }
        last_scan_code = scan_code;
    }
    timer_unsubscribe_int();
    keyboard_unsubscribe_interruptions();
    vg_exit();  ///< Exit graphics mode
    return 0;
}
