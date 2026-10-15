#include "menu.h"

action_t menu_selection = AFK;


void set_selection_menu(action_t action){
    menu_selection = action;
}

action_t mouse_ih_menu(){
    if(mouse_position(500, 300, 575, 315)){
        menu_selection = START;
    }
    else if(mouse_position(506, 330, 566, 345)){
        menu_selection = EXIT;
    }
    else{
        if(mouse_hover){
            mouse_hover = false;
            menu_selection = AFK;
        }
    }
    if(mouse_packet.lb && mouse_hover) {
        mouse_hover = false;
        return menu_selection;
    }
    return AFK;

}

action_t keyboard_ih_menu(uint32_t scancode){
    if(scancode == MAKE_ENTER){
        return menu_selection;
    }
    else if(scancode == MAKE_S){
        if(menu_selection == AFK) menu_selection = START;
        else if(menu_selection == START) menu_selection = EXIT;
    }
    else if(scancode == MAKE_W){
        if(menu_selection == AFK)menu_selection = START;
        else if(menu_selection == EXIT) menu_selection = START;
    }
    return AFK;
}

void draw_menu(){
    draw_menu_title();
    draw_menu_words();
    if(mouse_hover)draw(mouse_animation.animation[1], mouse);
    else draw(mouse_animation.animation[0], mouse);
}
void draw_menu_words(){
    int start_type = 0;
    int quit_type = 0;
    switch(menu_selection){
        case START:
            start_type = 1;
            quit_type = 0;
            
            break;
        case EXIT:
            start_type = 0;
            quit_type = 1;
            break;
        case AFK:
            break;
        default:
            break;
    }
    set_char(&s, 500, 300, start_type);
    set_char(&t, 515, 300, start_type);
    set_char(&a, 530, 300, start_type);
    set_char(&r, 545, 300, start_type);
    set_char(&t, 560, 300, start_type);

    set_char(&q, 506, 330, quit_type);
    set_char(&u, 521, 330, quit_type);
    set_char(&i, 536, 330, quit_type);
    set_char(&t, 551, 330, quit_type);
}
void draw_menu_title(){

    set_char(&a, 445, 100, 1);
    set_char(&l, 460, 100, 1);
    set_char(&i, 475, 100, 1);
    set_char(&e, 490, 100, 1);
    set_char(&n, 505, 100, 1);

    set_char(&e, 535, 100, 1);
    set_char(&x, 550, 100, 1);
    set_char(&p, 565, 100, 1);
    set_char(&r, 580, 100, 1);
    set_char(&e, 595, 100, 1);
    set_char(&s, 610, 100, 1);
    set_char(&s, 625, 100, 1);

}

