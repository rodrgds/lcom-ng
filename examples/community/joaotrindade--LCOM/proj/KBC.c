#include "KBC.h"

static int hook= 0;
unsigned char scancode;

int kbc_subscribe_int(void) {
	sys_irqsetpolicy(KB_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook);
	sys_irqenable(&hook);
	//printf("FEZ SUBSCRIBE\n");
	return 1<<0;

}

int kbc_unsubscribe_int(void) {
	sys_irqdisable(&hook);
}

long read_scancode() {
	long stat, data;

	//printf("Entrou read_scancode\n");

	while(1)
	{
		sys_inb(STAT_REG, &stat);
		if (stat & OBF)
		{
			sys_inb(OUT_BUF, &data);
			if ( (stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;

		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}
	//printf("Saiu read_scancode\n");

}


int test_scan(void) {

	int ipc_status, irq_set, esc_found;
	message msg;
	esc_found = 0;
	irq_set = kbc_subscribe_int();



	while(!esc_found) {
		if(driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE :
					if (msg.NOTIFY_ARG & irq_set)
					{
						//printf("entrou ciclo\n");
						scancode = read_scancode();
						if (scancode == 0x81) {
							esc_found = 1;
						}
						if (scancode == 0x48) //printf("FOUND SETA CIMA\n\n");
						{

						}
						if (scancode == 0x50) //printf("FOUND SETA BAIXO\n\n");
						{

						}
						if (scancode >> 7 == 1)
						{
							//printf("Breakcode: 0x%2x \n", scancode);
						}
						else printf("Makecode: 0x%2x \n\n",scancode);
					}
			}
		}

	}
	kbc_unsubscribe_int();
}

int test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
