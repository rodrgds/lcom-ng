#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "macros.h"

int timerHookID = TIMER0_IRQ;

uint32_t interruptCounter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (freq < TIMER_FREQ_MIN || freq > TIMER_FREQ)
  {
    return 1;
  }
  uint32_t div=TIMER_FREQ/freq;
  uint8_t st; //status of timer
  if(timer_get_conf(timer, &st) == 1)  //uses read-back command
  {
    return 1;
  }

  uint8_t timerMask = st & 0x0F;
  uint8_t msb, lsb;
  util_get_LSB(div, &lsb);
  util_get_MSB(div, &msb);

   timerMask |= TIMER_RB_SEL(timer)| TIMER_LSB_MSB;
   if(sys_outb(TIMER_CTRL, timerMask) != OK) return 1;
   if(sys_outb(TIMER_0+timer, lsb) != OK) return 1;
   if(sys_outb(TIMER_0+timer, msb) != OK) return 1;

  return 0;
}



int (timer_subscribe_int)(uint8_t *bit_no) {
  timerHookID = (int) *bit_no;

  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timerHookID) != OK) {
    return 1;
  }

*bit_no = (uint8_t) BIT(*bit_no);
return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timerHookID)!= OK){
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  interruptCounter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if(timer < 0 || timer > 2) {
      return 1;
    }

  uint8_t rbCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL,rbCommand) != 0) {
    return 1;
  }


  if (util_sys_inb(TIMER_0+timer,st) != 0) {
    return 1;
  }

    return 0;
  }

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val config;

if(timer < 0 || timer > 2) {
    return 1;
  }

  switch (field) {
    case tsf_all:
    config.byte = st;
    break;
  case tsf_initial:
    {
      uint8_t initMask = 0x30;
      uint8_t initCounter = (initMask & st) >> 4;
      enum timer_init initT;
      switch (initCounter) {

        case 0:
          return 1;

        case 1:
          initT = LSB_only;
          break;

        case 2:
          initT = MSB_only;
          break;

        case 3:
          initT = MSB_after_LSB;
          break;
        default:
          initT = INVAL_val;
      }
      config.in_mode = initT;
    break;
  }

  case tsf_mode:
  {
    uint8_t modeMask = 0x0E;
    uint8_t modeCounter = (modeMask & st) >> 1;
    config.count_mode= modeCounter;
    break;
  }
  case tsf_base:
  {

    uint8_t baseMask = 0x01;
    uint8_t baseMode = baseMask & st;
    config.bcd= baseMode;
    break;
  }
  default:
    return 1;
}

  timer_print_config(timer, field, config);

  return 0;
}
