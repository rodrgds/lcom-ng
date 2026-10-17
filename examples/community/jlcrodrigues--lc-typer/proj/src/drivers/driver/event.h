#ifndef EVENT_H
#define EVENT_H

/** @defgroup driver event
 * @{
 *
 * Defines the types of event that can occur.
 */

typedef enum {KEYBOARD, MOUSE, TIMER, BLANK} EventType;

/**
 * @brief Store information about an interrupt.
 * 
 */
typedef struct Event {
  /**
   * @brief The type of event.
   * 
   */
  EventType type;
  /**
   * @brief Info depends on the type of event. Each driver will
   * have a custom field under info. Check the type before trying
   * to access the info.
   * 
   */
  union {
    struct timer { 
      uint32_t count_interrupts;
    } timer;
    struct keyboard { 
      uint8_t size;
      uint8_t buff;
      char character;
    } keyboard;
    struct mouse {
      int16_t x_delta;
      int16_t y_delta;
      int clicked;
    } mouse;
  } info;
} Event;

/**@}*/

#endif
