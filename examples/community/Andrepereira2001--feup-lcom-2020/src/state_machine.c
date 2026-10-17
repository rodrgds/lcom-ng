#include "state_machine.h"
#include "sprite.h"
#include "i8042.h"
#include "game.h"
#include "uartVars.h"

/**
 * @brief saves the state of the screen
 * 
 */
extern screen_state_t screen_state;

/**
 * @brief the event that append is considered a click
 *
 */
bool click = false;

/**
 * @brief while game running represent the state of the game
 * 
 */
game_state_t game_state = CATCH;

//singleplayer
/**
 * @brief type of the cath of player 1
 */
int catch_type = 0, 
    catch_idx;/**< @brief index of the item catched by player 1 */ 

//multiplayer
/**
 * @brief type of the cath of player 2
 */
int catch_type2 = 0,
    catch_idx2; /**< @brief index of the item catched by player 2 */

 /**
  * @brief true if some machine is logged in the multiplayer
  * 
  */
extern bool playerIn;

/**
 * @brief true if multiplayer in one machine have been chosen
 */
extern bool one_machine_multiplayer;

//uart variavels 
/**
 * @brief byte that contain info about the current state of the 4 movemente keys
 * 
 */
uint8_t keys_byte = BEGIN_KEYS_BYTE;

//keyboar variavels
bool a_isDown = false, /**< @brief true if A key is down */
 d_isDown = false, /**< @brief true if D key is down */
 w_isDown = false, /**< @brief true if W key is down */
 s_isDown = false, /**< @brief true if S key is down */
 up_isDown = false, /**< @brief true if UP ARROW key is down */
 down_isDown = false, /**< @brief true if DOWN ARROW key is down */
 right_isDown = false, /**< @brief true if RIGHT ARROW key is down */
 left_isDown = false;  /**< @brief true if LEFT ARROW key is down */

//mouse variavels
bool rIsDown = false, /**< @brief true if mouse RIGHT botton is down */
 lIsDown = false, /**< @brief true if mouse LEFT botton is down */
 mIsDown = false; /**< @brief true if mouse MIDLE botton is down */



//KEYBOARD STATE MACHINE

/** this function validates the received keyboard interrupt
 *  after calls the functions to make the changes
 **/
void kbc_interrupt_validation(unsigned size ,uint8_t bytes[]){
    key_event event = KBC_INVALID;

    if(size == 1){
        if(kbc_get_event(bytes[0],&event) != 0){
            printf("Error in kbc_get_event size 1\n");
            return;
        }
    }
    else if(size == 2){
        if(kbc_get_event(bytes[1],&event) !=0){
            printf("Error in kbc_get_event size 2\n");
            return;
        }
    }

    kbc_event_handler(event);
}

int kbc_get_event(uint8_t code,key_event * event){

    if(code == MAKE_KEY_A && !a_isDown){
        *event = A_DOWN;
        a_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_A && a_isDown){
        *event = A_UP;
        a_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_D && !d_isDown){
        *event = D_DOWN;
        d_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_D && d_isDown){
        *event = D_UP;
        d_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_W && !w_isDown){
        *event = W_DOWN;
        w_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_W && w_isDown){
        *event = W_UP;
        w_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_S && !s_isDown){
        *event = S_DOWN;
        s_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_S && s_isDown){
        *event = S_UP;
        s_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_LEFT && !left_isDown && !playerIn){
        *event = LEFT_DOWN;
        left_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_LEFT && left_isDown && !playerIn){
        *event = LEFT_UP;
        left_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_RIGHT && !right_isDown && !playerIn){
        *event = RIGHT_DOWN;
        right_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_RIGHT && right_isDown && !playerIn){
        *event = RIGHT_UP;
        right_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_UP && !up_isDown && !playerIn){
        *event = UP_DOWN;
        up_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_UP && up_isDown && !playerIn){
        *event = UP_UP;
        up_isDown = false;
        return 0;
    }
    else if(code == MAKE_KEY_DOWN && !down_isDown && !playerIn){
        *event = DOWN_DOWN;
        down_isDown = true;
        return 0;
    }
    else if(code == BREAK_KEY_DOWN && down_isDown && !playerIn){
        *event = DOWN_UP;
        down_isDown = false;
        return 0;
    }
    else if(code == MAKE_ESC_DOWN){
        *event = ESC_DOWN;
        return 0;
    }
    else if(code == MAKE_ENTER_DOWN){
        *event = ENTER_DOWN;
        return 0;
    }
    else{
        *event = KBC_INVALID;
        return 0;
    }
    return 1;
}

int kbc_event_handler(key_event event){
    if(screen_state == GAME ){
        if(event == ESC_DOWN){
            screen_state = SCORE;
            add_score();
        }
        if(event == A_DOWN){
            set_car_speed(-1);
        }
        else if(event == A_UP){
            set_car_speed(0);
            if(d_isDown){
                set_car_speed(1);
            }
        }
        if(event == D_DOWN){
            set_car_speed(1);
        }
        else if(event == D_UP){
            set_car_speed(0);
            if(a_isDown){
                set_car_speed(-1);
            }
        }
        if(event == S_DOWN){
            set_claw_yspeed(1);
        }
        else if(event == S_UP){
            set_claw_yspeed(0);
            if(w_isDown){
                set_claw_yspeed(-1);
            }
        }
        if(event == W_DOWN){
            set_claw_yspeed(-1);
        }
        else if(event == W_UP){
            set_claw_yspeed(0);
            if(s_isDown){
                set_claw_yspeed(1);
            }
        }
    }
    else if(screen_state == LOADING){
        if(event == ESC_DOWN){
            playerIn = false;
            screen_state = MENU;
            send_keys_byte(UART_EXIT);
        }
        if(event == ENTER_DOWN){
            playerIn = false;
            one_machine_multiplayer = true;
            send_keys_byte(UART_EXIT);
            screen_state = MULTIPLAYER;
            reset_game(false);
        }
    }
    else if(screen_state == MULTIPLAYER){
        if(event == ESC_DOWN){
            if(one_machine_multiplayer){
                one_machine_multiplayer = false;
            }
            else{
                playerIn = false;
                send_keys_byte(UART_EXIT);
            }
            screen_state = MULTI_SCORE;
        }
        if(event == A_DOWN){
            set_car_speed(-1);
            keys_byte = 0x3F & keys_byte;
            keys_byte |= BITE_A_MAKE;
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        else if(event == A_UP){
            set_car_speed(0);
            keys_byte = 0x3F & keys_byte; //A A_ D D_ W W_ S S_    00111111   00010101
            keys_byte |= BITE_A_BREAK; // 01010101
            if(d_isDown){
                set_car_speed(1);
                keys_byte = 0xCF & keys_byte;
                keys_byte |= BITE_D_MAKE;
            }
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        if(event == D_DOWN){
            set_car_speed(1);
            keys_byte = 0xCF & keys_byte;
            keys_byte |= BITE_D_MAKE;
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        else if(event == D_UP){
            set_car_speed(0);
            keys_byte = 0xCF & keys_byte;
            keys_byte |= BITE_D_BREAK;
            if(a_isDown){
                set_car_speed(-1);
                keys_byte = 0x3F & keys_byte;
                keys_byte |= BITE_A_MAKE;
            }
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        if(event == S_DOWN){
            set_claw_yspeed(1);
            keys_byte = 0xFC & keys_byte;
            keys_byte |= BITE_S_MAKE;
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        else if(event == S_UP){
            set_claw_yspeed(0);
            keys_byte = 0xFC & keys_byte;
            keys_byte |= BITE_S_BREAK;
            if(w_isDown){
                set_claw_yspeed(-1);
                keys_byte = 0xF3 & keys_byte;
                keys_byte |= BITE_W_MAKE;
            }
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        if(event == W_DOWN){
            set_claw_yspeed(-1);
            keys_byte = 0xF3 & keys_byte;
            keys_byte |= BITE_W_MAKE;
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        else if(event == W_UP){
            set_claw_yspeed(0);
            keys_byte = 0xF3 & keys_byte;
            keys_byte |= BITE_W_BREAK;
            if(s_isDown){
                set_claw_yspeed(1);
                keys_byte = 0xFC & keys_byte;
                keys_byte |= BITE_S_MAKE;
            }
            if(!one_machine_multiplayer)
                send_keys_byte(keys_byte);
        }
        if(event == LEFT_DOWN){
            set_car2_speed(-1);
        }
        else if(event == LEFT_UP){
            set_car2_speed(0);
            if(right_isDown){
                set_car2_speed(1);
            }
        }
        if(event == RIGHT_DOWN){
            set_car2_speed(1);
        }
        else if(event == RIGHT_UP){
            set_car2_speed(0);
            if(left_isDown){
                set_car2_speed(-1);
            }
        }
        if(event == DOWN_DOWN){
            set_claw2_yspeed(1);
        }
        else if(event == DOWN_UP){
            set_claw2_yspeed(0);
            if(up_isDown){
                set_claw2_yspeed(-1);
            }
        }
        if(event == UP_DOWN){
            set_claw2_yspeed(-1);
        }
        else if(event == UP_UP){
            set_claw2_yspeed(0);
            if(down_isDown){
                set_claw2_yspeed(1);
            }
        }

    }
    else if(event == ESC_DOWN){
        screen_state = MENU;
    }
    
    return 0;
}
//MOUSE STATE MACHINE
int mouse_get_event_type(struct packet *pp,mouse_event_type * event){

    if(pp->lb == 1 && !lIsDown){
        *event = LDOWN;
        lIsDown = true;
    }
    else if(pp->lb == 0 && lIsDown){
        *event = LUP;
        lIsDown = false;
    }
    else if(pp->rb == 1 && !rIsDown){
        *event = RDOWN;
        rIsDown = true;
    }
    else if(pp->rb == 0 && rIsDown){
        *event = RUP;
        rIsDown = false;
    }
    else if(pp->mb == 1 && !mIsDown){
        *event = MDOWN;
        rIsDown = true;
        printf("mdown\n");
    }
    else if(pp->mb == 0 && mIsDown){
        *event = MUP;
        rIsDown = false;
        printf("rup\n");
    }
    else if(pp->delta_x != 0 || pp->delta_y != 0){
        *event = MOVE;
    }
    else{
        *event = INVALID;
        printf("INVALID\n");
    }
    
    return 0;
}

int mouse_event_handler(struct packet *pp,mouse_event_type event){
    click = false;
    if(event == MOVE && screen_state != SCORE){
        if(!pp->x_ov && !pp->y_ov)
            move_cursor(pp->delta_x, pp->delta_y);
        else if(pp->x_ov){
            move_cursor(0, pp->delta_y);
            printf("Cursor X overflow!!\n");
        }
        else if(pp->y_ov){
            move_cursor(pp->delta_x, 0);
            printf("Cursor Y overflow!!\n");
        }
    }
    //while in game
    else if(event == LDOWN){ 
        change_sprite_animation_cursor();
        click = true;
    }
    else if(event == LUP){
        change_sprite_animation_cursor();
    }
    return 0;
}

void handle_game_changes(long long unsigned int * score,int * clock_counter){
    if(game_state == CATCH){
        handle_sprite_changes(0,0,0,score,clock_counter,click);
    }
    else if(game_state == DROP){
        handle_sprite_changes(1,catch_type,catch_idx,score,clock_counter,click);
    }
}

void handle_multiplayer_changes(long long unsigned int * score,int * clock_counter,long long unsigned int * score2,int * clock_counter2){
    if(game_state == CATCH){
        handle_multiplayer_sprite_changes(0,0,0,0,0,score,score2,clock_counter,clock_counter2);
    }
    else if(game_state == DROP){
        handle_multiplayer_sprite_changes(1,catch_type,catch_idx,catch_type2,catch_idx2,score,score2,clock_counter,clock_counter2);
    }
    else if(game_state == DROP1){
        handle_multiplayer_sprite_changes(2,catch_type,catch_idx,0,0,score,score2,clock_counter,clock_counter2);
    }
    else if(game_state == DROP2){
        handle_multiplayer_sprite_changes(3,0,0,catch_type2,catch_idx2,score,score2,clock_counter,clock_counter2);
    }

}

void handle_menu_screen(Date_t * date){
    int menu = draw_menu_screen(click,date);
    if(menu == 0){
        screen_state = MENU;
    }
    else if(menu == 1){
        screen_state = GAME;
        reset_game(false);
    }
    else if(menu == 2){
        if(playerIn){
            send_keys_byte(UART_SEED);
            uint8_t seed = (uint8_t) rand();
            set_seed(seed);
            send_keys_byte(seed);
            tickdelay(20);
            send_keys_byte(UART_LOAD);
            reset_game(true);
            screen_state = MULTIPLAYER;
        }
        else{
            screen_state = LOADING;
            playerIn = true;
            send_keys_byte(UART_LOAD);
        }
    }
    else if(menu == 3){
        screen_state = LEADERBOARD;
    }
    else if(menu == 4){
        screen_state = EXIT;
    }
}

void set_drop_state(int type, int idx){
    catch_type = type;
    catch_idx = idx;
    game_state = DROP;
}

void set_catch_state(){
    catch_type = 0;
    game_state = CATCH;
}

//multi
void set_multiplayer_drop_state(int player,int type, int idx){
    if(player == 1){
        catch_type = type;
        catch_idx = idx;
        if(game_state == CATCH){
            game_state = DROP1;
        }
        else if(game_state == DROP2){
            game_state = DROP;
        }
    }
    if(player == 2){
        catch_type2 = type;
        catch_idx2 = idx;
        if(game_state == CATCH){
            game_state = DROP2;
        }
        else if(game_state == DROP1){
            game_state = DROP;
        }
    }
}

//multi
void set_multiplayer_catch_state(int player){
    if(player == 1){
        catch_type = 0;
        catch_idx = 0;
        if(game_state == DROP){
            game_state = DROP2;
        }
        else if(game_state == DROP1){
            game_state = CATCH;
        }
    }
    if(player == 2){
        catch_type2 = 0;
        catch_idx2 = 0;
        if(game_state == DROP){
            game_state = DROP1;
        }
        else if(game_state == DROP2){
            game_state = CATCH;
        }
    }
}

int key_byte_get_event(uint8_t code,key_event * event){

    if( ((code & BITE_A_MAKE) == BITE_A_MAKE) && !left_isDown){
        *event = LEFT_DOWN;
        left_isDown = true;
        return 0;
    }
    else if( ((code & BITE_A_BREAK) == BITE_A_BREAK) && left_isDown){
        *event = LEFT_UP;
        left_isDown = false;
        return 0;
    }
    else if( ((code & BITE_D_MAKE) == BITE_D_MAKE) && !right_isDown){
        *event = RIGHT_DOWN;
        right_isDown = true;
        return 0;
    }
    else if( ((code & BITE_D_BREAK) == BITE_D_BREAK) && right_isDown){
        *event = RIGHT_UP;
        right_isDown = false;
        return 0;
    }
    else if( ((code & BITE_W_MAKE) == BITE_W_MAKE) && !up_isDown){
        *event = UP_DOWN;
        up_isDown = true;
        return 0;
    }
    else if( ((code & BITE_W_BREAK) == BITE_W_BREAK) && up_isDown){
        *event = UP_UP;
        up_isDown = false;
        return 0;
    }
    else if( ((code & BITE_S_MAKE) == BITE_S_MAKE) && !down_isDown){
        *event = DOWN_DOWN;
        down_isDown = true;
        return 0;
    }
    else if( ((code & BITE_S_BREAK) == BITE_S_BREAK) && down_isDown){
        *event = DOWN_UP;
        down_isDown = false;
        return 0;
    }
    else{
        *event = KBC_INVALID;
    }
    return 1;
}
