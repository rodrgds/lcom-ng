#ifndef _EVENT_H_
#define _EVENT_H_ 

#include "event_handler.h"

enum event_type{KBD, MOUSE, TIMER};
typedef enum event_type event_type_t;

struct event {
  event_type_t eventType;
  void* eventData;
  struct packet* eventDataMouse;
};

typedef struct event event_t;

static event_t event;

/**
 * @brief Sends events to necessary handlers
 * Calls event_handler  
 */
void event_dispatcher();

/**
 * @brief Receives events from keyboard
 * Must add params
 */
void kbd_event(uint8_t scancode);

/**
 * @brief Receives events from timer
 * Must add params
 */
void timer_event();

/**
 * @brief Receives events from mouse
 * Must add params
 */
void mouse_event(struct packet* mouse_packet);

#endif

