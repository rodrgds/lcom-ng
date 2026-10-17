// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "ps2.h"
#include "mouse.h"

int hook_ps2_id = 12;

int (init_mouse)(uint8_t *bit_no){
	if (ps2_subscribe_int(bit_no) != 0)
	{
		printf("mouse_test_packet(49): Couldn't subscribe to mouse interrupts\n");
		return 1;
	}
	
	if(mouse_set_stream_mode() != 0){
		printf("mouse_test_packet(45): Couldn't set stream mode\n");
		return 1;
	}
	
	if(mouse_enable_data_rep() != 0){
		printf("mouse_test_packet(45): Couldn't enable data reporting\n");
		return 1;
	}
	
	
	
	if(ps2_ignore() != 0)
	{
		printf("mouse_test_packet(53): Failed ignoring ps2 buffer!\n");
		//return 1;
	}
	return 0;
}

int (ps2_subscribe_int)(uint8_t *bit_no){
    *bit_no = hook_ps2_id;
	if (sys_irqsetpolicy(PS2_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_ps2_id) != 0)
        return 1;
    paux = (packetData*)malloc(sizeof(packetData));
    memset(paux, 0, sizeof(packetData));
    counter = 0;
    return 0;
}

int(ps2_unsubscribe_int)(void)
{
    if (sys_irqrmpolicy(&hook_ps2_id) != 0)
        return 1;
    return 0;
}

int (ps2_ignore)() {
	uint32_t status, aux;
	
	if (sys_inb(KBC_CMD_REG, &status) != OK)
		return 1;
	
	if (status & OBF)
		sys_inb(OUT_BUF, &aux);
	
	return 0;

}

int (mouse_set_stream_mode)(void){
	if(mouse_write() == 0){
		if(mouse_send_argument(SET_STREAM_MODE) != 0){
			printf("%s: Failed sending argument!", __func__);
		}
		int a = mouse_cmd_receive();
		//printf("ret: %x\n", a);
		if(a != ACK){
			printf("Failed to enable data reporting!\n");
			return 1;
		}
		return 0;
	}
	return 2;
}

int (mouse_set_remote_mode)(void){
	if(mouse_send_command(WR_TO_PS2) == 0){
		if(mouse_send_argument(SET_REMOTE_MODE) != 0){
			printf("%s: Failed sending argument!", __func__);
		}
		if(mouse_cmd_receive() != ACK){
			printf("Failed to enable data reporting!\n");
			return 1;
		}
		return 0;
	}
	return 2;
}

int (mouse_write)(void){

	uint32_t stat;

	while(1)
	{
		sys_inb(KBC_CMD_REG, &stat); /*assuming it returns OK*/
		/*loop while 8042 input buffer is not empty*/
		if ((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, WR_TO_PS2);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

}

int (mouse_send_command)(unsigned long cmd){

	uint32_t stat;
	
	uint8_t i=0;

	while(i < 5)
	{
		sys_inb(KBC_CMD_REG, &stat); /*assuming it returns OK*/
		/*loop while 8042 input buffer is not empty*/
		if ((stat & IBF) == 0)
		{
			sys_outb(KBC_CMD_REG, cmd);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		++i;
	}
	return ERROR;
}

int (mouse_send_argument)(unsigned long cmd){

	uint32_t stat;
	
	uint8_t i=0;

	while(i < 5)
	{
		sys_inb(KBC_CMD_REG, &stat); /*assuming it returns OK*/
		/*loop while 8042 input buffer is not empty*/
		if ((stat & IBF) == 0)
		{
			sys_outb(OUT_BUF, cmd);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		++i;
	}
	return ERROR;
}

int (mouse_cmd_receive)(void){
	uint32_t stat = 0;
	uint32_t data = 0;
	//uint8_t i=0;

	//while(i < 5)
	//{
		//printf("\t\t%d\n", i);
		sys_inb(KBC_CMD_REG, &stat); /*assuming it returns OK*/
		/*loop while 8042 input buffer is not empty*/
		if (stat & OBF)
		{
			sys_inb(OUT_BUF, &data); /*assuming it returns OK*/
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return ERROR;
		}
		//tickdelay(micros_to_ticks(DELAY_US));
		//sleep(1);
	/*	++i;
	}*/
	return ERROR;
}
