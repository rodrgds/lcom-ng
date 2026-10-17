#include <lcom/lcf.h>
#include "controller.h"
#include "mouse.h"
#include "kbd.h"
#include "rtc.h"


void (controller) (int timer_irq, int mouse_irq, int kbd_irq, int rtc_irq){
    int r, ipc_status;
	message msg;
    
	while(true) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ){
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.m_notify.interrupts & mouse_irq) {
						mouse_ih();
					}
                    if (msg.m_notify.interrupts & timer_irq){
                        timer_int_handler();
                    }
                    if (msg.m_notify.interrupts & kbd_irq){
                        kbc_ih();
                    }
					if (msg.m_notify.interrupts & rtc_irq){
                        rtc_ih();
                    }
					return;
				default:
					break;
			}
		}
		else{
		}
	}
}
