#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/uart16550.h>

#include <stdint.h>

#define STARTUP_TIMEOUT_TICKS 300u

static int wait_ready(lcom_irq_t serial, uint64_t deadline) {
  for (;;) {
    lcom_event_t ev = {0};
    if (lcom_event_wait(&ev) != LCOM_OK) return 1;
    if (ev.irq_mask & serial.mask) {
      for (;;) {
        uint8_t status = 0, byte = 0;
        if (lcom_port_read8(COM1_BASE + SER_LSR, &status) != LCOM_OK) return 1;
        if (!(status & LSR_RX_RDY)) break;
        if (lcom_port_read8(COM1_BASE + SER_RBR, &byte) != LCOM_OK) return 1;
        if (byte == 'R') return 0;
      }
    }
    if (ev.tick >= deadline) {
      lcom_printf("peer sender timed out waiting for receiver\n");
      return 1;
    }
  }
}

int main(void) {
  if (lcom_init() != LCOM_OK) return 1;

  lcom_irq_t serial = {0}, timer = {0};
  if (lcom_irq_subscribe(COM1_IRQ, 0, &serial) != LCOM_OK) return 1;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer) != LCOM_OK) return 1;
  if (lcom_port_write8(COM1_BASE + SER_FCR, FCR_ENABLE_FIFO) != LCOM_OK) return 1;
  if (lcom_port_write8(COM1_BASE + SER_IER, IER_RDA) != LCOM_OK) return 1;

  lcom_event_t start = {0};
  if (lcom_delay_ticks(0, &start) != LCOM_OK) return 1;
  int result = wait_ready(serial, start.tick + STARTUP_TIMEOUT_TICKS);
  if (result == 0) {
    // Acknowledge readiness before sending the four-byte payload.
    if (lcom_port_write8(COM1_BASE + SER_THR, 'A') != LCOM_OK) result = 1;
    const char *message = "PAIR";
    for (int i = 0; result == 0 && message[i] != '\0'; i++) {
      if (lcom_port_write8(COM1_BASE + SER_THR, (uint8_t)message[i]) != LCOM_OK) result = 1;
    }
    if (result == 0) lcom_printf("peer sender sent PAIR\n");
  }

  lcom_irq_unsubscribe(&timer);
  lcom_irq_unsubscribe(&serial);
  lcom_exit();
  return result;
}
