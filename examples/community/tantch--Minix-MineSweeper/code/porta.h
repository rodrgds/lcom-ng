#ifndef PORTA_H
#define PORTA_H
#include <minix/syslib.h>
#include <minix/drivers.h>





#define UART_FREQ 115200

#define COM1_BASE_ADDR	0x3f8
#define COM2_BASE_ADDR	0x2f8
#define COM1_IRQ	4
#define COM2_IRQ	3

#define IER	1
#define IIR	2
#define FCR 2
#define LCR	3

#define DLL	0
#define DLM	1

#define LSR 5
#define RB	0
#define TH	0
#define ReceiverReady	BIT(0)
#define THREmpty		BIT(5)

unsigned long getAddr();
void setAddr(unsigned long n);
void printUARTConfiguration(unsigned short base_addr);
void getUARTConfiguration(unsigned short base_addr, unsigned long registers[]);
int setUARTConfiguration(unsigned short base_addr, unsigned long bits,
		unsigned long stop, long parity, unsigned long rate);
int sendChar(unsigned short base_addr, char ch);
int getBitRate(unsigned short base_addr);
int setBitRate(unsigned short base_addr, unsigned long rate);

void setDLAB(unsigned short base_addr, short bit);
int receiveChar(unsigned short base_addr, unsigned char *ch);
int receiveCharlight(unsigned short base_addr, unsigned char *ch);
void set_poll(unsigned short base_addr);
#endif
