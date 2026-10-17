#pragma once
#include <lcom/lcf.h>
#include <math.h>

#include "fruit.h"


/**
 * @brief Mouse state types.
 * 
 * Used as states for the mouse cutting gesture state machine.
 */
enum mouse_state_t {
  INIT,
  START_CUT,
  CUTTING,
  COMP
};


/**
 * @brief Mouse event types.
 * 
 * Used as events for the mouse cutting gesture event handler.
 */
enum mouse_event_t {
  LB_PRESS,
  LB_RELEASE,
  CUT_MOV,
  DEFAULT_EV
};


/**
 * @brief Mouse event struct.
 * 
 * Stores the mouse current position and the event type for updating
 * the state of the mouse cutting gesture state machine.
 */
typedef struct {
  enum mouse_event_t type;
  uint16_t x_pos, y_pos;
} mouse_event;


/**
 * @brief Event handler for mouse cutting gesture.
 * 
 * Changes the state of the mouse cutting gesture state according to the parameters recieved.
 * 
 * @param event the mouse event that was last detected.
 * @param slope the slope of the drawn line
 * @param x_i the initial horizontal position of the drawn line.
 * @param y_i the initial vertical position of the drawn line.
 * @param x_f the final horizontal position of the drawn line.
 * @param y_f the final vertical position of the drawn line.
 */
void check_cutting(mouse_event event, double* slope, uint16_t* x_i, uint16_t* y_i, uint16_t* x_f, uint16_t* y_f);


/**
 * @brief Event detector for mouse cutting gesture.
 * 
 * Detects what mouse event happened in the recieved packet.
 * 
 * @param pp a pointer to the mouse interrupt packet from the mouse interruption.
 * @param mouse_pointer a pointr to the mouse pointer sprite for positional information.
 * @param slope a pointer to the double where to store the drawn line slope.
 * @param tolerance an arbitrary tolerance for line drawing.
 */
mouse_event detect_mouse_event(struct packet* pp, Fruit* mouse_pointer, double* slope, uint16_t tolerance);
