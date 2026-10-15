#include "rtc.h"
#include "utils.c"

int (get_date)(uint8_t *day, uint8_t *month, uint8_t *year){

  set_binary();

  //read the day information
  if(read_RTC(DAY, day) != 0){
    return EXIT_FAILURE;
  }

  //read the month information
  if(read_RTC(MONTH, month) != 0){
    return EXIT_FAILURE;
  }

  //read the year information
  if(read_RTC(YEAR, year) != 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int (set_binary)(){
  //sets the values in the RTC to binary

  //firstly read the current config
  uint8_t value = 0;
  if(sys_outb(RTC_ADDR_REG, REG_B) != 0){
    return EXIT_FAILURE;
  }
  if(util_sys_inb(RTC_DATA_REG, &value) != 0){
    return EXIT_FAILURE;
  }

  //update value
  value = value | REG_B_DM;

  //insert the updated value
  if(sys_outb(RTC_ADDR_REG, REG_B) != 0){
    return EXIT_FAILURE;
  }
  if(sys_outb(RTC_DATA_REG, value) != 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int (read_RTC)(uint8_t command, uint8_t *output){

  //firstly checks if the RTC is updating (UIP) 
  uint8_t updating = 0;
  while(true){
    if(sys_outb(RTC_ADDR_REG, REG_A) != 0){
      return EXIT_FAILURE;
    }
    if(util_sys_inb(RTC_DATA_REG, &updating) != 0){
      return EXIT_FAILURE;
    }

    if((updating & REG_A_UIP) == 0){
      break;
    }
  }
  
  //check if the values are in binary
  uint8_t bin = 0;
  if(sys_outb(RTC_ADDR_REG, REG_B) != 0){
    return EXIT_FAILURE;
  }
  if(util_sys_inb(RTC_DATA_REG, &bin) != 0){
    return EXIT_FAILURE;
  }

  if((bin & REG_B_DM) == 0) {
    //the values are in BCD and need to be set to binary
    return EXIT_FAILURE;
  }

  //If the rtc isn't updating, reads the value
  if(sys_outb(RTC_ADDR_REG, command) != 0){
    return EXIT_FAILURE;
  }
  if(util_sys_inb(RTC_DATA_REG, output) != 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


