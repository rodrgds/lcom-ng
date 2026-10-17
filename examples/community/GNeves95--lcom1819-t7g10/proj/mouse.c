// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"
#include "ps2.h"

int (mouse_enable_data_rep)(void){
	if(mouse_write() == 0){
		mouse_send_argument(EN_DATA_REP);
		int ret = mouse_cmd_receive();
		//printf("ret: %x\n", ret);
		if(ret == NACK){
			//printf("Failed to enable data reporting!\n");
			return mouse_enable_data_rep();
		}
		else if(ret == ACK){
			return 0;
		}
		else{
			printf("Failed to enable data reporting!\n");
			return 1;
		}
		return 0;
	}
	return 2;
}
 
 
void (mouse_ih)(void){
	uint8_t ms_byte = mouse_cmd_receive();
	if(ms_byte == ERROR) counter = 0;
	
	else if (counter == 0 && ((ms_byte & BIT(3)))) {
		paux->bytes[counter] = ms_byte;
		paux->lb = BIT(0)&ms_byte;
		paux->rb = BIT(1)&ms_byte;
		paux->mb = BIT(2)&ms_byte;
		paux->x_sign = BIT(4)&ms_byte;
		paux->y_sign = BIT(5)&ms_byte;
		paux->x_ov = BIT(6)&ms_byte;
		paux->y_ov = BIT(7)&ms_byte;
		counter++;
	}
	else if (counter == 1) {
		if (paux->x_sign)
			paux->delta_x = -COMPLEMENT(ms_byte);
		else
			paux->delta_x = ms_byte;
		counter++;
	}
	else if (counter == 2) {
		if (paux->y_sign)
			paux->delta_y = -COMPLEMENT(ms_byte);
		else
			paux->delta_y = ms_byte;
		counter++;
	}
	else
		counter = 0;
}
