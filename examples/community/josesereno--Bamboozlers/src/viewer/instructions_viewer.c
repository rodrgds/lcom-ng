#include "instructions_viewer.h"


int (draw_instructions_menu)(){    
    
    if (draw_sprite(instructions_text_1)) return 1;
    if (draw_sprite(instructions_text_2)) return 1;
    if (draw_sprite(instructions_text_3)) return 1;
    if (draw_sprite(instructions_text_4)) return 1;
    if (draw_sprite(instructions_text_5)) return 1;
    if (draw_sprite(instructions_text_6)) return 1;
    return 0;
}
