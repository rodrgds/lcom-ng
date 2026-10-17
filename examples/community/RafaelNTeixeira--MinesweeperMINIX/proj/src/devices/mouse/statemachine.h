#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Enumeration of states used in the drawing handler.
 * - INIT: Initial state of the drawing operation. It indicates that no drawing operation is in progress and the system is waiting for the left button to be pressed.
 * - DRAWUP: State indicating that the left button is pressed and the mouse is moving upwards to perform the drawing operation. In this state, the system accumulates the movement and checks for the release of the left button or other transition conditions.
 * - HALF: State indicating that the drawing operation has met the minimum length requirement and is transitioning to a different direction. It could either transition to a downward movement or return to the initial state. This state is triggered by specific conditions based on the accumulated movement and mouse button events.
 * - DRAWDOWN: State indicating that the drawing operation is moving downwards. Similar to the DRAWUP state, it accumulates the movement and checks for the release of the right button or other transition conditions.
*/
typedef enum {INIT, DRAWUP, HALF, DRAWDOWN} States;

/**
 * @brief Generates a mouse event based on the provided mouse packet
 * @param pack pack Pointer to the mouse packet containing movement and button information
 * @return Pointer to the generated mouse event structure
*/
struct mouse_ev * mouse_events(struct packet * pack);

/**
 * @brief Handles mouse events for drawing operations
 * @param xlen Minimum length required for a valid drawing operation
 * @param tolerance Maximum movement tolerance allowed for drawing operation
 * @param pack Pointer to the mouse packet containing movement and button information
 * @return True if a valid drawing operation is detected, false otherwise
*/
bool (draw_handler)(uint8_t xlen, uint8_t tolerance, struct packet * pack);
