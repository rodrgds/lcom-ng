#include <lcom/lcf.h>

#include <stdint.h>


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;     // validamos o apontador
  *lsb = 0xFF & val;             // pomos no apontador os 8 bits menos significativos de val
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;     // validamos o apontador
  *msb = (val >> 8) & 0xFF;      // pomos no apontador os 8 bits mais significativos de val
  return 0;
}


/*no contexto de LCOM só vamos usar 8 bits, mas a função sys_inb usa 32,
portanto temos de criar esta função auxiliar que nos "filtra" apenas os
8 bits - less significant? - que queremos*/

int util_sys_inb(int port, uint8_t *value) {
  if (value == NULL) return 1;   // validamos o apontador
  uint32_t val;                  // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val); // pomos em val o valor lido de port --- timer
  *value = 0xFF & val;           // fazemos com que value fique apenas com os primeiros 8 bits de val (que tem 32)
  return ret;
}
