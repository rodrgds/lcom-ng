#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>

typedef enum {
    
    INIT,
    UP_DRAW,
    VERTEX,
    DOWN_DRAW,
    FINISH,
    EXIT

} States;

typedef enum {
    
    OTHER,
    LB_PRESS, 
    LB_RELEASE, 
    RB_PRESS, 
    RB_RELEASE, 
    MOVE,

} type;

typedef struct {
    
    type type;
    int16_t x_delta;
    int16_t y_delta;

} Event;

Event ev;

/**
 * @brief Parses a packet and verifies event type
 * 
 * @param pp pointer to packet
 * @param event pointer to event destination
 * 
 * @return Return 0 if success and 1 otherwise
 */
int get_event(struct packet *pp, Event *event);

#endif /* _EVENT_HANDLER_H */

