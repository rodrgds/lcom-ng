/** @file dispatcher.c
 * @brief file containing the definitions of the functions for subscribing interruptions and creating interruption loops
 */

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include "dispatcher.h"

int subscribe_int(int irq_line, int policy, int *hook_id, unsigned short enable) {
	int irq_set;
	irq_set = 0;
	irq_set |= (1 << *hook_id);						/* save hook_id bit mask for interrupt notification */
	if (sys_irqsetpolicy(irq_line, policy, hook_id) != 0) {
		printf("subscribe_int: sys_irqsetpolicy failed!\n");
		return -1;
	}
	if (enable) {
		if (sys_irqenable(hook_id) != 0) {
			printf("subscribe_int: sys_irqenable failed!\n");
			return -1;
		}
		return irq_set;
	}
	if (sys_irqdisable(hook_id) != 0) {
		printf("subscribe_int: sys_irqdisable failed!\n");
		return -1;
	}
	return irq_set;
}

int unsubscribe_int(int *hook_id) {
    if (sys_irqdisable(hook_id) != 0) {
        printf("unsubscribe_int: sys_irqdisable failed!\n");
        return 1;
    }
    if (sys_irqrmpolicy(hook_id) != 0) {
        printf("unsubscribe_int: sys_irqrmpolicy failed!\n");
        return 1;
    }
    return 0;
}

void interrupt_loop(int irq_set, Handler int_handler, Handler app_handler, long *arg) {
	unsigned short stop = 0;
	int r;
	message msg;
	int ipc_status;
	char event;
	while (stop == 0) {
		event = 0;
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {		/* get a request message */
			printf("interrupt_loop: driver_receive failed with %d!\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 								/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: 											/* hardware interrupt notification */
					if ((msg.NOTIFY_ARG) & irq_set) { 					/* subscribed interrupts */
						event = 1;
						int_handler(msg.NOTIFY_ARG, arg);				/* handle interruption and update stop condition */
                    }
					break;
				default:
					break; 												/* no other notifications expected: do nothing */
			}
		} else {
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
        }
		if (event) {
			stop = app_handler(msg.NOTIFY_ARG, arg);
		}
    }
}
