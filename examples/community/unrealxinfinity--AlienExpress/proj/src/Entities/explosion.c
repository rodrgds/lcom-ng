#include "explosion.h"

void set_explosion(int x, int y){

    for(int i =0 ; i<((int)(sizeof(explosions)/sizeof(explosion)));i++ ){
        if (explosions[i].is_dead){
            explosions[i].is_dead = false;
            explosions[i].hitbox_x = x;
            explosions[i].hitbox_y = y;
            explosions[i].idle_time=2;
            explosions[i].no_img=0;
            center_explosion(&explosions[i]);
            break;
        }
    }

}

void center_explosion(img_t *expl){
    expl->x = expl->hitbox_x - explosion_effect.animation[expl->no_img%8].width/2;
    expl->y = expl->hitbox_y - explosion_effect.animation[expl->no_img%8].height/2;
}

void draw_explosion(){
   for(int i =0 ; i < ((int)(sizeof(explosions)/sizeof(explosion)));i++ ){
        if (!explosions[i].is_dead){
             if(counter_timer % 3 == 0){
                explosions[i].no_img += 1;
                center_explosion(&explosions[i]);
             } 
             if(counter_timer%30==0) explosions[i].idle_time-=1;
             if(explosions[i].idle_time==0){
                 explosions[i].is_dead = true;
             }
             else if(explosions[i].no_img==7){
                explosions[i].is_dead = true;
             }
             draw(explosion_effect.animation[explosions[i].no_img%8],explosions[i]);
             
        }   
    }
}
