#include <lcom/lcf.h>
#include <stdint.h>

uint32_t sys_inb_counter = 0; 

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  
  sys_inb_counter++; 
  
  if (sys_inb(port, &val) != 0) {
    return 1;
  }

  *value = (uint8_t)val;
  return 0;
}
