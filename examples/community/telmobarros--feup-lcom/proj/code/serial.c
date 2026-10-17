#include "serial.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8042.h"

static int serial_hook_id=4;

int serial_subscribe_int() {
	int serial_id = BIT(serial_hook_id);

	//Activate interrupts
	sys_outb(COM1_ADDRESS + INTERRUPT_ENABLE_IER,
			BIT(RECEIVED_DATA_INT) |
			BIT(TRANSMITTER_EMPTY_INT) |
			BIT(LINE_STATUS_INT));

	if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &serial_hook_id) != OK)
		return 1;
	if (sys_irqenable(&serial_hook_id) != OK)
		return 1;

	return serial_id;
}

int serial_unsubscribe_int() {
	if (sys_irqrmpolicy(&serial_hook_id) != OK)
		return 1;
	if (sys_irqdisable(&serial_hook_id) != OK)
		return 1;
	return 0;
}

int serial_interrupt_identification(){
	unsigned long temp;
	sys_inb(COM1_ADDRESS + INTERRUPT_IDENTIFICATION_IIR, &temp);
	if (temp & BIT(INTERRUPT_STATUS) != 0)
		return -1;
	else{
		// gets origin of interrupt by reading three bits that store that information
		temp &= BIT(INTERRUPT_ORIGIN) | BIT(INTERRUPT_ORIGIN +1) | BIT(INTERRUPT_ORIGIN +2);
		temp >>= INTERRUPT_ORIGIN;
		return (int)temp;
	}
}

char serial_read_char(){
	unsigned long temp;
		sys_inb(COM1_ADDRESS + RECEIVER_BUFFER_RBR, &temp);
		return (char)temp;
}

int serial_write_char(char temp){
		sys_outb(COM1_ADDRESS + TRANSMITTER_HOLDING_THR, temp);
		return 0;
}
