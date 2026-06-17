/**
 * @file ringbuf.h
 * @brief Generic circular buffer (ring buffer) for fixed-size elements.
 * @ingroup group_collections
 *
 * Stores arbitrary fixed-size elements in a statically allocated circular
 * buffer. No heap allocations after init; all storage is preallocated.
 * Useful for event queues, UART I/O buffering, and producer-consumer
 * patterns.
 */

#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *buf;
  uint16_t capacity;
  uint16_t head;
  uint16_t tail;
  uint16_t count;
} ringbuf_t;

/**
 * @brief Initialise a ring buffer backed by caller-provided storage.
 * @param rb        Ring buffer to initialise.
 * @param storage   Preallocated byte array (capacity * element_size bytes).
 * @param capacity  Maximum number of elements.
 */
void ringbuf_init(ringbuf_t *rb, void *storage, uint16_t capacity);

/**
 * @brief Push an element onto the back of the buffer.
 * @param rb            Ring buffer.
 * @param element       Pointer to the element to copy in.
 * @param element_size  Size of a single element in bytes.
 * @return 0 on success, 1 if the buffer is full.
 */
int ringbuf_push(ringbuf_t *rb, const void *element, size_t element_size);

/**
 * @brief Pop an element from the front of the buffer.
 * @param rb            Ring buffer.
 * @param element       Pointer to destination where the element will be copied.
 * @param element_size  Size of a single element in bytes.
 * @return 0 on success, 1 if the buffer is empty.
 */
int ringbuf_pop(ringbuf_t *rb, void *element, size_t element_size);

/**
 * @brief Peek at the front element without removing it.
 * @param rb            Ring buffer.
 * @param element       Pointer to destination where the element will be copied.
 * @param element_size  Size of a single element in bytes.
 * @return 0 on success, 1 if the buffer is empty.
 */
int ringbuf_peek(const ringbuf_t *rb, void *element, size_t element_size);

/**
 * @brief Check whether the buffer is empty.
 * @param rb  Ring buffer.
 * @return true if empty.
 */
bool ringbuf_empty(const ringbuf_t *rb);

/**
 * @brief Check whether the buffer is full.
 * @param rb  Ring buffer.
 * @return true if full.
 */
bool ringbuf_full(const ringbuf_t *rb);

/**
 * @brief Return the number of elements currently stored.
 * @param rb  Ring buffer.
 * @return Count of enqueued elements.
 */
uint16_t ringbuf_size(const ringbuf_t *rb);

/**
 * @brief Remove all elements from the buffer.
 * @param rb  Ring buffer.
 */
void ringbuf_clear(ringbuf_t *rb);

#endif
