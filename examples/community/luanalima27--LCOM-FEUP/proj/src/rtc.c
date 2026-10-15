#include <lcom/lcf.h>

#include "rtc.h"

uint8_t hours = 0;
int rtc_output(uint8_t command, uint8_t *output){
  if (sys_outb(0x70, command) != 0){
    return 1;
  }
  if (util_sys_inb(0x71,output)!= 0){
    return 1;
  }
  return 0;
}

uint8_t bcd_to_binary (uint8_t num){
  return ((num >> 4)* 10) + (num & 0xF);
}



int rtc_getTime(){
  
  //See if its Binary or BCD
  uint8_t mode;
  //uint8_t isBinary;
  if (rtc_output(11, &mode) != 0){
    return 1;
  }
  
  
  bool isBinary = mode & BIT(2);
  
  
  uint8_t output = 0;
  if (rtc_output(4,&output)){
    return 1;
  }
  hours = output;
  if (hours < 4){
    hours += 24;
  }
  hours -=4;
  
 
  
  //Binary mode is active if mode
  if (isBinary){
    hours = bcd_to_binary(hours);
    
  }
   
  return 0;
}

