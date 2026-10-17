#include "mouse.h"

int mouse_hook_id = 2;
uint8_t mouse_info;

int coordinate_x_mouse = 625;
int coordinate_y_mouse = 550;

void updateMousePosition(struct packet *pp){
	
	coordinate_x_mouse += pp->delta_x;
	coordinate_y_mouse -= pp->delta_y;

	if(coordinate_x_mouse < 0) coordinate_x_mouse = 0;

	if(coordinate_x_mouse > 1228) coordinate_x_mouse = 1228;

	if(coordinate_y_mouse < 0) coordinate_y_mouse = 0;

	if(coordinate_y_mouse > 945) coordinate_y_mouse = 945;

}

void (mouse_ih)() {
	uint8_t status;

	if (util_sys_inb(COMMAND_REGISTER, &status) != 0) // Read status
		return;

	if ((status & OBF) != 0) {
		if (((status & PARITY_ERROR) != 0) || ((status & TIMEOUT_ERROR) != 0)) {
			printf("Parity or timeout error\n");
			return;
		}

		if (util_sys_inb(OUT_BUF, &mouse_info) != 0) {
			printf("Could not read info\n");
			return;
		}
	}
	return;
}

int mouse_subscribe_int(uint8_t* bit_no) {
	*bit_no = BIT(mouse_hook_id);

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0)
		return 1;

	return 0;
}

int mouse_unsubscribe_int() {
	if (sys_irqrmpolicy(&mouse_hook_id) != 0)
		return 1;

	return 0;
}

int parse_packet(struct packet *packet) {
	uint8_t xmsb, ymsb;
	
	if ((packet->bytes[0] & BIT(0)) != 0)
		packet->lb = true;
	else
		packet->lb = false;

	if ((packet->bytes[0] & BIT(1)) != 0)
		packet->rb = true;
	else
		packet->rb = false;

	if ((packet->bytes[0] & BIT(2)) != 0)
		packet->mb = true;
	else
		packet->mb = false;

	if ((packet->bytes[0] & BIT(4)) != 0)
		xmsb = 1;
	else
		xmsb = 0;

	if ((packet->bytes[0] & BIT(5)) != 0)
		ymsb = 1;
	else
		ymsb = 0;


	if ((packet->bytes[0] & BIT(6)) != 0)
		packet->x_ov = true;
	else
		packet->x_ov = false;

	if ((packet->bytes[0] & BIT(7)) != 0)
		packet->y_ov = true;
	else
		packet->y_ov =  false;


	int16_t deltax = conversion(packet->bytes[1], xmsb);
	int16_t deltay = conversion(packet->bytes[2], ymsb);

	packet->delta_x = deltax;
	packet->delta_y = deltay;	

	return 0;
}

int16_t conversion(uint8_t value, uint8_t msb)
{
	if (msb == 1)
		return value - 256;
	else
		return value;
}

void mouse_polling() {
	struct packet packet;
	uint8_t status;

	if (util_sys_inb(COMMAND_REGISTER, &status) != 0) { // Read status
		printf("Sys_inb failed\n");
		return;
	}

	if ((status & OBF) != 1) {
		printf("Output buffer needs to be set\n");
		return;

	}

	if (((status & PARITY_ERROR) != 0) || ((status & TIMEOUT_ERROR) != 0)) {
		printf("Parity or timeout error\n");
		return;
	}
		
	for (size_t i = 1; i < 4; i++)
	{
		if (util_sys_inb(OUT_BUF, &mouse_info) != 0) {
			printf("Could not read info\n");
			return;
		}

		if (i == 1) {
			if ((mouse_info & BIT(3)) != 0)
				packet.bytes[0] = mouse_info;
		}

		else if (i == 2) {
			packet.bytes[1] = mouse_info;
		}

		else if (i == 3) {
			packet.bytes[2] = mouse_info;
		}
	}

	parse_packet(&packet);
	mouse_print_packet(&packet);
}

int write_command(uint8_t command) {
	uint8_t status;
	uint8_t ack;
	
	while (true) { // If ack is 0xFE, it just ignores it and repeats the loop, then the next ack decides if valid or not
		if (util_sys_inb(COMMAND_REGISTER, &status) != 0)
			return 1;

		if (((status & BIT(1)) == 0) && ((status & BIT(5)) == 0)) {			
			if (sys_outb(COMMAND_REGISTER, 0xD4) != 0)
				return 1;
		}
		
		if (util_sys_inb(COMMAND_REGISTER, &status) != 0)
			return 1;
	
		if (((status & BIT(1)) == 0) && ((status & BIT(5)) == 0)) {
			if (sys_outb(OUT_BUF, command) != 0)
				return 1;
		}

		if (util_sys_inb(OUT_BUF, &ack) != 0)
			return 1;

		if (ack == 0xFA) // We good
			return 0;

		if (ack == 0xFC) // Fail
			return 1;
	}

	return 0;
}
