#include <minix/syslib.h>
#include <minix/drivers.h>
#include "timer.h"
#include "general.h"
int hook_id_timer=0;
int segundos = 0;


int configure_timer(unsigned timer, unsigned acess, unsigned mode, unsigned bcd) {
	unsigned long byte;//                              normally 3

	if (timer == 0)
		byte = (byte & uBIT(SEL_COUNTER2)) & uBIT(SEL_COUNTER1); //TIMER0
	else if (timer == 1)
		byte = (byte & uBIT(SEL_COUNTER2)) | BIT(SEL_COUNTER1); //TIMER1
	else if (timer == 2)
		byte = (byte | BIT(SEL_COUNTER2)) & uBIT(SEL_COUNTER1); //TIMER2

	if (acess == 0)
		byte = (byte & uBIT(ACESS2)) | BIT(ACESS1); //LSB
	else if (acess == 1)
		byte = (byte | BIT(ACESS2)) & uBIT(ACESS1); //MSB
	else if (acess == 2)
		byte = (byte | BIT(ACESS2)) | BIT(ACESS1); //LSB -> MSB

	if (mode == 0)
		byte = ((byte & uBIT(OPMODE3)) & uBIT(OPMODE2)) & uBIT(OPMODE1); //mode 0
	else if (mode == 1)
		byte = ((byte & uBIT(OPMODE3)) & uBIT(OPMODE2)) | BIT(OPMODE1); //mode 1
	else if (mode == 2)
		byte = ((byte & uBIT(OPMODE3)) | BIT(OPMODE2)) & uBIT(OPMODE1); // mode 2
	else if (mode == 3)
		byte = ((byte & uBIT(OPMODE3)) | BIT(OPMODE2)) | BIT(OPMODE1); // mode 3
	else if (mode == 4)
		byte = ((byte | BIT(OPMODE3)) & uBIT(OPMODE2)) & uBIT(OPMODE1); // mode 4
	else if (mode == 5)
		byte = ((byte | BIT(OPMODE3)) & uBIT(OPMODE2)) | BIT(OPMODE1); //mode 5

if(bcd == 1)
	byte= byte | BIT(CNTMODE);
else
	byte= byte & uBIT(CNTMODE);

sys_outb(TIMER_CTRL,byte);

return 0;
}



int subscribe_timer() {

	int irqf = 0;

	int irq_set = BIT(hook_id_timer);
	if (sys_irqsetpolicy(irqf, IRQ_REENABLE, &hook_id_timer) != OK
			|| sys_irqenable(&hook_id_timer) != OK)
		return -1;
	else
		return irq_set;
}

int unsubscribe_timer() {
	if (sys_irqdisable(&hook_id_timer) != OK) {
		printf("IRQDISABLE falhou /n");
		return -1;
	}
	if (sys_irqrmpolicy(&hook_id_timer) != OK) {
		printf("IRQRMPOLICY falhou /n");
		return -1;
	}
	return 0;
}

void set_frq(){
	int freq=60;
		unsigned long byte = TIMER_FREQ / freq;
		sys_outb(TIMER_0, (byte & 0xFF));
		sys_outb(TIMER_0, (byte >> 8));
}

void IncSeg(){
	segundos++;
}
int GetSeg(){
	return segundos;
}
