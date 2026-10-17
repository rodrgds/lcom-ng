static int keyboard_hook_id;
#include "kbd.h"
#include "utils.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

int kbd_subscribe_int()
{
	keyboard_hook_id = KBD_NOTIFICATION;
	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != OK)
	{
		printf("Error while subscribing keyboard - Set Policy\n");
		return 1;
	}
	if (sys_irqenable(&keyboard_hook_id) != OK)
	{
		printf("Error while subscribing keyboard - Irq Enable\n");
		return 1;
	}
	return 0;
}

int kbd_unsubscribe_int()
{
	if (sys_irqdisable(&keyboard_hook_id) != OK)
	{
		printf("Error while unsubscribing keyboard - Irq Disable\n");
		return 1;
	}

	if (sys_irqrmpolicy(&keyboard_hook_id) != OK)
	{
		printf("Error while unsubscribing keyboard - Remove Policy\n");
		return 1;
	}
	return 0;
}

int kbd_handler()
{
	unsigned long scancode;

	//Check if info read was done correctly
	if (readInfoFromKBD(&scancode) != 0)
	{
		return 1;
	}

	// 2 bytes
	if (scancode == KBD_TWO_BYTE)
	{
		twoBytes = 1;
		return 0;	//return function so it reads next byte in next iteration
	}

	if (twoBytes)
	{
		scancode = scancode | (KBD_TWO_BYTE << 8);
		twoBytes = 0;
	}

	if (scancode & BIT(7))
		printf("Break Code : 0x%X \n\n", scancode);
	else
		printf("Make Code : 0x%X \n", scancode);

	return scancode;
}

int readInfoFromKBD(unsigned long *data)
{
	unsigned long stat;
	unsigned int count = 0;

	while (count < 10)
	{
		if (sys_inb(KBD_IN_BUF, &stat) != OK)
		{
			printf(
					"Error no. 1 (check input buffer) - in function readInfoFromKBD().\n");
			return 1;
		}

		if (stat & BIT(0))
		{
			if (sys_inb(KBD_OUT_BUF, data) != OK)
			{
				printf(
						"Error no. 2 (writing from out buff to data) in function readInfoFromKBD().\n");
				return 2;
			}

			if ((stat & (PAR_ERR | TIMEOUT)) != 0)
			{
				printf(
						"Error no. 3 (invalid data error) - in function readInfoFromKBD().\n");
				return 3;
			}
			//Everything works right as intended.
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		count++;
	}
}

int writeInfoToKBD(unsigned long kbd_port)
{
	unsigned long stat;
	unsigned int count = 0;

	while (count < 10)
	{
		if (sys_inb(KBD_IN_BUF, &stat) != OK)
		{
			printf(
					"Error no. 1 - (input buff not ready) in function writeInfoToKBD().\n");
			return 1;
		}

		if ((stat & BIT(1)) == 0)
		{
			if (sys_outb(KBD_OUT_BUF, kbd_port) != OK)
			{
				printf(
						"Error no. 2 - (Can't read from out buff) in function writeInfoToKBD().\n");
				return 2;
			}
			//Everything works right as intended.
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		count++;
	}
}
