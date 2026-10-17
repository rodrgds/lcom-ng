#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "queue.h"

/* ======================= */
/* Private Queue Functions */
/* ======================= */

/**
 * @brief Adjusts the in and out members of the queue to the size of the array, in order to avoid
 *        accessing inexistent array elements
 * 
 * @param q   Queue pointer that points to the queue that needs to be adjusted
 */
static void adjust_queue(queue *q);

/**
 * @brief Resizes a given queue, doubling its previous size
 *  
 * @param q     Queue pointer that points to the queue that needs to be rezised
 * 
 * @return 0    The queue was rezised successfully
 * @return -1   It was not possible to allocate the memory needed to be allocated in order to
 *              double the size of the queue 
 */
static int resize_queue(queue *q);


queue *new_queue(unsigned int initial_size) {

  /* allocate memory for the queue object */
  queue *q = malloc(sizeof(*q));

  /* if there wasn't enough memory available */
  if ( q == NULL) {
    return NULL;
  }

  /* allocate space to store queue elements */
  q->size = initial_size ? initial_size : 1;
  q->array = malloc( q->size * sizeof(pair) );

  /* if there wasn't enough memory available */
  if ( q->array == NULL ) {
    free(q);
    return NULL;
  }

  /* initial state of queue */
  q->in = q->out = q->counter = 0;

  return q;
}


void delete_queue(queue *q) {
  free(q->array);
  free(q);
}


int push(queue *q, pair n) {

  /* if the queue is full */
  if ( q->counter == q->size ) {
    if ( resize_queue(q) )
      return -1;
  }

  q->array[q->in++] = n;
  q->counter++;

  adjust_queue(q);

  return 0;
}


int pop(queue *q, pair *n) {

  /* check if the queue is not empty */
  if ( q->counter != 0 ) {
    *n = q->array[q->out++];
    q->counter--;

    adjust_queue(q);

    return 0;
  }

  return -1;
}


bool is_empty(queue *q) {
  return q->counter == 0;
}


int num_elements(queue *q) {
  return q->counter;
}


static void adjust_queue(queue *q) {
  q->in = q->in % q->size;
  q->out = q->out % q->size;
}


static int resize_queue(queue *q) {

  /* allocate memory for an array of double the size of the previous one */
  pair *ptr = (pair*) realloc(q->array, 2 * q->size * sizeof(pair));

  /* if there wasn't enough memory available */
  if ( ptr == NULL ) {
    return -1;
  }

  q->array = ptr;
  
  int i;
  for ( i = 0; i < q->in; i++) {
    q->array[q->size + i] = q->array[i];
  }

  q->in = q->in + q->size;
  q->size = q->size * 2;

  return 0;
}
