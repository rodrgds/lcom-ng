#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"


int speaker_ctrl(unsigned char on) {
	if(on==0) //disable speaker
	{
		if(sys_outb(SPEAKER_CTRL,0x00)!= OK)
			return -1;
	}
	else //enable speaker
	{
		if(sys_outb(SPEAKER_CTRL,0x03)!= OK)
			return -1;
	}
	return 0;
}

int play_note(unsigned long freq, unsigned long time) {

	if(speaker_ctrl(1)!=0)
	{
		printf("Speaker_ctrl Failed /n");
		return -1;
	}

	if(timer_set_square(2,freq)!=0)
	{
		printf("Timer_set_square Failed /n");
		return -1;
	}

	if(timer_test_int(time)!=0)
	{
		printf("Timer_Test_Int Failed /n");
		return -1;
	}

	if(speaker_ctrl(0)!=0)
	{
		printf("Speaker_ctrl Failed /n");
	}
	return 1;
}

