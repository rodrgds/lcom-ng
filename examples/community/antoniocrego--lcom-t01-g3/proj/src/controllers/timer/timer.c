#include <lcom/lcf.h>

#include <stdint.h>

#include "timer.h"

int timerCounter = 0;
int timer_hook_id = 0;

void (timer_ih)(){
    timerCounter++;
}

int (timer_subscribe_int)(uint8_t *bit_no){
    if (bit_no==NULL) return 1;
    *bit_no = BIT(timer_hook_id);
    return sys_irqsetpolicy(0,IRQ_REENABLE,&timer_hook_id);
}

int (timer_unsubscribe_int)(){
    return sys_irqrmpolicy(&timer_hook_id);
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq){
    uint8_t status; // placeholder for timer config/status
    if (sys_outb(0x43,BIT(7)|BIT(6)|BIT(5)|BIT(1+timer))!=0) return 1;  // readback CMD sent to the timer control port to get config
    if (util_sys_inb(0x40+timer, &status)!=0) return 1; // read config from timer port and store on status variable
    status = status & 0x0F; // clears the first 4 bits of the status to set timer 0 and to allow replacing of init mode
    status = status | BIT(5) | BIT(4);  // activates MSB after LSB to input new frequency in two bytes
    if (sys_outb(0x43, status)!=0) return 1;  // send new config to control register, activating new settings on timer 0
    uint16_t divisionFrequency = 1193182/freq;  // get the new frequency
    uint8_t lsb,msb;
    if (util_get_LSB(divisionFrequency,&lsb)!=0) return 1;
    if (util_get_MSB(divisionFrequency,&msb)!=0) return 1;
    if (sys_outb(0x40,lsb)!=0) return 1;  // LSB
    if (sys_outb(0x40,msb)!=0) return 1;  // then MSB
    return 0;
}
