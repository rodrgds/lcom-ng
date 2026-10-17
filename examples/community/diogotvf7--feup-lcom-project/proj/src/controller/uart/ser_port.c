#include "ser_port.h"

int hook_id = 4;
Queue *xmit_fifo = NULL;
Queue *rcvr_fifo = NULL;

int config_uart() 
{
  uint8_t lcr = 0, ier = 0, fcr = 0;

  lcr = BITS_PER_CHAR(8) | NO_PARITY;
  ier = ENABLE_RECEIVED_DATA_INT | ENABLE_TRANSMITTER_EMPTY_INT | ENABLE_RECEIVER_LINE_INT;
  fcr = ENABLE_FIFO | CLEAR_RCVR_FIFO | CLEAR_XMIT_FIFO | RCVR_TRIGGER_8;

  if (set_uart_lcr(lcr) != OK) return !OK;      // SEND LCR CONFIG

  if (set_uart_dlab(1) != OK) return !OK;       //
  if (set_uart_freq(BITRATE_115200) != OK) return !OK;    // SEND BIT RATE FREQUENCY
  if (set_uart_dlab(0) != OK) return !OK;       //

  if (set_uart_ier(ier) != OK) return !OK;      // SEND IER CONFIG
  
  if (set_uart_fcr(fcr) != OK) return !OK;  // SEND FCR CONFIG

  return OK;
}

int set_uart_dlab(uint8_t status) {
  uint8_t lcr = 0;
  if (get_uart_lcr(&lcr) != OK) return 1;
  if (status) lcr |= DLAB;
  else lcr &= ~DLAB;
  return set_uart_lcr(lcr);
}

int subscribe_uart_int(uint8_t *bit_no) 
{
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int set_uart_fcr(uint8_t byte) 
{
  return sys_outb(COM1 + SER_FCR, byte);
}

int unsubscribe_uart_int() 
{
  return sys_irqrmpolicy(&hook_id);
}

int set_uart_ier(uint8_t byte)
{
  return sys_outb(COM1 + SER_IER, byte);
}

int get_uart_ier(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_IER, byte);
}

int set_uart_lcr(uint8_t byte)
{
  return sys_outb(COM1 + SER_LCR, byte);
}

int get_uart_lcr(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_LCR, byte);
}

int set_uart_iir(uint8_t byte)
{
  return sys_outb(COM1 + SER_IIR, byte);
}

int get_uart_iir(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_IIR, byte);
}

int get_uart_lsr(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_LSR, byte);
}

int set_uart_freq(uint16_t freq)
{
  uint8_t lcr, lsb, msb;
  if (get_uart_lcr(&lcr)) return !OK;
  if (set_uart_lcr(lcr | DLAB)) return !OK;
  
  util_get_LSB(freq, &lsb);
  util_get_MSB(freq, &msb);
  if (sys_outb(COM1 + SER_DLL, lsb) || sys_outb(COM1 + SER_DLM, msb))
    return !OK;
  return OK;
}

void uart_ih() 
{
  uint8_t iir, byte;
  util_sys_inb(COM1 + SER_IIR, &iir);
  if(!(iir & INTERRUPT_PENDING)) {
    switch(iir & INTERRUPT_ORIGIN_MASK) {
      case MODEM_STATUS_INT:
        break;
      case TRANSMITTER_EMPTY_INT:
        break;
      case CHAR_TIMEOUT:
      case RECEIVED_DATA_INT:
        while (receive_uart_byte(&byte));
        // util_sys_inb(COM1 + SER_IIR, &iir);
        break;
      case LINE_STATUS_INT:
        break;
      default:
        break;
    }
  }
}

int send_uart_byte(uint8_t byte) 
{
  uint8_t lsr;
  if (get_uart_lsr(&lsr)) return !OK;
  if (lsr & (OVERRUN_ERR | PARITY_ERR | FRAME_ERR | FIFO_ERROR)) {
    printf("UART: Error sending byte\n");
    return !OK;
  }
  // if (queue_empty(&xmit_fifo))
  if (queue_empty(&xmit_fifo) &&  (lsr & TRANSMITTER_EMPTY)) {
    return sys_outb(COM1 + SER_THR, byte);
  }
  queue_push(&xmit_fifo, &byte, sizeof(uint8_t));
  return OK;
}

int send_uart_bytes(uint8_t *bytes, uint32_t size) 
{
  for (uint32_t i = 0; i < size; i++) {
    if (send_uart_byte(bytes[i]) != OK) {
      printf("UART: Error sending bytes\n");
      return !OK;
    }
  }
  return OK;
}

int receive_uart_byte(uint8_t *byte) 
{
  uint8_t lsr;
  if (get_uart_lsr(&lsr) != OK) return !OK;
  if (lsr & RECEIVER_DATA) {
    if (util_sys_inb(COM1 + SER_RBR, byte) != OK) return !OK;
    queue_push(&rcvr_fifo, byte, sizeof(uint8_t));

    return OK;
  }
  return !OK;
}

int send_start_msg() {
  return send_uart_byte(START_GAME);
}
