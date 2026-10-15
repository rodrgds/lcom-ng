#include "../xpm/cursor.xpm"
#include "../xpm/ninja/ninja-down.xpm"
#include "../xpm/ninja/ninja-up.xpm"
#include "../xpm/ninja/ninja-diagonal-up.xpm"
#include "../xpm/ninja/ninja-diagonal-down.xpm"
#include "../xpm/ninja/ninja-right.xpm"
#include "../xpm/monster/monster-down.xpm"
#include "../xpm/monster/monster-up.xpm"
#include "../xpm/monster/monster-diagonal-up.xpm"
#include "../xpm/monster/monster-diagonal-down.xpm"
#include "../xpm/monster/monster-right.xpm"
#include "../xpm/backgrounds/fundo1.xpm"
#include "../xpm/backgrounds/fundo1-limites.xpm"
#include "../xpm/backgrounds/fundo2.xpm"
#include "../xpm/backgrounds/fundo2-limites.xpm"
#include "../xpm/backgrounds/fundo3.xpm"
#include "../xpm/backgrounds/fundo3-limites.xpm"
#include "../xpm/backgrounds/you_win.xpm"
#include "../xpm/backgrounds/you_win-limites.xpm"
#include "../xpm/game_elements/heart.xpm"
#include "../xpm/game_elements/star.xpm"
#include "../xpm/speed_ninja/speed-ninja-down.xpm"
#include "../xpm/speed_ninja/speed-ninja-up.xpm"
#include "../xpm/speed_ninja/speed-ninja-diagonal-up.xpm"
#include "../xpm/speed_ninja/speed-ninja-diagonal-down.xpm"
#include "../xpm/speed_ninja/speed-ninja-right.xpm"
#include "../xpm/game_elements/door.xpm"
#include "../xpm/game_elements/bar-speed.xpm"
#include "../xpm/game_elements/knife-diagonal-down.xpm"
#include "../xpm/game_elements/knife-diagonal-up.xpm"
#include "../xpm/game_elements/knife-down.xpm"
#include "../xpm/game_elements/knife-up.xpm"
#include "../xpm/game_elements/knife-right.xpm"
#include "../xpm/game_elements/key.xpm"
#include "../xpm/game_elements/explosion1.xpm"
#include "../xpm/game_elements/explosion2.xpm"
#include "../xpm/game_elements/explosion3.xpm"
#include "../xpm/game_elements/explosion4.xpm"
#include "../xpm/game_elements/explosion5.xpm"
#include "../xpm/game_elements/explosion6.xpm"
#include "../xpm/game_elements/reward.xpm"

#include <lcom/lcf.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "../states/game.h"

extern uint8_t scancode;
extern vbe_mode_info_t mode_info; // for the mouse
extern struct mouse_packet pack;
extern int state;
extern int substate; // para o menu
extern int (*map_funcs[2][4])(void);
int (*validate_pos[2])(int, int) = {check_player_col, check_knife_col};
bool reversed = false; // we use the same sprite for left and right so we need to know to reverse
bool reversed_monster1 = false;
bool reversed_monster2 = false;
bool reversed_monster3 = false;
bool reversed_monster4 = false;
bool reversed_knife = false;
bool star_collected[3] = {false};
bool heart_collected[3] = {false};
bool key_collected = false;
bool door_open = false;
bool exploded[3] = {false};
int count = 0;
int speed_scale = 1;
int stamina = 100;
int monster_move = 0;    
int level = 0;
int knife_active = 0;  
int player_vision = 1; // valores do enum direction mas associados à visao do player
int bomb_state[3][6] = {{0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}};
int bomb_interval[3] = {0};

const int monster_interval = 5;
const int bomb_interval_limit = 50;

struct track_press keys = {0, 0, 0, 0};
struct Sprite *wall_limit;
struct Sprite *mouse, *ninja_up, *ninja_down, *ninja_dup, *ninja_ddown, *ninja_right, *background, *monster1_up, *monster1_down, *monster1_dup, *monster1_ddown, *monster1_right,  *monster2_up, *monster2_down, *monster2_dup, *monster2_ddown, *monster2_right,  *monster3_up, *monster3_down, *monster3_dup, *monster3_ddown, *monster3_right,  *monster4_up, *monster4_down, *monster4_dup, *monster4_ddown, *monster4_right, *bar_speed, *door, *knife_up, *knife_down, *knife_dup, *knife_ddown, *knife_right, *key_door, *reward;
struct Sprite **player = &ninja_down; // current player sprite being displayed
struct Sprite* hearts[3];
struct Sprite *ninja_lives[3];
struct Sprite* stars[3];
struct Sprite **monster1 = &monster1_down;
struct Sprite **monster2 = &monster2_down;
struct Sprite **monster3 = &monster3_down;
struct Sprite **monster4 = &monster4_down;
struct Sprite **monsters[4];
struct Sprite **knife_sprite;
struct Sprite ***mov_sprites[6] = {&player, &knife_sprite, &monster1, &monster2, &monster3, &monster4};
bool monster_dead[4] = {false};
struct Sprite* explosion[3][6];

int lives;
int collision_timer = 0;

int (load_game_state)() {
    loadSprite(&mouse, (xpm_map_t)cursor_xpm);
    loadSprite(&ninja_down, (xpm_map_t)ninja_down_xpm);
    loadSprite(&ninja_up, (xpm_map_t)ninja_up_xpm);
    loadSprite(&ninja_dup, (xpm_map_t)ninja_diagonal_up_xpm);
    loadSprite(&ninja_ddown, (xpm_map_t)ninja_diagonal_down_xpm);
    loadSprite(&ninja_right, (xpm_map_t)ninja_right_xpm);

    loadSprite(&monster1_down, (xpm_map_t)monster_down_xpm);
    loadSprite(&monster1_up, (xpm_map_t)monster_up_xpm);
    loadSprite(&monster1_dup, (xpm_map_t)monster_diagonal_up_xpm);
    loadSprite(&monster1_ddown, (xpm_map_t)monster_diagonal_down_xpm);
    loadSprite(&monster1_right, (xpm_map_t)monster_right_xpm);
    
    loadSprite(&background, (xpm_map_t)fundo1_xpm);
    loadSprite(&wall_limit, (xpm_map_t)fundo1_limites_xpm);
    loadSprite(&bar_speed, (xpm_map_t)bar_speed_xpm);

    loadSprite(&knife_down, (xpm_map_t)knife_down_xpm);
    loadSprite(&knife_up, (xpm_map_t)knife_up_xpm);
    loadSprite(&knife_dup, (xpm_map_t)knife_diagonal_up_xpm);
    loadSprite(&knife_ddown, (xpm_map_t)knife_diagonal_down_xpm);
    loadSprite(&knife_right, (xpm_map_t)knife_right_xpm);
    
    bar_speed->x_pos = 650;
    bar_speed->y_pos = 575;
    loadSprite(&door, (xpm_map_t)door_xpm);
    
    loadSprite(&key_door, (xpm_map_t)key_xpm);

    for (int i = 0; i < 3; i++) {
        loadSprite(&hearts[i], (xpm_map_t)heart_xpm);
        loadSprite(&stars[i], (xpm_map_t)star_xpm);
    }
    
    lives = 3;
    for(int i = 0; i < lives; i++) {
        loadSprite(&ninja_lives[i], (xpm_map_t)heart_xpm);
    }

    loadSprite(&explosion[0][0], (xpm_map_t)explosion1_xpm);
    loadSprite(&explosion[0][1], (xpm_map_t)explosion2_xpm);
    loadSprite(&explosion[0][2], (xpm_map_t)explosion3_xpm);
    loadSprite(&explosion[0][3], (xpm_map_t)explosion4_xpm);
    loadSprite(&explosion[0][4], (xpm_map_t)explosion5_xpm);
    loadSprite(&explosion[0][5], (xpm_map_t)explosion6_xpm);

    loadSprite(&explosion[1][0], (xpm_map_t)explosion1_xpm);
    loadSprite(&explosion[1][1], (xpm_map_t)explosion2_xpm);
    loadSprite(&explosion[1][2], (xpm_map_t)explosion3_xpm);
    loadSprite(&explosion[1][3], (xpm_map_t)explosion4_xpm);
    loadSprite(&explosion[1][4], (xpm_map_t)explosion5_xpm);
    loadSprite(&explosion[1][5], (xpm_map_t)explosion6_xpm);

    loadSprite(&explosion[2][0], (xpm_map_t)explosion1_xpm);
    loadSprite(&explosion[2][1], (xpm_map_t)explosion2_xpm);
    loadSprite(&explosion[2][2], (xpm_map_t)explosion3_xpm);
    loadSprite(&explosion[2][3], (xpm_map_t)explosion4_xpm);
    loadSprite(&explosion[2][4], (xpm_map_t)explosion5_xpm);
    loadSprite(&explosion[2][5], (xpm_map_t)explosion6_xpm);

    
    monster2_down = malloc(sizeof(struct Sprite));
    monster2_up = malloc(sizeof(struct Sprite));
    monster2_dup = malloc(sizeof(struct Sprite));
    monster2_ddown = malloc(sizeof(struct Sprite));
    monster2_right = malloc(sizeof(struct Sprite));

    monster3_down = malloc(sizeof(struct Sprite));
    monster3_up = malloc(sizeof(struct Sprite));
    monster3_dup = malloc(sizeof(struct Sprite));
    monster3_ddown = malloc(sizeof(struct Sprite));
    monster3_right = malloc(sizeof(struct Sprite));

    monster4_down = malloc(sizeof(struct Sprite));
    monster4_up = malloc(sizeof(struct Sprite));
    monster4_dup = malloc(sizeof(struct Sprite));
    monster4_ddown = malloc(sizeof(struct Sprite));
    monster4_right = malloc(sizeof(struct Sprite));

    memcpy(monster2_down, monster1_down, sizeof(struct Sprite));
    memcpy(monster2_up, monster1_up, sizeof(struct Sprite));
    memcpy(monster2_dup, monster1_dup, sizeof(struct Sprite));
    memcpy(monster2_ddown, monster1_ddown, sizeof(struct Sprite));
    memcpy(monster2_right, monster1_right, sizeof(struct Sprite));

    memcpy(monster3_down, monster1_down, sizeof(struct Sprite));
    memcpy(monster3_up, monster1_up, sizeof(struct Sprite));
    memcpy(monster3_dup, monster1_dup, sizeof(struct Sprite));
    memcpy(monster3_ddown, monster1_ddown, sizeof(struct Sprite));
    memcpy(monster3_right, monster1_right, sizeof(struct Sprite));

    memcpy(monster4_down, monster1_down, sizeof(struct Sprite));
    memcpy(monster4_up, monster1_up, sizeof(struct Sprite));
    memcpy(monster4_dup, monster1_dup, sizeof(struct Sprite));
    memcpy(monster4_ddown, monster1_ddown, sizeof(struct Sprite));
    memcpy(monster4_right, monster1_right, sizeof(struct Sprite));

    monsters[0] = monster1;
    monsters[1] = monster2;
    monsters[2] = monster3;
    monsters[3] = monster4;


    player = &ninja_down;
    reversed = false;

    stars[0]->x_pos = 65; stars[0]->y_pos = 130;
    stars[1]->x_pos = 715; stars[1]->y_pos = 230;
    stars[2]->x_pos = 100; stars[2]->y_pos = 470;

    door->x_pos = 340; door->y_pos = 135;
    key_door->x_pos = 710; key_door->y_pos = 340;

    hearts[0]->x_pos = 220; hearts[0]->y_pos = 200;
    hearts[1]->x_pos = 465; hearts[1]->y_pos = 125;
    hearts[2]->x_pos = 715; hearts[2]->y_pos = 500;

    ninja_down->x_pos = 350;
    ninja_down->y_pos = 300;

    monster1_down ->x_pos = 465;
    monster1_down->y_pos = 330;

    monster2_down ->x_pos = 120;
    monster2_down->y_pos = 270;

    monster3_down ->x_pos = 280;
    monster3_down->y_pos = 470;

    monster4_down->x_pos = 520;
    monster4_down->y_pos = 230;
    bomb_state[0][0] = 0; bomb_state[0][1] = 0;
    bomb_state[0][2] = 0; bomb_state[0][3] = 0;
    bomb_state[0][4] = 0; bomb_state[0][5] = 0;

    bomb_state[1][0] = 0; bomb_state[1][1] = 0;
    bomb_state[1][2] = 0; bomb_state[1][3] = 0;
    bomb_state[1][4] = 0; bomb_state[1][5] = 0;
        
    bomb_state[2][0] = 0; bomb_state[2][1] = 0;
    bomb_state[2][2] = 0; bomb_state[2][3] = 0;
    bomb_state[2][4] = 0; bomb_state[2][5] = 0;
        
    bomb_interval[0] = 0;
    bomb_interval[1] = 0;
    bomb_interval[2] = 0;
    exploded[0] = false; exploded[1] = false; exploded[2] = false;

    for(int i = 0; i < 6; i++) {
        explosion[0][i]->x_pos = 620; explosion[0][i]->y_pos = 500;
    }

    for(int i = 0; i < 6; i++) {
        explosion[1][i]->x_pos = 100; explosion[1][i]->y_pos = 150;
    }

    for(int i = 0; i < 6; i++) {
        explosion[2][i]->x_pos = 650; explosion[2][i]->y_pos = 200;
    }

    player = &ninja_down;
    monster1 = &monster1_down;
    monster2 = &monster2_down;
    monster3 = &monster3_down;
    monster4 = &monster4_down;
    reversed = false;
    knife_sprite = &knife_down;

    for (int i=0; i < 3; i++) {
        star_collected[i] = false;
        heart_collected[i] = false;
    }
    for (int i = 0; i < 4; i++) monster_dead[i] = false;
    door_open = false;
    key_collected = false;
    level = 0;
    keys.w = false; keys.a = false; keys.s = false; keys.d = false;
    return 0;
}

int (load_game_level)() {
    keys.w = false; keys.a = false; keys.s = false; keys.d = false;
    player = &ninja_down;
    reversed = false;
    monster1 = &monster1_down;
    monster2 = &monster2_down;
    monster3 = &monster3_down;
    monster4 = &monster4_down;
    reversed = false;
    knife_sprite = &knife_down;

    for (int i=0; i < 3; i++) {
        star_collected[i] = false;
        heart_collected[i] = false;
    }
    for (int i = 0; i < 4; i++) monster_dead[i] = false;
    door_open = false;
    key_collected = false;
    knife_sprite = &knife_down;

    if (level == 1) {
        loadSprite(&background, (xpm_map_t)fundo2_xpm);
        loadSprite(&wall_limit, (xpm_map_t)fundo2_limites_xpm);

        stars[0]->x_pos = 709; stars[0]->y_pos = 90;
        stars[1]->x_pos = 525; stars[1]->y_pos = 65;
        stars[2]->x_pos = 240; stars[2]->y_pos = 145;
        key_door->x_pos = 130; key_door->y_pos = 232;
        hearts[0]->x_pos = 240; hearts[0]->y_pos = 345;
        hearts[1]->x_pos = 568; hearts[1]->y_pos = 345;
        hearts[2]->x_pos = 60; hearts[2]->y_pos = 92;
        door->x_pos = 330; door->y_pos = 145;
        ninja_down->x_pos = 345;
        ninja_down->y_pos = 480;
        monster1_down ->x_pos = 703;
        monster1_down->y_pos = 285;
        monster2_down ->x_pos = 523;
        monster2_down->y_pos = 366;
        monster3_down ->x_pos = 663;
        monster3_down->y_pos = 460;
        monster4_down->x_pos = 480;
        monster4_down->y_pos = 193;
        bomb_state[0][0] = 0; bomb_state[0][1] = 0;
        bomb_state[0][2] = 0; bomb_state[0][3] = 0;
        bomb_state[0][4] = 0; bomb_state[0][5] = 0;

        bomb_state[1][0] = 0; bomb_state[1][1] = 0;
        bomb_state[1][2] = 0; bomb_state[1][3] = 0;
        bomb_state[1][4] = 0; bomb_state[1][5] = 0;
        
        bomb_state[2][0] = 0; bomb_state[2][1] = 0;
        bomb_state[2][2] = 0; bomb_state[2][3] = 0;
        bomb_state[2][4] = 0; bomb_state[2][5] = 0;
        
        bomb_interval[0] = 0;
        bomb_interval[1] = 0;
        bomb_interval[2] = 0;
        exploded[0] = false; exploded[1] = false; exploded[2] = false;
        for(int i = 0; i < 6; i++) {
            explosion[0][i]->x_pos = 320; explosion[0][i]->y_pos = 300;
        }

        for(int i = 0; i < 6; i++) {
            explosion[1][i]->x_pos = 100; explosion[1][i]->y_pos = 100;
        }

        for(int i = 0; i < 6; i++) {
            explosion[2][i]->x_pos = 100; explosion[2][i]->y_pos = 500;
        }

    }
    else if (level == 2) {
        loadSprite(&background, (xpm_map_t)fundo3_xpm);
        loadSprite(&wall_limit, (xpm_map_t)fundo3_limites_xpm);

        stars[0]->x_pos = 709; stars[0]->y_pos = 200;
        stars[1]->x_pos = 385; stars[1]->y_pos = 375;
        stars[2]->x_pos = 205; stars[2]->y_pos = 155;
        key_door->x_pos = 345; key_door->y_pos = 225;
        hearts[0]->x_pos = 205; hearts[0]->y_pos = 370;
        hearts[1]->x_pos = 455; hearts[1]->y_pos = 115;
        hearts[2]->x_pos = 60; hearts[2]->y_pos = 92;
        door->x_pos = 330; door->y_pos = 145;
        ninja_down->x_pos = 345;
        ninja_down->y_pos = 480;
        monster1_down ->x_pos = 130;
        monster1_down->y_pos = 230;
        monster2_down ->x_pos = 709;
        monster2_down->y_pos = 105;
        monster3_down ->x_pos = 709;
        monster3_down->y_pos = 480;
        monster4_down->x_pos = 60;
        monster4_down->y_pos = 480;
        bomb_state[0][0] = 0; bomb_state[0][1] = 0;
        bomb_state[0][2] = 0; bomb_state[0][3] = 0;
        bomb_state[0][4] = 0; bomb_state[0][5] = 0;

        bomb_state[1][0] = 0; bomb_state[1][1] = 0;
        bomb_state[1][2] = 0; bomb_state[1][3] = 0;
        bomb_state[1][4] = 0; bomb_state[1][5] = 0;
        
        bomb_state[2][0] = 0; bomb_state[2][1] = 0;
        bomb_state[2][2] = 0; bomb_state[2][3] = 0;
        bomb_state[2][4] = 0; bomb_state[2][5] = 0;
        
        bomb_interval[0] = 0;
        bomb_interval[1] = 0;
        bomb_interval[2] = 0;
        exploded[0] = false; exploded[1] = false; exploded[2] = false;

        for(int i = 0; i < 6; i++) {
            explosion[0][i]->x_pos = 700; explosion[0][i]->y_pos = 200;
        }

        for(int i = 0; i < 6; i++) {
            explosion[1][i]->x_pos = 375; explosion[1][i]->y_pos = 380;
        }

        for(int i = 0; i < 6; i++) {
            explosion[2][i]->x_pos = 100; explosion[2][i]->y_pos = 380;
        }
    }
    else if (level == 3) {
        loadSprite(&background, (xpm_map_t)you_win_xpm);
        loadSprite(&wall_limit, (xpm_map_t)you_win_limites_xpm);
        loadSprite(&reward, (xpm_map_t)reward_xpm);

        for (int i=0; i < 3; i++) {
            star_collected[i] = true;
            heart_collected[i] = true;
        }
        lives = 0;
        door_open = true;
        key_collected = true;
        bar_speed->width = 0;

        ninja_down->x_pos = 375;
        ninja_down->y_pos = 490;
        reward->x_pos = 365;
        reward->y_pos = 320;

        for (int i = 0; i < 4; i++) monster_dead[i] = true;

    }
    knife_sprite = &knife_down;

    return 0;
}

int (draw_game_state)() {
    drawSprite(background, false);
    if (!door_open) {
        drawSprite(door, false);
    }
    move_sprites();

    if (collision_timer > 0) {
        collision_timer--;
    } else {
        for(int i = 0; i < 4; i++) {
            if(check_col(*monsters[i]) && !monster_dead[i]) {
                lives--;
                collision_timer = 30;
            }
        }
    }

    if ((*player)->y_pos < 0) {
        level++;
        load_game_level();
        return 0;
    }

    monster_move++;
    if (monster_move >= monster_interval) {
        update_monsters();
        monster_move = 0; 
    }
    bar_speed->width = stamina;
    if (level != 3) drawSprite(bar_speed, false);
    if (level == 3) drawSprite(reward, false);
    drawSprite(*player, reversed);
    if (!monster_dead[0]) drawSprite(*monster1, reversed_monster1);
    if (!monster_dead[1]) drawSprite(*monster2, reversed_monster2);
    if (!monster_dead[2]) drawSprite(*monster3, reversed_monster3);
    if (!monster_dead[3]) drawSprite(*monster4, reversed_monster4);
    for (int i = 0; i < 3; i++) {
        if (is_near_bomb(i) && exploded[i] == false) {
            bomb_state[i][0] = 1;
        }
    }

    for (int j = 0; j < 3; j++) {
        if (bomb_state[j][0] == 1) {
            if (bomb_state[j][1] == 0) {
                drawSprite(explosion[j][0], false);
            }
            bomb_interval[j]++;
            for (int i = 1; i < 6; i++) {
                if(bomb_interval[j] == bomb_interval_limit && bomb_state[j][i] == 0) {
                    bomb_interval[j] = 0;
                    drawSprite(explosion[j][i], false);
                    bomb_interval[j] = 45;
                    bomb_state[j][i] = 1;
                }
            }
            if (bomb_state[j][5] == 1 && is_near_bomb(j)) {
                lives--;
                exploded[j] = true;
                for (int i = 0; i < 6; i++ ) {
                    bomb_state[j][i] = 0;
                }
            }
            if (bomb_state[j][5] == 1 && !is_near_bomb(j)) {
                exploded[j] = true;
                for (int i = 0; i < 6; i++ ) {
                    bomb_state[j][i] = 0;
                }
            }
        }
    }
    if(lives == 0 && level != 3) {
        state = 0;
        substate = 2;
        map_funcs[state][0]();
    }
    for (int i = 0; i < lives; i++) {
        ninja_lives[i]->x_pos = i * 25;
        ninja_lives[i]->y_pos = 5;
        drawSprite(ninja_lives[i], false);
    } 
    bool temp = false;
    for (int i = 0; i < 3; i++) {
        if (!star_collected[i]) {
            drawSprite(stars[i], false);
            temp = true;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (!heart_collected[i]) {
            drawSprite(hearts[i], false);
        }
    }
    if (!temp && !key_collected) {
        drawSprite(key_door, false);
    }
    if (knife_active) drawSprite(*knife_sprite, reversed_knife);
    drawSprite(*player, reversed);
    drawSprite(mouse, false);
    return 0;
}

int (mouse_game_ih)() {
    set_mouse_pos();
    check_mouse_pos();
    if (mouse_hover(door) && key_collected && right_mouse_clicked(&pack)) {
        door_open = true;
    }
    if (right_mouse_clicked(&pack) && level == 3 && mouse_hover(reward) && abs((*player)->x_pos - reward->x_pos + (*player)->y_pos - reward->y_pos) < 100) {
        usleep(500);
        substate = 0;
        state = 0;
        map_funcs[state][0]();
    }
    if (left_mouse_clicked(&pack) && knife_active == 0) {
        attack();
    }
    return 0;
}

void (check_mouse_pos)() {
    double center_x = (*player)->x_pos + (*player)->width / 2; // posicao x do centro do sprite do player
    double center_y = (*player)->y_pos + (*player)->height / 2; // posicao y do centro do sprite do player

    double dx = mouse->x_pos - center_x;
    double dy = -(mouse->y_pos - center_y);
    double angle = atan2(dy, dx);

    if ((angle <= M_PI && angle >= M_PI*7/8) || (angle >= -M_PI && angle < -(M_PI*7/8))) {
        change_player(&ninja_right, true);
        player_vision = 2;
    } else if (angle >= -M_PI/8 && angle < M_PI/8) {
        change_player(&ninja_right, false);
        player_vision = 3;
    } else if (angle >= M_PI*3/8 && angle < M_PI*5/8) {
        change_player(&ninja_up, false);
        player_vision = 0;
    } else if (angle >= -M_PI*5/8 && angle < -M_PI*3/8) { 
        change_player(&ninja_down, false);
        player_vision = 1;
    } else if (angle >= -M_PI*3/8 && angle < -M_PI/8) {
        change_player(&ninja_ddown, true);
        player_vision = 4;
    } else if (angle >= -M_PI*7/8 && angle < -M_PI*5/8) {
        change_player(&ninja_ddown, false);
        player_vision = 5;
    } else if (angle >= M_PI/8 && angle < M_PI*3/8) {
        change_player(&ninja_dup, true);
        player_vision = 6;
    } else if (angle >= M_PI*5/8 && angle < M_PI*7/8) {
        change_player(&ninja_dup, false);
        player_vision = 7;
    }
}

void (set_mouse_pos)() {
    mouse->x_pos += pack.x_pos;
    mouse->y_pos += pack.y_pos;

    if (mouse->x_pos < 0) mouse->x_pos = 0;
    else if (mouse->x_pos > (int)mode_info.XResolution - 1) mouse->x_pos = mode_info.XResolution - 1;

    if (mouse->y_pos < 0) mouse->y_pos = 0;
    else if (mouse->y_pos > (int)mode_info.YResolution - 1) mouse->y_pos = mode_info.YResolution - 1;
}

void move_monster(struct Sprite **monster, struct Sprite *up, struct Sprite *down, struct Sprite *right, struct Sprite *ddown, struct Sprite *dup, bool *reverse) {
    int hold_x = (*monster)->x_pos, hold_y = (*monster)->y_pos;
    int dist[2] = {0};
    dist[0] = ((*player)->x_pos) - ((*monster)->x_pos);
    dist[1]= ((*player)->y_pos) - ((*monster)->y_pos); 
    int direction = 8;
    if (abs(dist[0]) >= 100 && abs(dist[1]) >= 50) direction = rand() % 9; 
    else {
        int temp_x = dist[0], temp_y = dist[1];
        if (temp_x == 0 && temp_y < 0) direction = 0;
        else if (temp_x == 0 && temp_y > 0) direction = 1;
        else if (temp_x < 0 && temp_y == 0) direction = 2;
        else if (temp_x > 0 && temp_y == 0) direction = 3;
        else if (temp_x < 0 && temp_y < 0) direction = 7;
        else if (temp_x > 0 && temp_y < 0) direction = 6;
        else if (temp_x < 0 && temp_y > 0) direction = 5;
        else if (temp_x > 0 && temp_y > 0) direction = 4;
    }
    switch (direction) {
            case 0: *monster = up; *reverse = false; break;
            case 1: *monster = down; *reverse = false; break;
            case 2: *monster = right; *reverse = true; break;
            case 3: *monster = right; *reverse = false; break;
            case 4: *monster = ddown; *reverse = true; break;
            case 5: *monster = ddown; *reverse = false; break;
            case 6: *monster = dup; *reverse = true; break;
            case 7: *monster = dup; *reverse = false; break;
    }
    (*monster)->dir = direction;
    if (direction == STOP) (*monster)->speed = 0;
    else (*monster)->speed = 1;
    (*monster)->x_pos = hold_x;
    (*monster)->y_pos = hold_y;
}


void update_monsters() {
    move_monster(monster1, monster1_up, monster1_down, monster1_right, monster1_ddown, monster1_dup, &reversed_monster1);
    move_monster(monster2, monster2_up, monster2_down, monster2_right, monster2_ddown, monster2_dup, &reversed_monster2);
    move_monster(monster3, monster3_up, monster3_down, monster3_right, monster3_ddown, monster3_dup, &reversed_monster3);
    move_monster(monster4, monster4_up, monster4_down, monster4_right, monster4_ddown, monster4_dup, &reversed_monster4);
}

bool check_col(struct Sprite *element) {

    int player_x = (*player)->x_pos;
    int player_y = (*player)->y_pos;
    int player_width = (*player)->width;
    int player_height = (*player)->height;

    int x = element->x_pos;
    int y = element->y_pos;
    int width = element->width;
    int height = element->height;

    bool x_collided = (player_x < x + width) && (player_x + player_width > x);
    bool y_collided = (player_y < y + height) && (player_y + player_height > y);

    if (x_collided && y_collided) {
        return true;
    }
    return false;
}

int (kbd_game_ih)() {
    uint8_t key   =  scancode;

    switch (key) {
      case 0x01:
          state = 0;
          substate = 1;
          map_funcs[state][0]();
          return 0;
      case 0x11: 
        if (keys.w) return 0;
        process_keys(W);
        break;
      case 0x1F: 
        if (keys.s) return 0;
        process_keys(S);
        break;
      case 0x1E:
        if (keys.a) return 0;
        process_keys(A);
        break;
      case 0x20:
        if (keys.d) return 0;
        process_keys(D);
        break;
      case 0x91:
        if (!keys.w) return 0;
        process_keys(BW);
        break;
      case 0x9F:
        if (!keys.s) return 0;
        process_keys(BS);
        break;
      case 0x9E: 
        if (!keys.a) return 0;
        process_keys(BA);
        break;
      case 0xA0:
        if (!keys.d) return 0;
        process_keys(BD);
        break;
        case     0x39:
        if (speed_scale == 2) break;
        speed_scale = 2;
        change_ninja();
        break;
      case     0xB9:
        if (speed_scale == 1) break;
        speed_scale = 1;
        change_ninja();
        break;
      default:    return 0;
    }

    (*player)->speed = 5 * speed_scale;
    switch ((*player)->dir) {
        case UP: change_player(&ninja_up, false); break;
        case DOWN: change_player(&ninja_down, false); break;
        case RIGHT: change_player(&ninja_right, false); break;
        case LEFT: change_player(&ninja_right, true); break;
        case LUP: change_player(&ninja_dup, false); break;
        case RUP: change_player(&ninja_dup, true); break;
        case LDOWN: change_player(&ninja_ddown, false); break;
        case RDOWN: change_player(&ninja_ddown, true); break;
        case STOP: 
            (*player)->speed = 0;
            change_player(&ninja_down, false);
            break;
    }
    bool temp = false;
    for (int i=0; i < 3; i++) {
        if (star_collected[i]) continue;
        star_collected[i] = check_col(stars[i]);
        if (!star_collected[i]) temp = true;
    }
    for (int i=0; i < 3; i++) {
        if (heart_collected[i]) continue;
        if(check_col(hearts[i]) && lives < 3) {
            heart_collected[i] = true;
            lives++;
        }
    }
    if (!key_collected && !temp) {
        key_collected = check_col(key_door);
        if(key_collected) printf("key collected");
    }
    return 0;
}


void change_ninja() {
        int temp_speed = (*player)->speed, temp_x = (*player)->x_pos, temp_y = (*player)->y_pos;
        enum direction temp_dir = (*player)->dir;
        if (speed_scale == 2) {
            loadSprite(&ninja_down, (xpm_map_t)speed_ninja_down_xpm);
            loadSprite(&ninja_up, (xpm_map_t)speed_ninja_up_xpm);
            loadSprite(&ninja_dup, (xpm_map_t)speed_ninja_diagonal_up_xpm);
            loadSprite(&ninja_ddown, (xpm_map_t)speed_ninja_diagonal_down_xpm);
            loadSprite(&ninja_right, (xpm_map_t)speed_ninja_right_xpm);
        } else if (speed_scale == 1) {
            loadSprite(&ninja_down, (xpm_map_t)ninja_down_xpm);
            loadSprite(&ninja_up, (xpm_map_t)ninja_up_xpm);
            loadSprite(&ninja_dup, (xpm_map_t)ninja_diagonal_up_xpm);
            loadSprite(&ninja_ddown, (xpm_map_t)ninja_diagonal_down_xpm);
            loadSprite(&ninja_right, (xpm_map_t)ninja_right_xpm);
        }
        (*player)->x_pos = temp_x;
        (*player)->y_pos = temp_y;
        (*player)->speed = temp_speed;
        (*player)->dir = temp_dir;
    }

void change_player(struct Sprite **new, bool new_reversed) {
    (*new)->x_pos = (*player)->x_pos;
    (*new)->y_pos = (*player)->y_pos;
    (*new)->speed = (*player)->speed;
    (*new)->dir = (*player)->dir;
    reversed = new_reversed;
    player = new;
}

void process_keys(enum keys key) {
    switch (key)
    {
    case W:
        keys.w = true;
        if ((*player)->dir == STOP) (*player)->dir = UP;
        else if ((*player)->dir == DOWN) (*player)->dir = STOP;
        else if ((*player)->dir == RIGHT) (*player)->dir = RUP;
        else if ((*player)->dir == LEFT) (*player)->dir = LUP;
        else if ((*player)->dir == LDOWN) (*player)->dir = LEFT;
        else if ((*player)->dir == RDOWN) (*player)->dir = RIGHT;
        break;
    case S:
        keys.s = true;
        if ((*player)->dir == STOP) (*player)->dir = DOWN;
        else if ((*player)->dir == UP) (*player)->dir = STOP;
        else if ((*player)->dir == RIGHT) (*player)->dir = RDOWN;
        else if ((*player)->dir == LEFT) (*player)->dir = LDOWN;
        else if ((*player)->dir == LUP) (*player)->dir = LEFT;
        else if ((*player)->dir == RUP) (*player)->dir = RIGHT;
        break;
    case A:
        keys.a = true;
        if ((*player)->dir == STOP) (*player)->dir = LEFT;
        else if ((*player)->dir == DOWN) (*player)->dir = LDOWN;
        else if ((*player)->dir == RIGHT) (*player)->dir = STOP;
        else if ((*player)->dir == UP) (*player)->dir = LUP;
        else if ((*player)->dir == RUP) (*player)->dir = UP;
        else if ((*player)->dir == RDOWN) (*player)->dir = DOWN;
        break;
    case D:
        keys.d = true;
        if ((*player)->dir == STOP) (*player)->dir = RIGHT;
        else if ((*player)->dir == DOWN) (*player)->dir = RDOWN;
        else if ((*player)->dir == LEFT) (*player)->dir = STOP;
        else if ((*player)->dir == UP) (*player)->dir = RUP;
        else if ((*player)->dir == LUP) (*player)->dir = UP;
        else if ((*player)->dir == LDOWN) (*player)->dir = DOWN;
        break;
    case BW:
        keys.w = false;
        if ((*player)->dir == STOP) (*player)->dir = DOWN;
        else if ((*player)->dir == UP) (*player)->dir = STOP;
        else if ((*player)->dir == RIGHT) (*player)->dir = RDOWN;
        else if ((*player)->dir == LEFT) (*player)->dir = LDOWN;
        else if ((*player)->dir == RUP) (*player)->dir = RIGHT;
        else if ((*player)->dir == LUP) (*player)->dir = LEFT;
        break;
    case BS:
        keys.s = false;
        if ((*player)->dir == STOP) (*player)->dir = UP;
        else if ((*player)->dir == DOWN) (*player)->dir = STOP;
        else if ((*player)->dir == RIGHT) (*player)->dir = RUP;
        else if ((*player)->dir == LEFT) (*player)->dir = LUP;
        else if ((*player)->dir == LDOWN) (*player)->dir = LEFT;
        else if ((*player)->dir == RDOWN) (*player)->dir = RIGHT;
        break;
    case BA:
        keys.a = false;
        if ((*player)->dir == STOP) (*player)->dir = RIGHT;
        else if ((*player)->dir == DOWN) (*player)->dir = RDOWN;
        else if ((*player)->dir == LEFT) (*player)->dir = STOP;
        else if ((*player)->dir == UP) (*player)->dir = RUP;
        else if ((*player)->dir == LUP) (*player)->dir = UP;
        else if ((*player)->dir == LDOWN) (*player)->dir = DOWN;
        break;
    case BD:
        keys.d = false;
        if ((*player)->dir == STOP) (*player)->dir = LEFT;
        else if ((*player)->dir == DOWN) (*player)->dir = LDOWN;
        else if ((*player)->dir == RIGHT) (*player)->dir = STOP;
        else if ((*player)->dir == UP) (*player)->dir = LUP;
        else if ((*player)->dir == RUP) (*player)->dir = UP;
        else if ((*player)->dir == RDOWN) (*player)->dir = DOWN;
        break;
    default:
        break;
    }
}

int check_player_col(int x, int y) {
    for (int i = x; i < x + (*player)->width; i++) {
        uint32_t color = wall_limit->colors[wall_limit->width * (y + (*player)->height) + i];
        if ((color == 0x00FF1B) || (color == 0x00EAFF && speed_scale == 1) || (color == 0xE4FF00 && !door_open)) return 1;
    }
    return 0;
}

int check_knife_col(int x, int y) {
    for (int i = x; i < x + (*knife_sprite)->width; i++) {
        uint32_t color = wall_limit->colors[wall_limit->width * (y + (*knife_sprite)->height) + i];
        if ((color == 0x00FF1B) || (color == 0x00EAFF) || (color == 0xE4FF00)) {
            (*knife_sprite)->dir = STOP;
            (*knife_sprite)->speed = 0;
            knife_active = 0;
        }
        for (int i = 2; i < 6; i++) {
            if (!monster_dead[i - 2]) {
                struct Sprite **monster = (*mov_sprites[i]);
                if ((*monster)->x_pos <= x && (*monster)->x_pos + (*monster)->width >= x && 
                (*monster)->y_pos <= y && (*monster)->y_pos + (*monster)->height >= y) {
                    (*monster)->speed = 0;
                    (*monster)->dir = STOP;
                    monster_dead[i - 2] = true;
                    (*knife_sprite)->dir = STOP;
                    (*knife_sprite)->speed = 0;
                    knife_active = 0;
                }
            }
        }
    }
    return 0;
}

int move_sprites() {
    if (count % 2 == 0 && speed_scale == 2 && stamina > 0) stamina--;
    else if (count % 2 == 0 && speed_scale == 1 && stamina < 100) stamina++; 
    if (stamina == 0 && speed_scale == 2) {
        speed_scale = 1;
        change_ninja();
    }
    int n = sizeof(mov_sprites) / sizeof(struct Sprite ***); 
    for (int i = 0; i < n; i++) {
        if ((i > 1 && monster_dead[i-2]) || (i == 1 && knife_active == 0)) continue;
        struct Sprite ***playing = mov_sprites[i];
        struct Sprite **moving = *playing;
        int this_speed = (*moving)->speed;
        if (this_speed < 0) {
            if (count % this_speed == 0) {
                change_pos(moving, 1, i);
            }
        } else if (this_speed > 0) {
            change_pos(moving, this_speed, i);
        }
    }
    count += 1;
    if (count == 101) count = 1;
    return 0;
}

int change_pos(struct Sprite **moved, int pixs, int i) {
    if (i > 1) i = 0;
    int x = (*moved)->x_pos, y = (*moved)->y_pos;
    switch ((*moved)->dir) {
    case UP:
        if (validate_pos[i](x, y - pixs) == 1) return 0;
        (*moved)->y_pos -= pixs;
        break;
    case DOWN:
        if (validate_pos[i](x, y + pixs) == 1) return 0;
        (*moved)->y_pos += pixs;
        break;
    case LEFT:
        if (validate_pos[i](x - pixs, y) == 1) return 0;
        (*moved)->x_pos -= pixs;
        break;
    case RIGHT:
        if (validate_pos[i](x + pixs, y) == 1) return 0;
        (*moved)->x_pos += pixs;
        break;
    case RUP:
        if (validate_pos[i](x + pixs, y - pixs) == 1) return 0;
        (*moved)->x_pos += pixs;
        (*moved)->y_pos -= pixs;
        break;
    case LUP:
        if (validate_pos[i](x - pixs, y - pixs) == 1) return 0;
        (*moved)->x_pos -= pixs;
        (*moved)->y_pos -= pixs;
        break;
    case LDOWN:
        if (validate_pos[i](x - pixs, y + pixs) == 1) return 0;
        (*moved)->x_pos -= pixs;
        (*moved)->y_pos += pixs;
        break;
    case RDOWN:
        if (validate_pos[i](x + pixs, y + pixs) == 1) return 0;
        (*moved)->x_pos += pixs;
        (*moved)->y_pos += pixs;
        break;
    default:
        break;
    }
    return 0;
}

void attack() {
    knife_active = 1;

    int knife_x = (*player)->x_pos + (*player)->width / 2;;
    int knife_y = (*player)->y_pos + (*player)->height / 2;;

    switch (player_vision) {
        case UP:
            knife_sprite = &knife_up;
            reversed_knife =  false;          
            break;
        case DOWN:
            knife_sprite = &knife_down;
            reversed_knife = false;
            break;
        case RUP:
            knife_sprite = &knife_dup;
            reversed_knife = false;
            break;
        case LDOWN:
            knife_sprite = &knife_ddown;
            reversed_knife = false;
            break;
        case RIGHT:
            knife_sprite = &knife_right;
            reversed_knife = false;
            break;
        case LUP:
            knife_sprite = &knife_dup;
            reversed_knife = true;  
            break;
        case RDOWN:
            knife_sprite = &knife_ddown;
            reversed_knife = true;           
            break;
        case LEFT:
            knife_sprite = &knife_right;
            reversed_knife = true;
            break;
        default:
            return;
        }
        (*knife_sprite)->speed = 12;
        (*knife_sprite)->dir = player_vision;
        (*knife_sprite)->x_pos = knife_x;
        (*knife_sprite)->y_pos = knife_y;
}


bool is_near_bomb(int i) {
    int dist_x = 0;
    int dist_y = 0;
    dist_x = abs(((*player)->x_pos) - explosion[i][0]->x_pos);
    dist_y = abs(((*player)->y_pos) - explosion[i][0]->y_pos);  

    if(dist_x < 100 && dist_y < 70) {
        return true;
    }
    return false;
}
