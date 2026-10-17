#include <lcom/lcf.h>
#include "keyboard.h"

#include <stdint.h>

int (kbd_subscribe_int)(uint8_t *bit_no){
  kbd_hookid = 1;
  *bit_no = kbd_hookid;

  if (sys_irqsetpolicy(KEYBOARD_IRQ,  IRQ_REENABLE|IRQ_EXCLUSIVE , &kbd_hookid)) return -1;

  return 0;
}

int (kbd_unsubscribe_int)(){

  return sys_irqrmpolicy(&kbd_hookid);

}

void (kbc_ih)(){
    uint8_t st;
    //kbc_read_keycode();
    if (util_sys_inb(STAT_REG, &st)) return;
    if (st & TO_ERR) return;
    if (st & PAR_ERR) return;

    if (st & OBF){
        kbc_read_keycode();
    }
}

int (kbc_read_keycode)(){
    uint8_t outp;
    if (util_sys_inb(KBC_OUT_BUF, &outp)) return 1;
    if (outp == FIRST_BYTE_SCAN){
        scancode[0] = FIRST_BYTE_SCAN;
        scancode[1] = INVALID;
        secondbyte = true;
        return 0;
    }
    else if (secondbyte) {
        scancode[1] = outp;
    }
    else {
        scancode[0] = outp;
        secondbyte = false;
    }
    if (outp & BREAK_CODE) make = false;
    else make = true;

    return 0;
}

int (kbc_interrupts_enable)(){
    uint8_t cmd_byte;
    if (write_kbc_reg(READ_COM_BYTE)) return 1;
    if (util_sys_inb(KBC_IN_BUF, &cmd_byte)) return 1;

    cmd_byte |= BIT(0); 

    if (write_kbc_reg(WRITE_COM_BYTE)) return 1;
    if (sys_outb(KBC_OUT_BUF, cmd_byte)) return 1;
    return 0;
}

int (write_kbc_reg)(uint8_t command){
    int i = 0;
    uint8_t st;
    while (i < 5){
        if (util_sys_inb(STAT_REG, &st)) return 1;
        if (!(st & TO_ERR) && !(st & PAR_ERR) && !(st & IBF)) return sys_outb(KBC_CMD_REG, command);

        i++;
        tickdelay(micros_to_ticks(POLL_DELAY));
    }
    return 0;
}
