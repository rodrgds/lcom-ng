/**
 * @file queue.c
 * @brief Implementation of circular queue management.
 *
 * This file contains the implementation of the functions declared in queue.h.
 */
#include "queue.h"

/**
 * @struct queue
 * @brief Structure representing a circular queue.
 *
 * Contains a buffer for storing elements, indices for insertion and removal,
 * the current size, element size, and the number of elements in the queue.
 */
struct queue {
    void *buf;      /**< Pointer to array storing queue elements. */
    int in, out;    /**< Indices for inserting and removing elements. */
    int size;       /**< Size of the queue. */
    int count;      /**< Number of elements currently in the queue. */
    int elem_size;  /**< Size of each element. */
};

//_________________________________________
//PRIVATE FUNCTIONS
//_________________________________________

/**
 * @brief Adjusts queue indices to ensure they remain within valid bounds.
 *
 * This function ensures that the `in` and `out` indices remain within 
 * the size limit of the circular queue by applying modular arithmetic.
 *
 * @param q Pointer to the queue structure.
 */
static void adjust_queue(queue_t *q) {
  q->in %= q->size;
  q->out %= q->size;
}

/**
 * @brief Resizes the queue dynamically to accommodate more elements.
 *
 * This function doubles the queue size, reallocating memory accordingly.
 * If successful, it moves existing elements and updates metadata.
 *
 * @param q Pointer to the queue structure.
 * @return 0 on success, -1 if memory allocation fails.
 */
static int resize_queue(queue_t *q) {
  //Get more space for queue
  int *p = (int *)realloc(q->buf, 2*(q->size)*q->elem_size);
  if( p == NULL ) {
    return -1;
  }

  //Move existing elements
  q->buf = p;
  memcpy((char*)q->buf + q->size * q->elem_size, q->buf, q->in * q->elem_size);

  //Adjust queue data
  q->in += q->size;
  q->size *= 2;
  return 0;
}

//_________________________________________
//PUBLIC FUNCTIONS
//_________________________________________

queue_t *new_queue(unsigned int size, int elem_size) {
  queue_t *q = (queue_t *)malloc(sizeof(queue_t));
  if( q == NULL ) {
    return NULL;
  }

  //Queue cannot have a size minor than 1
  q->size = size ? size : 1;
  q->elem_size = elem_size;

  q->buf = (int *)malloc(q->size * elem_size);
  if (q->buf == NULL) {
    free(q);
    return NULL;
  }

  //Initialize queue
  q->in = 0;
  q->out = 0;
  q->count = 0;
    
  return q;
}

void delete_queue(queue_t *q) {
  if (q != NULL) {
    if (q->buf != NULL) {
      free(q->buf);
      q->buf = NULL;
    }
    free(q);
    q = NULL;
  }
}

bool is_empty_queue(queue_t *q) {
  return q->count == 0;
}

bool is_not_empty_queue(queue_t *q) {
  return q->count != 0;
}

int put_queue(queue_t *q, void *elem) {
  //Check if queue is full
  if (q->count >= q->size) {
    if(resize_queue(q)) {
      return -1;
    }
  }

  //Update queue
  memcpy((char*)q->buf + q->in*q->elem_size, elem, q->elem_size);
  q->count++;
  q->in++;
  adjust_queue(q);
  return 0;
}

int get_queue(queue_t *q, void *elem) {
  if(is_not_empty_queue(q)) {
    memcpy(elem, (char*)q->buf + q->out*q->elem_size, q->elem_size);
    q->count--;
    q->out++;
    adjust_queue(q);
    return 0;
  }
  return -1;
}
