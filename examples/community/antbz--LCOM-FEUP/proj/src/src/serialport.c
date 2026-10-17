#include "../headers/serialport.h"

int ser_hookid = 5;
uint8_t ser_in;
bool ser_read = false;


int (subscribe_serialport)(uint8_t *bit_no) {
  *bit_no = ser_hookid;

  if (sys_irqsetpolicy(COM1_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &ser_hookid)) {
    printf("Error setting policy\n");
    return 1;
  }

  return 0;
}

int (unsubscribe_serialport)() {
  if (sys_irqrmpolicy(&ser_hookid)) {
    return 1;
  }
  return 0;
}

int read_ser_lsr(uint8_t* status) {
  uint8_t read;
  if (util_sys_inb(COM1 + UART_LSR, &read)) {
    return 1;
  }
  if (read & LSR_PAR_ERROR || read & LSR_OR_ERROR || read & LSR_FR_ERROR) {
    return 1;
  }
  *status = read;
  return 0;
}

void ser_ih() {
  uint8_t status;

  if (read_ser_lsr(&status)) { return; }

  if ((status & LSR_RD)) {
    util_sys_inb(COM1 + UART_RBR, &ser_in);
    ser_read = true;
  }
}

int serial_transmit_force(uint8_t data) {
 if (sys_outb(COM1 + UART_THR, data)) {
   printf("SER ERR\n");
   return 1;
 }
 printf("Message %d sent!", data);
 return 0;
}
