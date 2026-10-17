#include "timer.h"

int hook_id_t0 = 0;
int cont_int = 0;

int timer_int(unsigned long time) {
	int ipc_status;
	message msg;
	int r;
	int irq_set = ~hook_id_t0;
	timer_subscribe_int();
	while( 1 ) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set) {

					cont_int++;
					if((cont_int % 60) == 0){
						printf("One second\n");
						time--;
					}
					if(time == 0){
						timer_unsubscribe_int();
						printf("int: %u", cont_int);
						return 0;
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	return 1;
}
