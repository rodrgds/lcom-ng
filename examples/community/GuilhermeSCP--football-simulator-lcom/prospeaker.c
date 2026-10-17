#include <minix/syslib.h>
#include <minix/drivers.h>

#include "prospeaker.h"

int speaker_ctrl(unsigned char on) {
    unsigned long *speaker;
    speaker = (unsigned long*)malloc(sizeof(unsigned long));

		if(sys_inb(0x061,speaker)!=OK){
			return 1;
		}
		if(on!=0)
			*speaker|=0x02;
		else
			*speaker&=0xFD;

		sys_outb(0x061,*speaker);
		return 0;
}

int speaker_timer_ctrl(unsigned char on) {
	unsigned long *speaker;
	speaker = (unsigned long*)malloc(sizeof(unsigned long));
		if(sys_inb(0x061,speaker)!=OK){
			return 1;
		}
		if(on!=0)
			*speaker|=0x01;
		else
			*speaker&=0xFE;

		sys_outb(0x061,*speaker);
		return 0;
}
