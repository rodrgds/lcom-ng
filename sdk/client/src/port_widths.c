#include <lcom/lcom.h>

int lcom_port_read16(uint16_t port, uint16_t *value) {
  if (value == NULL) return LCOM_ERR;
  uint8_t lo = 0, hi = 0;
  if (lcom_port_read8(port, &lo) != LCOM_OK ||
      lcom_port_read8((uint16_t)(port + 1u), &hi) != LCOM_OK) return LCOM_ERR;
  *value = (uint16_t)(lo | ((uint16_t)hi << 8));
  return LCOM_OK;
}

int lcom_port_write16(uint16_t port, uint16_t value) {
  if (lcom_port_write8(port, (uint8_t)value) != LCOM_OK) return LCOM_ERR;
  return lcom_port_write8((uint16_t)(port + 1u), (uint8_t)(value >> 8));
}

int lcom_port_read32(uint16_t port, uint32_t *value) {
  if (value == NULL) return LCOM_ERR;
  uint16_t lo = 0, hi = 0;
  if (lcom_port_read16(port, &lo) != LCOM_OK ||
      lcom_port_read16((uint16_t)(port + 2u), &hi) != LCOM_OK) return LCOM_ERR;
  *value = (uint32_t)lo | ((uint32_t)hi << 16);
  return LCOM_OK;
}

int lcom_port_write32(uint16_t port, uint32_t value) {
  if (lcom_port_write16(port, (uint16_t)value) != LCOM_OK) return LCOM_ERR;
  return lcom_port_write16((uint16_t)(port + 2u), (uint16_t)(value >> 16));
}
