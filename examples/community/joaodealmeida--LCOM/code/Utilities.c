#include "Utilities.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"

#include "smart_vector.h"

FILE* logfd=NULL;

defineSmartVector(Interrupt);

Interrupt_SV * interruptList;


//==========Handler Stuff================================================
unsigned long subs_irq_set=0;//irq_set with this program subscriptions

void initInterruptList(){
	interruptList = Interrupt_SV_init(5,1);
}

void addToInterrupt(void (*handler) (unsigned int),int (*subscribe) (void), int (*unsubscribe) (void)){
	Interrupt i = {handler, subscribe, unsubscribe};
	SV_ADD(interruptList,i,Interrupt);
}

void subscribeInterrupts(){
	int i = 0;
	for(; i < interruptList->size ; i++)
		interruptList->elems[i].subscribe();
}

void callInterruptsHandler(unsigned int bitmask){
	int i = 0;
	for(; i < interruptList->size ; i++)
			interruptList->elems[i].handler(bitmask);
}

void unsubscribeInterrupts(){
	int i = 0;
	for(; i < interruptList->size ; i++)
			interruptList->elems[i].unsubscribe();
}

void freeInterruptList(){
	SV_FREE(interruptList);
}


void interruptHandler()
{
	int ipc_status;
	message msg;

	/* Get a request message. */
	int r;
	if ( (r = driver_receive(ANY, &msg, &ipc_status))  != 0 )
	{
		printf("driver_receive failed with: %d", r);
		return;
	}
	if (is_ipc_notify(ipc_status)) {
		/* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:	/* hardware interrupt notification */
			/* process timer interruption*/
			callInterruptsHandler(msg.NOTIFY_ARG);

		}
	}
	else {
		/* received a standard message, not a notification */
		/* no standard messages expected: do nothing */
	}
}

//=========in and out stuff==============================================
int write_to(unsigned long port,unsigned long arg)
{
	unsigned long status;

	if (sys_inb(STAT_REG, &status) != OK) {
		printf("\nwrite_to:sys_inb failed.\n");
		return -1;
	}

	if(status & IBF)
	{
		printf("\nnwrite_to:Input buffer full, could not write command");
		return -1;
	}

	if (sys_outb(port, arg) != OK) {
		printf("\nwrite_to:sys_outb failed.\n");
		return -1;
	}

	return 0;
}

int read_DATA_REG_Byte(unsigned long *ret)
{
	unsigned long status, data;
		if(sys_inb(STAT_REG,&status)!=OK)
		{
			printf("\nread_DATA_REG_Byte:Failed to get status");
			return -1;
		}
		//Check Parity or Timeout errors
		//error and resend in case of response are evaluated externally
		if(status & (TO_ERR | PAR_ERR))
		{
			printf("\nread_DATA_REG_Byte:Parity or Time Out error");
			return -1;
		}
		//Verify if data is available for reading
		if(status & OBF){
				//Get data from DATAREG
				if(sys_inb(DATA_REG, &data) != OK){
					printf("\nread_DATA_REG_Byte:Failed to get data");
					return -1;
				}


			(*ret)=data;
			return 0;
		}
		DEBUG_STR("\nread_DATA_REG_Byte:OBF 0, could not read");
		return -1;
}


void resetIrqSet() {subs_irq_set=0;}
void addIrq(unsigned short irqBitPosition) {subs_irq_set+=1<<irqBitPosition;}

