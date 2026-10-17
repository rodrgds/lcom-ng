
#include <stdint.h>
/**
 * @brief Gets the less significant byte
 * 
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb=(val & 0x00FF);


  return 0;
}
/**
 * @brief Gets the most significant byte
 * 
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb=val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
    //vai ler de um porto qualquer; o port indica a registo quero buscar a informacao
    uint32_t new_value;
    sys_inb(port, &new_value);
    *value = (uint8_t)new_value;
    return 0; //0 sucesso
}
