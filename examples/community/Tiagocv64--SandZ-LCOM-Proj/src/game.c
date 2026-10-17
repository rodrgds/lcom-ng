#include <lcom/lcf.h>
#include "game.h"
#include "libs/serial_port.h"
#include "libs/video.h"
#include "UI_game.h"
#include "text.h"
#include "obstacle.h"
#include "player.h"
#include "zombie.h"
#include "bullet.h"
#include "blood.h"
#include "enum.h"
#include "life.h"
#include "program.h"


extern uint8_t data_mouse;
extern uint8_t data_kb;
extern bool wasRead_kb;
extern unsigned long long counter;
extern bool two_player;
extern enum program_state p_state;

uint32_t playTime_s;
char player_name[50] = {0};
int player_name_size = 0;
int points_game = 0;
int points_adv = 0;


void init_game(){
  initUI();
  init_text();
  init_obstacles();
  init_weapons();
  init_player();
  init_bullets();
  init_zombies();
  init_blood();
  init_text();
  init_lives();
  playTime_s = 0;
  points_adv = 0;
}

void destroy_game(){
  destroyUI();
  destroy_text();
  destroy_obstacles();
  destroy_player();
  destroy_bullets();
  destroy_zombies();
  destroy_blood();
  destroy_text();
  destroy_lives();
}

void update_game(){
  if (counter % 600 == 0 && counter != 0) //spawn a pickupable life every 5 seconds.
   create_life();
  if (counter % 60 == 0) { //give points to player every second.
    update_points();
    playTime_s++;
    if (two_player){
      char tmp_p[25];
      uint32_t text_size = sprintf(tmp_p, "p%dP", points_game);
      uart_send_message(tmp_p, text_size);
    }
  }

  update_player();
  update_bullets();
  update_zombies();
  update_lives();
  update_blood();

  scheduleTimer(playTime_s);

  draw_background();
  draw_blood();
  draw_lives();
  draw_obstacles();
  draw_zombies_hp();
  draw_zombies();
  draw_player();
  draw_bullets();
  drawUI();

  if (p_state != PLAYING && p_state != PAUSE_GAME )
    destroy_game();

  flip_buff();
}

void pause_game(){
  pause_player();
}

void resume_game(){
  resume_player();
}
