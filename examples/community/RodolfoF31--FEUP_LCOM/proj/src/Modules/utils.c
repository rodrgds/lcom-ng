#include <lcom/lcf.h>

#include <stdint.h>
#define ERROR -1
#define OK 0

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  *lsb = (uint8_t) (val & 0xFF);
  return OK;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  *msb = (uint8_t) ((val >> 8) & 0xFF);
  return OK;
}

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t temp;

  if(sys_inb(port, &temp) != OK){
    printf("%s failed\n", __func__);
    return ERROR;
  }

  *value = (uint8_t) temp;

  return OK;
}
