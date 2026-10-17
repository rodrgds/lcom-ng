#ifndef EV_QUEUE_H
#define EV_QUEUE_H
#include <stdbool.h>
#include <stdlib.h>

/** @defgroup ev_queue ev_queue
 * @{
 *
 * Functions to manage event queues
 */

typedef enum {
    KEY_PRESS = 0,
    MOUSE_CHANGE = 1,
    NO_HP = 2
} ev_type_e;


/** @name event_st */
/**@{
 *
 * event structure
 */
typedef struct{
  ev_type_e eventType;
  unsigned long keyCode;
  int mouseX, mouseY;
  bool lmb, rmb;
} event_st;
/** @} end of event_st*/

/** @name ev_queue_elem_st */
/**@{
 *
 * event queue element structure
 */
typedef struct _ev_queue_elem_st {

  event_st data; /**< @brief queue element data */
  struct _ev_queue_elem_st * nextElem; /**< @brief pointer to the next queue element */

} ev_queue_elem_st;

/** @} end of ev_queue_elem_st*/


/** @name ev_queue_st */
/**@{
 *
 * event queue structure
 */
typedef struct {

  ev_queue_elem_st * begin; /**< @brief pointer to the first queue element */
  ev_queue_elem_st * end; /**< @brief pointer to the last queue element */
  int size;

} ev_queue_st;

/** @} end of ev_queue_st*/

/**
 * @brief Creates an event queue and returns a pointer to it.
 *
 * @return pointer to ev_queue_st on success, NULL otherwise
 */
ev_queue_st * ev_queue_create();
/**
 * @brief Creates an event and returns a pointer to it.
 *
 * @param eventType - type of the event to create
 *
 * @return pointer to event_st on success, NULL otherwise
 */
event_st* createEvent(ev_type_e eventType);

/**
 * @brief Frees an event queue releasing the memory.
 *
 * @param ev_queue - queue to release
 *
 * @return None
 */
void ev_queue_free(ev_queue_st * ev_queue);

/**
 * @brief Get the first element of the given queue.
 *
 * @param ev_queue - queue to use
 * @return pointer to ev_queue_st
 */
ev_queue_elem_st * ev_queue_begin(ev_queue_st* ev_queue);

/**
 * @brief Get the last element of the given queue.
 *
 * @param ev_queue - queue to use
 * @return pointer to ev_queue_st
 */
ev_queue_elem_st * ev_queue_end(ev_queue_st* ev_queue);

/**
 * @brief push a given event_st to the end of the given queue.
 *
 * @param ev_queue - queue to push
 * @param newElemData - element to push
 *
 * @return true on success, false otherwise
 */
bool ev_queue_push(ev_queue_st* ev_queue, event_st newElemData);

/**
 * @brief pop the first element of the given queue.
 *
 * @param ev_queue - queue to pop
 *
 * @return popped element pointer
 */
ev_queue_elem_st * ev_queue_pop(ev_queue_st* ev_queue);

/** @} end of ev_queue */
#endif
