#include "state/state.h"

#include "vector/vector.h"

static vector *states;
static state_t curr_state;

int (state_init)() {
  states = vec_create(sizeof(state_handler));

  curr_state = INIT;
  state_register(INIT, NULL, NULL);

  return OKAY;
}

int (state_cleanup)() {
  state_set(INIT);
  vec_destroy(states);

  return OKAY;
}

int (state_register)(state_t s, void (*init)(), void (*cleanup)()) {
  size_t i = 0;

  if (states->len > 0) {
    for (; i < states->len; i++) {
      state_handler *handler = vec_at(states, i);
      if (s > handler->state) {
        break;
      } else if (s == handler->state) {
        return 1;
      }
    }
  }

  state_handler handler = { s, init, cleanup };
  vec_insert(states, &handler, i);

  return OKAY;
}

int (state_set)(state_t s) {
  if (s == curr_state) {
    return 0;
  }

  state_handler *curr = NULL, *next = NULL;

  for (size_t i = 0; (curr == NULL || next == NULL) && i < states->len; i++) {
    state_handler *handler = vec_at(states, i);

    if (handler->state == s) {
      next = handler;
    }

    if (handler->state == curr_state) {
      curr = handler;
    }
  }

  if (curr == NULL || next == NULL) {
    return 1;
  }

  if (curr->cleanup != NULL) {
    curr->cleanup();
  }

  if (next->init != NULL) {
    next->init();
  }

  curr_state = s;
  return OKAY;
}

state_t (state_get)() {
  return curr_state;
}
