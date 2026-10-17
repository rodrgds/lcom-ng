#include <minix/syslib.h>
#include <minix/drivers.h>


#include "ModPortaSerie.h"


void changeDLAB(unsigned short base_addr,int bit){

	unsigned long LCRbits;

	sys_inb(base_addr + LCR, &LCRbits);

	if(bit == 1) LCRbits = LCRbits|BIT(7);
	else LCRbits = LCRbits & (~BIT(7));

	sys_outb(base_addr + LCR, LCRbits);
}


void setPorta(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate){


	unsigned long line_control;


	line_control |=  (~BIT(0));//por a 0
	line_control &=  (~BIT(1));//por a 0
	if (bits == 6 || bits == 8){
		line_control |=  (BIT(0));//por a 1
	}
	if (bits == 7 || bits == 8){
		line_control |=  (BIT(1));//por a 1
	}


	line_control &= (~BIT(2)); //por a 0
	if (stop == 2)
		line_control |= (BIT(2)); //por a 1



	if (parity == -1) {
		line_control &= (~BIT(3));//por a 0
	} else {
		line_control |= (BIT(3)); //por a 1

		line_control &= (~BIT(5));//por a  0 porque parity so pode ser none / odd / even


		if (parity == 0)
			line_control |=(BIT(4));//por a 1

		else if (parity == 1)
			line_control &= (~BIT(4));//por a 0
	}

	sys_outb(base_addr + LCR, line_control);



	unsigned long Divisor_Latch = 0;
	Divisor_Latch = UART_FREQ / rate;

	changeDLAB(base_addr, 1);
	sys_outb(base_addr + DLM, (Divisor_Latch >> 8));

	sys_outb(base_addr + DLL, Divisor_Latch);
	changeDLAB(base_addr, 0);

}


int getChar(unsigned short base_addr, unsigned char *c) {
	unsigned long temp1, temp2;

	while (1) {

		sys_inb(base_addr + LSR, &temp1);
		if ((temp1 & BIT(RR))!=0) {
			sys_inb(base_addr + RB, &temp2);
			*c = temp2;
			return 0;
		}

		tickdelay(micros_to_ticks(20));
	}
}

int getCharOne(unsigned short base_addr, unsigned char *c) {
	unsigned long temp1,temp2;

	sys_inb(base_addr + LSR, &temp1);
	if ((temp1 & BIT(RR))!=0) {
		sys_inb(base_addr + RB, &temp2);
		*c = temp2;
		return 0;
	}
	else
		return 1;
}

int sendChar(unsigned short base_addr,char c) {
	unsigned long temp;

	while(1) {
		sys_inb(base_addr+LSR, &temp);
		if (temp & THRE) {
			sys_outb(base_addr + TH, c);
			return 0;
		}
		printf("loop no sendchar\n");
		tickdelay(micros_to_ticks(20));

	}
}


