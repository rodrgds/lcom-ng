#include <lcom/lcf.h>
//#include "timer.h"

#include <stdint.h>

#include "i8254.h"


int timer_hook_id = 0;
int timer_irq_counter=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
     uint8_t status;

     if (timer>2 || timer<0 || freq<19 || freq > TIMER_FREQ) return 1;
     if (timer_get_conf(timer, &status)) return 1;
     
     status = status & (0x0F);
     uint8_t controlWord = status | TIMER_LSB_MSB;
     switch(timer){
         case 0:
            controlWord = controlWord | TIMER_SEL0;
            break;
         case 1:
            controlWord = controlWord | TIMER_SEL1;
            break;
         case 2:
            controlWord = controlWord | TIMER_SEL2;
            break;
         default:
            return 1;         
     }
     

     uint32_t frequency = TIMER_FREQ/freq;
     uint8_t lsb, msb;

     if (util_get_LSB(frequency, &lsb)) return 1;
     if (util_get_MSB(frequency, &msb)) return 1;

     if (sys_outb(TIMER_CTRL, controlWord)) return 1;

     if (sys_outb(TIMER_0+timer,lsb)) return 1;
     if (sys_outb(TIMER_0+timer, msb)) return 1;

     return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = timer_hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
   timer_irq_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
   if (timer>2 || timer<0 || st==NULL){
      return 1;
   }

   uint8_t readBack = TIMER_RB_CMD | TIMER_RB_COUNT_|TIMER_RB_SEL(timer);
   if (sys_outb(TIMER_CTRL, readBack)) return 1;
   if (util_sys_inb(TIMER_0+timer,st)) return 1;
   return 0;
      
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
   if (timer>2 || timer<0){
      return 1;
   }

   union timer_status_field_val val;

   switch(field){
      case tsf_all:
         val.byte = st;
         break;
      case tsf_initial:
         val.in_mode = (st & 0x30)>>4;
         break;
      case tsf_mode:
         val.count_mode = (st & 0x0E)>>1;
         break;
      case tsf_base:
         val.bcd = (st & 0x01);
         break;
      default:
         return 1;            
   }

   if (timer_print_config(timer,field,val)) return 1;

   return 0;
}
