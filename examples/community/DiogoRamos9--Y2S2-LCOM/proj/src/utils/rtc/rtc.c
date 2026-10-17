#include <lcom/lcf.h>

#include "rtc.h"

uint8_t temportc[2];
uint8_t datartc[3];

int rtc_hookid = 8;

int (rtc_subscribe_int)(uint8_t *bit_no) {
	if(bit_no == NULL){
		return 1;
	}
	*bit_no = BIT(rtc_hookid);
	if (sys_irqsetpolicy(rtc_irql, (IRQ_REENABLE | IRQ_EXCLUSIVE), &rtc_hookid) != 0) {
		return 1;
	}

	return 0;
}

int (rtc_unsubscribe_int)() {
	if (sys_irqrmpolicy(&rtc_hookid) != 0) {
		return 1;
	}

	return 0;
}

int rtc_output(uint8_t command, uint8_t *output) {
	if (sys_outb(inp_reg, command) != 0) return 1;
	if (util_sys_inb(out_reg, output) != 0) return 1;
	return 0;
}

int (rtcupdating)() {
    uint8_t rslt;
    if (rtc_output(upd_reg, &rslt) != 0) {
        return 1;
    }
	return rslt & updt;
}

int (rtc_read)(uint8_t inp){
	if (inp != minrtc && inp != hoursrtc && inp != dayrtc && inp != monthrtc && inp != yearrtc) {
		return 1;
	}

	uint8_t out;

	if (rtc_output(inp, &out) != 0) {
		return 1;
	}

	if(inp==minrtc){
		temportc[0] = convert_tobin((uint8_t) (out));
	}
	else if(inp==hoursrtc){
		temportc[1] = convert_tobin((uint8_t) (out));
	}
	else if(inp==dayrtc){
		datartc[0] = convert_tobin((uint8_t) (out));
	}
	else if(inp==monthrtc){
		datartc[1] = convert_tobin((uint8_t) (out));
	}
	else if(inp==yearrtc){
		datartc[2] = convert_tobin((uint8_t) (out));
	}

	return out;
}

uint8_t convert_tobin (uint8_t bcd) {
	uint8_t binar = (bcd >> 4) * 10;
	binar += (bcd & 0x0F);

	return binar;
}


void (LoadRtc)() {
	rtc_read(dayrtc);
	rtc_read(monthrtc);
	rtc_read(yearrtc);
	rtc_read(minrtc);
	rtc_read(hoursrtc);
}
