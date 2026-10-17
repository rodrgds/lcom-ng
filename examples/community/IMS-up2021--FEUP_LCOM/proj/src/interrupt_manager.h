#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include <lcom/lcf.h>
#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "graphic.h"

extern uint32_t timer_irq_set;
extern uint32_t kbd_irq_set;
extern uint32_t mouse_irq_set;
extern void *frame_buffer;
extern void *second_buffer;
extern uint16_t center_x;
extern uint16_t center_y;
extern uint64_t delta_time;
/**
 * @brief Initializes the game or application.
 *
 * This function sets up necessary resources and initializes the game or application.
 *
 * @return Returns 0 upon successful initialization, or a non-zero error code otherwise.
 */
int (init)();

/**
 * @brief The main loop of the game or application.
 *
 * This function contains the main loop that runs the game or application's logic, 
 * handling events and updating the state accordingly.
 *
 * @return Returns 0 upon normal termination, or a non-zero error code otherwise.
 */
int (main_loop)();

/**
 * @brief Cleans up and exits the game or application.
 *
 * This function releases resources and performs necessary cleanup before the game 
 * or application exits.
 *
 * @return Returns 0 upon successful cleanup and exit, or a non-zero error code otherwise.
 */
int (leave)();

/**
 * @brief Loads the XPM images used in the game or application.
 *
 * This function loads and initializes the XPM images required for the game's or 
 * application's graphics.
 */
void (load_xpms)();

/**
 * @brief Handles timer events.
 *
 * This function processes events related to the system timer, typically used for 
 * updating the game or application's state at regular intervals.
 */
void (handle_timer_event)();

/**
 * @brief Handles keyboard events.
 *
 * This function processes keyboard input events, updating the game or application's 
 * state based on the provided scancode.
 *
 * @param scancode An array of bytes representing the keyboard scancode.
 */
void (handle_kbd_event)(uint8_t scancode[]);


/**
 * @brief Handles mouse events.
 * 
 * @param pp The packet with the mouse event data.
 */
void (handle_mouse_event)(struct packet pp);

/**
 * @brief Frees the array of scores.
 * 
 * @param scores The pointer to the array of scores.
 */
void free_array(int **scores);

#endif // INTERRUPT_MANAGER_H
