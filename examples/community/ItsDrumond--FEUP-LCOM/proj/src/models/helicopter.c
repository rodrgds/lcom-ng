#include <lcom/lcf.h>

#include "helicopter.h"
#include "bullet.h"
#include "devices/video.h"
#include "devices/i8042.h"

#include "xpm/xpm_id.h"

bool isRight = true;
bool isGoingUp = true;
uint32_t explosion = 1;
uint32_t heli_shoot_time = 60 * 2;


void update_heli_move(Helicopter* heli) {
  if(!heli->alive){
    destroy_helicopter(heli);
    return;
  }
  if (isGoingUp) {
    heli->y++;
  } else {
    heli->y--;
  }

  if (heli->y >= 120) {
    isGoingUp = false;
  } else if (heli->y <= 100) {
    isGoingUp = true;
  }

  if (heli->x < 100 && !isRight) {
    heli->x+= heli->vx;
  } else if (heli->x > 900 && isRight) {
    heli->x-=heli->vy;
  } else {
    if (isRight) {
      heli->x+= heli->vx;
    } else {
      heli->x-=heli->vy;
    }
  }

  if (heli->x >= 900) {
    isRight = false;
  } else if (heli->x <= 100) {
    isRight = true;
  }
}

void helicopter_shoot(Helicopter* heli ,Player* player){
  heli_create_bullet(heli,player);
}

void draw_helicopter(Helicopter* heli){
  
  if(!heli->alive){
    if(!draw_explosions(heli)){ 
    vg_draw_xpm(HELI_DEAD_XPM_ID, heli->x, heli->y);
    }
  }
  else{
    vg_draw_xpm(HELI_XPM_ID, heli->x, heli->y);
  }

}

void init_helicopter(Helicopter * heli){
  heli->x = HELI_INI_X;
  heli->y = HELI_INI_Y;
  heli->hp = HELI_HP;
  heli->alive = true;
}


void helicopter_add_difficulty(Helicopter * heli){
  heli->vx++;
  heli->vy++;
  if(heli_shoot_time > 30){
    heli_shoot_time -= 20;
  }
}

int  draw_explosions(Helicopter * heli){
 
     explosion == 0 ? true:explosion++;
  
    if(explosion >= 2 && explosion <= 22){
      vg_draw_xpm(HELI_XPM_ID, heli->x, heli->y);
      vg_draw_xpm(EXPLOSION1_XPM_ID,heli->x ,heli->y);
      return 1;

    }
    else if(explosion > 22 && explosion <= 44)
    {
      vg_draw_xpm(HELI_XPM_ID, heli->x, heli->y);
      vg_draw_xpm(EXPLOSION2_XPM_ID,heli->x ,heli->y );
      return 1; 
    }
    else if(explosion < 66 && explosion > 44){
      vg_draw_xpm(EXPLOSION3_XPM_ID,heli->x ,heli->y);}

    else explosion = 0;

    

    return 0;


  

}


void destroy_helicopter(Helicopter * heli){

  if(explosion) return;
  
  if(heli->y < Y_RESOLUTION){
    heli->y+=5;
  }
  else{
    
    explosion = 1;
    init_helicopter(heli);
  }
}
