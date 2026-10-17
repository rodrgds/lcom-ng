#include "queue/queue.h"

#include "utils.h"

void *front(queue *q) {
  return q->front != NULL ? q->front->value : NULL;
}

void *back(queue *q) {
  return q->back != NULL ? q->back->value : NULL;
}

unsigned size(queue *q) {
  return q->size;
}

bool is_empty(queue *q) {
  return q->size == 0;
}

bool is_full(queue *q) {
  return q->size == q->capacity;
}

queue *queue_create(size_t capacity, size_t base_size) {
  queue *q;
  q = malloc(sizeof(queue));

  if (q == NULL) {
    printf("queue_create: failed to create queue\n");
    return NULL;
  }

  q->back = NULL;
  q->front = NULL;
  q->size = 0;
  q->capacity = capacity;
  q->base_size = base_size;

  return q;
}

int queue_push(queue *q, void *val) {
  if (is_full(q)) {
    return ERR_MAX_CAPACITY_EXCEEDED;
  }

  queue_node *node = malloc(sizeof(queue_node));

  if (node == NULL) {
    printf("Error: Failed to create the Node\n");
    return 1;
  }

  void *copy;
  util_copy_elem(&copy, val, q->base_size);
  
  node->value = copy;
  node->next = NULL;

  // Inserting the first element in the queue
  if (q->front == NULL) {
    q->back = node;
    q->front = node;
    q->size = 1;

    return 0;
  }

  // Regular insertion
  q->back->next = node;
  q->back = node;
  q->size += 1;
  return 0;
}

void *queue_pop(queue *q) {
  if (is_empty(q))
    return NULL;

  void *val = q->front->value;
  queue_node *tmp = q->front;

  q->front = q->front->next;
  q->size -= 1;

  if (is_empty(q) == 0)
    q->back = NULL;

  free(tmp->value);
  free(tmp);

  return val;
}

void queue_destroy(queue *q) {
  if (q == NULL)
    return;

  while (q->front != NULL) {
    queue_node *tmp = q->front;
    q->front = q->front->next;

    if (tmp->value) {
      free(tmp->value);
    }

    free(tmp);
  }

  if (q->back) {
    free(q->back->value);
    free(q->back);
  }

  free(q);
}
