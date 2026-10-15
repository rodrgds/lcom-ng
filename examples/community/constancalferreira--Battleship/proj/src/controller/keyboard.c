#include "keyboard.h"


uint8_t scancode = 0;
int kbd_hook_id = 1;


//retrieves the scancode
void (kbc_ih)() {
    if(read_KBC_output(OUT_BUF, &scancode) != 0) {
        printf("Error reading scancode\n"); 
        scancode = 0; 
    }
}



static uint8_t bytes[2];  // buffer for multi-byte scancodes
static bool waiting_second_byte = false;  

/*why is this needed?
When we press a key like an arrow key (which sends a two-byte scancode), the first byte is always 0xE0. 
This tells us that the next byte will be the second part of the scancode, so we wait for the next call*/
void (process_scancode)(uint8_t scancode) {
    
    // it means it has 2 bytes, we signal that we're expecting a second one
    if (scancode == 0xE0) {
        waiting_second_byte = true; 
        return; // we return because we need a 2nd call to process the 2nd byte of the scancode
    }

    bool make = !(scancode & IS_BREAK);

    // if the previous call to process_scancode set the flag:
    if (waiting_second_byte) {
        bytes[1] = scancode;
        kbd_print_scancode(make, 2, bytes);
        waiting_second_byte = false;
        return;
    }

    // else, we are dealing with a single byte scancode
    bytes[0] = scancode;
    kbd_print_scancode(make, 1, bytes);
}





int(kbd_subscribe_int)(uint8_t *bit_no) {  //is uint8_t correct?
    if (bit_no == NULL) return 1;
    *bit_no = BIT(kbd_hook_id);
    if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_hook_id) != 0) return 1;
    return 0;
}


int (kbd_unsubscribe_int)() {
    if (sys_irqrmpolicy(&kbd_hook_id) != 0) return 1;
    return 0;
}




/*  POOLING

-> guião secção 5
Before returning, kbd_test_poll() must enable interrupts, by writing an appropriate KBC command byte. 
If it does not do it, the KBC will not generate interrupts, and thus the keyboard will get stuck. 
This is because lcf_start() disables keyboard interrupts, before calling kbd_test_poll(), 
to prevent Minix's keyboard IH from stealing the scancodes from your program.
*/
int (kbd_restore)() {

    uint8_t commandByte;

    //lemos a configuração atual
    if (write_KBC_cmd(KBC_CMD_REG, KBC_READ_CMD) != 0) return 1; 
    if (read_KBC_output(OUT_BUF, &commandByte) != 0) return 1; 

    //ativamos o bit 0 para ativar as interrupts do keyboard
    commandByte = commandByte | BIT(0);

    //escrevemos a nova configuração
    if (write_KBC_cmd(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return 1; 
    if (write_KBC_cmd(KBC_ARGS, commandByte) != 0) return 1;

    return 0;
}
