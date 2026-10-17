#define COM1_BASE 0x3F8
#define COM2_BASE 0x2F8

#define COM1_IRQ 4
#define COM2_IRQ 3

// Registers
#define SER_DATA 0 //DLAB = 0
#define SER_IER 1 //DLAB = 0
#define SER_IIR 2
#define SER_FIFO_CTR 2
#define SER_LCR 3
#define SER_LSR 5
#define SER_DLL 0 //DLAB = 1
#define SER_DLM 1 //DLAB = 1

//LCR Bits
#define LCR_5BITS 0x00
#define LCR_6BITS 0x01
#define LCR_7BITS 0x02
#define LCR_8BITS 0x03

#define LCR_1STOPBIT 0x00
#define LCR_2STOPBITS 0x04

#define LCR_NOPARITY 0x00 //YOLO
#define LCR_ODDPARITY 0x08
#define LCR_EVENPARITY 0x18
#define LCR_1PARITY 0x28
#define LCR_0PARITY 0x38

#define LCR_DLAB 0x80


#define SER_BASE_FREQ 115200

//IER Bits

#define IER_RECIEVEDDATA 0x01
#define IER_TRANSMITTEREMTY 0x02
#define IER_RECIEVEDLINESTATUS 0x04

//LSR
#define LSR_INREADY 0x01
#define LSR_OUTREADY 0x20
#define SER_OVERRUN_ERR (1<<1)
#define SER_PARITY_ERR (1<<2)
#define SER_FRAME_ERR (1<<3)

//IIR

#define IIR_INT_PENDING 0x00
#define IIR_INT_NOTPENDING 0x01
#define IIR_MODEMSTATUS 0x00
#define IIR_TXREADY 	0x02
#define IIR_CHARTIMEOUT 0x08
#define IIR_RXREADY 	0x04
#define IIR_LINESTATUS 	0x06
#define IIR_64FIFO 0x20
#define IIR_NOFIFO 0x00
#define IIR_UNUSABLEFIFO 0x80
#define IIR_ENABLEDFIFO 0xC0

//FIFO CTRL

#define FIFO_ENABLE 0x01
#define FIFO_CLEAR_RX 0x02
#define FIFO_CLEAR_TX 0x04
#define FIFO_LEVEL1 0x00
#define FIFO_LEVEL4 0x40
#define FIFO_LEVEL8 0x80
#define FIFO_LEVEL14 0xC0


static int SER_HOOK_ID;


int ser_set_dlab(unsigned short base_addr, int dlab);

int ser_show_config(unsigned short base_addr);

int ser_set_config(unsigned short base_addr, unsigned long bits, unsigned long stop,
		long parity, unsigned long rate);

int ser_poll_recieve(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);
int ser_poll_send(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);

int ser_int_recieve(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);
int ser_int_send(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);


int ser_subscribe_int(unsigned short com);
int ser_unsubscribe_int();


int ser_fifo_recieve(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);
int ser_fifo_send(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);


int ser_recieve_char(unsigned short base_addr, char* c);
int ser_send_char(unsigned short base_addr, char c);

int ser_enable_int(unsigned short base_addr, unsigned long interrupt);
int ser_disable_int(unsigned short base_addr, unsigned long interrupt);

int ser_poll_send_int(unsigned short base_addr, unsigned long bits, unsigned long stop,	long parity, unsigned long rate, int val);
