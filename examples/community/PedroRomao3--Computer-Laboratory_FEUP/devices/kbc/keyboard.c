#include "keyboard.h"

int keybord_hook_id = 1;
uint8_t scancode = 0;


int (keyboard_subscribe_int)(uint8_t *bit_no){
    if(bit_no == NULL) return 1;

    *bit_no = BIT(keybord_hook_id);

    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_EXCLUSIVE | IRQ_REENABLE, &keybord_hook_id);
}

int (keyboard_unsubscribe_int)(){
  return sys_irqrmpolicy(&keybord_hook_id);
}

void (kbc_ih)(){
  if(get_KBC_out(KBC_OUT_CMD, &scancode, 0)) printf("Error: read scancode");
}

char getLetter(){
  switch (scancode) {
        case Ql: return 'q';
        case Wl: return 'w';
        case El: return 'e';
        case Rl: return 'r';
        case Tl: return 't';
        case Yl: return 'y';
        case Ul: return 'u';
        case Il: return 'i';
        case Ol: return 'o';
        case Pl: return 'p';
        case Al: return 'a';
        case Sl: return 's';
        case Dl: return 'd';
        case Fl: return 'f';
        case Gl: return 'g';
        case Hl: return 'h';
        case Jl: return 'j';
        case Kl: return 'k';
        case Ll: return 'l';
        case Zl: return 'z';
        case Xl: return 'x';
        case Cl: return 'c';
        case Vl: return 'v';
        case Bl: return 'b';
        case Nl: return 'n';
        case Ml: return 'm';
        case BACK: return '\b'; 
        case n1: return '1';
        case n2: return '2';
        case n3: return '3';
        case n4: return '4';
        case n5: return '5';
        case n6: return '6';
        case n7: return '7';
        case n8: return '8';
        case n9: return '9';
        case n0: return '0'; 
        default: return '\0'; 
    }
}

bool isl(char l){
  return l >= 'a' && l <= 'z';
}

bool isd(char l){
  return l >= '0' && l <= '9';
}
