#include "menu.h"
#include "view/canvas.h"
#include "models/bullet.h"
extern Player player;
extern Helicopter heli;
extern Bullet player_bullets[BULLETS];
extern Bullet heli_bullets[BULLETS];



extern GameState game_state; 

void restore_game_def(){

  //player
  player.x = PLAYER_INI_X; //subs pelos valores reais
  player.y = PLAYER_INI_Y;
  player.frame = 0;
  player.hp = PLAYER_HP;

  heli.x = HELI_INI_X;
  heli.y = HELI_INI_Y;
  heli.hp = HELI_HP;
  heli.vx = 1;
  heli.vy = 1;

  for(int i = 0; i< BULLETS; i++){
    heli_bullets[i].in_game=false;
    player_bullets[i].in_game=false;
  }
}

void menu_mouse(Mouse *mouse, struct packet *pp){
  switch (game_state)
  {
  case MAINMENU:
    main_menu_mouse(mouse, pp);
    break;

  case INSTRUCTIONS:
    instructions_mouse(mouse, pp);
    break;

  case GAMEOVER:
    game_over_mouse(mouse, pp);
    break;

  default:
    break;
  }
}


void main_menu_mouse(Mouse *mouse, struct packet *pp){
  if(pp->lb){
    
      if(mouse->x >= 500 && mouse->x <=700 && mouse->y >= 400 && mouse->y <= 494){
        game_state = GAME;
      }
      else if(mouse->x >= 400 && mouse->x <=800 && mouse->y >= 600 && mouse->y <= 680){
        game_state =  INSTRUCTIONS; 
      }

    }
}

void instructions_mouse(Mouse *mouse, struct packet *pp){
  if(pp->lb){
    if(mouse->x >= 450 && mouse->x <=650 && mouse->y >= 900 && mouse->y <= 1000){
          game_state =  MAINMENU; 
        }
  }
}

void (game_over_mouse)(Mouse *mouse, struct packet *pp){
  if(pp->lb){
    if(mouse->x >= 400 && mouse->x <=750 && mouse->y >= 600 && mouse->y <= 700){
          game_state =  MAINMENU; 
          restore_game_def(); // bullets, player , helicopter
        }
  }
}


