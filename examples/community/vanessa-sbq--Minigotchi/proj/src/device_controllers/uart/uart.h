#include "serialPort.h"
#include "queue.h"

#include "../utils.h"


int serial_read_byte();
int serial_send_byte(uint8_t byte);
int serial_subscribe_int(uint8_t* bit_no);
int serial_init();
int serial_unsubscribe_int();
void serial_exit();
Queue* serial_get_receive_queue();
void serial_interrupt_handler();
int serial_clear_interrupts();
