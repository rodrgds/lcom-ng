#include "kbc.h"

static lcom_irq_t g_kbc_irq;
static int g_kbc_subscribed = 0;
static uint8_t g_kbc_byte = 0;
static bool g_kbc_has_byte = false;
static bool g_kbc_error = false;

int kbc_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &g_kbc_irq) != LCOM_OK) return 1;
  *bit_no = g_kbc_irq.bit_no;
  g_kbc_subscribed = 1;
  return 0;
}

int kbc_unsubscribe_int(void) {
  if (!g_kbc_subscribed) return 0;
  g_kbc_subscribed = 0;
  return lcom_irq_unsubscribe(&g_kbc_irq) == LCOM_OK ? 0 : 1;
}

void kbc_ih(void) {
  uint8_t status = 0;
  uint8_t data = 0;
  g_kbc_has_byte = false;
  g_kbc_error = false;
  if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK || (status & KBC_ST_OBF) == 0) return;
  if (lcom_port_read8(KBC_OUT_BUF, &data) != LCOM_OK) {
    g_kbc_error = true;
    return;
  }
  if ((status & (KBC_ST_ERR | KBC_ST_AUX)) != 0) {
    g_kbc_error = true;
    return;
  }
  g_kbc_byte = data;
  g_kbc_has_byte = true;
}

int kbc_get_byte(uint8_t *byte) {
  if (byte == NULL || !g_kbc_has_byte) return 1;
  *byte = g_kbc_byte;
  g_kbc_has_byte = false;
  return 0;
}

bool kbc_had_error(void) {
  return g_kbc_error;
}

int kbc_read_output(uint8_t *data) {
  return data != NULL && lcom_port_read8(KBC_OUT_BUF, data) == LCOM_OK ? 0 : 1;
}

int kbc_issue_cmd(uint8_t cmd) {
  return lcom_port_write8(KBC_CMD_REG, cmd) == LCOM_OK ? 0 : 1;
}

int kbc_write_arg(uint8_t arg) {
  return lcom_port_write8(KBC_IN_BUF, arg) == LCOM_OK ? 0 : 1;
}

int kbc_read_cmd_byte(uint8_t *cb) {
  if (cb == NULL) return 1;
  if (kbc_issue_cmd(KBC_CMD_READ_CB) != 0) return 1;
  return lcom_port_read8(KBC_OUT_BUF, cb) == LCOM_OK ? 0 : 1;
}

int kbc_write_cmd_byte(uint8_t cb) {
  if (kbc_issue_cmd(KBC_CMD_WRITE_CB) != 0) return 1;
  return kbc_write_arg(cb);
}

int kbc_enable_int(void) {
  uint8_t cb = 0;
  if (kbc_read_cmd_byte(&cb) != 0) return 1;
  return kbc_write_cmd_byte((uint8_t)(cb | KBC_CB_INT));
}

int kbc_disable_int(void) {
  uint8_t cb = 0;
  if (kbc_read_cmd_byte(&cb) != 0) return 1;
  return kbc_write_cmd_byte((uint8_t)(cb & ~KBC_CB_INT));
}
