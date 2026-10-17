/** @file rtc.c
 * @brief file containing the function definitions to handle the RTC
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include "rtc.h"
#include "rtc_static.h"

#define DEBUG_MALLOC 0

DateTime *create_datetime() {
	DateTime *datetime = (DateTime *)malloc(sizeof(DateTime));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for datetime\n", sizeof(DateTime));
	if (datetime == NULL) {
		printf("create_datetime: error allocating space for datetime");
		return NULL;
	}
	datetime->seconds = 0;
	datetime->minutes = 0;
	datetime->hours = 0;
	datetime->day = 0;
	datetime->month = 0;
	datetime->year = 0;
	return datetime;
}

RTC *create_rtc() {
	RTC *rtc = (RTC *)malloc(sizeof(RTC));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for rtc\n", sizeof(RTC));
	if (rtc == NULL) {
		printf("create_rtc: error allocating space for rtc");
		return NULL;
	}
	unsigned short i;
	for (i = 0; i < 10; i++)
		rtc->registers[i] = 0;
	if ((rtc->datetime = create_datetime()) == NULL) {
	    free(rtc);
		if (DEBUG_MALLOC) printf("Freed rtc\n");
		return NULL;
	}
	return rtc;
}

void destroy_rtc(RTC *rtc) {
    free(rtc->datetime);
	if (DEBUG_MALLOC) printf("Freed datetime\n");
    free(rtc);
	if (DEBUG_MALLOC) printf("Freed rtc\n");
}

static void wait_valid_rtc() {
    unsigned long registerA = 0;
    do {
        sys_outb(RTC_ADDR_REG, RTC_REGISTER_A);
        sys_inb(RTC_DATA_REG, &registerA);
    } while (registerA & UIP_REGISTER_A);
}

static unsigned short bcd2dec(unsigned long bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void get_data_rtc(RTC *rtc) {
	wait_valid_rtc();
	unsigned short i;
	for (i = 0; i < 10; i++) {
        sys_outb(RTC_ADDR_REG, i);
        sys_inb(RTC_DATA_REG, &rtc->registers[i]);
	}
	rtc->datetime->seconds = bcd2dec(rtc->registers[RTC_SECONDS]);
	rtc->datetime->minutes = bcd2dec(rtc->registers[RTC_MINUTES]);
	rtc->datetime->hours = bcd2dec(rtc->registers[RTC_HOURS]);
	rtc->datetime->day = bcd2dec(rtc->registers[RTC_MONTH_DAY]);
	rtc->datetime->month = bcd2dec(rtc->registers[RTC_MONTH]);
	rtc->datetime->year = 2000 + bcd2dec(rtc->registers[RTC_YEAR]);
}


