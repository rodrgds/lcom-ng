/**
 * @file ringbuf.c
 * @brief Ring buffer implementation.
 * @ingroup group_collections
 */

#include "ringbuf.h"

#include <string.h>

void ringbuf_init(ringbuf_t *rb, void *storage, uint16_t capacity)
{
  if (rb == NULL || storage == NULL)
    return;

  rb->buf = (uint8_t *)storage;
  rb->capacity = capacity;
  rb->head = 0;
  rb->tail = 0;
  rb->count = 0;
}

int ringbuf_push(ringbuf_t *rb, const void *element, size_t element_size)
{
  if (rb == NULL || element == NULL || rb->count >= rb->capacity)
    return 1;

  memcpy(rb->buf + (rb->tail * element_size), element, element_size);
  rb->tail = (uint16_t)((rb->tail + 1U) % rb->capacity);
  rb->count++;
  return 0;
}

int ringbuf_pop(ringbuf_t *rb, void *element, size_t element_size)
{
  if (rb == NULL || element == NULL || rb->count == 0)
    return 1;

  memcpy(element, rb->buf + (rb->head * element_size), element_size);
  rb->head = (uint16_t)((rb->head + 1U) % rb->capacity);
  rb->count--;
  return 0;
}

int ringbuf_peek(const ringbuf_t *rb, void *element, size_t element_size)
{
  if (rb == NULL || element == NULL || rb->count == 0)
    return 1;

  memcpy(element, rb->buf + (rb->head * element_size), element_size);
  return 0;
}

bool ringbuf_empty(const ringbuf_t *rb)
{
  return rb == NULL || rb->count == 0;
}

bool ringbuf_full(const ringbuf_t *rb)
{
  return rb == NULL || rb->count >= rb->capacity;
}

uint16_t ringbuf_size(const ringbuf_t *rb)
{
  if (rb == NULL)
    return 0;

  return rb->count;
}

void ringbuf_clear(ringbuf_t *rb)
{
  if (rb == NULL)
    return;

  rb->head = 0;
  rb->tail = 0;
  rb->count = 0;
}
