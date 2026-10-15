#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EVENT_NONE,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_BTN,
    EVENT_TICK
} EventType;

typedef struct {
    EventType type;
    union {
        uint8_t key;
        struct {
            int16_t x;
            int16_t y;
            bool lb;
            bool rb;
        } mouse;
    } data;
} GameEvent;

typedef struct EventQueue EventQueue;

/**
 * @brief Creates a fixed-capacity FIFO event queue.
 * @param capacity Maximum number of events. Zero is rejected.
 * @return Queue pointer, or NULL on allocation failure or invalid capacity.
 */
EventQueue *event_queue_create(uint16_t capacity);

/**
 * @brief Destroys an event queue.
 * @param q Queue to destroy. NULL is accepted.
 */
void event_queue_destroy(EventQueue *q);

/**
 * @brief Pushes one event to the queue tail.
 * @param q Queue to modify.
 * @param event Event to copy into the queue.
 * @return 0 on success, 1 on invalid input or full queue.
 */
int event_queue_push(EventQueue *q, GameEvent *event);

/**
 * @brief Pops one event from the queue head.
 * @param q Queue to modify.
 * @param event Output event storage.
 * @return 0 on success, 1 on invalid input or empty queue.
 */
int event_queue_pop(EventQueue *q, GameEvent *event);

/**
 * @brief Checks whether a queue has no events.
 * @param q Queue to inspect. NULL is treated as empty.
 * @return true if empty, false otherwise.
 */
bool event_queue_empty(const EventQueue *q);

/**
 * @brief Checks whether a queue is full.
 * @param q Queue to inspect. NULL is treated as full.
 * @return true if full, false otherwise.
 */
bool event_queue_full(const EventQueue *q);

#endif
