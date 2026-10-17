#include <lcom/lcf.h>
#include <rtc.h>


int RTC_HOOK_ID = RTC_IRQ;

/*int rtc_enable() {
    if (sys_irqenable(&RTC_HOOK_ID) != OK)
        return 1;
    return 0;
}

int rtc_disable() {
    if (sys_irqdisable(&RTC_HOOK_ID) != OK)
        return 1;
    return 0;
}

int rtc_subscribe_int(uint8_t* bit_no) {
    *bit_no = RTC_HOOK_ID;
    uint8_t bStatus;

    if (sys_irqsetpolicy(RTC_IRQ,IRQ_REENABLE,&RTC_HOOK_ID) != OK)
        return 1;

    //writing the address of the B register to RTC_ADDR_REG
    if (sys_outb(RTC_ADDR_REG,RTC_REG_B) != 0)
        return 1;
    //reading byte from RTC_DATA_REG
    if (util_sys_inb(RTC_DATA_REG,&bStatus) != 0)
        return 1;

    bStatus = bStatus | RTC_UIE | RTC_AIE; //enable update and alarm interrupts

    //writing the address of the B register to RTC_ADDR_REG
    if (sys_outb(RTC_ADDR_REG,RTC_REG_B) != 0)
        return 1;
    //writing new status byte
    if (sys_outb(RTC_DATA_REG,bStatus) != 0)
        return 1;

    return 0;
}

int rtc_unsubscribe_int() {
    uint32_t bStatus;

    if (sys_irqrmpolicy(&RTC_HOOK_ID) != 0)
        return 1;

    if (sys_outb(RTC_ADDR_REG,RTC_REG_B) != 0)
        return 1;
    if (sys_inb(RTC_DATA_REG,&bStatus) != 0) 
        return 1;

    bStatus = (bStatus & (~RTC_UIE)) & (~RTC_AIE);//disable update and alarm interrupts

    if (sys_outb(RTC_ADDR_REG,RTC_REG_B) != 0)
        return 1;
    if (sys_outb(RTC_DATA_REG,bStatus) != 0) 
        return 1;

    return 0;
}*/

int rtc_read(uint32_t reg, uint32_t* data)
{   
    if(sys_outb(RTC_ADDR_REG, reg) != OK) return 1;
    if(sys_inb(RTC_DATA_REG, data) != OK) return 1;

    return 0;
}

void bcd_to_binary(uint32_t * val) {
	*val=((((*val) & 0xF0) >> 4) * 10 + ((*val) & 0x0F));
}

int isUpdating(){
	uint32_t Reg_A = 0;
	sys_outb(RTC_ADDR_REG, RTC_REG_A);
	sys_inb(RTC_DATA_REG, &Reg_A);

	if ((Reg_A & RTC_UIP) != 0)
		return 1;

	return 0;
}

bool bcd(){
	uint32_t Reg_B = 0;
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &Reg_B);
	if (!(Reg_B & RTC_DM))
		return true;
	else return false;
}

realTime rtc_read_date()
{
	uint32_t day; 
	uint32_t month;
	uint32_t year;

  realTime rt;

  while(isUpdating() != 0) {}

	rtc_read(RTC_YEAR,&year);
	rtc_read(RTC_MONTH,&month);
	rtc_read(RTC_DAY, &day);

  if(bcd()){
	  bcd_to_binary(&day);
	  bcd_to_binary(&month);
	  bcd_to_binary(&year);
  }
  rt.day = day;
  rt.month = month;
  rt.year = year;

  uint32_t minutes;
	uint32_t hour;

  while(isUpdating() != 0) {}

	rtc_read(RTC_MIN,&minutes);
	rtc_read(RTC_HOUR,&hour);

  if(bcd()){
	  bcd_to_binary(&minutes);
	  bcd_to_binary(&hour);
  }
  rt.minutes = minutes;
  rt.hour = hour;

	return rt;
}
