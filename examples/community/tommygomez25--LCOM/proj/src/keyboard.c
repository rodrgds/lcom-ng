#include <lcom/lcf.h>
#include "i8042.h"
#include <stdint.h>

uint8_t last = RIGHT;
int hook_id_kb = KBD_IRQ;

int (kbd_subscribe_int)(uint8_t *bit_no){
    *bit_no = hook_id_kb;
    sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_id_kb);
    return 0;
}

void (kbc_ih)() {
    uint8_t status = 0;
    uint8_t scancode = 0;
    bool make = true;
    if (util_sys_inb(STAT_REG,&status)){
      return;
    }
    if (status & ST_OUT_BUF){
        if (status &( ST_PAR_ERR | ST_TO_ERR)){
          return;
        }
        
        if(util_sys_inb(OUT_BUF,&scancode)){
          return;
        }
        last = scancode;
        uint8_t bytes[2];
        bytes[0] = scancode;
        if(scancode == TWOBYTES){
          if(util_sys_inb(OUT_BUF,&scancode)){
            return;
          }
          if(scancode & BIT(7)){
            make = false;
          }
          bytes[1]=scancode;
          kbd_print_scancode(make,2,bytes);
        }
        else{
          if(scancode & BIT(7)){
            make = false;
          }
          kbd_print_scancode(make,1,bytes);
        }
    }
}

int (kbd_unsubscribe_int)(){
  sys_irqrmpolicy(&hook_id_kb);
  return 0;
}
