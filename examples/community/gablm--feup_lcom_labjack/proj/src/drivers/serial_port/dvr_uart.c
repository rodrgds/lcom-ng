#include "serial_port.h"

int hook_id_uart = 4;

queue_t *transmitter = NULL;
queue_t *receiver = NULL;
int uart_disabled = 0;

int (uart_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(hook_id_uart);

  return sys_irqsetpolicy(IRQ_COM1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_uart);
}

int (uart_unsubscribe_int)() {
  if (hook_id_uart < 0)
    return 1;

  return sys_irqrmpolicy(&hook_id_uart);
}

int uart_read(int base_addr, uint8_t offset, uint8_t *out) {
  return util_sys_inb(base_addr + offset, out);
}

int uart_write(int base_addr, uint8_t offset, uint8_t in) {
  return sys_outb(base_addr + offset, in);
}

int uart_DL_access(int base_addr, int enable) {
  uint8_t response;

  if (uart_read(base_addr, UART_LCR, &response))
    return 1;

  if (enable)
    response |= UART_LCR_DLAB;
  else
    response &= ~UART_LCR_DLAB;

  return uart_write(base_addr, UART_LCR, response);
}

int uart_set_bit_rate(int base_addr, int rate) {
  uint8_t lsb, msb;
  uint16_t d_rate = UART_MAX_BIT_RATE / rate;

  if (util_get_LSB(d_rate, &lsb))
    return 1;
  if (util_get_MSB(d_rate, &msb))
    return 1;

  if (uart_DL_access(base_addr, true))
    return 1;

  if (uart_write(base_addr, UART_DLL, lsb))
    return 1;
  if (uart_write(base_addr, UART_DLM, msb))
    return 1;

  return uart_DL_access(base_addr, false);
}

int uart_fifo_read(int base_addr) {
  uint8_t status;
  uint8_t response;

  while (true) {
    if (uart_read(base_addr, UART_LSR, &status))
      return 1;

    if (!(status & UART_LSR_HAS_DATA))
      return 0;

    if (uart_read(base_addr, UART_RBR, &response))
      return 1;

    uint8_t *info = malloc(sizeof(uint8_t));
    *info = response;
    if (queue_push(receiver, info)) {
      free(info);
      return 1;
    }
  }
  return 0;
}

int uart_fifo_write(int base_addr) {
  uint8_t status;

  while (true) {
    if (uart_read(base_addr, UART_LSR, &status))
      return 1;

    if (!(status & UART_LSR_EMPTY_HOLD))
      return 0;

    if (queue_empty(transmitter))
      return 0;

    uint8_t *content = queue_pop(transmitter);
    if (content == NULL)
      return 1;

    if (uart_write(base_addr, UART_THR, *content))
      return 1;
  }
  return 0;
}

int uart_send_byte(uint8_t byte) {
  uint8_t *info = malloc(sizeof(uint8_t));
  *info = byte;

  if (queue_push(transmitter, info)) {
    free(info);
    return 1;
  }

  return uart_fifo_write(UART_COM1);
}

int uart_get_byte(uint8_t *byte) {
  if (receiver == NULL)
    return 1;
  if (byte == NULL)
    return 1;

  uint8_t *msg = queue_pop(receiver);
  if (msg == NULL)
    return 1;

  *byte = *msg;
  free(msg);

  return 0;
}

int (uart_ih)() {
  uint8_t response;

  if (uart_read(UART_COM1, UART_IIR, &response))
    return 0;

  if (response & UART_IIR_NO_INTS)
    return 0;

  switch (response & UART_IIR_INT_MASK) {
    case UART_IIR_INT_RBR_FULL:
      uart_fifo_read(UART_COM1);
      return 1;
    case UART_IIR_INT_THR_EMPTY:
      uart_fifo_write(UART_COM1);
      return 1;
    case UART_IIR_INT_CHAR_TIMEOUT:
      uart_fifo_read(UART_COM1);
      return 1;
    default:
      return 0;
  }
  return 0;
}

int uart_init(uint8_t *bit_no, int bit_rate) {
  uint8_t cmd;

  transmitter = queue_create(UART_QUEUE_SIZE);
  if (transmitter == NULL)
    return 1;

  receiver = queue_create(UART_QUEUE_SIZE);
  if (receiver == NULL) {
    queue_destroy(&transmitter, free);
    return 1;
  }

  if (uart_set_bit_rate(UART_COM1, UART_MAX_BIT_RATE))
    return 1;

  cmd = UART_LCR_8BITCHAR | UART_LCR_1STOP | UART_LCR_PARITY_NONE;
  if (uart_write(UART_COM1, UART_LCR, cmd))
    return 1;

  cmd = UART_FCR_FIFO_ENABLE | UART_FCR_FIFO_RCVR_CLEAR | UART_FCR_FIFO_XMIT_CLEAR | UART_FCR_FIFO_RCVR_1BYTE;
  if (uart_write(UART_COM1, UART_FCR, cmd))
    return 1;

  cmd = UART_IER_ENABLE_INT_DATA | UART_IER_ENABLE_INT_THR;
  if (uart_write(UART_COM1, UART_IER, cmd))
    return 1;

  if (uart_fifo_read(UART_COM1)) {
    uart_disabled = 1;
    return 0;
  }

  return uart_subscribe_int(bit_no);
}

int uart_disable() {
  if (transmitter != NULL)
    queue_destroy(&transmitter, free);

  if (receiver != NULL)
    queue_destroy(&receiver, free);

  if (uart_disabled)
    return 0;

  return uart_unsubscribe_int();
}
