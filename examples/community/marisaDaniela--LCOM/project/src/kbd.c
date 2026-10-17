#include "utils.h"
#include "i8042.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"


int g_hook2 = 1;
int g_counter2 = 0;

// subscribe interruption
int print(unsigned short scancode);

int kbd_subscribe_int() {
	int bitmask = BIT(g_hook2);

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hook2) != 0) {
		printf("System call sys_irqsetpolicy() failed! \n");
		return -1;
	}

	if (sys_irqenable(&g_hook2) != 0) {
		printf("System call sys_irqenable() failed! \n");
		return -1;
	}

	return bitmask;

}

// unsubscribe interruption

int kbd_unsubscribe_int() {
	unsigned long in = 0;
	sys_inb(OUT_BUF, &in);

	if (sys_irqdisable(&g_hook2) != 0) {
			return -1;
	}
	if (sys_irqrmpolicy(&g_hook2) != 0) {
		return -1;
	}


	return 0;

}


// Function to read a key from the keyboard to be used by the function
// kbc_handler that prints makecode and breakcode

int kbc_read_key(unsigned char *key) {

	unsigned long key_tmp;
	unsigned long condition = 0;

	while(1) {
		sys_inb(STAT_REG, &condition);

		if ((condition & OUT_BUF_FULL) != 0) {
			if (sys_inb(OUT_BUF, &key_tmp) != OK) {
				return -1;
			}
			break;
		}
		tickdelay(micros_to_ticks(DELAY));
	}

	*key = (unsigned char) key_tmp;

	return 0;
}

// function to prints makecode and breakcode
// checks if the scancode is breakcode or makecode

unsigned short kbd_handler_c() {
	unsigned char key;
	unsigned short scancode;

	if(kbc_read_key(&key) != OK) {
		return -1;
	}

	if(key == 0xe0) {				// The codes e0 and e1 introduce scancode sequences.
		scancode = key << 8; 		// Apart from the Pause/Break key, that has an escaped sequence starting with e1, the escape used is e0. Often, // the codes are chosen in such a way that something meaningful happens when the receiver just discards the e0.

		if (kbc_read_key(&key) != OK) {
			return -1;
		}
		scancode |= key;
	}
	else
		scancode = key;

	print(scancode);
	return scancode;
}

int print(unsigned short scancode){
	if(scancode & BREAK_CODE_BIT)
		printf("BREAKCODE: 0x%x\n", scancode);
	else
		printf("MAKECODE:  0x%x\n", scancode);
	return scancode;
}

// Se o argumento for 1, vai resolver a funcao test_scan em assembly.
// Se for 0, vai resolver em C - using interrupts

int kbd_test_scan(unsigned short assembly) {
	int ipc_status, r;
	message msg;
	int irq_set = kbd_subscribe_int();
	unsigned char key;

	if(irq_set == -1) {
		printf("erros subscribing kbd interrupts..\n");
		return -1;
	}

	while(key != ESC) {
		if (driver_receive(ANY, &msg, &ipc_status) != OK) {
			printf("kbd_test_scan()::driver_receive failed with: %d\n",r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {

			//received notification
			case HARDWARE:

				//hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set) {
					if (assembly) {
						printf("We need to see this\n");
						//key = kbd_handler_assembly();
						//print(key);
					} else {
						key = kbd_handler_c();
					}
					if (key == ESC) {
						printf("kbd_test_scan()::ESC key was pressed! Press ENTER to continue! \n");
					}
				}
				break;
			}
		}
	}

	return kbd_unsubscribe_int(); // unsubscribe

}

// Don't use interrupts.
// The processor is always busy to polling the status register (0x64), and read the OB, if
// OBF is set and AUX is cleared

int kbd_test_poll() {
	unsigned char key = 0;
	unsigned short scancode;

	while(key != ESC)
	{
		if(kbc_read_key(&key) != OK) {
			return -1;
		}

		if(key == 0xe0) {				// The codes e0 and e1 introduce scancode sequences.
			scancode = key << 8; 		// Apart from the Pause/Break key, that has an escaped sequence starting with e1, the escape used is e0. Often, // the codes are chosen in such a way that something meaningful happens when the receiver just discards the e0.

			if (kbc_read_key(&key) != OK) {
				return -1;
			}
			scancode |= key;
		}
		else
			scancode = key;

		if(scancode & BREAK_CODE_BIT)
			printf("BREAKCODE: 0x%x\n", scancode);
		else
			printf("MAKECODE:  0x%x\n", scancode);

	}

	printf("kbd_test_scan()::ESC key was pressed! Enable kbc_cmdr to continue! \n");

}

// similar to test_scan, but in this function we use timer to count the time
// and passing n seconds is not possible to print more make/breakcodes

int kbd_test_timed_scan(unsigned short n) {
	int ipc_status, r;
	message msg;

	int irq_set = kbd_subscribe_int(); 				// to subscribes kbd
	int irq_set_timer = timer_subscribe_int();		// to subscribes timer

	unsigned char key;

	if(irq_set == -1) {
		printf("error subscribing kbd interrupts..\n");
		return -1;
	}
	if(irq_set_timer == -1) {
		printf("error subscribing timer interrupts..\n");
		return -1;
	}

	/* your program should exit not only when the user releases
	 * the ESC key, but also if it does not receive a scancode
	 * for a number seconds equal to its argument, n
	 */

	while(key != ESC && g_counter2 < 60 * n ) {
		if (driver_receive(ANY, &msg, &ipc_status) != OK) {
			printf("kbd_test_timed_scan()::driver_receive failed with: %d\n",r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {

			//received notification
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer)
				{
					timer_int_handler();				// subscrever o timer
					if (g_counter2>= n * 60)
					{
						printf("\nkbd_test_timed_scan()::timed out!\nPress ENTER to continue!\n");  // acabou o tempo
					}
				}
				if (msg.NOTIFY_ARG & irq_set) 			// subscrever o kbd
				{
					key = kbd_handler_c();
					if (key == ESC)
					{
						printf("kbd_test_scan()::ESC key was pressed! Press ENTER to continue! \n");
					}
				}
				break;
			}
		}
	}

	timer_unsubscribe_int();
	return kbd_unsubscribe_int(); // unsubscribe
}
