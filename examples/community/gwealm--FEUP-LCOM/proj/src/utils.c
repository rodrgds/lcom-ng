#include "utils.h"
#include "errors.h"

void *(util_addr)(void* ptr, size_t index, size_t base_size) {
  return ((uint8_t*) ptr) + index * base_size;
}

void *(util_memset)(void *dest, int fill, size_t num, size_t base_size) {
  return memset(dest, fill, num * base_size);
}

void *(util_memmove)(void *dest, const void *src, size_t num, size_t base_size) {
  return memmove(dest, src, num * base_size);
}

void *(util_memcpy)(void *dest, const void *src, size_t num, size_t base_size) {
  if (src == NULL) {
    return memset(dest, 0, num * base_size);
  } 
  
  return memcpy(dest, src, num * base_size);
}

int (util_copy_elem)(void **dest, void *src, size_t size) {
  if (dest == NULL) {
    return VEC_EDEST;
  }

  if (src == NULL) {
    *dest = NULL;
    return OKAY;
  }

  void *copy = malloc(size);
  if (copy == NULL) {
    return VEC_EALLOC;
  }

  memcpy(copy, src, size);
  *dest = copy;
  
  return OKAY;
}

int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)val;
  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *dest) {
  uint32_t temp;
  int success = sys_inb(port, &temp);

  *dest = temp;
  return success;
}
