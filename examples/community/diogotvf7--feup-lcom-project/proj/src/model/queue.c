#include "queue.h"

Queue *pos_queue = NULL;

int queue_size(Queue **queue) {
  if (*queue == NULL) return 0;
  return (*queue)->size;
}

int queue_empty(Queue **queue) {
  return *queue == NULL;
} 

void *queue_front(Queue **queue) {
  if (*queue == NULL) return NULL;
  return (*queue)->front->data;
}

void queue_push(Queue **queue, void *data, size_t size) {  
  Node *n = (Node *)malloc(sizeof(Node));
  n->data = malloc(size);
  memcpy(n->data, data, size);
  n->prev = NULL;
  if (*queue == NULL) {
    *queue = (Queue *)malloc(sizeof(Queue));
    (*queue)->size = 1; 
    (*queue)->back = n;
    (*queue)->front = n;
  }
  else {
    (*queue)->back->prev = n;
    (*queue)->back = n;
    (*queue)->size++;
  }
}

void queue_pop(Queue **queue) {
  if (*queue == NULL) return;
  if ((*queue)->size == 1) {
    *queue = NULL;
    return;
  }
  Node *old_front = (*queue)->front;
  (*queue)->front = (*queue)->front->prev;
  free(old_front->data);  
  free(old_front); 
  (*queue)->size--;
}

void queue_clear(Queue **queue) {
  while (*queue != NULL) {
    queue_pop(queue);
  }
}


