#include <lcom/lcf.h>

#include <stdint.h>

int scancode_counter = 0;
/*Gets the LSB of a 16 bit value. */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL) return 1; //Check if bit is nulll
  *lsb = val & 0xFF; //pointer to the value conversion result

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL) return 1;
  *msb = (val >> 8) & 0xFF; // shifts right until the MSB is the only value left in the 16 bit value.

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if(value == NULL) return 1;
  uint32_t aux;

//sends an input command to the port and reads the value parsed
  if(sys_inb(port,&aux)!=0) return 1;
  scancode_counter++;
  *value = aux & 0xFF; //For lcom, we are only interested in the LSB.

  return 0;
}
