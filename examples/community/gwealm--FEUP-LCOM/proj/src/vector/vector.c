#include "vector/vector.h"
#include "utils.h"

vector *(vec_create)(size_t base_size) {
  if (base_size == 0) {
    return NULL;
  }

  vector *vec = (vector*) malloc(sizeof(vector));
  if (vec == NULL) {
    return NULL;
  }

  void *arr = calloc(VEC_DEFAULT_CAPACITY, base_size);
  if (arr == NULL) {
    free(vec);
    return NULL;
  }

  vec->values = arr;
  vec->len = vec->capacity = VEC_DEFAULT_CAPACITY;
  vec->base_size = base_size;

  vec_clear(vec);
  return vec;
}

void (vec_destroy)(vector *vec) {
  free(vec->values);
  free(vec);
}

int (vec_insert)(vector *vec, void *element, size_t index) {
  if (index > vec->len) {
    index = vec->len;
  }

  if (index == vec->capacity) {
    int err_no;
    if ((err_no = vec_resize(vec, vec->capacity + VEC_DEFAULT_INCREMENT)) != OKAY) {
      printf("vec_insert: error while resizing vector (%d)\n", err_no);
      return err_no;
    }
  }

  // Shift posterior elements to the right
  void *pos = util_addr(vec->values, index, vec->base_size);
  void *dest = util_addr(vec->values, index + 1, vec->base_size);
  size_t num_right = vec->len - index;
  util_memmove(dest, pos, num_right, vec->base_size);

  // Add element to the vector
  util_memcpy(pos, element, 1, vec->base_size);
  vec->len++;

  return OKAY;
}

int (vec_erase)(vector *vec, size_t index) {
  if (index >= vec->len) {
    return VEC_EBOUNDS;
  }

  // Shift all the elements to the left one position to the right
  void *pos = util_addr(vec->values, index, vec->base_size);
  void *src = util_addr(vec->values, index + 1, vec->base_size);
  size_t num_right = vec->len - index - 1;
  util_memmove(pos, src, num_right, vec->base_size);

  vec->len--; 

  // Fill the last element's space with zeros, since the elements to the right were shifted to the left
  void *end = util_addr(vec->values, vec->len, vec->base_size);
  util_memset(end, 0, 1, vec->base_size);

  return OKAY;
}

int (vec_resize)(vector *vec, size_t new_capacity) {
  if (new_capacity == vec->capacity) {
    return OKAY;
  }

  // Remove old elements
  if (new_capacity < vec->capacity) {
    size_t num_old_values = vec->capacity - new_capacity;
    void *old_values = util_addr(vec, new_capacity, vec->base_size);
    util_memset(old_values, 0, num_old_values, vec->base_size);

    vec->len = new_capacity;
  }

  // Resize array
  if (reallocarr(&vec->values, new_capacity, vec->base_size) != 0) {
    return VEC_EALLOC;
  }

  // Fill the space for new elements with zeros
  if (new_capacity > vec->capacity) {
    size_t num_new_values = new_capacity - vec->capacity;
    void *new_values = util_addr(vec->values, vec->capacity, vec->base_size);
    util_memset(new_values, 0, num_new_values, vec->base_size);
  }

  vec->capacity = new_capacity;

  return OKAY;
}

void (vec_clear)(vector *vec) {
  util_memset(vec->values, 0, vec->len, vec->base_size);
  vec->len = 0;
}

int (vec_fill)(vector *vec, void *element) {
  vec_clear(vec);

  int err_no;
  for (size_t i = 0; i < vec->capacity; i++) {
    if ((err_no = vec_push(vec, element)) != OKAY) {
      printf("vec_fill: error while inserting element (%d)\n", err_no);
      return err_no;
    }
  }

  return OKAY;
}

int (vec_push)(vector *vec, void *element) {
  return vec_insert(vec, element, vec->len);
}

void *(vec_at)(vector *vec, size_t index) {
  if (index >= vec->len) {
    return NULL;
  }

  return util_addr(vec->values, index, vec->base_size);
}
