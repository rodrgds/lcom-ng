#include <lcom/lcf.h>
#include <stdint.h>

#include "../macros.h"

int timer_hook_id = 0;
int global_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    if (freq > TIMER_FREQ || freq < 19 || timer < 0 || timer > 2)
        return 1; 

    uint8_t cw;

    
    if (timer_get_conf(timer, &cw) != 0)
        return 1; 

    cw = (cw & 0xF) | TIMER_LSB_MSB;

    uint32_t squareWaveFreq = TIMER_FREQ / freq;
    
    uint8_t lsb, msb;
    if (util_get_LSB(squareWaveFreq, &lsb) != 0) return 1;
    if (util_get_MSB(squareWaveFreq, &msb) != 0) return 1;

    uint8_t port;
    switch (timer) {
        case 0:
            cw = cw | TIMER_SEL0;
            port = TIMER_0;
            break;
        case 1:
            cw = cw | TIMER_SEL1;
            port = TIMER_1;
            break;
        case 2:
            cw = cw | TIMER_SEL2;
            port = TIMER_2;
            break;
        default:
            return 1;
    }

    if (sys_outb(TIMER_CTRL, cw) != 0)
        return 1;

    if (sys_outb(port, lsb) != 0)
        return 1;
    if (sys_outb(port, msb) != 0)
        return 1;

    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("bit_no is NULL\n");
    return 1;
  }

  *bit_no = BIT(timer_hook_id);

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0){
    printf("error setting the policy\n");
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook_id) != 0){
    printf("error removing the policy\n");
    return 1;
  }
  
  return 0;
}

void (timer_int_handler)() {
  global_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t cw = TIMER_RB_CMD|TIMER_RB_SEL(timer)|TIMER_RB_COUNT_; 

  sys_outb(TIMER_CTRL,cw);                  

  if (util_sys_inb(timer + 0x40, st) != 0) return 1;          

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;

  if (field == tsf_all) conf.byte = st;   

  if (field == tsf_initial) conf.in_mode = (st & TIMER_LSB_MSB) >> 4;

  if (field == tsf_mode) conf.count_mode = (st & (BIT(3)|BIT(2)|BIT(1))) >> 1;

  if (field == tsf_base) conf.bcd = st & TIMER_BCD;
  
  if (timer_print_config(timer, field, conf) != 0) return 1;

  return 0;
}
