#ifndef NINJIX_SERIAL_PORT_H
#define NINJIX_SERIAL_PORT_H

#include "ninjix_platform.h"

#include <lcom/uart16550.h>

#ifndef FCR_TRIG_14
#define FCR_TRIG_14 0xC0
#endif

int ser_set_impl(uint32_t port, uint32_t baud, uint8_t bits, uint8_t stop, uint8_t parity);
int ser_enable_fifo(uint32_t port, uint8_t trigger);
int ser_subscribe_int(uint32_t port, int *bit_no);
int ser_unsubscribe_int(void);
void ser_ih(void);
void ser_clear_rx_state(uint32_t port);
int ser_send_char_int(uint32_t port, uint8_t data);
int ser_recv_char_int(uint8_t *data);

#endif
