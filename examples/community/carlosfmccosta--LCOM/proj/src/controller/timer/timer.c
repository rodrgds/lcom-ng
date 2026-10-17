/**
 * @file timer.c
 * @brief Implementation of i8254 timer controller operations.
 *
 * This file contains the implementation of the functions declared in timer.h.
 */

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int hookid;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //verify if frequency is in the limits
  if (freq < 19 || freq > TIMER_FREQ) {
    printf("Frequence is out of range.\n");
    return 1;
  }

  //get timer configuration and preserve the 4 least significant bits while setting the timer correctly
  uint8_t  config;
  timer_get_conf(timer, &config);
  config = (config & ((BIT(0) | BIT(1) | BIT(2) | BIT(3)) | TIMER_SQR_WAVE)) | TIMER_LSB_MSB; //setting to LSB after MSB because we are sending LSB first and then MSB

  //get the freq
  uint16_t divisor = TIMER_FREQ / freq;

  uint8_t lsb, msb; //variables to store a byte of the frequency
  //place freq corresponding bytes in the vars
  util_get_LSB(divisor, &lsb);
  util_get_MSB(divisor, &msb);

  //select the correct timer and use the correct TIMER_SEL on configuration
  uint8_t timerSel;
  switch (timer) {
    case 0: 
      timerSel = TIMER_0;
      config |= TIMER_SEL0;
      break;
    case 1: 
      timerSel = TIMER_1;
      config |= TIMER_SEL1;
      break;
    case 2: 
      timerSel = TIMER_2;
      config |= TIMER_SEL2;
      break;
    default:
      printf("Invalid timer selection.\n");
      return 1;
  }

  //configure the timer and handle error from call to sys_outb
  if(sys_outb(TIMER_CTRL, config) != 0) {
    printf("Error configuring timer.\n");
    return 1;
  }
  if(sys_outb(timerSel, lsb) != 0) {
    printf("Error sending LSB to timer.\n");
    return 1;
  }
  if(sys_outb(timerSel, msb) != 0) {
    printf("Error sending MSB to timer.\n");
    return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hookid = *bit_no;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookid) == 0) {
    return 0;
  }
  printf("Error: Subscribing timer failed.\n");
  return 1;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hookid) == 0) {
    return 0;
  }
  printf("Error: UnSubscribing timer failed.\n");
  return 1;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //readback command
  uint8_t rbcmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  //write the readback command to the timer control register
  sys_outb(TIMER_CTRL, rbcmd);
  if (sys_outb(TIMER_CTRL, rbcmd) != 0) {
    printf("Error writing to the timer control register.\n");
    return 1;
  }

  //read result from timer using auxiliary function to make a system call
  if (util_sys_inb(TIMER_0 + timer, st) != 0) {
    printf("Error read data from timer: %d\n", timer);
    return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;
  uint8_t ini = ((st & (BIT(4) | BIT(5))) >> 4); //isolate the 4th and 5th 
  uint8_t cnt = ((st & (BIT(1) | BIT(2) | BIT(3))) >> 1);

  //Based on smth assign correct values to conf
  switch(field) {
    case tsf_all: //configuration/status
      conf.byte = st;
      break;
    case tsf_initial: //timer initialization mode
      switch (ini) {
        case 1: //LSB
          conf.in_mode = LSB_only;
          break;
        case 2: //MSB
        conf.in_mode = MSB_only;
          break;
        case 3: //LSB followed by MSB
          conf.in_mode = MSB_after_LSB;
          break;
        default:
          conf.in_mode = INVAL_val;
          break;
      }
      break;
    case tsf_mode: //timer counting mode
      switch (cnt) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
          conf.count_mode = cnt;
          break;
        case 6:
          conf.count_mode = 2;
          break;
        case 7:
          conf.count_mode = 3;
          break;
        default:
          printf("Error trying to set tsf_mode with invalid value: %d\n", cnt);
          return 1;
      }
      break;
    case tsf_base: //timer counting base
      conf.bcd = st & 0x01; //get least significant bit of st (bit 0)
      break;
  }

  //call predefined function to print timer
  timer_print_config(timer, field, conf);

  return 0;
}
