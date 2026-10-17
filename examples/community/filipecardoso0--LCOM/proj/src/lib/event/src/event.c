#include "../include/event.h"

void event_dispatcher(){
  switch(event.eventType){
    case KBD: kbd_handler(event.eventData); break;
    case TIMER: /*timer_handler();*/ break;
    case MOUSE: mouse_handler(event.eventDataMouse); break;
    default: break;
  }
}

void kbd_event(uint8_t scancode){
  event.eventType = KBD;
  event.eventData = &scancode;
  event_dispatcher();
}

void timer_event(){
  event.eventType = TIMER; 
  event_dispatcher(); 
}

void mouse_event(struct packet* mouse_packet) {
  event.eventType = MOUSE;
  event.eventDataMouse = mouse_packet; 
  event_dispatcher();
}
