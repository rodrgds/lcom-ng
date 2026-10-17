#include <lcom/lcf.h>
#include <math.h>

#include "bullet.h"
#include "game/menu.h"
#include "view/canvas.h"
#include "devices/video.h"
#include "xpm/xpm_sprites.h"
#include "xpm/xpm_id.h"

extern GameState game_state;

uint32_t n_player_bullets=0;
uint32_t n_heli_bullets=0;

Bullet player_bullets[BULLETS];
Bullet heli_bullets[BULLETS];

void (initialize_bullets)(){
    Bullet bullet = {0,0,0,0,false};
    for(int i = 0 ; i<BULLETS ; i++){
        player_bullets[i]=bullet;
        heli_bullets[i]=bullet;
    }
}

int (draw_bullet_player)(Bullet *bullet){
    if(bullet->in_game){
        return vg_draw_xpm(SOLDIER_BULLET_XPM_ID, bullet->x, bullet->y);
    }
    return 0;
}

int (draw_bullet_heli)(Bullet *bullet){
    if(bullet->in_game){
        return vg_draw_xpm(HELI_BULLET_XPM_ID, bullet->x, bullet->y);
    }
    return 0;
}

void player_update_bullets(Helicopter * heli){
    
    for(unsigned i = 0;i<BULLETS;i++){
        if(!player_bullets[i].in_game)
            continue;
        player_bullets[i].x += player_bullets[i].vx;
        player_bullets[i].y += player_bullets[i].vy;
        if(player_bullets[i].y<=BLTS_HEIGHT || player_bullets[i].y >= Y_RESOLUTION-BLTS_HEIGHT || player_bullets[i].x <= BLTS_HEIGHT || player_bullets[i].x >= X_RESOLUTION-BLTS_WIDTH){
            player_bullets[i].in_game=false;
            return;
    }
    draw_bullet_player(&player_bullets[i]);
    verify_heli_collision(heli, &player_bullets[i]);
    }
}

void player_create_bullet( Player* obj,struct packet *pp, Mouse *mouse){

    if(n_player_bullets == BULLETS){ 
      
        return;}
        
    int delta_x = mouse->x - obj->x;
    int delta_y = mouse->y - obj->y;
    double alpha = atan2((double)delta_y, (double)delta_x);
    player_bullets[n_player_bullets].x = obj->x;
    player_bullets[n_player_bullets].y = obj->y;
    player_bullets[n_player_bullets].in_game = true;
    player_bullets[n_player_bullets].vx = (int16_t)round(cos(alpha) * 5.0);
    player_bullets[n_player_bullets].vy = (int16_t)round(sin(alpha) * 5.0);  

    
    n_player_bullets++;
}

void heli_create_bullet( Helicopter* heli, Player* target){

    if(!heli->alive)
        return;

    if(n_heli_bullets == BULLETS)
        n_heli_bullets=0;

    int delta_x = target->x - (heli->x + HELI_WIDTH/2-10);
    int delta_y = target->y - (heli->y + HELI_HEIGHT);
    double alpha = atan2((double)delta_y, (double)delta_x);
    heli_bullets[n_heli_bullets].x = heli->x + HELI_WIDTH/2-10;
    heli_bullets[n_heli_bullets].y = heli->y + HELI_HEIGHT;
    heli_bullets[n_heli_bullets].in_game = true;
    heli_bullets[n_heli_bullets].vx = (int16_t)round(cos(alpha) * 5.0 * heli->vx);
    heli_bullets[n_heli_bullets].vy = (int16_t)round(sin(alpha) * 5.0 * heli->vy);  

    n_heli_bullets++;
}
 
 

void heli_update_bullets(Player * player){
    for(unsigned i = 0;i<BULLETS;i++){
        if(!heli_bullets[i].in_game)
            continue;
        heli_bullets[i].x += heli_bullets[i].vx;
        heli_bullets[i].y += heli_bullets[i].vy;
        if(heli_bullets[i].y<=BLTS_HEIGHT || heli_bullets[i].y >= Y_RESOLUTION-BLTS_HEIGHT || heli_bullets[i].x <= BLTS_WIDTH || heli_bullets[i].x >= X_RESOLUTION-BLTS_WIDTH){
            heli_bullets[i].in_game=false;
            return;
        }
        draw_bullet_heli(&heli_bullets[i]);
        verify_player_collision(player, &heli_bullets[i]);
    }
}

void verify_player_collision (Player * player, Bullet* bullet){

    if( bullet->x + BLTS_WIDTH >= player->x && bullet->x <= player->x + PLAYER_WIDTH && bullet->y + BLTS_HEIGHT >= player->y && bullet->y <= player->y + BLTS_WIDTH){
        bullet->in_game = false;
        player->hp -= HELI_BLTS_DMG;
    }
    if(player->hp == 0){
        game_state = GAMEOVER;
    }

    
}

void verify_heli_collision (Helicopter * heli, Bullet* bullet){

    if( bullet->x + BLTS_WIDTH >= heli->x && bullet->x <= heli->x + HELI_WIDTH && bullet->y + BLTS_HEIGHT >= heli->y && bullet->y <= heli->y + HELI_HEIGHT-70){
        bullet->in_game = false;
        heli->hp -= PLAYER_BLTS_DMG;
    }
    if(heli->hp == 0){
        heli->alive = false;
    }
}

void draw_c_bullets(){
    
    for(int i = 0; i < BULLETS; i++){

        draw_bullet_player(&player_bullets[i]);
        draw_bullet_heli(&heli_bullets[i]);

    }}

void draw_remaining_bullets(uint8_t bullets){

    uint16_t space = BLTS_WIDTH + 10;

        for(int i = 0; i < bullets; i++){
            vg_draw_xpm(SOLDIER_BULLET_XPM_ID,(BLTS_DISPLAY_X + i*space),BLTS_DISPLAY_Y);
        }

}




