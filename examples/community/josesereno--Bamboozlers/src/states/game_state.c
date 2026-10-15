#include "game_state.h"

extern uint8_t scancode;

extern Menu_state menu_state;
extern Level_state level_state;

extern uint8_t *drawing_frame_buffer;
extern struct packet mouse_packet;

extern uint16_t yRes, xRes;

Item_state item_state = INIT;
Panda_state panda_state = INACTIVE;

uint32_t y_time = 0;

int16_t ini_y;

bool isRightPressed = false;
bool isLeftPressed = false;

void (update_keyboard_game)(){
    read_scancode();

    switch (scancode) {
        case Q_KEY:
            menu_state = LEVEL_SELECTION;
            break;
        case R_KEY: case E_KEY:
            if (item_state == DRAG) update_item_animation();
            break;
        case ESC_BREAK:
            menu_state = EXIT;
            break;
        case ARROW_UP:
            /* go to jump state */
            if (panda_state == INACTIVE || panda_state == RUN){
                y_time = 0;
                ini_y = panda->y;
                panda_state = JUMP;
            }
            break;
        case ARROW_LEFT_PRESS:
            /* go to run state */
            isLeftPressed = true;
            if (panda_state == INACTIVE) panda_state = RUN;
            break;
        case ARROW_LEFT_RELEASE:
            /* go back to idle state */
            panda->i = 3;
            isLeftPressed = false;
            if (panda_state == RUN) panda_state = INACTIVE;
            break;
        case ARROW_RIGHT_PRESS:
            /* go to run state */
            isRightPressed = true;
            if (panda_state == INACTIVE) panda_state = RUN;
            break;
        case ARROW_RIGHT_RELEASE:
            /* go back to idle state */
            panda->i = 0;
            isRightPressed = false;
            if (panda_state == RUN) panda_state = INACTIVE;
            break;
    }
}

void (update_mouse_game)(){
    switch (item_state) {
        case INIT:
            if (mouse_packet.lb && is_selected_item()) item_state = DRAG;
            break;
        case DRAG:
            update_item_pos();
            if (!mouse_packet.lb) {
                check_item_pos();
                item_state = INIT;
            }
    }
}

void (update_timer_game)(){
    
    if (panda_state == DEAD) menu_state = GAME_OVER;
    
    else if (collide(panda, home) && !is_in_map(bamboo)){
        menu_state = LEVEL_SELECTION;
        level_state = (level_state == LEVEL_ONE) ? LEVEL_TWO : LEVEL_THREE;
    }

    else{
        timer_int_handler();
        if (counter == 2700) {
            panda_state = DEAD;
            return;
        }
        if (item_state == INIT) update_panda_state();
    }
}

void (update_panda_state)(){

    if (collide(panda, bamboo)) update_inventory();

    /* if there's nothing below the panda, it should fall */
    if (panda_state != JUMP && panda_state != FALL && !above_any_item()) {
        panda_state = FALL;
        ini_y = panda->y;
        y_time = 0;
    }

    if (hit_floor() || collide(panda, lava)) {
        panda_state = DEAD;
        return;
    }

    switch (panda_state) {
        case JUMP:
            /* update the panda pos */
            y_time++;
            jump(y_time, ini_y);

            /* if the panda hits the roof */
            if (panda->y < 25) {
                handle_boundary_conditions();
                panda_state = FALL;
                y_time = 0;
                ini_y = panda->y;
            }

            /* if the move causes a collision with any item */
            if (collide_with_items()) {
                /* correct the panda pos */
                fix_jump_collision();
                /* the panda should fall if it collides */
                panda_state = FALL;
                y_time = 0;
                ini_y = panda->y;
            }

            /* if the panda has ended the jump successfully, it should fall */
            else if (panda->yspeed == 0){
                ini_y = panda->y;
                y_time = 0;
                panda_state = FALL;
            }
            break;

        case FALL:
            /* update the panda pos */
            y_time++;
            fall(y_time, ini_y);

            /* if the move causes a collision with any item */
            if (collide_with_items()) {
                /* correct the panda pos */
                fix_fall_collision();
                panda_state = INACTIVE;
            }

            /* if the panda hits the floor */
            else if (hit_floor()){
                panda_state = DEAD;
                return;
            }
            break;

        default:
            break;
    }
    /* if the panda moves left or right */
    if (isRightPressed || isLeftPressed) {
        /* update the panda pos */
        isRightPressed ? move_right() : move_left();
        update_panda_animation();

        /* handle left and right bounds */
        handle_boundary_conditions();

        /* if the move causes a collision with any item */
        if (collide_with_items()) {
            /* correct the panda pos */
            fix_collision();
            panda_state = INACTIVE;
        }
    }
}
