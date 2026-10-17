#include <minix/drivers.h>
#include <assert.h>
#include <stdio.h>
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
typedef struct serial_port_class { unsigned short baseAddr; char lcr,lsr,ier,iir; bool transmitter; } SerialPortClass;
static inline
void RTCReadReg(unsigned long addr, unsigned long* config)
{
	asm volatile ("outb %0, %1"
			:
			: "a" ((char) addr), "Nd" (RTC_ADDR_REG)
	);

	char byte;
	asm volatile ("inb %1, %0"
			: "=a" (byte)
			  : "Nd" (RTC_DATA_REG)
	);

	*config = byte;
}

static inline
void RTCWriteReg(unsigned long addr, unsigned long config)
{
	asm volatile ("outb %0, %1"
			:
			: "a" ((char) addr), "Nd" (RTC_ADDR_REG)
	);

	char byte;
	asm volatile ("outb %0, %1"
			:
			: "a" ((char) config), "Nd" (RTC_DATA_REG)
	);
}

static inline
void SerialPortReadReg(SerialPortClass* SerialPort, unsigned short spReg, char* config)
{
	char byte;
	unsigned short port = SerialPort->baseAddr + spReg;
	asm volatile ("inb %1, %0"
			: "=a" (byte)
			  : "Nd" (port)
	);

	*config = byte;
}

static inline
void SerialPortWriteReg(SerialPortClass* SerialPort, unsigned short spReg, char config)
{
	unsigned short port = SerialPort->baseAddr + spReg;
	asm volatile ("outb %0, %1"
			:
			: "a" (config), "Nd" (port)
	);
}
static unsigned port_evals,value_evals,output_evals;
static unsigned short expression_port(void){port_evals++;return 0x3ff;}
static unsigned char expression_value(void){value_evals++;return 0x9d;}
int main(void) {
  sef_startup();
  unsigned long value=0;
  RTCWriteReg(0x3e,0xe7);RTCReadReg(0x3e,&value);
  assert(value==(unsigned long)(signed char)0xe7);
  SerialPortClass serial={.baseAddr=0x3f8};
  const unsigned char values[]={0,0x7f,0x80,0xa5,0xff};
  for(unsigned i=0;i<sizeof(values);i++){char result=0;SerialPortWriteReg(&serial,7,(char)values[i]);SerialPortReadReg(&serial,7,&result);assert((unsigned char)result==values[i]);}
  char slots[2]={0,0x66};
  asm volatile ("outb %0, %1" : : "a" (expression_value()), "Nd" (expression_port()));
  asm volatile ("inb %1, %0" : "=a" (slots[(output_evals++,0)]) : "Nd" (expression_port()));
  assert(port_evals==2&&value_evals==1&&output_evals==1);
  assert((unsigned char)slots[0]==0x9d&&slots[1]==0x66);
  puts("ORIGINAL_BYTE_PORTS_OK");return lcf_cleanup();
}
