#include "kbc.h"


int TIMERhook;
int KBChook;
int MOUSEhook;



int timer_set_rate(unsigned long timer, unsigned long rate) {
	unsigned long sqwave = (unsigned int)((TIMER_FREQ / rate) + 0.5);
	if (sqwave > 65535)
		return 1;

	switch(timer) {
	case 0:
		sys_outb(TIMER_CTRL, (TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN));
		sys_outb(TIMER_0, (unsigned char)(sqwave));
		sys_outb(TIMER_0, (unsigned char)(sqwave >> 8));
		break;
	case 2:
		sys_outb(TIMER_CTRL, (TIMER_SEL2 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN));
		sys_outb(TIMER_2, (unsigned char)(sqwave));
		sys_outb(TIMER_2, (unsigned char)(sqwave >> 8));
		break;
	}
	return 0;
}

int timer_get_status(unsigned long timer, u8_t *st) {
	unsigned char status;
	unsigned long *stl;

	stl = (unsigned long*)malloc(sizeof(unsigned long));
	switch(timer) {
	case 0:
		sys_outb(TIMER_CTRL, (0 | TIMER_RB_CMD | TIMER_READ0));
		sys_inb(TIMER_0, stl);
		break;
	case 2:
		sys_outb(TIMER_CTRL, (0 | TIMER_RB_CMD | TIMER_READ2));
		sys_inb(TIMER_2, stl);
		break;
	}
	*st = (unsigned char)*stl;

	return 0;
}

int timer_show_status(unsigned long timer) {
	unsigned char *st;
	unsigned char status;
	int i;

	st = (unsigned char*)malloc(sizeof(unsigned char));
	timer_get_status(timer, st);
	printf("     STATUS\nMSB   ---    LSB\n");
	for (i = 0; i < 8; i++) {
		status = *st;
		status = status >> (7-i);
		status = status & 0x01;
		printf ("%d ", status);
	}
}

unsigned long timer_subscribe_int(void ) {
	int* bitmask;
	int r;
	bitmask = (int*)malloc(sizeof(int));
	*bitmask = 4;
	r = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, bitmask);
	TIMERhook = *bitmask;
	if (r == OK && sys_irqenable(bitmask) == OK) {
		return 16;
	}
	else
		return 0;
}

int timer_unsubscribe_int() {
	if (sys_irqdisable(&TIMERhook) == OK && sys_irqrmpolicy(&TIMERhook) == OK) {
		return 0;
	}
	else
		return 1;
}

unsigned long kbc_subscribe_int(void ) {
	int* bitmask;
	int r;
	bitmask = (int*)malloc(sizeof(int));
	*bitmask = 2;
	r = sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, bitmask);
	KBChook = *bitmask;
	if (r == OK && sys_irqenable(bitmask) == OK) {
		return 4;
	}
	else
		return 0;
}

int kbc_unsubscribe_int() {
	if (sys_irqdisable(&KBChook) == OK && sys_irqrmpolicy(&KBChook) == OK) {
		return 0;
	}
	else
		return 1;
}

unsigned char kbc_read() {
	unsigned long stat;
	unsigned long data;
	while( 1 ) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 ) {
				return (unsigned char)data;
			}
			else
				return -1;
		}

	}
}

int mouse_unsubscribe_int() {
	unsigned long stat;
	unsigned long data;
	int i = 0;
	if (sys_irqrmpolicy(&MOUSEhook) == OK) {
		return 0;
	}
	else
		return 1;
}

static int mouse_command(unsigned char command) {
	unsigned char reply;
	int attempt;
	if (write_kbc(CMD_REG, 0xD4) != 0 || write_kbc(DATA_REG, command) != 0)
		return 1;
	for (attempt = 0; attempt < 10; attempt++) {
		reply = mouse_read();
		if (reply == 0xFA) return 0;
		if (reply != (unsigned char)-1) return 1;
		tickdelay(micros_to_ticks(WAIT_TIME));
	}
	return 1;
}

unsigned long mouse_subscribe_int(void) {
	if (mouse_command(0xF5) != 0) return 0;
	MOUSEhook = 2;
	if (sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &MOUSEhook) != OK)
		return 0;
	if (sys_irqenable(&MOUSEhook) != OK) {
		sys_irqrmpolicy(&MOUSEhook);
		return 0;
	}
	if (mouse_command(0xF4) != 0) {
		sys_irqdisable(&MOUSEhook);
		sys_irqrmpolicy(&MOUSEhook);
		return 0;
	}
	return 1 << 2;
}

unsigned char mouse_read()
{
	/*unsigned long st, dat;
	int j=1;

		sys_inb(STAT_REG, &st);
		if((st & OBF) !=0)
		{	sys_inb(DATA_REG, &dat);


		if((st &(PAR_ERR | TO_ERR))==0)
			return (char)dat;
		else if (dat == 0xfe)
			return dat;
		else if(dat==0xfc)
			return dat;
		else
			return -1;
		}
	return -1;*/
	unsigned long status, data;
	sys_inb(STAT_REG, &status);
	if((status & OBF) != 0)
	{
		sys_inb(DATA_REG, &data);

		if ((status &(PAR_ERR | TO_ERR))==0)
		{
			return (char)data;
		}
		else if (data == 0xFE)
		{
			return data;
		}
		else if (data==0xFC)
		{
			return data;
		}
		else
		{
			return -1;
		}
		return -1;
	}
	return (unsigned char)-1;
}

void kbc_clean_buf() {
	unsigned long stat;
	unsigned long data;
	while (1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			sys_outb(KBC_CMD_REG, 0xD4);
			sys_outb(OUT_BUF, 0xF5);
			sys_inb(STAT_REG, &stat);
			if (stat & OBF) {
				sys_inb(OUT_BUF, &data);
				if (data != ACK) {
					printf("disable data packets transmission failed\n");
					continue;
				}
				sys_outb(KBC_CMD_REG, 0xD4);
				sys_outb(OUT_BUF, 0xF4);
				sys_inb(OUT_BUF, &data);
				if (data != ACK) {
					printf("enable data packets transmission failed\n");
					continue;
				}
				break;
			}
		}
	}
}

int write_kbc(unsigned long port, unsigned char byte)
{
	unsigned long stat;
	while(1)
	{
		sys_inb(STAT_REG, &stat);
		/*loop until 8042 input buffer empty*/
		if((stat & IBF) == 0)
		{
			sys_outb(port, byte);
			return 0;
		}
		tickdelay(micros_to_ticks(WAIT_TIME));
	}
	return -1;
}

void init_kbc()
{
	write_kbc(CMD_REG, 0xD4);
	write_kbc(DATA_REG, 0xE8);

	write_kbc(CMD_REG, 0xD4);
	write_kbc(DATA_REG, 2);

	write_kbc(CMD_REG, 0xD4);
	write_kbc(DATA_REG, DISABLE_STREAM);

	write_kbc(CMD_REG, 0xD4);
	write_kbc(DATA_REG, ENABLE_PACKETS);

	write_kbc(CMD_REG, 0xD4);
	write_kbc(DATA_REG, ENABLE_STREAM);

	write_kbc(CMD_REG, 0xD4);
	write_kbc(DATA_REG, ENABLE_PACKETS);
}
