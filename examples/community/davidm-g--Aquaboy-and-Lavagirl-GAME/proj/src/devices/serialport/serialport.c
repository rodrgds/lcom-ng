#include "serialport.h"

int hook_id_COM1 = COM1_IRQ;
uint16_t ser_port = COM1;
uint8_t data = 0;
static Queue *rQueue; // queue for received characters
static Queue *tQueue; // queue for sent characters

int(ser_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_COM1);
  return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_COM1);
}

int(ser_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id_COM1);
}

int ser_get_status(uint8_t *st) {
  return util_sys_inb(ser_port + LSR, st);
}

int ser_init() {
  uint8_t ier;
  if (util_sys_inb(ser_port + IER, &ier))
    return 1;
  ier &= 0xF0;
  if (sys_outb(ser_port + IER, ier | RECEIVE_DATA_INT | SEND_DATA_INT))
    return 1;
  rQueue = createQueue();
  tQueue = createQueue();
  return 0;
}

void ser_exit() {
  clear(rQueue);
}

void ser_ih() {
  uint8_t iir;
  if (util_sys_inb(ser_port + IIR, &iir))
    return;
  if (!(iir & SER_NO_INT_PEND)) {
    if ((iir & INT_ID) == SER_FIFO_INT)
      while (ser_read_byte());

    if ((iir & INT_ID) == SEND_DATA_INT)
      while (ser_send_byte());
  }
}

int ser_send_byte() {
  uint8_t st, attempts = 10;
  while (attempts--) {
    if (ser_get_status(&st))
      return 1;
    if ((st & (BIT(5) | BIT(6))))
      return sys_outb(ser_port + THR, pop(tQueue));
    tickdelay(micros_to_ticks(10));
  }
  return 1;
}

int ser_read_byte() {
  uint8_t st, buffer;
  if (ser_get_status(&st))
    return 1;
  if (st & RECEIVE_DATA_INT) {
    if (util_sys_inb(ser_port + RBR, &buffer))
      return 1;
    if (st & SER_RX_ERR)
      return 1;
    push(rQueue, buffer);
    return 0;
  }
  return 1;
}
