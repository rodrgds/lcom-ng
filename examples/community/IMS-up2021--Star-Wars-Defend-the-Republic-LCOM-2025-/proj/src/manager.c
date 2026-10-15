#include <sys/time.h>
#include <lcom/lcf.h>
#include "manager.h"




uint32_t timer_irq_set, kbd_irq_set, mouse_irq_set;
bool mouse_ready;

int kbd_i;
int timer_counter;

uint64_t delta_time;

/**
 * @brief Initializes the graphics system, input devices, and game assets.
 *
 * This function performs several critical setup steps:
 * 1. Subscribes to interrupts for the timer, keyboard, and mouse.
 * 2. Stores the IRQ bitmasks for these devices in global variables (`timer_irq_set`, `kbd_irq_set`, `mouse_irq_set`).
 * 3. Sends a command to the mouse to enable data reporting.
 * 4. Initializes the frame buffer for the specified graphics mode (VBE_1024p_DC).
 * 5. Sets the video controller to the specified graphics mode.
 * 6. Sets the frequency of timer 0 to 60 Hz.
 * 7. Loads all necessary visual assets for the menu, instructions screen, and the main game.
 *
 * If any step fails, it attempts to clean up by unsubscribing any previously subscribed interrupts
 * and then returns an error code (1).
 *
 * @return int 0 upon successful initialization of all components, 1 if any error occurs.
 */
int (initialize_graphics)() {
    uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no;

    // Subscribe to timer interrupts
    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("Error subscribing timer interrupts\n");
        return 1;
    }

    // Subscribe to keyboard interrupts
    if (kbd_subscribe_int(&kbd_bit_no) != 0) {
        printf("Error subscribing keyboard interrupts\n");
        timer_unsubscribe_int(); // Clean up timer subscription
        return 1;
    }  

    // Enable mouse data reporting
    if (mouse_write(ENBL_DATA_REP)) { // Assuming ENBL_DATA_REP is defined in i8042.h
        printf("Error enabling mouse data reporting\n");
        // mouse_unsubscribe_int(); // Mouse not subscribed yet, but KBC state might be affected.
                                 // This call might be premature or unnecessary here if mouse_write failed
                                 // without successfully enabling reporting.
        kbd_unsubscribe_int();   // Clean up keyboard subscription
        timer_unsubscribe_int(); // Clean up timer subscription
        return 1;
    } 

    // Subscribe to mouse interrupts
    if (mouse_subscribe_int(&mouse_bit_no) != 0) {
        printf("Error subscribing mouse interrupts\n");
        // Attempt to disable mouse data reporting as a cleanup if subscription fails after enabling it.
        // mouse_write(DIS_DATA_REP); // Or some other cleanup for the mouse state.
        kbd_unsubscribe_int();   // Clean up keyboard subscription
        timer_unsubscribe_int(); // Clean up timer subscription
        return 1;
    }

    // Store IRQ bitmasks
    timer_irq_set = BIT(timer_bit_no);
    kbd_irq_set = BIT(kbd_bit_no);
    mouse_irq_set = BIT(mouse_bit_no);

    uint16_t mode_to_set = VBE_1024p_DC; // Renamed for clarity from mode_info to avoid conflict with global mode_info
                                        // if this were in a context where global mode_info is vbe_mode_info_t.

    // Initialize frame buffer for the chosen graphics mode
    if (set_frame_buffer(mode_to_set) == 1) { // LCF's set_frame_buffer usually doesn't return 1 for error, but 0 for success.
                                            // Assuming 1 is error as per the check.
        printf("Error initializing graphics mode (frame buffer setup)\n");
        mouse_unsubscribe_int();
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        return 1;
    }

    // Set the graphics mode
    if (set_graphic_mode(mode_to_set) == 1) { // LCF's set_graphic_mode usually returns non-zero for error.
                                             // Assuming 1 is error as per the check.
        printf("Error setting graphics mode\n");
        // vg_exit(); // If frame buffer was mapped, vg_exit might be needed here.
        mouse_unsubscribe_int();
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        return 1;
    } 

    // Set timer frequency
    if (timer_set_frequency(0, 60) != 0) { 
        printf("ERRO: Falha ao configurar a frequencia do Timer 0!\n");
        // Potentially add cleanup here as well if this is critical
    }

    // Load game assets
    load_menu_assets();
    load_instructions_assets();
    load_game_assets(); // Assuming load_game_assets() is defined elsewhere
    
    return 0; // Success
}


/**
 * @brief Cleans up resources and prepares the system for game termination.
 *
 * This function performs the necessary cleanup operations before the game exits:
 * 1. Unsubscribes mouse interrupts.
 * 2. Unsubscribes keyboard interrupts.
 * 3. Unsubscribes timer interrupts.
 * 4. Attempts to re-enable mouse data reporting. This step is unusual for an exit sequence,
 *    as disabling data reporting or resetting the mouse to defaults is more common.
 *    It might be intended to restore a specific mouse state expected by the OS/environment.
 * 5. Exits the graphics mode and returns to text mode using `vg_exit()`.
 *
 * If any unsubscription or cleanup step fails, it prints an error message and returns
 * an error code (1). `fflush(stdout)` is used to ensure error messages are displayed
 * immediately, which is useful if the program terminates abruptly afterwards.
 *
 * @return int 0 upon successful cleanup of all components, 1 if any error occurs.
 */
int (exit_game)() {
    // Unsubscribe mouse interrupts
    if (mouse_unsubscribe_int() != 0) {
        printf("Error unsubscribing mouse interrupts\n");
        fflush(stdout);
        // Continue cleanup despite error
    }

    // Unsubscribe keyboard interrupts
    if (kbd_unsubscribe_int() != 0) {
        printf("Error unsubscribing keyboard interrupts\n");
        fflush(stdout);
        // Continue cleanup despite error
    }

    // Unsubscribe timer interrupts
    if (timer_unsubscribe_int() != 0) {
        printf("Error unsubscribing timer interrupts\n");
        fflush(stdout);
        // Continue cleanup despite error
    }

    // Manage mouse data reporting state before exiting
    // Note: Enabling data reporting at exit is unusual. Typically, one might disable it
    // or reset mouse to defaults. This implies a specific reason for re-enabling.
    // Assuming `mouse_enable_data_reporting()` is a wrapper around `mouse_write(ENBL_DATA_REP)`
    // or a similar command to ensure mouse is actively reporting.
    if (mouse_write(ENBL_DATA_REP) != 0) { // Changed to mouse_write(ENBL_DATA_REP) for consistency with i8042.h
                                          // if mouse_enable_data_reporting() was a placeholder.
                                          // If it's a distinct function, its specific implementation matters.
        printf("Error managing mouse data reporting state on exit\n");
        fflush(stdout);
        // Continue cleanup despite error
    }

    // Exit graphics mode and return to text mode
    if(vg_exit() != 0) {
        printf("Error exiting graphics mode\n");
        fflush(stdout);
        return 1; // This error is more critical for system stability
    }

    return 0; // Success
}
