/**
 * @file main.c
 * @brief Main entry point and event loop for the typing test application.
 *
 * Initializes video mode, devices, and handles the main event loop,
 * including interrupt processing and application state transitions.
 */

#include <stdio.h>
#include <lcom/lcf.h>

#include "views/views.h"
#include "controllers/interrupts.h"
#include "model/model.h"

/**
 * @brief Current application state.
 */
extern State current_state;

/**
 * @brief Initializes the application, video mode, mouse, and device interrupts.
 * @return 0 on success, 1 on failure.
 */
int start(){
    // Enter Video Mode
    if (video_init(0x118) != 0) return 1; // 24-bit color 1024x768

    // Enable mouse data reporting
    if (mouse_enable_data_reporting() != 0) return 1;

    // Subscribe to device interrupts
    if (subscribe_devices() != 0) return 1;

    start_game();

    draw_screen(current_state);

    return 0;
}

/**
 * @brief Cleans up and shuts down the application, unsubscribing devices and exiting video mode.
 * @return 0 on success, 1 on failure.
 */
int end(){
    // Unsubscribe to devices
    if (unsubscribe_devices() != 0) return 1;

    // Disable data reporting
    if (mouse_disable_data_reporting() != 0) return 1;

    // Exit Video mode
    vg_exit();
    return 0;
}

/**
 * @brief Main event loop for the application.
 *
 * Receives and handles hardware interrupts and updates application state.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, 1 on failure.
 */
int (proj_main_loop)(int argc, char* argv[]) {

    if(start()) return 1;
    
    //uint8_t irq_set_keyboard;
    int ipc_status, r;
    message msg;

    while (current_state != END) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    interrupts_handler(msg.m_notify.interrupts);
                default:
                    break;
            }
        }
    }

    if (end()) return 1;

    return 0;
}

/**
 * @brief Program entry point.
 *
 * Initializes LCF, starts the main loop, and performs cleanup.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, 1 on failure.
 */
int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/grupo_2leic07_1/proj/src/trace.txt");
    lcf_log_output("/home/lcom/labs/grupo_2leic07_1/proj/src/output.txt");
  
    if (lcf_start(argc, argv)) return 1;
  
    lcf_cleanup();
  
    return 0;
}
