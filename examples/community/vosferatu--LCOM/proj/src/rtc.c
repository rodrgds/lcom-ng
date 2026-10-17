#include <minix/syslib.h>
#include <minix/drivers.h>
#include "rtc.h"
#include <stdlib.h>
#include <math.h>

int rtc_hook = RTC_IRQ;

Date* date = NULL;

void newDate(){

	date = (Date*) malloc(sizeof(Date));


	date->x = 734;
	date->y = 0;

	date->num0 = loadBitmap("0small.bmp");
	date->num1 = loadBitmap("1small.bmp");
	date->num2 = loadBitmap("2small.bmp");
	date->num3 = loadBitmap("3small.bmp");
	date->num4 = loadBitmap("4small.bmp");
	date->num5 = loadBitmap("5small.bmp");
	date->num6 = loadBitmap("6small.bmp");
	date->num7 = loadBitmap("7small.bmp");
	date->num8 = loadBitmap("8small.bmp");
	date->num9 = loadBitmap("9small.bmp");
	date->dots = loadBitmap("dots.bmp");

	date->w = date->num1->bitmapInfoHeader.width;
	date->h = date->num1->bitmapInfoHeader.height;

	date->minutes = 70;
	date->hours = 70;

}

Date* getDate(){
	if (date == NULL){
		newDate();
	}

	return date;
}

void drawDate(){
	int minutos; minutos= getDate()->minutes;
	int horas; horas = getDate()->hours;

	if (minutos > 65) //avoid drawing 00:00 on screen before rtc updates
		return;
	if (horas > 65)
		return;

	printf("%d:%d\n", horas, minutos);

	if (horas >= 10)
		drawSmall(floor(horas/10.0), date->x, date->y);
	else drawSmall(0, date->x, date->y);

	drawSmall((horas % 10), date->x + date->w, date->y);

	drawBitmapT(date->dots, date->x + date->w + 14, date->y, ALIGN_LEFT);

	if (minutos >= 10)
		drawSmall(floor(minutos/10.0), date->x + 2*date->w + 10, date->y);
	else drawSmall(0, date->x + 2*date->w + 10, date->y);

	drawSmall((minutos % 10), date->x + 3*date->w + 10, date->y);

}

void drawSmall(int number, int x, int y){

	switch(number){
		case 0:
			drawBitmapT(date->num0, x, y, ALIGN_LEFT);
			break;
		case 1:
			drawBitmapT(date->num1, x, y, ALIGN_LEFT);
			break;
		case 2:
			drawBitmapT(date->num2, x, y, ALIGN_LEFT);
			break;
		case 3:
			drawBitmapT(date->num3, x, y, ALIGN_LEFT);
			break;
		case 4:
			drawBitmapT(date->num4, x, y, ALIGN_LEFT);
			break;
		case 5:
			drawBitmapT(date->num5, x, y, ALIGN_LEFT);
			break;
		case 6:
			drawBitmapT(date->num6, x, y, ALIGN_LEFT);
			break;
		case 7:
			drawBitmapT(date->num7, x, y, ALIGN_LEFT);
			break;
		case 8:
			drawBitmapT(date->num8, x, y, ALIGN_LEFT);
			break;
		case 9:
			drawBitmapT(date->num9, x, y, ALIGN_LEFT);
			break;
		default:
			break;
	}

}

void deleteDate(){
	if(date == NULL){
		free(date);
		return;
	}

	deleteBitmap(date->num0);
	deleteBitmap(date->num1);
	deleteBitmap(date->num2);
	deleteBitmap(date->num3);
	deleteBitmap(date->num4);
	deleteBitmap(date->num5);
	deleteBitmap(date->num6);
	deleteBitmap(date->num7);
	deleteBitmap(date->num8);
	deleteBitmap(date->num9);
	deleteBitmap(date->dots);


	free(date);

}

int rtc_unsubscribe_int(){

	if (sys_irqdisable(&rtc_hook) != OK) return 1;
	if (sys_irqrmpolicy(&rtc_hook) != OK) return 1;

	return 0;
}

int rtc_subscribe_int(){
	unsigned long regB;
	regB = BIT(4) | BIT(1); //to set 24 hours

	if (sys_outb(RTC_ADDR_REG, REG_B) != OK)
		return 1;

	if (sys_outb(RTC_DATA_REG, (RTC_UF | BIT(0))) != OK)
		return 1;

	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook) != OK || sys_irqenable(&rtc_hook) != OK)
		return 1;

	sys_outb(RTC_ADDR_REG, REG_B);
	sys_outb(RTC_DATA_REG, regB);

	return BIT(RTC_IRQ);
}

unsigned char bcdToBinary(unsigned char bcd){

	//http://www.electro-tech-online.com/threads/convert-bcd-to-binary.41808/
	unsigned int result;
	result=(bcd>>12)*1000;
	result+=((bcd>>8)&0x0f)*100;
	result+=((bcd>>4)&0x0f)*10;
	result+=((bcd)&0x0f);
	return result;
}

int rtc_enable(){
	long unsigned int regB;

	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	regB = (regB & ~BIT(7));    //when set bit is 0 functions normally (datasheet)

    sys_outb(RTC_ADDR_REG, REG_B);
    sys_outb(RTC_DATA_REG, regB);

    return 0;
}

int rtc_disable(){

	long unsigned int regB;

	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	regB = (regB | BIT(7));//when SET bit of regB is 1 inhibits updates (datasheet)

    sys_outb(RTC_ADDR_REG, REG_B);
    sys_outb(RTC_DATA_REG, regB);

    return 0;
}


void rtc_int_handler(){
	unsigned long seconds, minutos, horas, regA, regB, regC;

	//rtc_disable();

	do{
		sys_outb(RTC_ADDR_REG, REG_A);
		sys_inb(RTC_DATA_REG, &regA);
	} while (regA & RTC_UIP); //validation

	sys_outb(RTC_ADDR_REG, REG_C); //clear IRQF
	sys_inb(RTC_DATA_REG, &regC);

	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, &seconds);

	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, &minutos);

	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, &horas);


	seconds = bcdToBinary(seconds);
	seconds++;
	minutos = bcdToBinary(minutos);
	horas = bcdToBinary(horas);

	if (seconds == 60) minutos++;
	if (minutos == 60) horas++;

	getDate()->minutes = minutos;
	getDate()->hours = horas;

	//rtc_enable();
}
