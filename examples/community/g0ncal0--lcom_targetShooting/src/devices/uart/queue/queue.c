#include "queue.h"

queue_t* create_queue(int size, int el_size) {
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    if (q == NULL) {
        printf("Error allocating memory for queue structure inside %s\n", __func__);
        return NULL;
    }

    if (queue_init(q, size, el_size) != EXIT_SUCCESS) {
        free(q);
        return NULL;
    }

    return q;
}

int (queue_init)(queue_t *q, int size, int el_size) {
  if (q == NULL || size <= 0 || el_size <= 0) {
    printf("Invalid arguments inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  q->buf = malloc(size * el_size);
  if (q->buf == NULL) {
    printf("Error allocating memory inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  q->in = 0;
  q->out = 0;
  q->size = size;
  q->count = 0;
  q->el_size = el_size;

  return EXIT_SUCCESS;
}

int (queue_destroy)(queue_t *q) {
  if (q == NULL) {
    printf("Invalid arguments inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  free(q->buf);
  q->buf = NULL;
  q->size = 0;
  q->count = 0;
  q->el_size = 0;
  
  return EXIT_SUCCESS;
}

bool (queue_is_empty)(queue_t *q) {
  if (q == NULL) {
    return 0;
  }

  return (q->count == 0);
}

bool (queue_is_full)(queue_t *q) {
  if (q == NULL) {
    return 0;
  }

  return (q->count == q->size);
}

int (queue_enqueue)(queue_t *q, void *elem) {
  if (q == NULL || elem == NULL) {
    printf("Invalid arguments inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (queue_is_full(q)) {
    printf("Queue is full inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  memcpy((char *)q->buf + q->in * q->el_size, elem, q->el_size);
  q->in = (q->in + 1) % q->size;
  q->count++;
  return EXIT_SUCCESS;
}

int (queue_dequeue)(queue_t *q, void *elem) {
  if (q == NULL || elem == NULL) {
    printf("Invalid arguments inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (queue_is_empty(q)) {
    printf("Queue is empty inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  memcpy(elem, (char *)q->buf + q->out * q->el_size, q->el_size);
  q->out = (q->out + 1) % q->size;
  q->count--;
  return EXIT_SUCCESS;
}

int (queue_front)(queue_t *q, void *elem) {
  if (q == NULL || elem == NULL) {
    printf("Invalid arguments inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (queue_is_empty(q)) {
    printf("Queue is empty inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  memcpy(elem, (char *)q->buf + q->out * q->el_size, q->el_size);

  return EXIT_SUCCESS;
}
