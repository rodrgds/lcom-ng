#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "timer.h"
#include "i8042.h"

//tickdelay(micros_to_ticks(DELAY_US);

int hook_id_1 = KEYBOARD_IRQ;

int keyboard_subscribe_int(void ) {

	if ((sys_irqsetpolicy(KEYBOARD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_1) == OK) && (sys_irqenable(&hook_id_1) == OK))
		return BIT(KEYBOARD_IRQ); //return activated bit by kernel on hook_id. which is value we set
	else
		{
			printf("\nCouldn't subscribe keyboard interrupts.\n");
			return -1;  //return negative on failure
		}
}

int keyboard_unsubscribe_int() {

	if ((sys_irqdisable (&hook_id_1) == OK) && (sys_irqrmpolicy(&hook_id_1) == OK))
		return 0;
	else
		{
			printf("\nUnable to unsubscribe interrupts.\n");
			return 1;
		}
}

unsigned long keyboard_c_handler(){
	unsigned long scan;

	if (kbd_read(&scan) != OK)
		return 1;

	if (scan == TWO_BYTE_SCAN){
		scan |= (TWO_BYTE_SCAN << 8);
	}

	return scan;
}

int kbd_write(unsigned long command){
	unsigned long stat;
	unsigned int i = 0;

	while (i < N_TRY){
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;
		if ((stat & IN_OK) == 0){
			if (sys_outb(KBD_CMD, command) != OK)
				return -1;
			else return 0;
		}
		//tickdelay(micros_to_ticks(DELAY_MS));
		i++;
	}

	return -1;
}

int kbd_read(unsigned long *data){
	unsigned long stat;
	unsigned int i = 0;

	while (i < N_TRY){
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;
		if ((stat & IN_OK)== 0){
			if (sys_inb(OUT_BUF, data) != OK)
				return -1;
			if ((stat & STAT_ERR) == 0)
				return 0;
			else return -1;
		}
		//tickdelay(micros_to_ticks(DELAY_MS));
		i++;
	}

}
