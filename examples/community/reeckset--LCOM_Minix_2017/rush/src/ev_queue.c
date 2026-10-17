#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "ev_queue.h"

//Creates an event queue and returns the pointer to it. If the pointer is NULL, the memory allocation has failed
// Starts with no elements, so the begin and end are null pointers
ev_queue_st * ev_queue_create() {
  ev_queue_st * ev_queue = malloc(sizeof(ev_queue_st));
  if(ev_queue == NULL) return NULL; // MEMORY ALLOCATION FAILED
  ev_queue->begin = NULL;
  ev_queue->end = NULL;
  ev_queue->size = 0;
  return ev_queue;
}

//Frees the memory occupied by the ev_queue and its elements
void ev_queue_free(ev_queue_st * ev_queue) {

  //Popping elements until the begin element is a null pointer, which means there are no more elements
  while(ev_queue_begin(ev_queue) != NULL) {
    ev_queue_pop(ev_queue);
  }

  free(ev_queue);
}

ev_queue_elem_st * ev_queue_begin(ev_queue_st* ev_queue) {
  return ev_queue->begin;
}

ev_queue_elem_st * ev_queue_end(ev_queue_st* ev_queue) {
  return ev_queue->end;
}

event_st* createEvent(ev_type_e eventType) {
  event_st* newEvent = malloc(sizeof(event_st));
  newEvent->eventType = eventType;

  return newEvent;
}

bool ev_queue_push(ev_queue_st* ev_queue, event_st newElemData) {
  ev_queue_elem_st * newElem = malloc(sizeof(ev_queue_elem_st));
  if(newElem == NULL) return false; //THE ALLOCATION OF NEW ELEMENT FAILED
  newElem->data = newElemData;
  newElem->nextElem = NULL;

  if(ev_queue_begin(ev_queue) == NULL){
    ev_queue->begin = newElem; // If the queue is empty, the new elem will be the begin of the list
    ev_queue->end = newElem;
  } else { // Otherwise, only the end is changed
    ev_queue->end->nextElem = newElem;
    ev_queue->end = newElem;
  }

  /*unsigned long newElementCode = newElemData.keyCode;
  if(newElemData.eventType == KEY_PRESS) printf("Pushed kbd event with code: %d\n", newElementCode);
  else if(newElemData.eventType == MOUSE_CHANGE) printf("Pushed mouse event\n");
  else printf("Pushed another event\n");*/

  ev_queue->size += 1;

  return true;
}

//Frees the memory occupied by the first element of the ev_queue, and re-assigns the begin to the next element
// If the ev_queue_begin is a NULL pointer, the function returns NULL, otherwise it returns the next element in the queue
ev_queue_elem_st * ev_queue_pop(ev_queue_st* ev_queue) {

  ev_queue_elem_st * oldBegin = ev_queue_begin(ev_queue);
  if(oldBegin == NULL) return NULL;
  if(ev_queue_begin(ev_queue) == ev_queue_end(ev_queue)){
    ev_queue->end = NULL;
    ev_queue->begin = NULL;
    ev_queue->size = 0;
    free(oldBegin);
    return NULL;
  }

  ev_queue_elem_st * newBegin = oldBegin->nextElem;

  ev_queue->begin = newBegin;


  /*unsigned long oldElementCode = oldBegin->data.keyCode;
  if(oldBegin->data.eventType == KEY_PRESS) printf("Popped kbd event with code: %d\n", oldElementCode);
  else if(oldBegin->data.eventType == MOUSE_CHANGE) printf("Popped mouse event\n");
  else printf("Popped another event\n");*/

  ev_queue->size -= 1;
  free(oldBegin);
  return newBegin;

}
