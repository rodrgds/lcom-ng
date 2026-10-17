#include "test5.h"

int hook_id_kbd = 0x01;
int hook_id_t0 = 0x00;
unsigned int cont_int = 0;

void *test_init(unsigned short mode, unsigned short delay) {

	int *l = vg_init(mode);
	sleep (delay);
	vg_exit();
	printf("%0x", l);
}

int kbd_subscribe_int(void) {
	if ((sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd))
			< 0)
		return -1;
	else
		return BIT(hook_id_kbd);
}

int kbd_unsubscribe_int(void) {
	int r = sys_irqrmpolicy(&hook_id_kbd);
	if (r != 0)
		return -1;
	else
		return r;
}

int kbd_int_handler() {
	unsigned long stat;
	unsigned long data;
	while (1) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		// loop while 8042 output buffer is empty
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}
int timer_subscribe_int(void) {
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_t0) < 0)
		return -1;
	sys_irqenable(&hook_id_t0);
	return BIT(hook_id_t0);
}

int timer_unsubscribe_int() {
	sys_irqdisable(&hook_id_t0);
	return (sys_irqrmpolicy(&hook_id_t0));
}

void timer_int_handler() {
	cont_int++;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	int *l = vg_init(LINEAR_FRAME);

	if (kbd_subscribe_int() == -1)
		return 1;

	int ipc_status, r;
	message msg;
	unsigned long kbd_irq_set = ~ hook_id_kbd;
	int end = 0;
	while (end == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & kbd_irq_set) {
					unsigned long data = kbd_int_handler();
					if (data == BC_ESC){
						end = 1;
						break;
					}
				}
				break;
			default:
				break;
			}
		}
		test_square_draw(x, y,size,color);
	}
	vg_exit();
	printf("ESC was pressed\n");
	if (kbd_unsubscribe_int() < 0)
		return -1;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {
	vg_init(LINEAR_FRAME);

	if (kbd_subscribe_int() == -1)
		return 1;

	int ipc_status, r;
	message msg;
	unsigned long kbd_irq_set = ~ hook_id_kbd;
	int end = 0;
	while (end == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & kbd_irq_set) {
					unsigned long data = kbd_int_handler();
					if (data == BC_ESC){
						end = 1;
						break;
					}
				}
				break;
			default:
				break;
			}
		}
		test_line_draw(xi, yi, xf,yf,color);
	}
	vg_exit();
	printf("ESC was pressed\n");
	if (kbd_unsubscribe_int() < 0)
		return -1;

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	vg_init(LINEAR_FRAME);

	if (kbd_subscribe_int() == -1)
		return 1;

	int ipc_status, r;
	message msg;
	unsigned long kbd_irq_set = ~ hook_id_kbd;
	int end = 0;

	int wd, ht;
	char *pix = read_xpm(xpm, &wd, &ht);
	test_xpm_show(xi, yi, wd, ht, pix);

	while (end == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & kbd_irq_set) {
					unsigned long data = kbd_int_handler();
					if (data == BC_ESC){
						end = 1;
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}


	vg_exit();
	printf("ESC was pressed\n");
	if (kbd_unsubscribe_int() < 0)
		return -1;


}

int test_move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time) {

	vg_init(LINEAR_FRAME);

	int r, ipc_status;
	message msg;
	int irq_set_kbd = ~hook_id_kbd;
	int irq_set_timer = ~hook_id_t0;
	if (kbd_subscribe_int() == -1)
		return 1;
	if (timer_subscribe_int() == -1)
		return 1;

	int wd,ht;
	char *pix = read_xpm(xpm,&wd,&ht);

	short xm = xi + wd;
	short ym = yi + ht;
	xpm_show(xi,yi,xm,ym,pix);

	double vlc = delta/(time*60);
	double dis = 0;

	int counter = 0;
	while (counter < time * 60) {

		//Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set_kbd) { // subscribed interrupt
					unsigned long data = kbd_int_handler();
					if (data == BC_ESC) {
						vg_exit();
						printf("ESC KEY PRESSED\n");
						kbd_unsubscribe_int();
						timer_unsubscribe_int();
						return 0;
					}

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					counter++;
					dis += vlc;
					if(dis < 1 || dis > -1){
						if (msg.NOTIFY_ARG & irq_set_kbd) { // subscribed interrupt
							unsigned long data = kbd_int_handler();
							vg_exit();
							printf("ESC KEY PRESSED\n");
							kbd_unsubscribe_int();
							timer_unsubscribe_int();
							return 0;
						}
					}

					clear_screen();
					if(hor == 0){
						yi += dis;
						xm = xi + wd;
						ym = yi + ht;
					}
					else{
						xi += dis;
						xm = xi + wd;
						ym = yi + ht;
					}
					if(xpm_show(xi,yi,xm,ym,pix) == -1){
						vg_exit();
						printf("LIMITS HAVE BEEN ACHIEVED\n");
						kbd_unsubscribe_int();
						timer_unsubscribe_int();
						return 0;
					}
					dis -= dis;
				}
				break;
			default:
				break; //no other notifications expected: do nothing
			}

		}
	}
	vg_exit();
	printf("Time has run out!\n");
	if (kbd_unsubscribe_int() < 0)
		return -1;
	else
		return kbd_unsubscribe_int();

}

int test_controller() {
/*
	vbe_info_block_t *vbe;
	vbe = malloc(sizeof(vbe_info_block_t));

	int16_t *modes = vbe_modes(vbe);


	//print controller capabilities as seen in VESA BIOS Extension manual
	if((vbe->Capabilities[0] & BIT(0)) == 0){
		printf("DAC width is fixed, with 6 bits per primary color\n");
	}
	else
		printf("DAC width is switchable to 8 bits per primary color\n");

	if((vbe->Capabilities[0] & BIT(1)) == 0){
		printf("Controller is VGA compatible\n");
	}
	else
		printf("Controller is not VGA compatible\n");

	if((vbe->Capabilities[0] & BIT(2)) == 0){
		printf("Normal RAMDAC operation\n");
	}
	else
		printf("When programming large blocks of information to the RAMDAC,use the blank bit in Function 09h\n");


	//print list of mode numbers (hexa) supported

	int16_t *ptr = modes;
	printf("List of modes:\n");
	while(*ptr != LAST_VBE_MODE){
		printf("0x%X\n", *ptr);
		ptr++;
		ptr++;
	}

	//print size of VRAM
	short vramsize = vbe->TotalMemory;
	prinft("VRAM Size: %u KB", vramsize);

	free(vbe);

	return 0;
*/
}

