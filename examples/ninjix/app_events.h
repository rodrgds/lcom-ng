/**
 * @file app_events.h
 * @brief Typed event system -- timer ticks, keyboard scancodes, and mouse packets.
 * @ingroup group_app
 *
 * All hardware interrupts are normalised into app_event_t structs before
 * being dispatched to the active screen handler. This decouples the ISRs
 * from the game logic.
 */

#ifndef PROJ_APP_EVENTS_H
#define PROJ_APP_EVENTS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  APP_EVENT_TIMER,
  APP_EVENT_KEY,
  APP_EVENT_MOUSE
} app_event_kind_t;

typedef struct {
  app_event_kind_t kind;

  union {
    struct {
      uint32_t tick;
    } timer;

    struct {
      bool make;
      uint8_t scancode;
    } key;

    struct {
      int16_t dx;
      int16_t dy;
      bool left_button;
      bool right_button;
      bool middle_button;
      bool x_overflow;
      bool y_overflow;
    } mouse;
  } data;
} app_event_t;

#endif
