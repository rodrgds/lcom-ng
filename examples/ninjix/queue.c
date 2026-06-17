/**
 * @file queue.c
 * @brief Linked-list FIFO queue implementation.
 * @ingroup group_collections
 *
 * A generic queue storing void* payloads. Used by the wave controller
 * to schedule spawn entries in order.
 */

#include "queue.h"

#include <stdlib.h>

typedef struct queue_node {
  void *data;
  struct queue_node *next;
} queue_node_t;

struct queue {
  queue_node_t *front;
  queue_node_t *rear;
  size_t size;
};

queue_t *queue_init(void)
{
  queue_t *q = malloc(sizeof(queue_t));
  if (q == NULL)
    return NULL;

  q->front = NULL;
  q->rear = NULL;
  q->size = 0;

  return q;
}

void queue_destroy(queue_t *q)
{
  if (q == NULL)
    return;

  while (!queue_empty(q))
    queue_dequeue(q);

  free(q);
}

int queue_enqueue(queue_t *q, void *data)
{
  if (q == NULL)
    return 1;

  queue_node_t *node = malloc(sizeof(queue_node_t));
  if (node == NULL)
    return 1;

  node->data = data;
  node->next = NULL;

  if (q->rear == NULL) {
    q->front = node;
    q->rear = node;
  } else {
    q->rear->next = node;
    q->rear = node;
  }

  q->size++;
  return 0;
}

void *queue_dequeue(queue_t *q)
{
  if (q == NULL || q->front == NULL)
    return NULL;

  queue_node_t *node = q->front;
  void *data = node->data;

  q->front = node->next;
  if (q->front == NULL)
    q->rear = NULL;

  free(node);
  q->size--;

  return data;
}

void *queue_front(queue_t *q)
{
  if (q == NULL || q->front == NULL)
    return NULL;

  return q->front->data;
}

bool queue_empty(queue_t *q)
{
  return q == NULL || q->front == NULL;
}

size_t queue_size(queue_t *q)
{
  if (q == NULL)
    return 0;

  return q->size;
}
