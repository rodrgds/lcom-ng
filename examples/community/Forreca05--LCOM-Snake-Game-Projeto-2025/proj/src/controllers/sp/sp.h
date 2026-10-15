#include <lcom/lcf.h>

#define COM1_IRQ 4
#define COM1_BASE_ADDR 0x3F8
#define IER 1
#define IIR 2
#define LCR 3
#define LSR 5
#define NO_INTERRUPT_PENDING BIT(0)

int sp_subscribe_int(uint8_t* irq_set);
int sp_unsubscribe_int();
int sp_send_byte(uint8_t byte);
void sp_ih();
// int sp_test_conf();



