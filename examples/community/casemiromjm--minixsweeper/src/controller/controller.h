/**
 * @file controller.h
 * @brief Main controller header for handling device initialization, game loops, and input handlers in the Minesweeper game.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <lcom/lcf.h>
#include "kbc.h"
#include "mouse.h"
#include "../view/view.h"
#include "../sprites/sprite.h"
#include "../model/map.h"
#include "../view/graphics.h"
#include <minix/syslib.h>

/**
 * @brief Initializes all devices required for the game
 * 
 * Sets up graphics mode, subscribes to mouse and timer interrupts,
 * initializes sprites and configures timer frequency.
 * 
 * @param mouse_irq Pointer to store mouse IRQ line
 * @param timer_irq Pointer to store timer IRQ line
 * @param irq_set_mouse Pointer to store mouse IRQ set
 * @param irq_set_timer Pointer to store timer IRQ set
 * @return 0 on success, 1 on failure
 */
int init_devices(uint8_t *mouse_irq, uint8_t *timer_irq, int *irq_set_mouse, int *irq_set_timer);

/**
 * @brief Cleans up all initialized devices
 * 
 * Disables mouse data reporting, unsubscribes from interrupts,
 * frees allocated memory and returns to text mode.
 */
void (cleanup_devices)();

/**
 * @brief Main menu loop handling user interaction
 * 
 * Manages the menu state, processes mouse and timer interrupts,
 * and handles menu navigation and game start.
 * 
 * @return 0 to start game, -1 to quit game
 */
int (menu_loop)();

/**
 * @brief Main game loop handling gameplay
 * 
 * Manages the game state, processes mouse and timer interrupts,
 * and handles game logic and win/lose conditions.
 * 
 * @param difficulty The selected game difficulty
 * @return Game result: -1 running, 0 lose, 1 win, -2 return to menu
 */
int (game_loop)(GameDifficulty difficulty);

/**
 * @brief Game over screen loop
 * 
 * Displays the game over screen and waits for user input
 * to return to the main menu.
 * 
 * @return 0 to return to menu
 */
int (game_over_loop)();

/**
 * @brief Game victory screen loop
 * 
 * Displays the victory screen with elapsed time and waits
 * for user input to return to the main menu.
 * 
 * @return 0 to return to menu
 */
int (game_win_loop)();

/**
 * @brief Handles mouse interrupts during menu state
 * 
 * Processes mouse movement and clicks in the menu, updates cursor position,
 * handles button hover effects and click events.
 * 
 * @param x Pointer to mouse X coordinate
 * @param y Pointer to mouse Y coordinate
 * @param running Pointer to running state flag
 * @return 1 if game should start, 0 otherwise
 */
int (handle_mouse_interrupt_menu)(int *x, int *y, bool *running);

/**
 * @brief Handles timer interrupts during menu state
 * 
 * Redraws the menu screen with current cursor position at regular intervals.
 * 
 * @param x Current mouse X coordinate
 * @param y Current mouse Y coordinate
 */
void (handle_timer_interrupt_menu)(int x, int y);

/**
 * @brief Handles mouse interrupts during game state
 * 
 * Processes mouse movement and clicks during gameplay, handles cell reveals,
 * flagging, and special interactions like lamp usage and exit button.
 * 
 * @param x Pointer to mouse X coordinate
 * @param y Pointer to mouse Y coordinate
 * @param running Pointer to running state flag
 * @param win Pointer to win state variable
 * @return 0 on normal execution
 */
int (handle_mouse_interrupt_game)(int *x, int *y, bool *running, int *win);

/**
 * @brief Handles timer interrupts during game state
 * 
 * Updates game timer, redraws the game screen with current cursor position,
 * and manages frame counting for time updates.
 * 
 * @param x Current mouse X coordinate
 * @param y Current mouse Y coordinate
 */
void (handle_timer_interrupt_game)(int x, int y);

/**
 * @brief Handles mouse interrupts during game over/win states
 * 
 * Processes mouse input to return to menu from game over or victory screens.
 * Any mouse button click will trigger return to menu.
 * 
 * @param x Pointer to mouse X coordinate
 * @param y Pointer to mouse Y coordinate
 * @param running Pointer to running state flag
 * @return 0 on normal execution
 */
int (handle_mouse_interrupt_game_over_win)(int *x, int *y, bool *running);

/**
 * @brief Handles timer interrupts during game over state
 * 
 * Redraws the game over screen with current cursor position.
 * 
 * @param x Current mouse X coordinate
 * @param y Current mouse Y coordinate
 */
void (handle_timer_interrupt_game_over)(int x, int y);

/**
 * @brief Handles timer interrupts during game win state
 * 
 * Redraws the victory screen with elapsed time and current cursor position.
 * 
 * @param x Current mouse X coordinate
 * @param y Current mouse Y coordinate
 */
void (handle_timer_interrupt_game_win)(int x, int y);

/**
 * @brief Handles mouse hover effects in the menu
 * 
 * Detects when the mouse cursor is over menu buttons and updates
 * their hover state for visual feedback.
 * 
 * @param menu Pointer to the menu structure
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 */
void (handle_mouse_hover)(Menu* menu, int mouse_x, int mouse_y);

/**
 * @brief Handles left mouse clicks in the menu
 * 
 * Processes clicks on menu buttons including start, mode selection arrows,
 * difficulty selection arrows, and exit button. Updates menu state accordingly.
 * 
 * @param menu Pointer to the menu structure
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 * @param running Pointer to running state flag
 * @return 1 if game should start, 0 otherwise
 */
int (handle_mouse_left_click_menu)(Menu* menu, int mouse_x, int mouse_y, bool *running);

/**
 * @brief Handles left mouse clicks during gameplay
 * 
 * Processes clicks on the game map, lamp usage, and handles cell revelation.
 * Manages first click logic, deceiver mode, bomb explosions, and win conditions.
 * 
 * @param map Pointer to the game map structure
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 * @param win Pointer to win state variable
 */
void (handle_mouse_left_click_game)(Map* map, int mouse_x, int mouse_y, int *win);

/**
 * @brief Handles right mouse clicks during gameplay
 * 
 * Processes right clicks to flag/unflag cells on the game map
 * and updates the bomb counter display.
 * 
 * @param map Pointer to the game map structure
 * @param mouse_x Mouse X coordinate
 * @param mouse_y Mouse Y coordinate
 */
void (handle_mouse_right_click_game)(Map* map, int mouse_x, int mouse_y);

#endif

