#include "elements.h"

int (move_rectangle)(Rectangle * r, Direction d){
    switch(d) {
        case UP: {
            r->pos.y--;
            return 0;
        }

        case DOWN: {
            r->pos.y++;
            return 0;
        }
        
        case RIGHT: {
            r->pos.x++;
            return 0;
        }
        
        case LEFT: {
            r->pos.x--;
            return 0;
        }
        
        default: return 1;
    }

    return 1;
}

bool (hit)(Rectangle * hitbox1, Rectangle * hitbox2){

    uint32_t x1 = hitbox1->pos.x, x2 = hitbox2->pos.x;
    uint32_t w1 = hitbox1->width, w2 = hitbox2->width;
    uint32_t y1 = hitbox1->pos.y, y2 = hitbox2->pos.y;
    uint32_t h1 = hitbox1->height, h2 = hitbox2->height;

    return ((!(x1 > x2 + w2 || x1 + w1 < x2)) && (!(y1 > y2 + h2 || y1 + h1 < y2)));
}

bool (hit_button)(Rectangle * hitbox1, Rectangle * hitbox2, Button_Type * button_type, bool click, Keyboard_Key key){
    switch (*button_type){
        case WHITE_MOLE: {
            if (click || key == SPACE_KEY) break;
            else return false;
        }

        case YELLOW_MOLE: {
            if (key == W_KEY) break;
            else return false;
        }

        case RED_MOLE: {
            if (key == A_KEY) break;
            else return false;
        }

        case GREEN_MOLE:{
            if (key == S_KEY) break;
            else return false;
        }

        case BLUE_MOLE: {
            if (key == D_KEY) break;
            else return false;
        }

        case BLACK_MOLE: {
            if (click || key) break;
            else return false;
        }

        default: return false;
    }

    return hit(hitbox1, hitbox2);
}

void (update_button_type)(Video_Mode vm, Button * button, Button_Type mt){
    button->button_type = mt;
    switch (mt) {
        case RED_MOLE: {
            button->hitbox.color = (vm == INDEXED) ? 3 : 0xff0000; // RED
            break;
        }
        case GREEN_MOLE: {
            button->hitbox.color = (vm == INDEXED) ? 4 : 0x00ff00; // GREEN
            break;
        }
        case BLUE_MOLE: {
            button->hitbox.color = (vm == INDEXED) ? 5 : 0x0000ff; // BLUE
            break;
        }
        case YELLOW_MOLE: {
            button->hitbox.color = (vm == INDEXED) ? 6 : 0xffff00; // YELLOW
            break;
        }
        case BLACK_MOLE: {
            button->hitbox.color = (vm == INDEXED) ? 0 : 0x000000; // BLACK
            break;
        }
        case WHITE_MOLE: {
            button->hitbox.color = (vm == INDEXED) ? 1 : 0xffffff; // WHITE
            break;
        }
    }
}

void (disable_button)(Button * button, bool hit){
    button->hitbox.active = false;
    button->counting = 0;
    
    if (++button->next_idx > 0){
        --button->next_idx;
    }
    if (hit){
        button->button_state = HIT;
        button->button_type = *(button->next_type_list + button->idx_type);
        
        if (button->idx_type == 9) button->idx_type = 0;
        else button->idx_type++;
    }
    else {
        button->button_state = DEAD;
    }
}

void (enable_button)(Button * button){
    button->hitbox.active = true;
    button->counting = button->counting_value;
    button->button_state = ALIVE;
}

bool (alert_button)(Button * button, Rectangle * hitbox, bool click, Keyboard_Key key){
    
    if (button->button_state == ALIVE ) {
        if (hit_button(&button->hitbox, hitbox, &button->button_type, click, key)){
            disable_button(button, true);
            return true;
        }
        
        if ((--button->counting == 0)){
            disable_button(button, false);
            return true;
        }
    }
    else {
        if ((++button->counting == *(button->time_to_appear + button->next_idx))){
            enable_button(button);
        }
    }
    return false;
}

void (create_time_to_appear)(Button * button){

   button->counting_value = (rand() % 60) + 500;

    for (int i = 0 ; i < 10 ; i++){
        *(button->time_to_appear + i) = (rand() % 60) + 100; //3600
    }
}

void (create_type_to_appear)(Button * button){

   button->button_type = (rand() % 6);

    for (int i = 0 ; i < 10 ; i++){
        *(button->next_type_list + i) = (rand() % 6);
    }
}
