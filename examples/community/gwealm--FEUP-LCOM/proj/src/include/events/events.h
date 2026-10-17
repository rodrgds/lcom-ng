#pragma once

#include <lcom/lcf.h>

enum event_type {
  EXIT,
  INTERRUPT,

  MOUSE_CLICK,
  KEY_PRESS,
  TIMER_TICK,
  SERIAL_PACKET,

  MULTIPLAYER_READY,
  MULTIPLAYER_SUBMISSION
};

struct event {
  enum event_type type;
  void *data;
};

struct event_handler {
  enum event_type type;
  void (*handle)(struct event*);

  bool once;
};

/**
 * @brief Handles operations to inicialize events for usage
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
void (event_init)();

/**
 * @brief Handles operations to terminate events usage
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
void (event_cleanup)();

/**
 * @brief Processes every event in the event queue through all of the handlers for that given event
 */
void (event_tick)();

/**
 * @brief 
 * 
 * @param type The event type
 * @param data The data associated to the event
 * @param data_size Size of the data
 */
void (event_dispatch)(enum event_type type, void *data, size_t data_size);

/**
 * @brief 
 * 
 * @param type The event type
 * @param handle Handler for the event
 */
void (event_on)(enum event_type type, void (*handle)(struct event*));

/**
 * @brief 
 * 
 * @param type The event type
 * @param handle Handler for the event
 */
void (event_once)(enum event_type type, void (*handle)(struct event*));

/**
 * @brief 
 * 
 * @param type The event type
 * @param handle Handler for the event
 */
void (event_unregister)(enum event_type type, void (*handle)(struct event*));
