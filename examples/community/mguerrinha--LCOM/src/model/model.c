#include "model.h"

SystemState systemState = RUN;
MenuState menuState = MENU;

extern uint8_t scancode;
extern uint8_t currentIndex;
extern MouseInfo mouse_info;
extern vbe_mode_info_t mode_info;
extern PlayerInfo player1_info;
extern PlayerInfo player2_info;
extern int p1_counter_territory;
extern int p2_counter_territory;
extern int timer_counter;
extern bool two_byte;

enum direction aux_dir_p1 = STOP;
enum direction aux_dir_p2 = STOP;
Arena arena_matrix;
int game_counter;
uint16_t old_y_mouse = 300;
uint16_t old_x_mouse = 400;
uint8_t aux_scancode;
bool pressed = false;
bool pressed2 = false;

Sprite *mouse;
Sprite *menu;
Sprite *player1;
Sprite *player2;
Sprite *p1_up;
Sprite *p1_down;
Sprite *p1_left;
Sprite *p1_right;
Sprite *p2_up;
Sprite *p2_down;
Sprite *p2_left;
Sprite *p2_right;
Sprite *arena;
Sprite *num0;
Sprite *num1;
Sprite *num2;
Sprite *num3;
Sprite *num4;
Sprite *num5;
Sprite *num6;
Sprite *num7;
Sprite *num8;
Sprite *num9;
Sprite *percentage;
Sprite *twopoints;
Sprite *bar;
Sprite * p1win;
Sprite * p2win;
Sprite *controls;
Sprite *numSprites[10];

void (setup_sprites)() {
    mouse = create_sprite((xpm_map_t) mouse_xpm);
    menu = create_sprite((xpm_map_t) menu_xpm);
    player1 = create_sprite((xpm_map_t) player1_xpm);
    player2 = create_sprite((xpm_map_t) player2_xpm);
    p1_up = create_sprite((xpm_map_t) p1_up_xpm);
    p1_left = create_sprite((xpm_map_t) p1_left_xpm);
    p1_right = create_sprite((xpm_map_t) p1_right_xpm);
    p2_up = create_sprite((xpm_map_t) p2_up_xpm);
    p2_left = create_sprite((xpm_map_t) p2_left_xpm);
    p2_right = create_sprite((xpm_map_t) p2_right_xpm);
    arena = create_sprite((xpm_map_t) arena_xpm);
    num0 = create_sprite((xpm_map_t) num0_xpm);
    num1 = create_sprite((xpm_map_t) num1_xpm);
    num2 = create_sprite((xpm_map_t) num2_xpm);
    num3 = create_sprite((xpm_map_t) num3_xpm);
    num4 = create_sprite((xpm_map_t) num4_xpm);
    num5 = create_sprite((xpm_map_t) num5_xpm);
    num6 = create_sprite((xpm_map_t) num6_xpm);
    num7 = create_sprite((xpm_map_t) num7_xpm);
    num8 = create_sprite((xpm_map_t) num8_xpm);
    num9 = create_sprite((xpm_map_t) num9_xpm);
    percentage = create_sprite((xpm_map_t) percentage_xpm);
    twopoints = create_sprite((xpm_map_t) twopoints_xpm);
    bar = create_sprite((xpm_map_t) bar_xpm);
    p1win = create_sprite((xpm_map_t) p1win_xpm);
    p2win = create_sprite((xpm_map_t) p2win_xpm);
    controls = create_sprite((xpm_map_t) controls_xpm);
    numSprites[0] = num0;
    numSprites[1] = num1;
    numSprites[2] = num2;
    numSprites[3] = num3;
    numSprites[4] = num4;
    numSprites[5] = num5;
    numSprites[6] = num6;
    numSprites[7] = num7;
    numSprites[8] = num8;
    numSprites[9] = num9;
}

void (free_sprites)() {
    destroy_sprite(mouse);
    destroy_sprite(menu);
    destroy_sprite(player1);
    destroy_sprite(player2);
    destroy_sprite(p1_up);
    destroy_sprite(p1_left);
    destroy_sprite(p1_right);
    destroy_sprite(p2_up);
    destroy_sprite(p2_left);
    destroy_sprite(p2_right);
    destroy_sprite(arena);
    destroy_sprite(num0);
    destroy_sprite(num1);
    destroy_sprite(num2);
    destroy_sprite(num3);
    destroy_sprite(num4);
    destroy_sprite(num5);
    destroy_sprite(num6);
    destroy_sprite(num7);
    destroy_sprite(num8);
    destroy_sprite(num9);
    destroy_sprite(percentage);
    destroy_sprite(twopoints);
    destroy_sprite(bar);
    destroy_sprite(p1win);
    destroy_sprite(p2win);
    destroy_sprite(controls);
}

void (change_timer_state)() {
    if (DOUBLE_BUFFERING) sync_to_main_buffer();
    timer_int_handler();
    if (timer_counter % 60 == 0 && menuState == PLAY && game_counter > 0) {
        game_counter--;
        if (game_counter == 0) {
        menuState = END;
        }
    }
}

void (change_rtc_state)() {
    if (timer_counter % 3600 == 0) { 
        update_date();
    }
}

void (change_kb_state)() {
    kbc_ih();
    if (scancode == 0xE0) {
        two_byte = true;
        return;
    }
    switch (scancode) {
        case Q_KEY:
            systemState = QUIT;
            break;

        case M_KEY:
            menuState = MENU;
            break;

        case C_KEY:
            if (menuState != MENU) return;
            menuState = CONTROLS;
            break;

        case ENTER_KEY:
            if (menuState == CONTROLS) return;
            if (menuState == PLAY) return;
            menuState = PLAY;
            game_counter = 180;
            player1_info.id = 1;
            player1_info.x = 200;
            player1_info.y = 300;
            player1_info.dir = STOP;
            player1_info.out_of_territory = 0;
            player2_info.id = 2;
            player2_info.x = 600;
            player2_info.y = 300;
            player2_info.dir = STOP;
            player2_info.out_of_territory = 0;
            create_player_area(mode_info.XResolution, mode_info.YResolution);
            count_territory();
            free_end_menu_buffer();
            break;

        case W_KEY:
            if (menuState == PLAY && player1_info.dir != DOWN && player1_info.dir != UP){
                aux_dir_p1 = UP;
            }
            break;

        case A_KEY:
            if (menuState == PLAY && player1_info.dir != RIGHT && player1_info.dir != LEFT){
                aux_dir_p1 = LEFT;
            }
            break;

        case S_KEY:
            if (menuState == PLAY && player1_info.dir != UP && player1_info.dir != DOWN){
                aux_dir_p1 = DOWN;
            }
            break;

        case D_KEY:
            if (menuState == PLAY && player1_info.dir != LEFT && player1_info.dir != RIGHT) {
                aux_dir_p1 = RIGHT;
            }
            break;
        
        case UP_ARROW_KEY:
            if (two_byte == true && menuState == PLAY && player2_info.dir != DOWN && player2_info.dir != UP){
                aux_dir_p2 = UP;
                two_byte = false;
            }
            break;

        case LEFT_ARROW_KEY:
            if (two_byte == true && menuState == PLAY && player2_info.dir != RIGHT && player2_info.dir != LEFT){
                aux_dir_p2 = LEFT;
                two_byte = false;
            }
            break;
    
        case RIGHT_ARROW_KEY:
            if (two_byte == true && menuState == PLAY && player2_info.dir != LEFT && player2_info.dir != RIGHT) {
                aux_dir_p2 = RIGHT;
                two_byte = false;
            }
            break;

        case DOWN_ARROW_KEY:
            if (two_byte == true && menuState == PLAY && player2_info.dir != UP && player2_info.dir != DOWN){
                aux_dir_p2 = DOWN;
                two_byte = false;
            }
            break;
        default:
            break;
    }
}

void (change_mouse_state)() {
    mouse_ih();
    mouse_sync_bytes();
    if (currentIndex == 3) {
        mouse_move_to_info();
        currentIndex = 0;
        if (menuState == MENU) {
            if (mouse_info.l_click) {
                if (mouse_info.x >= 440 && mouse_info.x <= 760 && mouse_info.y >= 23 && mouse_info.y <= 70) {
                    menuState = PLAY;
                    game_counter = 180;
                    player1_info.id = 1;
                    player1_info.x = 200;
                    player1_info.y = 300;
                    player1_info.dir = STOP;
                    player1_info.out_of_territory = 0;
                    player2_info.id = 2;
                    player2_info.x = 600;
                    player2_info.y = 300;
                    player2_info.dir = STOP;
                    player2_info.out_of_territory = 0;
                    create_player_area(mode_info.XResolution, mode_info.YResolution);
                    count_territory();
                    free_menu_buffer();
                }
                if (mouse_info.x >= 34 && mouse_info.x <= 366 && mouse_info.y >= 540 && mouse_info.y <= 571) {
                    menuState = CONTROLS;
                }
            }
        }
        
        else if (menuState == CONTROLS) {
            if (mouse_info.l_click) {
                if (mouse_info.x >= 320 && mouse_info.x <= 440 && mouse_info.y >= 110 && mouse_info.y <= 130) {
                    menuState = MENU;
                }
            }
        }

        else if (menuState == PLAY) {
            if (pressed == false && mouse_info.l_click) {
                pressed = true;
                old_x_mouse = mouse_info.x;
                old_y_mouse = mouse_info.y;
            }

            if (pressed == true && !mouse_info.l_click) {
                pressed = false;
                uint16_t dif_position_x = abs(mouse_info.x - old_x_mouse);
                uint16_t dif_position_y = abs(mouse_info.y - old_y_mouse);
                if (dif_position_x > dif_position_y) {
                    if (mouse_info.x < old_x_mouse && player1_info.dir != RIGHT && player1_info.dir != LEFT) {
                        aux_dir_p1 = LEFT;
                    }
                    else if (mouse_info.x > old_x_mouse && player1_info.dir != LEFT && player1_info.dir != RIGHT) {
                        aux_dir_p1 = RIGHT;
                    }
                }
                else {
                    if (mouse_info.y < old_y_mouse && player1_info.dir != DOWN && player1_info.dir != UP) {
                        aux_dir_p1 = UP;
                    
                    }
                    
                    else if (mouse_info.y > old_y_mouse && player1_info.dir != UP && player1_info.dir != DOWN) {
                        aux_dir_p1 = DOWN;
                        
                    }
                }
            }

            if (pressed2 == false && mouse_info.r_click) {
                pressed2 = true;
                old_x_mouse = mouse_info.x;
                old_y_mouse = mouse_info.y;
            }

            if (pressed2 == true && !mouse_info.r_click) {
                pressed2 = false;
                uint16_t dif_position_x = abs(mouse_info.x - old_x_mouse);
                uint16_t dif_position_y = abs(mouse_info.y - old_y_mouse);
                if (dif_position_x > dif_position_y) {
                    if (mouse_info.x < old_x_mouse && player2_info.dir != RIGHT && player2_info.dir != LEFT) {
                        aux_dir_p2 = LEFT;
                    }
                    else if (mouse_info.x > old_x_mouse && player2_info.dir != LEFT && player2_info.dir != RIGHT) {
                        aux_dir_p2 = RIGHT;
                    }
                }
                else {
                    if (mouse_info.y < old_y_mouse && player2_info.dir != DOWN && player2_info.dir != UP) {
                        aux_dir_p2 = UP;
                    
                    }
                    
                    else if (mouse_info.y > old_y_mouse && player2_info.dir != UP && player2_info.dir != DOWN) {
                        aux_dir_p2 = DOWN;
                        
                    }
                }
            }
        }
        
        else if (menuState == END) {
            if (mouse_info.l_click) {
                if (p1_counter_territory > p2_counter_territory) {
                    if (mouse_info.x >= 20 && mouse_info.x <= 395 && mouse_info.y >= 10 && mouse_info.y <= 45) {
                        menuState = PLAY;
                        game_counter = 180;
                        player1_info.id = 1;
                        player1_info.x = 200;
                        player1_info.y = 300;
                        player1_info.dir = STOP;
                        player1_info.out_of_territory = 0;
                        player2_info.id = 2;
                        player2_info.x = 600;
                        player2_info.y = 300;
                        player2_info.dir = STOP;
                        player2_info.out_of_territory = 0;
                        create_player_area(mode_info.XResolution, mode_info.YResolution);
                        count_territory();
                        free_menu_buffer();
                    }
                    else if (mouse_info.x >= 480 && mouse_info.x <= 760 && mouse_info.y >= 555 && mouse_info.y <= 592) {
                        menuState = MENU;
                    }
                }
                else {
                    if (mouse_info.x >= 400 && mouse_info.x <= 770 && mouse_info.y >= 15 && mouse_info.y <= 50) {
                        menuState = PLAY;
                        game_counter = 180;
                        player1_info.id = 1;
                        player1_info.x = 200;
                        player1_info.y = 300;
                        player1_info.dir = STOP;
                        player1_info.out_of_territory = 0;
                        player2_info.id = 2;
                        player2_info.x = 600;
                        player2_info.y = 300;
                        player2_info.dir = STOP;
                        player2_info.out_of_territory = 0;
                        create_player_area(mode_info.XResolution, mode_info.YResolution);
                        count_territory();
                        free_menu_buffer();
                    }
                    else if (mouse_info.x >= 34 && mouse_info.x <= 320 && mouse_info.y >= 555 && mouse_info.y <= 592) {
                        menuState = MENU;
                    }
                }
            }
        }
    }
    
}

void (create_player_area)(uint16_t width, uint16_t height) {
    arena_matrix.width = width;
    arena_matrix.height = height;
    arena_matrix.map = (uint8_t**)malloc(height * sizeof(uint8_t*));
    for (int i = 0; i < height; i++) {
        arena_matrix.map[i] = (uint8_t*)malloc(width * sizeof(uint8_t));
    }
    for (int i = 0; i < arena_matrix.height; i++) {
        for (int j = 0; j < arena_matrix.width; j++) {
            arena_matrix.map[i][j] = 0;
        }
    }
    for (int i = player1_info.y - 20; i < player1_info.y + 40; i++) {
        for (int j = player1_info.x - 20; j < player1_info.x + 40; j++) {
            arena_matrix.map[i][j] = 1;
        }
    }
    for (int i = player2_info.y - 20; i < player2_info.y + 40; i++) {
        for (int j = player2_info.x - 20; j < player2_info.x + 40; j++) {
            arena_matrix.map[i][j] = 2;
        }
    }
}
