#include <minix/syslib.h>
#include <minix/drivers.h>
#include "porta.h"
#include "general.h"
int hook_id;
unsigned long addr;

unsigned long getAddr() {
	return addr;
}
void setAddr(unsigned long n) {
	addr = n;
}


void printUARTConfiguration(unsigned short base_addr) {
	unsigned long registers[3];

	printf("\n\tCOM address:\t0x%X\n", base_addr);
	getUARTConfiguration(base_addr, registers);

	// LCR
	printf("\n\t\tLCR:\t0x%02X\n", registers[0]);

	printf("\tWord length:\t\t");
	int temp = 5;
	if (registers[0] & BIT(0)) temp += 1;
	if (registers[0] & BIT(1)) temp += 2;
	printf("%d\n", temp);

	printf("\tNo. stop bits:\t\t%d\n", ((registers[0] & BIT(2)) && 1) + 1);

	printf("\tParity:\t\t\t");
	if ((registers[0] & BIT(3)) == 0)
		printf("None\n");
	else {
		if ((registers[0] & BIT(5)) == 0) {
			if ((registers[0] & BIT(4)) == 0)
				printf("Odd\n");
			else
				printf("Even\n");
		} else {
			if ((registers[0] & BIT(4)) == 0)
				printf("1\n");
			else printf("0\n");
		}
	}

	printf("\tDLAB:\t\t\t%d\n", registers[0] & BIT(7));

	// DL
	printf("\n\tDL:\tDLM:\t0x%02X\n", registers[1] >> 8);
	printf("\t\tDLL:\t0x%02X\n", registers[1]);

	printf("\tBit-rate:\t\t%d\n", getBitRate(base_addr));

	// IER
	printf("\n\t\tIER:\t0x%02X\n", registers[2]);

	printf("\tRx interrupts:\t\t");
	if (registers[2] & BIT(2)) printf("Enabled\n");
	else printf("Disabled\n");

	printf("\tTx interrupts:\t\t");
	if (registers[2] & BIT(1)) printf("Enabled\n");
	else printf("Disabled\n");

	printf("\n");
}
void getUARTConfiguration(unsigned short base_addr, unsigned long registers[]) {
	// LCR
	sys_inb(base_addr + LCR, &registers[0]);

	// DL
	unsigned long temp = 0;
	setDLAB(base_addr, 1);

	sys_inb(base_addr + DLM, &temp);
	registers[1] = (temp & 0xFF) << 8;
	sys_inb(base_addr + DLL, &temp);
	registers[1] |= (temp & 0xFF);

	setDLAB(base_addr, 0);

	// IER
	sys_inb(base_addr + IER, &registers[2]);
}
int setUARTConfiguration(unsigned short base_addr, unsigned long bits,
		unsigned long stop, long parity, unsigned long rate) {
	if (bits < 5 || 8 < bits) {
		printf("\n\tser_set_params: invalid parameters\n");
		printf("\tbits:\t\t%d\n", bits);
		printf("\n\tERROR: bits < 5 || 8 < bits\n", rate);
		return 1;
	} else if (stop < 1 || 2 < stop) {
		printf("\n\tser_set_params: invalid parameters\n");
		printf("\tstop:\t\t%d\n", stop);
		printf("\n\tERROR: stop < 1 || 2 < stop\n", rate);
		return 1;
	} else if (parity < -1 || 1 < parity) {
		printf("\n\tser_set_params: invalid parameters\n");
		printf("\tparity:\t\t%d\n", parity);
		printf("\n\tERROR: parity < -1 || 1 < parity\n", rate);
		return 1;
	} else if (UART_FREQ % rate != 0) {
		printf("\n\tser_set_params: invalid parameters\n");
		printf("\trate:\t\t%d\n", rate);
		printf("\n\tERROR: UART_FREQ%rate != 0\n", rate);
		return 1;
	}

	// Getting current configuration
	unsigned long registers[3];
	getUARTConfiguration(base_addr, registers);

	// Setting new configuration
	// bits
	registers[0] &= ~BIT(0);
	registers[0] &= ~BIT(1);	// LCR = xxxx xxxx xxxx xx00
	if (bits == 6 || bits == 8)
		registers[0] |= BIT(0);	// LCR = xxxx xxxx xxxx xx01
	if (bits == 7 || bits == 8)
		registers[0] |= BIT(1);	// LCR = xxxx xxxx xxxx xx10

	// stop
	registers[0] &= ~BIT(2);	// LCR = xxxx xxxx xxxx x0xx
	if (stop == 2)
		registers[0] |= BIT(2);

	// parity
	// none
	if (parity == -1) {
		registers[0] &= ~BIT(3);
	} else {
		registers[0] |= BIT(3);

		registers[0] &= ~BIT(5);

		// even
		if (parity == 0)
			registers[0] |= BIT(4);
		// odd
		else if (parity == 1)
			registers[0] &= ~BIT(4);
	}

	// Writing new configuration
	sys_outb(base_addr + LCR, registers[0]);

	setBitRate(base_addr, rate);

	return 0;
}

int sendChar(unsigned short base_addr, char ch) {
	unsigned long LSRcontent;


	while(1) {
		// reading LSR
		sys_inb(base_addr+LSR, &LSRcontent);

		if (LSRcontent & THREmpty) {
			sys_outb(base_addr + TH, ch);
			printf(" %d sent \n",ch);
			return 0;
		}

		tickdelay(micros_to_ticks(20000));

	}


	return -1;
}
int receiveChar(unsigned short base_addr, unsigned char *ch) {
	unsigned long LSRcontent, tempCh;

	while (1) {
		// reading LSR
		sys_inb(base_addr + LSR, &LSRcontent);

		if (LSRcontent & ReceiverReady) {
			sys_inb(base_addr + RB, &tempCh);
			*ch = tempCh;
			return 0;
		}

		tickdelay(micros_to_ticks(500));
	}

	return -1;
}
int receiveCharlight(unsigned short base_addr, unsigned char *ch) {
	unsigned long LSRcontent, tempCh;

	int try=0;
	while (try<1) {
		// reading LSR
		sys_inb(base_addr + LSR, &LSRcontent);

		if (LSRcontent & ReceiverReady) {
			sys_inb(base_addr + RB, &tempCh);
			*ch = tempCh;
			return 0;
		}


		try++;
	}

	return -1;
}

int getBitRate(unsigned short base_addr) {
	unsigned long DL, temp = 0;
	setDLAB(base_addr, 1);

	if(sys_inb(base_addr + DLM, &temp) != OK)
		return -1;
	DL = (temp & 0xFF) << 8;

	if(sys_inb(base_addr + DLL, &temp) != OK)
		return -1;
	DL |= (temp & 0xFF);

	setDLAB(base_addr, 0);

	return UART_FREQ/DL;
}

int setBitRate(unsigned short base_addr, unsigned long rate) {
	unsigned long DL = 0;
	DL = UART_FREQ / rate;

	setDLAB(base_addr, 1);

	// writing to register
	if (sys_outb(base_addr + DLL, (DL & 0xFFFF)) != OK)
		return -1;
	if (sys_outb(base_addr + DLM, ((DL >> 8) & 0xFFFF)) != OK)
		return -1;

	setDLAB(base_addr, 0);

	return 0;
}


void setDLAB(unsigned short base_addr, short bit) {
	unsigned long LCRreg;

	sys_inb(base_addr + LCR, &LCRreg);

	if(bit) LCRreg |= BIT(7);
	else LCRreg &= ~BIT(7);

	sys_outb(base_addr + LCR, LCRreg);
}
void set_poll(unsigned short base_addr)
{
 unsigned long ier;
 sys_inb(base_addr + 1, &ier);
 ier = ier & 0xF8;
 sys_outb(base_addr + 1, ier);
}
