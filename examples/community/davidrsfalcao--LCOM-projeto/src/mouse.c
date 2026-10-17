#include "mouse.h"
#include "i8042.h"
#include "keyboard.h"

int mouse_subscribe_int(){
	int bitmask = BIT(hook_id_mouse);
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) == OK
			&& sys_irqenable(&hook_id_mouse) == OK){
		memset(packet, 0, sizeof(packet));
		byte_number = 0; // set all bytes bit 3 to 0 and byte_number (index of byte in packet of 3 bytes) to 0
		return bitmask;
	}
	else
		return EXIT_FAILURE;
}

int mouse_unsubscribe(){
	if (sys_irqdisable(&hook_id_mouse) == OK && sys_irqrmpolicy(&hook_id_mouse) == OK)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}



