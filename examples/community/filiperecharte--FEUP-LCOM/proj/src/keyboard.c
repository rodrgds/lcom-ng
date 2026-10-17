#include "keyboard.h"

int keyboard_hook_id = 1;
uint32_t key_pressed;

int keyboard_subscribe_int(uint8_t *bit_no) {
	*bit_no = BIT(keyboard_hook_id);

	if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0)
		return 1;
	
	return 0;
}

int keyboard_unsubscribe_int() {
	if (sys_irqrmpolicy(&keyboard_hook_id) != 0)
		return 1;

	return 0;
}

void (kbc_ih)(void) {	
	uint32_t status;
	
	if(util_sys_inb_counter(COMMAND_REGISTER, &status) != 0) // Read status
		return;

	if ((status & OBF) != 0) {
		if (((status & PARITY_ERROR) != 0) || ((status & TIMEOUT_ERROR) != 0)) {
			printf("Parity or timeout error\n");
			return;			
		}

		if (util_sys_inb_counter(OUT_BUF, &key_pressed) != 0) {
			printf("Could not read key\n");
			return;
		}
	}
}

void get_info(bool* is_makecode, uint8_t* scancode_size, uint8_t scancode_bytes[], bool *one_byte)
{
	if (*one_byte) {
		if (key_pressed & BIT(7))
			*is_makecode = false;
		else
			*is_makecode = true;

		*scancode_size = 1;
		scancode_bytes[0] = (uint8_t)key_pressed;
	}
	else
	{
		if (key_pressed & BIT(7))
			*is_makecode = false;
		else
			*is_makecode = true;
		
		*scancode_bytes = 2;

		scancode_bytes[0] = 0xE0;
		scancode_bytes[1] = (uint8_t)key_pressed;
	}
}

int kbd_reenable() {
	uint32_t status;

	if (sys_outb(COMMAND_REGISTER, 0x20) != 0)
		return 1;
	if (util_sys_inb_counter(OUT_BUF, &status) != 0)
		return 1;

	if ((status & BIT(1)) != 0) { // REVER ISTO
		printf("IBF FULL\n");
		return 1;
	}

	status = status | BIT(0); // Enable keyboard interrupt

	if (sys_outb(COMMAND_REGISTER, 0x60) != 0)
		return 1;


	if (sys_outb(IN_BUF, status) != 0)
		return 1;

	return 0;
}
