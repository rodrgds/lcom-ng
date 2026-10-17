#include <minix/drivers.h>

#include "Serial.h"


int ser_set_dlab(unsigned short base_addr, int dlab) {
	unsigned long word;

	sys_inb(base_addr + SER_LCR, &word);

	if (dlab) {word |= 0x80;}
	else {word &= ~0x80;}

	sys_outb(base_addr + SER_LCR, word);

	return 0;
}

int ser_show_config(unsigned short base_addr) {

	unsigned long lcr, ier, dll, dlm, d;
	unsigned long Freq;

	sys_inb(base_addr + SER_LCR, &lcr);

	ser_set_dlab(base_addr,0);

	sys_inb(base_addr + SER_IER, &ier);

	ser_set_dlab(base_addr,1);

	sys_inb(base_addr + SER_DLL, &dll);
	sys_inb(base_addr + SER_DLM, &dlm);
	d = (dlm << 8) + dll;

	Freq = SER_BASE_FREQ / d;

	ser_set_dlab(base_addr,0);


	//display

	switch(lcr & 0x03){
	case LCR_5BITS: printf("Word Length is %d bits\n", 5); break;
	case LCR_6BITS: printf("Word Length is %d bits\n", 6); break;
	case LCR_7BITS: printf("Word Length is %d bits\n", 7); break;
	case LCR_8BITS: printf("Word Length is %d bits\n", 8); break;}

	switch(lcr & 0x04){
	case LCR_1STOPBIT: printf("No of stop bits: %d \n", 1); break;
	case LCR_2STOPBITS: printf("No of stop bits: %d\n", 2); break;}

	switch(lcr & 0x38){
	case LCR_NOPARITY: printf("Parity: none\n"); break;
	case LCR_ODDPARITY: printf("Parity: odd\n"); break;
	case LCR_EVENPARITY: printf("Parity: even\n"); break;
	case LCR_1PARITY: printf("Parity: 1\n"); break;
	case LCR_0PARITY: printf("Parity: 0\n"); break;}

	switch(ier & 0x01){
	case IER_RECIEVEDDATA: printf("Received data interrupt: enabled\n"); break;
	default: printf("Received data interrupt: disabled\n"); break;}

	switch(ier & 0x02){
	case IER_TRANSMITTEREMTY: printf("Transmitter Empty Interrupt: enabled\n"); break;
	default: printf("Transmitter Empty Interrupt: disabled\n"); break;}

	switch(ier & 0x02){
	case IER_RECIEVEDLINESTATUS: printf("Receiver Line Status Interrupt: enabled\n"); break;
	default: printf("Receiver Line Status Interrupt: disabled\n"); break;}

	printf("Frequency: %d\n", Freq);

	return 0;
};


int ser_set_config(unsigned short base_addr, unsigned long bits, unsigned long stop,
		long parity, unsigned long rate) {

	unsigned long lcr = 0, ier = 0, dll, dlm, d;

	d = SER_BASE_FREQ / rate;
	dll = d & 0x00FF;
	dlm = (d & 0xFF00) >> 8;

	switch (bits) {
	case 5: lcr |= LCR_5BITS; break;
	case 6: lcr |= LCR_6BITS; break;
	case 7: lcr |= LCR_7BITS; break;
	case 8: lcr |= LCR_8BITS; break;
	default:
		printf("Serial:: Invalid number of bits!");
		return 1;
	}
	switch (stop) {
	case 1: lcr |= LCR_1STOPBIT; break;
	case 2: lcr |= LCR_2STOPBITS; break;
	default:
		printf("Serial:: Invalid number of stop bits!");
		return 1;
	}
	switch (parity) {
	case 0: lcr |= LCR_EVENPARITY; break;
	case 1: lcr |= LCR_ODDPARITY; break;
	case -1: lcr |= LCR_NOPARITY; break;
	default:
		printf("Serial:: Invalid parity!");
		return 1;
	}

	//Set LCR
	sys_outb(base_addr + SER_LCR, lcr);

	//Set Freq Divisor
	ser_set_dlab(base_addr, 1);
	sys_outb(base_addr + SER_DLL, dll);
	sys_outb(base_addr + SER_DLM, dlm);
	ser_set_dlab(base_addr, 0);

	return 0;
}


int ser_poll_recieve(unsigned short base_addr, unsigned long bits, unsigned long stop,
		long parity, unsigned long rate) {

	ser_set_config(base_addr, bits, stop, parity, rate);

	char string[256]; int i = 0;

	printf("Recieving...\n");

	while (1) {
		if (ser_recieve_char(base_addr, string + i) != 0) return 1;
		if (string[i] == '.') break;
		i++;
	}
	string[i+1] = 0;

	printf("%s", string);
	return 0;
}

int ser_poll_recieve_int(unsigned short base_addr, unsigned long bits, unsigned long stop,
		long parity, unsigned long rate) {

	//ser_set_config(base_addr, bits, stop, parity, rate);

	char chars[5]; int i = 0;

	while (1) {
		if (ser_recieve_char(base_addr, chars + i) != 0) return -1;
		if (chars[i] == '.') break;
		i++;
	}
	if (i != 4) return -1;

	int res;
	memcpy(&res,&chars,sizeof(int));

	return res;
}


int ser_recieve_char(unsigned short base_addr, char* c) {
	unsigned long lsr, in;
	ser_set_dlab(base_addr, 0);
	while (1) {
		if( sys_inb(base_addr + SER_LSR, &lsr) == OK ) {
			//Check for Errors
			if ( lsr & (SER_OVERRUN_ERR | SER_PARITY_ERR	| SER_FRAME_ERR) ) {
				printf("Serial:: Communication Error: ");
				if (lsr & SER_OVERRUN_ERR) printf("OVERRUN ");
				if (lsr & SER_PARITY_ERR) printf("PARITY ");
				if (lsr & SER_FRAME_ERR) printf("FRAME ");
				printf("\n");
				return 1;
			}
			// Recieve Char
			if (lsr & LSR_INREADY) {
				sys_inb(base_addr + SER_DATA, &in);
				*c = in & 0xFF;
				return 0;
			}
		} else {return 1;}
	}
	return 1;
}



int ser_enable_int(unsigned short base_addr, unsigned long interrupt) {
	unsigned long in;
	sys_inb(base_addr + SER_IER, &in);
	in |= interrupt;
	sys_outb(base_addr + SER_IER, in);
	return 0;
};

int ser_disable_int(unsigned short base_addr, unsigned long interrupt) {
	unsigned long in;
	sys_inb(base_addr + SER_IER, &in);
	in &= (~interrupt) & 0xFF;
	sys_outb(base_addr + SER_IER, in);
	return 0;
}


int ser_subscribe_int(unsigned short base_addr) {
	SER_HOOK_ID = 6;
	unsigned short irq;
	if (base_addr == COM1_BASE) {irq = COM1_IRQ;}
	else if (base_addr == COM2_BASE) {irq = COM2_IRQ;}
	else {printf("Invalid COM port!\n"); return -1;}


	        if (sys_irqsetpolicy(irq, IRQ_REENABLE | IRQ_EXCLUSIVE, &SER_HOOK_ID) != OK

	                        || sys_irqenable(&SER_HOOK_ID) != OK)

	                return -1;

	return (1<<6); //Returns bit order in interrupt mask
};

int ser_unsubscribe_int() {
    if (sys_irqrmpolicy(&SER_HOOK_ID) == OK)
        return 0;
    else
    	return 1;
};




