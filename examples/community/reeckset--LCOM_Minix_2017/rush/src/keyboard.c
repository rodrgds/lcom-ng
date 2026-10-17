#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include "keyboard.h"
#include "i8042.h"

static int g_hookID_KBD = 4;



/*KEYBOARD FUNCTIONS*/

int kbd_get_key(unsigned short stopCode) {

	//subscribe keyboard interrupts
	int initialHookID = kbd_subscribe_int();

	if(initialHookID == -1) return 1;

	int ipc_status, r;
	message msg;

	int stopLoop = 1; //0 = true -|- 1 = false

	while(stopLoop == 1) {
		// Get a request message.
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;//continue while loop
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification
					if (msg.NOTIFY_ARG & BIT(initialHookID)) { // subscribed interrupt
						stopLoop = kbd_int_handler(stopCode);
					}
					break;
				default:
					break; // no other notifications expected: do nothing
			}
		} else { // received a standard message, not a notification
			// no standard messages expected: do nothing
		}
	}

	//unsubscribe keyboard interrupts
	kbd_unsubscribe_int();
	return 0;
}



int kbd_subscribe_int(void ) {

	int initialID = g_hookID_KBD;

	if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hookID_KBD) != OK ) return -1;

	sys_irqenable(&g_hookID_KBD);


	return initialID;
}

int kbd_unsubscribe_int() {

	sys_irqrmpolicy(&g_hookID_KBD);


	return 0;
}

int kbd_int_handler(unsigned short stopCode) {
	int stopLoop = 1;

	static int is2Byte= 1; //0 = true -|- 1 = false

	unsigned long bufferData;
	sys_inb(KBD_OUT_BUF, &bufferData);


	if( (unsigned short) bufferData == stopCode) {
		stopLoop = 0;
	}

	else if ((unsigned short) bufferData == 0xe0 && is2Byte == 1) {
		is2Byte = 0;
	} else {
		if(is2Byte == 1) {
			//normal scancode processing
			//processOutputCode((unsigned short) bufferData); //if no 0xe0 was received before, process the scancode normally
		}
		else {
			//if an 0xe0 was received before, the function must process the 2 bytes
			//processDualOutputCode((unsigned short) 0xe0, (unsigned short) bufferData);
			//reset flag value
			is2Byte = 1;
		}
	}
	return stopLoop;//return the stopping flag
}

//RETURNS SCANCODE FOR PRESSED KEY
unsigned long kbd_get_pressed_key() {

	static int is2Byte= 1; //0 = true -|- 1 = false

	unsigned long bufferData;
	sys_inb(KBD_OUT_BUF, &bufferData);

	if ((unsigned short) bufferData == 0xe0 && is2Byte == 1) {
		is2Byte = 0;
	} else {
		if(is2Byte == 1) {
			//normal scancode processing
			return (unsigned long) bufferData;
		}
		else {
			//if an 0xe0 was received before, the function must process the 2 bytes
			//reset flag value
			is2Byte = 1;
			return (0xe000 & bufferData);
		}
	}
	return 0;
}

void processOutputCode(unsigned short scancode) {
	if(scancode < 0x80) {
		printf("Makecode: 0x%x\n", scancode);
	} else {
		printf("Breakcode: 0x%x\n", scancode);
	}
}

void processDualOutputCode(unsigned short firstByte, unsigned short scancode) {
	if(scancode < 0x80) {
		printf("Makecode: 0x%x 0x%x\n", firstByte, scancode);
	} else {
		printf("Breakcode: 0x%x 0x%x\n", firstByte, scancode);
	}
}

void keyboardHandler(ev_queue_st* events, unsigned long scancode){
	//processOutputCode(scancode);
	event_st* eventToAdd = createEvent(KEY_PRESS);
	eventToAdd->keyCode = scancode;

	ev_queue_push(events, *eventToAdd);
	//free(eventToAdd);
}
