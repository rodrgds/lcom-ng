#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t v;

  if (sys_inb(port, &v) != OK) {
    printf("Error in sys_inb()\n");
    return 1;
  }
  
  *value = v;

  return 0;
}
