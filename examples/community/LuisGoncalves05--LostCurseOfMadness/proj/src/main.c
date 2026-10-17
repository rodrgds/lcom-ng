#include "config.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/timer/i8254.h"
#include "drivers/video/gpu.h"
#include "model/game.h"
#include <lcom/lcf.h>

uint8_t timer_mask;    /**< Timer subscription ID */
uint8_t keyboard_mask; /**< Keyboard subscription ID */
uint8_t mouse_mask;    /**< Mouse subscription ID */

extern uint32_t interrupt_counter; /**< Timer interrupt counter */
extern uint8_t scan_code;          /**< Scan code read from keyboard */
extern uint8_t packet_byte;        /**< Current Mouse packet byte */
extern uint8_t packet[3];          /**< Mouse packet */
struct packet pp;                  /**< Mouse packet structure */
uint8_t packet_idx = 0;            /**< Index for the current Mouse packet byte */
int frame_counter = 0;             /**< Frame counter for the game loop */

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/grupo_2leic03_3/proj/src/debug/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/grupo_2leic03_3/proj/src/debug/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

/**
 * @brief Sets up the necessary drivers for the main game loop.
 *
 * @return 0 on success, 1 on failure.
 */
int setup() {
    srand(time(NULL));

    if (set_frame_buffers(VIDEO_MODE) != 0)
        return 1;
    if (set_graphic_mode(VIDEO_MODE) != 0)
        return 1;
    if (timer_subscribe_int(&timer_mask) != 0)
        return 1;
    if (kbd_subscribe_int(&keyboard_mask) != 0)
        return 1;
    if (mouse_subscribe_int(&mouse_mask) != 0)
        return 1;
    if (mouse_set_data_reporting(true) != 0)
        return 1;

    return 0;
}

/**
 * @brief Resets the drivers and cleans up resources after main game loop.
 *
 * @return 0 on success, 1 on failure.
 */
int reset() {
    if (vg_exit() != 0)
        return 1;
    if (timer_unsubscribe_int() != 0)
        return 1;
    if (kbd_unsubscribe_int() != 0)
        return 1;
    if (mouse_set_data_reporting(false) != 0)
        return 1;
    if (mouse_unsubscribe_int() != 0)
        return 1;

    return 0;
}

/**
 * @brief Main loop of the game, handling interrupts and game state.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, 1 on failure.
 */
int (proj_main_loop)(int argc, char *argv[]) {
    if (setup() != 0)
        return 1;

    Game *game = create_game();

    int ipc_status;
    message msg;
    while (get_state(game) != EXIT) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Error");
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_mask) {
                        if ((interrupt_counter % sys_hz() / GAME_FREQUENCY) == 0) {
                            game_timer_handler(game);
                            frame_counter++;
                        }
                    }
                    if (msg.m_notify.interrupts & keyboard_mask) {
                        kbd_int_handler();
                        game_keyboard_handler(game);
                    }
                    if (msg.m_notify.interrupts & mouse_mask) {
                        mouse_int_handler();
                        mouse_sync_packets(packet, &packet_idx, packet_byte);
                        if (packet_idx == 3) {
                            mouse_build_packet(packet, &packet_idx, &pp);
                            packet_idx = 0;
                        }
                        game_mouse_handler(game);
                    }
                    break;
            }
        }
    }

    destroy_game(game);

    if (reset() != 0)
        return 1;

    return 0;
}
