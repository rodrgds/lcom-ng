/**
 * @file timer.c
 * @brief Timer driver implementation
 */

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id=0;
int count=0;

/**
 * @brief Sets the frequency of a timer
 * @param timer Timer to set the frequency of
 * @param freq Desired frequency
 * @return 0 upon success, 1 otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(freq> TIMER_FREQ|| freq <19) return 1;

  uint8_t controlw;
  if(timer_get_conf(timer, &controlw) != 0)return 1;
  controlw=(controlw & 0x0F) | TIMER_LSB_MSB;

  uint32_t newfreq= TIMER_FREQ/freq;
  uint8_t LSB,MSB;
  util_get_LSB(newfreq, &LSB );
  util_get_MSB(newfreq, &MSB );

  uint8_t sel_time;
  switch(timer){
    case 0:
      controlw= controlw| TIMER_SEL0;
      sel_time=TIMER_0;
      break;
    case 1:
      controlw= controlw| TIMER_SEL1;
      sel_time=TIMER_1;
      break;
    case 2:
      controlw=controlw| TIMER_SEL2;
      sel_time=TIMER_2;
      break;

    default:
      return 1;
    
  }

  if(sys_outb(TIMER_CTRL, controlw)!= 0)return 1;

  if(sys_outb(sel_time,LSB)!=0)return 1;
  if(sys_outb(sel_time,MSB)!=0)return 1;
  
  return 0;

}

/**
 * @brief Subscribes and enables timer interrupts
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 upon success, 1 otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no ==NULL)return 1;
  *bit_no=BIT(hook_id);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!=0) return 1;
  return 0;
}

/**
 * @brief Unsubscribes timer interrupts
 * @return 0 upon success, 1 otherwise
 */
int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id)!=0)return 1;
  return 0;
}

/**
 * @brief Timer interrupt handler
 */
void (timer_int_handler)() {
  count++;
}

/**
 * @brief Gets the configuration of a timer
 * @param timer Timer to get the configuration of
 * @param st Address of memory to be initialized with the timer's configuration
 * @return 0 upon success, 1 otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(timer<0||timer>2)return 1;
  uint8_t rb=( TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer));
  if( sys_outb(TIMER_CTRL,rb)!=0) return 1;
  if(util_sys_inb(TIMER_0+timer,st)) return 1;
  return 0;

}

/**
 * @brief Displays the configuration of a timer
 * @param timer Timer to display the configuration of
 * @param st Timer's configuration
 * @param field Field of the configuration to display
 * @return 0 upon success, 1 otherwise
 */
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val conf;

  switch(field){
    case tsf_all:
      conf.byte = st;
      break;

    case tsf_initial:
      st=(st>>4)& 0x03;

      if(st==1)conf.in_mode=LSB_only;
      else if(st==2)conf.in_mode=MSB_only;
      else if(st==3)conf.in_mode=MSB_after_LSB;
      else conf.in_mode=INVAL_val;

      break;

    case tsf_mode:

      st=st & 0x0F;
      st=st>>1;

      if(st==6){
        conf.count_mode=2;
      }
      else if(st==7){
        conf.count_mode=3;
      }
      else conf.count_mode=st;
     
      break;

    case tsf_base:
      st = st & 0x01;

      if(st==0){
        conf.bcd=false;
      }
      else if(st==1){
        conf.bcd=true;
      }

      break;

      default:
        return 1;
  }


  if(timer_print_config(timer,field,conf)!=0)return 1;


  return 0;
}
