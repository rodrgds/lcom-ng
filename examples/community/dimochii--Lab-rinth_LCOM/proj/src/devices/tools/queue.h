#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_
#include <lcom/lcf.h>

typedef struct {
  uint8_t front, last, size; 
  unsigned capacity; 
  uint8_t * values; 

} Queue;

Queue* (newQueue)(unsigned capacity);

bool (queueIsFull)(Queue* queue);

bool (queueIsEmpty)(Queue* queue);

int (sizeQueue)(Queue * queue);

bool (enqueue)(Queue* queue, uint8_t element);

uint8_t (dequeue)(Queue* queue);

uint8_t (frontQueue)(Queue* queue);

uint8_t (backQueue)(Queue* queue);

void (clearQueue)(Queue* queue);

#endif

