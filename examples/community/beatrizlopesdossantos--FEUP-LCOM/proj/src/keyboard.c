#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"
#include "utils.h"


int kbd_hook_id;
uint8_t scancode = 0;


int (kbd_subscribe_int)(uint8_t *bit_no) {
    kbd_hook_id = KBD_IRQ;
    *bit_no = kbd_hook_id;  
    if (sys_irqsetpolicy(KBD_IRQ ,IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) == OK) {
        return 0;
    }
    else {
        printf("sys_irqsetpolicy() failed\n");
        return 1;    
    }

}

int (kbd_unsubscribe_int)() {
    if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
        printf("sys_irqrmpolicy() failed\n");
        return 1;
    }
    return 0;
}

void (kbc_ih)() {
    uint8_t st;
    if (util_sys_inb(KBC_ST_REG, &st) != OK) {
        printf("Error in sys_inb()\n");
        return;
    }     
    if (st & OBFULL) {   
      if (util_sys_inb(KBC_OUT_BUF, &scancode) != OK){
        printf("Error in sys_inb()\n");
        return;
      }
      if ((st & AUX) != OK) {
        printf("Bits in satatus register failed\n");
        return;
      }   
      if ((st & (PARITY_ERROR | TIMEOUT_ERROR)) != OK) {
        printf("Bits in satatus register failed\n");
        return;
      }  

    }
}
