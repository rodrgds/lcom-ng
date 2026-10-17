#include "Game.h"


void (init_game)(Game * game, uint32_t width, uint32_t height, Video_Mode vm){
     
    game->video_mode = vm;
    game->g_state = MENU;
    game->img_state = CHANGE_BG;
    game->lim_x = width;
    game->lim_y = height;
    game->pontuation = 0;
    game->text.cur_key = 0;
    game->wait_time = 20;
    game->added_top_name = false;

    for (int i = 0 ; i < 3 ; i++){
        for (int j = 0 ; j < 10 ; j++){
            game->top_score_list[i].name[j] = NO_KEY;
        }
        game->top_score_list[i].score = 0;
    }

    for (int i = 0 ; i < 10 ; i++){
        game->text.key_list[i] = NO_KEY;
    }

    init_mouse(game);

    init_menu(game);

    init_buttons(game);

    init_pontuation(game);

    init_leaderboard(game);
}

void (init_mouse)(Game * game){ // INIT MOUSE
    game->mouse.hitbox.pos.x = (game->lim_x)/2 - 5;
    game->mouse.hitbox.pos.y = (game->lim_y)/2 - 5;
    game->mouse.hitbox.width = 10;
    game->mouse.hitbox.height = 10;
    game->mouse.hitbox.color = (game->video_mode == INDEXED) ? 25 : 0xffffff;
    game->mouse.hitbox.active = true;
    game->mouse.mouse_state = M_IDLE;
}

void (init_menu)(Game * game){  // INIT MAIN MENUS

    // PLAY
    game->menu_elements[0].width = 179;
    game->menu_elements[0].height = 51;
    game->menu_elements[0].pos.x = (game->lim_x)/2 - (game->menu_elements[0].width)/2;
    game->menu_elements[0].pos.y = (game->lim_y)/2 - (game->menu_elements[0].height)/2 - 75;
    game->menu_elements[0].color = (game->video_mode == INDEXED) ? 3 : 0x00f0f0;
    game->menu_elements[0].active = false;

    // LEADERBOARD
    game->menu_elements[1].width = 500;
    game->menu_elements[1].height = 50;
    game->menu_elements[1].pos.x = (game->lim_x)/2 - (game->menu_elements[1].width)/2;
    game->menu_elements[1].pos.y = (game->lim_y)/2 - (game->menu_elements[1].height)/2 - 75 + 125;
    game->menu_elements[1].color = (game->video_mode == INDEXED) ? 3 : 0x00f0f0;
    game->menu_elements[1].active = false;

    // EXIT
    game->menu_elements[2].width = 155;
    game->menu_elements[2].height = 50;
    game->menu_elements[2].pos.x = (game->lim_x)/2 - (game->menu_elements[2].width)/2;
    game->menu_elements[2].pos.y = (game->lim_y)/2 - (game->menu_elements[2].height)/2 - 75 + 250;
    game->menu_elements[2].color = (game->video_mode == INDEXED) ? 3 : 0x00f0f0;
    game->menu_elements[2].active = false;
}

void (init_buttons)(Game * game){ // INIT MOLES
    Rectangle r = {{0,0},0,0,0,0};
    Button m = {0, 0, {0,0,0,0,0,0,0,0,0,0}, 0, r, ALIVE, RED_MOLE, {0,0,0,0,0,0,0,0,0,0}, 0};

    for (int i = 0; i < 9; i++){
        game->buttons[i] = m;
    }

    int idx = 0;
    int type = 0;
    for (int i = 1; i <= 3; i++){
        for (int j = 1; j <= 3; j++){
            game->buttons[idx].hitbox.width = 100;
            game->buttons[idx].hitbox.height = 100;
            game->buttons[idx].hitbox.pos.x = ((game->lim_x)/4 * j - (game->buttons[idx].hitbox.width)/2);
            game->buttons[idx].hitbox.pos.y = ((game->lim_y)/4 * i - (game->buttons[idx].hitbox.height)/2);
            game->buttons[idx].hitbox.color = (game->video_mode == INDEXED) ? 20 : 0xFF0000;
            game->buttons[idx].hitbox.active = true;
            switch (type % 3){
                case 0: {
                    update_button_type(game->video_mode, &game->buttons[idx], RED_MOLE);
                    break;
                }
                case 1: {
                    update_button_type(game->video_mode, &game->buttons[idx], GREEN_MOLE);
                    break;
                }
                case 2: {
                    update_button_type(game->video_mode, &game->buttons[idx], BLUE_MOLE);
                    break;
                }
                case 3: {
                    update_button_type(game->video_mode, &game->buttons[idx], YELLOW_MOLE);
                    break;
                }
                case 4: {
                    update_button_type(game->video_mode, &game->buttons[idx], BLACK_MOLE);
                    break;
                }
                case 5: {
                    update_button_type(game->video_mode, &game->buttons[idx], WHITE_MOLE);
                    break;
                }
                default: break;
            }
            idx++;
        }
        type++;
    }
    for (int i = 0 ; i < 9 ; i++){
        create_time_to_appear(&game->buttons[i]);
        create_type_to_appear(&game->buttons[i]);
    }
}

void init_pontuation(Game *game) {
    game->pontuation_menu.width = 576;
    game->pontuation_menu.height = 432;
    game->pontuation_menu.pos.x = (game->lim_x)/2 - (game->pontuation_menu.width)/2;
    game->pontuation_menu.pos.y = (game->lim_y)/2 - (game->pontuation_menu.height)/2;
    game->pontuation_menu.color = (game->video_mode == INDEXED) ? 1 : 0xffffff;
    game->pontuation_menu.active = false;

    game->pontuation_elements[0].width = 100;
    game->pontuation_elements[0].height = 50;
    game->pontuation_elements[0].pos.x = (game->lim_x)/2 - (game->pontuation_menu.width)/2 + 10;
    game->pontuation_elements[0].pos.y = (game->lim_y)/2 + (game->pontuation_menu.height)/2 - 10 - game->pontuation_elements[0].height;
    game->pontuation_elements[0].color = (game->video_mode == INDEXED) ? 2 : 0xff0000;
    game->pontuation_elements[0].active = false;

    game->pontuation_elements[1].width = 158;
    game->pontuation_elements[1].height = 50;
    game->pontuation_elements[1].pos.x = (game->lim_x)/2 - (game->pontuation_elements[1].width)/2;
    game->pontuation_elements[1].pos.y = (game->lim_y)/2 + (game->pontuation_menu.height)/2 - 10 -  game->pontuation_elements[1].height;
    game->pontuation_elements[1].color = (game->video_mode == INDEXED) ? 3 : 0x00ff00;
    game->pontuation_elements[1].active = false;

    game->pontuation_elements[2].width = 100;
    game->pontuation_elements[2].height = 50;
    game->pontuation_elements[2].pos.x = (game->lim_x)/2 + (game->pontuation_menu.width)/2 - game->pontuation_elements[2].width - 10;
    game->pontuation_elements[2].pos.y = (game->lim_y)/2 + (game->pontuation_menu.height)/2 - game->pontuation_elements[2].height - 10;
    game->pontuation_elements[2].color = (game->video_mode == INDEXED) ? 4 : 0x0000ff;
    game->pontuation_elements[2].active = false;
}

void init_leaderboard(Game *game) {
    game->leaderboard_menu.width = 750;
    game->leaderboard_menu.height = 432;
    game->leaderboard_menu.pos.x = (game->lim_x)/2 - (game->leaderboard_menu.width)/2;
    game->leaderboard_menu.pos.y = (game->lim_y)/2 - game->leaderboard_menu.height/2;
    game->leaderboard_menu.color = (game->video_mode == INDEXED) ? 1 : 0xffffff;
    game->leaderboard_menu.active = false;

    game->leaderboard_elements[0].width = 100;
    game->leaderboard_elements[0].height = 50;
    game->leaderboard_elements[0].pos.x = (game->lim_x)/2 - (game->leaderboard_elements[0].width)/2;
    game->leaderboard_elements[0].pos.y = (game->lim_y)/2 + (game->leaderboard_menu.height)/2 - game->leaderboard_elements[0].height - 10;
    game->leaderboard_elements[0].color = (game->video_mode == INDEXED) ? 2 : 0xff0000;
    game->leaderboard_elements[0].active = false;
}

int (process_data)(Game * game, int mouse_data[2], bool click, int keyboard_data[3], bool kb_press){

    Keyboard_Key key = KB_IDLE;

    if ((process_mouse_data_movement(game, mouse_data)) != 0) return 1;
    if (kb_press) {
        key = process_keyboard_data_input(game, keyboard_data);
    }

    switch (game->g_state){
        case MENU: {
            if (click){
                if (hit(&game->mouse.hitbox, &game->menu_elements[0])){
                    game->img_state = CHANGE_BG;
                    game->g_state = RUNNING;
                    game->game_timer = 1800;         // Run timer 300 = 5 sec, 360 = 6 sec, 660 = 11 sec, 1800 = 30 sec, 3600 = 1 min.
                    game->pontuation = 0;
                    for (int i = 0 ; i < 9 ; i++){
                        enable_button(&game->buttons[i]);
                    }
                }
                else if (hit(&game->mouse.hitbox, &game->menu_elements[1])){
                    for (int i = 0 ; i < 3 ; i++){
                        game->menu_elements[i].active = false;
                    }
                    game->g_state = LEADERBOARD;
                    game->img_state = CHANGE_ELEM;
                }
                else if (hit(&game->mouse.hitbox, &game->menu_elements[2])){
                    game->g_state = CLOSE;
                }
            }
            break;
        }
        
        case RUNNING: {
            game->added_top_name = false;
            if (--game->game_timer <= 0){
                for (int i = 0; i < 3; i++) {
                    game->pontuation_elements[i].active = true;
                }
                game->g_state = PONTUATION;
                game->img_state = CHANGE_ELEM;
            }

            if (game->game_timer % 60 == 0){
                game->img_state = CHANGE_ELEM;
            }
            
            else if (key == ESC_KEY){
                game->g_state = MENU;
                game->img_state = CHANGE_BG;
            }
            
            else {
                for (int i = 0 ; i < 9 ; i++){
                    if (alert_button(&game->buttons[i], &game->mouse.hitbox, click, key)){
                        game->img_state = CHANGE_ELEM;
                        if (game->buttons[i].button_state == HIT){
                            score_points(game, game->buttons[i].button_type);
                        }
                    }
                }
            }
            break;
        }
        
        case PONTUATION: {
            
            if (key != KB_IDLE){
                process_text_key(game, key);
                game->img_state = CHANGE_ELEM;
            }

            if (key == ENTER_KEY && !game->added_top_name){
                if ((process_pontuation(game)) != 0) return 1;
                game->added_top_name = true;
            }
            
            if (click){
                if (hit(&game->mouse.hitbox, &game->pontuation_elements[0])) {
                    leave_pontuation(game);

                    game->g_state = MENU;
                    game->img_state = CHANGE_BG;
                }

                else if (hit(&game->mouse.hitbox, &game->pontuation_elements[1])) {
                    leave_pontuation(game);

                    game->g_state = WAIT_LB;
                }
                
                else if (hit(&game->mouse.hitbox, &game->pontuation_elements[2])) {
                    leave_pontuation(game);

                    for (int i = 0; i < 9; i++){
                        enable_button(&game->buttons[i]);
                    }

                    game->game_timer = 1800;        // Run timer 300 = 5 sec, 360 = 6 sec, 660 = 11 sec, 1800 = 30 sec, 3600 = 1 min.
                    game->pontuation = 0;

                    game->g_state = RUNNING;
                    game->img_state = CHANGE_ELEM;
                }
            }
            break;
        }
        
        case LEADERBOARD: {
            if (click){
                if (hit(&game->mouse.hitbox, &game->leaderboard_elements[0])) {
                    game->leaderboard_elements[0].active = false;
                    game->g_state = WAIT_MENU;
                }
            }
            break;
        }
        
        case WAIT_MENU : {
            if (--game->wait_time <= 0){
                game->g_state = MENU;
                game->img_state = CHANGE_BG;
                game->wait_time = 20;
            }
            break;
        }

        case WAIT_LB : {
            if (--game->wait_time <= 0){
                game->g_state = LEADERBOARD;
                game->img_state = CHANGE_ELEM;
                game->wait_time = 20;
            }
            break;
        }
        
        default: break;
    }

    return 0;
}

int (process_mouse_data_movement)(Game * game, int mouse_data[2]){

    int new_x = game->mouse.hitbox.pos.x + mouse_data[0];
    int new_y = game->mouse.hitbox.pos.y - mouse_data[1];
    
    if (new_x < 0) new_x = 0;
    if (new_y < 0) new_y = 0;

    if (new_x >= game->lim_x) new_x = game->lim_x - 1;
    if (new_y >= game->lim_y) new_y = game->lim_y - 1;

    game->mouse.hitbox.pos.x = new_x;
    game->mouse.hitbox.pos.y = new_y;

    return 0;
}

Keyboard_Key (process_keyboard_data_input)(Game * game, int keyboard_data[3]){
    switch (keyboard_data[0]){
        case KBC_A_BR: return A_KEY;
        case KBC_B_BR: return B_KEY;
        case KBC_C_BR: return C_KEY;
        case KBC_D_BR: return D_KEY;
        case KBC_E_BR: return E_KEY;
        case KBC_F_BR: return F_KEY;
        case KBC_G_BR: return G_KEY;
        case KBC_H_BR: return H_KEY;
        case KBC_I_BR: return I_KEY;
        case KBC_J_BR: return J_KEY;
        case KBC_K_BR: return K_KEY;
        case KBC_L_BR: return L_KEY;
        case KBC_M_BR: return M_KEY;
        case KBC_N_BR: return N_KEY;
        case KBC_O_BR: return O_KEY;
        case KBC_P_BR: return P_KEY;
        case KBC_Q_BR: return Q_KEY;
        case KBC_R_BR: return R_KEY;
        case KBC_S_BR: return S_KEY;
        case KBC_T_BR: return T_KEY;
        case KBC_U_BR: return U_KEY;
        case KBC_V_BR: return V_KEY;
        case KBC_W_BR: return W_KEY;
        case KBC_X_BR: return X_KEY;
        case KBC_Y_BR: return Y_KEY;
        case KBC_Z_BR: return Z_KEY;
        case KBC_BKSPC : return BKSPC_KEY;
        case KBC_ENTER_BR: return ENTER_KEY;
        case KBC_SPACE_BR : return SPACE_KEY;
        case KBC_ESC_BR: return ESC_KEY;
        case KBC_2_BYTE: {
            switch (keyboard_data[1]) {
                case KBC_UP_BR: return UP_KEY;
                case KBC_LEFT_BR: return LEFT_KEY;
                case KBC_RIGHT_BR: return RIGHT_KEY;
                case KBC_DOWN_BR: return DOWN_KEY;
                default: break;
            }
        }
        default: break;
    }
    return 0;
}

void (score_points)(Game * game, Button_Type mt){
    switch (mt) {
        case WHITE_MOLE: { 
            game->pontuation += 10;
            break;
        }
        
        case RED_MOLE: { 
            game->pontuation += 50;
            break;
        }
        
        case GREEN_MOLE: { 
            game->pontuation += 50;
            break;
        }
        
        case BLUE_MOLE: { 
            game->pontuation += 50;
            break;
        }
        
        case YELLOW_MOLE: { 
            game->pontuation += 50;
            break;
        }
        
        case BLACK_MOLE: { 
            game->pontuation += 5;
            break;
        }
        
        default: break;
    }
}

void (process_text_key)(Game * game, Keyboard_Key key){
    switch (key) {
        case ESC_KEY:
        case UP_KEY:
        case LEFT_KEY:
        case DOWN_KEY:
        case RIGHT_KEY:
        case NO_KEY: break;
        
        case BKSPC_KEY: {
            if (game->text.cur_key > 0){
                game->text.cur_key--;
            }
            game->text.key_list[game->text.cur_key] = NO_KEY;
            break;
        }
        
        case ENTER_KEY: break;
        
        default: {
            if (game->text.cur_key < 9){
                game->text.key_list[game->text.cur_key] = key;
                game->text.cur_key++;
            } 
            break;
        }
    }
}

void (copy_key_list)(Keyboard_Key kl_1[10], Keyboard_Key kl_2[10]){
    for (int i = 0 ; i < 10 ; i++){
        kl_1[i] = kl_2[i]; 
    }
}

int (process_pontuation)(Game * game){
    for (int i = 0 ; i < 3 ; i++){
        if (game->pontuation > game->top_score_list[i].score){
            uint16_t tmp_1 = game->top_score_list[i].score, tmp_2;
            Keyboard_Key k_1[10], k_2[10];

            copy_key_list(k_1, game->top_score_list[i].name);

            game->top_score_list[i].score = game->pontuation;
            copy_key_list(game->top_score_list[i].name, game->text.key_list);

            for (int j = i + 1 ; j < 3 ; j++){
                tmp_2 = tmp_1;
                copy_key_list(k_2, k_1);
                tmp_1 = game->top_score_list[j].score;
                copy_key_list(k_1, game->top_score_list[j].name);
                game->top_score_list[j].score = tmp_2;
                copy_key_list(game->top_score_list[j].name, k_2);
            }
            break;
        }
    }

    return 0;
}

void (leave_pontuation)(Game * game) {
    for (int i = 0; i < 3; i++) {
        game->pontuation_elements[i].active = false;
    }

    for (int i = 0; i < 10; i++){
        game->text.cur_key = 0;
        game->text.key_list[i] = NO_KEY;
    }
}
