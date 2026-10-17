#ifndef __MODPORTASERIE_H
#define __MODPORTASERIE_H

int hook_id;

#define BIT(n) (0x01<<(n))

#define UART_FREQ 		115200

#define COM1_ADDR		0x3f8
#define COM1_IRQ		4
#define COM2_ADDR		0x2f8
#define COM2_IRQ		3

#define DLL 			0
#define DLM				1

#define LCR				3
#define IER				1
#define LSR				5
#define RB				0
#define TH				0



#define RR				0
#define THRE			BIT(5)

///@brief changes the bit of DLAB
///@param base_addr
///@param bit
void changeDLAB(unsigned short base_addr,int bit);
///@brief gets the character(c) by doing a while loop if not yet available
///@param base_addr
///@param c
///@return sucess
int getChar(unsigned short base_addr, unsigned char *c);
///@brief gets the character(c) if available if not ends
///@param base_addr
///@param c
///@return sucess
int getCharOne(unsigned short base_addr, unsigned char *c);
///@brief sends a character(c)
///@param base_addr
///@param c
///@return sucess
int sendChar(unsigned short base_addr,char c);
///@brief Configures UART
///@param base_addr
///@param bits of the word
///@param stop bits for stoping
///@param parity type of parity
///@param rate
void setPorta(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);



#endif
