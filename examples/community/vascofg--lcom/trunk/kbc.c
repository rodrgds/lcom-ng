#include <minix/syslib.h>
#include <minix/drivers.h>

#include "kbc.h"

int write_kbc(unsigned long port, unsigned char byte)
{
	unsigned long stat;
	while(1)
	{
		sys_inb(STAT_REG, &stat);
		if((stat & IBF) == 0) /*BUFFER EMPTY*/
		{
			sys_outb(port, byte);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return -1;
}
int read_kbc()
{
	unsigned long stat, data;
	while(1)
	{
		sys_inb(STAT_REG,&stat);

		if((stat & OBF) != 0) /*BUFFER FULL*/
		{
			if (AUX==1)
				return -1;
			else
				sys_inb(DATA_REG,&data);
				if(((stat & (PAR_ERR | TO_ERR)) == 0) || (data == 0xfe) || (data == 0xfc))
					return data;
				else
					return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return -1;
}
