#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "kbd.h"

//VARS============================
//unsigned int irq_set=0;
int irqKeyboardBIT=KEYBOARD_IRQ;//timer's bit on irq_set
int ddKeyboardID;
int stopHandler = 0;
unsigned long scanCode=0;
//extern long assScanCode;
short kbd_useAssembly=0;
//END VARS========================


int Kbd_subscribe_int(void){
	int hookid=irqKeyboardBIT;
	//irq_set=1<<irqKeyboardBIT;
	//printf("IRQ BIT: %d\n",irqKeyboardBIT);
	//Subscribe a notification on every interrupt in the input irq_line.
	if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookid)!=OK)
		{
			printf("Subscription FAILED!\n");
			return -1;
		}

		ddKeyboardID=hookid;

		return KEYBOARD_IRQ;
}

int Kbd_unsubscribe_int(){
	//Disables interrupts on the IRQ line associated with the specified hook_id
	int hookid=ddKeyboardID;

	if(sys_irqdisable(&hookid)!=OK)
	{
		printf("Unsubscription FAILED!\n");
		return -1;
	}

	//To ensure that we can use Minix 3 virtual terminals
	if(sys_irqrmpolicy(&hookid) != OK ){
		printf("sys_irqrmpolicy FAILED!\n");
		return -1;
	}

	return 0;
}



void Kbd_int_handler(unsigned int bitmask){
	if (bitmask & (1<<KEYBOARD_IRQ))
	{
		unsigned long scancode;
		scancode = Kbd_read();

		if(scancode == -1){
			printf("Error reading scancode: 0x%X\n", scancode);
			//errorCounter--;
		}
		//2 bytes scancode. We need to put the other 8 bits on "scancode" variable.
		if (scancode == 0xE0)
			scancode = ( (scancode << 8) | Kbd_read());

		//6 bytes scancode. We need to put the other 40 bits on "scancode" variable.
		else if (scancode == 0xE1){
			int i = 0;
			for(; i< 5; i++)
				scancode =( (scancode << 8) | Kbd_read());

			//printf("Makecode: 0x%X\n", scancode);
		}

		// Comparing MSB means our Release code
		/*if (scancode & 0x80)
			printf("Breakcode: 0x%X\n", scancode);

		else
			printf("Makecode: 0x%X\n", scancode);
		 */
		scanCode = scancode;

	}
}


int Kbd_read(){

	unsigned long status, data;
	if(sys_inb(STAT_REG,&status)!=OK)
	{
		printf("Failed to get configuration from Kbd");
		return -1;
	}
	//Verify if data is available for reading
	if(status & OBF){
		if(kbd_useAssembly==0){
			//Get data from DATAREG
			if(sys_inb(DATA_REG, &data) != OK){
				printf("Failed to get configuration from KBD");
				return -1;
			}
		}
		else
		{
			//Kbd_asm_read();
			//data=assScanCode;
		}


		//Check Parity or Timeout errors
		if((status & (TO_ERR | PAR_ERR)) /*|| data == RESEND || data == ERROR*/)
			return -1;

		return data;
	}
	//Give enough time for Kbd/KBD to respond
	if (tickdelay(micros_to_ticks(DELAY_US)) != 0){
		printf("Failed to use tickdelay");
		return -1;
	}

	return -1;

}

int Kbd_write(unsigned int args)
{
	unsigned long status, command,response;
	if(sys_inb(STAT_REG,&status)!=OK)
	{
		printf("Failed to get configuration from Kbd");
		return -1;
	}
	//Verify if data is available to write
	if(!(status & IBF)){
		//Get data from DATAREG
		command=SET_LEDS_KBD;
		if(sys_outb(DATA_REG, command) != OK){
			printf("Failed to write configuration from KBD");
			return -1;
		}

		if(sys_inb(DATA_REG, &response) != OK){
			printf("Failed to read configuration from KBD");
			return -1;
		}

		//Check Parity, Timeout errors and KBD response
		if((status & (TO_ERR | PAR_ERR)) || response == RESEND || response == ERROR)
		{
			printf("Error on KDB/status response");
			return -1;
		}

		//write leds to switch
		command=args;
		if(sys_outb(DATA_REG, command) != OK)
		{
			printf("Failed to send arguments to KBD");
			return -1;
		}

		return 0;//OK
	}
	//Give enough time for Kbd/KBD to respond
	if (tickdelay(micros_to_ticks(DELAY_US)) != 0){
		printf("Failed to use tickdelay");
		return -1;
	}

	return -1;
}

int Kbd_process()
{
	scanCode = Kbd_read();

	if(scanCode == -1){
		printf("Error reading scancode: 0x%X\n", scanCode);
		return -1;
	}
	//2 bytes scancode. We need to put the other 8 bits on "scanCode" variable.
	if (scanCode == 0xE0)
		scanCode = ( (scanCode << 8) | Kbd_read(0));

	//6 bytes scancode. We need to put the other 40 bits on "scanCode" variable.
	else if (scanCode == 0xE1){
		int i = 0;
		for(; i< 5; i++)
			scanCode =( (scanCode << 8) | Kbd_read(0));

		printf("Makecode: 0x%X\n", scanCode);
	}

	// Comparing MSB means our Release code
	if (scanCode & 0x80)
		printf("Breakcode: 0x%X\n", scanCode);

	else
		printf("Makecode: 0x%X\n", scanCode);

	return 0;
}

unsigned long Kbd_getLastScanCodeRead() {return scanCode;}
