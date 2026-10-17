#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "io/keyboard/keyboard.h"
#include "io/mouse/mouse.h"

#define GAME_FREQ 60

typedef enum {
  TIMER_TICK,
  KBD_ENTER,
  KBD_ARROWUP,
  KBD_ARROWDOWN,
  KBD_B_PRESS,
  MOUSE_UP,
  MOUSE_DOWN,
  MOUSE_MOVE
} io_event;

/**
 * @brief Initialize the game controller
 * by starting to listen to I/O interrupts
*/
void init_controller();

/**
 * @brief Subscribe to all I/O interrupts
 * @return non-0 in case of an i/o error
*/
int subscribe_all_int();

/**
 * @brief Unsubscribe to all I/O interrupts
 * @return non-0 in case of an i/o error
*/
int unsubscribe_all_int();

/**
 * @brief Handle a timer tick event
 * @return non-0 in case of a fail
*/
int on_timer_tick();

/**
 * @brief Handle a keyboard event
 * @param sc generated keyboard scancode
 * @return non-0 in case of a fail
*/
int on_kbd_event(scancode *sc);

/**
 * @brief Handle a mouse event
 * @param pkt generated mouse packet
 * @return non-0 in case of a fail
*/
int on_mouse_event(struct packet *pkt);

#endif
