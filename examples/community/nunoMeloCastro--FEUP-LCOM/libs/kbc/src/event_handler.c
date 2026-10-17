#include <lcom/lcf.h>

#include <lcom/lab4.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "libs/kbc/include/kbc.h"
#include "libs/kbc/include/i8042.h"
#include "libs/timer/include/i8254.h"
#include "libs/utils/include/utils.h"  
#include "libs/timer/include/timer.h" 
#include "libs/kbc/include/mouse.h" 
#include "libs/kbc/include/event_handler.h"
#include "libs/vbe/include/vbe_macros.h"
#include "libs/vbe/include/vbe_functions.h"
#include "xpm/numbers.h"
#include "xpm/highlighted_numbers.h"
#include "game/include/grid.h"
#include "xpm/menu.h"
#include "xpm/difficulties.h"
#include "xpm/game_elements.h"
#include "stdlib.h"

int get_event(struct packet *pp, Event *event){

    static uint8_t prev = 0;

    if (pp == NULL || event == NULL)
        return 1;

    uint8_t status = pp -> bytes[0];

    //if (pp -> bytes[1] != 0 || pp -> bytes[2] != 0)
    if (pp -> delta_x != 0 || pp -> delta_y != 0)

        event -> type = MOVE;

    else if  (status == (pp -> bytes[0] & BIT1_IDENTFIER)){

        if (prev == LEFT_BUTTON)
            event -> type = LB_RELEASE;

        else if (prev == RIGHT_BUTTON)
                event -> type = RB_RELEASE;
        
            else
                event -> type = OTHER;
        
        prev = 0;
    }

    else if (status == (pp -> bytes[0] & (LEFT_BUTTON | BIT1_IDENTFIER))){
        
        event -> type = LB_PRESS;
        prev = LEFT_BUTTON;

    }

    else if (status == (pp -> bytes[0] & (RIGHT_BUTTON | BIT1_IDENTFIER))){
        
        event -> type = RB_PRESS;
        prev = RIGHT_BUTTON;

    }

    else
        event -> type = OTHER;
    
    event -> x_delta = pp -> delta_x;
    event -> y_delta = pp -> delta_y;
    
    return 0;
}
