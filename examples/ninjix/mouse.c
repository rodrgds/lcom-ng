#include "mouse.h"

static lcom_irq_t g_mouse_irq;
static int g_mouse_subscribed = 0;
static uint8_t g_mouse_byte = 0;
static bool g_mouse_has_byte = false;
static bool g_mouse_error = false;

int mouse_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &g_mouse_irq) != LCOM_OK) return 1;
  *bit_no = g_mouse_irq.bit_no;
  g_mouse_subscribed = 1;
  return 0;
}

int mouse_unsubscribe_int(void) {
  if (!g_mouse_subscribed) return 0;
  g_mouse_subscribed = 0;
  return lcom_irq_unsubscribe(&g_mouse_irq) == LCOM_OK ? 0 : 1;
}

void mouse_ih(void) {
  uint8_t status = 0;
  uint8_t data = 0;
  g_mouse_has_byte = false;
  g_mouse_error = false;
  if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK || (status & KBC_ST_OBF) == 0) return;
  if (lcom_port_read8(KBC_OUT_BUF, &data) != LCOM_OK) {
    g_mouse_error = true;
    return;
  }
  if ((status & KBC_ST_ERR) != 0 || (status & KBC_ST_AUX) == 0) {
    g_mouse_error = true;
    return;
  }
  g_mouse_byte = data;
  g_mouse_has_byte = true;
}

int mouse_get_byte(uint8_t *byte) {
  if (byte == NULL || !g_mouse_has_byte) return 1;
  *byte = g_mouse_byte;
  g_mouse_has_byte = false;
  return 0;
}

bool mouse_had_error(void) {
  return g_mouse_error;
}

int mouse_send_cmd(uint8_t cmd) {
  if (lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) != LCOM_OK) return 1;
  return lcom_port_write8(KBC_IN_BUF, cmd) == LCOM_OK ? 0 : 1;
}

int mouse_enable_data_report(void) {
  return mouse_send_cmd(MOUSE_CMD_ENABLE_DR);
}

int mouse_disable_data_report(void) {
  return mouse_send_cmd(MOUSE_CMD_DISABLE_DR);
}
