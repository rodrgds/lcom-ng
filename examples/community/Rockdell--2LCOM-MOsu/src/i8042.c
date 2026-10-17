#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

int getResponse(unsigned long *response) {
	int valid = sys_inb(KBC, response);

	if(valid != OK)
		return 1;
	else
		return 0;
}

int getStatus(unsigned long *status) {
	int valid = sys_inb(KBC_STATUS, status);

	if(valid != OK)
		return 1;
	else
		return 0;
}

int writeCommand(unsigned long command) {
	int nrIter = 0;

	while(nrIter < 10)
	{
		nrIter++;

		//Get status
		unsigned long status;
		if(getStatus(&status))
			goto next;

		//Check if input buffer is full
		if(status & STATUS_IBF)
			goto next;

		//Send write_command to the register
		int valid1 = sys_outb(KBC_STATUS, WRITE_COMMAND);

		if(valid1 != OK)
			goto next;

		//Send command to KBC
		int valid2 = sys_outb(KBC, command);

		if(valid2 != OK)
			goto next;
		else
			return 0;

		next:
		tickdelay(micros_to_ticks(DELAY_US));
		continue;
	}

	return 1;
}

int readCommand() {
	int nrIter = 0;

	while(nrIter < 10)
	{
		nrIter++;

		//Get status
		unsigned long status;
		if(getStatus(&status))
			goto next;

		//Check if input buffer is full
		if(status & STATUS_IBF)
			goto next;

		int valid1 = sys_outb(KBC_STATUS, READ_COMMAND);

		if(valid1 != OK)
			goto next;

		//Get code
		unsigned long response;
		if(getResponse(&response))
			goto next;
		else
			return response;

		next:
		tickdelay(micros_to_ticks(DELAY_US));
		continue;
	}

	return 1;
}

int M_getResponse(unsigned long *response) {

	//Get status
	unsigned long status;
	if(getStatus(&status))
		return 1;

	//Check for parity and timeout error
	if(status & PAR_ERR || status & TO_ERR )
		return 1;

	//Is the data coming from the mouse
	if((status & STATUS_AUX) == 0)
		return 1;

	//Is output buffer full?
	if((status & STATUS_OBF) == 0)
		 return 1;

	//Get response from output buffer
	if(getResponse(response))
		return 1;

	return 0;
}

int M_writeCommand(unsigned long command) {
	int nrIter = 0;

	while(nrIter < 10)
	{
		nrIter++;

		//Get status
		unsigned long status;
		if(getStatus(&status))
			goto next;

		//Check if input buffer is full
		if(status & STATUS_IBF)
			goto next;

		//Send write_mouse to the register
		int valid1 = sys_outb(KBC_STATUS, WRITE_MOUSE);

		if(valid1 != OK)
			goto next;

		//Send command to KBC
		int valid2 = sys_outb(KBC, command);

		if(valid2 != OK)
			goto next;
		else {
			//Acknowledgement byte
			unsigned long acknowledge;

			//Check if the mouse controller sent an acknowledgment byte
			if(M_getResponse(&acknowledge))
				return 1;

			//Check if it's OK
			if(acknowledge == ACK)
				return 0;
			else
				return 1;
		}

		next:
		tickdelay(micros_to_ticks(DELAY_US));
		continue;
	}

	return 1;
}
