#include "keyboard.h"

int khook_id = KBC_IRQ;

int kbd_unsubscribe_int()
{

	if (OK == sys_irqdisable(&khook_id))
		if (OK == sys_irqrmpolicy(&khook_id))
			return 0;
	return -1;
}


int kbd_subscribe_int(){

	int hook_temp = khook_id;

	if (OK == sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &khook_id))
	{
		if (OK == sys_irqenable(&khook_id))
		{
			return BIT(hook_temp);
		}
	}
	return -1;
}


int kbd_scan_c(int *apt){

	unsigned long key_register;

	sys_inb(KBC_OUT_BUF, &key_register);
	*apt = (unsigned int) key_register;
	tickdelay(micros_to_ticks(DELAY_US));
	if (0xE0 == (0xE0 & *apt))
	{
		return 1;

	}
	return 0;
}


unsigned short read_data_OUTBUF_from_KBC(unsigned long* data){
	unsigned long status_register;
	while (1) {
			if (sys_inb(KBC_STAT_REG, &status_register) != OK)
				return EXIT_FAILURE;
			if (status_register & KBC_OBF) {
				if (sys_inb(KBC_OUT_BUF, data) == OK) {
					if ((status_register & (PAR_ERR | TO_ERR)) == 0)
						return EXIT_SUCCESS;
				} else
					return EXIT_FAILURE;
			}
			tickdelay(micros_to_ticks(DELAY_US));
		}
}

unsigned short KBC_issue_command_mouse(unsigned char command) {
	unsigned long stat = 0;
	if (sys_inb(KBC_STAT_REG, &stat) == OK) {
		if ((stat & KBC_IBF) == 0) {
			if (sys_outb(KBC_CMD_REG, command) == OK)
				return EXIT_SUCCESS;
			else
				return EXIT_FAILURE;
		}
		tickdelay(micros_to_ticks(DELAY_US));

	}
}

unsigned short issue_argument_KBC(unsigned char argument){
	unsigned long stat = 0;
	if (sys_inb(KBC_STAT_REG, &stat) == OK) {
		if ((stat & KBC_IBF) == 0) {
			if (sys_outb(KBC_IN_BUF, argument) == OK)
				return EXIT_SUCCESS;
			else
				return EXIT_FAILURE;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

}
