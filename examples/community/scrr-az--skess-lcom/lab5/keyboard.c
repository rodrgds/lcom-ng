#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"

int keyboard_hook_id = 1;
bool ESC_key = false;

int (keyboard_subscribe_int)(uint8_t *bit_no){
    if (bit_no==NULL) return 1; //check if the pointer is valid

    *bit_no = BIT(keyboard_hook_id); //changes bit_no to reflect the hook id
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_int)(){
    return sys_irqrmpolicy(&keyboard_hook_id);
}

int (keyboard_enable_interrupts)(){
    uint8_t command;

    if(write_KBC_command(KBC_CMD_REG, KBC_READ_CMD)) return 1; //warn kbc you are going to read command
    if(read_KBC_output(&command, false)) return 1; //read the command

    command |= ENABLE_INT; //activates the interruption bit, keeps the other 

    if(write_KBC_command(KBC_CMD_REG, KBC_WRITE_CMD)) return 1; //warn kbc you are going to write command
    if(write_KBC_command(KBC_WRITE_CMD, command)) return 1; //write command

    return 0;
}

void (kbc_ih)(){
    uint8_t output;
    int scan_size = 1;


    if(read_KBC_output(&output, false)) return; //error reading the output

    if(output==ESC){
        ESC_key = true;
    } //make esc_key true and return if the scancode is the break code from ESC


    if(output==0xE0) scan_size=2;
    kbd_print_scancode(!(output & CHK_MAKE), scan_size, &output);
}

int (await_ESC_break)(){
    int ipc_stat;
    uint8_t mask;
    message msg;

    if(keyboard_subscribe_int(&mask)) return 1; //error subscribing to keyboard interruptions

    while(!ESC_key){
        if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

        if(is_ipc_notify(ipc_stat)){ //if it was a notification
            switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

                case HARDWARE: //it came from a hardware device (like a keyboard)
                    if(msg.m_notify.interrupts & mask){ //check if the interrupt was from the keyboard
                        kbc_ih(); //call the interruption handler
                    }
                    break;

                default:
                    break; //if the notification is other than a keyboard interrupt, do nothing
            }
        }
    }

    //finished reading keyboard, unsubscribe KBC interrupt
    if(keyboard_unsubscribe_int()) return 1; //error unsubscribing

    return 0;
}
