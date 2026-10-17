unsigned int hook_id_rtc;
unsigned int irq_set_rtc;

#include "RTC.h"

int rtc_subscribe_int(int exclusive)
{
	hook_id_rtc = 3;
	if (exclusive)
	{
		if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id_rtc) != OK)
		{
			printf("rtc_subscribe failed\n");
			return -1;
		}
	}
	else
	{
		if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
				&hook_id_rtc) != OK)
		{
			printf("rtc_subscribe failed\n");
			return -1;
		}
	}
	return 0;
}


int rtc_unsubscribe_int()
{
	if (sys_irqrmpolicy(&hook_id_rtc) != OK)
	{
		printf("rtc_unsubscribe_int: sys_irqrmpolicy() failed\n");
		return -1;
	}
	return 0;
}

unsigned long read_rtc(unsigned long reg)
{
	unsigned long data = 0;
	sys_outb(RTC_ADDR_R, reg);
	sys_inb(RTC_DATA_R, &data);
	return data;
}

