#include "keyboard_state.h"
#include "i8042.h"
#include "gameSprites.c"
void(update_state)(keyboard_state *state, uint8_t scancode, bool is_left ) { 
    uint8_t codes [12] = {W_KEY, W_KEY_BREAK, S_KEY, S_KEY_BREAK, D_KEY, D_KEY_BREAK, 
    UP_KEY, UP_KEY_BREAK, DOWN_KEY, DOWN_KEY_BREAK, LEFT_KEY,  LEFT_KEY_BREAK};
    int i = 0;

    if (!is_left) {
        i = 6;
    }
           
    uint8_t up = codes[i];
    uint8_t up_break = codes[1+i];
    uint8_t down = codes[2+i];
    uint8_t down_break = codes[3+i];
    uint8_t right = codes[4+i];
    uint8_t right_break = codes[5+i];


   
    if (scancode == ESC_KEY) {
        *state = escape;
    }

    else if(scancode == up) {
        if(*state == up_pressed || *state == up_pressing || *state == down_pressed || 
          *state == right_pressed || *state == up_right_pressed || *state == down_right_pressed || *state == null) {
            *state = up_pressing;
        } 
        else if(*state == right_pressing || *state == up_pressed_right_pressing) {
            *state = up_right_pressing;
        }
    }

    else if (scancode == down) {
        if(*state == up_pressed || *state == down_pressing || *state == down_pressed || 
          *state == right_pressed || *state == up_right_pressed || *state == down_right_pressed||*state==null) {
              *state = down_pressing;
        }
        else if(*state == right_pressing || *state == down_pressed_right_pressing) {
              *state = down_right_pressing; 
        }
    }

    else if (scancode == right) {
        if (*state == up_pressing || *state == up_pressing_right_pressed) {
            *state = up_right_pressing;
        }
        else if (*state == down_pressing || *state == down_pressing_right_pressed) {
            *state = down_right_pressing;
        }
        else if (*state == right_pressing || *state == right_pressed || *state == up_pressed ||
           *state == down_pressed || *state == up_right_pressed || *state == down_right_pressed) {
              *state = right_pressing;
        }
    }

    else if (scancode == up_break) {
        if (*state == up_pressing) {
            *state = up_pressed;
        }
        else if (*state == up_right_pressing) {
          *state = up_pressed_right_pressing;
        }
        else if (*state == up_pressing_right_pressed) {
            *state = up_right_pressed;
        }
    }

    else if (scancode == down_break) {
        if (*state == down_pressing) {
            *state = down_pressed;
        }
         else if (*state == down_right_pressing) {
            *state = down_pressed_right_pressing;
        }
        else if (*state == down_pressing_right_pressed) {
            *state = down_right_pressed;
        }
    }

    else if (scancode == right_break) {
        if (*state == right_pressing) {
             *state = right_pressed;
        }

        else if(*state == up_right_pressing) {
            *state = up_pressing_right_pressed;
        }

        else if(*state == down_right_pressing) {
            *state = down_pressing_right_pressed;
        }
        
        else if(*state == up_pressed_right_pressing) {
            *state = up_right_pressed;
        }
        else if(*state == down_pressed_right_pressing) {
            *state = down_right_pressed;
        }
    }
    
}

void(print_state)(keyboard_state * state) {
    if (*state == up_pressing || *state == up_pressed)
    {
      printf("\ngoing up\n");
    }
    else if(*state == down_pressing || *state == down_pressed) {
      printf("\ngoing down\n");
    }
    else if(*state == right_pressing || *state == right_pressed) {
      printf("\nright\n");
    }
    else if(*state == up_right_pressing) {
      printf("\nentrar declive positivo\n");
    }
    else if (*state == down_right_pressing) {
      printf("\nentrar declive negativo\n");
    }
    else if( *state == up_right_pressed ) {
      printf("\nsair declive positivo\n");
    }
    else if( *state == down_right_pressed ) {
      printf("\nsair declive negativo\n");
    }
    else if(*state == up_pressed_right_pressing || *state == down_pressed_right_pressing) {
      printf("\nlarga o RIGHT\n");
    }
    else if(*state == up_pressing_right_pressed ) {
      printf("\nlarga o UP\n");
    }
    
    else if(*state == down_pressing_right_pressed ) {
      printf("\nlarga o DOWN\n");
    }
}

void keyboard_move_bar(keyboard_state state, bool is_left){

  if (is_left) {
      if (state == up_pressing)
      {
          go_up_lbar();
      }
      else if (state == down_pressing) {
          go_down_lbar();
      }
  }
  else {
      if (state == up_pressing)
      {
          go_up_rbar();
      }
      else if (state == down_pressing) {
          go_down_rbar();
      }
      
  }
    
}

