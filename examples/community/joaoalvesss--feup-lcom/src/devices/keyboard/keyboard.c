#include "keyboard.h"

uint8_t scancode = 0;
int hook_id_kbc = 1;

int (keyboard_subscribe_interrupts)() {
  return sys_irqsetpolicy(IRQ_KEYBOARD, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_kbc);
}

int (keyboard_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&hook_id_kbc);
}

void (keyboard_interrupt_handler)(){
    keyboard_mouse_read_output(0, KBC_OUT_CMD, &scancode);
}

int (keyboard_mouse_read_status)(uint8_t* status){
  return util_sys_inb(KBC_STATUS_REG, status);
}

int (keyboard_mouse_read_output)(uint8_t mouse, uint8_t port, uint8_t *output){
    uint8_t status;
    uint8_t tries = MAX_ATTEMPS;

    while(tries > 0){
        if(keyboard_mouse_read_status(&status) != 0) return 1;

        if((status & FULL_OUT_BUFFER) != 0){
            if(util_sys_inb(port, output) != 0) return 1;
            if((status & PARITY_ERROR) != 0) return 1;
            if((status & TIMEOUT_ERROR) != 0) return 1;
            if((!mouse == 1) && (status & MOUSE_CMP)) return 1;
            if((mouse == 1) && !(status & MOUSE_CMP)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(WAITING_TIME));
        tries--;
    }
    return 1;
}

int (keyboard_mouse_write_command)(uint8_t port, uint8_t command){
    uint8_t status;
    uint8_t tries = MAX_ATTEMPS;

    if(keyboard_mouse_read_status(&status) != 0) return 1;
            
    while(tries > 0){
        if((status & FULL_IN_BUFFER) != 0) return 1;
        else if ((status & FULL_IN_BUFFER) == 0){
            if(sys_outb(port, command) != 0){
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(WAITING_TIME));
        tries--;
    }
    return 1;
}

