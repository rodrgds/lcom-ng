#include <stdlib.h>
#include <stdio.h>
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/bitmap.h>

#include "rtc.h"

unsigned long bcd_to_dec(unsigned long bcd);

void rtc_wait_valid(void) {
	unsigned long reg_a, reg_b;
    
	rtc_read_register(RTC_REG_A, &reg_a);
	rtc_read_register(RTC_REG_B, &reg_b);
    
	bit_set(reg_a, RTC_SQWE_BIT);
	bit_unset(reg_b, RTC_SQWE_BIT);
    
	rtc_write_register(RTC_REG_A, reg_a);
	rtc_write_register(RTC_REG_B, reg_b);
    
	do {
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		
        if (sys_inb(RTC_DATA_REG, &reg_a))
            return;
        
	} while (bit_isset(reg_a, REG_UIP_BIT));
}

unsigned long bcd_to_dec(unsigned long bcd) {
	char buf[20];
    
	sprintf(buf, "%x", bcd);
    
	return strtoul(buf, NULL, 10);
}

int rtc_read_register(unsigned long reg, unsigned long *value) {
    int res;
    unsigned long data;
    
    if (res = sys_outb(RTC_ADDR_REG, reg))
        return res;
    
    if (res = sys_inb(RTC_DATA_REG, &data))
        return res;
    
    *value = data;
    
    return 0;
}

int rtc_write_register(unsigned long reg, unsigned long value) {
    int res;
    
    if (res = sys_outb(RTC_ADDR_REG, reg))
        return res;
    
    if (res = sys_outb(RTC_DATA_REG, value))
        return res;
    
    return 0;
}

void rtc_get_time(int *hours, int *minutes, int *seconds) {
    long unsigned int sec_tmp, min_tmp, hrs_tmp;
    
    rtc_read_register(RTC_LOC_SECONDS, &sec_tmp);
    rtc_read_register(RTC_LOC_MINUTES, &min_tmp);
    rtc_read_register(RTC_LOC_HOURS, &hrs_tmp);
    
    *hours = bcd_to_dec(hrs_tmp);
    *minutes = bcd_to_dec(min_tmp);
    *seconds = bcd_to_dec(sec_tmp);
}
