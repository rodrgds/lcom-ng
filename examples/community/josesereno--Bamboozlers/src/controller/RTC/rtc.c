#include "rtc.h"

int rtc_hook_id = 3;

uint8_t hour = 0, minute = 0, second = 0;

bool darkMode=false;

int (rtc_subscribe_int)(uint8_t *bit_no) {
  *bit_no=BIT(rtc_hook_id);
  return sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id);
}

int (rtc_unsubscribe_int)() {
  return sys_irqrmpolicy(&rtc_hook_id);
}

int (enable_update)(){
    return sys_irqenable(&rtc_hook_id);
}

int (disable_update)(){
    return sys_irqdisable(&rtc_hook_id);
}


int (wait_valid_rtc)(){

  uint8_t regA=0;

  do{
      if(disable_update()) return 1;
      if(sys_outb(ADDR_REG,REG_A)) return 1;
      if(util_sys_inb(DATA_REG,&regA)) return 1;
      if(enable_update()) return 1;
  } while(regA & UIP);

  return 0;
}
void (rtc_ih)(){
    uint8_t data;
    if(sys_outb(ADDR_REG, REG_C)) return ;
    if(util_sys_inb(DATA_REG, &data)) return ;
    if(data & UF) rtc_upd();
    if(data & AF) set_darkMode_alarm();
}

int (set_alarm_upd_interrupt)(bool value){
    uint8_t regB;
    if(sys_outb(ADDR_REG, REG_B)) return 1;
    if(util_sys_inb(DATA_REG, &regB)) return 1;
    
    if(value){
        regB = (UIE | regB);
        regB = (AIE | regB);
    }
    else{
        regB = (regB & (~UIE));
        regB = (regB & (~AIE));
    }

    if(sys_outb(ADDR_REG, REG_B)) return 1;
    if( sys_outb(DATA_REG, regB)) return 1;
    return 0;
}

int (rtc_read_hours)(uint8_t reg, uint8_t* hours){
    if (sys_outb(ADDR_REG, reg)) return 1;
    if (util_sys_inb(DATA_REG, hours)) return 1;
    return 0;
}

void (rtc_upd)(){
    disable_update();
    if (wait_valid_rtc()) return;
    if (rtc_read_hours(REG_HOUR, &hour)) return ;
    hour = ((hour & 0xF0) >> 4) * 10 + (hour & 0x0F);
    enable_update();
}

void (set_darkMode_alarm)(){
    uint8_t hour_alarm;
    if(wait_valid_rtc()) return;
    if(rtc_read_hours(REG_HOUR, &hour_alarm))return;

    if(hour >=20 || hour <6){
        hour_alarm=((6%10)) | ((6/10) << 4);
        darkMode=true;
    }
    else{
        darkMode=false;
        hour_alarm=((20%10)) |((20/10) << 4);
    }

    if(sys_outb(ADDR_REG, REG_SEC_ALARM)) return;
    if(sys_outb(DATA_REG, 0xFF)) return;

    if(sys_outb(ADDR_REG, REG_MIN_ALARM)) return;
    if(sys_outb(DATA_REG, 0xFF)) return;

    if(sys_outb(ADDR_REG, REG_HOUR_ALARM)) return;
    if(sys_outb(DATA_REG, hour_alarm))return;
}

