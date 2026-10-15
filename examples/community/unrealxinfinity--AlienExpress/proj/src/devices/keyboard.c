#include "keyboard.h"


int hook_id = 1;
uint8_t (keyboard_subscribe)(){
    uint8_t bit_no = BIT(hook_id);
    if(sys_irqsetpolicy(KBD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) return 0xFF;
    return bit_no;

}
int (keyboard_unsubscribe)(){
    return sys_irqrmpolicy(&hook_id);
}
int (kbc_read_output)(int port, uint32_t *command){
    uint32_t status;
    uint32_t scancode;
    sys_inb(KBC_STATUS, &status);
    if ((status & OUT_BUFFER_BIT) != 0) {

        sys_inb(port, &scancode);

        if((status & PARITY_ERROR) != 0){
            return ERROR;
        }

        if((status & TIME_OUT_ERROR) != 0){
            return ERROR;
        }

        if(status & BIT(5)) return 0xFFFF;
        *command = scancode;
        return scancode;

    }

    return ERROR;
}
int (kbc_write_output)(int port, unsigned long command){
    uint32_t status;

    if(sys_inb(KBC_STATUS, &status))return ERROR;

        if ((status & IN_BUFFER_BIT) == 0){
        if(sys_outb(port, command) != 0){
            return ERROR;
        }

        return 0;
    }

    
    return ERROR;
}
int (keyboard_restore)(){
    uint32_t command;

    if (kbc_write_output(KBC_STATUS, IN_BUFFER) != 0) return 1;          
    if (kbc_read_output(OUT_BUFFER, &command) != 0) return 1; 

    command |= OUT_BUFFER_BIT;  

    if (kbc_write_output(KBC_STATUS, OUT_BUFFER) != 0) return 1;    
    if (kbc_write_output(OUT_BUFFER, command) != 0) return 1;

    return 0;
}

