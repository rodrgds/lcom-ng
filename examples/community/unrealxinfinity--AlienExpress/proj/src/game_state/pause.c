#include "pause.h"

action_t pause_selection = AFK;


void set_selection_pause(action_t action){
    pause_selection = action;
}

action_t keyboard_ih_pause(uint32_t scancode){
    if(scancode == MAKE_ENTER){
        return pause_selection;
    }
    else if(scancode == MAKE_ESC){
        return START;
    }
    else if(scancode == MAKE_S){
        if(pause_selection == AFK) pause_selection = START;
        else if(pause_selection == START) pause_selection = EXIT;
    }
    else if(scancode == MAKE_W){
        if(pause_selection == AFK)pause_selection = START;
        else if(pause_selection == EXIT) pause_selection = START;
    }
    return AFK;
}
action_t mouse_ih_pause(){
    if(mouse_position(493, 300, 583, 315)){
        pause_selection = START;
    }
    else if(mouse_position(506, 360, 566, 375)){
        pause_selection = EXIT;
    }
    else{
        if(mouse_hover){
            mouse_hover = false;
            pause_selection = AFK;
        }
    }
    if(mouse_packet.lb && mouse_hover) {
        mouse_hover = false;
        return pause_selection;
    }
    return AFK;
}

void draw_pause(int type){
    if((counter_timer % 30)== 0)counter_timer--;
    draw_pause_words();

}


void draw_pause_words(){
    int resume_type = 0;
    int menu_type = 0;
     switch(pause_selection){
        case START:


        
            resume_type = 1;
            menu_type = 0;
            break;
        case EXIT:
            resume_type = 0;
            menu_type = 1;
            break;
        case AFK:
            break;
        default:
            break;
    }
    set_char(&r, 493, 300, resume_type);
    set_char(&e, 508, 300, resume_type);
    set_char(&s, 523, 300, resume_type);
    set_char(&u, 538, 300, resume_type);
    set_char(&m, 553, 300, resume_type);
    set_char(&e, 568, 300, resume_type);


    set_char(&m, 506, 360, menu_type);
    set_char(&e, 521, 360, menu_type);
    set_char(&n, 536, 360, menu_type);
    set_char(&u, 551, 360, menu_type);
    
}

