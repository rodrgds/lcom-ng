#include <lcom/lcf.h>
#include <stdint.h>


int (util_get_LSB)(uint16_t val, uint8_t *lsb)
{

  uint8_t byte = 0xFF;
  byte = byte & val;
  *lsb = byte;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb)
{

  uint8_t byte = 0xFF;
  byte = (val >> 8);
  *msb = byte;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value)
{

  uint32_t readBackStatus;
  if (sys_inb(port, &readBackStatus) != OK)
  {
    return 1;
  }
  *value = (uint8_t)readBackStatus;

  return 0;
}
