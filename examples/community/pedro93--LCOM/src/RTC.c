#include "libraries.h"

//static char background[25][140];

void wait_valid_rtc(void)
{
	int enabled;
	unsigned long regA = 0;

	do
	{
		enabled = disable(); // globally disable interrupts
		sys_outb(RTC_ADDR_REG,RTC_REG_A);
		sys_inb(RTC_DATA_REG,&regA);

		if(enabled)
			enable();
	}
	while(regA & RTC_UIP);
}

void wait_update_end(void)
{
	int enabled;
	unsigned long regC = 0;

	do
	{
		enabled = disable();
		sys_outb(RTC_ADDR_REG,RTC_REG_C);
		sys_inb(RTC_DATA_REG,&regC);

		if (enabled)
			enable();
	}
	while(regC & RTC_UF);
}

int RTC_subscribe()
{
	if(sys_irqsetpolicy(IRQ_RTC,IRQ_REENABLE,&hook_id_rtc)!=OK)
	{
		printf("IRQ_SETPOLICY failure /n");
		return -1;
	}
	if(sys_irqenable(&hook_id_rtc)!=OK)
	{
		printf("IRQENABLE failure /n");
		return -1;
	}
	return hook_id_rtc;
}

int RTC_unsubscribe()
{
	if(sys_irqdisable(&hook_id_rtc)!= OK)
	{
		printf("IRQDISABLE failed /n");
		return -1;
	}
	if(sys_irqrmpolicy(&hook_id_rtc)!=OK)
	{
		printf("IRQRMPOLICY failed /n");
		return -1;
	}
	return 0;
}

int RTC_handler_date()
{
	unsigned long regC;

	sys_outb(RTC_ADDR_REG,RTC_REG_C);
	sys_inb(RTC_DATA_REG,&regC);

	if(regC & RTC_UF)
	{
		//Data,Time and Alarm registers of the RTC
		unsigned long sec,sec_alarm,min,min_alarm,hours,hours_alarm,day_of_week,date_of_month,month,year;
		unsigned long regB;

		//when making the data human-readable :
		//verify bit 2 (DM) of register B to see if time,alarm and date registers are in binary or BCD (1-binary,0-BCD)
		//verify bit 1 (24/12) of register B to see if the hours range is from 0 to 23 or from 0 to 12 (1-> 0-23 , 0-> 0-12)
		//make the necessary conversions

		//read all time,alarm and date registers to the variables
		sec = read_rtc(0);
		//sec_alarm = read_rtc(1);
		min = read_rtc(2);
		//min_alarm = read_rtc(3);
		hours = read_rtc(4);
		//hours_alarm = read_rtc(5);
		day_of_week = read_rtc(6);
		date_of_month = read_rtc(7);
		month = read_rtc(8);
		year = read_rtc(9);

		//make the necessary conversions

		//read the register B
		sys_outb(RTC_ADDR_REG,RTC_REG_C);
		sys_inb(RTC_DATA_REG,&regC);

		//BCD to DEC || BIN to DEC
		if(regB & RTC_DM) //check if bit 2 of register B is 1
		{
			//convert from BCD to DEC
			sec = BIN_to_DEC(sec);
			min = BIN_to_DEC(min);
			hours = BIN_to_DEC(hours);
			day_of_week = BIN_to_DEC(day_of_week);
			date_of_month = BIN_to_DEC(date_of_month);
			month = BIN_to_DEC(month);
			year = BIN_to_DEC(year);

		}
		else //if bit 2 of register B is 0
		{
			//convert from binary do DEC
			sec = BCD_to_DEC(sec);
			min = BCD_to_DEC(min);
			hours = BCD_to_DEC(hours);
			day_of_week = BCD_to_DEC(day_of_week);
			date_of_month = BCD_to_DEC(date_of_month);
			month = BCD_to_DEC(month);
			year = BCD_to_DEC(year);
		}

		printf("Current Date: ");
		/*switch(day_of_week)
		{
		case 1:	printf("Sunday ");
		break;
		case 2:	printf("Monday ");
		break;
		case 3:	printf("Tuesday ");
		break;
		case 4:	printf("Wednesday ");
		break;
		case 5:	printf("Thursday ");
		break;
		case 6:	printf("Friday ");
		break;
		case 7: printf("Saturday ");
		break;
		default:printf("Day not defined\n");
		break;
		}*/

		//display the data
		//printf(" %d-%d-%d\n",date_of_month,month,year);
		printf("Current Time: %d:%d:%d\n",hours,min,sec);

		//display the data graphically
		//draw_time(hours,min,sec);

		//printf("Alarm Set to: %d:%d:%d\n",hours_alarm,min_alarm,sec_alarm);

		return 1;
	}

	return 0;
}

unsigned int getTime()
{
	int tempo_sec=0;

	unsigned long regC;
	sys_outb(RTC_ADDR_REG,RTC_REG_C);
	sys_inb(RTC_DATA_REG,&regC);

		//Data,Time and Alarm registers of the RTC
		unsigned long sec,sec_alarm,min,min_alarm,hours,hours_alarm,day_of_week,date_of_month,month,year;
		unsigned long regB;

		//read all time,alarm and date registers to the variables
		sec = read_rtc(0);
		//sec_alarm = read_rtc(1);
		min = read_rtc(2);
		//min_alarm = read_rtc(3);
		hours = read_rtc(4);
		//hours_alarm = read_rtc(5);
		day_of_week = read_rtc(6);
		date_of_month = read_rtc(7);
		month = read_rtc(8);
		year = read_rtc(9);

		//make the necessary conversions

		//read the register B
		sys_outb(RTC_ADDR_REG,RTC_REG_C);
		sys_inb(RTC_DATA_REG,&regC);

		//BCD to DEC || BIN to DEC
		if(regB & RTC_DM) //check if bit 2 of register B is 1
		{
			//convert from BCD to DEC
			sec = BIN_to_DEC(sec);
			min = BIN_to_DEC(min);
			hours = BIN_to_DEC(hours);
			day_of_week = BIN_to_DEC(day_of_week);
			date_of_month = BIN_to_DEC(date_of_month);
			month = BIN_to_DEC(month);
			year = BIN_to_DEC(year);

		}
		else //if bit 2 of register B is 0
		{
			//convert from binary do DEC
			sec = BCD_to_DEC(sec);
			min = BCD_to_DEC(min);
			hours = BCD_to_DEC(hours);
			day_of_week = BCD_to_DEC(day_of_week);
			date_of_month = BCD_to_DEC(date_of_month);
			month = BCD_to_DEC(month);
			year = BCD_to_DEC(year);
		}

		tempo_sec+=hours*3600;
		tempo_sec+=min*60;
		tempo_sec+=sec;
		//printf("getTime() function value %d\n", tempo_sec);

		return tempo_sec;
}

void print_game_duration(int start, int end)
{
	unsigned int duration = end-start;
	unsigned long aux=duration;
	unsigned int horas, minu, seg;
	horas = aux/3600;
	aux=duration;
	minu = (aux - horas*3600)/60;
	aux=duration;
	seg = (aux - horas*3600 - minu*60);
	draw_time(horas,minu,seg);
}

void draw_time(unsigned int hours, unsigned int min, unsigned int sec)
{
	vg_fill(11);
	Sprite *hour_dez,*hour_uni,*min_dez,*min_uni,*sec_dez,*sec_uni,*dots;
	Sprite *Gametime = create_sprite(game_time,0);
	hour_dez = draw_symbol(hours/10); // dezenas
	hour_uni = draw_symbol(hours%10); //unidades
	min_dez = draw_symbol(min/10);
	min_uni = draw_symbol(min%10);
	sec_dez = draw_symbol(sec/10);
	sec_uni = draw_symbol((int)sec%10);
	dots=draw_symbol(12);

	//printf("GAME TIME: %d%d:%d%d:%d%d \n", hours/10,hours%10,min/10,min%10,sec/10,sec%10);


	// Fazer o set dos pixeis com as cores devidas
	draw_sprite(325,250,Gametime);
	draw_sprite(392,360,hour_dez);
	draw_sprite(392+35,360,hour_uni);
	draw_sprite(392+70,365,dots);
	draw_sprite(392+105,360,min_dez);
	draw_sprite(392+140,360,min_uni);
	draw_sprite(392+175,365,dots);
	draw_sprite(392+210,360,sec_dez);
	draw_sprite(392+245,360,sec_uni);

	//Apagar as sprites senao memory leak
	/*destroy_sprite(hour_dez,0);
	destroy_sprite(hour_uni,0);
	destroy_sprite(min_dez,0);
	destroy_sprite(min_uni,0);
	destroy_sprite(sec_dez,0);
	destroy_sprite(sec_uni,0);
	destroy_sprite(Gametime,0);*/

	return;
}

Sprite * draw_symbol(unsigned int value)
{
	Sprite * temp;
	switch(value)
	{
	case 0:
		temp = create_sprite(num0_34,0);
		break;
	case 1:
		temp = create_sprite(num1_34,0);
		break;
	case 2:
		temp = create_sprite(num2_34,0);
		break;
	case 3:
		temp = create_sprite(num3_34,0);
		break;
	case 4:
		temp = create_sprite(num4_34,0);
		break;
	case 5:
		temp = create_sprite(num5_34,0);
		break;
	case 6:
		temp = create_sprite(num6_34,0);
		break;
	case 7:
		temp = create_sprite(num7_34,0);
		break;
	case 8:
		temp = create_sprite(num8_34,0);
		break;
	case 9:
		temp = create_sprite(num9_34,0);
		break;
	default: //? o ':'
		temp = create_sprite(dots,0);
		break;
	}
	return temp;
}

int RTC_handler_alarm(unsigned long delta)
{
	unsigned long regC;

	sys_outb(RTC_ADDR_REG,RTC_REG_C);
	sys_inb(RTC_DATA_REG,&regC);

	if(regC & RTC_AF)
	{
		printf("Alarm raised after %d s\n",delta);
		return 1;
	}

	return 0;
}

unsigned long read_rtc(unsigned long rtc_reg)
{
	unsigned long value_read;

	disable();
	sys_outb(RTC_ADDR_REG,rtc_reg);
	sys_inb(RTC_DATA_REG,&value_read);
	enable();

	return value_read;
}

unsigned long write_rtc(unsigned long rtc_reg,unsigned long value_to_write)
{
	disable();
	sys_outb(RTC_ADDR_REG,rtc_reg);
	sys_outb(RTC_DATA_REG,value_to_write);
	enable();

	return rtc_reg;
}

void enable_UIE()
{
	//enable Update-ended interrupt
	unsigned long regB;
	regB = read_rtc(RTC_REG_B);
	regB = regB ^ RTC_UIE;
	write_rtc(RTC_REG_B,regB);
}

void enable_AIE()
{
	//enable Alarm interrupt
	unsigned long regB;
	regB = read_rtc(RTC_REG_B);
	regB = regB ^ RTC_AIE;
	write_rtc(RTC_REG_B,regB);
}

void enable() {

	endpoint_t ep;
	char name[256];
	int priv_f;

	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);
	asm( "STI" );
}

unsigned long disable() {

	endpoint_t ep;
	char name[256];
	int priv_f;

	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);
	asm( "CLI" );

	return 1;
}



unsigned long BCD_to_DEC(unsigned long bcdByte)
{
	return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

unsigned long DEC_to_BCD(unsigned long decimalByte)
{
	return (((decimalByte / 10) << 4) | (decimalByte % 10));
}
