/**
 * @file menu.h 
 * @brief Simple menu system for memory game navigation
 * 
 * Tis header file defines the menu system intrface for navigagation between different 
 * screens in the memory game appication.It provides functionality for menu navigation, screen transitions, and input handling.
 */
#ifndef SIMPLE_MENU_H
#define SIMPLE_MENU_H

#include <stdbool.h>
#include <stdint.h>

#include "../controllers/keyboard_mouse/keyboard.h"

/**
 * @brief Enumeration of available game screens
 * 
 * Defines all possible screens/states that the application can be in.
 * Used to control which content is displayed and how input is handled.
 */
typedef enum { 
    SCREEN_MENU = 0,         /**< Main menu screen - default starting screen */
    SCREEN_INSTRUCTIONS = 1, /**< Instructions/help screen */
    SCREEN_GAME = 2,         /**< Active game screen */
    SCREEN_QUIT = 3           /**< Quit state - signals application should exit */
} GameScreen;

/**
 * @brief Current active screen state
 * 
 * Global variable that tracks which screen is currently active.
 * This determines what content is drawn and how input is processed.
 */
extern GameScreen current_screen;

/**
 * @brief Initialize the menu system
 * 
 * Sets up the initial state of the menu system. Should be called once
 * at application startup before any other menu functions.
 * 
 * @note Sets current_screen to SCREEN_MENU
 */
void menu_init(void);

/**
 * @brief Handle keyboard input for menu navigation
 * 
 * Processes keyboard input and performs appropriate screen transitions
 * based on the current screen and key pressed. Filters out key release
 * events and only processes key press events.
 * 
 * @param keycode The keyboard scancode of the pressed key
 * 
 * @note Key codes handled:
 *       - 0x1C (Enter): Select menu item or confirm action
 *       - 0x01 (ESC): Return to previous screen/menu
 * 
 * @warning Only processes make codes (key press). Break codes (key release)
 *          with bit 7 set (0x80) are ignored.
 */
void menu_handle_input(uint8_t keycode);

/**
 * @brief Draw the current screen
 * 
 * Renders the appropriate screen content based on the current_screen state.
 * Should be called every frame in the main game loop to update the display.
 * 
 * @note Calls different drawing functions based on current_screen:
 *       - SCREEN_MENU: calls desenhar_menu()
 *       - SCREEN_INSTRUCTIONS: calls desenhar_instrucoes()
 * TO DO--      - SCREEN_GAME: calls game drawing function
 *       - SCREEN_QUIT: shows menu one last time before exit
 */
void menu_draw(uint8_t* kbd_irq);

/**
 * @brief Check if the application should quit
 * 
 * Determines whether the main game loop should terminate based on
 * the current screen state.
 * 
 * @return true if the application should exit (user selected quit)
 * @return false if the application should continue running
 * 
 * @note Typically called in the main game loop condition:
 *       while (!menu_should_quit()) { ... }
 */
bool menu_should_quit(void);

/** 
* @brief Check if currently in game mode
* 
* Determines whether the application is currently in the active game state.
* Useful for the main loop to know when to process game logic.
* 
* @return true if currently on the game screen
* @return false if on menu, instructions, or quit screen
* 
* @note Use this in main loop to conditionally run game update logic:
*       if (menu_is_in_game()) { update_game_logic(); }
*/
bool menu_is_in_game(void);

/**
 * @brief Return to main menu from any screen
 * 
 * Forces a transition back to the main menu screen. Can be called
 * programmatically when game ends or from anywhere in the application.
 * 
 * @note Sets current_screen to SCREEN_MENU
 * @note Useful for returning to menu after game completion or error states
 */
void menu_return_to_menu(void);


/**
 * @brief Check if cursor is positioned on the START menu item
 * 
 * Helper function to determine if the cursor is currently positioned
 * over the START menu option based on Y coordinates.
 * 
 * @return true if cursor is on START item (Y: 190-220)
 * @return false otherwise
 * 
 * @note Requires global cursor_y variable to be available
 * @note Y coordinate range should match your menu layout
 */
bool is_cursor_on_start(void);

/**
 * @brief Check if cursor is positioned on the INSTRUCTIONS menu item
 * 
 * Helper function to determine if the cursor is currently positioned
 * over the INSTRUCTIONS menu option based on Y coordinates.
 * 
 * @return true if cursor is on INSTRUCTIONS item (Y: 290-320)
 * @return false otherwise
 * 
 * @note Requires global cursor_y variable to be available
 * @note Y coordinate range should match your menu layout
 */
bool is_cursor_on_instructions(void);

/**
 * Check if the cursor is positioned over the return button in the instructions screen
 * 
 * This function determines whether the current cursor position (cursor_y) falls within
 * the vertical bounds of the return button displayed on the instructions screen.
 * Used to detect when the user can press Enter to return to the main menu.
 * 
 * @return true if the cursor is positioned over the return button, false otherwise
 */
bool is_cursor_on_return_button(void);

/**
 * @brief Check if cursor is positioned on the QUIT menu item
 * 
 * Helper function to determine if the cursor is currently positioned
 * over the QUIT menu option based on Y coordinates.
 * 
 * @return true if cursor is on QUIT item (Y: 390-420)
 * @return false otherwise
 * 
 * @note Requires global cursor_y variable to be available
 * @note Y coordinate range should match your menu layout
 */
bool is_cursor_on_quit(void);

/**
 * @brief Handles ESC key navigation between different screens
 * 
 * Manages screen transitions when ESC key is pressed:
 * - From game/instructions: returns to main menu
 * - From main menu: goes to quit screen
 * - From quit screen: exit
 */
void handle_esc_navigation(void);


#endif
