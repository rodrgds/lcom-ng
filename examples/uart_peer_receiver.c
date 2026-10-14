#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/uart16550.h>

#include <stdint.h>

#define READY_RETRY_TICKS 15u
#define STARTUP_TIMEOUT_TICKS 300u

static int receive_message(lcom_irq_t serial, uint64_t start_tick, char message[5]) {
  int acknowledged = 0, received = 0;
  uint64_t next_ready = start_tick;
  lcom_event_t ev = {0};
  ev.tick = start_tick;

  while (received < 4) {
    // A peer enabling its FIFO clears earlier bytes, so retry until acknowledged.
    if (!acknowledged && ev.tick >= next_ready) {
      if (lcom_port_write8(COM1_BASE + SER_THR, 'R') != LCOM_OK) return 1;
      next_ready = ev.tick + READY_RETRY_TICKS;
    }
    if (lcom_event_wait(&ev) != LCOM_OK) return 1;
    if (ev.irq_mask & serial.mask) {
      while (received < 4) {
        uint8_t status = 0, byte = 0;
        if (lcom_port_read8(COM1_BASE + SER_LSR, &status) != LCOM_OK) return 1;
        if (!(status & LSR_RX_RDY)) break;
        if (lcom_port_read8(COM1_BASE + SER_RBR, &byte) != LCOM_OK) return 1;
        if (!acknowledged) {
          if (byte == 'A') acknowledged = 1;
        } else {
          message[received++] = (char)byte;
        }
      }
    }
    if (received < 4 && ev.tick >= start_tick + STARTUP_TIMEOUT_TICKS) {
      lcom_printf("peer receiver timed out waiting for sender\n");
      return 1;
    }
  }
  return 0;
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
  char message[5] = {0};
  int result = receive_message(serial, start.tick, message);
  if (result == 0) lcom_printf("peer receiver got %s\n", message);

  lcom_irq_unsubscribe(&timer);
  lcom_irq_unsubscribe(&serial);
  lcom_exit();
  return result;
}
