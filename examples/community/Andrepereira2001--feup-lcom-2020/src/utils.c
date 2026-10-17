#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief 
 * 
 * @param port - adress of the register that will be read 
 * @param value  - value returned after port reading
 * @return int - 0 if suceed else 1
 */
int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  if(sys_inb(port, &val) != 0){
    printf("error in sys_inb\n");
    return 1;
  }
  *value = (uint8_t) val;
  return 0;
}

