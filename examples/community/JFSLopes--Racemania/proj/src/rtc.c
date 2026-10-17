#include "rtc.h"
#include "rtc_macros.h"

bool alarm_out = false;
int hook_id_rtc = 8;

int (rtc_subscribe_int)(uint8_t *bit_no){
	if (bit_no == NULL) return -1;

	*bit_no = hook_id_rtc;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id_rtc) != 0){
		printf("Error subscribing rtc.\n");
		return -1;
	}
	return 0;
}

int (rtc_unsubscribe_int)() {
	if (sys_irqrmpolicy(&hook_id_rtc) != 0){
		printf("Error unsubscribing rtc.\n");
		return -1;
	}
	return 0;
}

int (read_register) (uint32_t reg, uint8_t* value){
	// Write the register to be read
	if (sys_outb(RTC_ADDR_REG, reg) != 0) return -1;
	// Read the register
	if (util_sys_inb(RTC_DATA_REG, value) != 0) return -1;
	return 0;
}

int (write_register) (uint32_t reg, uint32_t value){
	// Write the register to be written
	if (sys_outb(RTC_ADDR_REG, reg) != 0) return -1;
	// Read the register
	if (sys_outb(RTC_DATA_REG, value) != 0) return -1;
	return 0;
}

int (rtc_read_conf) (){
	// Read Registers
	uint8_t register_A, register_B, register_D;
	if (read_register(RTC_REGISTER_A, &register_A) != 0) return -1;
	if (read_register(RTC_REGISTER_B, &register_B) != 0) return -1;
	if (read_register(RTC_REGISTER_D, &register_D) != 0) return -1;

	// Display the config
	printf("A\n");
	if (register_A & 0x80) printf("Update in progress\n");
	else printf("Update not in progress\n");

	printf("B\n");
	if (register_B & 0x80) printf("Inhibit updates of date/time registers\n");
	else printf("Updates of date/time registers enable\n");

	if (register_B & 0x40) printf("PI enable\n");
	else printf("PI disable\n");

	if (register_B & 0x20) printf("AI enable\n");
	else printf("AI enable\n");

	if (register_B & 0x10) printf("AI enable\n");
	else printf("AI enable\n");

	if (register_B & 0x08) printf("Enable square-wave\n");
	else printf("Disable square-wave\n");

	if (register_B & 0x04) printf("Alarm and date in binary\n");
	else printf("Alarm and date in BCD\n");

	if (register_B & 0x02) printf("Hours in 0-23 format\n");
	else printf("Hours in 1-12 format\n");

	if (register_B & 0x01) printf("Enable Daylight Savings\n");
	else printf("Disable Daylight Savings\n");

	printf("D\n");
	if (register_D & 0x80) printf("Batery state: good\n");
	else printf("Batery state: bad\n");
	return 0;
}

/**
 * -1 if error, 0 if is updating, 1 if not updating
*/
int (update_in_progress) (){
	uint8_t register_A;
	if (read_register(RTC_REGISTER_A, &register_A) != 0) return -1;

	bool update = register_A & 0x80;
	if (update) return 0;
	return 1;
}

void (convert_bcd_decimal) (uint8_t* value){
	uint8_t decimal = (*value & 0xF0) >> 4;
	uint8_t unity = *value & 0x0F;
	*value = decimal * 10 + unity;
}

void (convert_decimal_bcd) (uint8_t* value){
	uint8_t first_hex = (*value / 10) << 4;
	uint8_t last_hex = *value % 10;
	*value = first_hex | last_hex;
}

void (adjust_dt_values) (date_time* dt){
	convert_bcd_decimal(&dt->seconds);
	convert_bcd_decimal(&dt->minutes);
	convert_bcd_decimal(&dt->hours);
	convert_bcd_decimal(&dt->day);
	convert_bcd_decimal(&dt->day_week);
	convert_bcd_decimal(&dt->month);
	convert_bcd_decimal(&dt->year);
}

int (rtc_get_datetime) (date_time* dt){
	int ret;
	while (true){
		ret = update_in_progress();
		if (ret == -1) return -1;
		else if (ret == 1) break;
	}
	// Seconds
	if (read_register(RTC_SECONDS, &dt->seconds) != 0) return -1;

	// Minutes
	if (read_register(RTC_MINUTES, &dt->minutes) != 0) return -1;

	// Hours
	if (read_register(RTC_HOURS, &dt->hours) != 0) return -1;

	// Day of the month
	if (read_register(RTC_DAY_OF_MONTH, &dt->day) != 0) return -1;

	// Day of the Week
	if (read_register(RTC_DAY_OF_WEEK, &dt->day_week) != 0) return -1;

	// Month
	if (read_register(RTC_MONTH, &dt->month) != 0) return -1;

	// Year
	if (read_register(RTC_YEAR, &dt->year) != 0) return -1;
	adjust_dt_values(dt);
	return 0;
}

void (show_date) (date_time* dt) {
    printf("%04d-%02d-%02d", 2000 + dt->year, dt->month, dt->day);
}

void (show_time) (date_time* dt) {
    printf("%02d:%02d:%02d", dt->hours, dt->minutes, dt->seconds);
}

int (set_alarm) (uint32_t sec){
	/// sec cannot be greater than a day
	sec = sec % (24 * 60 * 60);
	int ret;
	while (true){
		ret = update_in_progress();
		if (ret == -1) return -1;
		else if (ret == 1) break;
	}
	date_time curr;
	if (rtc_get_datetime(&curr) != 0) return -1;

	uint8_t hour_offset, minute_offset, second_offset;
	hour_offset = sec / 3600;
	sec %= 3600;
	minute_offset = sec / 60;
	second_offset = sec % 60;

	uint8_t hours, minutes, seconds, overflow;
	seconds = (curr.seconds + second_offset) % 60;
	overflow = (curr.seconds + second_offset) / 60;

	minutes = (curr.minutes + minute_offset + overflow) % 60;
	overflow = (curr.minutes + minute_offset + overflow) / 60;

	hours = (curr.hours + hour_offset + overflow) % 24;

	printf("Alarm at: %02d:%02d:%02d", hours, minutes, seconds);

	convert_decimal_bcd(&seconds);
	convert_decimal_bcd(&minutes);
	convert_decimal_bcd(&hours);

	/// Write to the registers
	if (write_register(RTC_SECONDS_ALARM, (uint32_t) seconds) != 0) return -1;
	if (write_register(RTC_MINUTES_ALARM, (uint32_t) minutes) != 0) return -1;
	if (write_register(RTC_HOURS_ALARM, (uint32_t) hours) != 0) return -1;
	return 0;
}

int (active_alarm_int) (){
	uint8_t register_B;
	if (read_register(RTC_REGISTER_B, &register_B) != 0) return -1;
	/// Activate alarm interrupts
	register_B |= 0x20;

	if (write_register(RTC_REGISTER_B, register_B) != 0) return -1;
	return 0;
}

int (disable_alarm_int) (){
    uint8_t register_B;
    if (read_register(RTC_REGISTER_B, &register_B) != 0) return -1;
    /// Activate alarm interrupts
    register_B &= 0xDF;

    if (write_register(RTC_REGISTER_B, register_B) != 0) return -1;
    return 0;
}

int (rtc_ih) (){
	uint8_t register_c;
	if (read_register(RTC_REGISTER_C, &register_c) != 0) return -1;

	if (register_c & 0x40){
		/// Periodic interrupt
	}
	if (register_c & 0x20){
		/// Alarm interrupt
		alarm_out = true;
		return 2;
	}
	if (register_c & 0x10){
		/// Update interrupt
	}
	return 0;
}
