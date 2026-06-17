#include "serial_port.h"

#define SER_BASE_CLOCK 115200u
#define SER_RX_BUFFER_SIZE 512u

static lcom_irq_t serial_irq;
static uint16_t serial_port;
static bool serial_subscribed;
static uint8_t rx_buffer[SER_RX_BUFFER_SIZE];
static uint16_t rx_head;
static uint16_t rx_tail;

static int ser_read_reg(uint32_t port, uint8_t reg, uint8_t *value) {
  if (value == NULL || port > UINT16_MAX) return 1;
  return lcom_port_read8((uint16_t)(port + reg), value) == LCOM_OK ? 0 : 1;
}

static int ser_write_reg(uint32_t port, uint8_t reg, uint8_t value) {
  if (port > UINT16_MAX) return 1;
  return lcom_port_write8((uint16_t)(port + reg), value) == LCOM_OK ? 0 : 1;
}

static uint8_t ser_irq_for_port(uint32_t port) {
  if (port == COM1_BASE) return COM1_IRQ;
  if (port == COM2_BASE) return COM2_IRQ;
  return 0xFF;
}

static int ser_line_control(uint8_t bits, uint8_t stop, uint8_t parity, uint8_t *out) {
  if (out == NULL || bits < 5 || bits > 8) return 1;

  uint8_t lcr = (uint8_t)(bits - 5);
  if (stop == 2) {
    lcr |= BIT(2);
  } else if (stop != 1) {
    return 1;
  }

  if (parity == 1) {
    lcr |= BIT(3);
  } else if (parity == 2) {
    lcr |= BIT(3) | BIT(4);
  } else if (parity != 0) {
    return 1;
  }

  *out = lcr;
  return 0;
}

static void rx_reset(void) {
  rx_head = 0;
  rx_tail = 0;
}

static bool rx_empty(void) {
  return rx_head == rx_tail;
}

static void rx_push(uint8_t data) {
  uint16_t next = (uint16_t)((rx_head + 1u) % SER_RX_BUFFER_SIZE);
  if (next == rx_tail) {
    rx_tail = (uint16_t)((rx_tail + 1u) % SER_RX_BUFFER_SIZE);
  }
  rx_buffer[rx_head] = data;
  rx_head = next;
}

static int rx_pop(uint8_t *data) {
  if (data == NULL || rx_empty()) return 1;
  *data = rx_buffer[rx_tail];
  rx_tail = (uint16_t)((rx_tail + 1u) % SER_RX_BUFFER_SIZE);
  return 0;
}

int ser_set_impl(uint32_t port, uint32_t baud, uint8_t bits, uint8_t stop, uint8_t parity) {
  if (ser_irq_for_port(port) == 0xFF || baud == 0) return 1;

  uint8_t lcr = 0;
  if (ser_line_control(bits, stop, parity, &lcr) != 0) return 1;

  uint32_t divisor = SER_BASE_CLOCK / baud;
  if (divisor == 0 || divisor > UINT16_MAX) return 1;

  if (ser_write_reg(port, SER_LCR, (uint8_t)(lcr | LCR_DLAB)) != 0) return 1;
  if (ser_write_reg(port, SER_DLL, (uint8_t)(divisor & 0xFFu)) != 0) return 1;
  if (ser_write_reg(port, SER_DLM, (uint8_t)(divisor >> 8)) != 0) return 1;
  if (ser_write_reg(port, SER_LCR, lcr) != 0) return 1;
  return ser_write_reg(port, SER_MCR, MCR_DTR | MCR_RTS | MCR_OUT2);
}

int ser_enable_fifo(uint32_t port, uint8_t trigger) {
  return ser_write_reg(port, SER_FCR,
                       (uint8_t)(FCR_ENABLE_FIFO | FCR_CLEAR_RX | FCR_CLEAR_TX |
                                 (trigger & FCR_TRIG_14)));
}

int ser_subscribe_int(uint32_t port, int *bit_no) {
  uint8_t irq = ser_irq_for_port(port);
  if (irq == 0xFF || bit_no == NULL) return 1;
  if (lcom_irq_subscribe(irq, 0, &serial_irq) != LCOM_OK) return 1;
  if (ser_write_reg(port, SER_IER, IER_RDA | IER_RLS) != 0) {
    (void)lcom_irq_unsubscribe(&serial_irq);
    return 1;
  }
  serial_port = (uint16_t)port;
  serial_subscribed = true;
  *bit_no = serial_irq.bit_no;
  rx_reset();
  return 0;
}

int ser_unsubscribe_int(void) {
  if (serial_subscribed) {
    (void)ser_write_reg(serial_port, SER_IER, 0);
    if (lcom_irq_unsubscribe(&serial_irq) != LCOM_OK) return 1;
    serial_subscribed = false;
  }
  return 0;
}

void ser_ih(void) {
  if (!serial_subscribed) return;

  uint16_t limit = 256;
  while (limit-- > 0) {
    uint8_t lsr = 0;
    if (ser_read_reg(serial_port, SER_LSR, &lsr) != 0) return;
    if ((lsr & LSR_RX_RDY) == 0) return;

    uint8_t data = 0;
    if (ser_read_reg(serial_port, SER_RBR, &data) != 0) return;
    rx_push(data);
  }
}

void ser_clear_rx_state(uint32_t port) {
  if (port == serial_port) rx_reset();
  (void)ser_write_reg(port, SER_FCR, FCR_ENABLE_FIFO | FCR_CLEAR_RX);
}

int ser_send_char_int(uint32_t port, uint8_t data) {
  uint8_t lsr = 0;
  if (ser_read_reg(port, SER_LSR, &lsr) != 0) return 1;
  if ((lsr & LSR_THRE) == 0) return 1;
  return ser_write_reg(port, SER_THR, data);
}

int ser_recv_char_int(uint8_t *data) {
  return rx_pop(data);
}
