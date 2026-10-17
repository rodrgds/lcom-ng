#include "rtc.h"

int rtc_hook_id=8;
uint8_t hour;
uint8_t minute;

int (rtc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = rtc_hook_id;
  if (sys_irqsetpolicy(8, (IRQ_REENABLE | IRQ_EXCLUSIVE), &rtc_hook_id)){
    return 1;
  }
  uint32_t tempRegB;
  if (sys_outb(0x70, 11)){
    return 1;
  }
  if (sys_inb(0x71, &tempRegB) != 0){
    return 1;
  }

  tempRegB = tempRegB | BIT(4);
  

  if (sys_outb(0x70, 11)){
    return 1;
  }
  if (sys_outb(0x71, tempRegB)){
    return 1;
  }

  

  return 0;
}


int (rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&rtc_hook_id)){
    return 1;
  }

  uint32_t tempRegB;
  if (sys_outb(0x70, 11)){
    return 1;
  }
  if (sys_inb(0x71, &tempRegB) != 0){
    return 1;
  }

  tempRegB = tempRegB & ~BIT(4);
  

  if (sys_outb(0x70, 11)){
    return 1;
  }
  if (sys_outb(0x71, tempRegB)){
    return 1;
  }

  
  

  return 0;
}

void waitUpdate(){
  uint32_t regA = 0;
  if (sys_irqdisable(&rtc_hook_id)) {
      
    }

  if (sys_outb(0x70, 10)) {
    
  }
  if (sys_inb(0x71, &regA)) {
    
  }

  if (sys_irqenable(&rtc_hook_id)) {
    
  }

  while (regA & BIT(7)){
    if (sys_irqdisable(&rtc_hook_id)) {
      
    }

    if (sys_outb(0x70, 10)) {
      
    }
    if (sys_inb(0x71, &regA)) {
     
    }

    if (sys_irqenable(&rtc_hook_id)) {
     
    }
  }


}

int hexInt(uint8_t time){
  int digit2 = (time>>4);
  uint8_t temp = 0x0F;
  temp = temp & time;
  int digit1=temp;
  time = (digit2*10 + digit1);
  return time;
} 

void (rtc_ih)() {
  uint32_t regC;

  sys_outb(0x70, 12);
  sys_inb(0x71, &regC);

  if (regC & BIT(4)) {
    waitUpdate();
    uint32_t hours;
    if (sys_outb(0x70, 4)) {
      
    }
    if (sys_inb(0x71, &hours)) {
      
    }
    hour=hours;
    
  
    sys_outb(0x70, 12);
    sys_inb(0x71, &regC);

    waitUpdate();
    uint32_t minutes;
    if (sys_outb(0x70, 2)) {
      
    }
    if (sys_inb(0x71, &minutes)) {
      
    }
    minute=minutes;
    hour=hexInt(hour);
    minute=hexInt(minute);

  }
  
}
