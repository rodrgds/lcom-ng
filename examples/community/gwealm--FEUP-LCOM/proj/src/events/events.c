#include "events/events.h"
#include "vector/vector.h"
#include "utils.h"

static vector *handlers;
static vector *queue;

void (event_init)() {
  handlers = vec_create(sizeof(struct event_handler));
  queue = vec_create(sizeof(struct event));
}

void (event_cleanup)() {
  vec_destroy(handlers);
  vec_destroy(queue);
}

void (event_tick)() {
  while (queue->len > 0) {
    struct event *first = vec_at(queue, 0);
    
    for (size_t i = 0; i < handlers->len;) {
      struct event_handler *handler = vec_at(handlers, i);
      if (handler->type == first->type) {
        handler->handle(first);

        if (handler->once) {
          vec_erase(handlers, i);
          continue;
        }
      }

      i++;
    }

    if (first->data != NULL) {
      free(first->data);
    }
  
    vec_erase(queue, 0);
  }
}

void (event_dispatch)(enum event_type type, void *data, size_t data_size) {
  void *copy = NULL;
  if (data != NULL) {
    util_copy_elem(&copy, data, data_size);
  }

  struct event ev = { type, copy };
  vec_push(queue, &ev);
}

void (event_on)(enum event_type type, void (*handle)(struct event*)) {
  struct event_handler handler = { type, handle, false };
  vec_push(handlers, &handler);
}

void (event_once)(enum event_type type, void (*handle)(struct event*)) {
  struct event_handler handler = { type, handle, true };
  vec_push(handlers, &handler);
}

void (event_unregister)(enum event_type type, void (*handle)(struct event*)) {
  for (size_t i = 0; i < handlers->len;) {
    struct event_handler *handler = vec_at(handlers, i);
    if (handler->type == type && handler->handle == handle) {
      vec_erase(handlers, i);
      continue;
    }

    i++;
  }
}
