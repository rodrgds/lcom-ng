#include <lcom/lcf.h>

#include "Date.h"

extern uint8_t temportc[2];
extern uint8_t datartc[3];

bool first_time = true;

Date * date;

Date * Loaddate() {
	date = (Date*) malloc(sizeof(Date));
	
	date->min = 0;
	date->hour = 0;
	date->day = 0;
	date->month = 0;
	date->year = 0;
	

	return date;
}

void updtdate() { 
    rtc_read(minrtc);
    rtc_read(hoursrtc);
    rtc_read(dayrtc);
    rtc_read(monthrtc);
    rtc_read(yearrtc);
    date->min = temportc[0];
    date->hour = temportc[1];
    date->day = datartc[0];
    date->month = datartc[1];
    date->year = datartc[2];
}

void get_date_time(){
    if (first_time){
      Loaddate();
      first_time = false;
    }
    updtdate();
}



