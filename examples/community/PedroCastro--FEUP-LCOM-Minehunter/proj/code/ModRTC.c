#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <machine/int86.h>
#include <stdlib.h>
#include <stdio.h>
#include "ModRTC.h"




unsigned long regA;
unsigned long regB;
unsigned long regC;

int scan_rtc(unsigned long index,  unsigned long *dado){
        if(sys_outb(RTC_ADDR_REG, index) != OK)
                return 1;
        if(sys_inb(RTC_DATA_REG, dado) != OK)
                return 1;
        return 0;
}

int write_rtc(unsigned long index, unsigned long dado){
        if(sys_outb(RTC_ADDR_REG, index) != OK)
                return 1;
        if(sys_outb(RTC_DATA_REG, dado) != OK)
                return 1;
        return 0;
}

void disable(){

        long unsigned int reg_b = scan_rtc(RTC_REG_B,&reg_b);
        reg_b |= BIT(7);
        write_rtc(RTC_REG_B, reg_b);

}


void enable(){
        long unsigned int reg_b = scan_rtc(RTC_REG_B,&reg_b);
        reg_b &= ~BIT(7);
        write_rtc(RTC_REG_B, reg_b);
}

void wait_valid_rtc(void) {
        unsigned long regA = 0;
        do {
                disable();
                sys_outb(RTC_ADDR_REG, RTC_REG_A);
                sys_inb(RTC_DATA_REG, &regA);
                enable();
        } while ( regA & RTC_UIP);
}

unsigned char bcd_to_binary(unsigned char bcdvalue)
{
        char buff[32];
        sprintf(buff, "%x", bcdvalue);
        sscanf(buff, "%d", &bcdvalue);
        return bcdvalue;
}
unsigned char binary_to_bcd(unsigned char binaryvalue)
{
        char buff[32];
        sprintf(buff, "%d", binaryvalue);
        sscanf(buff, "%x", &binaryvalue);
        return binaryvalue;
}

int rtc_date(){

        disable();

        scan_rtc(RTC_REG_A, &regA);
        if(regA & RTC_UIP){
                wait_valid_rtc();
        }

        scan_rtc(RTC_SEC, &(sec));
        scan_rtc(RTC_MIN, &(min));
        scan_rtc(RTC_HOUR, &(hour));
        scan_rtc(RTC_DAY_MONTH, &(day_month));
        scan_rtc(RTC_DAY_WEEK, &(day_week));
        scan_rtc(RTC_MONTH, &(month));
        scan_rtc(RTC_YEAR, &(year));




        scan_rtc(RTC_REG_B, &regB);


        if(!(regB & RTC_DATA_MODE)) {

        	day_week = bcd_to_binary(day_week);
        	day_month = bcd_to_binary(day_month);
        	year = bcd_to_binary(year);
        	month = bcd_to_binary(month);

        	int pm = 0;
        	if(!(regB & BIT(1))){
        		if(hour & BIT(7)){
        			hour &= ~BIT(7);
        			pm = 1;
        		}
        	}
        	hour = bcd_to_binary(hour);
        	if(pm == 1)
        		hour += 12;
        	min = bcd_to_binary(min);
        	sec = bcd_to_binary(sec);
        }

        enable();
        return 0;
}
