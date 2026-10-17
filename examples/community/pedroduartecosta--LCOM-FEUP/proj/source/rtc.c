#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "rtc.h"
#include "string.h"


int rtc_subscribe(){

	if ((sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_rtc))==OK)
	{
		sys_irqenable(&hook_rtc);//ativa as interrupcoes
		return BIT(NOTIFICATION_RTC);
	}
	else return -1;
}

int rtc_unsubscribe(){
	if(sys_irqdisable(&hook_rtc)==OK) //desativar as interrupcoes
	{
		sys_irqrmpolicy(&hook_rtc); //Remove a policy do qual foi feito o disable
		return 0;
	}
	else return 1;
}

//code provided by slides given by the professor (https://web.fe.up.pt/~pfs/aulas/lcom2013/at/7rtc.pdf)
void wait_valid_rtc(void) {
	unsigned long regA = 0;
	do {
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);
	} while (regA & RTC_UIP);
}


void display_date(char temp[]){
	long unsigned int seconds=0, minutes=0, hours=0, day=0, day_of_the_week=0, month=0, year=0;

	wait_valid_rtc();


//read date

	//seconds - rtc adress 0
	sys_outb(RTC_ADDR_REG, 0);
	sys_inb(RTC_DATA_REG, &seconds);

	//minutes - rtc adress 2
	sys_outb(RTC_ADDR_REG, 2);
	sys_inb(RTC_DATA_REG, &minutes);

	//hours - rtc adress 4
	sys_outb(RTC_ADDR_REG, 4);
	sys_inb(RTC_DATA_REG, &hours);

	//day - rtc adress 7
	sys_outb(RTC_ADDR_REG, 7);
	sys_inb(RTC_DATA_REG, &day);

	//day of the week - rtc adress 6
	sys_outb(RTC_ADDR_REG, 6);
	sys_inb(RTC_DATA_REG, &day_of_the_week);


	//month - rtc adress 8
	sys_outb(RTC_ADDR_REG, 8);
	sys_inb(RTC_DATA_REG, &month);

	//year - rtc adress 9
	sys_outb(RTC_ADDR_REG, 9);
	sys_inb(RTC_DATA_REG, &year);


	char weekday[10];
	sprintf(weekday, "%x", day_of_the_week);

	switch(atoi(weekday)){
	case '1':
		strcpy(weekday, "Sunday");
		break;
	case '2':
		strcpy(weekday, "Monday");
		break;
	case '3':
		strcpy(weekday, "Tuesday");
		break;
	case '4':
		strcpy(weekday, "Wednesday");
		break;
	case '5':
		strcpy(weekday, "Thursday");
		break;
	case '6':
		strcpy(weekday, "Friday");
		break;
	case '7':
		strcpy(weekday, "Saturday");
		break;


	}

	char str[50];
	sprintf(str, "%x/%x/%x_%x:%x:%x", day, month, year, hours, minutes, seconds);
	//printf("%s", str);

	sprintf(temp,"%s",str);
}



//Alarm interrupts (AI)  AIE -> BIT5 Register B
//Update interrupts (UI) UIE -> BIT4 Register B
//Periodic interrupts (PI) PIE -> BIT6 Register B
