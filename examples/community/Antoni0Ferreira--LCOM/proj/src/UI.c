#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>


#include "UI.h"


extern int counter_boost;
extern int draw_b;
extern int player_boosted;


int boost_time = 0;


int (initialize_game_sprites)(struct astro astros[], struct shot shots[], struct shot_animation shot_animations[], struct player *player, struct boost *boost){
	

    initialize_player(player);
	
    initialize_points();

    initialize_shots(shots, shot_animations);

    initialize_astros(astros);

    initialize_boost(boost);


    if(vg_draw_sprite(player->sprite,player->x,player->y,player->img) != 0) return 1;

    for(int i = 0; i < 8; i++){
        if(vg_draw_sprite(astros[i].sprite,astros[i].x,astros[i].y,astros[i].img) != 0) return 1;
    }

    copy_buffer();

	return 0;
	
}

uint8_t* (initialize_game_background)(xpm_image_t *imgBG){
    uint8_t minutes;
    uint8_t aux;
    get_mins(&minutes);
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_C);
    util_sys_inb(RTC_DATA_REG, &aux);
    uint8_t* bg;

    if((minutes % 2) == 0){
        xpm_map_t mapBG = background_xpm;
        bg = xpm_load(mapBG,XPM_8_8_8,imgBG);
    }
    else{
        xpm_map_t mapBG = day_bg_image;
        bg = xpm_load(mapBG,XPM_8_8_8,imgBG);     
    }
    return bg; 
}

int(vg_draw_frame)(struct astro astros[], struct shot shots[], struct shot_animation shot_animations[], struct player *player,struct boost *boost, struct keyboard_state *state, struct cursor *cursor, uint8_t *bg, int *points){

    uint8_t boost_value = 0;
                    
    if(player_boosted){
        boost_value = 3;
        boost_time++;
        
        if(boost_time == 2 * 60){
            player_boosted = 0;
            boost_value = 0;
            boost_time = 0;
        }
    } 
    move_astros(astros);
    draw_shots(shots, shot_animations, player, bg);
    draw_player(player,state,boost_value, points);

    draw_points(*points);

    draw_astros(astros, shots, shot_animations, player, bg);

    capture_boost(boost,cursor,bg);

    copy_buffer();

    return 0;

}

uint8_t* (initialize_menu_background)(xpm_image_t *imgmenuBG){

    xpm_map_t mapmenuBG = menu_background_xpm;
    uint8_t *bg = xpm_load(mapmenuBG,XPM_8_8_8,imgmenuBG);
    vg_draw_sprite(bg,0,0,*imgmenuBG);
    return bg;
}

int (menu_option_sprite)(int option, uint8_t *replacementbg){
    uint16_t x1st = 220,y1st = 290,x2nd = 245,y2nd = 440;

    xpm_image_t imgShip;
    xpm_map_t imgAsteroidmap = ship_xpm;
    uint8_t *ship_sprite = xpm_load(imgAsteroidmap,XPM_8_8_8,&imgShip);

    //Option 0: Play; 1: Quit
    switch (option)
    {
    case 0:
        if(vg_draw_sprite(ship_sprite,x1st,y1st,imgShip) != 0) return 1;
        vg_destroy_sprite(x2nd, y2nd, imgShip, replacementbg);
        break;
    case 1:
        if(vg_draw_sprite(ship_sprite,x2nd,y2nd,imgShip) != 0) return 1;
        vg_destroy_sprite(x1st, y1st, imgShip, replacementbg);
        break;
    
    default:
        break;
    }
    return 0;
}

void (state_scancode)(uint8_t scanCode, struct keyboard_state *state){

    switch (scanCode) {
    case W_BREAK_CODE:
        state->W_DOWN = false;
        break;

    case S_BREAK_CODE:
        state->S_DOWN = false;
        break;

    case A_BREAK_CODE:
        state->A_DOWN = false;
        break;

    case D_BREAK_CODE:
        state->D_DOWN = false;
        break;

    case W_MAKE_CODE:
        state->W_DOWN = true;
        break;

    case S_MAKE_CODE:
        state->S_DOWN = true;
        break;

    case A_MAKE_CODE:
        state->A_DOWN = true;
        break;

    case D_MAKE_CODE:
        state->D_DOWN = true;
        break;

    default:
        break;
    }
}

void (draw_scores)(int points, int highscore){
    
    int auxP = points;
    uint16_t x = (uint16_t)170, y = (uint16_t)350;
    int mod = 0;
    do{
        mod = auxP % 10;
        vg_draw_sprite(maps[mod], x, y, imgs[mod]);
        auxP /= 10;
        x -= 40;
    }while(auxP != 0);


    int auxH = highscore;
    x = (uint16_t)550; y = (uint16_t)350;

    do{
        mod = auxH % 10;
        vg_draw_sprite(maps[mod], x, y, imgs[mod]);
        auxH /= 10;
        x -= 40;
    }while(auxH != 0);
    return;

}

void (capture_boost)(struct boost *boost, struct cursor *cursor, uint8_t *bg){
    if(draw_b){
        draw_boost(boost,bg);
        if(cursor->RBpressed){
            if(cursor->x + cursor->img.width / 2 >= boost->x && cursor->x + cursor->img.width / 2 <= boost->x + boost->img.width  &&
                cursor->y + cursor->img.height / 2 >= boost->y && cursor->y + cursor->img.height / 2 <= boost->y + boost->img.height){
                vg_destroy_sprite(boost->x, boost->y, boost->img, bg);
                draw_b = 0;
                player_boosted = 1;
                
            }
        }
        
    }
    vg_draw_sprite(cursor->sprite, cursor->x, cursor->y, cursor->img);
}

void (draw_astros)(struct astro astros[], struct shot shots[], struct shot_animation shot_animations[], struct player *player, uint8_t *bg){
    for (int i = 0; i < 8; i++) {
        int option = 0;
        if ((option = vg_draw_astro(astros[i].sprite, astros[i].x, astros[i].y, astros[i].img, bg)) != 0) {

            vg_destroy_sprite(astros[i].x, astros[i].y, astros[i].img, bg);
            bool colided = false;
            if (option == 1) {
                vg_destroy_sprite(player->x, player->y, player->img, bg);
                player->x = X_INITIAL_PLAYER;
                player->y = Y_INITIAL_PLAYER;
                vg_draw_sprite(player->sprite, player->x, player->y, player->img);
            }
            else{
                for(int j = 0; j < 20; j++){
                    if(shots[j].x + shot_animations[6].img.width >= astros[i].x &&
                        shots[j].x - shot_animations[6].img.width <= astros[i].x &&
                        shots[j].y + shot_animations[6].img.height >= astros[i].y &&
                        shots[j].y - shot_animations[6].img.height <= astros[i].y && !colided){

                            vg_destroy_sprite(shots[j].x, shots[j].y, shot_animations[6].img, bg);
                            shots[j].animation += 1;
                            vg_draw_sprite(shot_animations[7].sprite, shots[i].x, shots[i].y, shot_animations[7].img);
                            colided = true;
                        }
                }
                if(i % 2 == 0) astros[i].x = 0;
                else astros[i].x = RIGHT_BORDER_LIMIT - astros[i].img.width;
            }

            vg_draw_sprite(astros[i].sprite, astros[i].x, astros[i].y, astros[i].img);
        
        
        }
    }
}

void (draw_shots)(struct shot shots[], struct shot_animation shot_animations[], struct player *player, uint8_t *bg){

    for (int i = 0; i < 20; i++) {
        int k = shots[i].animation;
        if (k == -1){
            continue;
        }
            
        vg_destroy_sprite(shots[i].x, shots[i].y, shot_animations[k].img, bg);
        if (k < 6) {
            shots[i].animation += 1;
            shots[i].x = player->x + 2;
            shots[i].y = player->y - 50;
            vg_draw_sprite(shot_animations[k + 1].sprite, shots[i].x, shots[i].y, shot_animations[k + 1].img);
        }
        else if (k > 6 || (shots[i].y - 15 < 0 && k == 6)) {
            shots[i].animation += 1;
            vg_draw_sprite(shot_animations[k + 1].sprite, shots[i].x, shots[i].y, shot_animations[k + 1].img);
        }
        else {
            shots[i].y -= 15;
            vg_draw_sprite(shot_animations[6].sprite, shots[i].x, shots[i].y, shot_animations[6].img);
        }

        if (shots[i].animation == 11) {
            vg_destroy_sprite(shots[i].x, shots[i].y, shot_animations[k + 1].img, bg);
            shots[i].x = -100;
            shots[i].y = -100;
            shots[i].animation = -1;
        }
    }
}

