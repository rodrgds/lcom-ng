#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>
#include "keyboard.h"

int keyboard_hook_id = 1;
char makecode_to_char[128] = {
    [0x1E] = 'A',
    [0x30] = 'B',
    [0x2E] = 'C',
    [0x20] = 'D',
    [0x12] = 'E',
    [0x21] = 'F',
    [0x22] = 'G',
    [0x23] = 'H',
    [0x17] = 'I',
    [0x24] = 'J',
    [0x25] = 'K',
    [0x26] = 'L',
    [0x32] = 'M',
    [0x31] = 'N',
    [0x18] = 'O',
    [0x19] = 'P',
    [0x10] = 'Q',
    [0x13] = 'R',
    [0x1F] = 'S',
    [0x14] = 'T',
    [0x16] = 'U',
    [0x2F] = 'V',
    [0x11] = 'W',
    [0x2D] = 'X',
    [0x15] = 'Y',
    [0x2C] = 'Z',
    [0x39] = ' '
};

int keyboard_subscribe_int(uint8_t *bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_int)(){
    return sys_irqrmpolicy(&keyboard_hook_id);
}

/* Handler de interrupção do teclado */
void kbc_ih1(char string[], int *size_str, bool *ESC_key, bool *submit){
    uint8_t output;
    if(read_KBC_output(&output, false)) return;
    
    if((*size_str) != 0 && output == 0x0E){ // Backspace: remove o último caractere
        (*size_str)--;
        string[*size_str] = '\0';
        return;
    }
    
    if((*size_str) != 0 && output == 0x1C){ 
        *submit = true;
        return;
    }
    
    if (output == 0x81) { 
        *ESC_key = true;
        return;
    }
    
    if(*size_str == 3*MAX_STR_LEN){ 
        return;
    }

    char c = '\0';
    if (output < 128) {
        c = makecode_to_char[output];
    }

    if (c != '\0') {
        string[*size_str] = c;
        (*size_str)++;
    }
}
