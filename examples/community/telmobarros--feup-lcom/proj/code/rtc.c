#include "rtc.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "video_gr.h"

static int rtc_hook_id=3;

int rtc_subscribe_int() {
	int rtc_id = BIT(rtc_hook_id);

	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &rtc_hook_id) != OK)
		return 1;
	//if (sys_irqenable(&rtc_hook_id) != OK)
	//	return 1;
	return rtc_id;

}

int rtc_unsubscribe_int() {
	if (sys_irqrmpolicy(&rtc_hook_id) != OK)
		return 1;
	//if (sys_irqdisable(&rtc_hook_id) != OK)
	//	return 1;
	return 0;
}

unsigned char bcd_convert(unsigned char bin)
{
	//simple conversion from binary to BCD
	return ((bin >> 4) * 10) + (bin & 0x0F);
}

int rtc_current_time(unsigned long *hour, unsigned long *min, unsigned long *sec)
{

	unsigned long res;

	do
	{
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &res);
	} while(res & 0x80);

	sys_outb(RTC_ADDR_REG, RTC_SECONDS);
	sys_inb(RTC_DATA_REG, sec);

	do
	{
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &res);
	} while(res & 0x80);

	sys_outb(RTC_ADDR_REG, RTC_MINUTES);
	sys_inb(RTC_DATA_REG, min);

	do
	{
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &res);
	} while(res & 0x80);


	sys_outb(RTC_ADDR_REG, RTC_HOURS);
	sys_inb(RTC_DATA_REG, hour);

	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &res);

	if(!(res & 0x04))
	{
		*sec = bcd_convert(*sec);
		*min = bcd_convert(*min);
		*hour = bcd_convert(*hour);
	}

	return 0;
}


void rtc_draw_current_time(unsigned int x, unsigned int y){
	unsigned long hour,min,sec;
	char temp[3];

	rtc_current_time(&hour, &min, &sec);

	//draws hours
	sprintf(temp, "%d", hour);
	if (hour < 10){
		temp[2] = temp[1];
		temp[1] = temp[0];
		temp[0] = '0';
	}
	vg_string(temp,x,y, 2, WHITE);

	//draws ":"
	x += 40;
	vg_string(":",x,y, 2, WHITE);

	//draws minutes
	x += 10;
	sprintf(temp, "%d", min);
	if (min < 10){
		temp[2] = temp[1];
		temp[1] = temp[0];
		temp[0] = '0';
	}
	vg_string(temp,x,y, 2, WHITE);

	//draws ":"
	x += 40;
	vg_string(":",x,y, 2, WHITE);

	//draws seconds
	x += 10;
	sprintf(temp, "%d", sec);
	if (sec < 10){
		temp[2] = temp[1];
		temp[1] = temp[0];
		temp[0] = '0';
	}
	vg_string(temp,x,y, 2, WHITE);
}


void rtc_get_highscores(unsigned int highscores[3]){
	unsigned long temp;

	sys_outb(RTC_ADDR_REG, HIGH_REG_1);
	sys_inb(RTC_DATA_REG, &temp);
	highscores[0]= (unsigned int)temp *  HIGHSCORES_MULTIPLE;

	sys_outb(RTC_ADDR_REG, HIGH_REG_2);
	sys_inb(RTC_DATA_REG, &temp);
	highscores[1]= (unsigned int)temp *  HIGHSCORES_MULTIPLE;

	sys_outb(RTC_ADDR_REG, HIGH_REG_3);
	sys_inb(RTC_DATA_REG, &temp);
	highscores[2]= (unsigned int)temp *  HIGHSCORES_MULTIPLE;
}

void rtc_set_highscores(unsigned int highscores[3]){
	unsigned long temp;

	temp = highscores[0]/ HIGHSCORES_MULTIPLE;
	sys_outb(RTC_ADDR_REG, HIGH_REG_1);
	sys_outb(RTC_DATA_REG, temp);


	temp = highscores[1]/50;
	sys_outb(RTC_ADDR_REG, HIGH_REG_2);
	sys_outb(RTC_DATA_REG, temp);

	temp = highscores[2]/50;
	sys_outb(RTC_ADDR_REG, HIGH_REG_3);
	sys_outb(RTC_DATA_REG, temp);
}
