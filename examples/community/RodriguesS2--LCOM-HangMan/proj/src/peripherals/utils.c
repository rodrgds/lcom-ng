#include <lcom/lcf.h>

#include <stdint.h>

// sys_counter = 0; 

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) {
    return 1;
  }

  *lsb = 0xFF & val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) {
    return 1;
  }

  uint16_t ex = 0xFF00 & val; //aqui apenas os MSB estão ativos
  *msb = (ex >> 8); //depois como msb so tem 8 bits temos de fazer um shift right de 8 bits a ex (que tinha 16 bits) para colocar os MSB em msb
  return 0;         //uma alternativa seria usar *msb = (val >> 8) & 0xFF; que seria mais compacto
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;
  }

  uint32_t val; //declara uma variavel com 32 bits
  int res = sys_inb(port, &val); //mando para o temporizador para receber informação, ou seja, receber a configuralçao atual (os 32 bits
                                //da porta "port")

  *value = 0xFF & val; //bitmask que faz com que de 32 bits passamos para 8 bits, pois em 0xFF apenas os 8 bits mais a direita estão a
                      //"1", estando os outros a "0", logo ao fazer um AND, apenas ficamos com os 8 bits da direita
  return res;
}
