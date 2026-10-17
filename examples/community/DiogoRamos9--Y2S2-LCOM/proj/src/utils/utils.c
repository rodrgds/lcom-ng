#include <lcom/lcf.h>
#include <stdint.h>
#define LSB_MASK 0xFF

int(util_get_LSB)(uint16_t value, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = (value & LSB_MASK);
  return 0;
}

int(util_get_MSB)(uint16_t value, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (value >> 8) & LSB_MASK;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {  // Para prevenir erros
  if (value == NULL) return 1;                  // Verifica se é válido
  uint32_t new_value;                                 // sys_inb só aceita com 32 bits
  if (sys_inb(port, &new_value)) return 1;       // meter em val o valor de port
  *value = (0xFF & new_value);                          // fica apenas com os 8 primeiros bits de val
  return 0;
}
