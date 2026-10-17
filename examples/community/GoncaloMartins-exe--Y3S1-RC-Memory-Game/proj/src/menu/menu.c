#include "menu.h"
#include "../sprites/sprites.h"
#include "../game_logic/game/Game.h"
#include "../controllers/keyboard_mouse/keyboard.h"

/// External functions from existing code
extern void desenhar_menu(void);             /**< Function to draw the main menu screen */
extern void desenhar_instrucoes(void);       /**< Function to draw the instructions screen */
extern void game_draw(const Game* game);     /**< Function to draw the game screen */
extern int cursor_x, cursor_y;               /**< Global cursor position variables */

/// Configurable Button Layout
#define RETURN_BUTTON_X      340
#define RETURN_BUTTON_Y      500
#define RETURN_BUTTON_WIDTH  (6 * 16 + 5 * 10) // Width of return button 
#define RETURN_BUTTON_HEIGHT 30               // Height of return button 

/// Global screen state
GameScreen current_screen = SCREEN_MENU; //Current screen being displayed 
Game game;                               //Global game instance 

/**
 * @brief Gets the height of a single letter sprite.
 * 
 * @return Height in pixels of a letter sprite, or 20 as fallback if not loaded.
 */

int get_letter_height() {
    return letter_A ? letter_A->height : 20;// Use sprite height or default to 20
}

/**
 * @brief Checks if the cursor is over the "Start" button.
 * 
 * @return true if the cursor is on the "Start" button, false otherwise.
 */

bool is_cursor_on_start(void) {
    int y = 200;
    int h = get_letter_height();
    return cursor_y >= y - 5 && cursor_y < y + h + 5;  // 5px padding
}

/**
 * @brief Checks if the cursor is over the "Instructions" button.
 * 
 * @return true if the cursor is on the "Instructions" button, false otherwise.
 */

bool is_cursor_on_instructions(void) {
    int y = 300;       // Instructions button Y position
    int h = get_letter_height();
    return cursor_y >= y - 5 && cursor_y < y + h + 5;  // 5px padding
}

/**
 * @brief Checks if the cursor is over the "Quit" button.
 * 
 * @return true if the cursor is on the "Quit" button, false otherwise.
 */

bool is_cursor_on_quit(void) {
    int y = 400;    // Quit button Y position
    int h = get_letter_height();
    return cursor_y >= y - 5 && cursor_y < y + h + 5;  // 5px padding
}

/**
 * @brief Checks if the cursor is over the "Return" button.
 * 
 * @return true if the cursor is on the "Return" button, false otherwise.
 */

bool is_cursor_on_return_button(void) {
    int y = 500;   // Return button Y position
    int h = get_letter_height();
    return cursor_y >= y - 5 && cursor_y < y + h + 5;  // 5px padding
}

/**
 * @brief Handles ESC key navigation based on current screen.
 */

void handle_esc_navigation(void) {
    switch (current_screen) {
        case SCREEN_GAME:
            break;
        case SCREEN_INSTRUCTIONS:
            current_screen = SCREEN_MENU;  // Return to menu from instructions
            printf("ESC: Returning from Instructions to Main Menu\n");
            break;
        case SCREEN_MENU:
            current_screen = SCREEN_QUIT;
            printf("ESC: Exiting from Main Menu\n");
            break;
        case SCREEN_QUIT:
            // Already quitting, do nothing
            break;
        default:
            current_screen = SCREEN_MENU;
            break;
    }
}

/**
 * @brief Initializes the menu screen.
 */

void menu_init(void) {
    current_screen = SCREEN_MENU;
}

/**
 * @brief Handles keyboard input while in the menu.
 * 
 * @param keycode The keycode from the keyboard input.
 */

void menu_handle_input(uint8_t keycode) {
    // Handle ESC key globally for all screens
    if (keycode == 0x01) { // ESC key
        handle_esc_navigation();
        return;
    }

    switch (current_screen) {
        case SCREEN_MENU:
            if (keycode == 0x9C) {  // Enter keycode
                // Check which button is selected and navigate accordingly
                if (is_cursor_on_start()) {

                    current_screen = SCREEN_GAME;
                    printf("Starting game...\n");
                }
                else if (is_cursor_on_instructions()) {
                    current_screen = SCREEN_INSTRUCTIONS;
                    printf("Opening instructions...\n");
                }
                else if (is_cursor_on_quit()) {
                    current_screen = SCREEN_QUIT;
                    printf("Quitting game...\n");
                }
            }
            break;

        case SCREEN_INSTRUCTIONS:
            // Return to menu if Enter pressed on return button
            if (keycode == 0x9C && is_cursor_on_return_button()) {
                current_screen = SCREEN_MENU;
                printf("Returning to menu from instructions...\n");
            }
            break;

        case SCREEN_GAME:
            break;

        case SCREEN_QUIT:
            // No input needed
            break;
    }
}

/**
 * @brief Draws the appropriate screen based on the current state.
 * 
 * @param kbd_irq Pointer to the keyboard interrupt line.
 */

void menu_draw(uint8_t* kbd_irq) {
    static Game game; // Static game instance

    switch (current_screen) {
        case SCREEN_MENU:
            desenhar_menu();   // Draw main menu
            break;
        case SCREEN_INSTRUCTIONS:
            desenhar_instrucoes();  // Draw instructions screen
            break;
        case SCREEN_GAME:
            // Draw game timer at top-left corner
            desenhar_timer(game_seconds, 10,10);

            // Run game logic and rendering  
            game_run(&game, kbd_irq);

            current_screen = SCREEN_MENU;
            my_vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000FF);
            menu_draw(kbd_irq);
            swap_buffers();
            break;
        case SCREEN_QUIT:
            desenhar_menu(); // Show menu as goodbye screen
            break;
    }
}

// --- State Accessors ---
/**
 * @brief Checks whether the application should quit.
 * 
 * @return true if the user has selected to quit, false otherwise.
 */

bool menu_should_quit(void) {
    return current_screen == SCREEN_QUIT;
}

/**
 * @brief Checks if the game is currently running.
 * 
 * @return true if the game is active, false otherwise.
 */

bool menu_is_in_game(void) {
    return current_screen == SCREEN_GAME;
}

/**
 * @brief Forces the application to return to the main menu screen.
 */

void menu_return_to_menu(void) {
    current_screen = SCREEN_MENU;
}

