#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  //gets the LSB of a 8bit value
  
  if(lsb == NULL){
    return EXIT_FAILURE;
  }

  *lsb = val & 0XFF;

  return EXIT_SUCCESS;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  //gets the MSB of a 8bit value

  if(msb == NULL){
    return EXIT_FAILURE;
  }

  *msb = (val>>8) & 0XFF;

  return EXIT_SUCCESS;
}

int (util_sys_inb)(int port, uint8_t *value) {
  //auxiliary function to help use the sys_inb with 8bit values

  uint32_t aux = 0;
  if(sys_inb(port,&aux)!=0){
    return EXIT_FAILURE;
  }
  *value = (uint8_t)(aux);
  return EXIT_SUCCESS;

}
