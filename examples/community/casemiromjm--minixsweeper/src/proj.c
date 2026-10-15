/// @file proj.c
/// @brief Entry point and main loop for the Minesweeper project on Minix3.

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "controller/controller.h"
#include <lcom/timer.h>

/// @brief Main entry point. Sets up LCF environment and starts the application.
/// @param argc Argument count
/// @param argv Argument vector
/// @return 0 on success, 1 on failure
int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");
  
    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
  
    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab5/output.txt");
  
    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
      return 1;
  
    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();
  
    return 0;
}

/// @brief Mouse interrupt number
uint8_t mouse_irq;

/// @brief Timer interrupt number
uint8_t timer_irq;

/// @brief Bitmask for mouse IRQ subscription
int irq_set_mouse;

/// @brief Bitmask for timer IRQ subscription
int irq_set_timer;

/// @brief Main application loop that manages the game states and transitions.
/// @param argc Argument count
/// @param argv Argument vector
/// @return 0 on normal exit, 1 on error
int(proj_main_loop)(int argc, char* argv[]) {

    if (init_devices(&mouse_irq, &timer_irq, &irq_set_mouse, &irq_set_timer) != 0) {
        printf("Error initializing devices\n");
        return 1;
    }

    GameState game_state = GAME_STATE_MENU;
    menu = create_menu();

    while (true) {

        if (game_state == GAME_STATE_MENU) {
            int result = menu_loop();
            if (result == 0) { // Start game
                game_state = GAME_STATE_RUNNING;
            } else {
                break; // Exit game
            }
        } 
        else if (game_state == GAME_STATE_RUNNING) {
            int result = game_loop(menu->selected_difficulty);
            if (result == 0) { // Game over
                game_state = GAME_STATE_OVER;
            } else if (result == 1) { // Game win
                game_state = GAME_STATE_WIN;
            } else if (result == -2) { // Return to menu
                game_state = GAME_STATE_MENU;
            }
        }
        else if (game_state == GAME_STATE_OVER) {
            int result = game_over_loop();
            if (result == 0) { // Return to menu
                game_state = GAME_STATE_MENU;
            }
        }
        else if (game_state == GAME_STATE_WIN) {
            int result = game_win_loop();
            if (result == 0) { // Return to menu
                game_state = GAME_STATE_MENU;
            }
        }
    }

    cleanup_devices();

    return 0;
}

