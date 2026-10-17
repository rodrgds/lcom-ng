#include <minix/syslib.h>
#include <minix/drivers.h>
#include "kbd.h"

static unsigned int intcounter = 0;

int kbd_subscribe_int(void ) {

	sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbdhook);
	sys_irqenable(&kbdhook);

	return 1;
}

int kbd_unsubscribe_int() {

	sys_irqrmpolicy(&kbdhook);
	sys_irqdisable(&kbdhook);

	return 1;
}

unsigned char kbd_readscancode() {

	 unsigned long data, stat;

	 while( 1 ) {
		 sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		 /* loop while 8042 output buffer is empty */
		 if( stat & OBF ) {
			 sys_inb(OUT_BUF, &data); /* assuming it returns OK */

			 if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				 return (unsigned char)data;
			 else return -1;
		 }

		 tickdelay(micros_to_ticks(DELAY_US));
	 }
}
