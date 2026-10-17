#include <lcom/lcf.h>
#include "mouse.h"

#include <stdint.h>

#include "i8254.h"
#include "i8042.h"

int hookid_mouse = 2;

int (mouse_subscribe_int)(uint8_t *bit_no){

    *bit_no = hookid_mouse;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookid_mouse) != OK)
		return -1;  
    
    return 0;  
}

int(mouse_unsubscribe_int)(){
    if (sys_irqrmpolicy(&hookid_mouse) != OK) {
        return -1;
    }
    return 0;
}

int (mouse_enable_int)(){
	if (sys_irqenable(&hookid_mouse) != OK)
		return -1;

	return 0;
}

int (mouse_disable_int)(){   

	if (sys_irqdisable(&hookid_mouse) != OK)
		return -1;

	return 0;
}

uint32_t mouseData;
void (mouse_ih)(){
    
    uint32_t stat = 0;
	uint32_t data = 0;

	sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
	if( stat & OBF && stat & AUX) {
		sys_inb(OUT_BUF, &data); /* assuming it returns OK */
		if ((stat & (PAR_ERR | TO_ERR)) == 0 ){
			mouseData = data;
        }
		else
			mouseData = 0;
    return;
	}
}

uint32_t (read_kbc)(){
	int i = 0;
	uint32_t data;
	uint32_t stat;

	while (i < 8) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if (stat & OBF) {
			sys_inb(OUT_BUF, &data);

			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}
	return -1;

}

int (write_kbc_command)(uint8_t command){
	uint32_t stat = 0;
	int attempts = 10;

    while( attempts > 0 ) {
        sys_inb(STAT_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
        if( (stat & IBF) == 0 ) {
            sys_outb(KBC_CMD_REG, command); /* no args command */
			//printf("d4 to kbc_reg\n");
            return 0;
        }	
        tickdelay(micros_to_ticks(WAIT_KBC));
		attempts--;
    }
	return -1;
}

int (write_kbc_argument)(uint8_t argument){
	uint32_t stat = 0;
	int attempts = 10;
    while( attempts > 0 ) {
        sys_inb(STAT_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
        if( (stat & IBF) == 0 ) {
            sys_outb(IN_BUF, argument);
			//printf("arg sent\n");
            return 0;
        }	
        tickdelay(micros_to_ticks(WAIT_KBC));
		attempts--;
    }
	return -1;
}

int (write_kbc_cmd_byte)(uint32_t cmd_byte){
	if (write_kbc_command(KBC_WRITE_CMD) != 0)
		return -1;
	if (write_kbc_argument(cmd_byte) != 0)
		return -1;

	return 0;
}

int (write_mouse_command)(uint32_t command){
	if (write_kbc_command(MOUSE_WRITE_CMD) != 0)
		return -1;
	if (write_kbc_argument(command) != 0)
		return -1;

	return 0;
}


int (enable_data_report)(){
	uint8_t response;
	do {
		write_mouse_command(EN_DATA_REPORT);
		response = read_kbc();

		if (response == ERROR)
			return -1;
	}
	while (response != ACK);

	return 0;
}

int (disable_data_report)(){
	uint8_t response;
	do {
		write_mouse_command(DIS_DATA_REPORT);
		response = read_kbc();

		if (response == ERROR)
			return -1;
	}
	while (response != ACK);

	return 0;
}

int (set_stream_mode)(){
	uint8_t response;
	do {
		write_mouse_command(SET_STREAM);
		response = read_kbc();

		if (response == ERROR)
			return -1;
	}
	while (response != ACK);

	return 0;
}

int (read_mouse_data)(uint32_t *packet){
	uint32_t response;
	do {
		write_mouse_command(READ_DATA);
		response = read_kbc();
	
		if ((uint8_t)response == ERROR)
			return -1;
	}
	while ((uint8_t)response != ACK);

	

	packet[0] = read_kbc();
	packet[1] = read_kbc();
	packet[2] = read_kbc();

	return 0;
}


static enum state st = INIT;
static uint8_t x_len = 0;

void updateState(enum event_type ev){
		
	switch (st)
	{
		case INIT:
			if (ev == LBDOWN)
				st = DRAWINGUP;
			break;

		case DRAWINGUP:
			if (ev == MOVEUP)
				st = DRAWINGUP;
			else if (ev == LBUP)
				st = VERTEX;
			else st = INIT;
			break;

		case VERTEX:
			if (ev == RESIDUAL)
				st = VERTEX;
			else if (ev == RBDOWN)
				st = DRAWINGDOWN;
			else if (ev == LBDOWN)
				st = DRAWINGUP;
			else st = INIT;
			break;
		
		case DRAWINGDOWN:
			if (ev == MOVEDOWN)
				st = DRAWINGDOWN;
			else if (ev == RBUP){
				st = FINAL;
			}
			else st = INIT;

			break;

		default:
			break;
	}
}



bool (event) (struct packet *pp , uint8_t x, uint8_t tolerance){ 
	enum event_type evt = OTHER;

	if (pp->lb == 1 && pp->mb + pp->rb == 0)   //////// LBDOWN ////////
		if(pp->delta_x + pp->delta_y == 0){
			evt = LBDOWN;
			updateState(evt);
			return false;
		}

	if (pp->lb == 1 && pp->mb + pp->rb ==0)    /////// MOVEUP /////// 
		if (pp->delta_x > 0 && pp->delta_y > 0)
			if ((float)pp->delta_y / pp->delta_x > 1)
				if (pp->delta_x < tolerance && pp->delta_y < tolerance) {
					evt = MOVEUP;
					x_len += pp->delta_x;
					updateState(evt);
					return false;
				}

	if (st != DRAWINGDOWN)
		if (pp->lb == 0 && pp->mb + pp->rb ==0)   /////// LBUP ///////
			if(pp->delta_x + pp->delta_y == 0 && x_len >= x){
				x_len = 0;
				evt = LBUP;
				updateState(evt);
				return false;
			}

	if (st == VERTEX)
		if (pp->lb + pp->mb + pp->rb == 0)
			if (pp->delta_x != 0 || pp->delta_y != 0)
				if (pp->delta_x <= tolerance && pp->delta_y <= tolerance)
					evt = RESIDUAL;


	if (pp->rb == 1 && pp->mb + pp->lb == 0)   //////// RBDOWN ////////
		if(pp->delta_x + pp->delta_y == 0){
			evt = RBDOWN;
			updateState(evt);
			return false;
		}

	if (pp->rb == 1 && pp->mb + pp->lb == 0)   //////// MOVEDOWN ////////
		if(pp->delta_x > 0 && pp->delta_y < 0)
			if (abs((float)pp->delta_y / pp->delta_x) > 1)
				if (pp->delta_x < tolerance && pp->delta_y < tolerance){
					x_len += pp->delta_x;
					evt = MOVEDOWN;
					updateState(evt);
					return false;
				}

	if (pp->rb == 0 && pp->mb + pp->lb ==0)   /////// RBUP ///////
		if(pp->delta_x + pp->delta_y == 0 && x_len >= x){
			evt = RBUP;
		}

	x_len = 0;
	updateState(evt); // Updates in case the state is RBUP OR OTHER

	if (st == FINAL)
		return true;

	return false;
}

