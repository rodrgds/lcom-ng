#include <lcom/lcf.h>
#include "mouse.h"

#include <stdint.h>

#include "i8042.h"

static int hook_id = 4;
struct packet pp;
int bytes_received = 0;

extern unsigned hres, vres;

int x_init = 0;
int y_init = 0;

int(track_mouse_movement)() {
	if (pp.x_ov || pp.y_ov) {
		return 1;
	}

	x_init += pp.delta_x;
	y_init -= pp.delta_y;

	if (x_init < 0) x_init = 0;
	if (y_init < 0) y_init = 0;
	if (hres && x_init >= (int)hres) x_init = (int)hres - 1;
	if (vres && y_init >= (int)vres) y_init = (int)vres - 1;

	return 0;
}

int (check_status) (uint8_t st) {
    if(st & PARITY)
        return PARITY_ERR;

    if(st & TIME_OUT) 
        return TIME_OUT_ERR;

    if(st & AUX)
        return AUX_DATA;

    return OK;
}

void(mouse_ih)() {
	uint8_t st;
	uint8_t mouse_byte;

	for (int attempt = 0; attempt < 3; attempt++) {
		if (util_sys_inb(STATUS_REG, &st) != 0) {
			printf("Error: Status not available!\n");
			continue;
		}
		
		if ((st & BIT(0)) != 0) {

			if (util_sys_inb(OUT_BUF, &mouse_byte) != 0) {
				printf("Error: Failed to read from OUT_BUF!\n");
				continue;
			}

			if (check_status(st) != AUX_DATA) {
				printf("Error: Status check failed!\n");
				continue;
			}
			//printf("DEBUG: mouse_byte = %x\n", mouse_byte);

			if (bytes_received == 0 && (mouse_byte & BIT(3)) != 0) {
				//printf("DEBUG: first byte = 0x%02X\n", mouse_byte);
				pp.bytes[0] = mouse_byte;
				pp.x_ov = (mouse_byte & BIT(6)) >> 6;
				pp.y_ov = (mouse_byte & BIT(7)) >> 7;
				pp.lb = (mouse_byte & BIT(0));
				pp.rb = (mouse_byte & BIT(1)) >> 1;
				pp.mb = (mouse_byte & BIT(2)) >> 2;
				bytes_received++;
				break;
			}
			else if (bytes_received == 1) {
				//printf("DEBUG: second byte = 0x%02X\n", mouse_byte);
				pp.bytes[1] = mouse_byte;
				pp.delta_x = (pp.bytes[0] & BIT(4)) ? (pp.bytes[1] | 0xFF00) : pp.bytes[1];	//takes into consideration the sign bit in first byte
				bytes_received++;
				break;
			}
			else if (bytes_received == 2) {
				//printf("DEBUG: third byte = 0x%02X\n", mouse_byte);
				pp.bytes[2] = mouse_byte;
				pp.delta_y = (pp.bytes[0] & BIT(5)) ? (pp.bytes[2] | 0xFF00) : pp.bytes[2]; //takes into consideration the sign bit in first byte
				bytes_received++;
				break;
			}
		}
		
		tickdelay(micros_to_ticks(DELAY_US));
	}
	//printf("DEBUG: bytes_received = %d\n", bytes_received);
}



int(mouse_unsubscribe_int)() {

	return sys_irqrmpolicy(&hook_id);
}

int(mouse_subscribe_int)(uint8_t *bit_n){

    *bit_n = hook_id;

    return sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id);
}
