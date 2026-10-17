#include <lcom/lcf.h>

#include "sprite.h"

Sprite *menu_start_selected;
Sprite *menu_settings_selected;
Sprite *menu_instructions_selected;
Sprite *menu_exit_selected;
Sprite *menu_none_selected;
Sprite *leaderboard;
Sprite *leaderboard_selected;
Sprite *leaderboard_background;

Sprite *settings_resolution_480_selected;
Sprite *settings_resolution_600_selected;
Sprite *settings_resolution_768_selected;
Sprite *settings_resolution_864_selected;
Sprite *settings_resolution_1024_selected;
Sprite *settings_clear_selected;
Sprite *settings_back_selected;

Sprite *instructions;


Sprite *gameover_exit_selected;
Sprite *gameover_tryagain_selected;

Sprite *pause_resume_selected;
Sprite *pause_exit_selected;

Sprite *player;
Sprite *background;
Sprite *laser;
Sprite *cannon_up;
Sprite *cannon_right;
Sprite *cannon_down;
Sprite *cannon_left;
Sprite *cannon_diag1;
Sprite *cannon_diag2;
Sprite *cannon_diag3;
Sprite *cannon_diag4;
Sprite *asteroid;
Sprite *cursor;

Sprite *letters[26];
Sprite *numbers[10];
Sprite *special_chars[3];

Sprite *coin;
Sprite *bomb;

Sprite *create_sprite(xpm_map_t pic) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    // read the sprite pixmap
    sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);
    if(sp->map == NULL) {
        printf("create_sprite: xpm_load failed\n");
        free(sp);
        return NULL;
    }
    sp->width = img.width;
    sp->height = img.height;

    sp->x = 0;
    sp->y = 0;

    sp->xspeed = 0;
    sp->yspeed = 0;

    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
    if( sp ->map ) free(sp->map);
    free(sp);
    sp = NULL;
}

int draw_sprite(Sprite *sp, int x, int y) {
    if (sp == NULL) {
        printf("draw_sprite: sp is NULL\n");
        return 1;
    }
    for (int i = 0; i < sp->height; i++) {
        for (int j = 0; j < sp->width; j++) {
            if (vg_draw_pixel(x + j, y + i, sp->map[i * sp->width + j]) != 0) return 1;
        }
    }
    return 0;
}

void load_xpms() {
    // Menu
    menu_start_selected = create_sprite((xpm_map_t) menu_start_selected_xpm);
    if (menu_start_selected == NULL) printf("menu_start_selected is NULL\n");

    menu_settings_selected = create_sprite((xpm_map_t) menu_settings_selected_xpm);
    if (menu_settings_selected == NULL) printf("menu_settings_selected is NULL\n");

    menu_instructions_selected = create_sprite((xpm_map_t) menu_instructions_selected_xpm);
    if (menu_instructions_selected == NULL) printf("menu_instructions_selected is NULL\n");

    menu_exit_selected = create_sprite((xpm_map_t) menu_exit_selected_xpm);
    if (menu_exit_selected == NULL) printf("menu_exit_selected is NULL\n");

    menu_none_selected = create_sprite((xpm_map_t) menu_none_selected_xpm);
    if (menu_none_selected == NULL) printf("menu_none_selected is NULL\n");

    // Settings
    settings_resolution_480_selected = create_sprite((xpm_map_t) settings_resolution_480_selected_xpm);
    if (settings_resolution_480_selected == NULL) printf("settings_resolution_480_selected is NULL\n");

    settings_resolution_600_selected = create_sprite((xpm_map_t) settings_resolution_600_selected_xpm);
    if (settings_resolution_600_selected == NULL) printf("settings_resolution_600_selected is NULL\n");

    settings_resolution_768_selected = create_sprite((xpm_map_t) settings_resolution_768_selected_xpm);
    if (settings_resolution_768_selected == NULL) printf("settings_resolution_768_selected is NULL\n");

    settings_resolution_864_selected = create_sprite((xpm_map_t) settings_resolution_864_selected_xpm);
    if (settings_resolution_864_selected == NULL) printf("settings_resolution_864_selected is NULL\n");

    settings_resolution_1024_selected = create_sprite((xpm_map_t) settings_resolution_1024_selected_xpm);
    if (settings_resolution_1024_selected == NULL) printf("settings_resolution_1024_selected is NULL\n");

    settings_clear_selected = create_sprite((xpm_map_t) settings_clear_selected_xpm);
    if (settings_clear_selected == NULL) printf("settings_clear_selected is NULL\n");

    settings_back_selected = create_sprite((xpm_map_t) settings_back_selected_xpm);
    if (settings_back_selected == NULL) printf("settings_back_selected is NULL\n");

    // Instructions
    instructions = create_sprite((xpm_map_t) instructions_xpm);
    if (instructions == NULL) printf("instructions is NULL\n");

    // Leaderboard
    leaderboard = create_sprite((xpm_map_t) leaderboard_xpm);
    if (leaderboard == NULL) printf("leaderboard is NULL\n");

    leaderboard_selected = create_sprite((xpm_map_t) leaderboard_selected_xpm);
    if (leaderboard_selected == NULL) printf("leaderboard_selected is NULL\n");

    leaderboard_background = create_sprite((xpm_map_t) leaderboard_background_xpm);
    if (leaderboard_background == NULL) printf("leaderboard_background is NULL\n");

    // Game Over
    gameover_tryagain_selected = create_sprite((xpm_map_t) gameover_tryagain_selected_xpm);
    if (gameover_tryagain_selected == NULL) printf("gameover_tryagain_selected is NULL\n");

    gameover_exit_selected = create_sprite((xpm_map_t) gameover_exit_selected_xpm);
    if (gameover_exit_selected == NULL) printf("gameover_exit_selected is NULL\n");

    // Pause
    pause_resume_selected = create_sprite((xpm_map_t) pause_resume_selected_xpm);
    if (pause_resume_selected == NULL) printf("pause_resume_selected is NULL\n");

    pause_exit_selected = create_sprite((xpm_map_t) pause_exit_selected_xpm);
    if (pause_exit_selected == NULL) printf("pause_exit_selected is NULL\n");

    // Game
    player = create_sprite((xpm_map_t) player_xpm);
    if (player == NULL) printf("player is NULL\n");
    player->x = (mode_info.XResolution / 2) - (player->width / 2);
    player->y = (mode_info.YResolution / 2) - (player->height / 2);
    
    background = create_sprite((xpm_map_t) background_xpm);
    if (background == NULL) printf("background is NULL\n");

    laser = create_sprite((xpm_map_t) laser_xpm);
    if (laser == NULL) printf("laser is NULL\n");

    cannon_up = create_sprite((xpm_map_t) cannon_up_xpm);
    if (cannon_up == NULL) printf("cannon_up is NULL\n");

    cannon_right = create_sprite((xpm_map_t) cannon_right_xpm);
    if (cannon_right == NULL) printf("cannon_right is NULL\n");

    cannon_down = create_sprite((xpm_map_t) cannon_down_xpm);
    if (cannon_down == NULL) printf("cannon_down is NULL\n");

    cannon_left = create_sprite((xpm_map_t) cannon_left_xpm);
    if (cannon_left == NULL) printf("cannon_left is NULL\n");

    cannon_diag1 = create_sprite((xpm_map_t) cannon_diag1_xpm);
    if (cannon_diag1 == NULL) printf("cannon_diag1 is NULL\n");

    cannon_diag2 = create_sprite((xpm_map_t) cannon_diag2_xpm);
    if (cannon_diag2 == NULL) printf("cannon_diag2 is NULL\n");

    cannon_diag3 = create_sprite((xpm_map_t) cannon_diag3_xpm);
    if (cannon_diag3 == NULL) printf("cannon_diag3 is NULL\n");

    cannon_diag4 = create_sprite((xpm_map_t) cannon_diag4_xpm);
    if (cannon_diag4 == NULL) printf("cannon_diag4 is NULL\n");

    asteroid = create_sprite((xpm_map_t) asteroid_xpm);
    if (asteroid == NULL) printf("asteroid is NULL\n");

    cursor = create_sprite((xpm_map_t) cursor_xpm);
    if (cursor == NULL) printf("cursor is NULL\n");
    cursor->x = 10;
    cursor->y = 10;

    // Numbers
    numbers[0] = create_sprite((xpm_map_t) char_0_xpm);
    if (numbers[0] == NULL) printf("numbers[0] is NULL\n");
    numbers[1] = create_sprite((xpm_map_t) char_1_xpm);
    if (numbers[1] == NULL) printf("numbers[1] is NULL\n");
    numbers[2] = create_sprite((xpm_map_t) char_2_xpm);
    if (numbers[2] == NULL) printf("numbers[2] is NULL\n");
    numbers[3] = create_sprite((xpm_map_t) char_3_xpm);
    if (numbers[3] == NULL) printf("numbers[3] is NULL\n");
    numbers[4] = create_sprite((xpm_map_t) char_4_xpm);
    if (numbers[4] == NULL) printf("numbers[4] is NULL\n");
    numbers[5] = create_sprite((xpm_map_t) char_5_xpm);
    if (numbers[5] == NULL) printf("numbers[5] is NULL\n");
    numbers[6] = create_sprite((xpm_map_t) char_6_xpm);
    if (numbers[6] == NULL) printf("numbers[6] is NULL\n");
    numbers[7] = create_sprite((xpm_map_t) char_7_xpm);
    if (numbers[7] == NULL) printf("numbers[7] is NULL\n");
    numbers[8] = create_sprite((xpm_map_t) char_8_xpm);
    if (numbers[8] == NULL) printf("numbers[8] is NULL\n");
    numbers[9] = create_sprite((xpm_map_t) char_9_xpm);
    if (numbers[9] == NULL) printf("numbers[9] is NULL\n");

    // Letters
    letters[0] = create_sprite((xpm_map_t) char_a_xpm);
    if (letters[0] == NULL) printf("letters[0] is NULL\n");
    letters[1] = create_sprite((xpm_map_t) char_b_xpm);
    if (letters[1] == NULL) printf("letters[1] is NULL\n");
    letters[2] = create_sprite((xpm_map_t) char_c_xpm);
    if (letters[2] == NULL) printf("letters[2] is NULL\n");
    letters[3] = create_sprite((xpm_map_t) char_d_xpm);
    if (letters[3] == NULL) printf("letters[3] is NULL\n");
    letters[4] = create_sprite((xpm_map_t) char_e_xpm);
    if (letters[4] == NULL) printf("letters[4] is NULL\n");
    letters[5] = create_sprite((xpm_map_t) char_f_xpm);
    if (letters[5] == NULL) printf("letters[5] is NULL\n");
    letters[6] = create_sprite((xpm_map_t) char_g_xpm);
    if (letters[6] == NULL) printf("letters[6] is NULL\n");
    letters[7] = create_sprite((xpm_map_t) char_h_xpm);
    if (letters[7] == NULL) printf("letters[7] is NULL\n");
    letters[8] = create_sprite((xpm_map_t) char_i_xpm);
    if (letters[8] == NULL) printf("letters[8] is NULL\n");
    letters[9] = create_sprite((xpm_map_t) char_j_xpm);
    if (letters[9] == NULL) printf("letters[9] is NULL\n");
    letters[10] = create_sprite((xpm_map_t) char_k_xpm);
    if (letters[10] == NULL) printf("letters[10] is NULL\n");
    letters[11] = create_sprite((xpm_map_t) char_l_xpm);
    if (letters[11] == NULL) printf("letters[11] is NULL\n");
    letters[12] = create_sprite((xpm_map_t) char_m_xpm);
    if (letters[12] == NULL) printf("letters[12] is NULL\n");
    letters[13] = create_sprite((xpm_map_t) char_n_xpm);
    if (letters[13] == NULL) printf("letters[13] is NULL\n");
    letters[14] = create_sprite((xpm_map_t) char_o_xpm);
    if (letters[14] == NULL) printf("letters[14] is NULL\n");
    letters[15] = create_sprite((xpm_map_t) char_p_xpm);
    if (letters[15] == NULL) printf("letters[15] is NULL\n");
    letters[16] = create_sprite((xpm_map_t) char_q_xpm);
    if (letters[16] == NULL) printf("letters[16] is NULL\n");
    letters[17] = create_sprite((xpm_map_t) char_r_xpm);
    if (letters[17] == NULL) printf("letters[17] is NULL\n");
    letters[18] = create_sprite((xpm_map_t) char_s_xpm);
    if (letters[18] == NULL) printf("letters[18] is NULL\n");
    letters[19] = create_sprite((xpm_map_t) char_t_xpm);
    if (letters[19] == NULL) printf("letters[19] is NULL\n");
    letters[20] = create_sprite((xpm_map_t) char_u_xpm);
    if (letters[20] == NULL) printf("letters[20] is NULL\n");
    letters[21] = create_sprite((xpm_map_t) char_v_xpm);
    if (letters[21] == NULL) printf("letters[21] is NULL\n");
    letters[22] = create_sprite((xpm_map_t) char_w_xpm);
    if (letters[22] == NULL) printf("letters[22] is NULL\n");
    letters[23] = create_sprite((xpm_map_t) char_x_xpm);
    if (letters[23] == NULL) printf("letters[23] is NULL\n");
    letters[24] = create_sprite((xpm_map_t) char_y_xpm);
    if (letters[24] == NULL) printf("letters[24] is NULL\n");
    letters[25] = create_sprite((xpm_map_t) char_z_xpm);
    if (letters[25] == NULL) printf("letters[25] is NULL\n");

    // Special Characters
    special_chars[0] = create_sprite((xpm_map_t) char_dash_xpm);
    if (special_chars[0] == NULL) printf("special_chars[0] is NULL\n");

    special_chars[1] = create_sprite((xpm_map_t) char_doubledot_xpm);
    if (special_chars[1] == NULL) printf("special_chars[1] is NULL\n");

    // Powerups
    coin = create_sprite((xpm_map_t) coin_xpm);
    if (coin == NULL) printf("coin is NULL\n");

    bomb = create_sprite((xpm_map_t) bomb_xpm);
    if (bomb == NULL) printf("bomb is NULL\n");
}

void destroy_xpms() {
    destroy_sprite(menu_start_selected);
    destroy_sprite(menu_settings_selected);
    destroy_sprite(menu_instructions_selected);
    destroy_sprite(menu_exit_selected);
    destroy_sprite(settings_resolution_480_selected);
    destroy_sprite(settings_resolution_600_selected);
    destroy_sprite(settings_resolution_768_selected);
    destroy_sprite(settings_resolution_864_selected);
    destroy_sprite(settings_resolution_1024_selected);
    destroy_sprite(settings_clear_selected);
    destroy_sprite(settings_back_selected);
    destroy_sprite(gameover_exit_selected);
    destroy_sprite(gameover_tryagain_selected);
    destroy_sprite(player);
    destroy_sprite(background);
    destroy_sprite(laser);
    destroy_sprite(cannon_up);
    destroy_sprite(cannon_right);
    destroy_sprite(cannon_down);
    destroy_sprite(cannon_left);
    destroy_sprite(cannon_diag1);
    destroy_sprite(cannon_diag2);
    destroy_sprite(cannon_diag3);
    destroy_sprite(cannon_diag4);
    destroy_sprite(asteroid);
    destroy_sprite(cursor);

    for (int i = 0; i < 10; i++) {
        destroy_sprite(numbers[i]);
    }
    for (int i = 0; i < 26; i++) {
        destroy_sprite(letters[i]);
    }
    for (int i = 0; i < 3; i++) {
        destroy_sprite(special_chars[i]);
    }
}
