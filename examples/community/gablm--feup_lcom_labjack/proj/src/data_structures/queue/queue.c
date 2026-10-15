#include "queue.h"

queue_t *queue_create(size_t size) {
  queue_t *queue;

  queue = malloc(sizeof(queue_t));
  if (queue == NULL)
    return NULL;

  queue->content = malloc(size * sizeof(void *));
  if (queue->content == NULL) {
    free(queue);
    return NULL;
  }

  queue->base_size = size;
  queue->curr_size = 0;
  queue->front_pos = 0;
  queue->end_pos = 0;

  return queue;
}

size_t queue_size(queue_t *queue) {
  if (queue == NULL)
    return 0;

  return queue->curr_size;
}

int queue_full(queue_t *queue) {
  if (queue == NULL)
    return 0;

  return queue->base_size == queue->curr_size;
}

int queue_empty(queue_t *queue) {
  if (queue == NULL)
    return 0;

  return queue->curr_size == 0;
}

void queue_destroy(queue_t **queue, void (*f)(void *)) {
  if (queue == NULL || *queue == NULL || f == NULL)
    return;

  if ((*queue)->front_pos < (*queue)->end_pos) {
    for (size_t i = (*queue)->front_pos; i <= (*queue)->end_pos; i++)
      f((*queue)->content[i]);
  }
  else if ((*queue)->curr_size != 0) {
    for (size_t i = 0; i <= (*queue)->end_pos; i++)
      f((*queue)->content[i]);

    for (size_t i = (*queue)->front_pos; i < (*queue)->base_size; i++)
      f((*queue)->content[i]);
  }
  free(*queue);
  *queue = NULL;
}

void *queue_at(queue_t *queue, size_t pos) {
  size_t f_pos;

  if (pos >= queue->curr_size)
    return NULL;

  f_pos = queue->front_pos + pos;
  if (queue->front_pos < queue->end_pos)
    return queue->content[f_pos];

  long excedent = queue->front_pos + pos - queue->base_size;
  return excedent >= 0 ? queue->content[excedent] : queue->content[f_pos];
}

void **queue_at_ref(queue_t *queue, size_t pos) {
  size_t f_pos;

  if (pos >= queue->curr_size)
    return NULL;

  f_pos = queue->front_pos + pos;
  if (queue->front_pos < queue->end_pos)
    return queue->content + f_pos;

  long excedent = queue->front_pos + pos - queue->base_size;
  return excedent >= 0 ? queue->content + excedent : queue->content + f_pos;
}

void *queue_pop(queue_t *queue) {
  if (queue == NULL)
    return NULL;

  if (queue_empty(queue))
    return NULL;

  void *result = queue->content[queue->front_pos];

  queue->curr_size -= 1;
  if (queue->front_pos == queue->base_size - 1)
    queue->front_pos = 0;
  else if (queue->curr_size == 0)
    queue->front_pos = queue->end_pos;
  else
    queue->front_pos = queue->front_pos + 1;

  return result;
}

int queue_push(queue_t *queue, void *content) {
  if (queue_full(queue) || content == NULL)
    return 1;

  if (queue_empty(queue)) {
    queue->curr_size = 1;
    queue->content[queue->front_pos] = content;
    return 0;
  }

  queue->curr_size += 1;
  queue->end_pos = queue->end_pos == queue->base_size - 1 ? 0 : queue->end_pos + 1;
  queue->content[queue->end_pos] = content;
  return 0;
}

void queue_shuffle(queue_t *queue) {
  if (queue == NULL)
    return;

  for (size_t i = queue->curr_size - 1; i > 0; i--) {
    int k = rand() % (i + 1);

    void *tmp = queue_at(queue, k);
    *queue_at_ref(queue, k) = queue_at(queue, i);
    *queue_at_ref(queue, i) = tmp;
  }
}

void queue_destroy_nothing(void *ptr) {
  (void) ptr;
}
