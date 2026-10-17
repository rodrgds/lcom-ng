#include "libraries.h"

unsigned char scancode; //!< keyboard's keys code
unsigned long BUFFER_CLEAN_BYTE = 0; //!< value used to reset keyboard buffer

int bool = 1; //!< Interruption Cycle Controller 1=true 0=false
int TWO_BYTE_KEY =0;
unsigned long stat,data;


int read_keyboard()
{
while(1)
{
	sys_inb(STAT_REG,&stat); //assuming it returns OK

	if(stat & OBF)
	{
		sys_inb(OUT_BUF,&data); //assuming it returns OK

		if((stat&(PAR_ERR|TO_ERR))==0)
			return data;
		else
			return -1;
	}
	tickdelay(micros_to_ticks(DELAY_US));
	}
}

void keyboard_handler()
{
	scancode = read_keyboard();

	if(TWO_BYTE_KEY!=2) //It hasnt find a two byte scancode yet
	{
		if(scancode==TWO_BYTE_BREAKCODE)
			TWO_BYTE_KEY=2;
		else
			TWO_BYTE_KEY=0;
	}
	else
		TWO_BYTE_KEY=1;//It has found a two byte scancode

	if(TWO_BYTE_KEY==0)
	{
		if(scancode == 0x02) //1
			player1_horizontal_volley();
		else
			if(scancode == 0x03) //2
				player1_vertical_volley();
			else
				if(scancode == 0x04) //3
					player1_scout();
				else
					if(scancode == 0x05) //4
						player1_bombardement();

		if(scancode == 0x1C) //ENTER
			{
				check_square_keyboard();
			}
		else
		{
			if(scancode == 0x11)  //w
			{
				move_keyboard_cursor(1);
			}else
				if(scancode == 0x1E) //a
				{
					move_keyboard_cursor(2);
				}else
					if(scancode == 0x1F) //s
					{
						move_keyboard_cursor(3);
					}else
						if(scancode == 0x20) //d
						{
							move_keyboard_cursor(4);
						}
		}
	}
	else
		if(TWO_BYTE_KEY==1) // two byte code found
		{
			if(scancode == 0X48)  //U arrow
					{
						move_keyboard_cursor(1);
					}else
						if(scancode == 0x04B) //L arrow
						{
							move_keyboard_cursor(2);
						}else
							if(scancode == 0x50) //D arrow
							{
								move_keyboard_cursor(3);
							}else
								if(scancode == 0X4D) //R arrow
								{
									move_keyboard_cursor(4);
								}
		}
}


int keyboard_subscribe_int()
{
if(sys_irqsetpolicy(IRQ_KBD,(IRQ_REENABLE|IRQ_EXCLUSIVE),&hook_id_kbd)!=OK)
{
	printf("IRQ_SETPOLICY failure /n");
	return -1;
}
if(sys_irqenable(&hook_id_kbd)!=OK)
{
	printf("IRQENABLE failure /n");
	return -1;
}

return hook_id_kbd;
}

int keyboard_unsubscribe_int()
{
if(sys_irqdisable(&hook_id_kbd)!= OK)
{
	printf("IRQDISABLE failed /n");
	return -1;
}
if(sys_irqrmpolicy(&hook_id_kbd)!=OK)
{
	printf("IRQRMPOLICY failed /n");
	return -1;
}
return 0;
}

void clean_OBF()
{
	unsigned long stat;

	do
	{sys_inb(STAT_REG, &stat);
	if(stat & OBF) //if OBF has something
		sys_inb(OUT_BUF,&BUFFER_CLEAN_BYTE);
	}
	while(stat & OBF); //while OBF is full
}
